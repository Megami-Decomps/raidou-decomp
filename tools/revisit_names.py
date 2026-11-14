#!/usr/bin/env python3
"""List the names to revisit: tier B, C and D names from name_sources.txt, worst tier first.

    python3 tools/revisit_names.py [--check]

Writes config/<version>/revisit_names.tsv; --check fails when a file is stale. Names from older passes with
no tier are tier D (unmeasured) when their source is a DDS-only, fuzzy or low-confidence HD name."""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
VERSIONS = ("raidou1", "raidou2")
HEAD = "# addr\tname\tkind\ttier\tmethod\tp\tevidence\n"
ROW = re.compile(r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;(.*)$")
FIELD = re.compile(r"(\w+)=(.*?)(?:; (?=\w+=)|$)")
LEGACY = (("dds-decomp", "legacy-dds"), ("fuzzy name", "legacy-fuzzy"))


def classify(kind, note):
    """(tier, method, p, evidence) or None for names that need no review"""
    f = dict(FIELD.findall(note))
    if "tier" in f:
        if f["tier"] == "A":
            return None
        return f["tier"], f.get("method", ""), f.get("p", "na"), f.get("evidence", "")
    for start, method in LEGACY:
        if note.startswith(start):
            return "D", method, "na", note
    if kind == "hd-inferred" or (kind == "hd" and re.search(r"\b(low|medium) confidence", note)):
        return "D", f"legacy-{kind}", "na", note
    return None


def build(version):
    cfg = ROOT / "config" / version
    kinds = {}
    for line in (cfg / "symbol_addrs.txt").read_text().splitlines():
        if m := ROW.match(line):
            kinds.setdefault(int(m[2], 16), "func" if "type:func" in m[3] else "data")
    rows = []
    for line in (cfg / "name_sources.txt").read_text().splitlines():
        c = line.split("\t")
        if line.startswith("#") or len(c) < 4:
            continue
        r = classify(c[2], c[3])
        if r:
            rows.append((r[0], int(c[0], 16), c[1], kinds.get(int(c[0], 16), "data"), *r[1:]))
    rows.sort(key=lambda r: ({"D": 0, "C": 1, "B": 2}.get(r[0], 0), r[1]))
    return HEAD + "".join(f"0x{a:08X}\t{n}\t{k}\ttier={t}\t{m}\t{p}\t{e}\n" for t, a, n, k, m, p, e in rows)


def main():
    check = "--check" in sys.argv
    stale = []
    for v in VERSIONS:
        path = ROOT / "config" / v / "revisit_names.tsv"
        text = build(v)
        if check:
            if not path.is_file() or path.read_text() != text:
                stale.append(f"{v}: config/{v}/revisit_names.tsv is stale; run tools/revisit_names.py")
        else:
            path.write_text(text)
            print(v, text.count("\n") - 1, "names")
    if stale:
        print("\n".join(stale), file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
