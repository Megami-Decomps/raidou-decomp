#!/usr/bin/env python3
"""Export functions from an ELF plus a splat symbol_addrs.txt, in ida_export_funcs.py's format.

Usage: elf_export_funcs.py ELF SYMBOL_ADDRS OUT.json
A function ends at the next function symbol or at the end of .text. Calls are jal
targets; strings are lui/addiu pairs that point at a NUL-terminated printable run.
"""
import json
import re
import struct
import sys


def sections(elf):
    shoff, = struct.unpack_from("<I", elf, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", elf, 0x2E)
    hdrs = [struct.unpack_from("<IIIIIIIIII", elf, shoff + i * shentsize) for i in range(shnum)]
    strtab = hdrs[shstrndx][4]
    out = {}
    for h in hdrs:
        name = elf[strtab + h[0]:elf.index(b"\0", strtab + h[0])].decode()
        out[name] = (h[3], h[4], h[5], h[1])  # addr, offset, size, type
    return out


def trim_padding(words):
    """Drop alignment nops after the last jump, keeping its delay slot."""
    n = len(words)
    while n > 2 and words[n - 1] == 0 and words[n - 2] == 0:
        n -= 1
    if n > 1 and words[n - 1] == 0 and not is_jump(words[n - 2]):
        n -= 1
    return words[:n]


def is_jump(w):
    op = w >> 26
    return op in (2, 3) or (op == 0 and (w & 0x3F) in (8, 9)) or 4 <= op <= 7 or op in (1, 0x14, 0x15, 0x16, 0x17)


def main():
    elf = open(sys.argv[1], "rb").read()
    secs = sections(elf)
    text_addr, text_off, text_size, _ = secs[".text"]
    text_end = text_addr + text_size
    data = [(a, o, s) for n, (a, o, s, t) in secs.items() if t == 1 and n not in (".text", ".vutext") and a]

    def read_str(ea):
        for a, o, s in data:
            if a <= ea < a + s:
                end = elf.find(b"\0", o + ea - a, o + s)
                raw = elf[o + ea - a:end]
                if len(raw) >= 4 and all(32 <= c < 127 or c in (9, 10) for c in raw):
                    return raw.decode("latin-1")
        return None

    syms = {}
    for line in open(sys.argv[2]):
        m = re.match(r"\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+);.*type:func", line)
        if m:
            ea = int(m.group(2), 16)
            if text_addr <= ea < text_end and ea not in syms:
                syms[ea] = m.group(1)
    starts = sorted(syms)
    funcs = []
    for i, ea in enumerate(starts):
        end = starts[i + 1] if i + 1 < len(starts) else text_end
        words = list(struct.unpack_from(f"<{(end - ea) // 4}I", elf, text_off + ea - text_addr))
        words = trim_padding(words)
        calls, strs, drefs, hi = [], [], [], {}
        for k, w in enumerate(words):
            op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
            if op == 3:
                t = ((ea + 4 * k) & 0xF0000000) | ((w & 0x03FFFFFF) << 2)
                if t in syms and t != ea:
                    calls.append(t)
            elif op == 15:
                hi[rt] = imm << 16
            elif op == 9 and rs in hi:
                target = (hi[rs] + (imm - 0x10000 if imm & 0x8000 else imm)) & 0xFFFFFFFF
                drefs.append(target)
                s = read_str(target)
                if s:
                    strs.append(s)
        funcs.append({"ea": ea, "name": syms[ea], "seg": ".text", "words": words,
                      "calls": calls, "strs": strs, "drefs": drefs})
    json.dump(funcs, open(sys.argv[3], "w"))
    print(len(funcs), "functions,", sum(1 for f in funcs if f["strs"]), "with strings")


if __name__ == "__main__":
    main()
