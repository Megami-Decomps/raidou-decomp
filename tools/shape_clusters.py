#!/usr/bin/env python3
"""Group unmatched Raidou functions by asm shape so one drafted exemplar serves a cluster.

Usage: shape_clusters.py [--version raidou1] [--out DIR] [--floor 0.9] [-n 40]
Reads the shape index (tools/similar_index.py) and writes DIR/clusters.tsv and DIR/members.tsv.
"""
from __future__ import annotations

import argparse
import bisect
import collections
import difflib
import multiprocessing
import os
from pathlib import Path

import find_similar as fs
import similar_index

HOME = Path.home()
WORK = HOME / ".local/state/raidou-decomp-work"
OUT = WORK / "clusters"
BATCH_DIRS = WORK / "port/rest/batches"
EPILOG = ("Targets are the version's functions still assembly, outside sdk/, minus every name with a row in a "
          "results.tsv under --batch-dirs (drafted, not landed). Functions of 8 instructions or more are linked "
          "when their mnemonic sequences reach --floor (difflib ratio, the index's mnem tier); shorter ones only "
          "when their mnemonic sequence is identical. Clusters are picked greedily: the exemplar is the function "
          "whose unassigned neighbours cover the most words, and its cluster is those neighbours. A cluster has "
          "a template when a member's precomputed neighbour list (matched C in raidou1/2, smt3, dds1/2) reaches "
          "--floor; otherwise its exemplar needs drafting from scratch.")
BAND = 0.25
RERANK = 60

_W: tuple = ()


def drafted_names(dirs: Path) -> dict[str, str]:
    """name -> batch directory of every function a batch directory has a result for."""
    out = {}
    for res in sorted(dirs.glob("*/results.tsv")):
        for line in res.read_text().splitlines():
            f = line.split("\t")
            if f and f[0] and f[0] != "name":
                out[f[0]] = res.parent.name
    return out


def batch_of(path: Path) -> dict[str, str]:
    out = {}
    if path.exists():
        lines = path.read_text().splitlines()
        head = lines[0].split("\t")
        for line in lines[1:]:
            r = dict(zip(head, line.split("\t")))
            if r.get("name"):
                out[r["name"]] = r["batch"].split("-")[0]
    return out


def _edges(chunk: list[str]) -> list[tuple[str, list[tuple[float, str]]]]:
    funcs, idx, sizes, by_size, floor = _W
    out = []
    for k in chunk:
        t = funcs[k]
        lo, hi = t["n"] * (1 - BAND), t["n"] * (1 + BAND)
        pool = [c for s in sizes[bisect.bisect_left(sizes, lo):bisect.bisect_right(sizes, hi)]
                for c in by_size[s] if c != k]
        tok, bag, norm = idx.shape(t)
        scored = []
        for c in pool:
            _, cbag, cnorm = idx.shape(funcs[c])
            scored.append((sum(v * cbag[x] for x, v in bag.items() if x in cbag) / (norm * cnorm), c))
        scored.sort(reverse=True)
        got = []
        for _, c in scored[:RERANK]:
            r = difflib.SequenceMatcher(None, tok, idx.shape(funcs[c])[0], autojunk=False).ratio()
            if r >= floor:
                got.append((round(r, 4), c))
        out.append((k, got))
    return out


def link(funcs: dict, keys: list[str], floor: float) -> dict[str, dict[str, float]]:
    """Neighbours at --floor or better among keys (8 instructions or more), as key -> {key: ratio}."""
    global _W
    idx = fs.Index({})
    by_size = collections.defaultdict(list)
    for k in keys:
        by_size[funcs[k]["n"]].append(k)
        idx.shape(funcs[k])
    _W = (funcs, idx, sorted(by_size), by_size, floor)
    chunks = [keys[i:i + 50] for i in range(0, len(keys), 50)]
    edges = collections.defaultdict(dict)
    with multiprocessing.get_context("fork").Pool(os.cpu_count()) as pool:
        for got in pool.imap_unordered(_edges, chunks):
            for k, near in got:
                for r, c in near:
                    edges[k][c] = max(r, edges[k].get(c, 0))
                    edges[c][k] = max(r, edges[c].get(k, 0))
    _W = ()
    return edges


def greedy(funcs: dict, keys: list[str], edges: dict) -> list[tuple[str, list[tuple[str, float]]]]:
    """(exemplar, [(member, ratio to exemplar)]) picked by words covered, largest first."""
    left, out = set(keys), []
    while left:
        def cover(k):
            return funcs[k]["n"] + sum(funcs[c]["n"] for c in edges.get(k, ()) if c in left)
        k = max(left, key=lambda k: (cover(k), -funcs[k]["addr"]))
        members = sorted(((c, r) for c, r in edges.get(k, {}).items() if c in left), key=lambda x: -x[1])
        out.append((k, members))
        left -= {k} | {c for c, _ in members}
    return out


def template(funcs: dict, members: list[str], floor: float) -> tuple[float, str, str] | None:
    """(ratio, member, CORPUS:NAME) of the best matched template any member has at --floor."""
    best = None
    for m in members:
        for r, c in funcs[m].get("near", ())[:1]:
            if r >= floor and (best is None or r > best[0]):
                best = (r, m, c)
    return best


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0], epilog=EPILOG)
    ap.add_argument("--version", default="raidou1", choices=("raidou1", "raidou2"))
    ap.add_argument("--index", type=Path, default=similar_index.INDEX)
    ap.add_argument("--batch-dirs", type=Path, default=BATCH_DIRS)
    ap.add_argument("--batches", type=Path, default=similar_index.BATCHES)
    ap.add_argument("--out", type=Path, default=OUT)
    ap.add_argument("--floor", type=float, default=similar_index.FLOOR)
    ap.add_argument("-n", "--count", type=int, default=25, help="clusters printed")
    a = ap.parse_args()

    funcs = fs.load(a.index)
    drafted = drafted_names(a.batch_dirs)
    batch = batch_of(a.batches)
    other = "raidou2" if a.version == "raidou1" else "raidou1"
    keys = sorted((k for k, f in funcs.items() if f["version"] == a.version and f["state"] == "asm"
                   and not f["unit"].startswith("sdk/") and f["name"] not in drafted),
                  key=lambda k: funcs[k]["addr"])
    long_keys = [k for k in keys if funcs[k]["n"] >= fs.MIN_INSNS]
    edges = link(funcs, long_keys, a.floor)
    exact = collections.defaultdict(list)
    for k in keys:
        if funcs[k]["n"] < fs.MIN_INSNS:
            exact[funcs[k]["hash"][3]].append(k)
    for group in exact.values():
        for x in group:
            edges[x] = {y: 1.0 for y in group if y != x}
    groups = greedy(funcs, keys, edges)

    a.out.mkdir(parents=True, exist_ok=True)
    crow, mrow = [], []
    for i, (ex, members) in enumerate(groups, 1):
        names = [ex] + [m for m, _ in members]
        words = sum(funcs[k]["n"] for k in names)
        tpl = template(funcs, names, a.floor)
        r2 = [g for k in names if (g := funcs.get(f"{other}:{funcs[k]['name']}"))]
        cid = f"C{i:04d}"
        crow.append((cid, funcs[ex]["name"], funcs[ex]["n"], len(names), words, words - funcs[ex]["n"],
                     f"{tpl[2]}:{tpl[0]:.2f}" if tpl else "", sum(1 for g in r2 if g["state"] == "asm"),
                     sum(g["n"] for g in r2 if g["state"] == "asm")))
        for k, r in [(ex, 1.0)] + members:
            f = funcs[k]
            g = funcs.get(f"{other}:{f['name']}")
            near = f.get("near", [])[:1]
            mrow.append((cid, f["name"], f"{f['addr']:08X}", f["n"], f["unit"], f"{r:.2f}", batch.get(f["name"], ""),
                         g["state"] if g else "", f"{near[0][1]}:{near[0][0]:.2f}" if near else ""))
    (a.out / "clusters.tsv").write_text(
        "cluster\texemplar\texemplar_words\tsize\twords\tmember_words\ttemplate\tr2_asm\tr2_words\n"
        + "".join("\t".join(map(str, r)) + "\n" for r in crow))
    (a.out / "members.tsv").write_text(
        "cluster\tname\taddr\twords\tunit\tratio\tbatch\tr2\tbest_template\n"
        + "".join("\t".join(map(str, r)) + "\n" for r in mrow))

    multi = [r for r in crow if r[3] > 1]
    sizes = collections.Counter(min(r[3], 10) for r in multi)
    print(f"{len(keys)} targets ({sum(funcs[k]['n'] for k in keys)} words), {len(drafted)} drafted names left out; "
          f"{len(multi)} clusters of 2+ cover {sum(r[3] for r in multi)} functions, {sum(r[4] for r in multi)} words "
          f"({sum(r[5] for r in multi)} beyond the exemplars)")
    print("size: " + ", ".join(f"{'10+' if s == 10 else s}:{n}" for s, n in sorted(sizes.items())))
    with_tpl = [r for r in multi if r[6]]
    print(f"with a matched template: {len(with_tpl)} clusters, {sum(r[4] for r in with_tpl)} words; "
          f"fresh exemplar: {len(multi) - len(with_tpl)} clusters, {sum(r[4] for r in multi if not r[6])} words")
    for r in sorted(multi, key=lambda r: -r[5])[:a.count]:
        print("\t".join(map(str, r)))


if __name__ == "__main__":
    main()
