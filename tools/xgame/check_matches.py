#!/usr/bin/env python3
"""Cross-check match tables: triangle agreement and callee consistency per method."""
import csv, json, sys
from collections import Counter, defaultdict
D = sys.argv[1]

def load(n):
    return {int(r["b_ea"], 16): (int(r["a_ea"], 16), r["method"]) for r in csv.DictReader(open(f"{D}/{n}.tsv"), delimiter="\t")}

r1, r2, r21 = load("r1_vs_smt3"), load("r2_vs_smt3"), load("r2_vs_r1")
agree, disagree, only = Counter(), Counter(), Counter()
for b, (a, meth) in r2.items():
    if b in r21 and r21[b][0] in r1:
        (agree if r1[r21[b][0]][0] == a else disagree)[meth] += 1
    else:
        only[meth] += 1
print("triangle r2->smt3 vs r2->r1->smt3 by r2 method:")
for m in sorted(set(agree) | set(disagree) | set(only)):
    print(f"  {m:8} agree {agree[m]:5} disagree {disagree[m]:4} unchecked {only[m]:4}")
via = sum(1 for b, (a1, _) in r21.items() if a1 in r1 and b not in r2)
print("r2 funcs reachable via r1 but not matched directly:", via)

def callee_check(name, A, B):
    a = {f["ea"]: f for f in json.load(open(f"{D}/{A}.json"))}
    b = {f["ea"]: f for f in json.load(open(f"{D}/{B}.json"))}
    m = load(name)
    ok, bad = Counter(), Counter()
    for be, (ae, meth) in m.items():
        ac = set(a[ae]["calls"])
        for c in b[be]["calls"]:
            if c in m:
                (ok if m[c][0] in ac else bad)[meth] += 1
    print(f"{name} callee agreement by method:")
    for k in sorted(ok | bad):
        print(f"  {k:8} {ok[k]:6} ok {bad[k]:5} bad ({bad[k]/(ok[k]+bad[k]):.1%})")

callee_check("r1_vs_smt3", "smt3", "raidou1")
