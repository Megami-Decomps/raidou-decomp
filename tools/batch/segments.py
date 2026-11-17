"""Splat config edits for units that become C: subsegment kind, tu_evidence kind, and .rodata/.lit4 ownership."""
import re
from dataclasses import dataclass, field
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
    left: list[str] = field(default_factory=list)  # the unit's leading labels kept in the chunk for alignment
    c_only: bool = False  # only the labels its C functions use; its asm functions' labels stay in the chunk


def plan(chunk: str, kind: str, labels: list[tuple[int, str]], users: dict[str, set[str]],
         unit: str, c_used: set[str] = frozenset()) -> tuple[Plan | None, str]:
    """Decide the unit's share of one chunk: the labels from its first to its last, none used elsewhere, starting at
    the first one its section alignment allows. When that range is refused, the labels its C functions use
    (`c_used`), which the compiler emits into the unit's own .rodata, are the share instead."""
    mine = [i for i, (_, n) in enumerate(labels) if unit in users.get(n, ())]
    if not mine:
        return None, ""
    p, note = plan_range(chunk, kind, labels, users, unit, mine)
    if p or not note:
        return p, note
    c = [i for i in mine if labels[i][1] in c_used]
    if not c:
        return None, note
    p, c_note = plan_range(chunk, kind, labels, users, unit, c)
    if not p:
        a, n = labels[c[0]]
        return None, f"{note}; its C uses {n} at 0x{a:08X}, which needs its .rodata: {c_note}"
    p.c_only = True
    return p, ""


def plan_range(chunk: str, kind: str, labels: list[tuple[int, str]], users: dict[str, set[str]], unit: str,
               mine: list[int]) -> tuple[Plan | None, str]:
    shared = [labels[i][1] for i in mine if users[labels[i][1]] - {unit}]
    if shared:
        return None, f"{unit}: {kind} {chunk}: {', '.join(shared)} also used by " + \
            ", ".join(sorted(set().union(*(users[n] for n in shared)) - {unit}))
    lo, hi = mine[0], mine[-1]
    between = [n for _, n in labels[lo:hi + 1] if users.get(n, set()) - {unit}]
    if between:
        return None, f"{unit}: {kind} {chunk}: other units' labels {', '.join(between)} lie inside its range"
    rest = labels[hi + 1][0] if hi + 1 < len(labels) else None
    first = aligned_start(kind, labels, lo, hi)
    if first is None:
        return None, f"{unit}: {kind} {chunk}: no start from 0x{labels[lo][0]:08X} on fits its section alignment"
    left = [n for _, n in labels[lo:first]]
    return Plan(unit, kind, chunk, labels[first][0], rest, left), ""


def label_align(name: str, addr: int) -> int:
    """The alignment a label gets once the unit owns it: 16 for a jump table, which ee-gcc always starts with
    .align 4, else what its address allows up to 8 (as tools/include_rodata.py aligns included files)."""
    if name.startswith("jtbl_"):
        return 16
    return next(a for a in (8, 4, 2, 1) if addr % a == 0)


def aligned_start(kind: str, labels: list[tuple[int, str]], lo: int, hi: int) -> int | None:
    """Index of the first label from lo on where the unit's .rodata can start. The object's section is aligned to
    its most aligned label, so a start off that alignment moves the whole section; the labels before the returned
    index stay in the chunk."""
    if kind != "rodata":
        return lo
    if any(a % label_align(n, a) for a, n in labels[lo:hi + 1]):
        return None
    for i in range(lo, hi + 1):
        if labels[i][0] % max(label_align(n, a) for a, n in labels[i:hi + 1]) == 0:
            return i
    return None


def piece_ranges(yaml_text: str) -> dict[str, tuple[int, int | None]]:
    """Unowned chunk name -> vram range [start, end), the end being the next subsegment's start or None."""
    off = rom_offset(yaml_text)
    subs = [m for l in yaml_text.splitlines() if (m := SUBSEG.match(l))]
    out = {}
    for i, m in enumerate(subs):
        if m.group(3) in KINDS:
            out[m.group(4)] = (int(m.group(2), 16) + off, int(subs[i + 1].group(2), 16) + off if i + 1 < len(subs)
                               else None)
    return out


def piece_labels(data: Path, yaml_text: str, found: dict[str, str]) -> tuple[dict, dict, list[str]]:
    """(chunk -> labels, chunk -> files they came from, notes). A piece an earlier plan in this run split off has no
    file until splat reruns, so its labels come from the files of its kind that cover its range."""
    ranges = piece_ranges(yaml_text)
    labels, sources, notes = {}, {}, []
    for name, kind in found.items():
        own = data / f"{name}.{kind}.s"
        if own.exists():
            labels[name], sources[name] = chunk_labels(own), [own]
            continue
        lo, hi = ranges[name]
        got, files = {}, []
        for path in sorted(data.glob(f"*.{kind}.s")):
            inside = [(a, n) for a, n in chunk_labels(path) if a >= lo and (hi is None or a < hi)]
            if inside:
                files.append(path)
                for a, n in inside:
                    got.setdefault(a, n)
        labels[name], sources[name] = sorted(got.items()), files
        if not got:
            notes.append(f"{kind} {name} (0x{lo:08X}): no asm/data file holds its labels; rerun configure.py "
                         "and land the units that use it again")
    return labels, sources, notes


def function_words(asm_text: str, names: set[str]) -> set[str]:
    """Words of the named functions' blocks in a unit's asm (each from its `nonmatching` line to the next)."""
    out: set[str] = set()
    for block in re.split(r"^(?=nonmatching \w+, 0x)", asm_text, flags=re.M):
        m = re.match(r"nonmatching (\w+), 0x", block)
        if m and m.group(1) in names:
            out |= set(WORD.findall(block))
    return out


def plan_unit(tree: Path, game: str, unit: str, c_funcs: set[str] = frozenset()) -> tuple[list[Plan], list[str]]:
    """Plans for the rodata and lit4 that a gap unit's asm uses, and the cases left for a hand decision. `c_funcs`
    are the unit's functions that become C, by their names in its asm."""
    asm = tree / "asm" / game
    text = yaml_path(tree, game).read_text()
    found = {name: kind for name, kind in chunks(text).items()}
    labels, sources, notes = piece_labels(asm / "data", text, found)
    where = {n: c for c, ls in labels.items() for _, n in ls}
    asm_text = (asm / f"{unit}.s").read_text()
    words = set(WORD.findall(asm_text))
    c_used = function_words(asm_text, c_funcs)
    used = {where[w] for w in words if w in where}
    if not used:
        return [], notes
    wanted = {n for c in used for _, n in labels[c]}
    skip = {f for c in used for f in sources[c]}
    users: dict[str, set[str]] = {}
    for path in asm.rglob("*.s"):
        if path in skip:
            continue
        o = owner(asm, path)
        for w in set(WORD.findall(path.read_text(errors="replace"))) & wanted:
            users.setdefault(w, set()).add(o)
    plans = []
    for c in sorted(used):
        p, note = plan(c, found[c], labels[c], users, unit, c_used)
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
    """Give the unit its labels: a .<kind> subsegment at its first label, the chunk resuming after its last. The
    chunk may already be split by another unit's plan; the unit goes into the piece that holds its first label."""
    off = rom_offset(yaml_text)
    lines = yaml_text.split("\n")
    target, rest = p.start - off, None if p.rest is None else p.rest - off
    starts = [(int(m.group(2), 16), i) for i, l in enumerate(lines) if (m := SUBSEG.match(l))]
    at = max(((a, i) for a, i in starts if a <= target), default=(0, None))[1]
    if at is None:
        raise ValueError(f"{p.unit}: no subsegment holds 0x{p.start:08X}")
    m = SUBSEG.match(lines[at])
    if m.group(3) != p.kind or not (m.group(4) == p.chunk or m.group(4).startswith(f"{p.kind}_")):
        raise ValueError(f"{p.unit}: {p.kind} at 0x{p.start:08X} lies in {m.group(4)}, not in {p.chunk}")
    head = m.group(1)
    new = [f"{head}0x{target:X}, .{p.kind}, {p.unit}]"]
    nxt = SUBSEG.match(lines[at + 1]) if at + 1 < len(lines) else None
    if rest is not None and not (nxt and int(nxt.group(2), 16) == rest):
        if nxt and int(nxt.group(2), 16) < rest:
            raise ValueError(f"{p.unit}: {nxt.group(4)} starts inside its {p.kind}")
        new.append(f"{head}0x{rest:X}, {p.kind}, {p.kind}_{p.rest:08X}]")
    if int(m.group(2), 16) == target:
        lines[at:at + 1] = new
    else:
        lines[at + 1:at + 1] = new
    return "\n".join(lines)
