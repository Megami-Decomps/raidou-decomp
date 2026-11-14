#!/usr/bin/env python3
"""Apply Raidou HD names from the mapping tables (functions.tsv, globals.tsv).

    python3 tools/xgame/hd_names.py functions|globals TABLE [--dry-run]

functions takes action=rename rows (band A and B). globals takes confidence=high rows.
A name two symbols would share, or one already in use, gets `_<module>` appended.
Old names that carried C is keyed on (smt3_c.tsv, smt3_data.tsv, smt3_symbols.txt) stay."""
from __future__ import annotations

import argparse
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common_names as cn  # noqa: E402

ROOT = cn.ROOT
V1, V2 = cn.V1, cn.V2


def carried_names():
    out = set()
    for v in (V1, V2):
        for f in ("smt3_c.tsv", "smt3_data.tsv"):
            for line in (ROOT / "config" / v / f).read_text().splitlines():
                if line and not line.startswith("#"):
                    out.add(line.split("\t")[0])
        out |= set(cn.aliases(v))
    return out


def load(kind, path):
    """[(r1, r2, old, new, module, how, note)]"""
    rows = []
    for line in Path(path).read_text().splitlines():
        if line.startswith("#") or not line:
            continue
        c = line.split("\t")
        if c[9] != ("rename" if kind == "func" else "high"):
            continue
        r1, r2, old = int(c[0], 16), int(c[1], 16), c[2]
        if kind == "func":
            new, mod = c[4], c[5]
            how = "hd-inferred" if c[6] == "I" else "hd"
            note = f"hd: Raidou HD {mod}.{new}, score {c[7]} band {c[8]}"
        else:
            new, mod = c[3], c[4]
            how = "hd"
            note = f"hd: Raidou HD {mod}.{new}, {c[7]} of {c[8]} functions agree, high confidence"
        rows.append([r1, r2, old, new, mod, how, note])
    return rows


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("kind", choices=("functions", "globals"))
    ap.add_argument("table")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args()
    kind = "func" if a.kind == "functions" else "data"
    pairs = cn.read_table(cn.FUNC_PAIRS if kind == "func" else cn.DATA_PAIRS)
    fwd = {int(r["r1_addr"], 16): int(r["r2_addr"], 16) for r in pairs}
    sym = {v: cn.symbol_rows(v) for v in (V1, V2)}
    keep = carried_names()
    rows, skipped = [], defaultdict(list)
    for r in load(kind, a.table):
        r1, r2, old = r[0], r[1], r[2]
        if kind == "func" and fwd.get(r1) not in (None, r2):
            skipped["pair differs"].append(old)
        elif sym[V1].get(r1, cn.default_name(kind, r1)) != old and \
                sym[V1].get(r1) != old:
            skipped["old name changed"].append(old)
        elif old in keep:
            skipped["carried name"].append(old)
        elif not (cn.is_placeholder(old) or kind == "func"):
            skipped["not placeholder"].append(old)
        else:
            rows.append(r)
    taken = defaultdict(set)
    for v in (V1, V2):
        for ad, n in sym[v].items():
            taken[n].add(("x", ad))
    for n in keep:
        taken[n].add(("keep", 0))
    moved = {r[2] for r in rows}
    by_new = defaultdict(list)
    for r in rows:
        by_new[r[3]].append(r)
    suffixed = 0
    for n, rs in by_new.items():
        clash = len(rs) > 1 or any(t for t in taken.get(n, ()) if not (t[0] == "x" and sym[V1].get(t[1]) in moved))
        if clash:
            for r in rs:
                r[3] = f"{n}_{r[4]}"
                suffixed += 1
    final = defaultdict(list)
    for r in rows:
        final[r[3]].append(r)
    for n, rs in final.items():
        if len(rs) > 1 or n in taken and any(t[0] == "keep" or sym[V1].get(t[1]) not in moved for t in taken[n]):
            for r in rs:
                r[3] = f"{r[3]}_{r[0]:08X}"
    print(f"{a.kind}: {len(rows)} to apply, {suffixed} suffixed by module;",
          {k: len(v) for k, v in skipped.items()})
    if a.dry_run:
        return
    src_note = {}
    for v in (V1, V2):
        for line in (ROOT / "config" / v / "name_sources.txt").read_text().splitlines():
            c = line.split("\t")
            if len(c) >= 4 and not line.startswith("#"):
                src_note[(v, int(c[0], 16))] = c[3]
    renames = {V1: {}, V2: {}}
    for v, side in ((V1, 0), (V2, 1)):
        new_rows, notes = {}, {}
        for r in rows:
            ad, old, new = r[side], r[2], r[3]
            cur = sym[v].get(ad, cn.default_name(kind, ad))
            note = r[6]
            if not cn.is_placeholder(cur):
                note += f"; replaces {cur}: {src_note.get((v, ad), '')}"
            new_rows[ad] = f"{new} = 0x{ad:08X};" + (" // type:func" if kind == "func" else "")
            notes[ad] = f"0x{ad:08X}\t{new}\t{r[5]}\t{note}"
            renames[v][cur] = new
        cn.set_names(v, new_rows, notes)
    newname = {r[0]: r[3] for r in rows}
    for path in (cn.FUNC_PAIRS, cn.DATA_PAIRS):
        table = cn.read_table(path)
        for t in table:
            if (n := newname.get(int(t["r1_addr"], 16))) and ((path == cn.FUNC_PAIRS) == (kind == "func")):
                t["name"] = n
        head = cn.FUNC_HEAD if path == cn.FUNC_PAIRS else cn.DATA_HEAD
        path.write_text(head + "\n" + "\n".join("\t".join(t[c] for c in head.split("\t")) for t in table) + "\n")
    existing = {v: set(sym[v].values()) | set(cn.aliases(v)) for v in (V1, V2)}
    changed, unresolved = cn.rename_tree(renames, existing, {V1: set(), V2: set()})
    for v in (V1, V2):
        p = ROOT / "config" / v / "ida_protos.txt"
        out = []
        for line in p.read_text().splitlines():
            c = line.split(" ", 2)
            if len(c) == 3 and c[1] in renames[v]:
                n = renames[v][c[1]]
                line = f"{c[0]} {n} {c[2].replace(c[1], n)}"
            out.append(line)
        p.write_text("\n".join(out) + "\n")
    print("files changed:", dict(changed), "unresolved:", dict(unresolved))
    (Path.home() / ".local/state/raidou-decomp-work/hdnames" / f"skipped-{a.kind}.txt").write_text(
        "\n".join(f"{k}\t{n}" for k, v in skipped.items() for n in v) + "\n")


main()
