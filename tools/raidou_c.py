#!/usr/bin/env python3
"""Carry smt3-decomp C bodies into Raidou units where the code is identical.

    python3 tools/raidou_c.py pairs|port|verify   (see docs/shared-c.md)
"""

from __future__ import annotations

import argparse
import bisect
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from check_unit import drop_nonmatching  # noqa: E402
from retail_elf import VERSIONS  # noqa: E402
from units import SRC, shared_sources, unit_name, unit_sources  # noqa: E402

OUT = ROOT / "build" / "raidou_c"
# Read only: the smt3-port checkout follows smt3-decomp upstream.
SMT3 = Path.home() / "projects/smt3-decomp"
# Read only. A DDS source is `dds1/<path>` or `dds2/<path>` under its src/; SMT3 ones are relative to src/smt3.
DDS = Path.home() / "projects/dds-decomp"
DDS_GAMES = ("dds1", "dds2")

INSN = re.compile(r"^\s*/\* [0-9A-Fa-f]+ ([0-9A-Fa-f]+) ([0-9A-Fa-f]{8}) \*/\s+(.*?)\s*$")
GLABEL = re.compile(r"^\s*glabel\s+(\S+)")
INCLUDE_ASM = re.compile(r'INCLUDE_ASM\(\s*[^,]+,\s*"([^"]+)"\s*,\s*(\w+)\s*\)')
INCLUDE_BODY = re.compile(r'^#include "shared/(\S+)/(\w+)\.c"$', re.M)
RELOC = re.compile(r"%(hi|lo|gp_rel)\(([^)]+)\)")
SYMROW = re.compile(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;(.*)$")


def parse_asm(path):
    """glabel name -> (address, masked words, slots); a slot is (kind, operand text)."""
    out, name, address, words, slots = {}, None, None, [], []
    for line in path.read_text(errors="replace").splitlines():
        label = GLABEL.match(line)
        if label:
            if name and words:
                out[name] = (address, words, slots)
            name, address, words, slots = label.group(1), None, [], []
            continue
        insn = INSN.match(line)
        if not insn or name is None:
            continue
        if address is None:
            address = int(insn.group(1), 16)
        word = int.from_bytes(bytes.fromhex(insn.group(2)), "little")
        text = insn.group(3)
        reloc = RELOC.search(text)
        if word >> 26 in (2, 3):
            words.append(word & 0xFC000000)
            slots.append(("call", text.split()[-1]))
        elif reloc:
            words.append(word & 0xFFFF0000)
            slots.append((reloc.group(1), reloc.group(2)))
        else:
            words.append(word)
    if name and words:
        out[name] = (address, words, slots)
    return out


def code_view(text):
    """Comments and literals blanked, offsets kept."""
    return re.sub(
        r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
        lambda m: re.sub(r"[^\n]", " ", m.group()), text, flags=re.S)


def definitions(text):
    """Function name -> (start, end) offsets of each definition in text."""
    code = code_view(text)
    found = {}
    nesting, level = [], 0
    for ch in code:
        nesting.append(level)
        level += (ch == "{") - (ch == "}")
    candidate = re.compile(r"^[\w \t*\n]*?(\w+)[ \t]*(?:\n[ \t]*)?\(", re.M)
    for m in candidate.finditer(code):
        name = m.group(1)
        if nesting[m.start(1)]:
            continue  # a nested function belongs to its parent's body
        if not code[m.start():m.start(1)].strip() or name in ("if", "while", "for", "switch", "return"):
            continue
        depth, i = 0, m.end() - 1
        while i < len(code):
            if code[i] == "(":
                depth += 1
            elif code[i] == ")":
                depth -= 1
                if depth == 0:
                    break
            i += 1
        j = i + 1
        while j < len(code) and code[j] in " \t\r\n":
            j += 1
        if j >= len(code) or code[j] != "{":
            continue
        depth, k = 0, j
        while k < len(code):
            if code[k] == "{":
                depth += 1
            elif code[k] == "}":
                depth -= 1
                if depth == 0:
                    start = m.start() + len(m.group(0)) - len(m.group(0).lstrip())
                    found.setdefault(name, (start, k + 1))
                    break
            k += 1
    return found


def repo_symbols(root, game="smt3"):
    out = {}
    for line in (root / "config" / game / "symbol_addrs.txt").read_text().splitlines():
        if m := SYMROW.match(line):
            out.setdefault(m.group(1), int(m.group(2), 16))
    return out


def smt3_symbols(smt3):
    return repo_symbols(smt3)


def c_functions(root, game):
    """Address -> (name, source path) for every function a repo's normal build compiles from C."""
    syms = repo_symbols(root, game)
    out = {}
    for src in sorted((root / "src" / game).rglob("*.c")):
        text = drop_nonmatching(src.read_text(errors="replace"))
        if game in DDS_GAMES:
            text = resolve_version(text, game)
        for name in definitions(text):
            addr = syms.get(name)
            if addr is None and (m := re.fullmatch(r"func_([0-9A-F]{8})", name)):
                addr = int(m.group(1), 16)
            if addr is not None:
                out.setdefault(addr, (name, str(src.relative_to(root))))
    return out


def smt3_c_functions(smt3):
    return c_functions(smt3, "smt3")


def split_bodies(root, game):
    """Address -> (masked words, slots), from a repo's split assembly."""
    out = {}
    for sub in ("matchings", "nonmatchings"):
        for path in (root / "asm" / game / sub).rglob("*.s"):
            for _, (addr, words, slots) in parse_asm(path).items():
                if addr is not None:
                    out.setdefault(addr, (words, slots))
    return out


def smt3_bodies(smt3):
    return split_bodies(smt3, "smt3")


VERSION_IF = re.compile(r"^\s*#\s*(ifdef|ifndef|if|elif|else|endif)\b(.*)$")


def version_value(kind, arg, game):
    """True/False for a conditional on VERSION_DDS1/2 alone, else None."""
    arg = re.sub(r"/\*.*?\*/|//.*", "", arg).strip()
    if kind in ("ifdef", "ifndef"):
        if not re.fullmatch(r"VERSION_DDS[12]", arg):
            return None
        return (arg == f"VERSION_{game.upper()}") == (kind == "ifdef")
    expr = re.sub(r"defined\s*\(\s*VERSION_(DDS[12])\s*\)|(?:defined\s+)?\bVERSION_(DDS[12])\b",
                  lambda m: "1" if (m.group(1) or m.group(2)).lower() == game else "0", arg)
    if expr == arg or not re.fullmatch(r"[01\s()!&|]+", expr):
        return None
    return bool(eval(expr.replace("||", " or ").replace("&&", " and ").replace("!", " not ")))


def resolve_version(text, game):
    """text with conditionals on VERSION_DDS1/2 decided for game; others kept."""
    out, stack, skipping = [], [], False
    for line in text.split("\n"):
        d = VERSION_IF.match(line)
        if d:
            kind, arg = d.groups()
            if kind in ("ifdef", "ifndef", "if"):
                value = version_value(kind, arg, game)
                stack.append([skipping, value, bool(value)])
                if value is None:
                    if not skipping:
                        out.append(line)
                else:
                    skipping = skipping or not value
                continue
            if stack:
                parent, value, taken = stack[-1]
                if value is not None:
                    if kind == "endif":
                        stack.pop()
                        skipping = parent
                    elif kind == "else":
                        skipping = parent or taken
                    else:
                        branch = version_value("if", arg, game)
                        if branch is None:
                            sys.exit(f"#elif on more than VERSION_DDS after a VERSION_DDS conditional: {line}")
                        skipping = parent or taken or not branch
                        stack[-1][2] = taken or branch
                    continue
                if kind == "endif":
                    stack.pop()
        if not skipping:
            out.append(line)
    return "\n".join(out)


def is_dds(rel):
    return rel.split("/", 1)[0] in DDS_GAMES


def source_file(smt3, rel):
    return DDS / "src" / f"{rel}.c" if is_dds(rel) else smt3 / "src/smt3" / f"{rel}.c"


def name_sources(version):
    """Raidou address -> SMT3 address, from the name carrying step."""
    out = {}
    path = ROOT / "config" / version / "name_sources.txt"
    for line in path.read_text().splitlines():
        if line.startswith("#"):
            continue
        cols = line.split("\t")
        if len(cols) < 4:
            continue
        m = re.search(r"SLUS_209\.11 0x([0-9A-Fa-f]+)", cols[3])
        if m:
            out[int(cols[0], 16)] = int(m.group(1), 16)
    return out


def function_asm(version, unit, fn):
    """Split assembly of one function; splat moves it to matchings/ once the unit has C."""
    for sub in ("nonmatchings", "matchings"):
        path = ROOT / "asm" / version / sub / unit / f"{fn}.s"
        if path.exists():
            return path
    return None


def raidou_asm_functions(version):
    """Each INCLUDE_ASM line or C definition of a unit: (src, unit, name, address, words, slots)."""
    rows = []
    for unit, src in unit_sources(version).items():
        text = src.read_text(errors="replace")
        lines = INCLUDE_ASM.findall(text) + [(unit, fn) for fn in definitions(text)]
        for folder, fn in lines:
            asm = function_asm(version, folder, fn)
            if asm is None:
                continue
            body = parse_asm(asm).get(fn)
            if body:
                rows.append((str(src.relative_to(ROOT)), folder, fn, *body))
    return rows


def cmd_pairs(args):
    smt3 = args.smt3.expanduser().resolve()
    c_funcs = smt3_c_functions(smt3)
    bodies = smt3_bodies(smt3)
    OUT.mkdir(parents=True, exist_ok=True)
    lines = ["version\tsrc\tfunction\taddress\tsmt3_address\tsmt3_name\tsmt3_src\twords\tstatus"]
    for version in VERSIONS:
        sources = name_sources(version)
        counts = defaultdict(int)
        for src, unit, fn, addr, words, slots in raidou_asm_functions(version):
            smt3_addr = sources.get(addr)
            if smt3_addr is None:
                status, sname, ssrc = "no-partner", "", ""
            elif smt3_addr not in c_funcs:
                status, sname, ssrc = "smt3-asm", "", ""
            else:
                sname, ssrc = c_funcs[smt3_addr]
                theirs = bodies.get(smt3_addr)
                if theirs is None:
                    status = "no-smt3-asm"
                elif theirs[0] != words:
                    status = "differs"
                elif [k for k, _ in theirs[1]] != [k for k, _ in slots]:
                    status = "slots-differ"
                else:
                    status = "identical"
            counts[status] += 1
            lines.append(f"{version}\t{src}\t{fn}\t0x{addr:08X}\t"
                         f"{'' if smt3_addr is None else f'0x{smt3_addr:08X}'}\t{sname}\t{ssrc}\t{len(words)}\t{status}")
        print(version, dict(sorted(counts.items())))
    (OUT / "pairs.tsv").write_text("\n".join(lines) + "\n")


KNR_HEADER = re.compile(r"^[A-Za-z_][\w \t\*]*\b\w+\s*\(\s*\w+(\s*,\s*\w+)*\s*\)\s*$")
STATIC = re.compile(r"\bstatic\s+")
# Data layout directives: the data stays where the unit's split puts it.
DROP = re.compile(r"^\s*(INCLUDE_ASM|INCLUDE_RODATA|INCLUDE_SDATA|TEXT_ALIGN|SDATA_PAD|SDATA_RESUME|__asm__|asm)\b")
# SYMBOL_AT(type, name, base, offset): an SMT3 name inside other data; here an extern
# that smt3_symbols.txt places.
SYMBOL_AT = re.compile(r"^\s*SYMBOL_AT(_REGION)?\(\s*([^,]+?)\s*,\s*(\w+)\s*,[^;]*;", re.M)


def blocks(text):
    """Top-level items with the comment lines directly above them; preprocessor
    lines (with their continuations) are items of their own."""
    out, cur, depth, directive = [], [], 0, False
    for line, code in zip(text.split("\n"), code_view(text).split("\n")):
        code = code.strip()
        if directive:
            out[-1] += "\n" + line
            directive = line.rstrip().endswith("\\")
            continue
        if depth == 0 and not line.strip():
            if cur:
                out.append("\n".join(cur))
                cur = []
            continue
        if depth == 0 and code.startswith("#"):
            if cur:
                out.append("\n".join(cur))
                cur = []
            out.append(line)
            directive = line.rstrip().endswith("\\")
            continue
        cur.append(line)
        depth += code.count("{") - code.count("}")
        knr = depth == 0 and code.endswith(";") and "{" not in "".join(cur) \
            and KNR_HEADER.match(next((c for c in cur if c.strip() and not c.strip().startswith(("/*", "*", "//"))), "")) is not None
        if depth == 0 and code.endswith((";", "}")) and not knr:
            out.append("\n".join(cur))
            cur = []
    if cur:
        out.append("\n".join(cur))
    return out


def strip_static(head):
    """Drop `static` from a declaration unless it is inline: Raidou's asm units call
    these functions and read these objects by name."""
    return head if re.search(r"\binline\b|__inline", head) else STATIC.sub("", head)


def string_size(init):
    """sizeof a char array initialised by one string literal, else None."""
    m = re.fullmatch(r'\s*((?:"(?:\\.|[^"\\])*"\s*)+)', init)
    if not m:
        return None
    raw = "".join(re.findall(r'"((?:\\.|[^"\\])*)"', m.group(1)))
    try:
        return len(raw.encode("latin-1").decode("unicode_escape")) + 1
    except UnicodeDecodeError:
        return None


def top_level(code, char):
    """Offset of the first `char` outside brackets in code, or -1."""
    depth = 0
    for i, ch in enumerate(code):
        if ch in "([{":
            if ch == char and depth == 0:
                return i
            depth += 1
        elif ch in ")]}":
            depth -= 1
        elif ch == char and depth == 0:
            return i
    return -1


def blank_attributes(cv):
    """cv with each __attribute__((...)) blanked, offsets kept."""
    out = cv
    for m in re.finditer(r"__attribute__\s*\(", cv):
        depth, i = 0, m.end() - 1
        while i < len(cv):
            depth += {"(": 1, ")": -1}.get(cv[i], 0)
            if depth == 0:
                break
            i += 1
        out = out[:m.start()] + " " * (i + 1 - m.start()) + out[i + 1:]
    return out


def declaration(item):
    """One top-level item as a header declaration, or None to drop it."""
    cv = blank_attributes(code_view(item))
    code = cv.strip()
    if not code or DROP.match(code):
        return None
    if m := SYMBOL_AT.match(code):
        return f"extern {m.group(2)} {m.group(3)}{'[]' if m.group(1) else ''};"
    if code.startswith("#"):
        return item if re.match(r"#\s*(include|define|if|ifdef|ifndef|elif|else|endif)\b", code) else None
    brace, paren, eq = top_level(cv, "{"), top_level(cv, "("), top_level(cv, "=")
    if code.startswith("typedef") or re.fullmatch(r"(struct|union|enum)\s+\w+\s*;", code) \
            or (re.match(r"(struct|union|enum)\b", code) and code.rstrip(";").rstrip().endswith("}")
                and not 0 <= paren < brace):
        return item
    if paren >= 0 and (eq < 0 or eq > paren) and code.endswith("}") and brace > paren:
        # A function definition: inline helpers stay whole, the rest become prototypes.
        if re.search(r"\binline\b|__inline", cv[:brace]):
            return item
        head = item[:brace].rstrip()
        if KNR_HEADER.match(next(c for c in cv[:brace].split("\n") if "(" in c).strip()) and ";" in cv[:brace]:
            head = item[:paren] + "()"  # old-style definition: no prototype
        return strip_static(head) + ";"
    if not code.endswith(";"):
        return None
    if paren >= 0 and (eq < 0 or eq > paren) and brace < 0 \
            and not re.search(r"\(\s*\*\s*\w+\s*(\[[^\]]*\]\s*)*\)\s*\(", code):
        first = next(c for c in cv.split("\n") if "(" in c).strip()
        if KNR_HEADER.match(first) and first != code:
            return strip_static(item[:paren]) + "();"  # head of an old-style definition
        return strip_static(item)  # a prototype; a function pointer object is data
    # Data: an extern declaration without its initializer.
    end = cv.rstrip().rfind(";")
    body = item[:eq if eq >= 0 else end].rstrip()
    init = item[eq + 1:end] if eq >= 0 else None
    if top_level(code_view(body), ",") >= 0 and init is not None:
        return None  # several declarators, initialised
    body = STATIC.sub("", body)
    if init is not None and re.search(r"\[\s*\]\s*$", body):
        size = string_size(init)
        if size is not None:
            body = re.sub(r"\[\s*\]\s*$", f"[{size}]", body)
    if not re.match(r"(\s*/\*.*?\*/\s*|\s*//[^\n]*\n)*\s*(extern|EXTERN_LATE_CONST)\b", body, re.S):
        lead = len(body) - len(code_view(body).lstrip()) if code_view(body).strip() else 0
        body = body[:lead] + "extern " + body[lead:]
    return body + ";"


def shared_rel(src):
    """src/smt3/<x>.c -> <x>; src/ddsN/<x>.c -> ddsN/<x>."""
    path = Path(src).with_suffix("")
    return (path.relative_to("src/smt3") if path.parts[1] == "smt3" else path.relative_to("src")).as_posix()


_SMT3_TEXT = {}
_DDS_TYPES = []


def dds_rename(text):
    """dds_<name> for each type config/dds_types.txt lists: dds-decomp defines it with
    another layout than SMT3's type of that name, and the DDS code needs its own."""
    if not _DDS_TYPES:
        lines = (ROOT / "config/dds_types.txt").read_text().splitlines()
        names = [ln.split("#")[0].strip() for ln in lines if ln.split("#")[0].strip()]
        _DDS_TYPES.append(re.compile(r"\b(" + "|".join(map(re.escape, names)) + r")\b") if names else None)
    return _DDS_TYPES[0].sub(r"dds_\1", text) if _DDS_TYPES[0] else text


def smt3_text(smt3, rel):
    if rel not in _SMT3_TEXT:
        text = drop_nonmatching(source_file(smt3, rel).read_text(errors="replace"))
        _SMT3_TEXT[rel] = dds_rename(resolve_version(text, rel.split("/", 1)[0])) if is_dds(rel) else text
    return _SMT3_TEXT[rel]


def decl_begin(rel):
    return f"/* Declarations from dds-decomp src/{rel}.c */" if is_dds(rel) \
        else f"/* Declarations from smt3-decomp src/smt3/{rel}.c */"


def decl_end(rel):
    return f"/* End of declarations from src/{'' if is_dds(rel) else 'smt3/'}{rel}.c */"


DECL_BLOCK = re.compile(r"^/\* Declarations from (?:smt3-decomp src/smt3/|dds-decomp src/)(\S+)\.c \*/\n.*?"
                        r"^/\* End of declarations from src/(?:smt3/)?\1\.c \*/\n\n?", re.M | re.S)
DDS_INCLUDE = re.compile(r'^(\s*#\s*include\s+)"([^"/]+)"', re.M)


def declared_names(item):
    """(kind, name) for each name a top-level item defines: kind "macro", "tag" (a struct,
    union or enum with a body) or "ident" (typedef, enum constant, function, data)."""
    cv = blank_attributes(code_view(item)).strip()
    if m := re.match(r"#\s*define\s+(\w+)", cv):
        return {("macro", m.group(1))}
    out = set()
    if not cv or cv.startswith("#"):
        return out
    out |= {("tag", m.group(2)) for m in re.finditer(r"\b(struct|union|enum)\s+(\w+)\s*\{", cv)}
    for m in re.finditer(r"\benum\b[^{;]*\{([^}]*)\}", cv):
        out |= {("ident", c.split("=")[0].strip()) for c in m.group(1).split(",") if c.split("=")[0].strip()}
    flat = cv
    while "{" in flat:
        flat = re.sub(r"\{[^{}]*\}", "", flat)
    brace, paren = top_level(cv, "{"), top_level(cv, "(")
    if cv.endswith("}") and 0 <= paren < brace and (m := re.search(r"(\w+)\s*$", cv[:paren])):
        return {("ident", m.group(1))}  # a function definition
    if not flat.endswith(";") or re.fullmatch(r"(struct|union|enum)\s+\w+\s*;", cv):
        return out
    # A function pointer declarator, not a parameter: it is the first top-level parenthesis.
    pointer = re.match(r"\(\s*\*\s*(\w+)\s*(\[[^\]]*\]\s*)*\)\s*\(", flat[max(top_level(flat, "("), 0):])
    if flat.startswith("typedef"):
        if pointer:
            return out | {("ident", pointer.group(1))}
        parts = flat[:-1].split(",")
    else:
        eq = top_level(flat, "=")
        head = flat[:eq] if eq >= 0 else flat[:-1]
        paren = top_level(head, "(")
        if pointer:
            return out | {("ident", pointer.group(1))}
        parts = [head[:paren]] if paren >= 0 else [head]
    for part in parts:
        if (m := re.search(r"(\w+)\s*(\[[^\]]*\]\s*)*$", part.strip())) and m.group(1) not in ("struct", "union", "enum"):
            out.add(("ident", m.group(1)))
    return out


def header_guard(text):
    m = re.search(r"^\s*#\s*ifndef\s+(\w+)\s*\n\s*#\s*define\s+\1\b", text, re.M)
    return m.group(1) if m else None


_SMT3_NAMES = []


def smt3_header_names():
    """(kind, name) -> SMT3 header (relative to include/) for each name include/ defines."""
    if not _SMT3_NAMES:
        names = {}
        for h in sorted((ROOT / "include").rglob("*.h")):
            rel = h.relative_to(ROOT / "include").as_posix()
            if rel.startswith("dds/"):
                continue
            text = h.read_text(errors="replace")
            guard = header_guard(text)
            for b in blocks(text):
                for n in declared_names(b):
                    if n != ("macro", guard):
                        names.setdefault(n, rel)
        _SMT3_NAMES.append(names)
    return _SMT3_NAMES[0]


def is_object_decl(item):
    """A function or data declaration or definition (not a type, macro or directive)."""
    cv = code_view(item).strip()
    return bool(cv) and not cv.startswith(("typedef", "#")) and not re.match(r"(struct|union|enum)\b[^(]*\{", cv)


def is_movable(item):
    """A prototype, extern or enum: a declaration a TU can carry in place of a header's."""
    cv = code_view(item).strip()
    return (is_object_decl(item) and "{" not in cv) or bool(re.match(r"(typedef\s+)?enum\b[^;(]*\{", cv))


_SMT3_SRC_NAMES = []


def extern_names(text):
    """Names text declares extern anywhere: a block-scope extern clashes with a
    file-scope declaration just the same."""
    return set(re.findall(r"\bextern\b[^;{}()=]*?\b(\w+)\s*(?:\(|\[|;)", code_view(text)))


def smt3_source_names():
    """Functions and data the SMT3 files carried into Raidou declare at top level."""
    if not _SMT3_SRC_NAMES:
        rels = {shared_rel(r["smt3_src"]) for r in read_pairs(OUT / "pairs.tsv") if r["smt3_src"]}
        names = set()
        for rel in rels:
            if source_file(SMT3, rel).exists():
                text = smt3_text(SMT3, rel)
                for b in blocks(text):
                    if is_object_decl(b) or is_movable(b):
                        names |= {n for k, n in declared_names(b) if k == "ident"}
                names |= extern_names(text)
        _SMT3_SRC_NAMES.append(names)
    return _SMT3_SRC_NAMES[0]


_SMT3_SRC_TYPES = []


def smt3_source_types():
    """(kind, name) of the types and tags the SMT3 files carried into Raidou define; a
    dds-decomp header's definition of one is skipped where SMT3_DEFINES_<name> is set."""
    if not _SMT3_SRC_TYPES:
        rels = {shared_rel(r["smt3_src"]) for r in read_pairs(OUT / "pairs.tsv") if r["smt3_src"]}
        names = set()
        for rel in rels:
            if source_file(SMT3, rel).exists():
                for b in blocks(smt3_text(SMT3, rel)):
                    if not is_object_decl(b):
                        names |= {(k, n) for k, n in declared_names(b) if k != "macro"}
        _SMT3_SRC_TYPES.append(names)
    return _SMT3_SRC_TYPES[0]


def smt3_includes(text, seen=None):
    """SMT3 headers (relative to include/) text includes, directly or through them."""
    seen = set() if seen is None else seen
    for h in re.findall(r'^\s*#\s*include\s+"([^"]+)"', text, re.M):
        if h.startswith("dds/") or h in seen or not (ROOT / "include" / h).exists():
            continue
        seen.add(h)
        smt3_includes((ROOT / "include" / h).read_text(errors="replace"), seen)
    return seen


_DDS_DROPPED = {}


def dds_dropped(game):
    """name -> (header, declaration) for the prototypes, externs and enums dropped from
    include/dds because SMT3 declares a name of theirs; DDS declaration blocks add them
    back where no SMT3 declaration is in the TU."""
    if game not in _DDS_DROPPED:
        out = _DDS_DROPPED[game] = {}
        shared = smt3_source_names() | {n for k, n in smt3_header_names() if k == "ident"}
        for h in sorted((DDS / "include").rglob("*.h")):
            if h.relative_to(DDS / "include").parts[0] == "irx":
                continue  # IOP module headers
            raw = dds_rename(h.read_text(errors="replace"))
            kept, _ = defer_to_smt3(blocks(raw))
            stays = {n for b in kept for n in declared_names(b)}
            for b in blocks(dds_rename(resolve_version(raw, game))):
                if is_movable(b):
                    for k, n in declared_names(b):
                        if k == "ident" and n in shared and (k, n) not in stays:
                            out.setdefault(n, (h.relative_to(DDS / "include").as_posix(), b))
    return _DDS_DROPPED[game]


def defer_to_smt3(items, taken=(), header=True):
    """Drop the items defining a name SMT3 defines: SMT3 is authoritative on shared code.
    A header gives way to SMT3's headers (types, macros) and to any SMT3 declaration of
    its prototypes, externs and enums; a declaration block to the names in taken.
    Returns (kept items, SMT3 headers to include)."""
    smt3 = smt3_header_names()
    shared = smt3_source_names() | {n for k, n in smt3 if k == "ident"} if header else set()
    kept, need = [], set()
    idents = {n for k, n in smt3 if k != "macro"}
    items = list(items)
    words = [set(re.findall(r"\w+", code_view(i))) for i in items]
    for at, item in enumerate(items):
        names = declared_names(item)
        cv = code_view(item)
        if re.match(r"\s*typedef\s+char\s+\w+\s*\[.*\?\s*1\s*:\s*-1\s*\]\s*;", cv, re.S) \
                and set(re.findall(r"\w+", cv)) & idents:
            continue  # a layout check of a type SMT3 owns: the build compares the code instead
        if names & set(taken):
            continue
        # An enum the rest of the header uses (its type or constants) stays with it.
        typed = {n for n in re.findall(r"\benum\s+(\w+)|\}\s*(\w+)\s*;", cv) for n in n if n}
        if re.match(r"\s*(typedef\s+)?enum\b", cv):
            typed |= {n for k, n in names if k == "ident"}  # its constants too
        used = typed and any(typed & w for i, w in enumerate(words) if i != at)
        if is_movable(item) and not used:
            if {n for k, n in names if k == "ident"} & shared:
                continue
            if not header and (hit := {smt3[n] for n in names if n in smt3}):
                need |= hit
                continue
        elif hit := {smt3[n] for n in names if n in smt3}:
            need |= hit
            if alias := enum_alias(item):
                kept.append(alias)
            continue
        kept.append(item)
    return kept, sorted(need)


def enum_alias(item):
    """`typedef int N;` for a typedef'd enum whose constants SMT3 defines under another type name."""
    m = re.search(r"^\s*typedef\s+enum\b[^{]*\{.*\}\s*(\w+)\s*;\s*$", code_view(item), re.S)
    if m and ("ident", m.group(1)) not in smt3_header_names():
        n = m.group(1)
        return f"#ifndef DDS_ENUM_{n}\n#define DDS_ENUM_{n}\ntypedef int {n};\n#endif"
    return None


def decl_block(smt3, rel, unsized=(), taken=(), text_macros=()):
    """Every declaration of one source file (types, prototypes, its data as extern);
    arrays named in unsized are declared without a size (Raidou's differs). A DDS file's
    declarations give way to SMT3's headers and to the names in taken."""
    decls, unprototyped = [], set()
    for d in (declaration(b) for b in blocks(smt3_text(smt3, rel))):
        if d is None:
            continue
        # The file calls `f()` with arguments before defining f(void); a later prototype breaks that.
        if (m := re.search(r"\b(\w+)\s*\(([^()]*)\)\s*;$", code_view(d).strip())) and "{" not in d:
            if not m.group(2).strip():
                unprototyped.add(m.group(1))
            elif m.group(1) in unprototyped:
                continue
        if (name := data_name(d.replace("extern", "", 1))) in unsized:
            d = re.sub(rf"\b{name}\s*\[[^\]]*\]", f"{name}[]", d, count=1)
        decls.append(d)
    if is_dds(rel):
        # dds-decomp's headers live in include/dds/; its headers include each other by bare name.
        macro = f"VERSION_{rel.split('/', 1)[0].upper()}"
        decls, need = defer_to_smt3(decls, taken, header=False)
        used = set(re.findall(r"\w+", code_view(smt3_text(smt3, rel))))
        back, seen = [], {n for d in decls for n in declared_names(d)}
        for n, (h, d) in sorted(dds_dropped(rel.split("/", 1)[0]).items()):
            names = declared_names(d)
            if {x for _, x in names} & used and not names & (set(taken) | seen):
                back.append((h, d))
                seen |= names
        # What SMT3's headers declare comes from them; the rest is dds-decomp's declaration.
        smt3_hdr = smt3_header_names()
        by_header = defaultdict(set)
        for n, h in smt3_hdr.items():
            by_header[h].add(n)
        own, own_text, included = [], [], {n for k, n in taken if k == "include"}
        for h, d in back:
            hit = {smt3_hdr[n] for n in declared_names(d) if n in smt3_hdr}
            if hit and hit <= included:
                continue  # the TU's SMT3 files already include the header
            # An SMT3 header that redeclares what the TU's SMT3 files declare would clash.
            if hit and not any(by_header[x] & set(taken) for x in hit):
                need = sorted(set(need) | hit)
                if alias := enum_alias(d):
                    own_text.append(alias)
            else:
                own.append((h, d))
        # The file's own declarations use these, so they come first.
        decls = [f'#include "dds/{h}"' for h in sorted({h for h, _ in own})] + [d for _, d in own] + own_text + decls
        # An SMT3 header that redeclares what the TU's SMT3 files define locally would clash.
        types = smt3_source_types()
        local = (set(taken) & types) | {("ident", n) for k, n in taken if k == "extern"}
        need = [h for h in need if h in included or not by_header[h] & local]
        guards = sorted({n for k, n in taken if (k, n) in types})
        decls = [f"#ifndef {macro}\n#define {macro} 1\n#endif"] \
            + [f"#define SMT3_DEFINES_{n} 1" for n in guards if f"SMT3_DEFINES_{n}" not in text_macros] \
            + [f'#include "{h}"' for h in need] \
            + [DDS_INCLUDE.sub(r'\1"dds/\2"', d) for d in decls]
    return decl_begin(rel) + "\n" + "\n\n".join(decls) + "\n" + decl_end(rel) + "\n"


_BODY = {}


def body_text(smt3, rel, fn):
    """One SMT3 function definition with the comment directly above it, `static` dropped."""
    if (rel, fn) not in _BODY:
        text = smt3_text(smt3, rel)
        if ("defs", rel) not in _BODY:
            _BODY[("defs", rel)] = definitions(text)
        start, end = _BODY[("defs", rel)][fn]
        cv = code_view(text)
        lead = start
        while lead > 0:
            prev = text.rfind("\n", 0, lead - 1) + 1
            if text[prev:lead].strip() and not cv[prev:lead].strip():
                lead = prev
            else:
                break
        body = text[lead:end]
        brace = body.find("{", body.find(fn))
        _BODY[(rel, fn)] = strip_static(body[:brace]) + body[brace:]
    return _BODY[(rel, fn)]


def tu_bodies(smt3, text, candidates):
    """Carried bodies present in a TU: fn -> (rel, start, end), from candidates {fn: rel}."""
    out = {}
    for fn, (start, end) in definitions(text).items():
        rel = candidates.get(fn)
        if rel is None:
            continue
        body = body_text(smt3, rel, fn)
        at = text.find(body)
        if at >= 0:
            out[fn] = (rel, at, at + len(body))
            continue
        # The data tools may have put INCLUDE_RODATA/SDATA lines between the comment and the definition.
        split = definitions(body)[fn][0]
        if text[start:end] != body[split:] and not (is_dds(rel) and dds_rename(text[start:end]) == body[split:]):
            continue
        lead, comment = start, body[:split]
        while (m := re.search(r"(^INCLUDE_(RODATA|SDATA)\(.*\);\n+)\Z", text[:lead], re.M)):
            lead = m.start(1)
        if comment and text[:lead].endswith(comment):
            out[fn] = (rel, lead - len(comment), end)
        elif not comment.strip():
            out[fn] = (rel, start, end)
    return out


def set_decl_blocks(smt3, text, rels, unsized=(), own=()):
    """Keep exactly the declaration blocks of rels, new ones after the TU's leading #includes.
    own: functions the TU defines itself (not carried); a DDS block does not declare them."""
    # SMT3's blocks come first and a DDS block drops what they define.
    order = sorted(rels, key=lambda r: (is_dds(r), r))
    taken = {("ident", n) for n in own}
    for rel in order:
        if not is_dds(rel):
            block = decl_block(smt3, rel, unsized)
            for b in blocks(block):
                taken |= declared_names(b)
            taken |= {("include", h) for h in smt3_includes(block)}
            externs = extern_names(smt3_text(smt3, rel))
            taken |= {("ident", n) for n in externs} | {("extern", n) for n in externs}
    text = DECL_BLOCK.sub(lambda m: "", text)
    add = order
    if add:
        at = re.match(r"(?:[ \t]*(?:#include[^\n]*)?\n)*", text).end()
        head = text[:at].rstrip("\n")
        made = []
        for rel in add:
            made.append(decl_block(smt3, rel, unsized, taken, set(re.findall(r"SMT3_DEFINES_\w+", "".join(made)))))
            if is_dds(rel):
                taken = taken | {n for b in blocks(made[-1]) for n in declared_names(b)}
        text = (head + "\n\n" if head else "") + "\n".join(made) + "\n" + text[at:].lstrip("\n")
    return text


DATA_NAME = re.compile(r"(\w+)\s*(?:\[[^\]]*\]\s*)*$")


def data_name(item):
    """The name a top-level item defines as data (not extern, one declarator), or None."""
    cv = blank_attributes(code_view(item))
    code = cv.strip()
    if not code.endswith(";") or code.startswith(("#", "extern", "typedef", "EXTERN")) \
            or DROP.match(code) or SYMBOL_AT.match(code):
        return None
    paren, eq = top_level(cv, "("), top_level(cv, "=")
    head = cv[:eq if eq >= 0 else cv.rstrip().rfind(";")]
    if top_level(head, ",") >= 0:
        return None
    pointer = re.search(r"\(\s*\*\s*(\w+)\s*(\[[^\]]*\]\s*)*\)\s*\(", head)
    if paren >= 0 and (eq < 0 or eq > paren) and not pointer:
        return None  # a prototype
    if re.fullmatch(r"(struct|union|enum)\b[^=]*\}\s*;", code, re.S) or re.fullmatch(r"(struct|union|enum)\s+\w+\s*;", code):
        return None
    m = pointer or DATA_NAME.search(head.rstrip())
    return m.group(1) if m else None


_DATA = {}


def smt3_data(smt3):
    """Name -> [(rel, definition with `static` dropped)] for data SMT3 defines in C."""
    if not _DATA:
        for src in sorted((smt3 / "src/smt3").rglob("*.c")):
            rel = shared_rel(str(src.relative_to(smt3)))
            for item in blocks(smt3_text(smt3, rel)):
                if name := data_name(item):
                    m = STATIC.search(code_view(item))
                    _DATA.setdefault(name, []).append((rel, item[:m.start()] + item[m.end():] if m else item))
    return _DATA


def data_rows(version):
    """config/<v>/smt3_data.tsv: name -> {address, kind, rel, status, reason, size}."""
    path = ROOT / "config" / version / "smt3_data.tsv"
    out = {}
    if path.exists():
        for line in path.read_text().splitlines():
            if line and not line.startswith("#"):
                name, addr, kind, rel, status, reason, size = (line.split("\t") + [""] * 7)[:7]
                out[name] = {"address": int(addr, 16), "kind": kind, "rel": rel, "status": status, "reason": reason,
                             "size": int(size, 16) if size else None}
    return out


def write_data_rows(version, rows):
    path = ROOT / "config" / version / "smt3_data.tsv"
    lines = ["# SMT3 data definitions carried by tools/raidou_c.py: name, address, RODATA|SDATA, "
             "SMT3 file, c|asm, reason it stays split, Raidou array size"]
    lines += [(f"{n}\t0x{r['address']:08X}\t{r['kind']}\t{r['rel']}\t{r['status']}\t{r['reason']}"
               + (f"\t0x{r['size']:X}" if r.get("size") else "")).rstrip("\t")
              for n, r in sorted(rows.items())]
    path.write_text("\n".join(lines) + "\n")


def data_line(unit, kind, address):
    return f'INCLUDE_{kind}(const s32, "{unit}", D_{address:08X});'


def resize(d, name, size):
    """d with its array size set to size."""
    return re.sub(rf"\b{name}\s*\[[^\]]*\]", f"{name}[0x{size:X}]", d, count=1) if size else d


def raidou_size(version, kind, address, d):
    """Raidou's label extent for a string array SMT3 declares larger than it, else None."""
    m = re.search(r"\[\s*(0x[0-9A-Fa-f]+|\d+)\s*\]\s*(?:__attribute__\(\(.*?\)\)\s*)?=(.*);\s*$", code_view(d), re.S)
    if not m or not re.search(r"\bchar\b", d[:m.start()]):
        return None
    labels = all_labels(version, kind.lower())
    i = bisect.bisect_right(labels, address)
    extent = labels[i] - address if i < len(labels) else None
    need = string_size(d[m.start(2):m.end(2)])
    if kind == "RODATA" and extent is not None and extent <= 8:
        return None  # -G 8 would put it in .sdata
    return extent if extent and need and need <= extent < int(m.group(1), 0) else None


def data_definition(smt3, name, rel, size=None):
    d = next((d for r, d in smt3_data(smt3).get(name, []) if r == rel), None)
    return resize(d, name, size) if d else None


def tu_data(smt3, text, rows):
    """Carried data definitions present in a TU: name -> (rel, start, end)."""
    out = {}
    for name, row in rows.items():
        d = data_definition(smt3, name, row["rel"])
        if not d:
            continue
        # Match any array size: a reverted row may have lost the Raidou size.
        pattern = re.escape(resize(d, name, 1)).replace(re.escape(f"{name}[0x1]"), rf"{name}\[[^\]]*\]")
        if m := re.search(pattern, text):
            out[name] = (row["rel"], m.start(), m.end())
    return out


def port_data(smt3, text, unit, rels, ctx):
    """Swap INCLUDE_RODATA/SDATA lines at SMT3-named addresses for SMT3's C definition,
    preferring a file whose declarations the TU has (else the definition alone, if it
    compiles); put failed ones back."""
    rows, retry = ctx["rows"], ctx["retry"]
    for name, (rel, a, b) in sorted(tu_data(smt3, text, rows).items(), key=lambda x: -x[1][1]):
        row = rows[name]
        if row["status"] == "asm":
            text = text[:a] + data_line(unit, row["kind"], row["address"]) + text[b:]

    def to_c(m):
        address = int(m.group(2), 16)
        found = []
        for name in ctx["alias"].get(address, ()):
            if re.fullmatch(r"(D|jtbl)_[0-9A-F]{8}", name):
                continue  # an SMT3 placeholder would clash with Raidou's own D_ names
            row = rows.get(name)
            if row and row["status"] == "asm" and not (retry and retry.search(row["reason"])):
                continue
            have = smt3_data(smt3).get(name, [])
            found += [(name, r, d) for r, d in have if r in rels] or (
                [(name, r, d) for r, d in have] if len(have) == 1 else [])
        if len(found) != 1:
            return m.group(0)
        name, rel, d = found[0]
        size = raidou_size(ctx["version"], m.group(1), address, d)
        rows[name] = {"address": address, "kind": m.group(1), "rel": rel, "status": "c", "reason": "", "size": size}
        return resize(d, name, size)

    return re.sub(rf'^INCLUDE_(RODATA|SDATA)\(const s32, "{re.escape(unit)}", D_([0-9A-F]{{8}})\);$', to_c, text, flags=re.M)


def alias_rows(version):
    path = ROOT / "config" / version / "smt3_symbols.txt"
    return [(m.group(1), int(m.group(2), 16)) for line in path.read_text().splitlines() if (m := SYMROW.match(line))]


def write_alias_rows(version, rows):
    path = ROOT / "config" / version / "smt3_symbols.txt"
    lines = [path.read_text().splitlines()[0]]
    lines += [f"{n} = 0x{a:08X};" for n, a in sorted(rows, key=lambda x: (x[1], x[0]))]
    text = "\n".join(lines) + "\n"
    if text != path.read_text():
        path.write_text(text)


def outcomes(version):
    """config/<v>/smt3_c.tsv: function -> (status, reason)."""
    path = ROOT / "config" / version / "smt3_c.tsv"
    out = {}
    if path.exists():
        for line in path.read_text().splitlines():
            if line and not line.startswith("#"):
                fn, status, reason = (line.split("\t") + ["", ""])[:3]
                out[fn] = (status, reason)
    return out


def write_outcomes(version, rows):
    path = ROOT / "config" / version / "smt3_c.tsv"
    lines = ["# SMT3 and DDS bodies carried by tools/raidou_c.py: function, c|asm, reason it stays asm"]
    lines += [f"{fn}\t{st}\t{why}".rstrip("\t") for fn, (st, why) in sorted(rows.items())]
    path.write_text("\n".join(lines) + "\n")


def read_pairs(path=OUT / "pairs.tsv"):
    rows = []
    if not path.exists():
        return rows
    with open(path) as f:
        head = f.readline().rstrip("\n").split("\t")
        for line in f:
            rows.append(dict(zip(head, line.rstrip("\n").split("\t"))))
    return rows


SDK_BLOCK = "# SMT3 settings for SDK members with carried C (tools/raidou_c.py)"


def sdk_settings(smt3, version):
    """Give each SDK member that compiles SMT3 C the compiler, cflags and asflags SMT3
    uses for that library."""
    users = {unit for unit, src in unit_sources(version).items()
             if unit.startswith("sdk/") and "Declarations from smt3-decomp" in src.read_text()}
    for name in ("compilers.txt", "cflags.txt", "asflags.txt"):
        theirs = {}
        for line in (smt3 / "config/smt3" / name).read_text().splitlines():
            code = line.split("#", 1)[0].strip()
            if code:
                lib, value = code.split(None, 1)
                theirs[lib] = value.strip()
        path = ROOT / "config" / version / name
        text = path.read_text().split("\n" + SDK_BLOCK)[0].rstrip("\n") + "\n"
        rows = [f"{u}  {theirs[u.rsplit('/', 1)[0]]}" for u in sorted(users)
                if u.rsplit("/", 1)[0] in theirs]
        if rows:
            text += "\n" + SDK_BLOCK + "\n" + "\n".join(rows) + "\n"
        path.write_text(text)


def candidates(version):
    """fn -> source rel for every Raidou function identical to SMT3 C of the same name,
    then to DDS C (raidou_dds.py pairs) for the rest."""
    out = {}
    for path in (OUT / "pairs.tsv", OUT / "pairs_dds.tsv"):
        for r in read_pairs(path):
            if r["version"] == version and r["status"] == "identical" and r["function"] == r["smt3_name"]:
                out.setdefault(r["function"], shared_rel(r["smt3_src"]))
    return out


def nested_names(text):
    """Assembler names given to nested functions (emitted ahead of their parent)."""
    code = re.sub(r"/\*.*?\*/|//[^\n]*", "", text, flags=re.S)
    names = re.findall(r'FAKEMATCH_ASM_NAME\("(\w+)"\)', code)
    # Without an assembler name the nested function keeps its own (as name.0, local).
    for m in re.finditer(r"^[ \t]+[A-Za-z_][\w \t]*[\s*](\w+)[ \t]*\(([^;{}()]*)\)([^;{}]*)\{", code, re.M):
        if m.group(1) not in ("if", "while", "for", "switch", "return") and "FAKEMATCH_ASM_NAME" not in m.group(3):
            names.append(m.group(1))
    return names


def port_text(smt3, text, unit, carry, cands, ctx):
    """A TU with carry's bodies written inline and every other carried body back to INCLUDE_ASM."""
    def asm_line(fn):
        return f'INCLUDE_ASM(const s32, "{unit}", {fn});'

    text = INCLUDE_BODY.sub(lambda m: asm_line(m.group(2)), text)
    for fn, (rel, a, b) in sorted(tu_bodies(smt3, text, cands).items(), key=lambda x: -x[1][1]):
        if fn not in carry or (is_dds(rel) and body_text(smt3, rel, fn) not in text[a:b]):
            data = "".join(m.group(0) + "\n\n" for m in re.finditer(r"^INCLUDE_(RODATA|SDATA)\(.*\);$", text[a:b], re.M))
            asm = "".join(asm_line(n) + "\n\n" for n in nested_names(text[a:b])) + asm_line(fn)
            text = text[:a] + data + asm + text[b:]

    def to_c(m):
        fn = m.group(2)
        return body_text(smt3, carry[fn], fn) if fn in carry and m.group(1) == unit else m.group(0)

    text = re.sub(r'^INCLUDE_ASM\(\s*const s32,\s*"([^"]+)"\s*,\s*(\w+)\s*\);$', to_c, text, flags=re.M)
    # A nested function with an assembler name is emitted by its parent's body.
    for nested in nested_names(text):
        text = re.sub(rf'^INCLUDE_ASM\(\s*const s32,\s*"[^"]+"\s*,\s*{nested}\s*\);\n\n?', "", text, flags=re.M)
    carried = tu_bodies(smt3, text, cands)
    rels = {rel for rel, _, _ in carried.values()}
    own = set(definitions(text)) - set(carried)
    text = port_data(smt3, text, unit, rels, ctx)
    # Only the defining TU needs the unsized declaration; elsewhere SMT3's size is harmless.
    unsized = {n for n in tu_data(smt3, text, ctx["rows"]) if ctx["rows"][n].get("size")}
    return set_decl_blocks(smt3, text, rels, unsized, own)


def write_tus(texts):
    """texts: unit -> {version: text}. A unit with the same text in both games is one
    shared file; otherwise each game has its own. Returns the number of files changed."""
    written = 0
    for unit, by_version in texts.items():
        shared = SRC / f"{unit}.c"
        own = {v: SRC / v / f"{unit}.c" for v in VERSIONS}
        same = len(by_version) == len(VERSIONS) and len(set(by_version.values())) == 1
        want = {shared: next(iter(by_version.values()))} if same else {own[v]: x for v, x in by_version.items()}
        for path in [shared, *own.values()]:
            if path in want:
                if not path.exists() or path.read_text() != want[path]:
                    path.parent.mkdir(parents=True, exist_ok=True)
                    path.write_text(want[path])
                    written += 1
            elif path.exists():
                path.unlink()
                written += 1
    return written


def cmd_port(args):
    smt3 = args.smt3.expanduser().resolve()
    retry = re.compile(args.retry) if args.retry else None
    carry, cands = {}, {}
    for version in VERSIONS:
        cands[version] = candidates(version)
        done = outcomes(version)
        carry[version] = {}
        for fn, rel in cands[version].items():
            status, why = done.get(fn, ("c", ""))
            if status == "asm" and not (retry and retry.search(why)):
                continue
            carry[version][fn] = rel
    texts = defaultdict(dict)
    ctx = {}
    for version in VERSIONS:
        rows = data_rows(version)
        alias = defaultdict(list)
        for name, address in alias_rows(version):
            alias[address].append(name)
        for name, row in rows.items():
            if name not in alias[row["address"]]:
                alias[row["address"]].append(name)
        ctx[version] = {"rows": rows, "alias": alias, "retry": retry, "version": version}
        for unit, src in unit_sources(version).items():
            if args.units and not any(unit.startswith(u) for u in args.units):
                continue
            texts[unit][version] = port_text(smt3, src.read_text(), unit, carry[version], cands[version], ctx[version])
    written = write_tus(texts)
    for version in VERSIONS:
        done = outcomes(version)
        kept, data = set(), set()
        for unit, src in unit_sources(version).items():
            kept.update(tu_bodies(smt3, src.read_text(), cands[version]))
            data.update(tu_data(smt3, src.read_text(), ctx[version]["rows"]))
        rows = ctx[version]["rows"]
        dropped = [(n, r["address"]) for n, r in rows.items() if n not in data]
        write_data_rows(version, {n: r for n, r in rows.items() if r["status"] == "asm" or n in data})
        # A carried definition provides its name; the linker script must not.
        names = {n for n, _ in dropped}
        write_alias_rows(version, [(n, a) for n, a in alias_rows(version) if n not in data and n not in names] + dropped)
        for fn in kept:
            done[fn] = ("c", "")
        write_outcomes(version, done)
        sdk_settings(smt3, version)
        print(f"{version}: {len(kept)} functions, {len(data)} data definitions from C")
    print(f"{written} files changed; {len(shared_sources())} shared TUs")


WORK = Path.home() / ".local/state/raidou-decomp-work/stage4"
ERROR = re.compile(r"^(\S+?):(\d+):(?:\d+:)? (?!warning)(.*)$")
FROM = re.compile(r"^(?:In file included|\s+) from (\S+?):(\d+)[:,]")


def known_symbols(version):
    """Name -> address in the all-asm link (saved before any C was carried)."""
    out = {}
    for line in (WORK / f"{version}.nm").read_text().splitlines():
        parts = line.split()
        if len(parts) == 3:
            out.setdefault(parts[2], int(parts[0], 16))
    # Names given since that link.
    for line in (ROOT / "config" / version / "symbol_addrs.txt").read_text().splitlines():
        if m := SYMROW.match(line):
            out.setdefault(m.group(1), int(m.group(2), 16))
    return out


def compile_failures(log):
    """(version, unit) -> [(TU line or None, message)] for each compiler error, at the
    line of the TU itself (an error in a header is placed at the TU's #include)."""
    out, version, unit, chain = defaultdict(list), None, None, []
    for line in log.read_text(errors="replace").splitlines():
        if line.startswith("FAILED: "):
            m = re.search(r"build/(raidou\d)/src/(?:raidou\d/)?(\S+)\.o\b", line)
            version, unit = (m.group(1), m.group(2)) if m else (None, None)
            chain = []
            continue
        if m := FROM.match(line):
            chain.append((m.group(1), int(m.group(2))))
            continue
        m = ERROR.match(line)
        if not m or version is None:
            if not line.startswith((" ", "In ")):
                chain = []
            continue
        at = [(m.group(1), int(m.group(2)))] + chain
        tu = [n for path, n in at if path.startswith("src/") and path.endswith(".c")]
        out[(version, unit)].append((tu[0] if tu else None, f"{m.group(1)}:{m.group(2)}: {m.group(3).strip()}"))
        chain = []
    return out


def blame_errors(text, bodies, errors):
    """Map TU-line errors to bodies: {fn: reason}, plus the unit's other errors."""
    line_of = lambda off: text.count("\n", 0, off) + 1
    regions = [(line_of(a), line_of(b), fn) for fn, (rel, a, b) in bodies.items()]
    decls = {m.group(1): (line_of(m.start()), line_of(m.end())) for m in DECL_BLOCK.finditer(text)}
    fails, other = {}, []
    # SMT3 is authoritative: where DDS declarations meet SMT3's, the DDS bodies give way.
    dds_fns = [fn for fn, (r, _, _) in bodies.items() if is_dds(r)]
    mixed = dds_fns and any(not is_dds(r) for r, _, _ in bodies.values())
    for line, msg in errors:
        msg = msg.split(": ", 1)[1] if ": " in msg else msg
        hit = [fn for a, b, fn in regions if line is not None and a <= line <= b]
        rel = next((r for r, (a, b) in decls.items() if line is not None and a <= line <= b), None)
        if hit and not (mixed and not is_dds(bodies[hit[0]][0])):
            fails.setdefault(hit[0], f"compile error: {msg}")
        elif mixed:
            for fn in dds_fns:
                fails.setdefault(fn, f"declarations clash: {msg}")
        elif rel:
            # A clash in one SMT3 file's declarations fails all of its bodies in the unit.
            for fn, (r, _, _) in bodies.items():
                if r == rel:
                    fails.setdefault(fn, f"declarations clash: {msg}")
        else:
            other.append(msg)
    return fails, other


R_26, R_HI16, R_LO16, R_GPREL16, R_LITERAL = 4, 5, 6, 7, 8


def owned_data(asm, known):
    """Addresses of the data labels in a function's asm file (data only it uses)."""
    out = set()
    for kind, name in re.findall(r"^(dlabel|jlabel) (\S+)", asm.read_text(), re.M):
        if m := re.fullmatch(r"(?:D|jtbl)_([0-9A-F]{8})", name):
            out.add(int(m.group(1), 16))
        elif name in known:
            out.add(known[name])
        else:
            out.add(-1)  # unknown: never covered
    return out


def check_object(version, obj, bodies, retail, segs, gp, asm_sizes, known, owns={}, own_data=()):
    """Compare each carried body in obj with retail. Returns ({fn: reason},
    {fn: [(symbol, address)]})."""
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection
    from retail_elf import va_to_off

    fails, needs, refs = {}, {}, defaultdict(list)
    with open(obj, "rb") as f:
        elf = ELFFile(f)
        symtab = elf.get_section_by_name(".symtab")
        syms = list(symtab.iter_symbols())
        text_index = next(i for i, s in enumerate(elf.iter_sections()) if s.name == ".text")
        text = elf.get_section(text_index).data()
        rel = elf.get_section_by_name(".rel.text")
        relocs = sorted(((r["r_offset"], r["r_info_type"], r["r_info_sym"]) for r in rel.iter_relocations())
                        if isinstance(rel, RelocationSection) else [])
        sections = {i: s.name for i, s in enumerate(elf.iter_sections())}
        reginfo = elf.get_section_by_name(".reginfo")
        gp0 = int.from_bytes(reginfo.data()[20:24], "little") if reginfo else 0
        defined = {s.name: s for s in syms if s["st_shndx"] == text_index and s["st_info"]["type"] == "STT_FUNC"}
        # The unit's data comes from the split: C must not define any of its own.
        stray = [s.name for s in syms if s["st_info"]["bind"] != "STB_LOCAL" and s.name
                 and (s["st_shndx"] == "SHN_COMMON"
                      or (isinstance(s["st_shndx"], int) and s["st_shndx"] != text_index
                          and s["st_info"]["type"] == "STT_OBJECT" and s.name not in known and s.name not in own_data))]
        if stray:
            return {fn: f"unit defines {stray[0]}" for fn in bodies}, {}
        for fn, addr in bodies.items():
            sym = defined.get(fn)
            if sym is None:
                fails[fn] = "not emitted"
                continue
            off, size = sym["st_value"], sym["st_size"]
            roff = va_to_off(segs, addr)
            tail = retail[roff + size:roff + asm_sizes[fn] * 4]
            if size > asm_sizes[fn] * 4 or any(tail):
                fails[fn] = f"size 0x{size:X}, retail 0x{asm_sizes[fn] * 4:X}"
                continue
            mine = [int.from_bytes(text[off + i:off + i + 4], "little") for i in range(0, size, 4)]
            theirs = [int.from_bytes(retail[roff + i:roff + i + 4], "little") for i in range(0, size, 4)]
            own = [r for r in relocs if off <= r[0] < off + size]
            masks = {(r[0] - off) // 4: (0xFC000000 if r[1] == R_26 else 0xFFFF0000) for r in own}
            bad = [i for i in range(len(mine)) if (mine[i] ^ theirs[i]) & masks.get(i, 0xFFFFFFFF)]
            if bad:
                fails[fn] = f"{len(bad)} words differ, first at +0x{bad[0] * 4:X}"
                continue
            need, reason, last_hi = [], None, {}
            for r_off, r_type, r_sym in own:
                i = (r_off - off) // 4
                s = syms[r_sym]
                if s["st_shndx"] == text_index and r_type == R_26:
                    # A jump within the unit: same distance from the unit start.
                    target = ((theirs[i] & 0x3FFFFFF) << 2) | ((addr + i * 4) & 0xF0000000)
                    if target - (addr - off) != ((mine[i] & 0x3FFFFFF) << 2) + s["st_value"]:
                        reason = f"local jump at +0x{i * 4:X}"
                        break
                    continue
                if s["st_shndx"] == text_index and s["st_info"]["type"] == "STT_SECTION":
                    name = "@text"
                elif s["st_info"]["type"] == "STT_SECTION" or s["st_info"]["bind"] == "STB_LOCAL":
                    # Data the body emits itself: the need is the section's base address.
                    sec = sections.get(s["st_shndx"])
                    if sec not in DATA_SECTIONS:
                        reason = f"emits {sec} data"
                        break
                    name = "@" + sec
                    local = s["st_value"]
                else:
                    name = s.name
                if r_type == R_26:
                    target = ((theirs[i] & 0x3FFFFFF) << 2) | ((addr + i * 4) & 0xF0000000)
                    need.append((name, target - ((mine[i] & 0x3FFFFFF) << 2)))
                elif r_type == R_HI16:
                    last_hi[name] = i
                elif r_type == R_LO16:
                    h = last_hi.get(name)
                    if h is None:
                        reason = f"%lo({name}) without %hi"
                        break
                    value = ((theirs[h] & 0xFFFF) << 16) + sext16(theirs[i])
                    addend = ((mine[h] & 0xFFFF) << 16) + sext16(mine[i])
                    if name[0] == "@" and name != "@text":
                        addend += local
                        refs[fn].append((name[1:], addend))
                    need.append((name, value - addend))
                elif r_type in (R_GPREL16, R_LITERAL):
                    addend = sext16(mine[i])
                    if name[0] == "@":
                        addend += local + gp0    # a local gp-relative field is offset by the object's gp0
                        refs[fn].append((name[1:], addend))
                    need.append((name, gp + sext16(theirs[i]) - addend))
                else:
                    reason = f"relocation type {r_type}"
                    break
            base = {a for n, a in need if n == "@text"}
            if not reason and base and base != {addr - off}:
                reason = "address of unit code differs"
            need = [x for x in need if x[0] != "@text"]
            if reason:
                fails[fn] = reason
            else:
                needs[fn] = need
        text_base = next((addr - defined[fn]["st_value"] for fn, addr in bodies.items() if fn in needs), None)
        fails.update(check_sections(elf, needs, refs, retail, segs, text_base))
        for fn, owned in dict(owns).items():
            # Owned data the body names externally is kept by include_rodata.py.
            if fn in needs and fn not in fails and not refs[fn] and not owned <= {a for _, a in needs[fn]}:
                fails[fn] = "its rodata is in its asm file"
    for fn in fails:
        needs.pop(fn, None)
    for fn in needs:
        needs[fn] = [x for x in needs[fn] if x[0][0] != "@"]
    return fails, needs


DATA_SECTIONS = (".rodata", ".lit4", ".data", ".sdata")
R_32 = 2


def check_sections(elf, needs, refs, retail, segs, text_base):
    """Each data section the bodies emit into must have one base, and the whole
    section must equal retail there (relocated words: .text targets checked, others
    skipped). Returns {fn: reason}."""
    from elftools.elf.relocation import RelocationSection
    from retail_elf import va_to_off

    fails = {}
    users = defaultdict(dict)    # section -> fn -> [section offsets it refers to]
    bases = defaultdict(set)
    for fn, need in needs.items():
        for name, base in need:
            if name[0] == "@" and name != "@text":
                users[name[1:]].setdefault(fn, [])
                bases[name[1:]].add(base)
    for sec, fns in users.items():
        if len(bases[sec]) > 1:
            for fn in fns:
                fails[fn] = f"{sec} base " + ", ".join(f"0x{b:08X}" for b in sorted(bases[sec]))
            continue
        base = next(iter(bases[sec]))
        data = elf.get_section_by_name(sec).data()
        roff = va_to_off(segs, base)
        if roff is None:
            for fn in fns:
                fails[fn] = f"{sec} base 0x{base:08X} outside the image"
            continue
        want = retail[roff:roff + len(data)]
        relsec = elf.get_section_by_name(".rel" + sec)
        skip, text_words = set(), {}
        if isinstance(relsec, RelocationSection):
            symtab = elf.get_section(relsec["sh_link"])
            for r in relsec.iter_relocations():
                o = r["r_offset"]
                s = symtab.get_symbol(r["r_info_sym"])
                if r["r_info_type"] == R_32 and s["st_info"]["type"] == "STT_SECTION" \
                        and elf.get_section(s["st_shndx"]).name == ".text" and text_base is not None:
                    text_words[o] = text_base
                else:
                    skip.add(o)
        first = None
        for o in range(0, len(data) - len(data) % 4, 4):
            mw = int.from_bytes(data[o:o + 4], "little")
            rw = int.from_bytes(want[o:o + 4], "little")
            if o in skip:
                continue
            if o in text_words:
                mw = (mw + text_words[o]) & 0xFFFFFFFF
            if mw != rw:
                first = o
                break
        if first is None and data[len(data) - len(data) % 4:] != want[len(data) - len(data) % 4:]:
            first = len(data) - len(data) % 4
        if first is None:
            continue
        # Blame the body whose data starts closest before the difference.
        why = f"{sec} differs at 0x{base + first:08X}"
        before = [(o, fn) for fn in fns for s, o in refs[fn] if s == sec and o <= first]
        for fn in [max(before)[1]] if before else fns:
            fails[fn] = why
    return fails


_LABELS = {}


def data_labels(version, unit, sec):
    """Addresses of the labels in a unit's split .rodata or .sdata, sorted."""
    path = ROOT / "asm" / version / "data" / f"{unit}.{sec.lstrip('.')}.s"
    if not path.exists():
        return []
    return sorted({int(a, 16) for a in re.findall(r"^dlabel \S+\n\s*/\* [0-9A-F]+ ([0-9A-F]{8})", path.read_text(), re.M)})


def all_labels(version, sec):
    """Addresses of every label in the game's split .rodata or .sdata, sorted."""
    if (version, sec) not in _LABELS:
        found = set()
        for path in (ROOT / "asm" / version / "data").rglob(f"*.{sec.lstrip('.')}.s"):
            found.update(int(a, 16) for a in re.findall(r"^dlabel \S+\n\s*/\* [0-9A-F]+ ([0-9A-F]{8})", path.read_text(), re.M))
        _LABELS[(version, sec)] = sorted(found)
    return _LABELS[(version, sec)]


def first_displaced(path):
    """(displaced object, object before it) at the first D_ label the link moved."""
    obj = prev = None
    for line in path.read_text().splitlines():
        if m := re.match(r"^ \.\w+\s+0x[0-9a-f]+\s+0x([0-9a-f]+) (\S+\.o)$", line):
            if int(m.group(1), 16) and m.group(2) != obj:
                prev, obj = obj, m.group(2)
        elif (m := re.match(r"^\s+0x([0-9a-f]+)\s+D_([0-9A-F]{8})\s*$", line)) \
                and int(m.group(1), 16) != int(m.group(2), 16) and int(m.group(1), 16) > 0x100000:
            return obj, prev
    return None, None


def map_starts(path):
    """(object path, section) -> address of that input section in a link map."""
    out = {}
    for line in path.read_text().splitlines():
        if m := re.match(r"^ (\.\w+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (\S+\.o)$", line):
            if int(m.group(3), 16):
                out.setdefault((m.group(4), m.group(1)), int(m.group(2), 16))
    return out


def check_data_object(version, obj, want, units, retail, segs):
    """Each definition in a unit object against the split: no larger than the label
    it replaces, with retail's bytes (relocated words skipped). Returns
    ({name: reason}, {name: section})."""
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection
    from retail_elf import va_to_off

    fails, section = {}, {}
    with open(obj, "rb") as f:
        elf = ELFFile(f)
        symbols = [s for s in elf.get_section_by_name(".symtab").iter_symbols() if isinstance(s["st_shndx"], int)]
        syms = {s.name: s for s in symbols if s.name in want and s["st_info"]["bind"] != "STB_LOCAL"}
        for n, address in want.items():
            s = syms.get(n)
            if s is None:
                fails[n] = "not defined in the object"
                continue
            sec = elf.get_section(s["st_shndx"])
            section[n] = sec.name
            off, size = s["st_value"], s["st_size"]
            labels = data_labels(version, units[n], sec.name)
            glob = all_labels(version, sec.name)
            later = glob[bisect.bisect_right(glob, address):][:1]
            if address not in labels:
                fails[n] = f"in {sec.name}, retail label is not"
                continue
            if later and size > later[0] - address:
                fails[n] = f"size 0x{size:X}, retail 0x{later[0] - address:X}"
                continue
            # What follows it in the object must follow it in retail by the same distance.
            after = [x["st_value"] for x in symbols if x["st_shndx"] == s["st_shndx"] and x["st_value"] > off
                     and x["st_info"]["type"] in ("STT_OBJECT", "STT_NOTYPE")]
            if after and later and min(after) - off != later[0] - address:
                fails[n] = f"next item at +0x{min(after) - off:X}, retail +0x{later[0] - address:X}"
                continue
            if sec["sh_type"] == "SHT_NOBITS":
                continue
            skip = set()
            relsec = elf.get_section_by_name(".rel" + sec.name)
            if isinstance(relsec, RelocationSection):
                skip = {r["r_offset"] for r in relsec.iter_relocations()}
            data, roff = sec.data(), va_to_off(segs, address)
            bad = next((i for i in range(size) if (off + i) & ~3 not in skip
                        and data[off + i] != retail[roff + i]), None)
            if bad is not None:
                fails[n] = f"bytes differ at +0x{bad:X}"
    return fails, section


def check_data(version, want, objs, units, retail, segs):
    """Each carried definition must match the split in its object and sit at its address
    in the link. A unit already shifted where it starts is not blamed; within a unit only
    the first definition off its place is. Returns {name: reason}."""
    from elftools.elf.elffile import ELFFile
    from retail_elf import VERSIONS as INFO

    if not want:
        return {}
    fails, section = {}, {}
    by_obj = defaultdict(dict)
    for n, a in want.items():
        by_obj[objs[n]][n] = a
    for obj, names in by_obj.items():
        f, sec = check_data_object(version, obj, names, units, retail, segs)
        fails.update(f)
        section.update(sec)
    serial = INFO[version]["serial"]
    elf_path = ROOT / "build" / version / f"{serial}.elf"
    if not elf_path.exists():
        return {n: fails.get(n, "no link") for n in want}
    stale = {n for n, obj in objs.items() if obj.stat().st_mtime > elf_path.stat().st_mtime}
    for n in stale:
        fails.setdefault(n, "no link")
    with open(elf_path, "rb") as f:
        linked = {s.name: s["st_value"] for s in ELFFile(f).get_section_by_name(".symtab").iter_symbols()
                  if s.name in want and s["st_info"]["bind"] != "STB_LOCAL"}
    starts = map_starts(elf_path.with_suffix(".map"))
    groups = defaultdict(list)
    for n, a in want.items():
        if n in section:
            groups[(objs[n], section[n])].append((a, n))
    off_place = []
    for (obj, sec), items in groups.items():
        labels = data_labels(version, units[items[0][1]], sec)
        start = starts.get((str(obj.relative_to(ROOT)), sec))
        d0 = start - labels[0] if start is not None and labels else None
        for a, n in sorted(items):
            if n not in linked:
                fails.setdefault(n, "not in the link")
                continue
            if linked[n] == a:
                continue
            off_place.append((a, n))
            if d0 == 0 and linked[n] - a != d0:
                if not any(m in fails for b, m in items if b < a):
                    fails.setdefault(n, f"linked at 0x{linked[n]:08X}, retail 0x{a:08X}")
                break
    if off_place and not any(n in fails for _, n in off_place):
        # The first label the link moved follows the object that grew or shrank.
        _, prev = first_displaced(elf_path.with_suffix(".map"))
        before = [(a, n) for n, a in want.items() if prev and str(objs[n].relative_to(ROOT)) == prev]
        if before:
            a, n = max(before)
            fails[n] = "last in the object before the first moved label"
        else:
            a, n = min(off_place)
            fails[n] = f"linked at 0x{linked[n]:08X}, retail 0x{a:08X}"
    return fails


def sext16(v):
    return ((v & 0xFFFF) ^ 0x8000) - 0x8000


PLACEHOLDER = re.compile(r"(func|D|jtbl|L|lbl)_[0-9A-F]{8}\w*|\..*")


def choose_addresses(want, named_at, smt3_fns=frozenset()):
    """For a name bodies want at several addresses: the address most SMT3 bodies (then
    most bodies) want, when no other real name is there and no other chosen name takes it.
    SMT3 decides shared names; bodies that want another address differ in a callee or an offset."""
    options = []
    for name, by_address in want.items():
        if len(by_address) < 2:
            continue
        counts = sorted((((len(fns & smt3_fns), len(fns)), a) for a, fns in by_address.items()
                         if not (named_at.get(a, set()) - {name})), reverse=True)
        if counts and (len(counts) == 1 or counts[0][0] > counts[1][0]):
            options.append((counts[0][0], name, counts[0][1]))
    chosen, taken = {}, set()
    for _, name, address in sorted(options, reverse=True):
        if address not in taken:
            chosen[name] = address
            taken.add(address)
    return chosen


def cmd_verify(args):
    from retail_elf import RETAIL, VERSIONS as INFO, load_segments

    smt3 = args.smt3.expanduser().resolve()
    compile_fails = compile_failures(args.log)
    for version in VERSIONS:
        retail = (ROOT / RETAIL[version]).read_bytes()
        segs = load_segments(retail)
        gp = int(INFO[version]["gp"], 16)
        known = known_symbols(version)
        done = outcomes(version)
        fails, needs = {}, {}
        cands = candidates(version)
        rows = data_rows(version)
        carried = {n: r for n, r in rows.items() if r["status"] == "c"}
        data_fails, data_objs, data_units = {}, {}, {}
        for unit, src in unit_sources(version).items():
            text = src.read_text()
            spans = tu_bodies(smt3, text, cands)
            bodies = {fn: known[fn] for fn in spans}
            dspans = tu_data(smt3, text, carried)
            if not bodies and not dspans:
                continue
            f, other = blame_errors(text, {**spans, **dspans}, compile_fails.get((version, unit), []))
            data_fails.update({n: why for n, why in f.items() if n in dspans})
            f = {fn: why for fn, why in f.items() if fn in spans}
            fails.update(f)
            if other or f:
                # Bodies that compiled are checked once the unit builds again.
                for fn in [*bodies, *dspans]:
                    if fn not in fails and fn not in data_fails and other:
                        (data_fails if fn in dspans else fails)[fn] = "unit compile error: " + other[0]
                continue
            obj = ROOT / "build" / version / src.relative_to(ROOT).with_suffix(".o")
            if not obj.exists() or obj.stat().st_mtime < src.stat().st_mtime:
                for fn in bodies:
                    fails.setdefault(fn, "no object")
                for n in dspans:
                    data_fails.setdefault(n, "no object")
                continue
            data_objs.update({n: obj for n in dspans if n not in data_fails})
            data_units.update({n: unit for n in dspans})
            sizes = {fn: len(parse_asm(function_asm(version, unit, fn))[fn][1])
                     for fn in bodies}
            # splat moves rodata only this function uses into its asm file and
            # drops it once the function is C, so the C must emit it.
            owns = {fn: owned_data(function_asm(version, unit, fn), known) for fn in bodies}
            owns = {fn: a for fn, a in owns.items() if a}
            if bodies:
                f, n = check_object(version, obj, {k: v for k, v in bodies.items() if k not in fails},
                                    retail, segs, gp, sizes, known, owns, set(dspans))
                fails.update(f)
                needs.update(n)
        data_fails.update(check_data(version, {n: carried[n]["address"] for n in data_objs}, data_objs, data_units, retail, segs))
        # A link that did not happen decides nothing: those stay C for the next build.
        data_fails = {n: why for n, why in data_fails.items() if why != "no link"}
        defined = set(data_objs) - set(data_fails)
        # Every name a body uses must name one Raidou address.
        users = defaultdict(set)
        want = defaultdict(lambda: defaultdict(set))  # name -> address -> functions
        for fn, need in needs.items():
            for name, address in need:
                users[name].add(fn)
                want[name][address].add(fn)
        named_at = defaultdict(set)
        for name, address in known.items():
            if not PLACEHOLDER.fullmatch(name):
                named_at[address].add(name)
        aliases, chosen = {}, choose_addresses(want, named_at, {fn for fn, rel in cands.items() if not is_dds(rel)})
        for name, by_address in want.items():
            if len(by_address) > 1:
                pick = chosen.get(name)
                listed = ", ".join(f"0x{a:08X}" for a in sorted(by_address))
                if pick is not None:
                    aliases[name] = pick
                    for address, fns in by_address.items():
                        for fn in fns:
                            if address != pick:
                                fails.setdefault(fn, f"{name} at 0x{address:08X}, others at 0x{pick:08X}")
                    continue
                why = f"{name} at {listed}"
            elif name in known and known[name] != next(iter(by_address)):
                why = f"{name} is 0x{known[name]:08X}, code wants 0x{next(iter(by_address)):08X}"
            else:
                if name not in known:
                    aliases[name] = next(iter(by_address))
                continue
            for fn in users[name]:
                fails.setdefault(fn, why)
        aliases = {n: a for n, a in aliases.items() if not users[n] <= set(fails) and n not in defined}
        for fn, why in fails.items():
            done[fn] = ("asm", why)
        write_outcomes(version, done)
        for n, why in data_fails.items():
            rows[n] = dict(rows[n], status="asm", reason=why)
        write_data_rows(version, rows)
        path = ROOT / "config" / version / "smt3_symbols.txt"
        lines = [path.read_text().splitlines()[0]]
        lines += [f"{n} = 0x{a:08X};" for n, a in sorted(aliases.items(), key=lambda x: (x[1], x[0]))]
        path.write_text("\n".join(lines) + "\n")
        kept = sum(1 for fn in needs if fn not in fails)
        print(f"{version}: {kept} match, {len(fails)} back to asm, {len(aliases)} SMT3 names; "
              f"data {len(defined)} match, {len(data_fails)} back to the split")
        for n, why in sorted(data_fails.items())[:20]:
            print(f"  data {n}: {why}")
        reasons = defaultdict(int)
        for why in fails.values():
            reasons[re.sub(r"0x[0-9A-F]+|\d+|'[^']*'|`[^']*'|\w+ (is|at) ", "#", why)[:60]] += 1
        for why, count in sorted(reasons.items(), key=lambda x: -x[1])[:12]:
            print(f"  {count:5d}  {why}")


MAP = Path.home() / ".local/state/smt3-port-work/raidou-map"


def cmd_names(args):
    """Name func_* functions whose masked words occur once in SMT3 and once in the game,
    when the SMT3 function is compiled from C. Writes names_<v>.tsv for raidou_names.py --apply."""
    smt3 = args.smt3.expanduser().resolve()
    c_funcs = smt3_c_functions(smt3)
    by_words = defaultdict(list)
    for addr, (words, slots) in smt3_bodies(smt3).items():
        by_words[tuple(words)].append((addr, slots))
    OUT.mkdir(parents=True, exist_ok=True)
    for version in VERSIONS:
        taken = {m.group(1) for line in (ROOT / "config" / version / "symbol_addrs.txt").read_text().splitlines()
                 if (m := SYMROW.match(line))}
        fwd = {}
        for line in (MAP / f"{version}_map.tsv").read_text().splitlines()[1:]:
            cols = line.split("\t")
            if len(cols) >= 4 and cols[3]:
                fwd[int(cols[0], 16)] = int(cols[3], 16)
        ours = defaultdict(list)
        for src, unit, fn, addr, words, slots in raidou_asm_functions(version):
            ours[tuple(words)].append((fn, addr, slots))
        rows, counts = [], defaultdict(int)
        for key, found in ours.items():
            fn, addr, slots = found[0]
            if not re.fullmatch(r"func_[0-9A-F]{8}", fn) or len(key) < 16:
                continue
            theirs = by_words.get(key, [])
            if len(found) > 1 or len(theirs) != 1:
                counts["not unique" if theirs else "no match"] += 1
                continue
            saddr, sslots = theirs[0]
            if saddr not in c_funcs:
                counts["smt3 asm"] += 1
                continue
            name = c_funcs[saddr][0]
            if re.fullmatch(r"func_[0-9A-F]{8}", name) or name in taken:
                counts["name taken" if name in taken else "smt3 unnamed"] += 1
                continue
            if [k for k, _ in sslots] != [k for k, _ in slots]:
                counts["slots differ"] += 1
                continue
            mapped = fwd.get(addr)
            if mapped is not None and mapped != saddr:
                counts["map disagrees"] += 1
                continue
            verdict = "map-agrees" if mapped == saddr else "map-silent"
            taken.add(name)
            counts["named"] += 1
            rows.append(f"0x{addr:08X}\t{name}\tinferred\tsmt3-decomp C name via SLUS_209.11 "
                        f"0x{saddr:08X}, unique-masked-hash-c; {verdict}")
        (OUT / f"names_{version}.tsv").write_text("\n".join(sorted(rows)) + "\n")
        print(version, dict(sorted(counts.items())))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("pairs")
    p.add_argument("--smt3", type=Path, default=SMT3)
    p.set_defaults(func=cmd_pairs)
    p = sub.add_parser("port")
    p.add_argument("--smt3", type=Path, default=SMT3)
    p.add_argument("--units", nargs="*", help="only units under these paths (e.g. sdf sdk)")
    p.add_argument("--retry", help="carry again asm rows whose reason matches this regex")
    p.set_defaults(func=cmd_port)
    p = sub.add_parser("verify")
    p.add_argument("--smt3", type=Path, default=SMT3)
    p.add_argument("--log", type=Path, default=Path.home() / ".local/state/raidou-decomp-work/stage4/ninja.log")
    p.set_defaults(func=cmd_verify)
    p = sub.add_parser("names")
    p.add_argument("--smt3", type=Path, default=SMT3)
    p.set_defaults(func=cmd_names)
    for p in sub.choices.values():
        p.add_argument("--dds", type=Path, default=Path.home() / "projects/dds-decomp",
                       help="dds-decomp checkout, configured (split)")
    args = ap.parse_args()
    global DDS
    DDS = args.dds.expanduser().resolve()
    args.func(args)


if __name__ == "__main__":
    main()
