#!/usr/bin/env python3
"""For unmatched SMT3 functions, list the Raidou functions between the images of
their nearest matched neighbours, with opcode similarity.

Usage: gap_check.py DIR GAME NAME...
An empty gap means the function is absent; a candidate needs reading."""
import csv, difflib, json, sys
D, game, names = sys.argv[1], sys.argv[2], sys.argv[3:]
S = sorted(json.load(open(f"{D}/smt3.json")), key=lambda f: f["ea"])
R = sorted(json.load(open(f"{D}/{game}.json")), key=lambda f: f["ea"])
ri = {f["ea"]: i for i, f in enumerate(R)}
m = {int(r["smt3_ea"], 16): int(r["ea"], 16) for r in csv.DictReader(open(f"{D}/{game}_map.tsv"), delimiter="\t") if r["smt3_ea"]}
ops = lambda f: [w & 0xFC00003F if w >> 26 == 0 else w & 0xFC000000 for w in f["words"]]
for name in names:
    i = next(k for k, f in enumerate(S) if f["name"] == name)
    lo = next(j for j in range(i - 1, -1, -1) if S[j]["ea"] in m)
    hi = next(j for j in range(i + 1, len(S)) if S[j]["ea"] in m)
    a, b = ri[m[S[lo]["ea"]]], ri[m[S[hi]["ea"]]]
    cands = R[a + 1:b] if 0 <= b - a <= 60 else None
    if cands is None:
        print(f"{name}\t{len(S[i]['words'])}w\tneighbours out of order")
        continue
    scored = sorted(((difflib.SequenceMatcher(None, ops(S[i]), ops(f), autojunk=False).ratio(), f["ea"], len(f["words"])) for f in cands), reverse=True)[:3]
    print(f"{name}\t{len(S[i]['words'])}w\tsmt3 gap {hi - lo - 1}\t{game} gap {len(cands)}\t" + " ".join(f"{ea:#x}:{n}w:{r:.2f}" for r, ea, n in scored))
