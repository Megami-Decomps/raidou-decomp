#!/usr/bin/env python3
"""Put the declarations a draft store entry assumes (D/NAME.decls) into headers of the drafting tree, never the .c.

    python3 tools/batch/place_decls.py D --tree T [--batches batches.tsv]
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import batch_files  # noqa: E402
import decls  # noqa: E402

EPILOG = ("Each declaration ends in one of these actions, written to D/store_decls.tsv (name, action, where, decl): "
          "present (include/ already declares it the same way and the unit sees it), include (include/ declares it "
          "in a header the unit does not reach; that header is added to the unit's includes), placed (added to the "
          "header that declares the symbols next to it by address, when that header already sees every type the "
          "declaration uses, else to the unit's own include/<unit>.h, created when missing), differs (include/ "
          "declares it another way; the header wins, review the draft against it) and required (no header can "
          "take it: a missing header or a static forward declaration; a header edit for the handoff). An include "
          "the unit needs goes into T's .c after its last #include and into D/includes.tsv (source, header), which "
          "land_batch applies; for a gap unit it goes into D/units/UNIT.pre.")
NEIGHBOURS = 16


class Headers:
    """include/*.h of a tree: what each declares and includes, edited in memory and written back at the end."""

    def __init__(self, tree: Path):
        self.tree = tree
        self.text: dict[str, str] = {}
        self.decls: dict[str, list[decls.Decl]] = {}
        self.changed: set[str] = set()
        self.symbols: dict[str, list[str]] = {}
        self.types: dict[str, list[str]] = {}
        for path in sorted((tree / "include").glob("*.h")):
            self.load(path.name, path.read_text(errors="replace"))

    def load(self, name: str, text: str) -> None:
        self.text[name] = text
        self.decls[name] = decls.parse(text)
        for index in (self.symbols, self.types):
            for key in [k for k, hs in index.items() if name in hs]:
                index[key] = [x for x in index[key] if x != name]
        for d in self.decls[name]:
            if d.kind in ("proto", "extern"):
                self.symbols.setdefault(d.name, []).append(name)
            for t in decls.defines(d):
                self.types.setdefault(t, []).append(name)

    def exists(self, name: str) -> bool:
        return name in self.text or (self.tree / "src" / name).exists()

    def includes(self, name: str) -> list[str]:
        return [d.name for d in self.decls.get(name, ()) if d.kind == "include"]

    def closure(self, roots: list[str]) -> list[str]:
        seen, todo = [], list(roots)
        while todo:
            h = todo.pop(0)
            if h in seen:
                continue
            seen.append(h)
            todo += self.includes(h)
        return seen

    def declared(self, name: str) -> list[tuple[str, decls.Decl]]:
        return [(h, d) for h in dict.fromkeys(self.symbols.get(name, ())) for d in self.decls[h]
                if d.name == name and d.kind in ("proto", "extern")]

    def type_home(self, name: str) -> str | None:
        homes = self.types.get(name)
        return homes[0] if homes else None

    def visible(self, roots: list[str]) -> set[str]:
        out = set()
        for h in self.closure(roots):
            for d in self.decls.get(h, ()):
                out |= decls.defines(d)
        return out

    def needed_types(self, d: decls.Decl) -> set[str]:
        """Type names the declaration uses that some header defines."""
        return {t for t in decls.type_names(d) if self.type_home(t)}

    def create(self, name: str) -> None:
        guard = re.sub(r"\W", "_", name).upper()
        self.load(name, f"#ifndef {guard}\n#define {guard}\n\n#include \"common.h\"\n\n#endif\n")
        self.changed.add(name)

    def add(self, name: str, line: str) -> None:
        """Add a line before the include guard's #endif (an #include after the last #include)."""
        if name not in self.text:
            self.create(name)
        text = self.text[name]
        lines = text.rstrip("\n").split("\n")
        if line.startswith("#include"):
            at = max((i for i, l in enumerate(lines) if decls.INCLUDE.match(l)), default=1) + 1
            lines.insert(at, line)
        else:
            end = max((i for i, l in enumerate(lines) if l.strip() == "#endif"), default=len(lines))
            if end > 0 and not lines[end - 1].strip():
                lines.insert(end - 1, line)
            else:
                lines[end:end] = [line, ""] if end < len(lines) else [line]
        self.load(name, "\n".join(lines) + "\n")
        self.changed.add(name)

    def write(self) -> None:
        for name in sorted(self.changed):
            (self.tree / "include" / name).write_text(self.text[name])


class Unit:
    """The includes of one unit: the .c head for a C unit, D/units/UNIT.pre for a gap unit."""

    def __init__(self, tree: Path, batch_dir: Path, row: batch_files.Row):
        self.tree, self.row = tree, row
        self.path = batch_dir / "units" / f"{row.unit}.pre" if row.gap else tree / row.source
        self.text = self.path.read_text() if self.path.exists() else '#include "common.h"\n'
        self.added: list[str] = []
        self.own = f"{Path(row.unit).name}.h"

    def includes(self) -> list[str]:
        return [d.name for d in decls.parse(self.text) if d.kind == "include"]

    def add(self, header: str) -> None:
        if header in self.includes():
            return
        lines = self.text.split("\n")
        last = max((i for i, l in enumerate(lines) if decls.INCLUDE.match(l)), default=-1)
        lines.insert(last + 1, f'#include "{header}"')
        self.text = "\n".join(lines)
        self.added.append(header)

    def write(self) -> None:
        if self.added:
            self.path.parent.mkdir(parents=True, exist_ok=True)
            self.path.write_text(self.text)


def read_owners(tree: Path, version: str) -> dict[str, tuple[str, list[str]]]:
    """Symbol -> (the unit its asm or data file belongs to, every symbol of that file in address order)."""
    out = subprocess.run(["grep", "-rnE", r"^(glabel|dlabel) \w+$", "--include=*.s", str(tree / "asm" / version)],
                         capture_output=True, text=True).stdout
    files: dict[str, list[str]] = {}
    for line in out.splitlines():
        path, _, label = line.split(":", 2)
        if "/matchings/" not in path and "/nonmatchings/" not in path:
            files.setdefault(path, []).append(label.split()[1])
    owners = {}
    for path, names in files.items():
        stem = re.sub(r"(\.(data|rodata|bss|sdata|sbss|lit4|lit8))?\.s$", "", Path(path).name)
        for n in names:
            owners.setdefault(n, (stem, names))
    return owners


def neighbours(names: list[str], name: str) -> list[str]:
    """Symbols next to name in its file, nearest first."""
    idx = names.index(name)
    return [names[j] for k in range(1, NEIGHBOURS + 1) for j in (idx - k, idx + k) if 0 <= j < len(names)]


class Placer:
    def __init__(self, tree: Path, version: str):
        self.tree = tree
        self.headers = Headers(tree)
        self.owners = read_owners(tree, version)

    def place(self, unit: Unit, d: decls.Decl) -> tuple[str, str]:
        """(action, where) for one declaration, editing the headers and the unit's includes."""
        h = self.headers
        if d.kind == "include":
            if not h.exists(d.name):
                return "required", f"no header {d.name}"
            if d.name in h.closure(unit.includes()):
                return "present", d.name
            unit.add(d.name)
            return "include", d.name
        if d.kind in ("define", "other", "def", "data") or not d.name:
            return "required", f"not a declaration a header takes ({d.kind})"
        if d.text.lstrip().startswith("static"):
            return "required", "static forward declaration"
        if d.kind in ("type", "tag"):
            home = h.type_home(d.name if d.kind == "type" else next(iter(decls.defines(d)), d.name))
            if home:
                same = any(x.norm() == d.norm() for x in h.decls[home])
                return self.reach(unit, home, "present" if same else "differs")
            return "placed", self.put(unit, unit.own, d, own=True)
        found = h.declared(d.name)
        if found:
            home, x = next(((a, b) for a, b in found if b.norm() == d.norm()), found[0])
            return self.reach(unit, home, "present" if x.norm() == d.norm() else "differs")
        for home in self.homes(d.name):
            if h.needed_types(d) <= h.visible([home]):
                return "placed", self.put(unit, home, d)
        return "placed", self.put(unit, unit.own, d, own=True)

    def homes(self, name: str) -> list[str]:
        """The owning unit's include/<unit>.h, then headers declaring the symbols next to it in its file."""
        if name not in self.owners:
            return []
        stem, names = self.owners[name]
        out = [f"{stem}.h"] if f"{stem}.h" in self.headers.text else []
        for n in neighbours(names, name):
            out += [h for h, _ in self.headers.declared(n)]
        return list(dict.fromkeys(out))

    def reach(self, unit: Unit, home: str, action: str) -> tuple[str, str]:
        if home in self.headers.closure(unit.includes()):
            return action, home
        unit.add(home)
        return ("include" if action == "present" else action), home

    def put(self, unit: Unit, header: str, d: decls.Decl, own: bool = False) -> str:
        h = self.headers
        if own and header not in h.text:
            h.create(header)
        if own:
            for t in sorted(h.needed_types(d) - h.visible([header])):
                home = h.type_home(t)
                if home and home != header:
                    h.add(header, f'#include "{home}"')
        h.add(header, d.text.strip() if d.text.strip().endswith(";") else d.text.strip() + ";")
        if header not in h.closure(unit.includes()):
            unit.add(header)
        return header


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0], epilog=EPILOG)
    ap.add_argument("batch_dir", type=Path)
    ap.add_argument("--tree", type=Path, required=True, help="the drafting tree whose include/ takes them")
    ap.add_argument("--batches", type=Path, help="batches.tsv (default: <batch>/../../batches.tsv)")
    ap.add_argument("--batch-id", help="batch column value (default: the directory name)")
    ap.add_argument("--version", default="raidou1")
    a = ap.parse_args()
    batch = batch_files.load(a.batch_dir, a.batches, a.batch_id)
    placer = Placer(a.tree.resolve(), a.version)
    units: dict[str, Unit] = {}
    rows, counts = ["name\taction\twhere\tdecl"], {}
    for path in sorted(a.batch_dir.glob("*.decls")):
        name = path.stem
        if name not in batch.rows:
            print(f"{path.name}: {name} is not a row of {batch.id}", file=sys.stderr)
            continue
        row = batch.rows[name]
        unit = units.setdefault(row.source, Unit(placer.tree, batch.dir, row))
        for d in decls.parse_lines(path.read_text().splitlines()):
            action, where = placer.place(unit, d)
            counts[action] = counts.get(action, 0) + 1
            rows.append(f"{name}\t{action}\t{where}\t{' '.join(d.text.split())}")
    placer.headers.write()
    includes = []
    for unit in units.values():
        unit.write()
        if not unit.row.gap:
            includes += [f"{unit.row.source}\t{h}" for h in unit.added]
    if includes:
        old = (batch.dir / "includes.tsv").read_text().splitlines() if (batch.dir / "includes.tsv").exists() else []
        (batch.dir / "includes.tsv").write_text("\n".join(dict.fromkeys(old + includes)) + "\n")
    (batch.dir / "store_decls.tsv").write_text("\n".join(rows) + "\n")
    print(", ".join(f"{k} {v}" for k, v in sorted(counts.items())) or "no .decls files",
          f"; headers edited: {' '.join(sorted(placer.headers.changed)) or 'none'}")
    return 1 if counts.get("required") else 0


if __name__ == "__main__":
    sys.exit(main())
