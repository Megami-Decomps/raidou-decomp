"""Where each C translation unit's source lives.

A TU both games build the same way is one file, src/shared/<unit>.c, compiled into
each version; a TU that differs has a file per game, src/<version>/<unit>.c. The
per-game file wins when both exist.
"""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = json.loads((ROOT / "config" / "versions.json").read_text())
SRC = ROOT / "src"
SHARED = SRC / "shared"


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


def write_unit(version, path, text):
    """Write a version's text for a unit. A shared TU that the second version would
    change no longer builds the same in both games: each game gets its own file."""
    path = Path(path)
    if text == path.read_text():
        return
    if not is_shared(path) or version == next(iter(VERSIONS)):
        path.write_text(text)
        return
    unit = unit_name(path)
    for v in VERSIONS:
        own = SRC / v / f"{unit}.c"
        own.parent.mkdir(parents=True, exist_ok=True)
        own.write_text(text if v == version else path.read_text())
    path.unlink()


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
