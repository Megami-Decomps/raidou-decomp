#!/usr/bin/env python3
"""Source coverage per version: functions and code bytes written as C.

    python3 tools/progress.py [raidou1 raidou2] [--markdown]

Every retail function of a unit is either C in src/ or an INCLUDE_ASM line.
Sizes come from the split assembly in asm/<version>/ (run configure.py first).
Hand-written assembly (hand/) and the VU microcode are not counted. This is a
source inventory; the retail checksum build and check_unit.py verify matching.

Rows split by unit kind: game (Atlus code) and sdk (Sony libraries and C runtime).
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
ROWS = ("game", "sdk")


def row_for(unit):
    return "sdk" if unit.startswith("sdk/") or "/sdk/" in unit else "game"


def version_stats(version):
    """Row -> [functions, bytes, C functions, C bytes]."""
    sources = unit_sources(version)
    stats = defaultdict(lambda: [0] * 4)
    for asm in sorted((ROOT / "asm" / version).rglob("*.s")):
        rel = asm.relative_to(ROOT / "asm" / version).as_posix()
        if rel.startswith(SKIP):
            continue
        unit = rel[:-2]
        src = sources.get(unit)
        assembled = set(INCLUDE.findall(src.read_text(errors="replace"))) if src else None
        row = stats[row_for(unit)]
        for name, body in GLABEL.findall(asm.read_text()):
            size = 4 * len(INSN.findall(body))
            row[0] += 1
            row[1] += size
            if assembled is not None and name not in assembled:
                row[2] += 1
                row[3] += size
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
        total = [sum(stats[r][i] for r in stats) for i in range(4)]
        rows = [(r, stats[r]) for r in ROWS if r in stats] + [("total", total)]
        if md:
            print(f"\n`{version}`\n\n| Unit kind | Functions in C | Code bytes in C |"
                  "\n|---|---|---|")
            for r, s in rows:
                print(f"| {r} | {s[2]:,} / {s[0]:,} ({pct(s[2], s[0])}) | {s[3]:,} / {s[1]:,} "
                      f"({pct(s[3], s[1])}) |")
        else:
            for r, s in rows:
                print(f"{version} {r}: {s[2]}/{s[0]} functions in C ({pct(s[2], s[0])}), "
                      f"{s[3]}/{s[1]} bytes ({pct(s[3], s[1])})")


if __name__ == "__main__":
    main()
