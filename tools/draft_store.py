#!/usr/bin/env python3
"""Drafts of Raidou functions kept outside any batch, one file per function, for find_body.py to pick up.

Usage: draft_store.py save NAME DRAFT.c --exemplar CORPUS:NAME [--decls FILE] [--cluster C0014]
       draft_store.py list [--cluster C0014] | check
"""
from __future__ import annotations

import argparse
import datetime
import hashlib
import json
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import body_check  # noqa: E402
from units import VERSIONS, unit_sources  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
STORE = Path.home() / ".local/state/raidou-decomp-work/clusters/store"
STATUS = re.compile(r"^(match|nocompile|words \d+)$")
# find_body ranks a draft by how its status compares with a tree definition's guard.
GAP_HEAD = '#include "common.h"\n\n'
GUARD = {"match": "matched", "words": "nonmatching", "nocompile": "disabled"}
EPILOG = ("The store is STORE/<version>/<ADDR>.c (the definition alone) and <ADDR>.json: name, addr, version, "
          "words, asm_hash (blake2b-8 of the retail bytes, as similar_index.py hashes them), exemplar, cluster, "
          "status (match, words N, nocompile), decls (declarations the draft assumes, spliced in before it) and "
          "date. `save` reads the address and size from build/<version>/<serial>.elf and measures the status with "
          "check_unit.py in the function's unit, or alone after common.h when the unit has no .c yet (a gap unit). "
          "`check` lists entries whose bytes, size or name no longer agree with the build.")


@dataclass
class Entry:
    name: str
    addr: int
    version: str
    words: int
    asm_hash: str
    exemplar: str
    status: str
    decls: list[str] = field(default_factory=list)
    cluster: str = ""
    unit: str = ""
    date: str = ""
    body: str = ""

    def meta(self) -> dict:
        return {"name": self.name, "addr": f"{self.addr:08X}", "version": self.version, "words": self.words,
                "asm_hash": self.asm_hash, "exemplar": self.exemplar, "cluster": self.cluster, "unit": self.unit,
                "status": self.status, "decls": self.decls, "date": self.date}

    def guard(self) -> str:
        return GUARD[self.status.split()[0]]

    def text(self) -> str:
        """What a splice puts at the INCLUDE_ASM: the assumed declarations, then the definition."""
        return "".join(d.rstrip("\n") + "\n" for d in self.decls) + self.body


def hash_code(code: bytes) -> str:
    return hashlib.blake2b(code, digest_size=8).hexdigest()


def load_image(version: str, root: Path = ROOT) -> body_check.Image:
    path = root / "build" / version / f"{VERSIONS[version]['serial']}.elf"
    if not path.exists():
        raise SystemExit(f"{path}: not built; run configure.py and ninja")
    return body_check.Image(path)


def get_paths(store: Path, version: str, addr: int) -> tuple[Path, Path]:
    base = store / version / f"{addr:08X}"
    return base.with_suffix(".c"), base.with_suffix(".json")


def load(store: Path = STORE, version: str = "raidou1") -> dict[int, Entry]:
    """Every entry of one version by address."""
    out = {}
    for meta_path in sorted((store / version).glob("*.json")):
        m = json.loads(meta_path.read_text())
        c = meta_path.with_suffix(".c")
        out[int(m["addr"], 16)] = Entry(m["name"], int(m["addr"], 16), m["version"], m["words"], m["asm_hash"],
                                        m["exemplar"], m["status"], m.get("decls", []), m.get("cluster", ""),
                                        m.get("unit", ""), m.get("date", ""),
                                        c.read_text() if c.exists() else "")
    return out


def find_problem(e: Entry, img: body_check.Image, words: int | None = None, name: str | None = None) -> str | None:
    """Why an entry cannot stand for the function at its address now, or None."""
    names = img.names.get(e.addr, set())
    if name is not None and name not in names and name != e.name:
        return f"{name} is not at {e.addr:08X}"
    current = next((n for n in sorted(names) if n in img.funcs), None)
    if current is None:
        return f"no function at {e.addr:08X}"
    size = img.size_of(current)
    if size != e.words * 4:
        return f"size {size // 4} words, draft for {e.words}"
    if words and words != e.words:
        return f"row has {words} words, draft for {e.words}"
    if hash_code(img.code(e.addr, size)) != e.asm_hash:
        return "retail bytes differ (stale)"
    if not e.body or get_body_name(e.body) != e.name:
        return f"draft defines {get_body_name(e.body)}, not {e.name}"
    return None


def get_body_name(body: str) -> str | None:
    m = re.search(r"^[A-Za-z_][^;{}#\n]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{", body, re.M)
    return m.group(1) if m else None


def get_status(tried: body_check.Tried) -> str | None:
    if tried.result == "ok":
        return "match"
    if tried.result.startswith("diff"):
        m = re.match(r"diff (\d+)", tried.result)
        return f"words {m.group(1)}" if m else "words 0"
    if tried.result in ("fail", "other"):
        return "nocompile"
    return None


def find_unit(version: str, addr: int, root: Path = ROOT) -> str:
    """The unit under root/src (without .c) that holds the function at addr, else its asm/ unit."""
    import similar_index
    for f in similar_index.inventory(version):
        if f["addr"] == addr:
            for folder in (version, "shared"):
                if (root / "src" / folder / f"{f['unit']}.c").exists():
                    return f"src/{folder}/{f['unit']}"
            return f"asm/{version}/{f['unit']}"
    return ""


def save(a) -> None:
    img = load_image(a.version, a.root)
    if a.name not in img.funcs:
        raise SystemExit(f"{a.name}: not a function of build/{a.version}")
    body = a.draft.read_text()
    if get_body_name(body) != a.name:
        raise SystemExit(f"{a.draft}: defines {get_body_name(body)}, not {a.name}")
    addr = img.funcs[a.name][0]
    size = img.size_of(a.name)
    decls = [d for d in a.decls.read_text().splitlines() if d.strip()] if a.decls else []
    unit = a.unit or find_unit(a.version, addr, a.root)
    e = Entry(a.name, addr, a.version, size // 4, hash_code(img.code(addr, size)), a.exemplar, "", decls,
              a.cluster, unit, datetime.date.today().isoformat(), body if body.endswith("\n") else body + "\n")
    if unit.startswith("asm/"):
        gap = a.root / "src" / a.version / f"{unit.split('/', 2)[2]}.c"
        tried = body_check.check_source(a.root, sys.executable, gap, GAP_HEAD + e.text(), a.name, a.version)
    else:
        tried = body_check.try_compile(a.root, sys.executable, unit, a.name, e.text(), a.version) if unit else None
    measured = get_status(tried) if tried else None
    if measured and a.status and a.status != measured:
        raise SystemExit(f"{a.name}: check_unit says {measured}, not {a.status}")
    e.status = measured or a.status or ""
    if not STATUS.match(e.status):
        raise SystemExit(f"{a.name}: {tried.result if tried else 'no unit'}; pass --status match|nocompile|'words N'")
    c, meta = get_paths(a.store, a.version, addr)
    c.parent.mkdir(parents=True, exist_ok=True)
    c.write_text(e.body)
    meta.write_text(json.dumps(e.meta(), indent=1) + "\n")
    print(f"{a.name} {addr:08X} {e.words}w {e.status} ({'measured' if measured else 'stated'}) -> {c}")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0], epilog=EPILOG)
    ap.add_argument("--store", type=Path, default=STORE)
    ap.add_argument("--version", default="raidou1", choices=list(VERSIONS))
    sub = ap.add_subparsers(dest="cmd", required=True)
    s = sub.add_parser("save")
    s.add_argument("name")
    s.add_argument("draft", type=Path)
    s.add_argument("--exemplar", required=True, help="CORPUS:NAME the draft was adapted from, or 'fresh'")
    s.add_argument("--decls", type=Path, help="file of declarations the draft assumes, one per line")
    s.add_argument("--cluster", default="")
    s.add_argument("--unit", help="src/... unit to measure in (default: the one holding its INCLUDE_ASM)")
    s.add_argument("--root", type=Path, default=ROOT, help="tree to measure in (its build/ names the function)")
    s.add_argument("--status", help="only when check_unit cannot say: match, nocompile or 'words N'")
    lp = sub.add_parser("list")
    lp.add_argument("--cluster")
    sub.add_parser("check")
    a = ap.parse_args()
    if a.cmd == "save":
        save(a)
        return
    entries = load(a.store, a.version)
    if a.cmd == "list":
        for e in entries.values():
            if not a.cluster or e.cluster == a.cluster:
                print(f"{e.addr:08X}\t{e.name}\t{e.words}\t{e.status}\t{e.cluster}\t{e.exemplar}\t{e.date}")
        return
    img, bad = load_image(a.version), 0
    for e in entries.values():
        why = find_problem(e, img)
        names = img.names.get(e.addr, set())
        if why is None and e.name not in names:
            why = f"renamed to {', '.join(sorted(names))}"
        if why:
            bad += 1
            print(f"{e.addr:08X}\t{e.name}\t{why}")
    print(f"{len(entries)} entries, {bad} with a problem", file=sys.stderr)
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
