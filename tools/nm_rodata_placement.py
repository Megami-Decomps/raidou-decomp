#!/usr/bin/env python3
"""List INCLUDE_RODATA lines inside the asm side (#else) of a NON_MATCHING block
that a -DNON_MATCHING build still needs: no function uses the symbol, a function
other than a draft uses it, or a draft's C names it.

    python3 tools/nm_rodata_placement.py [raidou1 raidou2]

A -DNON_MATCHING build drops those lines, so its .rodata loses data the other
functions need. Exits 1 when any is found.
"""
import re
import sys
from pathlib import Path

from units import read_unit, unit_sources

ROOT = Path(__file__).resolve().parent.parent
NM_BLOCK = re.compile(r"^#ifdef NON_MATCHING\n(.*?)^#else\n(.*?)^#endif", re.M | re.S)
INCLUDE_ASM = re.compile(r'^INCLUDE_ASM\([^,]+,\s*"[^"]+",\s*(\w+)\);$', re.M)
INCLUDE_RODATA = re.compile(r'^INCLUDE_RODATA\([^,]+,\s*"[^"]+",\s*(\w+)\);$', re.M)


def retail_users(version, unit):
    """Symbol -> functions of the unit's disassembly that name it."""
    path = ROOT / "asm" / version / f"{unit}.s"
    users = {}
    if path.exists():
        for f in re.finditer(r"^glabel (\w+)\n(.*?)^endlabel \1", path.read_text(), re.M | re.S):
            for sym in set(re.findall(r"%lo\((\w+)\)", f.group(2))):
                users.setdefault(sym, set()).add(f.group(1))
    return users


def misplaced(version):
    """(unit, symbol, functions of the #else, users that need it) per misplaced line."""
    out = []
    for unit, c in unit_sources(version).items():
        text = read_unit(version, c)
        if "INCLUDE_RODATA" not in text:
            continue
        users = retail_users(version, unit)
        blocks = list(NM_BLOCK.finditer(text))
        c_side = {f: b.group(1) for b in blocks for f in INCLUDE_ASM.findall(b.group(2))}
        for block in blocks:
            funcs = set(INCLUDE_ASM.findall(block.group(2)))
            for sym in INCLUDE_RODATA.findall(block.group(2)):
                need = {f for f in users.get(sym, ()) if f not in c_side or re.search(rf"\b{sym}\b", c_side[f])}
                if need or sym not in users:
                    out.append((unit, sym, funcs, need))
    return out


if __name__ == "__main__":
    total = 0
    for v in sys.argv[1:] or ["raidou1", "raidou2"]:
        found = misplaced(v)
        for unit, sym, funcs, others in found:
            print(f"{v} {unit}: {sym} in #else of {','.join(sorted(funcs))}, used by "
                  f"{','.join(sorted(others)) or 'no function'}")
        print(f"{v}: {len(found)} misplaced INCLUDE_RODATA lines")
        total += len(found)
    sys.exit(1 if total else 0)
