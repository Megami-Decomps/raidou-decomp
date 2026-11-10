#!/usr/bin/env python3
"""Carry smt3-decomp C bodies into Raidou units where the code is identical.

    python3 tools/raidou_c.py pairs|port|verify   (see docs/shared-c.md)
"""

from __future__ import annotations

import argparse
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


def smt3_symbols(smt3):
    out = {}
    for line in (smt3 / "config/smt3/symbol_addrs.txt").read_text().splitlines():
        if m := SYMROW.match(line):
            out.setdefault(m.group(1), int(m.group(2), 16))
    return out


def smt3_c_functions(smt3):
    """SMT3 address -> (name, source path) for every function the normal build compiles from C."""
    syms = smt3_symbols(smt3)
    out = {}
    for src in sorted((smt3 / "src/smt3").rglob("*.c")):
        text = drop_nonmatching(src.read_text(errors="replace"))
        for name in definitions(text):
            addr = syms.get(name)
            if addr is None and (m := re.fullmatch(r"func_([0-9A-F]{8})", name)):
                addr = int(m.group(1), 16)
            if addr is not None:
                out.setdefault(addr, (name, str(src.relative_to(smt3))))
    return out


def smt3_bodies(smt3):
    """SMT3 address -> (masked words, slots), from the split assembly."""
    out = {}
    for sub in ("matchings", "nonmatchings"):
        for path in (smt3 / "asm/smt3" / sub).rglob("*.s"):
            for _, (addr, words, slots) in parse_asm(path).items():
                if addr is not None:
                    out.setdefault(addr, (words, slots))
    return out


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


def shared_rel(smt3_src):
    return Path(smt3_src).relative_to("src/smt3").with_suffix("").as_posix()


_SMT3_TEXT = {}


def smt3_text(smt3, rel):
    if rel not in _SMT3_TEXT:
        _SMT3_TEXT[rel] = drop_nonmatching((smt3 / "src/smt3" / f"{rel}.c").read_text(errors="replace"))
    return _SMT3_TEXT[rel]


def decl_begin(rel):
    return f"/* Declarations from smt3-decomp src/smt3/{rel}.c */"


def decl_end(rel):
    return f"/* End of declarations from src/smt3/{rel}.c */"


DECL_BLOCK = re.compile(r"^/\* Declarations from smt3-decomp src/smt3/(\S+)\.c \*/\n.*?"
                        r"^/\* End of declarations from src/smt3/\1\.c \*/\n\n?", re.M | re.S)


def decl_block(smt3, rel):
    """Every declaration of one SMT3 file (types, prototypes, its data as extern)."""
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
        decls.append(d)
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
        if text[start:end] != body[split:]:
            continue
        lead, comment = start, body[:split]
        while (m := re.search(r"(^INCLUDE_(RODATA|SDATA)\(.*\);\n+)\Z", text[:lead], re.M)):
            lead = m.start(1)
        if comment and text[:lead].endswith(comment):
            out[fn] = (rel, lead - len(comment), end)
        elif not comment.strip():
            out[fn] = (rel, start, end)
    return out


def set_decl_blocks(smt3, text, rels):
    """Keep exactly the declaration blocks of rels, new ones after the TU's leading #includes."""
    have = {m.group(1) for m in DECL_BLOCK.finditer(text)}
    text = DECL_BLOCK.sub(lambda m: decl_block(smt3, m.group(1)) + "\n" if m.group(1) in rels else "", text)
    add = sorted(set(rels) - have)
    if add:
        at = re.match(r"(?:[ \t]*(?:#include[^\n]*)?\n)*", text).end()
        head = text[:at].rstrip("\n")
        text = (head + "\n\n" if head else "") + "\n".join(decl_block(smt3, rel) for rel in add) \
            + "\n" + text[at:].lstrip("\n")
    return text


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
    lines = ["# SMT3 bodies carried by tools/raidou_c.py: function, c|asm, reason it stays asm"]
    lines += [f"{fn}\t{st}\t{why}".rstrip("\t") for fn, (st, why) in sorted(rows.items())]
    path.write_text("\n".join(lines) + "\n")


def read_pairs():
    rows = []
    with open(OUT / "pairs.tsv") as f:
        head = f.readline().rstrip("\n").split("\t")
        for line in f:
            rows.append(dict(zip(head, line.rstrip("\n").split("\t"))))
    return rows


SDK_BLOCK = "# SMT3 settings for SDK members with carried C (tools/raidou_c.py)"


def sdk_settings(smt3, version):
    """Give each SDK member that compiles SMT3 C the compiler, cflags and asflags SMT3
    uses for that library."""
    users = {unit for unit, src in unit_sources(version).items()
             if unit.startswith("sdk/") and DECL_BLOCK.search(src.read_text())}
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
    """fn -> SMT3 rel for every Raidou function identical to SMT3 C of the same name."""
    return {r["function"]: shared_rel(r["smt3_src"]) for r in read_pairs()
            if r["version"] == version and r["status"] == "identical" and r["function"] == r["smt3_name"]}


def nested_names(text):
    """Assembler names given to nested functions (emitted ahead of their parent)."""
    return re.findall(r'FAKEMATCH_ASM_NAME\("(\w+)"\)', re.sub(r"/\*.*?\*/|//[^\n]*", "", text, flags=re.S))


def port_text(smt3, text, unit, carry, cands):
    """A TU with carry's bodies written inline and every other carried body back to INCLUDE_ASM."""
    def asm_line(fn):
        return f'INCLUDE_ASM(const s32, "{unit}", {fn});'

    text = INCLUDE_BODY.sub(lambda m: asm_line(m.group(2)), text)
    for fn, (rel, a, b) in sorted(tu_bodies(smt3, text, cands).items(), key=lambda x: -x[1][1]):
        if fn not in carry:
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
    rels = {rel for rel, _, _ in tu_bodies(smt3, text, cands).values()}
    return set_decl_blocks(smt3, text, rels)


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
    for version in VERSIONS:
        for unit, src in unit_sources(version).items():
            if args.units and not any(unit.startswith(u) for u in args.units):
                continue
            texts[unit][version] = port_text(smt3, src.read_text(), unit, carry[version], cands[version])
    written = write_tus(texts)
    for version in VERSIONS:
        done = outcomes(version)
        kept = set()
        for unit, src in unit_sources(version).items():
            kept.update(tu_bodies(smt3, src.read_text(), cands[version]))
        for fn in kept:
            done[fn] = ("c", "")
        write_outcomes(version, done)
        sdk_settings(smt3, version)
        print(f"{version}: {len(kept)} functions from C")
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
    for line, msg in errors:
        msg = msg.split(": ", 1)[1] if ": " in msg else msg
        hit = [fn for a, b, fn in regions if line is not None and a <= line <= b]
        rel = next((r for r, (a, b) in decls.items() if line is not None and a <= line <= b), None)
        if hit:
            fails.setdefault(hit[0], f"compile error: {msg}")
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


def check_object(version, obj, bodies, retail, segs, gp, asm_sizes, known, owns={}):
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
                          and s["st_info"]["type"] == "STT_OBJECT" and s.name not in known))]
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


def sext16(v):
    return ((v & 0xFFFF) ^ 0x8000) - 0x8000


PLACEHOLDER = re.compile(r"(func|D|jtbl|L|lbl)_[0-9A-F]{8}\w*|\..*")


def choose_addresses(want, named_at):
    """For a name bodies want at several addresses: the address most of them want,
    when no other real name is there and no other chosen name takes it. Bodies that
    want another address differ from SMT3 in a callee or an offset."""
    options = []
    for name, by_address in want.items():
        if len(by_address) < 2:
            continue
        counts = sorted(((len(fns), a) for a, fns in by_address.items()
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
        for unit, src in unit_sources(version).items():
            text = src.read_text()
            spans = tu_bodies(smt3, text, cands)
            bodies = {fn: known[fn] for fn in spans}
            if not bodies:
                continue
            f, other = blame_errors(text, spans, compile_fails.get((version, unit), []))
            fails.update(f)
            if other or f:
                # Bodies that compiled are checked once the unit builds again.
                for fn in bodies:
                    if fn not in fails and other:
                        fails[fn] = "unit compile error: " + other[0]
                continue
            obj = ROOT / "build" / version / src.relative_to(ROOT).with_suffix(".o")
            if not obj.exists() or obj.stat().st_mtime < src.stat().st_mtime:
                for fn in bodies:
                    fails.setdefault(fn, "no object")
                continue
            sizes = {fn: len(parse_asm(function_asm(version, unit, fn))[fn][1])
                     for fn in bodies}
            # splat moves rodata only this function uses into its asm file and
            # drops it once the function is C, so the C must emit it.
            owns = {fn: owned_data(function_asm(version, unit, fn), known) for fn in bodies}
            owns = {fn: a for fn, a in owns.items() if a}
            f, n = check_object(version, obj, {k: v for k, v in bodies.items() if k not in fails},
                                retail, segs, gp, sizes, known, owns)
            fails.update(f)
            needs.update(n)
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
        aliases, chosen = {}, choose_addresses(want, named_at)
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
        aliases = {n: a for n, a in aliases.items() if not users[n] <= set(fails)}
        for fn, why in fails.items():
            done[fn] = ("asm", why)
        write_outcomes(version, done)
        path = ROOT / "config" / version / "smt3_symbols.txt"
        lines = [path.read_text().splitlines()[0]]
        lines += [f"{n} = 0x{a:08X};" for n, a in sorted(aliases.items(), key=lambda x: (x[1], x[0]))]
        path.write_text("\n".join(lines) + "\n")
        kept = sum(1 for fn in needs if fn not in fails)
        print(f"{version}: {kept} match, {len(fails)} back to asm, {len(aliases)} SMT3 names")
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
    args = ap.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
