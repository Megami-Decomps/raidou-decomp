#!/usr/bin/env python3
"""List DDS1/DDS2 functions absent from SMT3 that appear in Raidou 1/2.

Usage: dds_unique_in_raidou.py DIR (holding the match tables); writes dds_unique_in_raidou.tsv.
Unique: no SMT3 match and not an SMT3 name. Raidou hits also matched to SMT3 are dropped.
"""
import bisect
import csv
import difflib
import json
import os
import re
import sys
from collections import Counter, defaultdict

D = sys.argv[1]
DDS = os.path.expanduser("~/projects/dds-decomp")


def load(name):
    return {int(r["b_ea"], 16): (int(r["a_ea"], 16), r["method"])
            for r in csv.DictReader(open(f"{D}/{name}.tsv"), delimiter="\t")}


def funcs(name):
    return {f["ea"]: f for f in json.load(open(f"{D}/{name}.json"))}


def units(version, elf):
    rows = []
    for line in open(f"{DDS}/config/{version}/{elf}.yaml"):
        m = re.match(r"\s*- \[(0x[0-9A-Fa-f]+), (c|asm|hasm), ([^\]]+)\]", line)
        if m:
            rows.append((int(m.group(1), 16) - 0x1000 + 0x100000, m.group(3)))
    starts = [r[0] for r in rows]
    return lambda ea: rows[bisect.bisect_right(starts, ea) - 1][1] if starts and ea >= starts[0] else ""


def ops(f):
    return [w & 0xFC00003F if w >> 26 == 0 else w & 0xFC000000 for w in f["words"]]


def sim(a, b):
    x, y = ops(a), ops(b)
    return 1.0 if x == y else difflib.SequenceMatcher(None, x, y, autojunk=False).ratio()


smt3_names = {f["name"] for f in json.load(open(f"{D}/smt3.json"))}
dds = {"dds1": (funcs("dds1"), units("dds1", "SLUS_209.74")),
       "dds2": (funcs("dds2"), units("dds2", "SLUS_211.52"))}
unique = {}
for v, (F, _) in dds.items():
    in_smt3 = load(f"{v}_vs_smt3")
    unique[v] = {ea for ea, f in F.items() if ea not in in_smt3 and f["name"] not in smt3_names}

rows = []
dropped = Counter()
for game in ("raidou1", "raidou2"):
    G = funcs(game)
    smt3_claim = load({"raidou1": "r1_vs_smt3", "raidou2": "r2_vs_smt3"}[game])
    hits = defaultdict(dict)
    for v in dds:
        for b, (a, meth) in load(f"{game}_vs_{v}").items():
            if a in unique[v]:
                if b in smt3_claim:
                    dropped[game] += 1
                else:
                    hits[b][v] = (a, meth)
    for b in sorted(hits):
        cols = [game, f"{b:#x}", str(len(G[b]["words"]))]
        best = 0.0
        for v in ("dds1", "dds2"):
            F, unit = dds[v]
            if v in hits[b]:
                a, meth = hits[b][v]
                s = sim(F[a], G[b])
                best = max(best, s)
                cols += [F[a]["name"], f"{a:#x}", unit(a), meth, f"{s:.2f}"]
            else:
                cols += [""] * 5
        rows.append(cols + [f"{best:.2f}"])

with open(f"{D}/dds_unique_in_raidou.tsv", "w") as fp:
    fp.write("game\tea\twords\tdds1_name\tdds1_ea\tdds1_unit\tdds1_method\tdds1_sim"
             "\tdds2_name\tdds2_ea\tdds2_unit\tdds2_method\tdds2_sim\tbest_sim\n")
    for r in rows:
        fp.write("\t".join(r) + "\n")

for v in dds:
    print(f"{v}: {len(unique[v])} of {len(dds[v][0])} functions not in SMT3")
for game in ("raidou1", "raidou2"):
    g = [r for r in rows if r[0] == game]
    ident = sum(1 for r in g if r[-1] == "1.00")
    strong = sum(1 for r in g if float(r[-1]) >= 0.8)
    print(f"{game}: {len(g)} DDS-only functions found ({ident} identical, {strong} >= 0.8); "
          f"{dropped[game]} dropped as also matched to SMT3")
by_unit = defaultdict(Counter)
for r in rows:
    u = r[5] or r[10]
    by_unit[u.split("/")[0] if "/" in u else u][r[0]] += 1
for u, c in sorted(by_unit.items(), key=lambda x: -sum(x[1].values()))[:25]:
    print(f"  {u or '(no unit)':30} raidou1 {c['raidou1']:4}  raidou2 {c['raidou2']:4}")
