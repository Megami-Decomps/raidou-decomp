#!/usr/bin/env python3
"""Give C units the .data ranges in config/<version>/data_c.tsv (unit, start, end vram).

    python3 tools/split_data.py raidou1 [--write]
"""
import argparse
import json
import re
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = json.loads((ROOT / "config/versions.json").read_text())


def rows(version):
    path = ROOT / "config" / version / "data_c.tsv"
    out = []
    for line in path.read_text().splitlines() if path.exists() else []:
        if line and not line.startswith("#"):
            unit, start, end = line.split("\t")[:3]
            out.append((int(start, 16), int(end, 16), unit))
    return sorted(out)


def lines_for(version, cfg, first_rom, end_rom):
    main = next(s for s in cfg["segments"] if isinstance(s, dict) and s.get("subsegments"))
    rom0, vram0 = main["start"], main["vram"]
    to_rom = lambda vram: vram - vram0 + rom0
    out, pos = [], to_rom(first_rom)
    first = True
    for start, end, unit in rows(version):
        if to_rom(start) > pos:
            out.append(f"      - [0x{pos:X}, data, {'data' if first else f'data_{pos - rom0 + vram0:08X}'}]")
        elif to_rom(start) < pos:
            raise SystemExit(f"{unit}: range starts inside the previous one")
        first = False
        out.append(f"      - [0x{to_rom(start):X}, .data, {unit}]")
        pos = to_rom(end)
    if pos < end_rom:
        out.append(f"      - [0x{pos:X}, data, {'data' if first else f'data_{pos - rom0 + vram0:08X}'}]")
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version")
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()
    cfg_path = ROOT / "config" / args.version / f"{VERSIONS[args.version]['serial']}.yaml"
    text = cfg_path.read_text()
    cfg = yaml.safe_load(text)
    start_m = re.search(r"^      - \[0x([0-9A-F]+), data, data\]\n", text, re.M)
    end_m = re.search(r"^(      #[^\n]*\n)*      - \[0x([0-9A-F]+), databin, eh_frame\]\n", text, re.M)
    first_rom, end_rom = int(start_m[1], 16), int(end_m[2], 16)
    main_seg = next(s for s in cfg["segments"] if isinstance(s, dict) and s.get("subsegments"))
    vram_of_first = first_rom - main_seg["start"] + main_seg["vram"]
    new = lines_for(args.version, cfg, vram_of_first, end_rom)
    print(f"{args.version}: {sum(1 for l in new if '.data,' in l)} C ranges, {sum(1 for l in new if ', data,' in l)} asm chunks")
    if args.write:
        body = text[start_m.end():end_m.start()]
        keep = "".join(l + "\n" for l in body.splitlines() if l.strip().startswith("#"))
        cfg_path.write_text(text[:start_m.start()] + "\n".join(new) + "\n" + keep + text[end_m.start():])
        print(f"-> {cfg_path.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
