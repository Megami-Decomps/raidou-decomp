#!/usr/bin/env python3
"""Check the committed tables and source layout. Needs no retail executable.

    python3 tools/check_config.py
"""
import re
import sys
from pathlib import Path

import yaml

sys.path.insert(0, str(Path(__file__).resolve().parent))
from units import SHARED, SRC, VERSIONS  # noqa: E402
from xgame.common_names import check as check_common_names  # noqa: E402

ROOT = SRC.parent
errors = []


def table(path):
    return [l.split("\t") for l in path.read_text().splitlines() if l and not l.startswith("#")]


def duplicates(values):
    seen, dup = set(), set()
    for v in values:
        (dup if v in seen else seen).add(v)
    return sorted(dup)


for version, spec in VERSIONS.items():
    cfg = ROOT / "config" / version
    if not re.fullmatch(r"[0-9a-f]{40}", spec["elf_sha1"]):
        errors.append(f"{version}: elf_sha1 is not a SHA-1")
    yaml.safe_load((cfg / f"{spec['serial']}.yaml").read_text())
    for name in ("smt3_c.tsv", "tu_evidence.tsv", "name_sources.txt", "symbol_addrs.txt"):
        if not (cfg / name).is_file():
            errors.append(f"{version}: config/{version}/{name} missing")
            continue
    carried = table(cfg / "smt3_c.tsv")
    errors += [f"{version}: smt3_c.tsv has {n} twice" for n in duplicates(r[0] for r in carried)]
    errors += [f"{version}: smt3_c.tsv status {r[1]!r} for {r[0]}" for r in carried if r[1] not in ("c", "asm")]
    rows = [r for r in table(cfg / "tu_evidence.tsv") if r[0] != "start"]
    errors += [f"{version}: tu_evidence.tsv has unit {n} twice" for n in duplicates(r[2] for r in rows)]
    starts = [int(r[0], 16) for r in rows]
    if starts != sorted(starts):
        errors.append(f"{version}: tu_evidence.tsv is not in address order")
    names = table(cfg / "name_sources.txt")
    errors += [f"{version}: name_sources.txt has address {a} twice" for a in duplicates(r[0] for r in names)]
    own = {p.relative_to(SRC / version).with_suffix("").as_posix() for p in (SRC / version).rglob("*.c")}
    shared = {p.relative_to(SHARED).with_suffix("").as_posix() for p in SHARED.rglob("*.c")}
    errors += [f"{version}: {u} is both shared and per-game" for u in sorted(own & shared)]

errors += check_common_names()

if errors:
    print("\n".join(errors), file=sys.stderr)
    sys.exit(1)
print(f"checked {', '.join(VERSIONS)}")
