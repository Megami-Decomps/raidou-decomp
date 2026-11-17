#!/usr/bin/env python3
"""Rewrite splat/spimdisasm assembly into syntax the original ee-as accepts.

C translation units are assembled by Sony's ee-as (binutils 2.9-ee), because
its macro expansion (`move` -> daddu, R5900 hazard handling, delay-slot
filling) is what produced retail. Functions still in assembly are pulled into
those units with INCLUDE_ASM, so their text must also be ee-as syntax:

  * `%gp_rel(sym)` is not understood: the operand is replaced by the numeric
    offset taken from the instruction word splat prints in the line comment.
  * A float load from the retail `.lit4` pool (`lwc1 $fN, %gp_rel(D_...)($28)`)
    becomes `li.s $fN, <value>`, which is what ee-gcc emitted: ee-as puts the
    constant into the unit's own `.lit4` (merging repeats within the object) and
    the linker resolves the offset. C functions and asm functions of a unit then
    build its `.lit4` together, in source order, exactly as the original object
    did. Needs the retail ELF and gp (config/versions.json) for the version.
  * Instructions ee-as rejects (spimdisasm's VU0 macro-mode spelling) become
    `.word` with the original text kept as a comment.
  * A `.rodata` block that retail puts on a 16-byte boundary after zero
    padding gets `.align 4` (splat writes `.align 3`). The original item was
    16-aligned, e.g. a quadword vector or a 16-byte array. While the preceding
    function is asm its own blob carries the padding, but once it is C (a
    switch table, say) nothing else would supply it. Only done when the
    unit's .rodata itself starts 16-aligned, so the object's placement can't
    move.

The other rewrites take the encoding from retail, so they are byte-exact; the
cost is that those instructions carry no relocation, which is irrelevant for a
matching (non-shiftable) build.

The build writes one file per split folder: each IN.s becomes an assembler
macro `__ia_<name>` with a `.L__ia_<name>` symbol marking it defined, and
INCLUDE_ASM includes the folder file once per unit and expands the macro.

    python tools/eeas_compat.py IN.s OUT.s
    python tools/eeas_compat.py --folder OUT.s IN.s...
"""
from __future__ import annotations

import functools
import json
import re
import struct
import sys
from pathlib import Path

# /* ROM VRAM BYTES */  mnemonic operands   -- BYTES as printed by spimdisasm (file order)
INSN = re.compile(r"^(?P<pre>\s*/\*\s*[0-9A-F]+\s+[0-9A-F]+\s+(?P<word>[0-9A-F]{8})\s*\*/\s*)(?P<mn>\S+)(?P<ops>.*)$")
GP_REL = re.compile(r"%gp_rel\([^()]*(?:\([^()]*\)[^()]*)*\)")

# Mnemonics spimdisasm spells differently from ee-as (VU0 macro mode, COP2).
EE_AS_UNSUPPORTED = re.compile(r"^(v[a-z0-9]+(\.[xyzw]+)?|vcallms|vcallmsr|qmfc2|qmtc2|cfc2|ctc2|lqc2|sqc2)$")


def word_value(word: str) -> int:
    return int.from_bytes(bytes.fromhex(word), "little")


LITERAL_LOAD = re.compile(r"^\s+(\$f\d+),\s*%gp_rel\(\w+\)\(\$28\)\s*$")


class Pool:
    """Retail .lit4 words by address, for one version."""

    def __init__(self, version: str):
        root = Path(__file__).resolve().parent.parent
        info = json.loads((root / "config/versions.json").read_text())[version]
        self.gp = int(info["gp"], 16)
        sys.path.insert(0, str(Path(__file__).resolve().parent))
        from retail_elf import RETAIL, load_segments, va_to_off
        self.elf = (root / RETAIL[version]).read_bytes()
        self.segs, self.va_to_off = load_segments(self.elf), va_to_off
        self.lo, self.hi = lit4_range(self.elf)

    def word(self, addr: int) -> int | None:
        if not self.lo <= addr < self.hi:
            return None
        return int.from_bytes(self.elf[self.va_to_off(self.segs, addr):][:4], "little")


def lit4_range(elf: bytes) -> tuple[int, int]:
    """[start, end) of the .lit4 section from the ELF section headers."""
    shoff = int.from_bytes(elf[0x20:0x24], "little")
    shnum = int.from_bytes(elf[0x30:0x32], "little")
    shstrndx = int.from_bytes(elf[0x32:0x34], "little")
    hdr = lambda i: [int.from_bytes(elf[shoff + i * 40 + k:shoff + i * 40 + k + 4], "little") for k in range(0, 40, 4)]
    names = hdr(shstrndx)[4]
    for i in range(shnum):
        h = hdr(i)
        name = elf[names + h[0]:elf.index(b"\0", names + h[0])]
        if name == b".lit4":
            return h[3], h[3] + h[5]
    return 0, 0


def as_float(bits: int) -> str:
    value = struct.unpack("<f", bits.to_bytes(4, "little"))[0]
    text = f"{value:.9e}"
    assert struct.unpack("<f", struct.pack("<f", float(text)))[0] == value
    return text


def convert_line(line: str, pool: Pool | None = None) -> str:
    m = INSN.match(line)
    if not m:
        return line
    mn = m.group("mn")
    ops = m.group("ops")
    value = word_value(m.group("word"))
    if EE_AS_UNSUPPORTED.match(mn):
        return f"{m.group('pre')}.word 0x{value:08X} /* {mn}{ops.rstrip()} */"
    if "%gp_rel(" in ops:
        imm = value & 0xFFFF
        if imm >= 0x8000:
            imm -= 0x10000
        if pool and mn == "lwc1" and (lit := LITERAL_LOAD.match(ops)):
            bits = pool.word(pool.gp + imm)
            # ee-as materialises 0.0 and values with a zero low half inline, so
            # a pooled constant is always one of the others.
            if bits:
                return f"{m.group('pre')}li.s {lit.group(1)}, {as_float(bits)}"
        return f"{m.group('pre')}{mn}{GP_REL.sub(str(imm), ops)}"
    return line


DLABEL_ADDR = re.compile(r"^\s*/\*\s*[0-9A-F]+\s+([0-9A-F]{8})\b")


def realign_rodata(text: str, pool: Pool, unit_rodata_start: int | None) -> str:
    """`.align 3` -> `.align 4` for 16-aligned retail .rodata blocks after zero padding."""
    if unit_rodata_start is None or unit_rodata_start % 16:
        return text
    lines = text.split("\n")
    section = None
    for i, line in enumerate(lines):
        if line.startswith(".section"):
            section = line.split()[1]
        elif section == ".rodata" and line.strip() == ".align 3":
            addr = next((int(m.group(1), 16) for l in lines[i + 1:i + 8] if (m := DLABEL_ADDR.match(l))), None)
            if addr is not None and addr % 16 == 0 and addr >= unit_rodata_start:
                before = pool.elf[pool.va_to_off(pool.segs, addr - 8):][:8]
                if before == bytes(8):
                    lines[i] = ".align 4"
    return "\n".join(lines)


@functools.cache
def rodata_start(version: str, unit: str) -> int | None:
    """Retail address of a unit's .rodata subsegment, from the splat yaml."""
    root = Path(__file__).resolve().parent.parent
    info = json.loads((root / "config/versions.json").read_text())[version]
    yaml_text = (root / "config" / version / f"{info['serial']}.yaml").read_text()
    m = re.search(rf"\[0x([0-9A-F]+), \.?rodata, {re.escape(unit)}\]", yaml_text)
    return int(m.group(1), 16) + 0xFF000 if m else None


def convert(text: str, pool: Pool | None = None) -> str:
    return "\n".join(convert_line(line, pool) for line in text.split("\n"))


def unit_rodata_start(version: str, src: Path) -> int | None:
    """Retail address of the .rodata subsegment of the unit an asm file belongs to."""
    parts = src.resolve().parts
    if "nonmatchings" not in parts:
        return None
    return rodata_start(version, "/".join(parts[parts.index("nonmatchings") + 1:-1]))


@functools.cache
def known_versions() -> tuple[str, ...]:
    root = Path(__file__).resolve().parent.parent
    return tuple(json.loads((root / "config/versions.json").read_text()))


def version_of(src: Path) -> str | None:
    return next((p for p in src.resolve().parts if p in known_versions()), None)


def convert_file(src: Path, pools: dict) -> str:
    """src in ee-as syntax, reusing one Pool per version."""
    version = version_of(src)
    if version not in pools:
        pools[version] = Pool(version) if version else None
    pool = pools[version]
    text = convert(src.read_text(), pool)
    if pool:
        text = realign_rodata(text, pool, unit_rodata_start(version, src))
    return text


MACRO = re.compile(r"^\.macro __ia_(\w+)\n(.*?)^\.endm$", re.M | re.S)


def write_folder(out: Path, sources: list[Path]) -> None:
    """One macro per source file, named after its stem."""
    pools: dict = {}
    parts = []
    for src in sorted(sources):
        name = src.stem
        parts.append(f".equ .L__ia_{name}, 1\n.macro __ia_{name}\n{convert_file(src, pools).rstrip()}\n.endm\n")
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("".join(parts))


def read_folder(path: Path) -> dict[str, str]:
    """Name -> converted text of each macro in a folder file."""
    return {m.group(1): m.group(2) for m in MACRO.finditer(path.read_text())} if path.exists() else {}


def main() -> None:
    # @file names a response file of more arguments: a large folder's file list
    # exceeds the shell's command length limit.
    args = [arg for raw in sys.argv[1:]
            for arg in (Path(raw[1:]).read_text().split() if raw.startswith("@") else [raw])]
    if args and args[0] == "--folder":
        if len(args) < 3:
            raise SystemExit(__doc__)
        write_folder(Path(args[1]), [Path(name) for name in args[2:]])
        return
    if len(args) != 2:
        raise SystemExit(__doc__)
    Path(args[1]).parent.mkdir(parents=True, exist_ok=True)
    Path(args[1]).write_text(convert_file(Path(args[0]), {}))


if __name__ == "__main__":
    main()
