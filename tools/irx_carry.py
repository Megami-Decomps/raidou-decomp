#!/usr/bin/env python3
"""Write a Raidou IRX module's C from smt3-decomp's, one function at a time.

    python tools/irx_carry.py raidou1/SDFSDMAN
"""
# A chunk `same` in smt3_pairs.tsv takes SMT3's text (C or NON_MATCHING draft);
# zero padding ending on 16 is ALIGN_TEXT(4); the rest is INCLUDE_ASM.
from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import irx  # noqa: E402
import irx_config  # noqa: E402
from irx_match import SMT3  # noqa: E402

INCLUDE_ASM = re.compile(r'^INCLUDE_ASM\("\w+",\s*(\w+)\);')
IMPORT_MAGIC = (0x41E00000).to_bytes(4, "little")


def parse_smt3(text: str) -> tuple[list[str], dict[str, list[str]], list[str]]:
    """(preamble, function name -> its text, import tables) of an smt3-decomp IRX file."""
    lines = text.splitlines()
    items: dict[str, list[str]] = {}
    pending: list[str] = []
    preamble: list[str] | None = None
    i = 0
    while i < len(lines):
        line = lines[i]
        if line.startswith("IRX_IMPORT_TABLE("):
            imports = lines[i:]
            break
        if line.startswith("/*"):
            while not lines[i].rstrip().endswith("*/"):
                pending.append(lines[i])
                i += 1
            pending.append(lines[i])
            i += 1
            continue
        if not line.strip() or line.startswith("ALIGN_TEXT("):
            # Alignment between items follows the Raidou padding, not SMT3's.
            if preamble is None:
                pending.append(line)
            i += 1
            continue
        start = i
        if line.startswith("#ifdef NON_MATCHING"):
            depth = 0
            name = None
            while True:
                l = lines[i]
                if l.startswith("#if"):
                    depth += 1
                elif l.startswith("#endif"):
                    depth -= 1
                m = INCLUDE_ASM.match(l)
                if m:
                    name = m[1]
                i += 1
                if depth == 0:
                    break
        elif INCLUDE_ASM.match(line):
            name = INCLUDE_ASM.match(line)[1]
            i += 1
        elif line.startswith(("#", "extern", "typedef", "struct", "static const")) or line.rstrip().endswith(";"):
            if preamble is not None:
                raise SystemExit(f"top-level declaration after the first function: {line}")
            pending.append(line)
            i += 1
            continue
        else:
            name = re.search(r"(\w+)\s*\(", line)[1]
            while lines[i] != "}":
                i += 1
            i += 1
        if preamble is None:
            # Comments directly above the first function belong to it.
            cut = len(pending)
            while cut and (pending[cut - 1].startswith(("/*", " *")) or pending[cut - 1].rstrip().endswith("*/")):
                cut -= 1
            preamble, pending = pending[:cut], pending[cut:]
        items[name] = [l for l in pending if l.strip()] + lines[start:i]
        pending = []
    return preamble, items, imports


def import_bytes(path: Path) -> bytes:
    text = irx.read(path).section(".text").data
    return text[text.find(IMPORT_MAGIC):]


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("key")
    args = ap.parse_args()
    cfg = irx_config.load(args.key)
    module, version = cfg["module_dir"], cfg["version"]
    preamble, items, imports = parse_smt3((SMT3 / "src/irx" / f"{module}.c").read_text())
    pairs = {}
    for row in irx_config.path(cfg, "symbols").with_name("smt3_pairs.tsv").read_text().splitlines():
        f = row.split("\t")
        if f[0] == "F" and f[3]:
            pairs[int(f[1], 16)] = (f[3], f[4])
    syms = {}
    for line in irx_config.path(cfg, "symbols").read_text().splitlines():
        m = re.match(r"(\w+) = (0x[0-9A-F]+);", line)
        if m:
            syms[m[1]] = int(m[2], 16)
    order = irx_config.path(cfg, "build") / "text_order.txt"
    chunks = [l for l in order.read_text().splitlines() if l and not l.startswith("#")]
    text = irx.read(irx_config.path(cfg, "target")).section(".text").data
    imports_at = text.find(IMPORT_MAGIC)
    same_imports = import_bytes(irx_config.path(cfg, "target")) == import_bytes(SMT3 / "orig/irx" / f"{module}.IRX")

    out = [l.replace("asm/irx/", "asm/<version>/irx/") for l in preamble]
    while out and not out[-1].strip():
        out.pop()
    carried = drafts = asm = 0
    for i, name in enumerate(chunks):
        addr = syms.get(name, int(name[2:], 16) if name.startswith("D_") else None)
        end = syms.get(chunks[i + 1], len(text)) if i + 1 < len(chunks) and not chunks[i + 1].startswith("D_") \
            else (int(chunks[i + 1][2:], 16) if i + 1 < len(chunks) else len(text))
        if same_imports and addr == imports_at:
            out += ["", "ALIGN_TEXT(4);"] + imports
            break
        pair = pairs.get(addr)
        if pair and pair[1] == "same" and pair[0] == name and name in items:
            body = items[name]
            draft = any(l.startswith("#ifdef NON_MATCHING") for l in body)
            drafts += draft
            carried += not draft
            out += [""] + body
        elif name.startswith("D_") and end % 16 == 0 and not any(text[addr:end]):
            out += ["", "ALIGN_TEXT(4);"]
        else:
            asm += 1
            out += ["", f'INCLUDE_ASM("{module}", {name});']
    src = ROOT / "src" / "irx" / version / f"{module}.c"
    src.write_text("\n".join(out).rstrip() + "\n")
    print(f"{args.key}: {carried} functions from SMT3 C, {drafts} SMT3 NON_MATCHING drafts, {asm} INCLUDE_ASM")


if __name__ == "__main__":
    main()
