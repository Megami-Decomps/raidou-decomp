#!/usr/bin/env python3
"""List smt3_symbols.txt entries whose Raidou address the evidence does not support.

    python3 tools/xgame/audit_smt3_symbols.py [--smt3 DIR] [-o OUT.tsv] [--all]
"""
import argparse
import bisect
import difflib
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from retail_elf import RETAIL, VERSIONS, load_segments, va_to_off  # noqa: E402

ROW = re.compile(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;(.*)$")
NM = ROOT / "tools/bin/mips-ps2-decompals-nm"


def read_symbols(path):
    """{name: (addr, is_func, size or None)}, first row wins."""
    out = {}
    for line in path.read_text().splitlines():
        if m := ROW.match(line):
            size = re.search(r"\bsize:(0x[0-9A-Fa-f]+|\d+)", m.group(3))
            out.setdefault(m.group(1), (int(m.group(2), 16), "type:func" in m.group(3),
                                        int(size.group(1), 0) if size else None))
    return out


class Image:
    def __init__(self, elf, syms):
        self.data = elf.read_bytes()
        self.segs = load_segments(self.data)
        self.syms = syms
        self.starts = sorted({a for a, f, _ in syms.values() if f})
        self.objects = sorted((a, s, n) for n, (a, f, s) in syms.items() if not f and s)

    def words(self, addr, size):
        off = va_to_off(self.segs, addr)
        return [int.from_bytes(self.data[off + i:off + i + 4], "little") for i in range(0, size, 4)]

    def func_size(self, addr, size):
        if size:
            return size
        i = bisect.bisect_right(self.starts, addr)
        return min((self.starts[i] - addr) if i < len(self.starts) else 0x400, 0x4000)

    def containing(self, addr):
        """(start, size, name) of the sized data object that holds addr past its start."""
        i = bisect.bisect_right(self.objects, (addr, 1 << 40, "")) - 1
        if i >= 0:
            a, s, n = self.objects[i]
            if a < addr < a + s:
                return a, s, n
        return None


def shape(word):
    """Instruction with link-time fields masked: jump targets and 16-bit immediates."""
    op = word >> 26
    if op in (2, 3):
        return op << 26
    if op >= 4:
        return word & 0xFFFF0000
    return word


def similarity(a, b):
    return difflib.SequenceMatcher(None, [shape(w) for w in a], [shape(w) for w in b], autojunk=False).ratio()


def referenced_names(version, names):
    """Names that objects of the matching build reference; the ELF checksum covers their address."""
    out = set()
    objs = [str(p) for p in (ROOT / "build" / version / "src").rglob("*.o")]
    for i in range(0, len(objs), 400):
        r = subprocess.run([str(NM), "-u", *objs[i:i + 400]], capture_output=True, text=True)
        out |= {line.split()[-1] for line in r.stdout.splitlines() if line.strip()} & names
    return out


def elf_symbols(version, serial):
    r = subprocess.run([str(NM), str(ROOT / "build" / version / f"{serial}.elf")], capture_output=True, text=True)
    return {p[2]: int(p[0], 16) for p in (line.split() for line in r.stdout.splitlines()) if len(p) == 3}


def facts(smt3_img, img, entries, referenced, linked):
    """{name: (addr, smt3 entry, shape score or None, problems, proven, differs)}"""
    out = {}
    for name, (addr, _, _) in entries.items():
        problems, notes, differs, score = [], [], False, None
        native = img.syms.get(name)
        if native and native[0] != addr:
            problems.append(f"symbol_addrs has {name} at 0x{native[0]:08X}")
        if name in linked and linked[name] != addr:
            problems.append(f"the build links {name} at 0x{linked[name]:08X}")
        theirs = smt3_img.syms.get(name)
        if theirs is None:
            notes.append("not in SMT3 symbol_addrs")
        elif theirs[1]:
            if addr not in img.starts:
                notes.append("SMT3 function, Raidou address is no function start")
            mine = img.words(addr, img.func_size(addr, native[2] if native else None))
            ref = smt3_img.words(theirs[0], smt3_img.func_size(theirs[0], theirs[2]))
            score = similarity(mine, ref)
            if score < 0.5:
                differs = True
                notes.append(f"code shape {score:.2f} of SMT3's ({len(ref) * 4:#x} vs {len(mine) * 4:#x} bytes)")
        else:
            if addr in img.starts:
                differs = True
                notes.append("SMT3 data, Raidou address is a function start")
            if inside := img.containing(addr):
                notes.append(f"inside {inside[2]} 0x{inside[0]:08X}+0x{addr - inside[0]:X}")
        names_here = sorted(n for n, (a, _, _) in img.syms.items() if a == addr)
        if names_here:
            notes.append("Raidou " + ",".join(names_here[:3]))
        out[name] = (addr, theirs, score, problems, notes, name in referenced, differs, names_here)
    return out


def verdicts(version, mine, other):
    rows = []
    for name, (addr, theirs, score, problems, notes, proven, differs, here) in sorted(
            mine.items(), key=lambda kv: kv[1][0]):
        twin = other.get(name)
        twin_proven = bool(twin and twin[5] and not twin[3] and set(here) & set(twin[7]))
        if problems:
            verdict = "wrong"
        elif not proven and not twin_proven and differs and name not in here:
            verdict = "unverified"
        elif differs:
            verdict = "other-object"
        else:
            verdict = "ok"
        why = problems + notes + ["used by matching C" if proven
                                  else "twin of a used entry" if twin_proven else "no matching user"]
        rows.append((version, name, f"0x{addr:08X}", f"0x{theirs[0]:08X}" if theirs else "",
                     "func" if theirs and theirs[1] else "data" if theirs else "?",
                     f"{score:.2f}" if score is not None else "", verdict, "; ".join(why)))
    return rows


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--smt3", type=Path, default=Path.home() / "projects/smt3-decomp")
    ap.add_argument("-o", "--out", type=Path)
    ap.add_argument("--all", action="store_true", help="list every entry, not only the flagged ones")
    args = ap.parse_args()
    smt3 = Image(args.smt3 / "orig/smt3/SLUS_209.11", read_symbols(args.smt3 / "config/smt3/symbol_addrs.txt"))
    games = {}
    for v in ("raidou1", "raidou2"):
        entries = read_symbols(ROOT / "config" / v / "smt3_symbols.txt")
        img = Image(ROOT / RETAIL[v], read_symbols(ROOT / "config" / v / "symbol_addrs.txt"))
        games[v] = facts(smt3, img, entries, referenced_names(v, set(entries)),
                         elf_symbols(v, VERSIONS[v]["serial"]))
    rows = []
    for v in games:
        rows += verdicts(v, games[v], games["raidou2" if v == "raidou1" else "raidou1"])
    flagged = [r for r in rows if r[6] != "ok"]
    lines = ["version\tname\traidou_addr\tsmt3_addr\tkind\tshape\tverdict\tnotes"]
    lines += ["\t".join(r) for r in (rows if args.all else flagged)]
    text = "\n".join(lines) + "\n"
    if args.out:
        args.out.write_text(text)
    else:
        sys.stdout.write(text)
    counts = {}
    for r in rows:
        counts[r[6]] = counts.get(r[6], 0) + 1
    print(f"{len(rows)} entries: " + ", ".join(f"{n} {k}" for k, n in sorted(counts.items())), file=sys.stderr)


if __name__ == "__main__":
    main()
