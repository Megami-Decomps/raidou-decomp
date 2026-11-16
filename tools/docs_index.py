#!/usr/bin/env python3
"""Rebuild docs/matching/README.md and INDEX.tsv from the per-unit draft notes.

    python3 tools/docs_index.py [--check]   # --check: fail if stale, a draft has no entry,
                                            # or a FAKEMATCH_ASM_* user has no Fakematch line
Entry heading: "## <function> (<unit>, <game> <size>[, <game> <size>])".
A line "Fragile: <cause>" in an entry marks a function whose result moves with
declarations alone. A line "Fakematch: <what was tried first>" is required for every
function that uses a FAKEMATCH_ASM_* macro; it is also the one thing that lets a
matched function have an entry. INDEX.tsv carries both lines in its last columns.
"""
import argparse
import pathlib
import re
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
from units import VERSIONS, unit_name, unit_sources  # noqa: E402
from version_guard import view  # noqa: E402

ROOT = pathlib.Path(__file__).resolve().parent.parent
MATCHING = ROOT / "docs/matching"

HEADING = re.compile(r"^## (.+?)(?: \(([^,]+), ([^)]+)\))?$")
GAME = re.compile(r"\b(" + "|".join(VERSIONS) + r")\b")
FRAGILE = re.compile(r"^Fragile: (.+?)(?=\n\s*\n|\Z)", re.M | re.S)
GUARD = re.compile(r"^\s*#\s*(ifdef|ifndef|if)\b(.*)")
FAKEMATCH = re.compile(r"^Fakematch: (.+?)(?=\n\s*\n|\Z)", re.M | re.S)
USE = re.compile(r"\bFAKEMATCH_ASM_\w+")
FN_HEAD = re.compile(r"^[A-Za-z_][^;=]*?\b(\w+)\s*\(")
FN_DECL = re.compile(r"\b(\w+)\s*\([^()]*\)\s*FAKEMATCH_ASM_")
ASM = re.compile(r'INCLUDE_ASM\([^,]+,\s*"[^"]+",\s*(\w+)\)')


def entry_names(names: str) -> list[str]:
    """"a, b and c" -> [a, b, c]: one entry may cover siblings with one residual."""
    return [n for n in re.split(r",\s*|\s+and\s+", names) if n]


def draft_functions(text: str) -> list[str]:
    """Functions whose C sits behind #ifdef NON_MATCHING, named by its INCLUDE_ASM fallback."""
    out, depth, inside = [], 0, False
    for line in text.split("\n"):
        directive = GUARD.match(line)
        if directive:
            depth += 1
            if depth == 1 and directive.group(1) == "ifdef" \
                    and directive.group(2).split()[:1] == ["NON_MATCHING"]:
                inside = True
            continue
        if re.match(r"^\s*#\s*endif\b", line):
            depth -= 1
            if depth == 0:
                inside = False
            continue
        if inside and (m := ASM.search(line)):
            out.append(m.group(1))
    return out


def strip_noise(text: str) -> list[str]:
    """Lines of text with comments and string literals blanked, line count kept."""
    text = re.sub(r"/\*.*?\*/", lambda m: re.sub(r"[^\n]", " ", m.group(0)), text, flags=re.S)
    out = []
    for line in text.split("\n"):
        line = re.sub(r'"(?:[^"\\]|\\.)*"|\'(?:[^\'\\]|\\.)*\'', '""', line)
        out.append(re.sub(r"//.*$", "", line))
    return out


def fakematch_functions(text: str) -> set[str]:
    """Functions that use a FAKEMATCH_ASM_* macro: in the body, or on the declaration
    of the function (a retail name). A use on data names no function and is skipped."""
    out, depth, head = set(), 0, None
    for line in strip_noise(text):
        if depth == 0 and (m := FN_HEAD.match(line)):
            head = m.group(1)
        if USE.search(line):
            if depth > 0 and head:
                out.add(head)
            elif depth == 0 and (m := FN_DECL.search(line)):
                out.add(m.group(1))
        depth += line.count("{") - line.count("}")
    return out


def fakematch_uses() -> set[tuple[str, str, str]]:
    """(function, unit, game) for every function that uses a FAKEMATCH_ASM_* macro."""
    out = set()
    for version in VERSIONS:
        for unit, path in unit_sources(version).items():
            text = view(path.read_text(), version)
            for fn in fakematch_functions(text):
                out.add((fn, unit, version))
    return out


def drafts() -> set[tuple[str, str, str]]:
    """(function, unit, game) for every draft each game compiles."""
    out = set()
    for version in VERSIONS:
        for unit, path in unit_sources(version).items():
            text = view(path.read_text(), version)
            for fn in draft_functions(text):
                out.add((fn, unit, version))
    return out


def matching_rows() -> list[tuple[str, ...]]:
    rows = []
    for page in sorted(MATCHING.rglob("*.md")):
        if page.name == "README.md":
            continue
        lines = page.read_text().split("\n")
        heads = [i for i, l in enumerate(lines) if l.startswith("## ")]
        for n, i in enumerate(heads):
            end = heads[n + 1] if n + 1 < len(heads) else len(lines)
            m = HEADING.match(lines[i])
            if not m:
                continue
            body = "\n".join(lines[i + 1:end])
            # "N of M words differ" is the convention; a bare "N of M words" is
            # captured as written so the lint below can name it.
            d = (re.search(r"\b([\w-]+ of [\w-]+ words?[^.;]{0,40}?"
                           r"(?:differ|match)\w*)", body)
                 or re.search(r"\b([\w-]+ of [\w-]+ words?)\b", body))
            rows.append((
                m.group(1),
                m.group(2) or "",
                m.group(3) or "",
                re.sub(r"\s+", " ", d.group(1)) if d else "",
                page.relative_to(ROOT).as_posix(),
                str(i + 1),
                re.sub(r"\s+", " ", f.group(1)).strip() if (f := FRAGILE.search(body)) else "",
                re.sub(r"\s+", " ", f.group(1)).strip() if (f := FAKEMATCH.search(body)) else "",
            ))
    return rows


def fragile_names(rows: list[tuple[str, ...]] | None = None) -> set[str]:
    """Functions whose entry carries a Fragile line."""
    out = set()
    for row in rows if rows is not None else matching_rows():
        if row[6]:
            out.update(entry_names(row[0]))
    return out


def fakematch_entries(rows: list[tuple[str, ...]]) -> set[tuple[str, str, str]]:
    """(function, unit, game) for every entry that has a Fakematch line."""
    return entries([r for r in rows if r[7]])


def entries(rows: list[tuple[str, ...]]) -> set[tuple[str, str, str]]:
    """(function, unit, game) for every entry, the games read from the size field."""
    out = set()
    for names, unit, size, *_ in rows:
        for fn in entry_names(names):
            for game in GAME.findall(size):
                out.add((fn, unit, game))
    return out


def page_unit(page: pathlib.Path) -> str:
    return page.relative_to(MATCHING).with_suffix("").as_posix()


def misplaced(rows: list[tuple[str, ...]]) -> list[str]:
    """Entries filed on a page other than their unit's, or with no game named."""
    bad = []
    for names, unit, size, _d, page, line, *_ in rows:
        where = f"{page}:{line}"
        if unit != page_unit(ROOT / page):
            bad.append(f"{where}: unit {unit!r} belongs on docs/matching/{unit}.md")
        if not GAME.search(size):
            bad.append(f"{where}: the size field names no game ({', '.join(VERSIONS)})")
    return bad


def matching_readme(rows: list[tuple[str, ...]]) -> str:
    """A browsable list of the unit pages, most entries first."""
    counts, games = {}, {}
    for _fn, unit, size, _d, page, _l, *_ in rows:
        counts[(page, unit)] = counts.get((page, unit), 0) + 1
        games.setdefault(page, set()).update(GAME.findall(size))
    table = sorted(((n, page, unit) for (page, unit), n in counts.items()),
                   key=lambda r: (-r[0], r[1]))
    body = ["# Notes on the non-matching drafts, by unit", "",
            "Generated by `tools/docs_index.py`. `INDEX.tsv` beside this file is",
            "the searchable form: one row per entry with its distance and the",
            "page and line to open. Every `NON_MATCHING` draft in `src/` has an",
            "entry: the measured distance, what blocks the match and the shapes",
            "already tried. A matched function that uses a `FAKEMATCH_ASM_*` macro",
            "also has one, with a `Fakematch:` line saying what was tried first.",
            "See [the source form](../CONTRIBUTING.md#non-matching-c).", "",
            "The size field of each heading names the game it was measured in.",
            "Distances say whether words match or differ, and give both native",
            "and emitted sizes when they differ. \"Call records\" are function entry",
            "and exit captures from retail Raidou 1 under PCSX2 (boot, ingame and",
            "menu scenarios); an entry says which paths they covered.", "",
            "| Entries | Unit | Games | Page |", "|---|---|---|---|"]
    for n, page, unit in table:
        rel = page.split("docs/matching/", 1)[1]
        body.append(f"| {n} | {unit} | {', '.join(sorted(games[page]))} | [{rel}]({rel}) |")
    return "\n".join(body) + "\n"


def render(header: str, rows: list[tuple[str, ...]]) -> str:
    return "\n".join([header] + ["\t".join(r) for r in rows]) + "\n"


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--check", action="store_true",
                    help="exit non-zero if an index is stale or a draft or FAKEMATCH_ASM_* user has no entry")
    args = ap.parse_args(argv)
    rows = matching_rows()
    outputs = {
        MATCHING / "README.md": matching_readme(rows),
        MATCHING / "INDEX.tsv": render("function\tunit\tsize\tdistance\tpage\tline\tfragile\tfakematch", rows),
    }
    failed = 0
    for path, want in outputs.items():
        have = path.read_text() if path.exists() else ""
        if want == have:
            continue
        if args.check:
            print(f"{path.relative_to(ROOT)} is stale; run tools/docs_index.py")
            failed += 1
        else:
            path.write_text(want)
            print(f"{path.relative_to(ROOT)}: rewritten")
    for problem in misplaced(rows):
        print(problem)
        failed += 1
    have, want = entries(rows), drafts()
    for fn, unit, game in sorted(want - have):
        print(f"{game} {unit}: draft {fn} has no entry in docs/matching/{unit}.md")
        failed += 1
    uses, noted = fakematch_uses(), fakematch_entries(rows)
    for fn, unit, game in sorted(uses - noted):
        print(f"{game} {unit}: {fn} uses FAKEMATCH_ASM_* and needs a Fakematch: line "
              f"in docs/matching/{unit}.md")
        failed += 1
    for fn, unit, game in sorted(have - want - (noted & uses)):
        print(f"docs/matching/{unit}.md: {fn} is not a {game} draft; drop or fix the entry")
        failed += 1
    vague = [r for r in rows if r[3] and "match" not in r[3] and "differ" not in r[3]]
    for r in vague:
        print(f"{r[4]}:{r[5]}: write \"N of M words differ\" or \"match\", not \"{r[3]}\"")
        failed += 1
    if not args.check:
        print(f"docs/matching: {len(rows)} entries, {len(want)} drafts, "
              f"{len(uses)} fakematch users")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
