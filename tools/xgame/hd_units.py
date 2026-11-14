#!/usr/bin/env python3
"""Name placeholder code_<vram> units after Raidou HD modules.

    python3 tools/xgame/hd_units.py TUS_TSV [--dry-run]

Takes the proposal column of tus.tsv. The Raidou 2 unit is the one most of the unit's
paired functions sit in (r1_r2_pairs.tsv), and takes the same name."""
from __future__ import annotations

import argparse
import re
import subprocess
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CODE = re.compile(r"code_[0-9A-F]{8}$")
PAIRS = ROOT / "config/xgame/r1_r2_pairs.tsv"
YAML = {"raidou1": "SLUS_214.31.yaml", "raidou2": "SLUS_218.45.yaml"}


def unit_of(path):
    stem = Path(path).stem
    return stem if CODE.match(stem) else None


def sub_unit(text, old, new):
    return re.sub(rf"\b{old}\b(?!\w)", new, text)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("tus")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args()
    lines = PAIRS.read_text().splitlines()
    head, body = lines[0], [l.split("\t") for l in lines[1:]]
    cfg_text = "".join((ROOT / "config" / v / YAML[v]).read_text() for v in YAML)
    existing = {n.split("/")[-1] for n in re.findall(r"\[0x[0-9A-Fa-f]+, \w+, ([^\]]+)\]", cfg_text)}
    plan = []
    used2, used_names = set(), set()
    for line in Path(a.tus).read_text().splitlines():
        c = line.split("\t")
        if line.startswith("#") or len(c) < 9 or not c[8]:
            continue
        u1, new = c[0], c[8]
        if new in existing or new in used_names:
            print(f"skip {u1}: unit {new} exists")
            continue
        share = Counter(unit_of(r[6]) for r in body if unit_of(r[5]) == u1)
        share.pop(None, None)
        u2 = share.most_common(1)[0][0] if share else None
        if u2 in used2:
            u2 = None
        used2.add(u2)
        used_names.add(new)
        plan.append((u1, u2, new))
    print(f"{len(plan)} units,", sum(1 for p in plan if p[1]), "with a raidou2 unit")
    if a.dry_run:
        for p in plan:
            print(*p)
        return
    for v, idx in (("raidou1", 0), ("raidou2", 1)):
        for p in plan:
            old, new = p[idx], p[2]
            if old is None:
                continue
            for f in [ROOT / "config" / v / YAML[v], ROOT / "config" / v / "tu_evidence.tsv"] + \
                    sorted((ROOT / "src" / v).rglob("*.[ch]")):
                t = f.read_text()
                n = sub_unit(t, old, new)
                if n != t:
                    f.write_text(n)
            src = ROOT / "src" / v / f"{old}.c"
            if src.exists():
                subprocess.run(["git", "mv", str(src), str(src.with_name(f"{new}.c"))], check=True)
    for r in body:
        for p in plan:
            if unit_of(r[5]) == p[0]:
                r[5] = r[5].replace(p[0], p[2])
            if p[1] and unit_of(r[6]) == p[1]:
                r[6] = r[6].replace(p[1], p[2])
    PAIRS.write_text(head + "\n" + "\n".join("\t".join(r) for r in body) + "\n")


main()
