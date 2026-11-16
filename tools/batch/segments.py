"""Splat config edits for units that become C: subsegment kind, tu_evidence kind, and .rodata/.lit4 ownership."""
import re
from dataclasses import dataclass
from pathlib import Path

SUBSEG = re.compile(r"^(\s*- \[)(0x[0-9A-Fa-f]+), (\.?\w+), ([\w/]+)\](.*)$")
DLABEL = re.compile(r"^dlabel (\S+)\s*\n\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ", re.M)
WORD = re.compile(r"[A-Za-z_][\w.]*")
KINDS = ("rodata", "lit4")


def yaml_path(tree: Path, game: str) -> Path:
    return next((tree / "config" / game).glob("*.yaml"))


def make_c(tree: Path, game: str, unit: str) -> bool:
    """Turn the unit's asm subsegment into c, in the yaml and in tu_evidence.tsv; False when it is not asm."""
    path = yaml_path(tree, game)
    text = path.read_text()
    new = re.sub(r"^(\s*- \[0x[0-9A-Fa-f]+, )asm(, %s\])" % re.escape(unit), r"\1c\2", text, count=1, flags=re.M)
    if new == text:
        return False
    path.write_text(new)
    ev = tree / "config" / game / "tu_evidence.tsv"
    if ev.exists():
        ev.write_text(re.sub(r"^([0-9A-F]+\t[0-9A-F]+\t%s\t)asm\t" % re.escape(unit), r"\1c\t",
                             ev.read_text(), flags=re.M))
    return True


def rom_offset(yaml_text: str) -> int:
    """vram - rom of the main segment."""
    start = int(re.search(r"^\s+start: (0x[0-9A-Fa-f]+)", yaml_text, re.M).group(1), 16)
    vram = int(re.search(r"^\s+vram: (0x[0-9A-Fa-f]+)", yaml_text, re.M).group(1), 16)
    return vram - start


def chunks(yaml_text: str) -> dict[str, str]:
    """Unowned rodata/lit4 chunk name -> kind."""
    return {m.group(4): m.group(3) for l in yaml_text.splitlines()
            if (m := SUBSEG.match(l)) and m.group(3) in KINDS}


def chunk_labels(path: Path) -> list[tuple[int, str]]:
    return [(int(a, 16), n) for n, a in DLABEL.findall(path.read_text())] if path.exists() else []


def owner(asm: Path, path: Path) -> str:
    rel = path.relative_to(asm)
    if rel.parts[0] in ("nonmatchings", "matchings"):
        return "/".join(rel.parts[1:-1])
    if rel.parts[0] == "data":
        return f"data:{rel.name}"
    return rel.with_suffix("").as_posix()


@dataclass
class Plan:
    unit: str
    kind: str
    chunk: str
    start: int  # vram of the unit's first label
    rest: int | None  # vram where the chunk resumes after the unit, or None


def plan(chunk: str, kind: str, labels: list[tuple[int, str]], users: dict[str, set[str]],
         unit: str) -> tuple[Plan | None, str]:
    """Decide the unit's share of one chunk: the labels from its first to its last, none used elsewhere."""
    mine = [i for i, (_, n) in enumerate(labels) if unit in users.get(n, ())]
    if not mine:
        return None, ""
    shared = [labels[i][1] for i in mine if users[labels[i][1]] - {unit}]
    if shared:
        return None, f"{unit}: {kind} {chunk}: {', '.join(shared)} also used by " + \
            ", ".join(sorted(set().union(*(users[n] for n in shared)) - {unit}))
    lo, hi = mine[0], mine[-1]
    between = [n for _, n in labels[lo:hi + 1] if users.get(n, set()) - {unit}]
    if between:
        return None, f"{unit}: {kind} {chunk}: other units' labels {', '.join(between)} lie inside its range"
    rest = labels[hi + 1][0] if hi + 1 < len(labels) else None
    return Plan(unit, kind, chunk, labels[lo][0], rest), ""


def plan_unit(tree: Path, game: str, unit: str) -> tuple[list[Plan], list[str]]:
    """Plans for the rodata and lit4 that a gap unit's asm uses, and the cases left for a hand decision."""
    asm = tree / "asm" / game
    text = yaml_path(tree, game).read_text()
    found = {name: kind for name, kind in chunks(text).items()}
    labels = {name: chunk_labels(asm / "data" / f"{name}.{kind}.s") for name, kind in found.items()}
    where = {n: c for c, ls in labels.items() for _, n in ls}
    words = set(WORD.findall((asm / f"{unit}.s").read_text()))
    used = {where[w] for w in words if w in where}
    if not used:
        return [], []
    wanted = {n for c in used for _, n in labels[c]}
    users: dict[str, set[str]] = {}
    for path in asm.rglob("*.s"):
        if path.parent.name == "data" and path.stem.rsplit(".", 1)[0] in used:
            continue
        o = owner(asm, path)
        for w in set(WORD.findall(path.read_text(errors="replace"))) & wanted:
            users.setdefault(w, set()).add(o)
    plans, notes = [], []
    for c in sorted(used):
        p, note = plan(c, found[c], labels[c], users, unit)
        if p:
            plans.append(p)
        if note:
            notes.append(note)
    for kind in KINDS:
        if len([p for p in plans if p.kind == kind]) > 1:
            notes.append(f"{unit}: {kind} in {len([p for p in plans if p.kind == kind])} chunks")
            plans = [p for p in plans if p.kind != kind]
    return plans, notes


def apply_plan(yaml_text: str, p: Plan) -> str:
    """Give the unit its labels: a .<kind> subsegment at its first label, the chunk resuming after its last."""
    off = rom_offset(yaml_text)
    out = []
    for line in yaml_text.split("\n"):
        m = SUBSEG.match(line)
        if not (m and m.group(4) == p.chunk and m.group(3) == p.kind):
            out.append(line)
            continue
        head = m.group(1)
        if int(m.group(2), 16) + off < p.start:
            out.append(line)
        out.append(f"{head}0x{p.start - off:X}, .{p.kind}, {p.unit}]")
        if p.rest is not None:
            out.append(f"{head}0x{p.rest - off:X}, {p.kind}, {p.kind}_{p.rest:08X}]")
    return "\n".join(out)
