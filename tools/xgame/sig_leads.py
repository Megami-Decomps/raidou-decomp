#!/usr/bin/env python3
"""Write config/<v>/ida_protos.txt from the IDB signature proposals (strong rows only).

    python3 tools/xgame/sig_leads.py raidou1 SIG_TSV
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HEADER = "# from the IDB signature proposal, strong rows only; leads, nothing reads this. 0xADDR name prototype"
EXTRA = {  # weak rows with a hand-checked prototype
    "raidou1": {0x37EF10: "s32 at3DecInit(void *param);", 0x37EFD8: "s32 at3DecDecode(void *param);"},
    "raidou2": {0x3F4B50: "s32 at3DecInit(void *param);", 0x3F4C18: "s32 at3DecDecode(void *param);"},
}


def main():
    ver, tsv = sys.argv[1], Path(sys.argv[2])
    names = {}
    for line in (ROOT / "config" / ver / "symbol_addrs.txt").read_text().splitlines():
        m = re.match(r"\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;", line)
        if m:
            names.setdefault(int(m.group(2), 16), m.group(1))
    protos = dict(EXTRA[ver])
    for row in tsv.read_text().splitlines()[1:]:
        addr, proto, cls = row.split("\t")[:3]
        if cls == "strong":
            protos.setdefault(int(addr, 16), proto)
    out = [HEADER]
    for addr in sorted(protos):
        proto = protos[addr].rstrip(";")
        name = names.get(addr, f"func_{addr:08X}")
        proto = re.sub(r"\w+\(", name + "(", proto, count=1)
        out.append(f"0x{addr:08X} {name} {proto}")
    (ROOT / "config" / ver / "ida_protos.txt").write_text("\n".join(out) + "\n")
    print(ver, len(out) - 1)


main()
