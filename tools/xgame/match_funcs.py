#!/usr/bin/env python3
"""Match functions between two IDA exports from ida_export_funcs.py.

Usage: match_funcs.py A.json B.json OUT.tsv
Writes one row per B function matched to A: b_ea, a_ea, a_name, method, score.
"""
import bisect
import difflib
import hashlib
import json
import sys
from collections import Counter, defaultdict

GP = 28
MIN_WORDS = 6
# I-type opcodes that write rt: immediate ALU ops and GPR loads (lq included)
GPR_WRITERS = set(range(8, 15)) | {0x18, 0x19, 0x1A, 0x1B, 0x1E} | set(range(0x20, 0x28)) | {0x37}


def op(w):
    return w >> 26


def normalize(words):
    """Return (strict, loose) token lists.

    strict masks jump targets and immediates that form addresses (lui, and
    I-type users of a lui-loaded or gp register). loose masks every immediate.
    """
    strict, loose = [], []
    lui_regs = set()
    for w in words:
        o = op(w)
        rs, rt = (w >> 21) & 31, (w >> 16) & 31
        if o in (2, 3):
            strict.append(w & 0xFC000000)
            loose.append(w & 0xFC000000)
            continue
        itype = o >= 4 and o not in (0x10, 0x11, 0x12, 0x13)
        if o == 15:  # lui
            strict.append(w & 0xFFFF0000)
            lui_regs.add(rt)
        elif itype and (rs == GP or rs in lui_regs):
            strict.append(w & 0xFFFF0000)
        else:
            strict.append(w)
        loose.append(w & 0xFFFF0000 if itype else w)
        if o in (9, 13) and rs in lui_regs:  # addiu/ori finishing an address
            lui_regs.add(rt)
        elif o in GPR_WRITERS:
            lui_regs.discard(rt)
    return strict, loose


def h(tokens):
    return hashlib.blake2b(b"".join(t.to_bytes(4, "little") for t in tokens), digest_size=12).digest()


class Side:
    def __init__(self, path):
        self.funcs = json.load(open(path))
        self.funcs.sort(key=lambda f: f["ea"])
        self.by_ea = {f["ea"]: f for f in self.funcs}
        self.eas = [f["ea"] for f in self.funcs]
        self.index = {f["ea"]: i for i, f in enumerate(self.funcs)}
        self.callers = defaultdict(set)
        for f in self.funcs:
            s, l = normalize(f["words"])
            f["strict"], f["loose"] = h(s), h(l)
            f["ops"] = [w & 0xFC00003F if op(w) == 0 else w & 0xFC000000 for w in f["words"]]
            f["strkey"] = tuple(sorted(set(f["strs"])))
            for c in f["calls"]:
                self.callers[c].add(f["ea"])


def unique_pairs(A, B, key, filt, unmatched_a, unmatched_b):
    ca = defaultdict(list)
    cb = defaultdict(list)
    for ea in unmatched_a:
        f = A.by_ea[ea]
        if filt(f):
            ca[key(f)].append(ea)
    for ea in unmatched_b:
        f = B.by_ea[ea]
        if filt(f):
            cb[key(f)].append(ea)
    return [(ca[k][0], cb[k][0]) for k in ca if len(ca[k]) == 1 and len(cb.get(k, ())) == 1]


def ratio(fa, fb):
    la, lb = len(fa["ops"]), len(fb["ops"])
    if min(la, lb) / max(la, lb) < 0.5:
        return 0.0
    return difflib.SequenceMatcher(None, fa["ops"], fb["ops"], autojunk=False).ratio()


def main():
    A, B, out = Side(sys.argv[1]), Side(sys.argv[2]), sys.argv[3]
    m = {}       # b_ea -> (a_ea, method, score)
    used_a = set()

    def add(a, b, method, score=1.0):
        if b in m or a in used_a:
            return False
        m[b] = (a, method, score)
        used_a.add(a)
        return True

    def free():
        return [ea for ea in A.eas if ea not in used_a], [ea for ea in B.eas if ea not in m]

    big = lambda f: len(f["words"]) >= MIN_WORDS
    for method, key, filt in (
        ("strict", lambda f: f["strict"], big),
        ("loose", lambda f: f["loose"], big),
        ("strings", lambda f: f["strkey"], lambda f: len(f["strkey"]) > 0),
    ):
        fa, fb = free()
        for a, b in unique_pairs(A, B, key, filt, fa, fb):
            add(a, b, method)

    changed = True
    while changed:
        changed = False
        # Calls: a matched pair with identical structure pairs its callees in order.
        for b, (a, _, _) in list(m.items()):
            ca, cb = A.by_ea[a]["calls"], B.by_ea[b]["calls"]
            if len(ca) == len(cb) and A.by_ea[a]["loose"] == B.by_ea[b]["loose"]:
                for x, y in zip(ca, cb):
                    if A.by_ea[x]["loose"] == B.by_ea[y]["loose"] and add(x, y, "callee"):
                        changed = True
        # Link order: a gap between two consecutive matches pairs functions
        # one to one when both sides hold the same count and similar code.
        pairs = sorted((m[b][0], b) for b in m)
        for (a0, b0), (a1, b1) in zip(pairs, pairs[1:]):
            if a1 < a0 or b1 < b0:
                continue
            ga = [ea for ea in A.eas[A.index[a0] + 1:A.index[a1]] if ea not in used_a]
            gb = [ea for ea in B.eas[B.index[b0] + 1:B.index[b1]] if ea not in m]
            if not ga or not gb or len(ga) > 40 or len(gb) > 40:
                continue
            if len(ga) == len(gb):
                for x, y in zip(ga, gb):
                    fx, fy = A.by_ea[x], B.by_ea[y]
                    if fx["loose"] == fy["loose"] or ratio(fx, fy) >= 0.6:
                        add(x, y, "order", ratio(fx, fy)) and (changed := True)
            else:
                # unequal gap: greedy best similarity, kept in order
                last = -1
                for y in gb:
                    best, bi = 0.0, None
                    for i, x in enumerate(ga):
                        if i <= last or x in used_a:
                            continue
                        r = ratio(A.by_ea[x], B.by_ea[y])
                        if r > best:
                            best, bi = r, i
                    if bi is not None and best >= 0.8:
                        add(ga[bi], y, "gap", best) and (changed := True)
                        last = bi

    # Leftovers with a strong whole-binary nearest neighbour, among same-size buckets.
    fa, fb = free()
    by_len = defaultdict(list)
    for ea in fa:
        by_len[len(A.by_ea[ea]["ops"]) // 8].append(ea)
    for y in fb:
        fy = B.by_ea[y]
        if len(fy["ops"]) < 16:
            continue
        k = len(fy["ops"]) // 8
        cands = [x for d in (-1, 0, 1) for x in by_len.get(k + d, ()) if x not in used_a]
        scored = sorted(((ratio(A.by_ea[x], fy), x) for x in cands), reverse=True)[:2]
        if scored and scored[0][0] >= 0.9 and (len(scored) == 1 or scored[0][0] - scored[1][0] >= 0.05):
            add(scored[0][1], y, "fuzzy", scored[0][0])

    with open(out, "w") as fp:
        fp.write("b_ea\ta_ea\ta_name\tmethod\tscore\tb_words\n")
        for b in sorted(m):
            a, method, score = m[b]
            fp.write(f"{b:#x}\t{a:#x}\t{A.by_ea[a]['name']}\t{method}\t{score:.2f}\t{len(B.by_ea[b]['words'])}\n")
    c = Counter(v[1] for v in m.values())
    wa = sum(len(A.by_ea[v[0]]["words"]) for v in m.values())
    print(f"{sys.argv[2]}: {len(m)}/{len(B.funcs)} B funcs matched, "
          f"{len(m)}/{len(A.funcs)} A funcs ({wa}/{sum(len(f['words']) for f in A.funcs)} A words) {dict(c)}")


if __name__ == "__main__":
    main()
