"""Where each C translation unit's source lives.

A TU both games build the same way is one file, src/<unit>.c, compiled into each
version; a TU that differs has a file per game, src/<version>/<unit>.c. The
per-game file wins when both exist.
"""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = json.loads((ROOT / "config" / "versions.json").read_text())
SRC = ROOT / "src"


def shared_sources():
    """Unit name -> path of every shared TU."""
    out = {}
    for path in sorted(SRC.rglob("*.c")):
        rel = path.relative_to(SRC)
        if rel.parts[0] not in VERSIONS:
            out[rel.with_suffix("").as_posix()] = path
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
    if rel.parts[0] in VERSIONS:
        rel = rel.relative_to(rel.parts[0])
    return rel.with_suffix("").as_posix()


def is_shared(path):
    path = Path(path)
    rel = (path if not path.is_absolute() else path.relative_to(ROOT)).relative_to("src")
    return rel.parts[0] not in VERSIONS


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
