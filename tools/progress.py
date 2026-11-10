#!/usr/bin/env python3
"""Source coverage per version: functions and code bytes written as C.

    python3 tools/progress.py [raidou1 raidou2] [--markdown]

Every retail function of a unit is either C in src/ or an INCLUDE_ASM line.
Sizes come from the split assembly in asm/<version>/ (run configure.py first).
Hand-written assembly (hand/) and the VU microcode are not counted. This is a
source inventory; the retail checksum build and check_unit.py verify matching.

Rows split by unit kind (game or SDK) and, for game units, by what the unit
was found in: an SMT3 unit, a DDS unit, or neither (config/<v>/tu_evidence.tsv).
Columns split C functions by the source of their body: carried from SMT3 or DDS
(config/<v>/smt3_c.tsv, with the provenance of the name in name_sources.txt), or
written here (functions the tables do not list, such as empty ones).
"""
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from units import VERSIONS, unit_sources  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
INCLUDE = re.compile(r"^INCLUDE_ASM\([^\n]*\b(\w+)\);", re.M)
GLABEL = re.compile(r"^glabel (\w+)\n(.*?)^endlabel \1", re.M | re.S)
INSN = re.compile(r"^\s*/\* [0-9A-F]+ [0-9A-F]{8} [0-9A-F]{8} \*/", re.M)
SKIP = ("hand/", "data/", "matchings/", "nonmatchings/", "vu/")
ROWS = ("game: from SMT3 units", "game: from DDS units", "game: Raidou only", "sdk")


def unit_origins(version):
    """Unit name -> first evidence source (smt3, dds, sdk, none)."""
    out = {}
    for line in (ROOT / "config" / version / "tu_evidence.tsv").read_text().splitlines():
        if line.startswith("#") or line.startswith("start\t"):
            continue
        cols = line.split("\t")
        out[cols[2]] = cols[4].split(":")[0]
    return out


def body_sources(version):
    """Function -> smt3 or dds for each body carried from them (smt3_c.tsv)."""
    carried = set()
    for line in (ROOT / "config" / version / "smt3_c.tsv").read_text().splitlines():
        cols = line.split("\t")
        if not line.startswith("#") and cols[1:2] == ["c"]:
            carried.add(cols[0])
    out = {}
    for line in (ROOT / "config" / version / "name_sources.txt").read_text().splitlines():
        if line.startswith("#"):
            continue
        cols = line.split("\t")
        if len(cols) > 3:
            out[cols[1]] = "dds" if cols[3].startswith("dds-decomp") else "smt3"
    return {name: out.get(name, "dds") for name in carried}


def row_for(unit, origin):
    if unit.startswith("sdk/") or origin == "sdk":
        return "sdk"
    return {"smt3": ROWS[0], "dds": ROWS[1]}.get(origin, ROWS[2])


def version_stats(version):
    """Row -> [functions, bytes, C functions, C bytes, C from SMT3, from DDS, written here]."""
    sources = unit_sources(version)
    origins = unit_origins(version)
    bodies = body_sources(version)
    stats = defaultdict(lambda: [0] * 7)
    for asm in sorted((ROOT / "asm" / version).rglob("*.s")):
        rel = asm.relative_to(ROOT / "asm" / version).as_posix()
        if rel.startswith(SKIP):
            continue
        unit = rel[:-2]
        src = sources.get(unit)
        assembled = set(INCLUDE.findall(src.read_text(errors="replace"))) if src else None
        row = stats[row_for(unit, origins.get(unit))]
        for name, body in GLABEL.findall(asm.read_text()):
            size = 4 * len(INSN.findall(body))
            row[0] += 1
            row[1] += size
            if assembled is not None and name not in assembled:
                row[2] += 1
                row[3] += size
                row[{"smt3": 4, "dds": 5}.get(bodies.get(name), 6)] += 1
    return stats


def pct(a, b):
    return f"{100 * a / b:.1f}%" if b else "n/a"


def main():
    md = "--markdown" in sys.argv
    versions = [a for a in sys.argv[1:] if not a.startswith("--")] or list(VERSIONS)
    for version in versions:
        if not (ROOT / "asm" / version).is_dir():
            print(f"{version}: no split assembly (run configure.py)", file=sys.stderr)
            continue
        stats = version_stats(version)
        total = [sum(stats[r][i] for r in stats) for i in range(7)]
        rows = [(r, stats[r]) for r in ROWS if r in stats] + [("total", total)]
        if md:
            print(f"\n`{version}`\n\n| Unit kind | Functions in C | Code bytes in C | C from SMT3 | C from DDS | C written here |"
                  "\n|---|---|---|---|---|")
            for r, s in rows:
                print(f"| {r} | {s[2]:,} / {s[0]:,} ({pct(s[2], s[0])}) | {s[3]:,} / {s[1]:,} "
                      f"({pct(s[3], s[1])}) | {s[4]:,} | {s[5]:,} | {s[6]:,} |")
        else:
            for r, s in rows:
                print(f"{version} {r}: {s[2]}/{s[0]} functions in C ({pct(s[2], s[0])}), "
                      f"{s[3]}/{s[1]} bytes ({pct(s[3], s[1])}); from SMT3 {s[4]}, from DDS {s[5]}, written here {s[6]}")


if __name__ == "__main__":
    main()
