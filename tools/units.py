"""Where each C translation unit's source lives.

A TU both games build the same way is one file, src/shared/<unit>.c, compiled into
each version; a TU that differs has a file per game, src/<version>/<unit>.c. The
per-game file wins when both exist.
"""
import json
import re
from pathlib import Path

try:
    from version_guard import merge, view
except ImportError:
    from tools.version_guard import merge, view

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = json.loads((ROOT / "config" / "versions.json").read_text())
SRC = ROOT / "src"
SHARED = SRC / "shared"
SYMBOL_BLOCK = re.compile(r"(?:\.align \d+\n)?nonmatching (\w+).*?enddlabel \1\n", re.S)


def shared_sources():
    """Unit name -> path of every shared TU."""
    out = {}
    for path in sorted(SHARED.rglob("*.c")):
        out[path.relative_to(SHARED).with_suffix("").as_posix()] = path
    return out


def unit_sources(version):
    """Unit name -> path of the source a version compiles, per-game file first."""
    out = shared_sources()
    for path in sorted((SRC / version).rglob("*.c")):
        out[path.relative_to(SRC / version).with_suffix("").as_posix()] = path
    return dict(sorted(out.items()))


def vu_source(version, name):
    """Path, relative to the repository, of the VU listing a version assembles, or None."""
    for folder in (version, "shared"):
        path = Path("src") / folder / "sdf" / "vu" / f"{name}.vsm"
        if (ROOT / path).exists():
            return path
    return None


def unit_name(path):
    """Unit name of a source path (absolute, or relative to the repository)."""
    path = Path(path)
    rel = (path if not path.is_absolute() else path.relative_to(ROOT)).relative_to("src")
    if rel.parts[0] in VERSIONS or rel.parts[0] == "shared":
        rel = rel.relative_to(rel.parts[0])
    return rel.with_suffix("").as_posix()


def is_shared(path):
    path = Path(path)
    rel = (path if not path.is_absolute() else path.relative_to(ROOT)).relative_to("src")
    return rel.parts[0] == "shared"


def read_unit(version, path):
    """The text a version compiles from a unit's source, without its VERSION_ guards."""
    return view(Path(path).read_text(), version)


def write_unit(version, path, text):
    """Write a version's view of a unit. A shared TU keeps the other version's view and
    puts the lines that differ under VERSION_ guards."""
    path = Path(path)
    current = path.read_text()
    if text == view(current, version):
        return
    if not is_shared(path):
        path.write_text(text)
        return
    views = {v: text if v == version else view(current, v) for v in VERSIONS}
    path.write_text(merge(views))


def carried_data(version):
    """Name -> address of SMT3 data definitions carried into C (config/<v>/smt3_data.tsv)."""
    path = ROOT / "config" / version / "smt3_data.tsv"
    out = {}
    if path.exists():
        for line in path.read_text().splitlines():
            row = line.split("\t")
            if line and not line.startswith("#") and len(row) >= 5 and row[4] == "c":
                out[row[0]] = int(row[1], 16)
    return out


def function_rodata(version):
    """Unit -> [(function, rodata symbols splat migrated into it)], in address order, from
    asm/<v>/function_rodata.tsv (tools/splat_split.py). Unmigrated rodata has function ""."""
    out = {}
    path = ROOT / "asm" / version / "function_rodata.tsv"
    for line in path.read_text().splitlines() if path.exists() else ():
        unit, func, syms = line.split("\t")
        out.setdefault(unit, []).append((func, syms.split()))
    return out


def rodata_blocks(version, unit):
    """Rodata symbol -> its block (`nonmatching` to `enddlabel`) in asm/<v>/data/<unit>.rodata.s."""
    path = ROOT / "asm" / version / "data" / f"{unit}.rodata.s"
    text = path.read_text() if path.exists() else ""
    return {m.group(1): m.group(0) for m in SYMBOL_BLOCK.finditer(text)}
