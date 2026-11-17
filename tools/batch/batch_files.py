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
    shared: str = ""  # batches.tsv shared column: R1, R2 or R1/R2, the game the row was drafted against
    key: str = ""  # the row's key in Batch.rows and results.tsv: the name, or NAME@raidou2 for a both-game name

    def __post_init__(self) -> None:
        self.key = self.key or self.name

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
    def version(self) -> str:
        """The game whose copy of the function the draft matches: the unit's game, else raidou2 for an R2 row."""
        if self.game in ("raidou1", "raidou2"):
            return self.game
        return "raidou2" if self.shared == "R2" else "raidou1"

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

    def final(self, key: str, names: dict[str, str] | None = None) -> str | None:
        """final/<key>.c, else <key>.c in its NON_MATCHING form when the result says so; None if not drafted.
        A NAME@raidou2 row without its own file uses NAME's (the same C in both games)."""
        row = self.rows[key]
        draft = next((self.dir / f"{k}.c" for k in dict.fromkeys([key, row.name]) if (self.dir / f"{k}.c").exists()),
                     None)
        path = self.dir / "final" / f"{key}.c"
        if path.exists():
            text = path.read_text()
        elif draft and row.result:
            text = draft.read_text()
            if row.nonmatch is not None:
                comment = self.comments.get(key, self.comments.get(row.name, ""))
                text = unit_text.nonmatching_final(text, row.unit, row.name, row.words, row.nonmatch, comment.strip())
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
    tsv = list(csv.reader(batches.read_text().splitlines(), delimiter="\t"))
    col = tsv[0].index("shared") if tsv and "shared" in tsv[0] else None
    rows: dict[str, Row] = {}
    for r in tsv:
        if not r or r[0] != batch_id:
            continue
        row = Row(r[1], r[4], addr=r[2], shared=r[col] if col is not None and col < len(r) else "")
        if row.name in rows and f"{row.name}@raidou2" in rows:
            raise SystemExit(f"land_batch: {row.name} has three rows in {batch_id}")
        if row.name in rows or f"{row.name}@raidou2" in rows:
            # one name drafted for both games: the Raidou 2 row is NAME@raidou2
            pair = [rows.pop(row.name, None) or rows.pop(f"{row.name}@raidou2"), row]
            if sorted(p.version for p in pair) != ["raidou1", "raidou2"]:
                raise SystemExit(f"land_batch: {row.name} has two {row.version} rows in {batch_id}")
            for p in pair:
                p.key = p.name if p.version == "raidou1" else f"{p.name}@raidou2"
                rows[p.key] = p
            continue
        rows[row.key] = row
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
