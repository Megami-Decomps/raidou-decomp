#!/usr/bin/env python3
"""Prototypes raidou-decomp gives each function in symbol_addrs.txt: a C definition's head first,
then the declaration most units agree on. Same preprocessing as extract_types.py.
Writes protos-<v>.tsv: addr, name, source (def|decl), units, proto."""
import os, re, sys
from collections import Counter
sys.path.insert(0, os.path.dirname(__file__))
from extract_types import RD, OUT, preprocess, statements, strip_attr, norm

IDB = os.path.dirname(OUT)
KW = {"if", "while", "for", "switch", "return", "sizeof"}


def fname(decl):
    t = strip_attr(decl)
    if t.startswith("typedef") or "{" in t or "=" in t.split("(")[0]:
        return None
    m = re.search(r"\b(\w+)\s*\(([^()]|\([^()]*\))*\)\s*$", t.rstrip(";").strip())
    if not m or m.group(1) in KW:
        return None
    pre = t[:m.start(1)].strip()
    if not pre or pre.endswith("(") or re.search(r"\(\s*\*\s*$", pre):
        return None
    return m.group(1)


def clean(decl):
    t = strip_attr(decl).rstrip(";").strip()
    t = re.sub(r"^\s*(extern|static|inline|__inline__|__inline)\s+", "", t)
    t = re.sub(r"\b(static|inline|__inline__|__inline)\s+", "", t)
    return norm(t) + ";"


def main(v):
    names = {}
    for line in open(f"{RD}/config/{v}/symbol_addrs.txt"):
        m = re.match(r"(\w+) = 0x([0-9A-Fa-f]+);.*type:func", line)
        if m:
            names[m.group(1)] = int(m.group(2), 16)
    units = sorted(os.path.join(d, f) for top in (f"{RD}/src/{v}", f"{RD}/src/shared")
                   for d, _, fs in os.walk(top) for f in fs if f.endswith(".c"))
    borrow = {u for u in units if re.search(r"^\s*#\s*define\s+VERSION_DDS", open(u).read(), re.M)}
    defs, decls = {}, {}
    for u in units:
        text = None
        for raidou in ((True, False) if u in borrow else (True,)):
            text, _ = preprocess(u, v, raidou)
            if text is not None:
                break
        if text is None:
            continue
        heads = []
        for _, s in statements(text, heads):
            n = fname(s)
            if n in names:
                decls.setdefault(n, Counter())[clean(s)] += 1
        for h in heads:
            n = fname(h)
            if n in names and "..." not in h or (n in names):
                defs.setdefault(n, Counter())[clean(h)] += 1
    rows = 0
    with open(f"{IDB}/protos-{v}.tsv", "w") as f:
        f.write("addr\tname\tsource\tunits\tproto\n")
        for n, a in sorted(names.items(), key=lambda x: x[1]):
            for src, c in (("def", defs.get(n)), ("decl", decls.get(n))):
                if c:
                    p, k = c.most_common(1)[0]
                    f.write(f"0x{a:08X}\t{n}\t{src}\t{k}\t{p}\n")
                    rows += 1
                    break
    print(v, "functions", len(names), "with C definition", len(defs), "declared", len(decls), "rows", rows)


if __name__ == "__main__":
    for v in sys.argv[1:] or ("raidou1", "raidou2"):
        main(v)
