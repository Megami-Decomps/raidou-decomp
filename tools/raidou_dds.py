#!/usr/bin/env python3
"""Names and C from dds-decomp for Raidou functions identical to DDS1/DDS2 code.

    python3 tools/raidou_dds.py names|pairs --dds DIR   (see docs/shared-c.md)
"""

from __future__ import annotations

import argparse
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import raidou_c as rc  # noqa: E402
from retail_elf import VERSIONS  # noqa: E402

MAP_FILE = rc.MAP / "dds_unique_in_raidou.tsv"
SERIAL = {"dds1": "SLUS_209.74", "dds2": "SLUS_211.52"}
PLACEHOLDER = re.compile(r"(func|sub|nullsub|D|jtbl|lbl|L)_[0-9A-Fa-f]+\w*")


class Side:
    """One DDS game: names, C functions, masked bodies and name provenance."""

    def __init__(self, root, game):
        self.game = game
        syms = rc.repo_symbols(root, game)
        self.name_at = {}
        for name, addr in syms.items():
            self.name_at.setdefault(addr, name)
        self.c = rc.c_functions(root, game)
        for addr, (name, _) in self.c.items():
            self.name_at.setdefault(addr, name)
        self.bodies = rc.split_bodies(root, game)
        self.source = {}
        for line in (root / "config" / game / "name_sources.txt").read_text().splitlines():
            cols = line.split("\t")
            if len(cols) >= 4 and not line.startswith("#"):
                self.source[int(cols[0], 16)] = (cols[2], cols[3])

    def provenance(self, addr):
        """(evidence|inferred, how dds-decomp got the name)."""
        kind, note = self.source.get(addr, ("inferred", ""))
        if note.startswith("SMT3HD"):
            how = "SMT3HD name"
        elif "evidence" in kind.split("|")[0]:
            how = "evidence"
        elif note:
            how = "descriptive"
        else:
            how = "no provenance row"
        return kind, how


def map_rows(version):
    """Raidou address -> [(game, DDS address, method)]."""
    out = defaultdict(list)
    lines = MAP_FILE.read_text().splitlines()
    head = lines[0].split("\t")
    for line in lines[1:]:
        row = dict(zip(head, line.split("\t")))
        if row["game"] != version:
            continue
        for game in rc.DDS_GAMES:
            if row.get(f"{game}_ea"):
                out[int(row["ea"], 16)].append((game, int(row[f"{game}_ea"], 16), row[f"{game}_method"]))
    return out


def verified(version, sides):
    """(src, unit, fn, address, words, [(game, DDS address, method)]) for each Raidou
    function whose map partners have the same masked words and slot kinds."""
    partners = map_rows(version)
    out = []
    rows = rc.raidou_asm_functions(version)
    for path in sorted((ROOT / "asm" / version).glob("code_*.s")):  # gaps: asm units without C
        rows += [(None, path.stem, fn, *body) for fn, body in rc.parse_asm(path).items()]
    for src, unit, fn, addr, words, slots in rows:
        good = []
        for game, ea, method in partners.get(addr, ()):
            theirs = sides[game].bodies.get(ea)
            if theirs and theirs[0] == words and [k for k, _ in theirs[1]] == [k for k, _ in slots]:
                good.append((game, ea, method))
        out.append((src, unit, fn, addr, words, good))
    return out


def load_sides(args):
    rc.DDS = args.dds.expanduser().resolve()
    return {g: Side(rc.DDS, g) for g in rc.DDS_GAMES}


def cmd_names(args):
    sides = load_sides(args)
    out = rc.OUT / "dds"
    out.mkdir(parents=True, exist_ok=True)
    for version in VERSIONS:
        taken = {m.group(1) for line in (ROOT / "config" / version / "symbol_addrs.txt").read_text().splitlines()
                 if (m := rc.SYMROW.match(line))}
        counts = Counter()
        want = {}
        compiled = {fn for src in rc.unit_sources(version).values() for fn in rc.definitions(src.read_text())}
        for src, unit, fn, addr, words, good in verified(version, sides):
            if not re.fullmatch(r"func_[0-9A-F]{8}", fn) or fn in compiled:
                continue
            if not good:
                counts["no verified partner"] += 1
                continue
            named = [(g, ea, m) for g, ea, m in good
                     if (n := sides[g].name_at.get(ea)) and not PLACEHOLDER.fullmatch(n)]
            if not named:
                counts["DDS placeholder"] += 1
                continue
            names = {sides[g].name_at[ea] for g, ea, _ in named}
            if len(names) > 1:
                counts["DDS1/DDS2 names differ"] += 1
            # C first, then DDS1.
            game, ea, method = min(named, key=lambda x: (x[1] not in sides[x[0]].c, x[0]))
            want[addr] = (sides[game].name_at[ea], game, ea, method)
        uses = Counter(n for n, *_ in want.values())
        rows = []
        for addr, (name, game, ea, method) in sorted(want.items()):
            if name in taken or uses[name] > 1:
                counts["name taken" if name in taken else "name wanted twice"] += 1
                continue
            kind, how = sides[game].provenance(ea)
            rows.append(f"0x{addr:08X}\t{name}\t{kind}\tdds-decomp {game.upper()} name via {SERIAL[game]} "
                        f"0x{ea:08X}, masked-words-identical ({method} map pair); dds-decomp: {how}")
            counts[f"named ({how})"] += 1
        (out / f"names_{version}.tsv").write_text("\n".join(rows) + "\n")
        print(version, dict(sorted(counts.items())))


def cmd_rename(args):
    """Rename func_ references in the TUs after raidou_names.py --apply of names_<v>.tsv."""
    texts = defaultdict(dict)
    for version in VERSIONS:
        new = {}
        for line in (rc.OUT / "dds" / f"names_{version}.tsv").read_text().splitlines():
            if line:
                addr, name = line.split("\t")[:2]
                new[f"func_{int(addr, 16):08X}"] = name
        for unit, src in rc.unit_sources(version).items():
            # Only INCLUDE_ASM lines: carried C may use SMT3's own func_ names.
            texts[unit][version] = re.sub(r"^(INCLUDE_ASM\([^,]+,[^,]+,\s*)(func_[0-9A-F]{8})\);$",
                                          lambda m: m.group(1) + new.get(m.group(2), m.group(2)) + ");",
                                          src.read_text(), flags=re.M)
    print(f"{rc.write_tus(texts)} files changed; {len(rc.shared_sources())} shared TUs")


def cmd_pairs(args):
    sides = load_sides(args)
    smt3 = {(r["version"], r["function"]) for r in rc.read_pairs(rc.OUT / "pairs.tsv")
            if r["status"] == "identical" and r["function"] == r["smt3_name"]}
    lines = ["version\tsrc\tfunction\taddress\tsmt3_address\tsmt3_name\tsmt3_src\twords\tstatus"]
    for version in VERSIONS:
        # SMT3's and dds-decomp's headers define the same types differently: no unit gets both.
        smt3_units = {r["src"] for r in rc.read_pairs(rc.OUT / "pairs.tsv")
                      if r["version"] == version and (version, r["function"]) in smt3}
        rows = [r for r in verified(version, sides) if r[0] and r[5] and r[0] not in smt3_units]
        # One DDS game per unit: two VERSION_DDS macros in one TU select conflicting headers.
        votes = defaultdict(Counter)
        for src, unit, fn, addr, words, good in rows:
            for game, ea, _ in good:
                if ea in sides[game].c and sides[game].c[ea][0] == fn:
                    votes[unit][game] += 1
        counts = Counter()
        for src, unit, fn, addr, words, good in rows:
            game = min(votes[unit], key=lambda g: (-votes[unit][g], g)) if votes[unit] else None
            hit = next(((g, ea) for g, ea, _ in good if g == game), None)
            if hit is None:
                status, ea, name, path = "other-game", good[0][1], "", ""
            elif hit[1] not in sides[game].c:
                status, ea, name, path = "dds-asm", hit[1], "", ""
            else:
                ea = hit[1]
                name, path = sides[game].c[ea]
                status = "identical" if name == fn else "name-differs"
            counts[status] += 1
            lines.append(f"{version}\t{src}\t{fn}\t0x{addr:08X}\t0x{ea:08X}\t{name}\t{path}\t{len(words)}\t{status}")
        print(version, dict(sorted(counts.items())))
    (rc.OUT / "pairs_dds.tsv").write_text("\n".join(lines) + "\n")


def cmd_headers(args):
    """Copy the dds-decomp headers the TUs' DDS declaration blocks include (and theirs) into include/dds/."""
    rc.DDS = args.dds.expanduser().resolve()
    want, todo = set(), []
    for src in rc.SRC.rglob("*.c"):
        todo += re.findall(r'^\s*#\s*include\s+"dds/([^"]+)"', src.read_text(errors="replace"), re.M)
    while todo:
        name = todo.pop()
        if name in want or not (rc.DDS / "include" / name).exists():
            continue
        want.add(name)
        todo += re.findall(r'^\s*#\s*include\s+"([^"]+)"', (rc.DDS / "include" / name).read_text(errors="replace"), re.M)
    out = ROOT / "include" / "dds"
    have = {p.relative_to(out).as_posix() for p in out.rglob("*.h")} if out.exists() else set()
    for name in sorted(want):
        dst = out / name
        dst.parent.mkdir(parents=True, exist_ok=True)
        text = (rc.DDS / "include" / name).read_text(errors="replace")
        if not dst.exists() or dst.read_text(errors="replace") != text:
            dst.write_text(text)
    for name in have - want:
        (out / name).unlink()
    print(f"{len(want)} dds-decomp headers in include/dds ({len(want - have)} new, {len(have - want)} removed)")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name, func in (("names", cmd_names), ("rename", cmd_rename), ("pairs", cmd_pairs), ("headers", cmd_headers)):
        p = sub.add_parser(name)
        p.add_argument("--dds", type=Path, default=rc.DDS, help="dds-decomp checkout, configured (split)")
        p.set_defaults(func=func)
    args = ap.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
