#!/usr/bin/env python3
"""Global variable declarations raidou-decomp gives data the linked ELF places (names in
build/<v>/<elf>.map, so a declaration nothing links is left out). Writes globals-<v>.tsv:
addr, name, units, decl."""
import os, re, sys
from collections import Counter
sys.path.insert(0, os.path.dirname(__file__))
from extract_types import RD, OUT, preprocess, statements, strip_attr, norm

IDB = os.path.dirname(OUT)


def var(s):
    t = strip_attr(s).strip()
    if re.match(r"(typedef|struct\s+\w+\s*\{|union\s+\w+\s*\{|enum\b)", t) or t.startswith("asm"):
        return None
    t = re.sub(r"^\s*(extern|static|const|volatile)\s+", lambda m: m.group(1) + " ", t)
    head = t.split("=")[0].strip().rstrip(";").strip()
    if "(" in head and not re.search(r"\(\s*\*", head):
        return None  # function declaration
    m = re.search(r"\(\s*\*\s*(\w+)\s*\)", head) or re.search(r"(\w+)\s*(\[[^\]]*\]\s*)*$", head)
    if not m or "," in head.split("[")[0]:
        return None
    decl = re.sub(r"^(extern|static)\s+", "", head)
    return m.group(1), norm(decl) + ";"


def main(v):
    addrs = {}
    mp = [f for f in os.listdir(f"{RD}/build/{v}") if f.endswith(".map")][0]
    for line in open(f"{RD}/build/{v}/{mp}"):
        m = re.match(r"\s+0x([0-9a-f]+)\s+(\w+)(\s*=.*)?$", line)
        if m:
            addrs[m.group(2)] = int(m.group(1), 16)
    funcs = {m.group(1) for m in re.finditer(r"^(\w+) = 0x[0-9A-Fa-f]+;.*type:func", open(f"{RD}/config/{v}/symbol_addrs.txt").read(), re.M)}
    units = sorted(os.path.join(d, f) for top in (f"{RD}/src/{v}", f"{RD}/src/shared")
                   for d, _, fs in os.walk(top) for f in fs if f.endswith(".c"))
    borrow = {u for u in units if re.search(r"^\s*#\s*define\s+VERSION_DDS", open(u).read(), re.M)}
    decls = {}
    for u in units:
        text = None
        for raidou in ((True, False) if u in borrow else (True,)):
            text, _ = preprocess(u, v, raidou)
            if text is not None:
                break
        if text is None:
            continue
        for _, s in statements(text):
            r = var(s)
            if not r or r[0] in funcs:
                continue
            name, decl = r
            if name in addrs:
                decls.setdefault(name, Counter())[decl] += 1
    with open(f"{IDB}/globals-{v}.tsv", "w") as f:
        f.write("addr\tname\tunits\tdecl\n")
        for name, c in sorted(decls.items(), key=lambda x: addrs[x[0]]):
            a = addrs[name]
            d, k = c.most_common(1)[0]
            f.write(f"0x{a:08X}\t{name}\t{k}\t{d}\n")
    print(v, "globals", len(decls))


if __name__ == "__main__":
    for v in sys.argv[1:] or ("raidou1", "raidou2"):
        main(v)
