"""docs/matching entries: write one per NON_MATCHING draft from its measured distance, drop those of matched C."""
import re
import subprocess
from pathlib import Path

LEVERS = "https://github.com/Megami-Decomps/smt3-decomp/blob/main/docs/levers"
LEVER = {
    "scheduling": ("store scheduling", "scheduling.md"),
    "stores": ("stores", "stores.md"),
    "control": ("control flow", "control-flow.md"),
    "branch": ("callee visibility and branch-likely", "callee-visibility.md"),
    "crossblock": ("cross-block locals with swapped registers", "cross-block-locals.md"),
    "spills": ("scalar spill slots", "scalar-spills.md"),
}
GAME = {"raidou1": "Raidou 1", "raidou2": "Raidou 2"}
OK = re.compile(r"^OK +\S+ @ \S+ \((\d+) bytes\)", re.M)
DIFF = re.compile(r"^DIFF \S+ @ \S+: (\d+) of (\d+) words differ \(first at (\+0x[0-9A-F]+)\)", re.M)
SIZES = re.compile(r"(\d+) emitted bytes(?:, (\d+) native bytes)?")


def lever_link(lever: str) -> str:
    if lever in LEVER:
        title, page = LEVER[lever]
        return f"[smt3 levers: {title}]({LEVERS}/{page})"
    return lever


def measure(tree: Path, source: str, game: str, fn: str) -> tuple[str, str]:
    """(size field, distance sentence) of a draft compiled with -DNON_MATCHING."""
    out = subprocess.run([str(tree / ".venv/bin/python"), "tools/check_unit.py", source, "--version", game,
                          "--cflags=-DNON_MATCHING", "--func", fn], cwd=tree, capture_output=True, text=True).stdout
    if ok := OK.search(out):
        n = int(ok.group(1))
        return f"{game} 0x{n:X}", f"{GAME[game]}: 0 of {n // 4} words differ."
    m, s = DIFF.search(out), SIZES.search(out)
    if not (m and s):
        raise RuntimeError(f"{fn} {game}: no measurement\n{out[-800:]}")
    emitted, native = int(s.group(1)), int(s.group(2) or s.group(1))
    line = f"{GAME[game]}: {m.group(1)} of {m.group(2)} words differ, first at {m.group(3)}"
    if emitted != native:
        line += f"; emitted {emitted} bytes, native {native} bytes"
    return f"{game} 0x{native:X}", line + "."


def entry(fn: str, unit: str, measured: list[tuple[str, str]], text: str, lever: str) -> str:
    sizes = ", ".join(s for s, _ in measured)
    lines = "\n".join(l for _, l in measured)
    related = f"\n\nRelated: {lever_link(lever)}." if lever else ""
    return f"## {fn} ({unit}, {sizes})\n\n{lines}\n\n{text}{related}\n"


def append(tree: Path, unit: str, entries: list[str]) -> None:
    path = tree / "docs/matching" / f"{unit}.md"
    head = path.read_text().rstrip("\n") + "\n\n" if path.exists() else f"# Draft notes: {unit}\n\n"
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(head + "\n".join(entries))


def drop(text: str, fn: str) -> str:
    """The page without the entry headed `## fn (`, from its heading to the next one."""
    lines = text.split("\n")
    starts = [i for i, l in enumerate(lines) if l.startswith("## ")]
    for k, i in enumerate(starts):
        if re.match(r"^## %s \(" % re.escape(fn), lines[i]):
            end = starts[k + 1] if k + 1 < len(starts) else len(lines)
            return "\n".join(lines[:i] + lines[end:]).rstrip("\n") + "\n"
    return text


def drop_entries(tree: Path, names: set[str]) -> list[str]:
    """Remove the entries of these functions from every page; returns "page: fn" for each one removed."""
    removed = []
    for path in sorted((tree / "docs/matching").rglob("*.md")):
        if path.name == "README.md":
            continue
        text = path.read_text()
        for fn in sorted(names):
            new = drop(text, fn)
            if new != text:
                removed.append(f"{path.relative_to(tree)}: {fn}")
                text = new
        if text != path.read_text():
            if any(l.startswith("## ") for l in text.split("\n")):
                path.write_text(text)
            else:
                path.unlink()
    return removed


NOT_A_DRAFT = re.compile(r"^docs/matching/(.+)\.md: (\w+) is not a (\w+) draft", re.M)
ENTRY_HEAD = re.compile(r"^## (.+?) \(([^,]+), ([^)]+)\)$", re.M)


def stale_entries(tree: Path) -> dict[tuple[str, str], set[str]]:
    """(unit, fn) -> games for which docs_index.py --check says the entry's function is not a draft in the tree."""
    out = subprocess.run([str(tree / ".venv/bin/python"), "tools/docs_index.py", "--check"], cwd=tree,
                         capture_output=True, text=True).stdout
    stale: dict[tuple[str, str], set[str]] = {}
    for unit, fn, game in NOT_A_DRAFT.findall(out):
        stale.setdefault((unit, fn), set()).add(game)
    return stale


def drop_stale(tree: Path) -> tuple[list[str], list[str]]:
    """Drop every entry whose function is a draft in none of the entry's games, wherever it was promoted.
    Returns (dropped, left): left names entries that are stale for only some games or cover several functions."""
    dropped, left = [], []
    for (unit, fn), games in sorted(stale_entries(tree).items()):
        path = tree / "docs/matching" / f"{unit}.md"
        text = path.read_text() if path.exists() else ""
        head = next((m for m in ENTRY_HEAD.finditer(text) if m.group(1) == fn), None)
        if head is None:
            left.append(f"{path.relative_to(tree)}: {fn} ({', '.join(sorted(games))}), entry covers several functions")
            continue
        listed = set(re.findall(r"\b(raidou\d)\b", head.group(3)))
        if listed - games:
            left.append(f"{path.relative_to(tree)}: {fn} is no longer a {', '.join(sorted(games))} draft only")
            continue
        new = drop(text, fn)
        if any(l.startswith("## ") for l in new.split("\n")):
            path.write_text(new)
        else:
            path.unlink()
        dropped.append(f"{path.relative_to(tree)}: {fn}")
    return dropped, left
