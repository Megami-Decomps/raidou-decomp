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

VERSIONS = ("raidou1", "raidou2")
OUT = ROOT / "build" / "raidou_c"

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
    candidate = re.compile(r"^[\w \t*\n]*?(\w+)[ \t]*(?:\n[ \t]*)?\(", re.M)
    for m in candidate.finditer(code):
        name = m.group(1)
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
    """Each INCLUDE_ASM or carried body line of a C unit: (src, unit, name, address, words, slots)."""
    rows = []
    for src in sorted((ROOT / "src" / version).rglob("*.c")):
        text = src.read_text(errors="replace")
        unit = src.relative_to(ROOT / "src" / version).with_suffix("").as_posix()
        lines = INCLUDE_ASM.findall(text) + [(unit, m.group(2)) for m in INCLUDE_BODY.finditer(text)]
        for unit, fn in lines:
            asm = function_asm(version, unit, fn)
            if asm is None:
                continue
            body = parse_asm(asm).get(fn)
            if body:
                rows.append((str(src.relative_to(ROOT)), unit, fn, *body))
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


SHARED = ROOT / "src" / "shared"
KNR_HEADER = re.compile(r"^[A-Za-z_][\w \t\*]*\b\w+\s*\(\s*\w+(\s*,\s*\w+)*\s*\)\s*$")
STATIC = re.compile(r"\bstatic\s+")
DROP = re.compile(r"^\s*(INCLUDE_ASM|INCLUDE_RODATA|INCLUDE_SDATA|TEXT_ALIGN|__asm__|asm)\b")


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
    if code.startswith("#"):
        return item if re.match(r"#\s*(include|define|undef|if|ifdef|ifndef|elif|else|endif)\b", code) else None
    if code.startswith("typedef") or re.fullmatch(r"(struct|union|enum)\s+\w+\s*;", code) \
            or (re.match(r"(struct|union|enum)\b", code) and code.rstrip(";").rstrip().endswith("}")):
        return item
    brace, paren, eq = top_level(cv, "{"), top_level(cv, "("), top_level(cv, "=")
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
    if paren >= 0 and (eq < 0 or eq > paren) and brace < 0:
        return strip_static(item)
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
    if not re.match(r"(\s*/\*.*?\*/\s*|\s*//[^\n]*\n)*\s*extern\b", body, re.S):
        lead = len(body) - len(code_view(body).lstrip()) if code_view(body).strip() else 0
        body = body[:lead] + "extern " + body[lead:]
    return body + ";"


def shared_rel(smt3_src):
    return Path(smt3_src).relative_to("src/smt3").with_suffix("").as_posix()


def write_shared(smt3, rel, functions):
    """src/shared/<rel>.h (the SMT3 file's declarations) and one body file per function."""
    text = drop_nonmatching((smt3 / "src/smt3" / f"{rel}.c").read_text(errors="replace"))
    guard = "SHARED_" + re.sub(r"\W", "_", rel).upper() + "_H"
    decls = [d for d in (declaration(b) for b in blocks(text)) if d is not None]
    header = SHARED / rel / "decls.h"
    header.parent.mkdir(parents=True, exist_ok=True)
    header.write_text(
        f"/* Declarations of smt3-decomp src/smt3/{rel}.c, for its bodies in this directory. */\n"
        f"#ifndef {guard}\n#define {guard}\n\n" + "\n\n".join(decls) + f"\n\n#endif\n")
    spans = definitions(text)
    for fn in functions:
        start, end = spans[fn]
        # Keep the comment directly above the definition, not the data before it.
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
        body = strip_static(body[:brace]) + body[brace:]
        path = SHARED / rel / f"{fn}.c"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(f'#include "shared/{rel}/decls.h"\n\n{body}\n')


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
    users = set()
    for src in (ROOT / "src" / version / "sdk").rglob("*.c"):
        if INCLUDE_BODY.search(src.read_text()):
            users.add(src.relative_to(ROOT / "src" / version).with_suffix("").as_posix())
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


def cmd_port(args):
    smt3 = args.smt3.expanduser().resolve()
    pairs = [r for r in read_pairs() if r["status"] == "identical" and r["function"] == r["smt3_name"]]
    if args.units:
        pairs = [r for r in pairs if any(r["src"].startswith(f"src/{r['version']}/{u}") for u in args.units)]
    carry = defaultdict(dict)    # version -> function -> rel
    for r in pairs:
        if outcomes(r["version"]).get(r["function"], ("c",))[0] == "asm":
            continue
        carry[r["version"]][r["function"]] = shared_rel(r["smt3_src"])
    by_rel = defaultdict(set)
    for funcs in carry.values():
        for fn, rel in funcs.items():
            by_rel[rel].add(fn)
    for rel, funcs in sorted(by_rel.items()):
        write_shared(smt3, rel, funcs)
    for version in VERSIONS:
        done = outcomes(version)
        changed = 0
        for src in sorted((ROOT / "src" / version).rglob("*.c")):
            text = src.read_text()
            unit = src.relative_to(ROOT / "src" / version).with_suffix("").as_posix()
            if args.units and not any(unit.startswith(u) for u in args.units):
                continue

            def to_c(m):
                fn = m.group(2)
                rel = carry[version].get(fn)
                return f'#include "shared/{rel}/{fn}.c"' if rel and m.group(1) == unit else m.group(0)

            def to_asm(m):
                fn = m.group(2)
                return m.group(0) if fn in carry[version] else f'INCLUDE_ASM(const s32, "{unit}", {fn});'

            new = INCLUDE_BODY.sub(to_asm, text)
            new = re.sub(r'^INCLUDE_ASM\(\s*const s32,\s*"([^"]+)"\s*,\s*(\w+)\s*\);$', to_c, new, flags=re.M)
            if new != text:
                src.write_text(new)
                changed += 1
        for fn in carry[version]:
            done.setdefault(fn, ("c", ""))
        write_outcomes(version, done)
        sdk_settings(smt3, version)
        print(f"{version}: {len(carry[version])} functions from C, {changed} units rewritten")
    remove_unused_bodies()


WORK = Path.home() / ".local/state/raidou-decomp-work/stage4"
ERROR = re.compile(r"^(\S+?):(\d+):(?:\d+:)? (?!warning)(.*)$")
FROM = re.compile(r"^(?:In file included|\s+) from (\S+?):\d+[:,]")


def known_symbols(version):
    """Name -> address in the all-asm link (saved before any C was carried)."""
    out = {}
    for line in (WORK / f"{version}.nm").read_text().splitlines():
        parts = line.split()
        if len(parts) == 3:
            out.setdefault(parts[2], int(parts[0], 16))
    return out


def compile_failures(log):
    """(version, function) -> message, for each body a compiler error is traced to."""
    out, unit_errors, version, chain = {}, defaultdict(list), None, []
    for line in log.read_text(errors="replace").splitlines():
        if line.startswith("FAILED: "):
            m = re.search(r"build/(raidou\d)/src/raidou\d/(\S+)\.o\b", line)
            version, unit = (m.group(1), m.group(2)) if m else (None, None)
            chain = []
            continue
        if m := FROM.match(line):
            chain.append(m.group(1))
            continue
        m = ERROR.match(line)
        if not m or version is None:
            if not line.startswith((" ", "In ")):
                chain = []
            continue
        path, msg = m.group(1), m.group(3).strip()
        bodies = [p for p in [path] + chain if INCLUDE_PATH.match(p)]
        if path.endswith("/decls.h") and path.startswith("src/shared/"):
            # A clash in one SMT3 file's declarations fails all of its bodies in the unit.
            out.setdefault((version, "decls:" + unit + ":" + path[len("src/shared/"):-len("/decls.h")]),
                           f"declarations clash: {msg}")
        elif bodies:
            fn = Path(bodies[-1] if path not in bodies else path).stem
            out.setdefault((version, fn), f"compile error: {msg}")
        else:
            unit_errors[(version, unit)].append(f"{path}:{m.group(2)}: {msg}")
        chain = []
    return out, unit_errors


INCLUDE_PATH = re.compile(r"^src/shared/.+/\w+\.c$")
R_26, R_HI16, R_LO16, R_GPREL16 = 4, 5, 6, 7


def check_object(version, obj, bodies, retail, segs, gp, asm_sizes, known):
    """Compare each carried body in obj with retail. Returns ({fn: reason},
    {fn: [(symbol, address)]})."""
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection
    from retail_elf import va_to_off

    fails, needs = {}, {}
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
                    reason = f"emits {sections.get(s['st_shndx'], s['st_shndx'])} data"
                    break
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
                    need.append((name, value - (((mine[h] & 0xFFFF) << 16) + sext16(mine[i]))))
                elif r_type == R_GPREL16:
                    need.append((name, gp + sext16(theirs[i]) - sext16(mine[i])))
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
    return fails, needs


def sext16(v):
    return ((v & 0xFFFF) ^ 0x8000) - 0x8000


def cmd_verify(args):
    from retail_elf import RETAIL, VERSIONS as INFO, load_segments

    compile_fails, unit_errors = compile_failures(args.log)
    for version in VERSIONS:
        retail = (ROOT / RETAIL[version]).read_bytes()
        segs = load_segments(retail)
        gp = int(INFO[version]["gp"], 16)
        known = known_symbols(version)
        done = outcomes(version)
        fails, needs = {}, {}
        for src in sorted((ROOT / "src" / version).rglob("*.c")):
            text = src.read_text()
            unit = src.relative_to(ROOT / "src" / version).with_suffix("").as_posix()
            bodies = {m.group(2): known[m.group(2)] for m in INCLUDE_BODY.finditer(text)}
            if not bodies:
                continue
            rels = {m.group(2): m.group(1) for m in INCLUDE_BODY.finditer(text)}
            for fn in bodies:
                if (version, fn) in compile_fails:
                    fails[fn] = compile_fails[(version, fn)]
                clash = compile_fails.get((version, f"decls:{unit}:{rels[fn]}"))
                if clash:
                    fails[fn] = clash
            if (version, unit) in unit_errors or any(fn in fails for fn in bodies):
                for fn in bodies:
                    if fn not in fails and (version, unit) in unit_errors:
                        fails[fn] = "unit compile error: " + unit_errors[(version, unit)][0]
                continue
            obj = ROOT / "build" / version / "src" / version / f"{unit}.o"
            if not obj.exists() or obj.stat().st_mtime < src.stat().st_mtime:
                for fn in bodies:
                    fails.setdefault(fn, "no object")
                continue
            sizes = {fn: len(parse_asm(function_asm(version, unit, fn))[fn][1])
                     for fn in bodies}
            for fn in bodies:
                # splat moves rodata only this function uses into its asm file and
                # drops it once the function is C, so the C must emit it.
                if re.search(r"^(dlabel|jlabel) ", function_asm(version, unit, fn).read_text(), re.M):
                    fails[fn] = "its rodata is in its asm file"
            f, n = check_object(version, obj, {k: v for k, v in bodies.items() if k not in fails},
                                retail, segs, gp, sizes, known)
            fails.update(f)
            needs.update(n)
        # Every name a body uses must name one Raidou address.
        users = defaultdict(set)
        want = defaultdict(set)
        for fn, need in needs.items():
            for name, address in need:
                users[name].add(fn)
                want[name].add(address)
        aliases = {}
        for name, addresses in want.items():
            if len(addresses) > 1:
                why = f"{name} at " + ", ".join(f"0x{a:08X}" for a in sorted(addresses))
            elif name in known and known[name] != next(iter(addresses)):
                why = f"{name} is 0x{known[name]:08X}, code wants 0x{next(iter(addresses)):08X}"
            else:
                if name not in known:
                    aliases[name] = next(iter(addresses))
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


def remove_unused_bodies():
    """Delete body files and declaration headers no Raidou unit includes."""
    used = set()
    for version in VERSIONS:
        for src in (ROOT / "src" / version).rglob("*.c"):
            used.update(f"{m.group(1)}/{m.group(2)}" for m in INCLUDE_BODY.finditer(src.read_text()))
    for body in SHARED.rglob("*.c"):
        if body.relative_to(SHARED).with_suffix("").as_posix() not in used:
            body.unlink()
    for decls in SHARED.rglob("decls.h"):
        if not any(decls.parent.glob("*.c")):
            decls.unlink()
            for d in [decls.parent, *decls.parent.parents]:
                if d == SHARED or any(d.iterdir()):
                    break
                d.rmdir()


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("pairs")
    p.add_argument("--smt3", type=Path, default=ROOT.parent / "smt3-decomp")
    p.set_defaults(func=cmd_pairs)
    p = sub.add_parser("port")
    p.add_argument("--smt3", type=Path, default=ROOT.parent / "smt3-decomp")
    p.add_argument("--units", nargs="*", help="only units under these paths (e.g. sdf sdk)")
    p.set_defaults(func=cmd_port)
    p = sub.add_parser("verify")
    p.add_argument("--log", type=Path, default=Path.home() / ".local/state/raidou-decomp-work/stage4/ninja.log")
    p.set_defaults(func=cmd_verify)
    args = ap.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
