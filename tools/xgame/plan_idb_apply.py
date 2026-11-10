#!/usr/bin/env python3
"""Plan names, prototypes, comments and data symbols for a Raidou IDB from SMT3 and DDS.

Usage: plan_idb_apply.py raidou1|raidou2; writes apply/<game>_plan.json and _plan.log.
Name priority: raidou-decomp symbol_addrs, syscall number, SMT3 map, DDS.
"""
import bisect
import csv
import json
import os
import re
import sys
from collections import Counter, defaultdict

GAME = sys.argv[1]
D = os.path.expanduser("~/.local/state/smt3-port-work/raidou-map")
A = D + "/apply"
DDS = os.path.expanduser("~/projects/dds-decomp/config")
RAIDOU = os.path.expanduser("~/projects/raidou-decomp/config")
PAIRS = os.path.expanduser(f"~/.local/state/raidou-decomp-work/names/out/pairs_{GAME}.tsv")
DUMMY = re.compile(r"^start$|^(sub|off|dword|unk|byte|word|qword|xmmword|flt|dbl|asc|stru|jpt|def|loc|nullsub|algn"
                   r"|func|D|jtbl|j)_[0-9A-Fa-f]+(_\d+)?$")
LOG = []


def log(*a):
    LOG.append("\t".join(map(str, a)))


def funcs(name):
    return {f["ea"]: f for f in json.load(open(f"{D}/{name}.json"))}


def meta(name):
    m = json.load(open(f"{A}/{name}_meta.json"))
    return {int(k): v for k, v in m["funcs"].items()}, {int(k): v for k, v in m["data"].items()}


def same_code(a, b):
    """Equal apart from immediates and jump targets, which relocate."""
    if len(a) != len(b):
        return False
    for x, y in zip(a, b):
        op = x >> 26
        if op == 0 or op == 0x1C:
            if x != y:
                return False
        elif op in (2, 3):
            if y >> 26 != op:
                return False
        elif x >> 16 != y >> 16:
            return False
    return True


LOADSTORE = set(range(0x20, 0x40)) | {0x1A, 0x1B, 0x1E, 0x1F}


def data_refs(words, gp):
    """(word index, address) for lui/lo pairs and $gp-relative accesses, decoded linearly."""
    hi, out = {}, []
    for k, w in enumerate(words):
        op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
        simm = imm - 0x10000 if imm & 0x8000 else imm
        if op == 0x0F:
            hi[rt] = imm << 16
            continue
        if op == 0x09 or op in LOADSTORE:
            if rs == 28 and gp:
                out.append((k, (gp + simm) & 0xFFFFFFFF))
            elif rs in hi:
                out.append((k, (hi[rs] + simm) & 0xFFFFFFFF))
        if op == 0:
            hi.pop((w >> 11) & 31, None)
        elif op in (0x02, 0x03):
            hi.clear()
        elif op not in (0x2B, 0x29, 0x28, 0x3F, 0x1F, 0x2A, 0x2E, 0x39, 0x3E) and op not in range(4, 8) \
                and op not in (1, 0x14, 0x15, 0x16, 0x17):
            hi.pop(rt, None)
    return out


def gp_of(start_words):
    """crt0 passes $gp to SetupThread in $a0."""
    hi = None
    for w in start_words:
        if w >> 16 == 0x3C04:
            hi = (w & 0xFFFF) << 16
        elif w >> 16 == 0x2484 and hi is not None:
            s = w & 0xFFFF
            return (hi + (s - 0x10000 if s & 0x8000 else s)) & 0xFFFFFFFF
    return None


def syscall_no(words):
    if len(words) <= 4 and 0xC in words:
        i = words.index(0xC)
        if i and words[i - 1] >> 16 == 0x2403:
            return words[i - 1] & 0xFFFF
    return None


def symbol_addrs(path, want_func):
    out = {}
    for line in open(path):
        m = re.match(r"\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+);(.*)", line)
        if not m or ("type:func" in m.group(3)) != want_func:
            continue
        ty = re.search(r"type:(\w+)", m.group(3))
        size = re.search(r"size:(0x[0-9A-Fa-f]+|\d+)", m.group(3))
        out[int(m.group(2), 16)] = (m.group(1), ty.group(1) if ty else None, int(size.group(1), 0) if size else 0)
    return out


SPLAT_C = {"u8": ("u8", 1), "s8": ("s8", 1), "u16": ("u16", 2), "s16": ("s16", 2), "u32": ("u32", 4),
           "s32": ("s32", 4), "f32": ("f32", 4), "u64": ("u64", 8), "s64": ("s64", 8), "f64": ("f64", 8),
           "asciz": ("char", 1)}


def splat_decl(name, ty, size):
    if ty not in SPLAT_C:
        return None, size
    c, w = SPLAT_C[ty]
    if size > w and size % w == 0:
        return f"{c} {name}[{size // w}];", size
    return (f"{c} {name};", w) if size in (0, w) else (None, size)


# ---- inputs
R = funcs(GAME)
rmeta, rdata = meta(GAME)
S = funcs("smt3")
smeta, sdata = meta("smt3")
D1 = funcs("dds1")
d1meta, d1data = meta("dds1")
D2 = funcs("dds2")
gp = {"r": gp_of(R[min(R)]["words"]), "smt3": gp_of(S[min(S)]["words"]), "dds1": 0x3C0CF0, "dds2": 0x43AE70}
log("gp", hex(gp["r"]))
smt3_by_name = {v["name"]: ea for ea, v in smeta.items()}
mapped = {int(r["ea"], 16): r for r in csv.DictReader(open(f"{D}/{GAME}_map.tsv"), delimiter="\t")}
map_name = {ea: r["smt3_name"] for ea, r in mapped.items() if r["smt3_name"]}

# ---- function pairing with SMT3
pair = {ea: int(r["smt3_ea"], 16) for ea, r in mapped.items() if r["smt3_ea"]}
want, source = {}, {}
for ea, name in map_name.items():
    if not DUMMY.match(name):
        want[ea], source[ea] = name, "map"

sys_names = defaultdict(set)
for ea, f in S.items():
    n = syscall_no(f["words"])
    if n is not None:
        sys_names[n].add(smeta[ea]["name"] if ea in smeta else f["name"])
for ea, f in R.items():
    n = syscall_no(f["words"])
    names = sys_names.get(n, set())
    if n is None or len(names) != 1 or (ea in pair and pair[ea] in S and syscall_no(S[pair[ea]]["words"]) == n):
        continue
    name = next(iter(names))
    if DUMMY.match(name):
        continue
    if want.get(ea) != name:
        log("syscall", hex(ea), n, want.get(ea, ""), name)
        want[ea], source[ea] = name, "syscall"
        if name in smt3_by_name:
            pair[ea] = smt3_by_name[name]

auth = symbol_addrs(f"{RAIDOU}/{GAME}/symbol_addrs.txt", True)
for ea, (name, _, _) in auth.items():
    if name.startswith("func_") or ea not in R:
        continue
    if want.get(ea) != name:
        log("authoritative", hex(ea), want.get(ea, ""), name)
    want[ea], source[ea] = name, "symbol_addrs"
    if name in smt3_by_name:
        pair[ea] = smt3_by_name[name]
    elif ea in pair:
        del pair[ea]
for line in open(PAIRS):
    c = line.rstrip("\n").split("\t")
    if len(c) > 4 and c[4] == "reject:map-disagrees":
        log("pairs-map-disagrees", c[0], c[3], c[6])

# A name may sit at one address only; the highest-ranked claim keeps it.
RANK = {"symbol_addrs": 0, "syscall": 1, "map": 2, "dds": 3}
claims = defaultdict(list)
for ea, name in want.items():
    claims[name].append(ea)
for name, eas in claims.items():
    if len(eas) > 1:
        eas.sort(key=lambda e: RANK[source[e]])
        for e in eas[1:]:
            log("name-dup", name, hex(eas[0]), source[eas[0]], "drops", hex(e), source[e])
            del want[e]
            pair.pop(e, None)
# A map pairing whose SMT3 source a stronger claim placed elsewhere is wrong.
owner = {}
for ea, s in pair.items():
    if source.get(ea) in ("symbol_addrs", "syscall"):
        owner[s] = ea
for ea in list(pair):
    s = pair[ea]
    if owner.get(s, ea) != ea:
        log("pair-dropped", hex(ea), hex(s), "owned-by", hex(owner[s]))
        del pair[ea]
        if source.get(ea) == "map":
            del want[ea]

# DDS names for functions SMT3 does not name
for r in csv.DictReader(open(f"{D}/dds_unique_in_raidou.tsv"), delimiter="\t"):
    if r["game"] != GAME:
        continue
    ea = int(r["ea"], 16)
    if ea in want:
        continue
    cands = [(float(r[f"{v}_sim"] or 0), -i, r[f"{v}_name"]) for i, v in enumerate(("dds1", "dds2"))
             if r[f"{v}_name"] and not DUMMY.match(r[f"{v}_name"])]
    if not cands:
        continue
    cands.sort(reverse=True)
    name = cands[0][2]
    if len({c[2] for c in cands}) > 1:
        log("dds-name-choice", hex(ea), name, "over", cands[1][2])
    if name in claims and claims[name]:
        log("dds-name-taken", hex(ea), name)
        continue
    want[ea], source[ea] = name, "dds"
    claims[name] = [ea]

# ---- name ops: overwrite dummy names and names the map put there earlier
ops = []
cur = {ea: v["name"] for ea, v in rmeta.items()}
by_cur = {v: k for k, v in cur.items()}
fn_count = Counter()
clear = set()
for ea, name in sorted(want.items()):
    now = cur.get(ea, "")
    if now == name:
        fn_count["already"] += 1
        continue
    if not (DUMMY.match(now) or now == map_name.get(ea)):
        log("user-name-kept", hex(ea), now, name)
        fn_count["user-kept"] += 1
        continue
    other = by_cur.get(name)
    if other is not None and other != ea:
        if want.get(other) == name or not (DUMMY.match(name) or name == map_name.get(other)):
            log("name-held-elsewhere", hex(ea), name, hex(other))
            fn_count["held"] += 1
            continue
        clear.add(other)
    ops.append({"op": "fname", "ea": ea, "name": name, "expect": now, "src": source[ea]})
    fn_count[source[ea]] += 1
for ea in clear:
    if ea not in want:
        ops.insert(0, {"op": "fclear", "ea": ea, "expect": cur[ea]})
        fn_count["cleared"] += 1
    else:
        ops.insert(0, {"op": "fclear", "ea": ea, "expect": cur[ea]})

# ---- identical pairs
ident = {ea: s for ea, s in pair.items() if ea in R and s in S and same_code(R[ea]["words"], S[s]["words"])}
log("identical-smt3", len(ident), "of", len(pair))
proto_n = cmt_n = 0
for ea, s in sorted(ident.items()):
    sm, rm = smeta.get(s), rmeta.get(ea, {})
    if not sm:
        continue
    if sm["proto"] and not rm.get("proto"):
        ops.append({"op": "proto", "ea": ea, "decl": sm["proto"]})
        proto_n += 1
    for rep in (0, 1):
        if sm["fcmt"][rep] and not (rm.get("fcmt") or [None, None])[rep]:
            ops.append({"op": "fcmt", "ea": ea, "rep": rep, "text": sm["fcmt"][rep]})
            cmt_n += 1
    have = rm.get("cmts", {})
    for off, c in sm["cmts"].items():
        hc = have.get(off, [None, None])
        for rep in (0, 1):
            if c[rep] and not hc[rep]:
                ops.append({"op": "cmt", "ea": ea + int(off), "rep": rep, "text": c[rep]})
                cmt_n += 1


# ---- data symbols
def by_start(table):
    starts = sorted(table)
    return starts, table


def containing(index, addr):
    starts, table = index
    i = bisect.bisect_right(starts, addr) - 1
    if i < 0:
        return None
    st = starts[i]
    size = table[st]["size"]
    if addr == st or addr < st + size:
        return st
    return None


def votes_from(pairs, src_funcs, src_gp, index):
    v = defaultdict(lambda: defaultdict(int))
    for ea, s in pairs.items():
        a = dict(data_refs(src_funcs[s]["words"], src_gp))
        for k, t in data_refs(R[ea]["words"], gp["r"]):
            if k in a:
                st = containing(index, a[k])
                if st is not None:
                    v[st][t - (a[k] - st)] += 1
    return v


smt3_syms = {ea: {"name": d["name"], "decl": d["decl"], "size": d["size"]} for ea, d in sdata.items()
             if not DUMMY.match(d["name"])}
dds_syms = {}
for v, fns in (("dds1", D1), ("dds2", D2)):
    syms = {}
    if v == "dds1":
        for ea, d in d1data.items():
            if not DUMMY.match(d["name"]):
                syms[ea] = {"name": d["name"], "decl": d["decl"], "size": d["size"]}
    for ea, (name, ty, size) in symbol_addrs(f"{DDS}/{v}/symbol_addrs.txt", False).items():
        if DUMMY.match(name):
            continue
        decl, size = splat_decl(name, ty, size)
        if ea not in syms or syms[ea]["name"] == name:
            syms[ea] = {"name": name, "decl": (syms.get(ea) or {}).get("decl") or decl, "size": size}
    dds_syms[v] = syms

proposals = defaultdict(list)  # target -> [(rank, game, name, decl)]
for game, src, src_gp, syms, pairs in (
        ("smt3", S, gp["smt3"], smt3_syms, ident),
        *[(v, fns, gp[v], dds_syms[v],
           {int(r["b_ea"], 16): int(r["a_ea"], 16) for r in csv.DictReader(
               open(f"{D}/{GAME}_vs_{v}.tsv"), delimiter="\t")}) for v, fns in (("dds1", D1), ("dds2", D2))]):
    if game != "smt3":
        pairs = {ea: s for ea, s in pairs.items()
                 if ea in R and s in src and same_code(R[ea]["words"], src[s]["words"])}
        log(f"identical-{game}", len(pairs))
    votes = votes_from(pairs, src, src_gp, by_start(syms))
    for st, targets in votes.items():
        sym = syms[st]
        if len(targets) > 1:
            log("data-conflict", game, sym["name"], hex(st), " ".join(f"{t:#x}x{n}" for t, n in targets.items()))
            continue
        t = next(iter(targets))
        proposals[t].append((0 if game == "smt3" else 1, game, sym["name"], sym["decl"], sym["size"]))

data_n = Counter()
taken = set(want.values()) | set(cur.values())
names_at = defaultdict(set)
for t, ps in proposals.items():
    for p in ps:
        names_at[p[2]].add(t)
for t, ps in sorted(proposals.items()):
    ps.sort()
    best = [p for p in ps if p[0] == ps[0][0]]
    if len({p[2] for p in best}) > 1:
        log("data-target-conflict", hex(t), " ".join(f"{p[1]}:{p[2]}" for p in best))
        data_n["conflict"] += 1
        continue
    _, game, name, decl, size = best[0]
    if len({p[2] for p in ps}) > 1:
        log("data-name-choice", hex(t), name, "over", " ".join(f"{p[1]}:{p[2]}" for p in ps if p[2] != name))
    if len(names_at[name]) > 1:
        log("data-name-multi-target", name, " ".join(hex(x) for x in sorted(names_at[name])))
        data_n["conflict"] += 1
        continue
    now = rdata.get(t, {}).get("name")
    if now == name:
        data_n["already"] += 1  # still planned, so an oversized type gets removed
    elif now and not DUMMY.match(now):
        log("data-user-name-kept", hex(t), now, name)
        data_n["user-kept"] += 1
        continue
    if name in cur.values() and cur.get(t) != name:
        log("data-name-is-function", hex(t), name)
        data_n["conflict"] += 1
        continue
    ops.append({"op": "dname", "ea": t, "name": name, "decl": decl, "src": game, "size": size})
    data_n[game] += now != name

# A type must end before the next symbol here; ida_apply_plan.py checks that against the type's size.
dn = sorted([o for o in ops if o["op"] == "dname"], key=lambda o: o["ea"])
bounds = sorted({o["ea"] for o in dn} | set(rdata))
for o in dn:
    i = bisect.bisect_right(bounds, o["ea"])
    o["limit"] = bounds[i] if i < len(bounds) else None
    o.pop("size", None)

# Syscall stubs IDA left out of the function list, between stubs it has
stubs = sorted(ea for ea, f in R.items() if syscall_no(f["words"]) is not None)
starts = sorted(R)
for ea in stubs:
    gap = ea + 0x10
    i = bisect.bisect_left(starts, gap)
    if gap not in R and i < len(starts) and starts[i] - gap >= 0x10 and starts[i] in stubs:
        ops.append({"op": "stub", "ea": gap, "names": {str(n): [x for x in v if not DUMMY.match(x)][0] for n, v in sys_names.items()
                                                        if len([x for x in v if not DUMMY.match(x)]) == 1}})
        data_n["stub"] += 1

json.dump(ops, open(f"{A}/{GAME}_plan.json", "w"))
summary = [f"functions {dict(fn_count)}", f"identical {len(ident)} protos {proto_n} comments {cmt_n}",
           f"data {dict(data_n)}", f"ops {len(ops)}"]
open(f"{A}/{GAME}_plan.log", "w").write("\n".join(summary + LOG) + "\n")
print("\n".join(summary))
