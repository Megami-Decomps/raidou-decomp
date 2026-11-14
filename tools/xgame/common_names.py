#!/usr/bin/env python3
"""One name for every function and data symbol Raidou 1 and Raidou 2 share.

Subcommands: pairs --from PAIRS (needs the matched build), apply, name ROWS, check.
The naming rule is in docs/CONTRIBUTING.md."""

from __future__ import annotations

import argparse
import bisect
import functools
import hashlib
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from retail_elf import VERSIONS, load_segments, va_to_off  # noqa: E402
from raidou_names import reloc_hash  # noqa: E402

V1, V2 = "raidou1", "raidou2"
FUNC_PAIRS = ROOT / "config/xgame/r1_r2_pairs.tsv"
DATA_PAIRS = ROOT / "config/xgame/r1_r2_data_pairs.tsv"
FUNC_HEAD = "r1_addr\tr2_addr\tname\tidentical_bytes\tmasked_identical\tr1_unit\tr2_unit\tevidence"
DATA_HEAD = "r1_addr\tr2_addr\tname\tsection\tsize\tidentical_bytes\tevidence"
MARKER = "// Names from tools/xgame/common_names.py (provenance: name_sources.txt)"
ROW = re.compile(r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;(.*)$")
# Names that say only where a symbol is: splat's defaults and this tool's.
PLACEHOLDER = re.compile(r"(func|D)_(r1_)?[0-9A-F]{8}")
INSN = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+(\S+)\s*(.*)$")
DATA_LINE = re.compile(r"^\s*/\* (?:[0-9A-F]+ )?([0-9A-F]{8})(?: ([0-9A-F]+))? \*/\s+(\.\w+)\s*(.*)$")
REF = re.compile(r"%(?:hi|lo|gp_rel)\(([A-Za-z_]\w*)\)")
GLABEL = re.compile(r"^\s*glabel (\S+)")
DLABEL = re.compile(r"^\s*dlabel (\S+)")
SECTION = re.compile(r"^\.section (\.\w+)")
GAP_RATIO = (0.8, 1.25)


def common_func(r1_addr):
    return f"func_r1_{r1_addr:08X}"


def common_data(r1_addr):
    return f"D_r1_{r1_addr:08X}"


def is_placeholder(name):
    return PLACEHOLDER.fullmatch(name) is not None


# ---- inputs


def symbol_rows(version):
    """{addr: name} from symbol_addrs.txt, first name at an address."""
    out = {}
    for line in (ROOT / "config" / version / "symbol_addrs.txt").read_text().splitlines():
        if m := ROW.match(line):
            out.setdefault(int(m[2], 16), m[1])
    return out


def elf_functions(version):
    """{addr: (size, name)} of FUNC symbols in the built ELF."""
    from elftools.elf.elffile import ELFFile
    path = ROOT / "build" / version / f"{VERSIONS[version]['serial']}.elf"
    with path.open("rb") as f:
        elf = ELFFile(f)
        out = {}
        for s in elf.get_section_by_name(".symtab").iter_symbols():
            if s["st_info"]["type"] == "STT_FUNC" and s["st_size"]:
                out[s["st_value"]] = (s["st_size"], s.name)
    return out


def text_units(version):
    """Sorted [(vram, unit)] of the .text subsegments in the splat yaml."""
    import yaml
    spec = yaml.safe_load((ROOT / "config" / version / f"{VERSIONS[version]['serial']}.yaml").read_text())
    main = next(s for s in spec["segments"] if isinstance(s, dict) and s.get("name") == "main")
    base = main["vram"] - main["start"]
    out = []
    for sub in main["subsegments"]:
        if isinstance(sub, list) and len(sub) >= 3 and sub[1] in ("c", "asm", "hasm"):
            out.append((sub[0] + base, sub[2]))
    return out


def unit_path(version, unit, kind_c):
    if not kind_c:
        return f"asm/{version}/{unit}.s"
    own = ROOT / "src" / version / f"{unit}.c"
    return (own if own.exists() else ROOT / "src/shared" / f"{unit}.c").relative_to(ROOT).as_posix()


def read_cross_pairs(path):
    """pair_build.py output as [(r1, r2, tier, evidence)]."""
    out = []
    for line in Path(path).read_text().splitlines():
        if not line or line.startswith("#") or line.startswith("kind\t"):
            continue
        kind, a, _an, _asz, b, _bsz, _d, tier, ev = line.split("\t")
        if kind.startswith("pair"):
            out.append((int(a, 16), int(b, 16), tier, ev))
    return out


def asm_code(version):
    """{function: [(vram, data refs)]} from the split .text assembly."""
    out = {}
    base = ROOT / "asm" / version
    for path in base.rglob("*.s"):
        rel = path.relative_to(base).parts
        if rel[0] in ("nonmatchings", "data", "matchings"):
            continue
        cur = None
        for line in path.read_text().splitlines():
            if m := GLABEL.match(line):
                cur = out.setdefault(m[1], [])
            elif cur is not None and (m := INSN.match(line)):
                cur.append((int(m[1], 16), REF.findall(m[3])))
    return out


def asm_data(version):
    """{name: [section, addr, size, {offset: word symbol}]} from the split data assembly."""
    out = {}
    order = defaultdict(list)
    for path in sorted((ROOT / "asm" / version / "data").rglob("*.s")):
        section = None
        cur = None
        for line in path.read_text().splitlines():
            if m := SECTION.match(line):
                section = m[1]
            elif m := DLABEL.match(line):
                cur = out.setdefault(m[1], [section, None, 0, {}])
            elif cur is not None and (m := DATA_LINE.match(line)):
                addr = int(m[1], 16)
                if cur[1] is None:
                    cur[1] = addr
                    order[section].append(addr)
                op, arg = m[3], m[4].split("/*")[0].strip()
                if op == ".word" and re.fullmatch(r"[A-Za-z_]\w*", arg):
                    cur[3][addr - cur[1]] = arg
    # Sizes: up to the next symbol of the section.
    by_addr = {}
    for name, row in out.items():
        if row[1] is not None:
            by_addr.setdefault((row[0], row[1]), name)
    for section, starts in order.items():
        starts = sorted(set(starts))
        for a, b in zip(starts, starts[1:] + [None]):
            name = by_addr[(section, a)]
            out[name][2] = (b - a) if b is not None else 0
    return {n: r for n, r in out.items() if r[1] is not None}


class Image:
    def __init__(self, version):
        self.data = (ROOT / "orig" / version / VERSIONS[version]["serial"]).read_bytes()
        self.segs = load_segments(self.data)

    def read(self, addr, size):
        off = va_to_off(self.segs, addr)
        if off is None or size <= 0:
            return b""
        return self.data[off:off + size]


# ---- function pairs


def pair_functions(cross_path):
    f1, f2 = elf_functions(V1), elf_functions(V2)
    pairs = {}
    used2 = set()

    def add(a, b, ev):
        if a in pairs or b in used2 or a not in f1 or b not in f2:
            return False
        pairs[a] = (b, ev)
        used2.add(b)
        return True

    # Code the same after masking relocations, once in each game.
    img1, img2 = Image(V1), Image(V2)
    h1, h2 = defaultdict(list), defaultdict(list)
    for f, img, h in ((f1, img1, h1), (f2, img2, h2)):
        for a, (size, _n) in f.items():
            if size >= 16:
                h[reloc_hash(img.read(a, size))].append(a)
    for h, a in h1.items():
        if len(a) == 1 and len(h2.get(h, ())) == 1:
            add(a[0], h2[h][0], "unique-reloc-hash")
    for a, b, tier, ev in read_cross_pairs(cross_path):
        if tier in ("high", "medium"):
            add(a, b, ev.split("+")[0])
    names2 = {n: a for a, (_s, n) in f2.items() if not is_placeholder(n)}
    for a, (_s, n) in sorted(f1.items()):
        if not is_placeholder(n) and n in names2:
            add(a, names2[n], "same-name")
    # Unpaired runs between two pairs: same count on both sides, sizes close.
    s1, s2 = sorted(f1), sorted(f2)
    grown = True
    while grown:
        grown = False
        anchors = sorted(pairs)
        for x, y in zip(anchors, anchors[1:]):
            bx, by = pairs[x][0], pairs[y][0]
            if by < bx:
                continue
            g1 = s1[bisect.bisect_right(s1, x):bisect.bisect_left(s1, y)]
            g2 = s2[bisect.bisect_right(s2, bx):bisect.bisect_left(s2, by)]
            if not g1 or len(g1) != len(g2) or any(a in pairs for a in g1) or any(b in used2 for b in g2):
                continue
            ratios = [f2[b][0] / f1[a][0] for a, b in zip(g1, g2)]
            if all(GAP_RATIO[0] <= r <= GAP_RATIO[1] for r in ratios):
                for a, b, r in zip(g1, g2, ratios):
                    grown |= add(a, b, "run-order-size" if r == 1 else "run-order-near")
    return f1, f2, pairs


def write_function_pairs(cross_path):
    f1, f2, pairs = pair_functions(cross_path)
    img = {V1: Image(V1), V2: Image(V2)}
    units = {V1: text_units(V1), V2: text_units(V2)}
    kinds = {}
    for v in (V1, V2):
        import yaml
        spec = yaml.safe_load((ROOT / "config" / v / f"{VERSIONS[v]['serial']}.yaml").read_text())
        main = next(s for s in spec["segments"] if isinstance(s, dict) and s.get("name") == "main")
        kinds[v] = {sub[2]: sub[1] == "c" for sub in main["subsegments"]
                    if isinstance(sub, list) and len(sub) >= 3}

    def unit_of(v, addr):
        starts = units[v]
        i = bisect.bisect_right([s for s, _ in starts], addr) - 1
        u = starts[i][1]
        return unit_path(v, u, kinds[v].get(u, False))

    lines = [FUNC_HEAD]
    for a in sorted(pairs):
        b, ev = pairs[a]
        c1, c2 = img[V1].read(a, f1[a][0]), img[V2].read(b, f2[b][0])
        lines.append("\t".join((
            f"0x{a:08X}", f"0x{b:08X}", f1[a][1],
            "yes" if c1 == c2 else "no",
            "yes" if len(c1) == len(c2) and reloc_hash(c1) == reloc_hash(c2) else "no",
            unit_of(V1, a), unit_of(V2, b), ev)))
    FUNC_PAIRS.write_text("\n".join(lines) + "\n")
    print(f"functions: {len(pairs)} pairs of {len(f1)} / {len(f2)}; "
          + ", ".join(f"{k} {n}" for k, n in Counter(e for _b, e in pairs.values()).most_common()))
    return f1, f2, pairs


# ---- data pairs


def pair_data(f1, f2, fpairs):
    code = {V1: asm_code(V1), V2: asm_code(V2)}
    data = {V1: asm_data(V1), V2: asm_data(V2)}
    img = {V1: Image(V1), V2: Image(V2)}
    fname = {V1: {a: n for a, (_s, n) in f1.items()}, V2: {b: n for b, (_s, n) in f2.items()}}
    fpair_names = {fname[V1][a]: fname[V2][b] for a, (b, _e) in fpairs.items()}
    d1, d2 = data[V1], data[V2]

    def is_data(v, n):
        return n in data[v] and not n.startswith("jtbl_")

    pairs, back = {}, {}
    evidence = {}

    def accept(n1, n2, ev):
        if n1 in pairs or n2 in back:
            return False
        if d1[n1][0] != d2[n2][0]:
            return False
        pairs[n1], back[n2] = n2, n1
        evidence[n1] = ev
        return True

    # 1. The same instruction of a paired function refers to both.
    votes = defaultdict(Counter)
    rvotes = defaultdict(Counter)
    for a, (b, _ev) in fpairs.items():
        if f1[a][0] != f2[b][0]:
            continue
        i1, i2 = code[V1].get(fname[V1][a]), code[V2].get(fname[V2][b])
        if not i1 or not i2 or len(i1) != len(i2):
            continue
        for (_va, r1), (_vb, r2) in zip(i1, i2):
            for x, y in zip(r1, r2):
                if is_data(V1, x) and is_data(V2, y):
                    votes[x][y] += 1
                    rvotes[y][x] += 1
    conflicts = 0
    for x, c in votes.items():
        y, n = c.most_common(1)[0]
        if len(c) == 1 and len(rvotes[y]) == 1:
            accept(x, y, "code-ref")
        else:
            conflicts += 1
    # 2. Words of paired data that point at other data; check function words.
    word_check = Counter()
    grown = True
    while grown:
        grown = False
        wv, rwv = defaultdict(Counter), defaultdict(Counter)
        for x, y in list(pairs.items()):
            w1, w2 = d1[x][3], d2[y][3]
            if d1[x][2] != d2[y][2]:
                continue
            for off, s1 in w1.items():
                s2 = w2.get(off)
                if s2 is None:
                    continue
                if s1 in fpair_names:
                    word_check["func-agree" if fpair_names[s1] == s2 else "func-disagree"] += 1
                elif is_data(V1, s1) and is_data(V2, s2) and s1 not in pairs and s2 not in back:
                    wv[s1][s2] += 1
                    rwv[s2][s1] += 1
        for s1, c in wv.items():
            s2 = next(iter(c))
            if len(c) == 1 and len(rwv[s2]) == 1:
                grown |= accept(s1, s2, "data-word")
    # 3. Same real name.
    for n in d1:
        if not is_placeholder(n) and n in d2 and is_data(V1, n):
            accept(n, n, "same-name")
    # 4. Unpaired runs between pairs in one section: same count, same sizes.
    def runs():
        added = 0
        by_sec = defaultdict(list)
        for x, y in pairs.items():
            by_sec[d1[x][0]].append((d1[x][1], d2[y][1]))
        order = {V1: defaultdict(list), V2: defaultdict(list)}
        for v, d in ((V1, d1), (V2, d2)):
            for n, row in d.items():
                order[v][row[0]].append((row[1], n))
            for s in order[v]:
                order[v][s].sort()
        for sec, anchors in by_sec.items():
            anchors.sort()
            a1 = [a for a, _ in order[V1][sec]]
            a2 = [a for a, _ in order[V2][sec]]
            for (x, bx), (y, by) in zip(anchors, anchors[1:]):
                if by < bx:
                    continue
                g1 = order[V1][sec][bisect.bisect_right(a1, x):bisect.bisect_left(a1, y)]
                g2 = order[V2][sec][bisect.bisect_right(a2, bx):bisect.bisect_left(a2, by)]
                if not g1 or len(g1) != len(g2):
                    continue
                if any(n in pairs for _a, n in g1) or any(n in back for _b, n in g2):
                    continue
                if any(d1[n1][2] != d2[n2][2] for (_a, n1), (_b, n2) in zip(g1, g2)):
                    continue
                for (_a, n1), (_b, n2) in zip(g1, g2):
                    if not n1.startswith("jtbl_") and not n2.startswith("jtbl_"):
                        added += accept(n1, n2, "run-order-size")
        return added
    run_added = runs()
    # 5. Unique contents among what is left (not .bss: it has none).
    def digest(v, n):
        sec, addr, size, _w = data[v][n]
        if sec in (".bss", ".sbss") or size < 8:
            return None
        raw = img[v].read(addr, size)
        if not raw.strip(b"\0"):
            return None
        return hashlib.sha1(sec.encode() + raw).hexdigest()
    h1, h2 = defaultdict(list), defaultdict(list)
    for n in d1:
        if n not in pairs and is_data(V1, n) and (h := digest(V1, n)):
            h1[h].append(n)
    for n in d2:
        if n not in back and is_data(V2, n) and (h := digest(V2, n)):
            h2[h].append(n)
    for h, ns in h1.items():
        if len(ns) == 1 and len(h2.get(h, ())) == 1:
            accept(ns[0], h2[h][0], "unique-contents")
    print(f"data: {len(pairs)} pairs of {len(d1)} / {len(d2)}; vote conflicts {conflicts}; "
          f"function words {dict(word_check)}; "
          + ", ".join(f"{k} {n}" for k, n in Counter(evidence.values()).most_common()))
    return data, img, pairs, evidence


def write_data_pairs(f1, f2, fpairs):
    data, img, pairs, evidence = pair_data(f1, f2, fpairs)
    lines = [DATA_HEAD]
    for n1 in sorted(pairs, key=lambda n: data[V1][n][1]):
        n2 = pairs[n1]
        sec, a, size, _w = data[V1][n1]
        b, size2 = data[V2][n2][1], data[V2][n2][2]
        same = sec not in (".bss", ".sbss") and size == size2 and img[V1].read(a, size) == img[V2].read(b, size2)
        lines.append("\t".join((f"0x{a:08X}", f"0x{b:08X}", n1, sec, f"0x{size:X}",
                                "yes" if same else "no", evidence[n1])))
    DATA_PAIRS.write_text("\n".join(lines) + "\n")


# ---- naming


def read_table(path):
    rows = path.read_text().splitlines()
    head = rows[0].split("\t")
    return [dict(zip(head, r.split("\t"))) for r in rows[1:] if r]


def default_name(kind, addr):
    return f"func_{addr:08X}" if kind == "func" else f"D_{addr:08X}"


def current_names(version, kind, table):
    """Name each paired address has now: symbol_addrs.txt, else splat's default."""
    rows = symbol_rows(version)
    col = "r1_addr" if version == V1 else "r2_addr"
    return {int(r[col], 16): rows.get(int(r[col], 16), default_name(kind, int(r[col], 16))) for r in table}


def decide(kind, table):
    """[(r1, r2, old1, old2, new, reason)] and the conflicts left alone."""
    n1, n2 = current_names(V1, kind, table), current_names(V2, kind, table)
    taken = {}
    for v in (V1, V2):
        taken[v] = {n: a for a, n in symbol_rows(v).items()}
        taken[v].update(aliases(v))
    out, conflicts = [], []
    for r in table:
        a, b = int(r["r1_addr"], 16), int(r["r2_addr"], 16)
        o1, o2 = n1[a], n2[b]
        p1, p2 = is_placeholder(o1), is_placeholder(o2)
        if not p1 and not p2:
            if o1 != o2:
                conflicts.append((a, b, o1, o2, "two real names"))
            continue
        if not p1:
            new, why = o1, f"twin-of-raidou1-0x{a:08X}"
        elif not p2:
            new, why = o2, f"twin-of-raidou2-0x{b:08X}"
        else:
            new = common_func(a) if kind == "func" else common_data(a)
            why = None
        other = V2 if p2 else V1
        if new in taken[other] and taken[other][new] != (a if other == V1 else b):
            conflicts.append((a, b, o1, o2, f"{new} is another symbol in {other}"))
            continue
        out.append((a, b, o1, o2, new, why))
    return out, conflicts


def write_config(version, decisions, kind, evidence):
    sym = ROOT / "config" / version / "symbol_addrs.txt"
    src = ROOT / "config" / version / "name_sources.txt"
    side = 0 if version == V1 else 1
    new = {}
    notes = {}
    for d in decisions:
        addr, old, name, why = d[side], d[2 + side], d[4], d[5]
        if old == name:
            continue
        twin = d[1 - side]
        other = V2 if version == V1 else V1
        new[addr] = (name, kind)
        if why is None:
            note = f"placeholder for the pair with {other} 0x{twin:08X}, {evidence[(d[0], d[1])]}"
            notes[addr] = f"0x{addr:08X}\t{name}\tplaceholder\t{note}"
        else:
            notes[addr] = f"0x{addr:08X}\t{name}\tinferred\t{why}, {evidence[(d[0], d[1])]}"
    lines = sym.read_text().splitlines()
    keep = [l for l in lines if not ((m := ROW.match(l)) and int(m[2], 16) in new)]
    if MARKER not in keep:
        keep.append(MARKER)
    at = keep.index(MARKER) + 1
    end = next((i for i in range(at, len(keep)) if keep[i].startswith("//")), len(keep))
    block = [l for l in keep[at:end] if ROW.match(l)]
    block += [f"{n} = 0x{a:08X};" + (" // type:func" if k == "func" else "") for a, (n, k) in new.items()]
    block.sort(key=lambda l: int(ROW.match(l)[2], 16))
    out = keep[:at] + block + keep[end:]
    names = Counter(ROW.match(l)[1] for l in out if ROW.match(l))
    if dup := [n for n, c in names.items() if c > 1]:
        sys.exit(f"{version}: names used twice: {dup[:10]}")
    sym.write_text("\n".join(out) + "\n")
    rows = {}
    for line in src.read_text().splitlines():
        if line.strip() and not line.startswith("#"):
            rows[int(line.split("\t")[0], 16)] = line
    rows.update(notes)
    src.write_text("# addr\tname\tevidence|inferred|placeholder\tnote\n"
                   + "\n".join(rows[a] for a in sorted(rows)) + "\n")
    return {d[2 + side]: d[4] for d in decisions if d[2 + side] != d[4]}


def rename_tree(renames, existing, alias_names):
    """Rename references in src/ and include/. A file both versions compile takes a
    rename only when the old name means the same thing in both. An alias stays in a
    file that already names its target another way (the two can have other types)."""
    r1, r2 = renames[V1], renames[V2]
    both = {}
    clash = set()
    for old in r1.keys() | r2.keys():
        a, b = r1.get(old), r2.get(old)
        if (a and b and a != b) or (a and not b and old in existing[V2]) or (b and not a and old in existing[V1]):
            clash.add(old)
        else:
            both[old] = a or b
    word = re.compile(r"\b(?:func|D)_\w+|\b[A-Za-z_]\w*")
    changed = Counter()
    unresolved = Counter()
    for path in sorted(list((ROOT / "src").rglob("*.[ch]")) + list((ROOT / "include").rglob("*.h"))):
        rel = path.relative_to(ROOT).parts
        table = r1 if rel[:2] == ("src", V1) else r2 if rel[:2] == ("src", V2) else both
        text = path.read_text()
        present = set(word.findall(text))
        aliased = alias_names[V1] if table is r1 else alias_names[V2] if table is r2 \
            else alias_names[V1] | alias_names[V2]
        targets = defaultdict(set)
        for n in present:
            if n in table:
                targets[table[n]].add(n)
        keep = {n for n in present & aliased if n in table
                and (table[n] in present or len(targets[table[n]]) > 1)}

        def sub(m):
            n = m[0]
            if table is both and n in clash:
                unresolved[n] += 1
            if n in keep:
                return n
            return table.get(n, n)
        new = word.sub(sub, text)
        if new != text:
            path.write_text(new)
            changed[rel[1] if rel[0] == "src" else "include"] += 1
    return changed, unresolved


ALIAS = re.compile(r"^(\w+) = 0x([0-9A-Fa-f]+);$")


def aliases(version):
    """{name: addr} of smt3_symbols.txt, the SMT3 names carried C uses at Raidou addresses."""
    out = {}
    for line in (ROOT / "config" / version / "smt3_symbols.txt").read_text().splitlines():
        if m := ALIAS.match(line):
            out[m[1]] = int(m[2], 16)
    return out


def symbol_starts(version):
    """{addr: name} of every function and data symbol the split defines."""
    out = {body[0][0]: n for n, body in asm_code(version).items() if body}
    for n, row in asm_data(version).items():
        out.setdefault(row[1], n)
    return out


def alias_plan(renames, pairs):
    """SMT3 placeholder aliases give way to the name of what they point at; real SMT3
    names reach the paired symbol of the other game."""
    al = {V1: aliases(V1), V2: aliases(V2)}
    starts = {}
    for v in (V1, V2):
        starts[v] = {a: renames[v].get(n, n) for a, n in symbol_starts(v).items()}
        if overlap := set(al[v]) & set(renames[v]):
            sys.exit(f"{v}: smt3_symbols.txt names that are also renamed symbols: {sorted(overlap)[:10]}")
    to_alias = {V1: {}, V2: {}}
    for v in (V1, V2):
        for n, a in al[v].items():
            if is_placeholder(n) and a in starts[v]:
                to_alias[v][n] = starts[v][a]
    added = {V1: {}, V2: {}}
    conflicts = []
    twin = {V1: dict(pairs), V2: {b: a for a, b in pairs}}
    for v, o in ((V1, V2), (V2, V1)):
        taken = {n: a for a, n in starts[o].items()}
        taken.update(al[o])
        for n, a in al[v].items():
            if is_placeholder(n) or a not in twin[v]:
                continue
            b = twin[v][a]
            if taken.get(n) == b:
                continue
            if n in taken:
                conflicts.append(f"{n}: {v} 0x{a:08X}, {o} has it elsewhere")
                continue
            added[o][n] = b
    return to_alias, added, conflicts


def compiled_text(version):
    paths = list((ROOT / "src" / version).rglob("*.[ch]")) + list((ROOT / "src/shared").rglob("*.[ch]"))
    paths += list((ROOT / "include").rglob("*.h"))
    return "\n".join(p.read_text() for p in paths)


def write_aliases(to_alias, added):
    """Drop replaced aliases nothing compiled names any more; add the mirrored ones."""
    for v in (V1, V2):
        path = ROOT / "config" / v / "smt3_symbols.txt"
        used = set(re.findall(r"\b[A-Za-z_]\w*", compiled_text(v)))
        out = [line for line in path.read_text().splitlines()
               if not ((m := ALIAS.match(line)) and m[1] in to_alias[v] and m[1] not in used)]
        out += [f"{n} = 0x{a:08X};" for n, a in sorted(added[v].items(), key=lambda x: x[1])]
        path.write_text("\n".join(out) + "\n")


def apply():
    renames = {V1: {}, V2: {}}
    report = []
    all_pairs = []
    for kind, path in (("func", FUNC_PAIRS), ("data", DATA_PAIRS)):
        table = read_table(path)
        ev = {(int(r["r1_addr"], 16), int(r["r2_addr"], 16)): r["evidence"] for r in table}
        all_pairs += ev.keys()
        decisions, conflicts = decide(kind, table)
        for v in (V1, V2):
            renames[v].update(write_config(v, decisions, kind, ev))
        names = {(d[0], d[1]): d[4] for d in decisions}
        for r in table:
            key = (int(r["r1_addr"], 16), int(r["r2_addr"], 16))
            if key in names:
                r["name"] = names[key]
        head = FUNC_HEAD if kind == "func" else DATA_HEAD
        path.write_text(head + "\n" + "\n".join("\t".join(r[c] for c in head.split("\t")) for r in table) + "\n")
        report.append(f"{kind}: {len(decisions)} named, {len(conflicts)} left with two names")
        for c in conflicts:
            print(f"  conflict {kind} 0x{c[0]:08X}/0x{c[1]:08X} {c[2]} / {c[3]}: {c[4]}")
    to_alias, added, alias_conflicts = alias_plan(renames, all_pairs)
    for c in alias_conflicts:
        print("  alias conflict", c)
    existing = {v: set(symbol_starts(v).values()) | set(aliases(v)) for v in (V1, V2)}
    changed, unresolved = rename_tree({v: {**renames[v], **to_alias[v]} for v in (V1, V2)}, existing,
                                      {v: set(to_alias[v]) for v in (V1, V2)})
    write_aliases(to_alias, added)
    print("\n".join(report))
    print("aliases replaced:", {v: len(to_alias[v]) for v in to_alias},
          "mirrored:", {v: len(added[v]) for v in added})
    print("files changed:", dict(changed))
    if unresolved:
        print("left in files both versions compile (old name means two things):", dict(unresolved))


def pair_maps():
    """({r1: r2}, {r2: r1}, {(r1, r2): table}) over both tables."""
    fwd, kind = {}, {}
    for k, path in (("func", FUNC_PAIRS), ("data", DATA_PAIRS)):
        for r in read_table(path):
            a, b = int(r["r1_addr"], 16), int(r["r2_addr"], 16)
            fwd[a] = b
            kind[(a, b)] = k
    return fwd, {b: a for a, b in fwd.items()}, kind


def set_names(version, new, notes):
    """Replace or add symbol_addrs.txt rows ({addr: row text}) and name_sources rows."""
    sym = ROOT / "config" / version / "symbol_addrs.txt"
    lines = sym.read_text().splitlines()
    out = []
    for line in lines:
        if (m := ROW.match(line)) and int(m[2], 16) in new:
            out.append(new.pop(int(m[2], 16)))
        else:
            out.append(line)
    if new:
        if MARKER not in out:
            out.append(MARKER)
        at = out.index(MARKER) + 1
        end = next((i for i in range(at, len(out)) if out[i].startswith("//")), len(out))
        block = out[at:end] + list(new.values())
        block.sort(key=lambda l: int(ROW.match(l)[2], 16))
        out = out[:at] + block + out[end:]
    names = Counter(ROW.match(l)[1] for l in out if ROW.match(l))
    if dup := [n for n, c in names.items() if c > 1]:
        sys.exit(f"{version}: names used twice: {dup[:10]}")
    sym.write_text("\n".join(out) + "\n")
    src = ROOT / "config" / version / "name_sources.txt"
    rows = {}
    for line in src.read_text().splitlines():
        if line.strip() and not line.startswith("#"):
            rows[int(line.split("\t")[0], 16)] = line
    rows.update(notes)
    src.write_text("# addr\tname\tevidence|inferred|placeholder\tnote\n"
                   + "\n".join(rows[a] for a in sorted(rows)) + "\n")


def name(rows_path):
    """Give real names (rows: version, addr, name, evidence|inferred, note, optional
    symbol_addrs attributes) to symbols and to their twins."""
    fwd, back, kind = pair_maps()
    sym = {v: symbol_rows(v) for v in (V1, V2)}
    al = {v: aliases(v) for v in (V1, V2)}
    by_name = {v: {**{n: a for a, n in sym[v].items()}, **al[v]} for v in (V1, V2)}
    alias_at = {v: defaultdict(set) for v in (V1, V2)}
    for v in (V1, V2):
        for n, a in al[v].items():
            if not is_placeholder(n):
                alias_at[v][a].add(n)
    want = {V1: {}, V2: {}}
    skipped = []
    for line in Path(rows_path).read_text().splitlines():
        if not line.strip() or line.startswith("#"):
            continue
        v, addr, new, how, note, *attrs = line.split("\t")
        want[v][int(addr, 16)] = (new, how, note, attrs[0] if attrs else "")
    # A twin takes the name its sibling gets; two different names for one pair is a conflict.
    plan = {V1: {}, V2: {}}
    for v, o, twin in ((V1, V2, fwd), (V2, V1, back)):
        for a, (new, how, note, attrs) in want[v].items():
            plan[v][a] = (new, how, note, attrs)
            b = twin.get(a)
            if b is None:
                continue
            if b in want[o]:
                if want[o][b][0] != new:
                    skipped.append(f"{new}: {v} 0x{a:08X} and {o} 0x{b:08X} are proposed as {want[o][b][0]}")
                    plan[v].pop(a)
                continue
            plan[o].setdefault(b, (new, "inferred", f"twin-of-{v}-0x{a:08X}",
                                   "type:func" if attrs.startswith("type:func") else ""))
    for v, o, twin in ((V1, V2, fwd), (V2, V1, back)):
        for a in list(plan[v]):
            b = twin.get(a)
            if b is not None and (b not in plan[o] or plan[o][b][0] != plan[v][a][0]):
                plan[v].pop(a)
                skipped.append(f"{v} 0x{a:08X}: twin not named the same")
    renames = {V1: {}, V2: {}}
    for v in (V1, V2):
        new_rows, notes = {}, {}
        for a, (new, how, note, attrs) in sorted(plan[v].items()):
            k = "func" if attrs.startswith("type:func") or a in paired_functions(v) else "data"
            old = sym[v].get(a, default_name(k, a))
            if not is_placeholder(old):
                skipped.append(f"{new}: {v} 0x{a:08X} is already {old}")
                continue
            if alias_at[v][a] - {new}:
                skipped.append(f"{new}: {v} 0x{a:08X} is already {sorted(alias_at[v][a])[0]} (smt3_symbols.txt)")
                continue
            if by_name[v].get(new, a) != a:
                skipped.append(f"{new}: {v} has it at 0x{by_name[v][new]:08X}")
                continue
            new_rows[a] = f"{new} = 0x{a:08X};" + (f" // {attrs}" if attrs else (" // type:func" if k == "func" else ""))
            notes[a] = f"0x{a:08X}\t{new}\t{how}\t{note}"
            renames[v][old] = new
        plan[v] = new_rows
        set_names(v, dict(new_rows), notes)
    # Keep the tables' name column current.
    names = {}
    for v, side in ((V1, "r1_addr"), (V2, "r2_addr")):
        for old, new in renames[v].items():
            names[(side, old)] = new
    for path in (FUNC_PAIRS, DATA_PAIRS):
        table = read_table(path)
        for r in table:
            a = int(r["r1_addr"], 16)
            if a in plan[V1]:
                r["name"] = ROW.match(plan[V1][a])[1]
        head = FUNC_HEAD if path == FUNC_PAIRS else DATA_HEAD
        path.write_text(head + "\n" + "\n".join("\t".join(r[c] for c in head.split("\t")) for r in table) + "\n")
    existing = {v: set(sym[v].values()) | set(al[v]) for v in (V1, V2)}
    changed, unresolved = rename_tree(renames, existing, {V1: set(), V2: set()})
    print(f"named: raidou1 {len(plan[V1])}, raidou2 {len(plan[V2])}; files changed {dict(changed)}")
    for s in skipped:
        print("  skipped", s)
    if unresolved:
        print("left in files both versions compile:", dict(unresolved))


@functools.cache
def paired_functions(version):
    return {int(r["r1_addr" if version == V1 else "r2_addr"], 16) for r in read_table(FUNC_PAIRS)}


# ---- check


def check():
    """Errors for every pair whose two symbols have different names."""
    errors = []
    pairs = {}
    for kind, path in (("func", FUNC_PAIRS), ("data", DATA_PAIRS)):
        if not path.exists():
            continue
        table = read_table(path)
        n1, n2 = current_names(V1, kind, table), current_names(V2, kind, table)
        for r in table:
            a, b = int(r["r1_addr"], 16), int(r["r2_addr"], 16)
            if n1[a] != n2[b]:
                errors.append(f"{path.name}: 0x{a:08X} is {n1[a]} in raidou1, 0x{b:08X} is {n2[b]} in raidou2")
            elif n1[a] != r["name"]:
                errors.append(f"{path.name}: 0x{a:08X} is {n1[a]}, the table says {r['name']}")
            pairs[a] = b
    al = {V1: aliases(V1), V2: aliases(V2)}
    back = {b: a for a, b in pairs.items()}
    rows = {V1: symbol_rows(V1), V2: symbol_rows(V2)}
    for v, o, twin in ((V1, V2, pairs), (V2, V1, back)):
        for n, a in al[v].items():
            if is_placeholder(n) or a not in twin:
                continue
            if al[o].get(n) != twin[a] and rows[o].get(twin[a]) != n:
                errors.append(f"smt3_symbols.txt: {n} is at 0x{a:08X} in {v}, not at its twin 0x{twin[a]:08X} in {o}")
    return errors


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("pairs")
    p.add_argument("--from", dest="cross", required=True, type=Path,
                   help="pair_build.py output pairing raidou1 functions with raidou2's")
    sub.add_parser("apply")
    p = sub.add_parser("name")
    p.add_argument("rows", type=Path, help="version, addr, name, evidence|inferred, note[, attributes]")
    sub.add_parser("check")
    args = ap.parse_args()
    if args.cmd == "pairs":
        f1, f2, fpairs = write_function_pairs(args.cross)
        write_data_pairs(f1, f2, fpairs)
    elif args.cmd == "apply":
        apply()
    elif args.cmd == "name":
        name(args.rows)
    else:
        errors = check()
        print("\n".join(errors[:50]), file=sys.stderr)
        if errors:
            print(f"{len(errors)} pairs with two names", file=sys.stderr)
            return 1
        print("every pair has one name")
    return 0


if __name__ == "__main__":
    sys.exit(main())
