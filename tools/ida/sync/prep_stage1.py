#!/usr/bin/env python3
"""Offline stage 1 plan: raidou-decomp names plus SMT3/DDS1 prototypes for paired functions.
Writes names-<v>.tsv and stage1-protos-<v>.tsv under extract_types.DATA."""
import json, os, re, sys
sys.path.insert(0, os.path.dirname(__file__))
from extract_types import RD, DATA as OUT
RD += "/config"
META = os.path.expanduser("~/.local/state/smt3-port-work/raidou-map/apply")
src_meta = {"SLUS_209.11": json.load(open(f"{META}/smt3_meta.json"))["funcs"],
            "SLUS_209.74": json.load(open(f"{META}/dds1_meta.json"))["funcs"]}
for v in ("raidou1", "raidou2"):
    names = {}
    for line in open(f"{RD}/{v}/symbol_addrs.txt"):
        m = re.match(r"(\w+) = 0x([0-9A-Fa-f]+);.*type:func", line)
        if m:
            names[int(m.group(2), 16)] = m.group(1)
    srcs = {}
    for line in open(f"{RD}/{v}/name_sources.txt"):
        if line.startswith("#"):
            continue
        a, n, cls, note = (line.rstrip("\n").split("\t") + [""] * 4)[:4]
        srcs[int(a, 16)] = (cls, note)
    rmeta = json.load(open(f"{META}/{v}_meta.json"))["funcs"]
    nproto = 0
    with open(f"{OUT}/names-{v}.tsv", "w") as fn, open(f"{OUT}/stage1-protos-{v}.tsv", "w") as fp:
        fn.write("addr\tname\tclass\tevidence\n")
        fp.write("addr\tname\tsource\tsource_addr\tproto\n")
        for a in sorted(names):
            cls, note = srcs.get(a, ("inferred", "symbol_addrs.txt, no name_sources row"))
            fn.write(f"0x{a:08X}\t{names[a]}\t{cls}\t{note}\n")
            m = re.search(r"(SLUS_209\.11|SLUS_209\.74) (0x[0-9A-Fa-f]+)", note)
            if not m or note.startswith("fuzzy"):
                continue
            f = src_meta[m.group(1)].get(str(int(m.group(2), 16)))
            if not f or not f["proto"]:
                continue
            proto = re.sub(r"\b%s\(" % re.escape(f["name"]), names[a] + "(", f["proto"], count=1)
            if names[a] + "(" not in proto:
                continue
            fp.write(f"0x{a:08X}\t{names[a]}\t{m.group(1)}\t{m.group(2)}\t{proto}\n")
            nproto += 1
    have = sum(1 for k, x in rmeta.items() if int(k) in names and x["name"] == names[int(k)])
    print(v, "names", len(names), "already-in-old-meta", have, "protos", nproto)
