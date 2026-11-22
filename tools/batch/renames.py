"""Apply a batch's symbol renames and label deletions to a tree, per game in config and in the sources.

    python3 tools/batch/renames.py [--tree T] [--dry-run] TABLE...

TABLE rows are game<TAB>old<TAB>new<TAB>reason (# lines skipped). A row renames its game only when the other
game's symbol of that name is another function: the other game has a row old<TAB>old (it keeps the name) or
already holds the new name. Otherwise the other game gets the same rename. Shared lines that each game would
name differently get a VERSION_ guard; a line that cannot take one (a preprocessor directive, a macro continuation,
a line inside a block comment) is refused, naming the file and line. A name in a comment is renamed only when every
game reading the line takes the same name, and a name in a string or character literal is never renamed."""
import argparse
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
GAME_WORD = re.compile(r"\b(%s)\b" % "|".join(GAMES))
ROOT = Path(__file__).resolve().parents[2]

# path -> text before the first write (None: the file did not exist), while apply() can still roll back
journal: dict[Path, str | None] | None = None


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


def write(path: Path, text: str) -> None:
    if journal is not None and path not in journal:
        journal[path] = path.read_text() if path.exists() else None
    path.write_text(text)


def rewrite(path: Path, edit) -> None:
    if path.exists():
        text = path.read_text()
        new = edit(text)
        if new != text:
            write(path, new)


def rename_config(tree: Path, game: str, old: str, new: str, reason: str) -> None:
    cfg = tree / "config" / game
    text = (cfg / "symbol_addrs.txt").read_text()
    m = re.search(r"^%s = (0x[0-9A-Fa-f]+);" % re.escape(old), text, re.M)
    if not m:
        raise ValueError(f"{game}: {old} not in symbol_addrs.txt")
    if old == new:
        return
    if re.search(r"^%s = " % re.escape(new), text, re.M):
        raise ValueError(f"{game}: {new} already in symbol_addrs.txt")
    addr = m.group(1)
    write(cfg / "symbol_addrs.txt", text[:m.start()] + new + text[m.start() + len(old):])
    # an smt3 alias of the new name at the same address is now the symbol itself
    rewrite(cfg / "smt3_symbols.txt",
            lambda t: re.sub(r"^%s = %s;\n" % (re.escape(new), addr), "", t, flags=re.M | re.I))
    text = (cfg / "name_sources.txt").read_text()
    m = re.search(r"^%s\t%s\t.*$" % (addr, re.escape(old)), text, re.M | re.I)
    if not m:
        raise ValueError(f"{game}: {old} not in name_sources.txt")
    row = name_source_row(m.group(0).split("\t")[0], new, old, reason)
    write(cfg / "name_sources.txt", text[:m.start()] + row + text[m.end():])
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


def doc_lines(path: str, text: str, games: frozenset) -> list[tuple[str, frozenset]]:
    """A docs page: a line belongs to the games its "## name (unit, raidouN ...)" section names, a table row to
    the games it names."""
    out, section = [], games
    for line in text.splitlines(keepends=True):
        named = frozenset(GAME_WORD.findall(line)) & games
        if path.endswith(".md") and line.startswith("## "):
            section = named or games
        out.append((line, named or games if path.endswith(".tsv") else section))
    return out


def game_lines(path: str, text: str) -> list[tuple[str, frozenset]]:
    """(line, games that read it) for each line; a src/<game>/ file is that game's alone."""
    games = frozenset(g for g in GAMES if f"src/{g}/" in path) or frozenset(GAMES)
    if path.startswith("docs/"):
        return doc_lines(path, text, games)
    if not path.endswith((".c", ".h")) or not version_guard.GAME.search(text):
        return [(line, games) for line in text.splitlines(keepends=True)]
    kept = {g: version_guard.lines_for(text, g) for g in games}
    out = []
    for i, line in enumerate(text.splitlines(keepends=True)):
        read = frozenset(g for g in games if kept[g][i][1])
        if not read and version_guard.DIRECTIVE.match(line) and version_guard.GAME.search(line):
            out.append((line, frozenset()))  # a VERSION_ directive: its comment names each game's symbol as it is
        else:
            out.append((line, read or games))
    return out


def is_use(line: str, old: str) -> bool:
    """A line that needs this game's own symbol: not its INCLUDE_ASM and not a one-line prototype."""
    return not re.match(r"\s*(INCLUDE_ASM\(.*\b%s\);|\w[\w\s*]*?[\s*]%s\s*\([^;{=]*\)\s*;)\s*$"
                        % ((re.escape(old),) * 2), line)


CODE, COMMENT, LITERAL = "c", "/", '"'


def classify(text: str) -> list[tuple[str, str, bool, bool]]:
    """(line, kinds, starts in a block comment, ends in one) for each line; kinds has one of CODE, COMMENT, LITERAL per
    character. A literal's quotes count as LITERAL, a comment's delimiters as COMMENT."""
    out, in_comment = [], False
    for line in text.splitlines(keepends=True):
        starts, kinds, i, quote = in_comment, [], 0, ""
        while i < len(line):
            two = line[i:i + 2]
            if in_comment:
                kinds += COMMENT * (2 if two == "*/" else 1)
                in_comment = two != "*/"
                i += 2 if two == "*/" else 1
            elif quote:
                kinds += LITERAL * (2 if line[i] == "\\" else 1)
                if line[i] == quote:
                    quote = ""
                i += 2 if line[i] == "\\" else 1
            elif two == "//":
                kinds += COMMENT * (len(line) - i)
                i = len(line)
            elif two == "/*":
                kinds += COMMENT * 2
                in_comment, i = True, i + 2
            elif line[i] in "\"'":
                quote = line[i]
                kinds += LITERAL
                i += 1
            else:
                kinds += CODE
                i += 1
        out.append((line, "".join(kinds[:len(line)]), starts, in_comment))
    return out


def code_of(line: str, kinds: str) -> str:
    """The line with comments and literals blanked."""
    return "".join(ch if k == CODE or ch == "\n" else " " for ch, k in zip(line, kinds))


def substitute(word: re.Pattern, new: str, line: str, kinds: str) -> str:
    """Rename the matches in code and comments; literals keep their text."""
    return word.sub(lambda m: m.group(0) if kinds[m.start()] == LITERAL else new, line)


def check_balance(path: str, text: str) -> None:
    """ValueError unless every #if/#ifdef/#ifndef has its #endif, #else/#elif/#endif have an open #if and no
    #else or #elif follows an #else."""
    open_ifs = []  # per open #if: whether its #else was seen
    for number, (line, kinds, starts, _) in enumerate(classify(text), 1):
        m = None if starts else re.match(r"\s*#\s*(if|ifdef|ifndef|elif|else|endif)\b", code_of(line, kinds))
        if not m:
            continue
        kind = m.group(1)
        if kind.startswith("if"):
            open_ifs.append(False)
        elif not open_ifs:
            raise ValueError(f"{path}:{number}: #{kind} without #if after renaming")
        elif open_ifs[-1] and kind != "endif":
            raise ValueError(f"{path}:{number}: #{kind} after #else after renaming")
        elif kind == "endif":
            open_ifs.pop()
        elif kind == "else":
            open_ifs[-1] = True
    if open_ifs:
        raise ValueError(f"{path}: {len(open_ifs)} unterminated #if after renaming")
    if version_guard.GAME.search(text):
        for game in GAMES:
            version_guard.lines_for(text, game)


def rename_text(path: str, text: str, old: str, new: dict[str, str], has: dict[str, bool],
                unused: frozenset = frozenset()) -> str:
    """Rename old to new[game] in the lines each game reads (new[game] == old: that game keeps it). `has`: games
    where old is a symbol; `unused`: those whose C never uses it, so a line both games read is the other game's.
    A C line games would name differently gets a VERSION_ guard when the name is used in its code. A line that
    cannot take one (a preprocessor directive, a macro continuation, a line in or opening a block comment) is
    refused with ValueError; a name only in a comment is left as is, as is a docs line, with a warning. Names in
    string and character literals are never renamed. The result is checked for #if/#endif balance."""
    word = re.compile(r"\b%s\b" % re.escape(old))
    c_file = path.endswith((".c", ".h"))
    lines = game_lines(path, text)
    kinds_of = classify(text) if c_file else [(line, CODE * len(line), False, False) for line, _ in lines]
    out = []
    prev = ""
    guarded = False
    for number, ((line, games), (_, kinds, starts, ends)) in enumerate(zip(lines, kinds_of), 1):
        if games and word.search(line):
            relevant = [g for g in sorted(games) if has.get(g)] or sorted(new)
            if len(relevant) > 1:
                relevant = [g for g in relevant if g not in unused] or relevant
            names = {g: new.get(g, old) for g in relevant}
            code = code_of(line, kinds)
            if len(set(names.values())) == 1:
                line = substitute(word, names[relevant[0]], line, kinds)
            elif not c_file:
                print(f"{path}:{number}: left {old} on a line both games read: {line.strip()}")
            elif not word.search(code):
                print(f"{path}:{number}: left {old} in a comment or literal both games read: {line.strip()}")
            else:
                why = ("a preprocessor directive" if re.match(r"\s*#", code) and not starts else
                       "inside a macro" if prev.rstrip("\n").endswith("\\") or line.rstrip("\n").endswith("\\") else
                       "in a block comment" if starts or ends else "")
                if why:
                    raise ValueError(f"{path}:{number}: {old} would be {names} on a line both games read, {why};"
                                     f" guard it by hand: {line.strip()}")
                body = line if line.endswith("\n") else line + "\n"
                body_kinds = kinds if line.endswith("\n") else kinds + CODE
                first, *rest = relevant
                line = (f"#ifdef {version_guard._macro(first)}\n" + substitute(word, names[first], body, body_kinds)
                        + "#else\n" + substitute(word, names[rest[0]], body, body_kinds) + "#endif\n")
                guarded = True
                print(f"{path}: guarded {old} -> {names}")
        out.append(line)
        prev = line
    result = "".join(out)
    if guarded:
        check_balance(path, result)
    return result


def rename_sources_per_game(tree: Path, old: str, new: dict[str, str], has: dict[str, bool]) -> int:
    # --untracked: a batch's new headers and units are not added yet
    files = subprocess.run(["git", "grep", "--untracked", "-l", "-w", old, "--", "src", "include", "docs"], cwd=tree,
                           capture_output=True, text=True).stdout.split()
    word = re.compile(r"\b%s\b" % re.escape(old))
    used = set()
    for f in files:
        if not f.endswith((".c", ".h")):
            continue
        text = (tree / f).read_text()
        for (line, games), (_, kinds, _, _) in zip(game_lines(f, text), classify(text)):
            code = code_of(line, kinds)
            if word.search(code) and is_use(code, old):
                used |= games
    renamed = {g for g, n in new.items() if n != old}
    if not renamed:
        return 0
    unused = frozenset(g for g in GAMES if has.get(g) and g not in renamed and g not in used)
    global journal
    own = journal is None
    if own:
        journal = {}
    try:
        for f in files:
            rewrite(tree / f, lambda t: rename_text(f, t, old, new, has, unused))
    except ValueError:
        if own:
            restore(journal)
        raise
    finally:
        if own:
            journal = None
    return len(files)


def symbols(tree: Path, game: str) -> set[str]:
    path = tree / "config" / game / "symbol_addrs.txt"
    return set(re.findall(r"^(\w+) = ", path.read_text(), re.M)) if path.exists() else set()


def complete(tree: Path, rows: list[tuple[str, str, str, str]],
             one_game: set[str] = frozenset(), split: set[str] = frozenset()) -> tuple[list[tuple[str, str, str, str]], list]:
    """Rows plus the other game's row for a name both games share. The other game keeps its name, with no row, when
    its symbol is another function: it has its own row for old (old -> old keeps it), already holds the new name,
    or the name is in `split`. Refuses (ValueError) a shared name whose function is in `one_game` (r1only.txt:
    the other game's symbol may be another function) unless the other game's row says which."""
    games = GAMES
    names = {g: symbols(tree, g) for g in games}
    given = {(g, old) for g, old, _, _ in rows}
    out, added = [], []
    for row in rows:
        game, old, new, reason = row
        todo = [row]
        for other in games:
            if other == game or old == new or (other, old) in given or old not in names[other] or old in split:
                continue
            if new in names[other]:
                print(f"renames: {other} keeps {old}: it already has {new}, so its {old} is another function")
                continue
            if old in one_game or new in one_game:
                raise ValueError(f"{game} rename {old} -> {new}: {old} is shared with {other} but the function is "
                                 f"in r1only.txt; add a {other} row ({old} -> {old} if it keeps the name)")
            todo.append((other, old, new, reason))
        for g, o, n, _ in todo:
            names[g].discard(o)
            names[g].add(n)
        out += todo
        added += todo[1:]
    return out, added


def restore(saved: dict[Path, str | None]) -> None:
    for path, text in saved.items():
        if text is None:
            path.unlink(missing_ok=True)
        else:
            path.write_text(text)


def apply(tree: Path, rows: list[tuple[str, str, str, str]], dry_run: bool = False) -> list[Path]:
    """Rows (game, old, new, reason) in order: config per game, then source text once per old name, each game's
    lines taking that game's new name. All or nothing: a refused row (ValueError) restores every file it wrote.
    Returns the files written; `dry_run` restores them before returning."""
    global journal
    names = {g: symbols(tree, g) for g in GAMES}
    steps: dict[str, tuple[dict[str, str], dict[str, bool]]] = {}
    for game, old, new, _reason in rows:
        if old not in steps:
            steps[old] = ({}, {g: old in names[g] for g in GAMES})
        steps[old][0].setdefault(game, new)
        names[game].discard(old)
        names[game].add(new)
    journal = {}
    try:
        for game, old, new, reason in rows:
            rename_config(tree, game, old, new, reason)
        for old, (new, has) in steps.items():
            rename_sources_per_game(tree, old, new, has)
    except ValueError:
        restore(journal)
        raise
    finally:
        changed, journal = journal, None
    if dry_run:
        restore(changed)
    return sorted(changed)


def drop(tree: Path, rows: list[tuple[str, str]]) -> None:
    """Delete labels (game, name): symbol_addrs, the name_sources row and the data pair row."""
    for game, name in rows:
        cfg = tree / "config" / game
        text = (cfg / "symbol_addrs.txt").read_text()
        m = re.search(r"^%s = (0x[0-9A-Fa-f]+);.*\n" % re.escape(name), text, re.M)
        if not m:
            raise ValueError(f"{game}: {name} not in symbol_addrs.txt")
        addr = m.group(1)
        write(cfg / "symbol_addrs.txt", text[:m.start()] + text[m.end():])
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


def read_table(path: Path) -> list[tuple[str, str, str, str]]:
    rows = []
    for number, line in enumerate(path.read_text().splitlines(), 1):
        f = line.split("\t")
        if not line.strip() or line.startswith("#"):
            continue
        if len(f) < 4 or f[0] not in GAMES:
            raise SystemExit(f"{path}:{number}: expected game<TAB>old<TAB>new<TAB>reason, game one of {GAMES}")
        rows.append((f[0], f[1], f[2], f[3]))
    return rows


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("tables", nargs="+", type=Path, help="game, old, new, reason rows (tab separated)")
    ap.add_argument("--tree", type=Path, default=ROOT, help="tree to rename in (default: this checkout)")
    ap.add_argument("--dry-run", action="store_true", help="print the rows and files, then restore the tree")
    args = ap.parse_args(argv)
    rows = [r for t in args.tables for r in read_table(t)]
    try:
        rows, added = complete(args.tree, rows)
        for game, old, new, _ in added:
            print(f"renames: added {game} {old} -> {new}, a name both games share")
        changed = apply(args.tree, rows, args.dry_run)
    except ValueError as e:
        print(f"renames: refused: {e}", file=sys.stderr)
        return 1
    for game, old, new, _ in rows:
        print(f"{game}\t{old}\t{new}" + ("\t(keeps)" if old == new else ""))
    for path in changed:
        print(f"wrote {path.relative_to(args.tree)}")
    if args.dry_run:
        print("renames: dry run, tree restored")
        return 0
    return subprocess.run([sys.executable, "tools/revisit_names.py"], cwd=args.tree).returncode


if __name__ == "__main__":
    sys.exit(main())
