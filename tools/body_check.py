"""Checks on a body find_body.py found: is it the same function as the Raidou one, and does it compile in its unit.

Identity compares retail code on both sides: instruction count, call targets (by Raidou name, SMT3 alias or HD
name) and the opcode sequence. Compiling splices the body into the Raidou unit and runs check_unit.py on it.
"""
from __future__ import annotations

import difflib
import re
import subprocess
import tempfile
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
# Images of the trees find_body reads; each is byte-identical to its retail executable.
TREE_IMAGES = {"smt3": {"smt3": "build/smt3/SLUS_209.11.elf"},
               "dds": {"dds1": "build/dds1/SLUS_209.74.elf", "dds2": "build/dds2/SLUS_211.52.elf"}}
SYMROW = re.compile(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;.*type:func(?:.*size:0x([0-9A-Fa-f]+))?")
SAME_FUNCTION = 0.6

# Compiler messages, in the order they are tried, and the cause each one names. A cause of None is a message
# that follows from an earlier one and is not counted.
CAUSES = [
    (None, re.compile(r"previous (declaration|definition)|not within (a )?(switch|loop)|for each function")),
    ("signature", re.compile(r"conflicting types|number of arguments doesn't match")),
    ("member", re.compile(r"has no member named")),
    ("undeclared", re.compile(r"undeclared|implicit declaration")),
    ("arity", re.compile(r"too (few|many) arguments")),
    ("type", re.compile(r"unknown type|syntax error before|parse error|storage size|incomplete type")),
    ("conflict", re.compile(r"redefinition|redeclaration")),
    ("mismatch", re.compile(r"incompatible|type mismatch|invalid (operands|type argument)|makes (pointer|integer)|"
                            r"invalid lvalue|not a (structure|union)|subscripted value|lacks a cast")),
]


@dataclass
class Profile:
    """The opcodes of one function and the names its jal instructions call."""
    ops: list[str]
    calls: list[set[str]] = field(default_factory=list)


def profile(code: bytes, vram: int, names_at) -> Profile:
    import rabbitizer
    ops, calls = [], []
    for i in range(0, len(code) - len(code) % 4, 4):
        inst = rabbitizer.Instruction(int.from_bytes(code[i:i + 4], "little"), vram + i)
        ops.append(inst.getOpcodeName())
        if inst.isJumpWithAddress() and ops[-1] == "jal":
            calls.append(names_at(inst.getInstrIndexAsVram()))
    return Profile(ops, calls)


def call_score(mine: list[set[str]], theirs: list[set[str]]) -> float:
    """Share of calls whose target names agree, in order; 1 when neither side calls anything."""
    if not mine and not theirs:
        return 1.0
    a = [min(s) if s else "?" for s in mine]
    b = []
    for names in theirs:
        hit = next((min(s) for s in mine if s & names), None)
        b.append(hit if hit else "!" + min(names or {"?"}))
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    return 2 * sum(m.size for m in sm.get_matching_blocks()) / (len(a) + len(b))


@dataclass
class Identity:
    words: tuple[int, int]
    calls: float
    shape: float

    @property
    def confidence(self) -> float:
        lo, hi = sorted(self.words)
        size = lo / hi if hi else 0.0
        return round((size + self.calls + self.shape) / 3, 2)

    def text(self) -> str:
        return (f"{self.confidence:.2f} (words {self.words[0]}/{self.words[1]}, calls {self.calls:.2f}, "
                f"shape {self.shape:.2f})")


def identity(mine: Profile, theirs: Profile) -> Identity:
    shape = difflib.SequenceMatcher(None, mine.ops, theirs.ops, autojunk=False).ratio()
    return Identity((len(mine.ops), len(theirs.ops)), call_score(mine.calls, theirs.calls), shape)


class Image:
    """A linked ELF32: its bytes, function symbols by name and names by address."""

    def __init__(self, path: Path, symbol_text: str = ""):
        from elftools.elf.elffile import ELFFile
        self.data = path.read_bytes()
        self.segs, self.funcs, self.names = [], {}, {}
        with path.open("rb") as f:
            elf = ELFFile(f)
            for seg in elf.iter_segments():
                if seg["p_type"] == "PT_LOAD":
                    self.segs.append((seg["p_vaddr"], seg["p_offset"], seg["p_filesz"]))
            symtab = elf.get_section_by_name(".symtab")
            for s in symtab.iter_symbols() if symtab else ():
                if s["st_info"]["type"] == "STT_FUNC" or (s.name and s["st_shndx"] != "SHN_UNDEF"
                                                          and s["st_info"]["type"] == "STT_NOTYPE"):
                    if s.name and not s.name.startswith((".", "$")):
                        self.names.setdefault(s["st_value"], set()).add(s.name)
                        if s["st_info"]["type"] == "STT_FUNC":
                            self.funcs[s.name] = (s["st_value"], s["st_size"])
        # symbol_addrs.txt at the revision the bodies come from names what the (maybe older) link does not.
        sizes = {a: s for a, s in self.funcs.values() if s}
        for m in map(SYMROW.match, symbol_text.splitlines()):
            if m:
                addr = int(m.group(2), 16)
                self.funcs[m.group(1)] = (addr, int(m.group(3), 16) if m.group(3) else sizes.get(addr, 0))
                self.names.setdefault(addr, set()).add(m.group(1))
        self.starts = sorted(self.names)

    def code(self, addr: int, size: int) -> bytes:
        for vaddr, off, n in self.segs:
            if vaddr <= addr < vaddr + n:
                return self.data[off + addr - vaddr:off + addr - vaddr + size]
        return b""

    def size_of(self, name: str) -> int:
        addr, size = self.funcs[name]
        if size:
            return size
        import bisect
        i = bisect.bisect_right(self.starts, addr)
        return (self.starts[i] - addr) if i < len(self.starts) else 0

    def profile(self, name: str) -> Profile | None:
        if name not in self.funcs:
            return None
        addr = self.funcs[name][0]
        return profile(self.code(addr, self.size_of(name)), addr, lambda a: self.names.get(a, set()))


def tree_images(tree: str, repo: Path, rev: str) -> dict[str, Image]:
    """Version -> Image for each built version of a tree, named by its symbol_addrs.txt at rev."""
    out = {}
    for v, p in TREE_IMAGES.get(tree, {}).items():
        if (repo / p).exists():
            r = subprocess.run(["git", "-C", str(repo), "show", f"{rev}:config/{v}/symbol_addrs.txt"],
                               capture_output=True, text=True)
            out[v] = Image(repo / p, r.stdout if r.returncode == 0 else "")
    return out


def classify_errors(stderr: str) -> dict[str, list[str]]:
    """Compiler error lines grouped by cause (signature, member, undeclared, arity, type, conflict, mismatch, other)."""
    out: dict[str, list[str]] = {}
    for line in stderr.splitlines():
        if ": warning:" in line or not re.search(r":\d+: ", line):
            continue
        msg = line.split(": ", 1)[-1].strip()
        if msg.startswith("(Each undeclared") or msg.startswith("for each function") or msg.startswith("In function"):
            continue
        cause = next((c for c, pat in CAUSES if pat.search(msg)), "other")
        if cause:
            out.setdefault(cause, []).append(msg)
    return out


def splice(unit_text: str, name: str, body: str) -> str | None:
    pat = re.compile(rf'^INCLUDE_ASM\([^,]+,\s*"[^"]+",\s*{re.escape(name)}\);[^\n]*$', re.M)
    if not pat.search(unit_text):
        return None
    return pat.sub(lambda _: body.rstrip("\n"), unit_text, count=1)


@dataclass
class Tried:
    result: str                      # ok, diff N, fail, no-unit, no-asm-line
    errors: dict[str, list[str]] = field(default_factory=dict)

    def missing(self) -> set[str]:
        """Names the unit lacks: undeclared identifiers, members and types."""
        out = set()
        for cause in ("undeclared", "member", "type"):
            for msg in self.errors.get(cause, ()):
                out.update(re.findall(r"`(\w+)'", msg) or [msg])
        return out

    def light(self) -> bool:
        """Only the signature, callee arity and at most one global differ: a mechanical adaptation."""
        if set(self.errors) - {"signature", "arity", "undeclared"}:
            return False
        return len(self.missing()) <= 1

    def text(self) -> str:
        if not self.errors:
            return self.result
        return self.result + " (" + ", ".join(f"{c} {len(v)}" for c, v in sorted(self.errors.items())) + ")"


def try_compile(root: Path, python: str, unit: str, name: str, body: str, version: str) -> Tried:
    """Splice body into the unit's .c in place of its INCLUDE_ASM line and run check_unit.py on that function."""
    path = root / f"{unit}.c"
    if not unit.startswith("src/") or not path.exists():
        return Tried("no-unit")
    text = splice(path.read_text(), name, body)
    if text is None:
        return Tried("no-asm-line")
    return check_source(root, python, path, text, name, version)


def check_source(root: Path, python: str, path: Path, text: str, name: str, version: str) -> Tried:
    """check_unit.py on one function of text compiled as the unit at path (which may not exist yet)."""
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / path.name
        src.write_text(text)
        r = subprocess.run([python, str(root / "tools/check_unit.py"), str(path), "--version", version,
                            "--source", str(src), "--func", name], capture_output=True, text=True, cwd=root)
    if "compile failed" in r.stderr:
        return Tried("fail", classify_errors(r.stderr))
    for line in r.stdout.splitlines():
        if line.startswith(("OK ", "OK\t")) and f" {name} " in line + " ":
            return Tried("ok")
        if line.startswith("DIFF ") and f" {name} " in line:
            m = re.search(r"(\d+) of (\d+) words differ", line)
            return Tried(f"diff {m.group(1)}/{m.group(2)}" if m else "diff")
        if line.startswith(("OVER ", "CONTEXT ")) and f" {name}" in line:
            return Tried("diff")
    return Tried("other")


SIZE_RANGE = (0.5, 2.0)
DECL = re.compile(r"^([^#\n;{}()]*?)\b(\w+)\s*\(([^;{}]*)\)\s*;", re.M)


def size_ratio(ident: Identity) -> float:
    """The hit's word count over the Raidou function's."""
    mine, theirs = ident.words
    return theirs / mine if mine else 0.0


def size_ok(ident: Identity | None) -> bool:
    return ident is None or SIZE_RANGE[0] <= size_ratio(ident) <= SIZE_RANGE[1]


def split_params(params: str) -> list[str] | None:
    """Top-level parameters; None for a variadic or unprototyped list."""
    out, depth, cur = [], 0, ""
    for c in params:
        depth += (c == "(") - (c == ")")
        if c == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += c
    out.append(cur.strip())
    if "..." in out:
        return None
    return [] if out in ([""], ["void"]) else out


def prototypes(root: Path) -> dict[str, tuple[str, str]]:
    """Function name -> (return part, parameter list) of its first declaration in include/."""
    out: dict[str, tuple[str, str]] = {}
    for path in sorted((root / "include").rglob("*.h")):
        for m in DECL.finditer(path.read_text(errors="replace")):
            if m.group(1).strip() and not m.group(1).lstrip().startswith(("typedef", "return")):
                out.setdefault(m.group(2), (m.group(1), m.group(3)))
    return out


def signature(body: str, name: str) -> tuple[str, str] | None:
    """(return part, parameter list) of a definition."""
    m = re.search(rf"^([^{{;]*?)\b{re.escape(name)}\s*\(([^{{;]*)\)\s*\{{", body, re.M | re.S)
    return (m.group(1), m.group(2)) if m else None


def returns_void(ret: str) -> bool:
    return re.search(r"\bvoid\b", ret) is not None and "*" not in ret


def proto_differs(proto: tuple[str, str] | None, sig: tuple[str, str] | None) -> bool:
    """Parameter count or void return differ between the Raidou prototype and the hit's definition."""
    if proto is None or sig is None:
        return False
    a, b = split_params(proto[1]), split_params(sig[1])
    if a is not None and b is not None and len(a) != len(b):
        return True
    return returns_void(proto[0]) != returns_void(sig[0])
