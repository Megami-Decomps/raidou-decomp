#!/usr/bin/env python3
"""Rewrite the prototypes of functions in units that borrow a DDS layout (#define VERSION_DDS1/2)
to the <type>_dds1/_dds2 variants extract_types.py emitted (types/<v>-variants.txt).
Writes protos-<v>-variant.tsv (addr, name, unit, lineage, proto) for the rows that change."""
import csv, os, re, sys
sys.path.insert(0, os.path.dirname(__file__))
from extract_types import RD, DATA as W
ELF = {"raidou1": "SLUS_214.31", "raidou2": "SLUS_218.45"}
def main(v):
    var = {}
    for line in open(f"{W}/types/{v}-variants.txt"):
        if not line.strip():
            continue
        key, vname = line.split("\t")[:2]
        lin = vname.rsplit("_", 1)[1]
        base = vname[: -len(lin) - 1]
        var.setdefault(lin, {})[base] = vname
        tag = key.split()[-1]
        if tag != base:
            var[lin][tag] = tag + "_" + lin
    unit_of, cur = {}, None
    for line in open(f"{RD}/build/{v}/{ELF[v]}.map"):
        m = re.match(r"\s*\.text\s+0x[0-9a-f]+\s+0x[0-9a-f]+\s+build/%s/(src/\S+)\.o" % v, line)
        if m:
            cur = m.group(1) + ".c"
            continue
        if re.match(r"\s*\.\w+\s+0x", line) or line.startswith(" *("):
            cur = None if not line.lstrip().startswith(".text") else cur
        m = re.match(r"\s+0x0*([0-9a-f]+)\s+(\w+)\s*$", line)
        if m and cur:
            unit_of[int(m.group(1), 16)] = cur
    lin_of = {}
    n = 0
    out = open(f"{W}/protos-{v}-variant.tsv", "w")
    out.write("addr\tname\tunit\tlineage\tproto\n")
    for r in csv.DictReader(open(f"{W}/protos-{v}.tsv"), delimiter="\t"):
        u = unit_of.get(int(r["addr"], 16))
        if not u:
            continue
        if u not in lin_of:
            m = re.search(r"^\s*#\s*define\s+VERSION_(DDS\d)", open(f"{RD}/{u}").read(), re.M) if os.path.exists(f"{RD}/{u}") else None
            lin_of[u] = m.group(1).lower() if m else None
        L = lin_of[u]
        if L not in var:
            continue
        p = r["proto"]
        q = re.sub(r"\b(\w+)\b", lambda m: var[L].get(m.group(1), m.group(1)) if m.group(1) != r["name"] else m.group(1), p)
        if q != p:
            out.write("%s\t%s\t%s\t%s\t%s\n" % (r["addr"], r["name"], u, L, q))
            n += 1
    print(v, "variant protos", n)
for v in sys.argv[1:] or ("raidou1", "raidou2"):
    main(v)
