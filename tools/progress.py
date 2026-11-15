#!/usr/bin/env python3
"""Source coverage: functions and code bytes written as C.

    python3 tools/progress.py [raidou1 raidou2] [--markdown]

Every retail function of a unit is either C in src/ or an INCLUDE_ASM line.
Sizes come from the split assembly in asm/<version>/ (run configure.py first).
Hand-written assembly (hand/) is not counted. VU microcode is a row of its own
(bytes assembled from source, needs the retail image) and stays out of the
totals. This is a source inventory; the retail checksum build and check_unit.py verify matching.

Shared units (src/shared) are counted once: a function counts as C when every
game that has it compiles it from C, and its size is the first game's. Each
game's own units are those in src/<version>; its asm gaps (code no unit claims
yet, with no source) are a row of their own. A game's total is the shared units
as that game builds them plus its own units and gaps. Rows split by unit kind: game (Atlus code) and sdk (Sony libraries and C
runtime).
"""
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from units import VERSIONS, is_shared, unit_sources, vu_source  # noqa: E402
import vu_include  # noqa: E402
from version_guard import view  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
INCLUDE = re.compile(r"^INCLUDE_ASM\([^\n]*\b(\w+)\);", re.M)
GLABEL = re.compile(r"^glabel (\w+)\n(.*?)^endlabel \1", re.M | re.S)
INSN = re.compile(r"^\s*/\* [0-9A-F]+ [0-9A-F]{8} [0-9A-F]{8} \*/", re.M)
SKIP = ("hand/", "data/", "matchings/", "nonmatchings/", "vu/")
KINDS = ("game", "sdk")


def kind_of(unit):
    return "sdk" if unit.startswith("sdk/") or "/sdk/" in unit else "game"


def functions(version):
    """(unit, function) -> (scope, size, in C) for every function a version builds.
    Scope is "shared", "own" (src/<version>) or "gap" (no source)."""
    sources = unit_sources(version)
    out = {}
    for asm in sorted((ROOT / "asm" / version).rglob("*.s")):
        rel = asm.relative_to(ROOT / "asm" / version).as_posix()
        if rel.startswith(SKIP):
            continue
        unit = rel[:-2]
        src = sources.get(unit)
        assembled = set(INCLUDE.findall(view(src.read_text(errors="replace"), version))) if src else None
        scope = "gap" if src is None else "shared" if is_shared(src) else "own"
        for name, body in GLABEL.findall(asm.read_text()):
            out[unit, name] = (scope, 4 * len(INSN.findall(body)), assembled is not None and name not in assembled)
    return out


def tally(items):
    """Kind -> [functions, bytes, C functions, C bytes] of (unit, size, in C) items."""
    stats = defaultdict(lambda: [0] * 4)
    for unit, size, c in items:
        row = stats[kind_of(unit)]
        row[0] += 1
        row[1] += size
        if c:
            row[2] += 1
            row[3] += size
    return stats


def rows(label, stats):
    out = [(label, k, stats[k]) for k in KINDS if k in stats]
    if len(out) > 1:
        out.append((label, "all", [sum(stats[k][i] for k in stats) for i in range(4)]))
    return out


def vu_stats(version):
    """[None, microcode bytes, None, bytes from source] of a version's .vutext, or None."""
    try:
        vu = vu_include.coverage(version, vu_include.retail_elf(version))
    except FileNotFoundError:
        return None
    return [None, vu["microcode_bytes"], None, vu["source_bytes"]]


def vu_shared(version):
    paths = [vu_source(version, name) for name in vu_include.SOURCES.values()]
    return all(paths) and all(is_shared(p) for p in paths)


def count(n):
    return "-" if n is None else f"{n:,}"


def pct(a, b):
    return f"{100 * a / b:.1f}%" if a is not None and b else "n/a"


def main():
    md = "--markdown" in sys.argv
    versions = [a for a in sys.argv[1:] if not a.startswith("--")] or list(VERSIONS)
    funcs = {}
    for version in versions:
        if not (ROOT / "asm" / version).is_dir():
            print(f"{version}: no split assembly (run configure.py)", file=sys.stderr)
            continue
        funcs[version] = functions(version)
    shared = {}
    for version, fs in funcs.items():
        for key, (scope, size, c) in fs.items():
            if scope == "shared":
                old = shared.get(key)
                shared[key] = (old[0], old[1] and c) if old else (size, c)
    table = rows("shared", tally((u, s, c) for (u, _), (s, c) in shared.items()))
    vu_done = False
    for version in funcs:
        if vu_shared(version) and not vu_done and (vu := vu_stats(version)):
            table.append(("shared", "vu", vu))
            vu_done = True
    for version, fs in funcs.items():
        for scope, label in (("own", "only"), ("gap", "asm gaps")):
            table += rows(f"{version} {label}", tally((u, s, c) for (u, _), (sc, s, c) in fs.items() if sc == scope))
        total = tally((u, s, c) for (u, _), (_, s, c) in fs.items())
        if not vu_shared(version) and (vu := vu_stats(version)):
            table.append((f"{version} only", "vu", vu))
        table.append((f"{version} total", "all", [sum(total[k][i] for k in total) for i in range(4)]))
    if md:
        print("| Units | Kind | Functions in C | Code bytes in C |\n|---|---|---|---|")
        for label, kind, s in table:
            print(f"| {label} | {kind} | {count(s[2])} / {count(s[0])} ({pct(s[2], s[0])}) | {s[3]:,} / {s[1]:,} "
                  f"({pct(s[3], s[1])}) |")
    else:
        for label, kind, s in table:
            print(f"{label} {kind}: {count(s[2])}/{count(s[0])} functions in C ({pct(s[2], s[0])}), "
                  f"{s[3]}/{s[1]} bytes ({pct(s[3], s[1])})")


if __name__ == "__main__":
    main()
