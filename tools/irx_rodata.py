#!/usr/bin/env python3
"""Place a module's INCLUDE_RODATA lines: `python tools/irx_rodata.py raidou1/SDFSDMAN [--check]`."""
# Retail .rodata is in the order of the functions that first load each piece.
# A C function emits its own jump table, so the pieces between two tables go
# right before the second table's function (or at the end of the unit), late as
# possible: gas fills a delay slot with a %lo of a symbol it has already seen
# defined, which retail never does. A NON_MATCHING draft keeps its table's line
# in the #else beside its INCLUDE_ASM.
from __future__ import annotations

import argparse
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import irx  # noqa: E402
import irx_config  # noqa: E402
import irx_split  # noqa: E402

RODATA_LINE = re.compile(r'^[ \t]*INCLUDE_RODATA\("[^"]+",\s*\w+\s*\);\n', re.M)


def owners(cfg: dict) -> list[tuple[str, str]]:
    """(piece, owning function) for every .rodata piece, in address order."""
    mod = irx.read(irx_config.path(cfg, "target"))
    rows = irx_split.load_syms(cfg)
    syms = irx_split.SymbolMap(rows)
    jtbls = irx_split.jump_tables(mod)
    for addr in jtbls:
        syms.add(addr, f"jtbl_{addr:08X}")
    text, rod = mod.section(".text"), mod.section(".rodata")
    starts = [a for a in syms.sorted if rod.addr <= a < rod.addr + rod.size]
    if not starts or starts[0] != rod.addr:
        starts.insert(0, rod.addr)
    names = [syms.by_addr.get(a, f"D_{a:08X}") for a in starts]
    funcs = sorted((a, a + s, n) for a, n, s, k in rows if k == "F")

    def func_at(addr: int) -> int | None:
        for i, (lo, hi, _) in enumerate(funcs):
            if lo <= addr < hi:
                return i
        return None

    def piece_at(addr: int) -> int:
        return max(i for i, a in enumerate(starts) if a <= addr)

    first: dict[int, int] = {}
    refs = [(text.addr + off, addr) for off, addr in irx_split.pair_hi_lo(text, text.addr).items()]
    for off, ty in text.relocs:
        if ty == irx_split.R_32:
            refs.append((text.addr + off, struct.unpack_from("<I", text.data, off)[0]))
    for site, addr in refs:
        f = func_at(site)
        if f is not None and rod.addr <= addr < rod.addr + rod.size:
            p = piece_at(addr)
            first[p] = min(first.get(p, f), f)
    out, pending, last = [], [], None
    for i, name in enumerate(names):
        if i not in first:
            pending.append(name)
            continue
        f = first[i]
        if last is not None and f < last:
            sys.exit(f"{cfg['key']}: {name} is loaded first by {funcs[f][2]}, before {funcs[last][2]}'s rodata")
        last = f
        out += [(n, funcs[f][2]) for n in pending + [name]]
        pending = []
    if pending:
        if last is None:
            sys.exit(f"{cfg['key']}: no function loads .rodata")
        out += [(n, funcs[last][2]) for n in pending]
    return out


def definition_start(lines: list[str], func: str) -> int | None:
    """The line where a C function's definition starts, with its leading comment."""
    head = re.compile(rf"^[A-Za-z_].*\b{func}\s*\(")
    for i, line in enumerate(lines):
        if head.match(line) and not line.rstrip().endswith(";"):
            while i > 0 and lines[i - 1].strip() and not lines[i - 1].startswith("#") \
                    and (lines[i - 1].lstrip().startswith(("/*", "*")) or lines[i - 1].rstrip().endswith("*/")):
                i -= 1
            return i
    return None


def block_start(lines: list[str], module: str, func: str) -> tuple[int, int | None] | None:
    """(first line of func's block, line of its INCLUDE_ASM or None when it is C)."""
    asm = re.compile(rf'^\s*INCLUDE_ASM\("{module}",\s*{func}\s*\);')
    at = next((i for i, l in enumerate(lines) if asm.match(l)), None)
    if at is not None:
        if at > 0 and lines[at - 1].startswith("#else"):
            return max(i for i in range(at) if lines[i].startswith("#ifdef NON_MATCHING")), at
        return at, at
    at = definition_start(lines, func)
    return None if at is None else (at, None)


def place(cfg: dict) -> dict[Path, str]:
    module = cfg["module_dir"]
    texts = {ROOT / s: RODATA_LINE.sub("", (ROOT / s).read_text()) for s in cfg["paths"]["srcs"]}
    files = {p: t.splitlines(keepends=True) for p, t in texts.items()}
    before: dict[tuple[Path, int], list[str]] = {}
    pending: list[str] = []
    path = None
    for piece, func in owners(cfg):
        found = next(((p, b) for p, lines in files.items() if (b := block_start(lines, module, func))), None)
        if found is None:
            sys.exit(f"{cfg['key']}: {func} is in none of {cfg['paths']['srcs']}")
        owner_path, (start, asm) = found
        if path is not None and owner_path != path:
            before.setdefault((path, len(files[path])), []).extend(pending)
            pending = []
        path = owner_path
        line = f'INCLUDE_RODATA("{module}", {piece});\n'
        if not piece.startswith("jtbl_"):
            pending.append(line)
            continue
        before.setdefault((path, start), []).extend(pending)
        pending = []
        if asm is not None:
            before.setdefault((path, asm), []).append(line)
    if path is not None:
        before.setdefault((path, len(files[path])), []).extend(pending)
    out = {}
    for p, lines in files.items():
        text = []
        for i, l in enumerate(lines + [""]):
            text += before.get((p, i), [])
            text.append(l)
        out[p] = "".join(text)
    return out


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("keys", nargs="+")
    ap.add_argument("--check", action="store_true", help="fail if a C file is not up to date")
    args = ap.parse_args()
    stale = []
    for key in args.keys:
        for path, text in place(irx_config.load(key)).items():
            if path.read_text() != text:
                stale.append(path)
                if not args.check:
                    path.write_text(text)
    if args.check and stale:
        sys.exit("INCLUDE_RODATA placement out of date: " + ", ".join(str(p.relative_to(ROOT)) for p in stale))


if __name__ == "__main__":
    main()
