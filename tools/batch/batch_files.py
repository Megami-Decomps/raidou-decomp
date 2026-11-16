"""Read a batch directory: its rows in batches.tsv, results, final drafts and packaging tables."""
import csv
import json
import re
from dataclasses import dataclass, field
from pathlib import Path

import unit_text

NONMATCH = re.compile(r"^nonmatch (\d+)")


@dataclass
class Row:
    name: str
    path: str  # batches.tsv unit column: src/<folder>/<unit> or asm/<game>/<unit>
    words: int = 0
    result: str = ""
    addr: str = ""

    @property
    def gap(self) -> bool:
        return self.path.startswith("asm/")

    @property
    def unit(self) -> str:
        return re.sub(r"^(src/(shared|raidou\d)|asm/raidou\d)/", "", self.path)

    @property
    def game(self) -> str:
        return self.path.split("/")[1]

    @property
    def source(self) -> str:
        """The unit's C file relative to the tree, once it exists."""
        return f"src/{self.game}/{self.unit}.c" if self.gap else f"{self.path}.c"

    @property
    def nonmatch(self) -> int | None:
        m = NONMATCH.match(self.result)
        return int(m.group(1)) if m else None


@dataclass
class Batch:
    dir: Path
    id: str
    rows: dict[str, Row]
    r1only: set[str] = field(default_factory=set)
    renames: list[tuple[str, str, str, str]] = field(default_factory=list)
    drop: list[tuple[str, str]] = field(default_factory=list)
    docs: dict[str, list[str]] = field(default_factory=dict)
    comments: dict[str, str] = field(default_factory=dict)

    def final(self, name: str, names: dict[str, str] | None = None) -> str | None:
        """final/<name>.c, else <name>.c in its NON_MATCHING form when the result says so; None if not drafted."""
        row = self.rows[name]
        path = self.dir / "final" / f"{name}.c"
        if path.exists():
            text = path.read_text()
        elif (self.dir / f"{name}.c").exists() and row.result:
            text = (self.dir / f"{name}.c").read_text()
            if row.nonmatch is not None:
                text = unit_text.nonmatching_final(text, row.unit, name, row.words, row.nonmatch,
                                                   self.comments.get(name, "").strip())
        else:
            return None
        return unit_text.substitute(text, names or {})

    def tsv(self, name: str) -> list[list[str]]:
        path = self.dir / name
        if not path.exists():
            return []
        return [l.rstrip("\n").split("\t") for l in path.read_text().splitlines()
                if l.strip() and not l.startswith("#")]


def load(dir: Path, batches: Path | None = None, batch_id: str | None = None,
         renames: tuple[str, ...] = ("renames.tsv",)) -> Batch:
    dir = dir.resolve()
    batch_id = batch_id or dir.name
    batches = batches or dir.parent.parent / "batches.tsv"
    rows = {r[1]: Row(r[1], r[4], addr=r[2]) for r in csv.reader(batches.read_text().splitlines(), delimiter="\t")
            if r and r[0] == batch_id}
    if not rows:
        raise SystemExit(f"land_batch: no rows for {batch_id} in {batches}")
    b = Batch(dir, batch_id, rows)
    for r in b.tsv("results.tsv")[1:]:
        if r[0] in rows:
            rows[r[0]].words, rows[r[0]].result = int(r[1]), r[2]
    if (dir / "r1only.txt").exists():
        b.r1only = {l.split()[0] for l in (dir / "r1only.txt").read_text().splitlines()
                    if l.strip() and not l.startswith("#")}
    for name in renames:
        b.renames += [tuple(r[:4]) for r in b.tsv(name)]
    b.drop = [tuple(r[:2]) for r in b.tsv("drop_symbols.tsv")]
    for name in ("docs", "comments"):
        if (dir / f"{name}.json").exists():
            setattr(b, name, json.loads((dir / f"{name}.json").read_text()))
    return b
