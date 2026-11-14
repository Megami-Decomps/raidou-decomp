#!/usr/bin/env python3
"""Rename symbols in both games from a table (r1, r2, old, new, how, note), carried names included.

    python3 tools/xgame/rename_names.py TABLE [--dry-run]

how `alias` renames a smt3_symbols.txt name, `text` one with no Raidou address; DDS names go to config/dds_names.txt."""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common_names as cn  # noqa: E402

ROOT = cn.ROOT
V1, V2 = cn.V1, cn.V2
DDS_NAMES = ROOT / "config" / "dds_names.txt"
DDS_HEAD = ("# dds-decomp name -> Raidou name, applied to DDS text tools/raidou_c.py carries\n"
            "# (tools/xgame/rename_names.py writes it)\n")


def read_dds_names():
    out = {}
    if DDS_NAMES.exists():
        for line in DDS_NAMES.read_text().splitlines():
            if line and not line.startswith("#"):
                old, new = line.split("\t")[:2]
                out[old] = new
    return out


def load(path):
    rows = []
    for line in Path(path).read_text().splitlines():
        if not line or line.startswith("#"):
            continue
        c = line.split("\t")
        r1 = int(c[0], 16) if c[0] else None
        r2 = int(c[1], 16) if c[1] else None
        rows.append((r1, r2, c[2], c[3], c[4], c[5] if len(c) > 5 else ""))
    return rows


def rewrite_rows(path, fn):
    lines = path.read_text().splitlines()
    path.write_text("\n".join(fn(line) for line in lines) + "\n")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("table")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args()
    rows = load(a.table)
    sym = {v: cn.symbol_rows(v) for v in (V1, V2)}
    alias = {v: cn.aliases(v) for v in (V1, V2)}
    renames = {V1: {}, V2: {}}
    sym_new = {V1: {}, V2: {}}
    errors = []
    for r1, r2, old, new, how, note in rows:
        for v, ad in ((V1, r1), (V2, r2)):
            if how == "text":
                renames[v][old] = new
                continue
            if ad is None:
                continue
            if how == "alias":
                if alias[v].get(old) != ad:
                    errors.append(f"{v}: alias {old} is not at 0x{ad:08X}")
                if sym[v].get(ad) != new:
                    errors.append(f"{v}: alias {old} -> {new}, but 0x{ad:08X} is {sym[v].get(ad)}")
                renames[v][old] = new
                continue
            if sym[v].get(ad) != old:
                errors.append(f"{v}: 0x{ad:08X} is {sym[v].get(ad)}, not {old}")
            renames[v][old] = new
            sym_new[v][ad] = new
    for v in (V1, V2):
        moved = set(renames[v])
        for ad, new in sym_new[v].items():
            holders = [x for x, n in sym[v].items() if n == new and x != ad and n not in moved]
            if holders or (new in alias[v] and alias[v][new] != ad):
                errors.append(f"{v}: {new} is already used")
    if errors:
        sys.exit("\n".join(errors))
    print(f"{len(rows)} rows: {len(sym_new[V1])} raidou1 and {len(sym_new[V2])} raidou2 symbols renamed")
    if a.dry_run:
        return
    for v in (V1, V2):
        ren = renames[v]
        p = ROOT / "config" / v / "symbol_addrs.txt"

        def sym_line(line, v=v):
            m = cn.ROW.match(line)
            if m and int(m[2], 16) in sym_new[v] and m[1] == sym[v][int(m[2], 16)]:
                return line.replace(m[1], sym_new[v][int(m[2], 16)], 1)
            return line
        rewrite_rows(p, sym_line)
        notes = {}
        for line in (ROOT / "config" / v / "name_sources.txt").read_text().splitlines():
            c = line.split("\t")
            if len(c) >= 4 and not line.startswith("#"):
                notes[int(c[0], 16)] = c
        for r1, r2, old, new, how, note in rows:
            ad = r1 if v == V1 else r2
            if ad is None or how in ("alias", "text"):
                continue
            prev = notes.get(ad, [f"0x{ad:08X}", old, "", ""])
            full = note if cn.is_placeholder(old) else f"{note}; replaces {old}: {prev[3]}"
            notes[ad] = [f"0x{ad:08X}", new, how, full]
        (ROOT / "config" / v / "name_sources.txt").write_text(
            "# addr\tname\tevidence|inferred|placeholder\tnote\n" + "\n".join("\t".join(notes[k]) for k in sorted(notes)) + "\n")
        for f in ("smt3_c.tsv", "smt3_data.tsv"):
            rewrite_rows(ROOT / "config" / v / f, lambda line, ren=ren: (
                "\t".join([ren.get(line.split("\t")[0], line.split("\t")[0])] + line.split("\t")[1:])
                if line and not line.startswith("#") else line))
        defined = set(cn.symbol_rows(v).values())
        out = []
        for line in (ROOT / "config" / v / "smt3_symbols.txt").read_text().splitlines():
            m = cn.ALIAS.match(line)
            if m and m[1] in ren:
                if ren[m[1]] in defined:
                    continue  # symbol_addrs.txt defines the new name at this address
                line = f"{ren[m[1]]} = 0x{m[2]};"
            out.append(line)
        (ROOT / "config" / v / "smt3_symbols.txt").write_text("\n".join(out) + "\n")

        def proto(line, ren=ren):
            c = line.split(" ", 2)
            if len(c) == 3 and c[1] in ren:
                return f"{c[0]} {ren[c[1]]} {re.sub(rf'\b{re.escape(c[1])}\b', ren[c[1]], c[2])}"
            return line
        rewrite_rows(ROOT / "config" / v / "ida_protos.txt", proto)
    for path, head in ((cn.FUNC_PAIRS, cn.FUNC_HEAD), (cn.DATA_PAIRS, cn.DATA_HEAD)):
        table = cn.read_table(path)
        for t in table:
            n = sym_new[V1].get(int(t["r1_addr"], 16))
            if n and t["name"] == sym[V1][int(t["r1_addr"], 16)]:
                t["name"] = n
        path.write_text(head + "\n" + "\n".join("\t".join(t[c] for c in head.split("\t")) for t in table) + "\n")
    dds = read_dds_names()
    for v in (V1, V2):
        for old, new in renames[v].items():
            if not cn.is_placeholder(old):
                dds[old] = new
    for old in list(dds):  # chains: a -> b, b -> c
        while dds[old] in dds and dds[dds[old]] != dds[old]:
            dds[old] = dds[dds[old]]
    DDS_NAMES.write_text(DDS_HEAD + "".join(f"{o}\t{n}\n" for o, n in sorted(dds.items())))
    existing = {v: set(cn.symbol_rows(v).values()) | set(cn.aliases(v)) for v in (V1, V2)}
    changed, unresolved = cn.rename_tree(renames, existing, {V1: set(), V2: set()})
    print("files changed:", dict(changed), "unresolved:", dict(unresolved))


main()
