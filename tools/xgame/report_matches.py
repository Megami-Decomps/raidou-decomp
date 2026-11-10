#!/usr/bin/env python3
"""Join match tables into per-Raidou maps and a per-unit SMT3 coverage table."""
import bisect, csv, difflib, json, sys
from collections import defaultdict
D = sys.argv[1]

def load(n):
    return {int(r["b_ea"], 16): (int(r["a_ea"], 16), r["method"]) for r in csv.DictReader(open(f"{D}/{n}.tsv"), delimiter="\t")}

def funcs(n):
    return {f["ea"]: f for f in json.load(open(f"{D}/{n}.json"))}

def ops(f):
    return [w & 0xFC00003F if w >> 26 == 0 else w & 0xFC000000 for w in f["words"]]

def sim(fa, fb):
    a, b = ops(fa), ops(fb)
    if a == b:
        return 1.0
    return difflib.SequenceMatcher(None, a, b, autojunk=False).ratio() if max(len(a), len(b)) < 4000 else 0.0

units = [(int(l.split("\t")[0], 16), l.split("\t")[1].strip()) for l in open(f"{D}/smt3_units.tsv")]
ustarts = [u[0] for u in units]
def unit(ea):
    return units[bisect.bisect_right(ustarts, ea) - 1][1]

S = funcs("smt3")
r21 = load("r2_vs_r1")
out = {}
for game in ("raidou1", "raidou2"):
    G = funcs(game)
    m = load(f"{'r1' if game == 'raidou1' else 'r2'}_vs_smt3")
    if game == "raidou2":
        r1m = load("r1_vs_smt3")
        for b, (a1, _) in r21.items():
            if b not in m and a1 in r1m:
                m[b] = (r1m[a1][0], "via_r1")
    rows = []
    for b in sorted(G):
        a, meth = m.get(b, (None, ""))
        r1 = f"{r21[b][0]:#x}" if game == "raidou2" and b in r21 else ""
        s = sim(S[a], G[b]) if a else 0.0
        rows.append((b, a, meth, s, r1))
    with open(f"{D}/{game}_map.tsv", "w") as fp:
        fp.write("ea\twords\tsmt3_name\tsmt3_ea\tsmt3_unit\tmethod\tsimilarity" + ("\traidou1_ea" if game == "raidou2" else "") + "\n")
        for b, a, meth, s, r1 in rows:
            cols = [f"{b:#x}", str(len(G[b]["words"])), S[a]["name"] if a else "", f"{a:#x}" if a else "", unit(a) if a else "", meth, f"{s:.2f}"]
            if game == "raidou2":
                cols.append(r1)
            fp.write("\t".join(cols) + "\n")
    out[game] = {a: s for b, a, meth, s, r1 in rows if a}
    n = sum(1 for r in rows if r[1]); w = sum(len(G[r[0]]["words"]) for r in rows if r[1])
    ident = sum(1 for r in rows if r[1] and r[3] == 1.0)
    print(f"{game}: {n}/{len(G)} funcs named from SMT3 ({w}/{sum(len(f['words']) for f in G.values())} words), {ident} identical opcode streams")

agg = defaultdict(lambda: [0, 0, 0, 0, 0, 0])
for a, f in S.items():
    u = unit(a); n = len(f["words"])
    r = agg[u]; r[0] += 1; r[1] += n
    if a in out["raidou1"]: r[2] += n; r[3] += n * out["raidou1"][a]
    if a in out["raidou2"]: r[4] += n; r[5] += n * out["raidou2"][a]
with open(f"{D}/smt3_units_coverage.tsv", "w") as fp:
    fp.write("unit\tfuncs\twords\tr1_pct\tr1_sim\tr2_pct\tr2_sim\n")
    for _, u in units:
        if u not in agg: continue
        f, w, a1, s1, a2, s2 = agg[u]
        fp.write(f"{u}\t{f}\t{w}\t{100*a1/w:.0f}\t{s1/a1 if a1 else 0:.2f}\t{100*a2/w:.0f}\t{s2/a2 if a2 else 0:.2f}\n")
