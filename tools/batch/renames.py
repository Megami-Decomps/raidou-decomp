"""Apply a batch's symbol renames and label deletions to a tree, in both games' config and in the sources."""
import re
import subprocess
from pathlib import Path

CARRIED = ("ida_protos.txt", "claims.tsv", "tu_evidence.tsv", "data_c.tsv", "smt3_c.tsv", "smt3_data.tsv")
PAIRS = ("r1_r2_pairs.tsv", "r1_r2_data_pairs.tsv")
SOURCES = ("SMT3", "HD", "DDS1", "DDS2", "dds1")


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
    files = subprocess.run(["git", "grep", "-l", "-w", old, "--", "src", "include", "docs"], cwd=tree,
                           capture_output=True, text=True).stdout.split()
    for f in files:
        rewrite(tree / f, lambda t: re.sub(r"\b%s\b" % re.escape(old), new, t))
    return len(files)


def apply(tree: Path, rows: list[tuple[str, str, str, str]]) -> None:
    """Rows (game, old, new, reason) in order: config per game, then source text once per old name."""
    for game, old, new, reason in rows:
        rename_config(tree, game, old, new, reason)
    done = set()
    for _game, old, new, _reason in rows:
        if old not in done:
            done.add(old)
            rename_sources(tree, old, new)


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
