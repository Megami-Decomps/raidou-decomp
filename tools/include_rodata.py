#!/usr/bin/env python3
"""Place INCLUDE_RODATA for the rodata splat could not give to one asm function.

    python3 tools/include_rodata.py [raidou1]

This also runs include_sdata.py's placement, and each unit is written once with
the result of both: rodata placement alone leaves sdata blocks mid-move, and a
shared unit re-merged from that view can change its VERSION_ guard layout. The
pair is idempotent and the tree is committed in its output.

With per-unit .rodata (tools/split_rodata.py) a unit's rodata is emitted in
source order: each INCLUDE_ASM brings the rodata splat migrated into that
function's asm file, and each C function brings what the compiler emits for it
(jump tables). Two kinds of rodata have no such home:

- rodata of functions that are already C but whose C does not yet produce it
  (strings and constants still referenced as extern D_ symbols); splat migrated
  it to that function (asm/<v>/function_rodata.tsv). It goes before the function.
  Once the C writes the literal itself (the D_ symbol no longer appears in the
  function), the compiler emits it and no line is needed.
- rodata several functions share, or that only .data tables point at; splat
  migrated it nowhere. It goes where its address
  falls: before the first function whose rodata comes after it, or at the end.
  A line that would land on the asm side (#else) of a NON_MATCHING block goes
  before the block instead, unless every function using it is a draft whose C
  emits it (tools/nm_rodata_placement.py checks this).
- named rodata defined by the C unit (including aligned vectors) stays at its
  definition. The compiler provides that symbol even if an asm function still
  references it, so no INCLUDE_RODATA may duplicate it.

The first two kinds get per-symbol files in asm/<v>/nonmatchings/<unit>/, cut from
asm/<v>/data/<unit>.rodata.s, and an INCLUDE_RODATA line. Placement is recomputed
on every run.
"""
import bisect
import re
import sys
from pathlib import Path

from units import carried_data, deferred, function_rodata, read_unit, rodata_blocks, unit_sources, write_unit

from include_sdata import DATA_DEF
from include_sdata import place as place_sdata

ROOT = Path(__file__).resolve().parent.parent
INCLUDE = re.compile(r'^INCLUDE_(?:ASM|RODATA)\([^,]+,\s*"[^"]+",\s*(\w+)\);$', re.M)
INCLUDE_ASM = re.compile(r'^INCLUDE_ASM\([^,]+,\s*"[^"]+",\s*(\w+)\);$', re.M)
DEF = re.compile(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\([^;{]*\)\s*\{?[ \t]*$", re.M)
LABEL = re.compile(r"^dlabel (\w+)\n\s*/\* [0-9A-F]+ ([0-9A-F]{8})", re.M)
JTBL_ALIGN = re.compile(r"^\.align 3\n(?=nonmatching jtbl_)", re.M)


def write_rodata_file(path, block):
    """A per-symbol rodata file as splat writes it (a jump table starts 16-aligned,
    as tools/resolve_jtbl_targets.py makes splat's)."""
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(".section .rodata\n\n" + JTBL_ALIGN.sub(".align 4\n", block))


def place(version):
    added = 0
    aliases = {m.group(1): int(m.group(2), 16) for m in re.finditer(
        r"^(\w+) = 0x([0-9A-Fa-f]+);", (ROOT / "config" / version / "smt3_symbols.txt").read_text(), re.M)}
    carried = carried_data(version)
    aliases.update(carried)
    migration = function_rodata(version)
    for unit, c in unit_sources(version).items():
        nonmatchings = ROOT / "asm" / version / "nonmatchings" / unit
        if unit not in migration:
            # Nothing of the unit was split (its lines are left over from a split).
            text = read_unit(version, c)
            stripped = re.sub(r'^INCLUDE_RODATA\([^,]+,\s*"[^"]+",\s*(\w+)\);\n\n?', "", text, flags=re.M)
            if stripped != text:
                write_unit(version, c, stripped)
            continue
        # The unit's rodata as splat last split it; INCLUDE_RODATA lines and per-symbol
        # files for anything else are left over from an earlier split.
        full = ROOT / "asm" / version / "data" / f"{unit}.rodata.s"
        labels = dict(LABEL.findall(full.read_text())) if full.exists() else {}
        owned = set(labels)
        unit_rodata = set(owned)
        blocks = rodata_blocks(version, unit)
        text = read_unit(version, c)
        asm_funcs = set(INCLUDE_ASM.findall(text))
        migrated = {f: syms for f, syms in migration[unit] if f}
        unmigrated = [s for f, syms in migration[unit] if not f for s in syms]
        # Symbols splat migrated into an asm function's own file come with its
        # INCLUDE_ASM and must not be included again.
        for f in asm_funcs & migrated.keys():
            owned -= set(migrated[f])
        # Placed from scratch every time: once a function compiles its own
        # literal, the lines around it must move with it.
        text = re.sub(r'^INCLUDE_RODATA\([^,]+,\s*"[^"]+",\s*(\w+)\);\n\n?', "", text, flags=re.M)
        defined_at = {
            m.group(1): m.start() for m in DATA_DEF.finditer(text)
            if m.group(1) in unit_rodata
        }
        # An SMT3 data definition carried into C stands for the label at its address.
        at_addr = {int(a, 16): n for n, a in labels.items()}
        for m in DATA_DEF.finditer(text):
            if m.group(1) in carried and (sym := at_addr.get(carried[m.group(1)])) in unit_rodata:
                defined_at.setdefault(sym, m.start())
        # Rodata a C function no longer names is compiled by that function itself
        # (a string literal): nothing to include.
        # The retail references come from the unit's full disassembly, which
        # also covers rodata several functions share (splat leaves that
        # standalone instead of in one function's file).
        full = ROOT / "asm" / version / f"{unit}.s"
        retail_refs = {}
        if full.exists():
            for f in re.finditer(r"^glabel (\w+)\n(.*?)^endlabel \1", full.read_text(), re.M | re.S):
                retail_refs[f.group(1)] = set(re.findall(r"%lo\((\w+)\)", f.group(2)))
        bodies = {m.group(1): function_text(text, m.start()) for m in DEF.finditer(text)}

        # A source-local macro can name an external rodata symbol just like a
        # private helper. Join continuation lines before following its tokens;
        # do not mistake those references for literals emitted by the compiler.
        def code_tokens(code):
            # Macro names inside comments or literals are not expanded by C.
            return re.sub(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
                          " ", code, flags=re.S)

        macro_text = code_tokens(re.sub(r"\\\n", "", text))
        macros = {
            name: (bool(params), body)
            for name, params, body in re.findall(
                r"^[ \t]*#[ \t]*define[ \t]+(\w+)(?:(\([^\n]*?\))[ \t]*|[ \t]+)([^\n]*)",
                macro_text, re.M)
        }

        by_addr = sorted((int(a, 16), s) for s, a in labels.items())

        def code_refs(code, visited):
            code = code_tokens(code)
            tokens = set(re.findall(r"\b[A-Za-z_]\w*\b", code))
            calls = set(re.findall(r"\b(\w+)\s*\(", code))
            refs = unit_rodata.intersection(tokens)
            # C carried from SMT3 names Raidou data by SMT3's name (smt3_symbols.txt).
            for token in tokens & aliases.keys():
                i = bisect.bisect_right(by_addr, (aliases[token], "\uffff")) - 1
                end = by_addr[i + 1][0] if i + 1 < len(by_addr) else (by_addr[-1][0] + 0x100 if by_addr else 0)
                if i >= 0 and aliases[token] < end:
                    refs.add(by_addr[i][1])
            for token in tokens:
                if token in macros and ("macro", token) not in visited:
                    function_like, body = macros[token]
                    if function_like and token not in calls:
                        continue
                    visited.add(("macro", token))
                    refs.update(code_refs(body, visited))
            # A private inline helper has no separate native function entry.
            # Follow those calls, including calls inside macros, without changing
            # another retail function's ownership. Tag visited nodes so cycles
            # through either kind of source-local indirection terminate.
            for callee in calls:
                if callee in bodies and callee not in retail_refs:
                    refs.update(named_refs(callee, visited))
            return refs

        def named_refs(name, visited):
            if ("function", name) in visited:
                return set()
            visited.add(("function", name))
            return code_refs(bodies[name], visited)

        compiled = set()
        for name in bodies:
            refs = named_refs(name, set())
            for sym in retail_refs.get(name, ()):
                if (sym.startswith(("D_", "jtbl_")) or sym in unit_rodata) and sym not in refs:
                    compiled.add(sym)
        # Only this unit's own rodata can be compiled here: other names a C
        # function writes differently (D_X[9] for retail's D_Y) are other data.
        compiled &= unit_rodata
        compiled -= defined_at.keys()
        # ... unless an asm function still needs the retail copy (check_unit SHARED).
        for f in INCLUDE_ASM.findall(text):
            compiled -= retail_refs.get(f, set())
        # A string that only a compiled initializer table points at (`.word` entries of
        # a table the C now writes as `{"..", ..}`) is compiled with that table: no
        # function names it and no other rodata or data word holds it.
        # A holder in a .data chunk needs only the label's address, which
        # symbol_addrs.txt gives the link.
        asm_named = set().union(*retail_refs.values()) if retail_refs else set()
        holders = word_holders(version)
        grown = True
        while grown:
            grown = False
            for sym in unit_rodata - compiled - set(defined_at):
                held = holders.get(sym, set()) - data_chunk_labels(version)
                if held and held <= compiled and sym not in asm_named \
                        and not re.search(rf"\b{sym}\b", text):
                    compiled.add(sym)
                    grown = True
        # The compiler emits each such literal with the first function using it.
        compiled_at = {}
        for m in DEF.finditer(text):
            for sym in retail_refs.get(m.group(1), ()):
                if sym in compiled:
                    compiled_at.setdefault(sym, m.start())
        owned -= compiled
        owned -= defined_at.keys()
        have = set(INCLUDE.findall(text))
        anchors = []   # (address, position in text or None, symbol, needs a line)
        for sym, pos in defined_at.items():
            anchors.append((int(labels[sym], 16), pos, None, False))
        for m in INCLUDE_ASM.finditer(text):
            for sym in migrated.get(m.group(1), ()):
                if sym in labels:
                    anchors.append((int(labels[sym], 16), m.start(), None, False))
        for m in DEF.finditer(text):
            if m.group(1) in asm_funcs:
                continue
            for sym in migrated.get(m.group(1), ()):
                if sym not in blocks:
                    continue
                addr = int(LABEL.search(blocks[sym]).group(2), 16)
                if sym in defined_at:
                    continue  # the named C definition is the address anchor
                if sym.startswith("jtbl_") or sym in compiled:
                    anchors.append((addr, m.start(), None, False))  # the C emits it
                    continue
                out = nonmatchings / f"{sym}.s"
                if not out.exists():
                    # A unit whose functions are all C still includes the rodata they name.
                    write_rodata_file(out, blocks[sym])
                # Placed by address like any other included symbol: a function may
                # use rodata the compiler emitted after later functions.
                anchors.append((addr, None, sym, sym not in have))
        for sym, pos in compiled_at.items():
            if not any(a[2] == sym for a in anchors):
                anchors.append((int(labels[sym], 16), pos, None, False))
        # splat writes a file for unmigrated rodata only once a line includes it.
        anchored = {a[2] for a in anchors}
        for sym in unmigrated:
            out = nonmatchings / f"{sym}.s"
            if sym in owned and sym not in anchored and sym in blocks and not out.exists():
                write_rodata_file(out, blocks[sym])
        for s in sorted(s for s in nonmatchings.glob("*.s") if "glabel " not in s.read_text()):
            align_file(s)
            m = LABEL.search(s.read_text())
            if m and m.group(1) in owned and not any(sym == m.group(1) for _, _, sym, _ in anchors):
                anchors.append((int(m.group(2), 16), None, m.group(1), m.group(1) not in have))
        anchors.sort(key=lambda a: (a[0], a[1] is None, a[1] if a[1] is not None else -1, a[2] or ""))
        # Floating symbols take the position of the next anchored one. When that is
        # on the asm side (#else) of a NON_MATCHING block, a -DNON_MATCHING build
        # drops the line, so it stays there only if that build needs no copy:
        # every function using the symbol is a draft whose C emits it. Otherwise
        # it goes before the block.
        drafts = nm_asm_functions(text)

        def nm_build_needs(sym):
            users = {f for f, refs in retail_refs.items() if sym in refs}
            return not users or any(
                f not in drafts or f not in bodies or sym in named_refs(f, set()) for f in users)

        following = len(text)
        placed = []
        for addr, pos, sym, needed in reversed(anchors):
            if pos is None:
                pos = asm_start(text, following) if nm_build_needs(sym) else following
            following = pos
            placed.append((pos, addr, sym, needed))
        positions = [pos for pos, *_ in sorted(placed, key=lambda p: p[1])]
        if positions != sorted(positions):
            print(f"warning: {c.relative_to(ROOT)}: rodata order does not follow function order")
        inserts = {}
        for pos, addr, sym, needed in sorted(placed, key=lambda p: p[1]):
            if needed:
                inserts.setdefault(pos, []).append(sym)
        for pos in sorted(inserts, reverse=True):
            lines = "".join(f'INCLUDE_RODATA(const s32, "{unit}", {s});\n\n' for s in inserts[pos])
            text = text[:pos] + ("\n" if pos == len(text) else "") + lines + text[pos:]
            added += len(inserts[pos])
        # One blank line before each included item, however often this reruns.
        text = re.sub(r"\n{3,}(?=INCLUDE_(?:RODATA|SDATA)\()", "\n\n", text)
        write_unit(version, c, text)
    print(f"{version}: {added} INCLUDE_RODATA lines")


def align_file(path):
    """Give a standalone rodata file the alignment its address allows (at most
    8, or 16 for a jump table). ee-gcc aligned every literal it emitted; once a
    neighbour is compiled from C, the included data must realign exactly as
    the original did. At the retail address this adds no padding."""
    text = path.read_text()
    m = LABEL.search(text)
    if not m or re.search(r"^\.align \d+\nnonmatching", text, re.M):
        return
    addr = int(m.group(2), 16)
    cap = 4 if m.group(1).startswith("jtbl_") else 3
    k = next(k for k in range(cap, -1, -1) if addr % (1 << k) == 0)
    path.write_text(re.sub(r"^nonmatching", f".align {k}\nnonmatching", text, count=1, flags=re.M))


_WORD_HOLDERS = {}


def word_holders(version):
    """Map each symbol named by a `.word` entry anywhere in the version's data to
    the set of data symbols whose block holds that entry."""
    if version not in _WORD_HOLDERS:
        holders = {}
        for path in (ROOT / "asm" / version / "data").rglob("*.s"):
            current = None
            for line in path.read_text().splitlines():
                m = re.match(r"\s*dlabel (\w+)", line)
                if m:
                    current = m.group(1)
                    continue
                m = re.search(r"\.word (\w+)\s*$", line)
                if m and current and not m.group(1).startswith("0x"):
                    holders.setdefault(m.group(1), set()).add(current)
        _WORD_HOLDERS[version] = holders
    return _WORD_HOLDERS[version]


_DATA_CHUNK_LABELS = {}


def data_chunk_labels(version):
    """Labels of the version's asm .data chunks (asm/<v>/data/data_*.data.s)."""
    if version not in _DATA_CHUNK_LABELS:
        _DATA_CHUNK_LABELS[version] = {
            m for path in (ROOT / "asm" / version / "data").glob("data_*.data.s")
            for m in re.findall(r"^\s*dlabel (\w+)", path.read_text(), re.M)}
    return _DATA_CHUNK_LABELS[version]


PREPROCESSOR = re.compile(r"^[ \t]*#[ \t]*(if\w*|else|elif|endif)\b(.*)$", re.M)


def nm_asm_functions(text):
    """Functions included as asm on the #else side of a NON_MATCHING block."""
    return {m.group(1) for m in INCLUDE_ASM.finditer(text) if asm_start(text, m.start()) != m.start()}


def asm_start(text, pos):
    """`pos`, or the head of the NON_MATCHING block whose asm side (#else) holds it,
    with the `/* NON_MATCHING` comment line above it."""
    stack = []   # [start, is a NON_MATCHING block, past its #else]
    for m in PREPROCESSOR.finditer(text, 0, pos):
        if m.group(1).startswith("if"):
            stack.append([m.start(), m.group(1) == "ifdef" and m.group(2).split() == ["NON_MATCHING"], False])
        elif m.group(1) == "endif":
            if stack:
                stack.pop()
        elif stack:
            stack[-1][2] = True
    start = next((s for s, nm, asm in stack if nm and asm), None)
    if start is None:
        return pos
    comment = text.rfind("\n", 0, start - 1) + 1
    return comment if start and text.startswith("/* NON_MATCHING", comment) else start


def function_text(text, start):
    """Text of the C function definition starting at `start`."""
    depth, i = 0, text.index("{", start)
    for j in range(i, len(text)):
        depth += {"{": 1, "}": -1}.get(text[j], 0)
        if depth == 0:
            return text[start:j + 1]
    return text[start:]


if __name__ == "__main__":
    for v in sys.argv[1:] or ["raidou1"]:
        with deferred():
            place(v)
            place_sdata(v)
