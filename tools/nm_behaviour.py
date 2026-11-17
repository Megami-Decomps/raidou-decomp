#!/usr/bin/env python3
"""Compare each NON_MATCHING draft's behaviour with retail's, ignoring register allocation and scheduling.

Facts by class: call, mem (global or arg base), memx (unknown base), addr, imm, shift, float, branch.
Exit status 0 when every draft is clean, 1 when one differs, 2 on an error. --matched is the control.
"""
from __future__ import annotations

import argparse
import functools
import importlib.util
import os
import re
import struct
import subprocess
import sys
import tempfile
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BIN = ROOT / "tools/bin/mips-ps2-decompals"
CLASSES = ("call", "mem", "memx", "addr", "imm", "shift", "float", "branch")
GUARD = re.compile(r"^\s*#\s*ifdef\s+NON_MATCHING\b", re.M)
ROW = re.compile(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;(?:.*size:0x([0-9A-Fa-f]+))?")

WIDTH = {"lb": 1, "lbu": 1, "sb": 1, "lh": 2, "lhu": 2, "sh": 2,
         "lw": 4, "lwu": 4, "sw": 4, "lwc1": 4, "swc1": 4, "lwl": 4, "lwr": 4, "swl": 4, "swr": 4,
         "ld": 8, "sd": 8, "ldc1": 8, "sdc1": 8, "ldl": 8, "ldr": 8, "sdl": 8, "sdr": 8,
         "lq": 16, "sq": 16, "lqc2": 16, "sqc2": 16}
STORES = {"sb", "sh", "sw", "swc1", "swl", "swr", "sd", "sdc1", "sdl", "sdr", "sq", "sqc2"}
COND_BRANCH = {"beq", "bne", "beql", "bnel", "blez", "bgtz", "bltz", "bgez", "blezl", "bgtzl", "bltzl",
               "bgezl", "bc1t", "bc1f", "bc1tl", "bc1fl", "bltzal", "bgezal", "bc0t", "bc0f"}
# Instructions whose first operand is read, not written.
NO_DEST = STORES | COND_BRANCH | {"j", "jr", "jal", "jalr", "b", "mtc1", "ctc1", "mthi", "mtlo", "mult",
                                  "multu", "div", "divu", "dmult", "dmultu", "ddiv", "ddivu", "sync",
                                  "cache", "syscall", "break", "teq", "tne", "c.eq.s", "c.lt.s",
                                  "c.le.s", "c.olt.s", "c.ole.s", "c.f.s", "c.un.s", "qmtc2", "ctc2",
                                  "mtsa", "mtsab", "mtsah", "mult1", "multu1", "div1", "divu1",
                                  "madd", "maddu", "madd1", "maddu1", "mthi1", "mtlo1", "pmthi",
                                  "pmtlo", "bc2t", "bc2f", "mtc0", "ei", "di", "eret", "nop"}
LIKELY = {"beql", "bnel", "blezl", "bgtzl", "bltzl", "bgezl", "bc1tl", "bc1fl"}
CALLER_SAVED = {"v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                "t8", "t9", "at", "ra", *(f"$f{i}" for i in range(20))}
SHIFTS = {"sll", "srl", "sra", "dsll", "dsrl", "dsra", "dsll32", "dsrl32", "dsra32"}
IMM_OPS = {"andi", "ori", "xori", "slti", "sltiu"}
MOVES = {"addu", "daddu", "or", "addiu", "daddiu"}


def sext16(v: int) -> int:
    return v - 0x10000 if v & 0x8000 else v


def parse_int(s: str) -> int:
    return int(s, 0)


def run(*cmd, **kw) -> str:
    return subprocess.run(cmd, check=True, capture_output=True, text=True, **kw).stdout


# --- the retail image ------------------------------------------------------------------------------

@dataclass
class Image:
    version: str
    elf: bytes
    segs: list
    gp: int
    text: tuple            # (start, end) of code
    ro: tuple              # (start, end) of .rodata and .lit4
    funcs: dict            # name -> address
    func_at: dict          # address -> name
    func_starts: list      # sorted function addresses
    syms: list             # sorted (addr, name, size) of every named address
    sym_addr: dict         # name -> address
    unit_data: dict        # unit -> [(start, end)] of its data subsegments

    def read(self, addr: int, n: int) -> bytes | None:
        for vaddr, off, size in self.segs:
            if vaddr <= addr and addr + n <= vaddr + size:
                return self.elf[off + addr - vaddr: off + addr - vaddr + n]
        return None

    def func_end(self, addr: int) -> int:
        import bisect
        i = bisect.bisect_right(self.func_starts, addr)
        return self.func_starts[i] if i < len(self.func_starts) else self.text[1]

    def containing(self, addr: int):
        import bisect
        i = bisect.bisect_right(self.syms, (addr, "\x7f")) - 1
        if i < 0:
            return None
        a, name, size = self.syms[i]
        return name, addr - a


def load_image(version: str, root: Path, elf_path: Path | None) -> Image:
    vers = __import__("json").loads((root / "config/versions.json").read_text())
    info = vers[version]
    elf_path = elf_path or root / "build" / version / f"{info['serial']}.elf"
    elf = elf_path.read_bytes()
    phoff = struct.unpack_from("<I", elf, 0x1C)[0]
    segs = []
    for i in range(struct.unpack_from("<H", elf, 0x2C)[0]):
        p_type, p_off, p_vaddr, _, p_filesz = struct.unpack_from("<5I", elf, phoff + i * 32)
        if p_type == 1:
            segs.append((p_vaddr, p_off, p_filesz))
    yaml = (root / "config" / version / f"{info['serial']}.yaml").read_text()
    named = [(int(m.group(1), 16), m.group(2), m.group(3)) for m in
             re.finditer(r"^\s+- \[(0x[0-9A-Fa-f]+),\s*([a-z.0-9]+)(?:,\s*([\w/]+))?", yaml, re.M)]
    subs = [(r, t) for r, t, _ in named]
    vram = lambda rom: rom - 0x1000 + 0x100000  # noqa: E731
    ro_start = min(vram(r) for r, t in subs if t in ("rodata", ".rodata"))
    sd_start = min(vram(r) for r, t in subs if t in ("sdata", ".sdata") and vram(r) > ro_start)
    data_start = min(vram(r) for r, t in subs if t in ("data", ".data", "databin"))
    funcs, others = {}, {}
    for line in run(f"{BIN}-nm", "-n", str(elf_path)).splitlines():
        p = line.split()
        if len(p) == 3 and p[2].startswith((".", "$")):
            continue
        if len(p) == 3 and p[1] in "Tt" and int(p[0], 16) < data_start:
            funcs.setdefault(p[2], int(p[0], 16))
        elif len(p) == 3:
            others.setdefault(p[2], int(p[0], 16))
    func_at = {}
    for name, a in funcs.items():
        func_at.setdefault(a, name)
    syms = {}
    for line in (root / "config" / version / "symbol_addrs.txt").read_text().splitlines():
        if m := ROW.match(line):
            syms.setdefault(m.group(1), (int(m.group(2), 16), int(m.group(3) or "0", 16)))
    for name, a in (*funcs.items(), *others.items()):
        syms.setdefault(name, (a, 0))
    by_addr = {}
    for name, (a, size) in syms.items():
        if a not in by_addr or by_addr[a][0].startswith(("D_", "func_", "jtbl_")):
            by_addr[a] = (name, size)
    unit_data = {}
    for (rom, typ, unit), nxt in zip(named, named[1:]):
        if unit and typ.lstrip(".") in ("rodata", "sdata", "data", "lit4", "lit8"):
            unit_data.setdefault(unit, []).append((vram(rom), vram(nxt[0])))
    return Image(version, elf, segs, int(info["gp"], 16), (0x100000, data_start), (ro_start, sd_start),
                 funcs, func_at, sorted(func_at), sorted((a, n, s) for a, (n, s) in by_addr.items()),
                 {n: a for n, (a, _) in syms.items()}, unit_data)


# --- instructions ----------------------------------------------------------------------------------

@dataclass
class Insn:
    addr: int
    op: str
    args: list
    reloc: tuple | None = None   # (type, symbol, addend shown with the symbol)


INSN = re.compile(r"^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$")
RELOC = re.compile(r"^\s*([0-9a-f]+): (R_MIPS_\w+)\s+(\S+)")


def parse_objdump(text: str) -> list[Insn]:
    """Every instruction of objdump -dr -M no-aliases output, relocations attached."""
    out = []
    for line in text.splitlines():
        if m := RELOC.match(line):
            if out and out[-1].addr == int(m.group(1), 16):
                sym, _, add = m.group(3).partition("+")
                out[-1].reloc = (m.group(2), sym, int(add, 16) if add else 0)
        elif (m := INSN.match(line)) and m.group(2) != "...":
            args = [a.strip() for a in m.group(3).split("<")[0].split(",")] if m.group(3) else []
            out.append(Insn(int(m.group(1), 16), m.group(2), [a for a in args if a]))
    return out


def objdump_func(img: Image, elf_path: Path, name: str) -> list[Insn]:
    start = img.funcs[name]
    end = img.func_end(start)
    text = run(f"{BIN}-objdump", "-d", "-M", "no-aliases", "--no-show-raw-insn",
               f"--start-address={start:#x}", f"--stop-address={end:#x}", str(elf_path))
    return parse_objdump(text)


MEMOP = re.compile(r"^(-?\w+)\((\w+)\)$")


# --- reduction to facts ----------------------------------------------------------------------------

class Resolver:
    """Turns an address (retail) or a relocation (draft) into a canonical reference."""

    def __init__(self, img: Image, obj=None):
        self.img, self.obj = img, obj

    def name_addr(self, name: str):
        return self.img.sym_addr.get(name)

    def canon_ref(self, ref, how):
        if ref[0] == "A":
            return self.canon_addr(ref[1], how)
        if ref[0] == "N":
            return f"{ref[1]}+{ref[2]:#x}" if ref[2] else ref[1]
        return self.canon_local(ref[1], ref[2], how)

    def canon_addr(self, addr: int, how: str):
        """how: a load/store mnemonic, or '&' for an address taken."""
        img = self.img
        if img.ro[0] <= addr < img.ro[1]:
            return self.ro_content(lambda n, a=addr: img.read(a, n), how,
                                   lambda: self.sym_name(addr), lambda w: self.retail_pointer(addr, w))
        return self.sym_name(addr)

    def retail_pointer(self, addr: int, width: int):
        """'code' for a jump table word, else what each pointer word loaded from addr points at."""
        b = self.img.read(addr, width) if width >= 4 else None
        words = [struct.unpack_from("<I", b, k)[0] for k in range(0, len(b) // 4 * 4, 4)] if b else []
        if any(self.img.text[0] <= v < self.img.text[1] and v not in self.img.func_at for v in words):
            return "code"
        if any(v in self.img.func_at for v in words):
            return [self.img.func_at.get(v, f"{v:#x}") for v in words]
        if not any(self.img.text[1] <= v < self.img.ro[1] + 0x200000 for v in words):
            return None
        return [self.canon_addr(v, "&") if self.img.text[1] <= v < self.img.ro[1] + 0x200000 else f"{v:#x}"
                for v in words]

    def sym_name(self, addr: int):
        if addr in self.img.func_at:
            return self.img.func_at[addr]
        c = self.img.containing(addr)
        if c is None:
            return f"{addr:#x}"
        name, off = c
        return f"{name}+{off:#x}" if off else name

    @staticmethod
    def ro_content(read, how, name, pointer):
        """Read-only data is compared by content, so a literal the compiler emitted matches the
        retail split's named copy. pointer(width) says 'code', 'data' or None for the loaded word."""
        if how == "lwc1":
            b = read(4)
            return f"f32:{struct.unpack('<f', b)[0]!r}" if b else name()
        if how == "ldc1":
            b = read(8)
            return f"f64:{struct.unpack('<d', b)[0]!r}" if b else name()
        if how == "&":
            s = ro_string(read)
            if s is not None:
                return f"str:{s!r}"
            kind, b = pointer(4), read(8)
            if isinstance(kind, list):
                return "&ptr:" + ",".join(kind)
            return f"&ro:{b.hex()}" if b else name()
        width = WIDTH.get(how, 0)
        b = read(width) if width else None
        if not b:
            return name()
        kind = pointer(width)
        if kind == "code":
            return "jtbl"
        if kind:
            return "ptr:" + ",".join(kind)     # a table of pointers, compared by what they point at
        return f"ro:{b.hex()}"


def ro_string(read):
    out = bytearray()
    for i in range(256):
        b = read(i + 1)
        if b is None:
            return None
        c = b[-1]
        if c == 0:
            break
        out.append(c)
    else:
        return None
    try:
        s = out.decode("ascii")
    except UnicodeDecodeError:
        try:
            s = out.decode("shift_jis")
        except UnicodeDecodeError:
            return None
    if not s or not all(ch.isprintable() or ch in "\n\t" for ch in s):
        return None
    return s


@dataclass
class ObjInfo:
    sections: dict          # name -> bytes
    syms: dict              # (section, offset) -> local name
    data_relocs: dict       # section -> {offset: symbol} of the R_MIPS_32 relocations in data
    funcs: dict             # name -> (offset, size)


class Facts:
    def __init__(self):
        self.c = {k: Counter() for k in CLASSES}
        self.at = 0

    def add(self, cls, item):
        self.c[cls][item] += 1
        if TRACE:
            print(f"  {self.at:x} {cls} {item}", file=sys.stderr)


@dataclass
class Val:
    kind: str               # c const, a address, L obj-local, hi, p param, ld loaded, r return, u unknown
    v: object = None
    off: int = 0
    ix: bool = False        # an index was added to an address


UNK = Val("u")
TRACE = os.environ.get("NMB_TRACE")


def reduce_function(insns: list[Insn], res: Resolver, is_draft: bool, func_range: tuple) -> Facts:
    i = 0
    n = len(insns)
    order = []
    while i < n:
        ins = insns[i]
        if ins.op in ("jal", "jalr", "j", "jr") or ins.op in COND_BRANCH or ins.op == "b":
            if i + 1 < n:
                order.append((insns[i + 1], ins.addr))   # the slot runs first, but joins at the jump
            order.append((ins, None))
            i += 2
        else:
            order.append((ins, ins.addr))
            i += 1


    def scan(back: dict):
        """One pass in program order; back maps a loop head to the states its back edges carried
        on the previous pass."""
        incoming: dict[int, list] = {}
        out_back: dict[int, list] = {}
        dead = skipped_slot = False
        pre_slot: dict = {}
        f = Facts()
        regs: dict[str, Val] = {f"a{i}": Val("p", i) for i in range(4)}
        if not is_draft:
            regs["gp"] = Val("a", res.img.gp)
        img = res.img
        in_image = lambda v: img.text[0] <= v < img.ro[1] + 0x200000  # noqa: E731

        def get(r):
            if r == "zero":
                return Val("c", 0)
            return regs.get(r, UNK)

        def addr_of(val: Val):
            """Concrete reference for an address-like value, or None."""
            if val.kind == "a":
                return ("A", val.v + val.off)
            if val.kind == "c" and in_image(val.v + val.off):
                return ("A", val.v + val.off)
            if val.kind == "L":
                return (*val.v[:-1], val.v[-1] + val.off)
            return None

        canon = res.canon_ref

        def consume(r):
            """Register r is read as a value (not as an address base)."""
            val = get(r)
            if r == "zero":
                return
            ref = addr_of(val)
            if ref is not None:
                # A bare lui half (draft: an unpaired %hi) is shared by several addresses.
                if val.ix or (val.kind == "c" and not (val.v + val.off) & 0xFFFF):
                    return
                f.add("addr", canon(ref, "&"))
                return
            if val.kind == "c":
                if val.v + val.off not in (0,):
                    f.add("imm", val.v + val.off)
            elif val.off and val.kind != "hi":
                f.add("imm", val.off)

        def base_desc(val: Val):
            if val.kind == "p":
                return f"arg{val.v}"
            if val.kind == "ld" and not val.ix and isinstance(val.v, str) and not val.v.startswith(("?", "*", "arg")):
                return f"*({val.v})"
            return "?"

        def mem(insn: Insn):
            op = insn.op
            m = MEMOP.match(insn.args[1])
            if not m:
                return
            imm, base = parse_int(m.group(1)), m.group(2)
            kind = "S" if op in STORES else "L"
            width = WIDTH[op]
            if op in ("ldl", "lwl", "sdl", "swl"):
                imm -= width - 1    # the left half of an unaligned pair names the same object
            key = None
            if base == "sp":     # a stack slot holds a register's value across a spill
                if kind == "S":
                    regs[f"@{imm}"] = get(insn.args[0]) if op in ("sw", "sd", "sq") else UNK
                return None
            if insn.reloc and insn.reloc[0] in ("R_MIPS_GPREL16", "R_MIPS_LITERAL"):
                key = canon(res.reloc_ref(insn.reloc[1], insn.reloc[2] + imm), op)
            elif insn.reloc and insn.reloc[0] == "R_MIPS_LO16":
                bv = get(base)
                hi = bv.v[1] if bv.kind == "hi" else 0
                key = canon(res.reloc_ref(insn.reloc[1], hi + imm), op)
            elif base == "gp" and not is_draft:
                key = canon(("A", img.gp + imm), op)
            else:
                bv = get(base)
                ref = addr_of(bv)
                if ref is not None:
                    ref = ref[:-1] + (ref[-1] + imm,)
                    key = canon(ref, op)
                else:
                    key = f"{base_desc(bv)}+{bv.off + imm:#x}"
            if key.startswith(("f32:", "f64:")):
                f.add("float", key)
            elif key.startswith("?"):
                f.add("memx", (key, width, kind))
            else:
                f.add("mem", (key, width, kind))
            if kind == "S" and op not in ("swc1", "sdc1", "sqc2"):
                consume(insn.args[0])
            return key

        for ins, at in order:
            if at is None:
                if skipped_slot:
                    skipped_slot = False
                    continue
            elif dead and at not in incoming and at not in back:
                skipped_slot = ins.addr != at
                continue    # unreachable: padding after a jump, or reached by no branch
            elif dead or at in incoming or at in back:
                states = incoming.pop(at, []) + back.get(at, []) + ([] if dead else [regs])
                regs = meet(states)
                dead = False
            if at is not None and ins.addr != at:
                pre_slot = dict(regs)
            f.at = ins.addr
            target = branch_target(ins)
            if target is not None and func_range[0] <= target < func_range[1]:
                if target > ins.addr:
                    incoming.setdefault(target, []).append(dict(regs))
                else:
                    out_back.setdefault(target, []).append(dict(regs))
                dead = unconditional(ins)
            if ins.op in LIKELY:
                regs = pre_slot     # a likely branch runs its slot only when taken
            elif ins.op in ("jr", "j"):
                dead = True
            op, a = ins.op, ins.args
            if op == "nop" or (op == "sll" and a[:2] == ["zero", "zero"]):
                continue
            if is_restore(ins):
                continue
            if op in WIDTH:
                key = mem(ins)
                if op not in STORES and key is None and a[1].endswith("(sp)"):
                    regs[a[0]] = regs.get(f"@{parse_int(a[1].split('(')[0])}", UNK)
                elif op not in STORES:
                    regs[a[0]] = Val("ld", key) if key else UNK
                continue
            if op in COND_BRANCH:
                if not (op.startswith("beq") and a[0] == "zero" and a[1] == "zero"):
                    f.add("branch", "conditional")
                post, regs = regs, pre_slot     # the operands are read before the slot runs
                for r in a:
                    if r in regs:
                        consume(r)
                regs = post
                continue
            if op in ("jal", "j"):
                t = parse_int("0x" + a[0])
                if op == "j" and (not ins.reloc or ins.reloc[1] == ".text") and func_range[0] <= t < func_range[1]:
                    continue
                target = res.call_name(ins.reloc[1], t) if ins.reloc else img.func_at.get(t, f"{t:#x}")
                f.add("call", target)
                for r in CALLER_SAVED:
                    regs.pop(r, None)
                regs["v0"] = Val("r", target)
                continue
            if op == "jalr":
                f.add("call", "indirect")
                for r in CALLER_SAVED:
                    regs.pop(r, None)
                regs["v0"] = Val("r", "indirect")
                continue
            if op == "jr":
                if a and a[0] == "ra":
                    if "v0" in regs and regs["v0"].kind == "c" and addr_of(regs["v0"]) is None:
                        consume("v0")   # a constant returned; an address left in v0 may be garbage of a void
                continue
            if op == "b":
                continue
            if op == "lui":
                if ins.reloc and ins.reloc[0] == "R_MIPS_HI16":
                    regs[a[0]] = Val("hi", (ins.reloc[1], parse_int(a[1]) << 16))
                else:
                    regs[a[0]] = Val("c", parse_int(a[1]) << 16)
                continue
            if op in ("addiu", "daddiu", "ori"):
                src = get(a[1])
                imm = parse_int(a[2])
                if ins.reloc and ins.reloc[0] == "R_MIPS_LO16":
                    hi = src.v[1] if src.kind == "hi" else 0
                    ref = res.reloc_ref(ins.reloc[1], hi + sext16(imm & 0xFFFF) if op != "ori" else hi | imm)
                    regs[a[0]] = Val("a", ref[1]) if ref[0] == "A" else Val("L", ref)
                    continue
                if ins.reloc and ins.reloc[0] == "R_MIPS_GPREL16":
                    ref = res.reloc_ref(ins.reloc[1], ins.reloc[2] + imm)
                    regs[a[0]] = Val("a", ref[1]) if ref[0] == "A" else Val("L", ref)
                    continue
                if a[1] == "gp" and not is_draft:
                    regs[a[0]] = Val("a", img.gp + imm)
                    continue
                if op == "ori":
                    if src.kind == "c":
                        regs[a[0]] = Val("c", (src.v + src.off) | imm)
                    else:
                        f.add("imm", imm)
                        consume(a[1])
                        regs[a[0]] = UNK
                    continue
                if a[1] == "sp":
                    regs[a[0]] = UNK
                    continue
                if src.kind == "c":
                    regs[a[0]] = Val("c", src.v + src.off + imm)
                else:
                    regs[a[0]] = Val(src.kind, src.v, src.off + imm, src.ix)
                continue
            if op in IMM_OPS:
                src = get(a[1])
                imm = parse_int(a[2])
                if op == "slti" and imm == 0:   # x < 0, which gcc also writes as x >> 31
                    f.add("shift", "sign")
                    consume(a[1])
                    regs[a[0]] = UNK
                    continue
                if op == "andi" and addr_of(src) is not None:
                    regs[a[0]] = UNK    # an alignment test of a block copy's source
                    continue
                if src.kind == "c" and op == "andi":
                    regs[a[0]] = Val("c", (src.v + src.off) & imm)
                    continue
                f.add("imm", imm)
                consume(a[1])
                regs[a[0]] = UNK
                continue
            if op in SHIFTS:
                src = get(a[1])
                if src.kind == "c" and op == "sll":
                    regs[a[0]] = Val("c", ((src.v + src.off) << parse_int(a[2])) & 0xFFFFFFFF)
                    continue
                sa = parse_int(a[2]) + (32 if op.endswith("32") else 0)
                if op == "srl" and sa == 31:
                    f.add("shift", "sign")
                elif sa:
                    f.add("shift", (op.rstrip("32"), sa))
                consume(a[1])
                regs[a[0]] = UNK
                continue
            if op in ("addu", "daddu", "or") and len(a) == 3 and "zero" in a[1:]:
                other = a[1] if a[2] == "zero" else a[2]
                regs[a[0]] = get(other)
                continue
            if op in ("addu", "daddu") and len(a) == 3:
                x, y = get(a[1]), get(a[2])
                rx, ry = addr_of(x), addr_of(y)
                if y.kind == "c" and ry is None and x.kind != "c":
                    regs[a[0]] = Val(x.kind, x.v, x.off + y.v + y.off, x.ix)
                    continue
                if x.kind == "c" and rx is None and y.kind != "c":
                    regs[a[0]] = Val(y.kind, y.v, y.off + x.v + x.off, y.ix)
                    continue
                if rx is not None and ry is None:
                    consume(a[2])
                    regs[a[0]] = Val(x.kind, x.v, x.off, True)
                    continue
                if ry is not None and rx is None:
                    consume(a[1])
                    regs[a[0]] = Val(y.kind, y.v, y.off, True)
                    continue
            if op == "mtc1":
                src = get(a[0])
                if src.kind == "c" and src.v + src.off:
                    f.add("float", f"f32:{struct.unpack('<f', struct.pack('<I', (src.v + src.off) & 0xFFFFFFFF))[0]!r}")
                regs[a[1]] = UNK
                continue
            if op == "mfc1":
                regs[a[0]] = get(a[1])
                continue
            # Generic: every register operand after the destination is read.
            regops = [r for r in a if re.fullmatch(r"(zero|at|v[01]|a[0-3]|t\d|s[0-8]|k[01]|gp|sp|fp|ra)", r)]
            if op in NO_DEST:
                for r in regops:
                    consume(r)
            else:
                for r in regops[1:]:
                    consume(r)
                if a:
                    regs[a[0]] = UNK
        return f, out_back

    back: dict = {}
    for _ in range(8):
        f, out_back = scan(back)
        if out_back == back:
            break
        back = out_back
    return f


CALLEE_SAVED = re.compile(r"s[0-8]|ra|gp|fp|\$f(2\d|3[01])")


def is_restore(ins: Insn) -> bool:
    """An epilogue reload of a callee-saved register, which only precedes a return."""
    return (ins.op in WIDTH and ins.op not in STORES and len(ins.args) == 2
            and ins.args[1].endswith("(sp)") and bool(CALLEE_SAVED.fullmatch(ins.args[0])))


def branch_target(ins: Insn) -> int | None:
    if ins.op not in COND_BRANCH and ins.op not in ("b", "j"):
        return None
    if ins.reloc and ins.reloc[1] != ".text":
        return None     # a tail call to another function
    try:
        return int(ins.args[-1], 16)
    except (ValueError, IndexError):
        return None


def unconditional(ins: Insn) -> bool:
    return ins.op in ("b", "j") or (ins.op == "beq" and ins.args[0] == ins.args[1] == "zero")


def meet(states: list[dict]) -> dict:
    """Registers whose value is the same on every incoming path."""
    if not states:
        return {}
    first = states[0]
    return {r: v for r, v in first.items() if all(s.get(r) == v for s in states[1:])}


class DraftResolver(Resolver):
    def __init__(self, img: Image, obj: ObjInfo, unit: str):
        super().__init__(img, obj)
        self.unit = unit
        self.finding: set = set()

    def reloc_ref(self, sym: str, addend: int):
        if sym.startswith("."):
            c = self.local_containing(sym, addend)
            if c and not c[0].startswith("$") and (a := self.name_addr(c[0])) is not None:
                return ("A", a + c[1])
            if sym == ".text":
                name = next((n for n, (o, _) in self.obj.funcs.items() if o == addend), None)
                if name and (a := self.name_addr(name)) is not None:
                    return ("A", a)
            if not sym.startswith((".rodata", ".lit")) and (a := self.find_literal(sym, addend)) is not None:
                return ("A", a)
            return ("L", sym, addend)
        a = self.name_addr(sym)
        if a is None:
            loc = next(((s, o) for (s, o), n in self.obj.syms.items() if n == sym), None)
            if loc:
                return ("L", loc[0], loc[1] + addend)
            return ("N", sym, addend)
        return ("A", a + addend)

    def literal_span(self, sec: str, off: int):
        starts = sorted(o for (s, o) in self.obj.syms if s == sec)
        start = max((o for o in starts if o <= off), default=0)
        end = min((o for o in starts if o > start), default=len(self.obj.sections.get(sec, b"")))
        return start, end

    def find_literal(self, sec: str, off: int):
        """Retail address of a compiler literal ($LC, or data with no symbol), found by its bytes in
        the unit's own retail data; None if it is not there exactly once."""
        start, end = self.literal_span(sec, off)
        name = self.obj.syms.get((sec, start))
        if name and not name.startswith("$"):
            return None
        data = self.obj.sections.get(sec, b"")
        lit = data[start:end]
        relocs = {}
        for r, sym in self.obj.data_relocs.get(sec, {}).items():
            if start <= r < end and (sec, start) not in self.finding:
                self.finding.add((sec, start))
                ref = self.reloc_ref(sym, struct.unpack_from("<I", data, r)[0])
                self.finding.discard((sec, start))
                relocs[r - start] = struct.pack("<I", ref[1]) if ref[0] == "A" else None
            elif start <= r < end:
                relocs[r - start] = None
        if not relocs and len(lit.rstrip(b"\0")) >= 2:
            lit = lit.rstrip(b"\0") + b"\0" * (lit[-1:] == b"\0")
        if len(lit) - 4 * sum(v is None for v in relocs.values()) < 4:
            return None
        # A relocated word whose target has no retail address yet matches any value.
        pattern = re.compile(b"".join(re.escape(relocs[k]) if relocs.get(k) else b"...." if k in relocs
                                      else re.escape(lit[k:k + 4]) for k in range(0, len(lit), 4)), re.S)
        hits = []
        for lo, hi in self.img.unit_data.get(self.unit, ()):
            blob = self.img.read(lo, hi - lo) or b""
            hits += [lo + m.start() for m in pattern.finditer(blob) if (lo + m.start()) % 4 == start % 4]
        return hits[0] + off - start if len(hits) == 1 else None

    def local_containing(self, sec: str, off: int):
        best = max(((o, n) for (s, o), n in self.obj.syms.items() if s == sec and o <= off), default=None)
        return (best[1], off - best[0]) if best else None

    def call_name(self, sym: str, target: int):
        if sym == ".text":
            for name, (off, size) in self.obj.funcs.items():
                if off == target:
                    sym = name
                    break
        a = self.name_addr(sym)
        return self.img.func_at.get(a, sym) if a is not None else sym

    def canon_local(self, sec: str, off: int, how: str):
        data = self.obj.sections.get(sec, b"")
        read = lambda n: data[off:off + n] if off + n <= len(data) else None  # noqa: E731
        c = self.local_containing(sec, off)
        name = (f"{c[0]}+{c[1]:#x}" if c[1] else c[0]) if c else f"{sec}+{off:#x}"
        if sec.startswith((".rodata", ".lit")):
            return self.ro_content(read, how, lambda: name, lambda w: self.local_pointer(sec, off, w))
        return name

    def local_pointer(self, sec: str, off: int, width: int):
        relocs = self.obj.data_relocs.get(sec, {})
        data = self.obj.sections.get(sec, b"")
        if width < 4 or not any(off + k in relocs for k in range(0, width, 4)):
            return None
        out = []
        for k in range(0, width // 4 * 4, 4):
            sym = relocs.get(off + k)
            word = struct.unpack_from("<I", data, off + k)[0] if off + k + 4 <= len(data) else 0
            if sym is None:
                out.append(f"{word:#x}")
            elif sym == ".text":
                name = next((n for n, (o, _) in self.obj.funcs.items() if o == word), None)
                if name is None:
                    return "code"
                out.append(self.call_name(name, 0))
            elif sym in self.obj.funcs or self.name_addr(sym) in self.img.func_at:
                out.append(self.call_name(sym, 0))
            else:
                out.append(self.canon_ref(self.reloc_ref(sym, word), "&"))
        return out


def obj_info(obj: Path) -> ObjInfo:
    readelf = run(f"{BIN}-readelf", "-S", "-W", str(obj))
    raw = obj.read_bytes()
    sections = {}
    for m in re.finditer(r"\]\s+(\S+)\s+(PROGBITS|NOBITS)\s+[0-9a-f]+\s+([0-9a-f]+)\s+([0-9a-f]+)", readelf):
        name, typ, off, size = m.group(1), m.group(2), int(m.group(3), 16), int(m.group(4), 16)
        sections[name] = raw[off:off + size] if typ == "PROGBITS" else bytes(size)
    syms, funcs = {}, {}
    for line in run(f"{BIN}-objdump", "-t", str(obj)).splitlines():
        m = re.match(r"^([0-9a-f]{8}) (.{7}) (\S+)\s+([0-9a-f]{8}) (\S+)$", line)
        if not m:
            continue
        val, flags, sec, size, name = int(m.group(1), 16), m.group(2), m.group(3), int(m.group(4), 16), m.group(5)
        if "F" in flags and sec == ".text":
            funcs[name] = (val, size)
        elif (sec.startswith(".") and sec != ".text" and "d" not in flags and not name.startswith(".")
              and name not in ("gcc2_compiled.", "__gnu_compiled_c")):
            syms.setdefault((sec, val), name)
    data_relocs = {}
    for line in run(f"{BIN}-objdump", "-r", str(obj)).splitlines():
        if m := re.match(r"^RELOCATION RECORDS FOR \[(\S+)\]", line):
            cur = data_relocs.setdefault(m.group(1), {})
        elif m := re.match(r"^([0-9a-f]{8}) R_MIPS_\w+\s+([^+\s]+)", line):
            cur[int(m.group(1), 16)] = m.group(2)
    return ObjInfo(sections, syms, data_relocs, funcs)


# --- driver ----------------------------------------------------------------------------------------

@dataclass
class Result:
    version: str
    unit: str
    func: str
    diffs: dict = field(default_factory=dict)   # class -> (retail only, draft only)
    draft: bool = True
    error: str = ""


def comparable(facts: Facts, cls: str) -> Counter:
    """Calls and branches compare as counts. Everything else compares as a set: a load or constant
    can be fetched again, and a store in two arms can be merged into one tail."""
    c = facts.c[cls]
    if cls in ("call", "branch"):
        return c
    return Counter(dict.fromkeys(c, 1))


def diff_facts(retail: Facts, draft: Facts) -> dict:
    out = {}
    for cls in CLASSES:
        r, d = comparable(retail, cls), comparable(draft, cls)
        if r != d:
            out[cls] = (r - d, d - r)
    return out


def pair_literals(diffs: dict, res: "DraftResolver") -> dict:
    """Pair a draft load of a compiler literal that byte search could not place (an empty string,
    say) with a retail load of the same width from data holding the same bytes."""
    if "mem" not in diffs:
        return diffs
    ronly, donly = diffs["mem"]
    for dk in [k for k in donly if k[0].startswith("$")]:
        loc = next(((sec, o) for (sec, o), n in res.obj.syms.items() if n == dk[0]), None)
        if loc is None:
            continue
        start, end = res.literal_span(*loc)
        lit = res.obj.sections.get(loc[0], b"")[start:end]
        for rk in ronly:
            name, _, off = rk[0].partition("+")
            a = res.img.sym_addr.get(name)
            if rk[1:] == dk[1:] and not off and a is not None and res.img.read(a, len(lit)) == lit:
                donly[dk] -= 1
                ronly[rk] -= 1
                break
    ronly, donly = +ronly, +donly
    if ronly or donly:
        return {**diffs, "mem": (ronly, donly)}
    return {k: v for k, v in diffs.items() if k != "mem"}


def compile_unit(root: Path, path: Path, version: str, nonmatching: bool, out: Path) -> str:
    env = dict(os.environ, RAIDOU_VERSION=version, RAIDOU_AS_UNIT=str(path.relative_to(root)))
    r = subprocess.run([str(root / "tools/cc.sh"), "-DSKIP_ASM", *(["-DNON_MATCHING"] if nonmatching else []),
                        str(path), "-o", str(out)], capture_output=True, text=True, env=env, cwd=root)
    return "" if r.returncode == 0 else (r.stderr.strip().splitlines() or ["compile failed"])[-1]


def check_unit(root: Path, img: Image, elf_path: Path, path: Path, only: set | None,
               matched: bool = False) -> list[Result]:
    unit = str(path.relative_to(root))
    with tempfile.TemporaryDirectory() as tmp:
        nm_o, base_o = Path(tmp) / "nm.o", Path(tmp) / "base.o"
        for o, flag in ((nm_o, True), (base_o, False)):
            if err := compile_unit(root, path, img.version, flag, o):
                return [Result(img.version, unit, "-", error=err)]
        info = obj_info(nm_o)
        base_funcs = set(obj_info(base_o).funcs)
        drafts = [n for n in info.funcs if (matched or n not in base_funcs) and (not only or n in only)]
        if not drafts:
            return []
        dis = parse_objdump(run(f"{BIN}-objdump", "-dr", "-M", "no-aliases", "--no-show-raw-insn", str(nm_o)))
    out = []
    res = DraftResolver(img, info, load_units(root).unit_name(path.relative_to(root)))

    def draft_facts(name):
        off, size = info.funcs[name]
        return reduce_function([i for i in dis if off <= i.addr < off + size], res, True, (off, off + size))

    # A draft-only function retail has no symbol for is a helper retail inlined: its facts count
    # in each caller in place of the call.
    helpers = {n: draft_facts(n) for n in info.funcs if n not in img.funcs and n not in base_funcs}
    for name in drafts:
        if name in helpers:
            continue
        if name not in img.funcs:
            out.append(Result(img.version, unit, name, error="no retail symbol"))
            continue
        start = img.funcs[name]
        try:
            retail = reduce_function(objdump_func(img, elf_path, name), Resolver(img), False,
                                     (start, img.func_end(start)))
            draft = draft_facts(name)
            inline_helpers(draft, helpers)
        except Exception as e:  # noqa: BLE001
            out.append(Result(img.version, unit, name, error=f"{type(e).__name__}: {e}"))
            continue
        out.append(Result(img.version, unit, name, pair_literals(diff_facts(retail, draft), res),
                          name not in base_funcs))
    return out


def inline_helpers(facts: Facts, helpers: dict, depth: int = 0):
    for name, n in list(facts.c["call"].items()):
        if name in helpers and depth < 4:
            del facts.c["call"][name]
            sub = Facts()
            for cls in CLASSES:
                sub.c[cls] = helpers[name].c[cls].copy()
            inline_helpers(sub, helpers, depth + 1)
            for cls in CLASSES:
                for k, m in sub.c[cls].items():
                    facts.c[cls][k] += m * n


def fmt_items(c: Counter) -> str:
    def one(k):
        if isinstance(k, tuple) and len(k) == 3:
            return f"{k[2]}{k[1]} {k[0]}"
        if isinstance(k, tuple):
            return f"{k[0]} {k[1]}"
        if isinstance(k, int):
            return f"{k:#x}" if abs(k) > 9 else str(k)
        return str(k)
    return "; ".join(f"{one(k)}" + (f" x{n}" if n > 1 else "") for k, n in sorted(c.items(), key=str))


def result_rows(r: Result) -> list[tuple]:
    if r.error:
        return [(r.version, r.unit, r.func, "error", r.error, "")]
    return [(r.version, r.unit, r.func, cls, fmt_items(ro), fmt_items(do)) for cls, (ro, do) in r.diffs.items()]


@functools.cache
def load_units(root: Path):
    """tools/units.py of the tree being checked, which may not be this one."""
    spec = importlib.util.spec_from_file_location(f"units_{abs(hash(root))}", root / "tools/units.py")
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def draft_units(root: Path, version: str, every: bool = False) -> list[Path]:
    return [p for p in load_units(root).unit_sources(version).values() if every or GUARD.search(p.read_text())]


def run_all(root: Path, versions: list[str], paths: list[Path], only: set | None, elf: Path | None,
            jobs: int, matched: bool = False) -> list[Result]:
    results = []
    for version in versions:
        img = load_image(version, root, elf)
        elf_path = elf or root / "build" / version / f"{__import__('json').loads((root / 'config/versions.json').read_text())[version]['serial']}.elf"
        todo = paths or draft_units(root, version, matched)
        if paths:
            built = set(load_units(root).unit_sources(version).values())
            todo = [p for p in todo if p in built]
        with ThreadPoolExecutor(jobs) as ex:
            for rs in ex.map(lambda p: check_unit(root, img, elf_path, p, only, matched), todo):
                results.extend(rs)
    return results


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("units", nargs="*", type=Path)
    ap.add_argument("--version", action="append", help="default: every version")
    ap.add_argument("--func", action="append", help="only these functions")
    ap.add_argument("--root", type=Path, default=ROOT, help="tree to compile (default: this one)")
    ap.add_argument("--elf", type=Path, help="matching ELF for the retail side (default: <root>/build)")
    ap.add_argument("--tsv", type=Path, help="write version, unit, func, class, retail only, draft only")
    ap.add_argument("--matched", action="store_true",
                    help="check every C function, not only drafts (a control: all must be clean)")
    ap.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 4)
    args = ap.parse_args()
    root = args.root.resolve()
    versions = args.version or list(__import__("json").loads((root / "config/versions.json").read_text()))
    paths = [p.resolve() for p in args.units]
    results = run_all(root, versions, paths, set(args.func) if args.func else None, args.elf, args.jobs,
                      args.matched)
    rows = [row for r in results for row in result_rows(r)]
    if args.tsv:
        args.tsv.write_text("".join("\t".join(map(str, row)) + "\n" for row in rows))
    for row in rows:
        print(f"{row[0]} {row[2]} {row[3]}: retail [{row[4]}] draft [{row[5]}]")
    bad = {(r.version, r.func) for r in results if r.diffs}
    err = [r for r in results if r.error]
    drafts = [r for r in results if r.draft]
    print(f"{len(drafts)} drafts, {sum(bool(r.diffs) for r in drafts)} differ, {len(err)} errors", file=sys.stderr)
    if args.matched:
        ok = [r for r in results if not r.draft]
        print(f"{len(ok)} matched, {sum(bool(r.diffs) for r in ok)} differ", file=sys.stderr)
    return 2 if err else 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
