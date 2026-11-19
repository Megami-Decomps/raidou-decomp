"""Apply a batch's symbol renames and label deletions to a tree, in both games' config and in the sources."""
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import version_guard  # noqa: E402

CARRIED = ("ida_protos.txt", "claims.tsv", "tu_evidence.tsv", "data_c.tsv", "smt3_c.tsv", "smt3_data.tsv")
PAIRS = ("r1_r2_pairs.tsv", "r1_r2_data_pairs.tsv")
SOURCES = ("SMT3", "HD", "DDS1", "DDS2", "dds1")
GAMES = ("raidou1", "raidou2")


def tier(reason: str) -> tuple[str, str]:
    if "body" in reason:
        return "A", "body"
    if reason.startswith(("SMT3", "HD")) or "role" in reason:
        return "B", "role"
    return "C", "derived-role"


def name_source_row(addr_field: str, new: str, old: str, reason: str) -> str:
    tr, method = tier(reason)
    word = reason.split()[0] if reason.split() else ""
    src = word.lower() if word in SOURCES else "derived"
    return (f"{addr_field}\t{new}\tinferred\tmethod={src}-{method}; p=na; tier={tr}; source={src}; "
            f"evidence={reason}; replaces {old}")


def rewrite(path: Path, edit) -> None:
    if path.exists():
        text = path.read_text()
        new = edit(text)
        if new != text:
            path.write_text(new)


def rename_config(tree: Path, game: str, old: str, new: str, reason: str) -> None:
    cfg = tree / "config" / game
    text = (cfg / "symbol_addrs.txt").read_text()
    m = re.search(r"^%s = (0x[0-9A-Fa-f]+);" % re.escape(old), text, re.M)
    if not m:
        raise ValueError(f"{game}: {old} not in symbol_addrs.txt")
    if re.search(r"^%s = " % re.escape(new), text, re.M):
        raise ValueError(f"{game}: {new} already in symbol_addrs.txt")
    addr = m.group(1)
    (cfg / "symbol_addrs.txt").write_text(text[:m.start()] + new + text[m.start() + len(old):])
    # an smt3 alias of the new name at the same address is now the symbol itself
    rewrite(cfg / "smt3_symbols.txt",
            lambda t: re.sub(r"^%s = %s;\n" % (re.escape(new), addr), "", t, flags=re.M | re.I))
    text = (cfg / "name_sources.txt").read_text()
    m = re.search(r"^%s\t%s\t.*$" % (addr, re.escape(old)), text, re.M | re.I)
    if not m:
        raise ValueError(f"{game}: {old} not in name_sources.txt")
    row = name_source_row(m.group(0).split("\t")[0], new, old, reason)
    (cfg / "name_sources.txt").write_text(text[:m.start()] + row + text[m.end():])
    for name in CARRIED:
        rewrite(cfg / name, lambda t: re.sub(r"\b%s\b" % re.escape(old), new, t))
    col = 0 if game == "raidou1" else 1
    for name in PAIRS:
        def pair(t: str) -> str:
            out = []
            for line in t.split("\n"):
                f = line.split("\t")
                if len(f) > 2 and f[2] == old and f[col].lower() == addr.lower():
                    f[2] = new
                out.append("\t".join(f))
            return "\n".join(out)
        rewrite(tree / "config/xgame" / name, pair)


def rename_sources(tree: Path, old: str, new: str) -> int:
    return rename_sources_per_game(tree, old, {g: new for g in GAMES}, {g: True for g in GAMES})


def game_lines(path: str, text: str) -> list[tuple[str, frozenset]]:
    """(line, games that compile it) for each line; a src/<game>/ file is that game's alone."""
    games = frozenset(g for g in GAMES if f"src/{g}/" in path) or frozenset(GAMES)
    if not path.endswith((".c", ".h")) or not version_guard.GAME.search(text):
        return [(line, games) for line in text.splitlines(keepends=True)]
    kept = {g: version_guard.lines_for(text, g) for g in games}
    return [(line, frozenset(g for g in games if kept[g][i][1]) or games)
            for i, line in enumerate(text.splitlines(keepends=True))]


def is_use(line: str, old: str) -> bool:
    """A line that needs this game's own symbol: not its INCLUDE_ASM and not a one-line prototype."""
    return not re.match(r"\s*(INCLUDE_ASM\(.*\b%s\);|\w[\w\s*]*?[\s*]%s\s*\([^;{=]*\)\s*;)\s*$"
                        % ((re.escape(old),) * 2), line)


def rename_text(path: str, text: str, old: str, new: dict[str, str], has: dict[str, bool],
                unused: frozenset = frozenset()) -> str:
    """Rename old to new[game] in the lines each game compiles. `has`: games where old is a symbol; `unused`:
    those whose C never uses it, so a line both games read is the other game's. A line compiled by games that
    would rename it differently gets a VERSION_ guard (C) or is left with a warning."""
    word = re.compile(r"\b%s\b" % re.escape(old))
    out = []
    prev = ""
    for line, games in game_lines(path, text):
        if word.search(line):
            relevant = [g for g in sorted(games) if has.get(g)] or sorted(new)
            if len(relevant) > 1:
                relevant = [g for g in relevant if g not in unused] or relevant
            names = {g: new.get(g, old) for g in relevant}
            if len(set(names.values())) == 1:
                line = word.sub(names[relevant[0]], line)
            elif path.endswith((".c", ".h")) and not prev.rstrip("\n").endswith("\\") \
                    and not line.rstrip("\n").endswith("\\"):
                body = line if line.endswith("\n") else line + "\n"
                first, *rest = relevant
                line = (f"#ifdef {version_guard._macro(first)}\n" + word.sub(names[first], body) + "#else\n"
                        + word.sub(names[rest[0]], body) + "#endif\n")
                print(f"{path}: guarded {old} -> {names}")
            else:
                print(f"{path}: left {old} on a line both games read: {line.strip()}")
        out.append(line)
        prev = line
    return "".join(out)


def rename_sources_per_game(tree: Path, old: str, new: dict[str, str], has: dict[str, bool]) -> int:
    # --untracked: a batch's new headers and units are not added yet
    files = subprocess.run(["git", "grep", "--untracked", "-l", "-w", old, "--", "src", "include", "docs"], cwd=tree,
                           capture_output=True, text=True).stdout.split()
    word = re.compile(r"\b%s\b" % re.escape(old))
    used = set()
    for f in files:
        for line, games in game_lines(f, (tree / f).read_text()):
            if word.search(line) and is_use(line, old):
                used |= games
    unused = frozenset(g for g in GAMES if has.get(g) and g not in new and g not in used)
    for f in files:
        rewrite(tree / f, lambda t: rename_text(f, t, old, new, has, unused))
    return len(files)


def symbols(tree: Path, game: str) -> set[str]:
    path = tree / "config" / game / "symbol_addrs.txt"
    return set(re.findall(r"^(\w+) = ", path.read_text(), re.M)) if path.exists() else set()


def complete(tree: Path, rows: list[tuple[str, str, str, str]],
             one_game: set[str] = frozenset(), split: set[str] = frozenset()) -> tuple[list[tuple[str, str, str, str]], list]:
    """Rows plus the other game's row for a name both games share, since the sources are renamed once for both.
    Refuses (ValueError) a row whose new name the other game already has, or whose function is in `one_game`
    (r1only.txt: the other game's symbol may be another function). A name in `split` is another function in
    the other game: it gets no row there."""
    games = GAMES
    names = {g: symbols(tree, g) for g in games}
    given = {(g, old) for g, old, _, _ in rows}
    out, added = [], []
    for row in rows:
        game, old, new, reason = row
        todo = [row]
        for other in games:
            if other != game and (other, old) not in given and old in names[other] and old not in split:
                if old in one_game or new in one_game:
                    raise ValueError(f"{game} rename {old} -> {new}: {old} is shared with {other} but the function is "
                                     f"in r1only.txt; add the {other} row or rename its sites by hand")
                if new in names[other]:
                    raise ValueError(f"{game} rename {old} -> {new}: {other} has both names; add its row or drop this one")
                todo.append((other, old, new, reason))
        for g, o, n, _ in todo:
            names[g].discard(o)
            names[g].add(n)
        out += todo
        added += todo[1:]
    return out, added


def apply(tree: Path, rows: list[tuple[str, str, str, str]]) -> None:
    """Rows (game, old, new, reason) in order: config per game, then source text once per old name, each game's
    lines taking that game's new name."""
    names = {g: symbols(tree, g) for g in GAMES}
    steps: dict[str, tuple[dict[str, str], dict[str, bool]]] = {}
    for game, old, new, _reason in rows:
        if old not in steps:
            steps[old] = ({}, {g: old in names[g] for g in GAMES})
        steps[old][0].setdefault(game, new)
        names[game].discard(old)
        names[game].add(new)
    for game, old, new, reason in rows:
        rename_config(tree, game, old, new, reason)
    for old, (new, has) in steps.items():
        rename_sources_per_game(tree, old, new, has)


def drop(tree: Path, rows: list[tuple[str, str]]) -> None:
    """Delete labels (game, name): symbol_addrs, the name_sources row and the data pair row."""
    for game, name in rows:
        cfg = tree / "config" / game
        text = (cfg / "symbol_addrs.txt").read_text()
        m = re.search(r"^%s = (0x[0-9A-Fa-f]+);.*\n" % re.escape(name), text, re.M)
        if not m:
            raise ValueError(f"{game}: {name} not in symbol_addrs.txt")
        addr = m.group(1)
        (cfg / "symbol_addrs.txt").write_text(text[:m.start()] + text[m.end():])
        rewrite(cfg / "name_sources.txt",
                lambda t: re.sub(r"^%s\t%s\t.*\n" % (addr, re.escape(name)), "", t, flags=re.M | re.I))
        col = 0 if game == "raidou1" else 1
        for pairs in PAIRS:
            rewrite(tree / "config/xgame" / pairs, lambda t: "".join(
                l for l in t.splitlines(True)
                if not ((f := l.split("\t")) and len(f) > 2 and f[2] == name and f[col].lower() == addr.lower())))


def name_map(rows: list[tuple[str, str, str, str]], game: str = "raidou1") -> dict[str, str]:
    """Old name -> final name for one game, following chains in table order."""
    out: dict[str, str] = {}
    for g, old, new, _ in rows:
        if g != game:
            continue
        for k, v in out.items():
            if v == old:
                out[k] = new
        out.setdefault(old, new)
    return out
