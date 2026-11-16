#!/usr/bin/env python3
"""Run decomp-permuter on a draft compiled inside its real unit, for one or both games.

    tools/perm_unit.py import src/shared/effect/billManager.c dds3Billboard_P2ADraw [draft.c]
    tools/perm_unit.py import --all-under 10 --batch D    # every nonmatch in D/results.tsv under 10 words
    python3 ../decomp-permuter/permuter.py build/permuter/shared_dds3Billboard_P2ADraw -j8 --best-only --stop-on-zero
    tools/perm_unit.py scan build/permuter/shared_dds3Billboard_P2ADraw   # keep or reject each output-*

`import` writes <batch>/permuter/<shared|raidou1|raidou2>_<func>/ with --batch, else
build/permuter/ (which configure --clean removes). Opt-in: regular batches do not run it. Without a draft file it
takes the unit's own NON_MATCHING draft of the function. Each candidate replaces the
function's INCLUDE_ASM (or its NON_MATCHING block) in the unit and is compiled with
tools/cc.sh under the unit's name and flags, reduced to an object holding only that
function. A shared unit is compiled for both games and scored on both dumps, so 0 means
both match. Only passes that keep source ordinary run (NEAT_WEIGHTS); a hit is a hint
to reduce and verify with the build. Needs pycparser and toml for `import`, and a
decomp-permuter checkout (DECOMP_PERMUTER, else ../decomp-permuter, else ~/projects).
"""
from __future__ import annotations

import argparse
import base64
import importlib.util
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = ("raidou1", "raidou2")
AS = ROOT / "tools/compilers/ee-gcc2.96/ee/bin/as"
OBJDUMP = ROOT / "tools/bin/mips-ps2-decompals-objdump"

NEAT_WEIGHTS = {
    "perm_temp_for_expr": 40, "perm_expand_expr": 20, "perm_reorder_stmts": 15,
    "perm_reorder_decls": 10, "perm_cast_simple": 8, "perm_refer_to_var": 5,
    "perm_float_literal": 1, "perm_randomize_internal_type": 12, "perm_split_assignment": 10,
    "perm_ins_block": 4, "perm_struct_ref": 8, "perm_condition": 6, "perm_commutative": 10,
    "perm_add_sub": 3, "perm_inequalities": 5, "perm_compound_assignment": 5,
    "perm_chain_assignment": 5, "perm_long_chain_assignment": 2, "perm_inline": 3,
    "perm_remove_var": 6, "perm_reverse_cond": 6,
    # Never landable: masks, xor/mult zero, dummy commas, self/duplicate
    # assignments, padding variables, extern/prototype type changes, ...
    "perm_add_mask": 0, "perm_xor_zero": 0, "perm_randomize_external_type": 0,
    "perm_randomize_function_type": 0, "perm_sameline": 0, "perm_empty_stmt": 0,
    "perm_mult_zero": 0, "perm_factor_mult": 0, "perm_factor_shift": 0,
    "perm_dummy_comma_expr": 0, "perm_add_self_assignment": 0, "perm_remove_ast": 0,
    "perm_duplicate_assignment": 0, "perm_pad_var_decl": 0, "perm_var_cond_block": 0,
    "perm_alias_array": 0,
}


def find_def(text: str, fn: str) -> tuple[int, int]:
    """(start, end) of fn's definition; start is at the start of its line."""
    for m in re.finditer(r"^[^\n;{}#]*?\b" + re.escape(fn) + r"\s*\(", text, re.M):
        i, depth = m.end(), 1
        while depth:
            depth += {"(": 1, ")": -1}.get(text[i], 0)
            i += 1
        while text[i] in " \t\n":
            i += 1
        if text[i] != "{":
            continue  # a prototype
        depth = 0
        while True:
            depth += {"{": 1, "}": -1}.get(text[i], 0)
            if depth == 0:
                break
            i += 1
        i += 1
        if i < len(text) and text[i] == "\n":
            i += 1
        return m.start(), i
    raise SystemExit(f"no definition of {fn}")


def skip_literal(t: str, i: int) -> int:
    q, i = t[i], i + 1
    while t[i] != q:
        i += 2 if t[i] == "\\" else 1
    return i + 1


def prototypes_only(t: str, body: str) -> str:
    """t with other functions' definitions turned into prototypes, except inline
    helpers and functions body names (an earlier definition makes calls to a
    function no-throw, which changes delay-slot filling)."""
    used = set(re.findall(r"\b[A-Za-z_]\w*\b", body))
    i, depth, stmt = 0, 0, 0
    while i < len(t):
        c = t[i]
        if t.startswith("//", i):
            i = t.index("\n", i)
            continue
        if t.startswith("/*", i):
            i = t.index("*/", i) + 2
            continue
        if c in "\"'":
            i = skip_literal(t, i)
            continue
        if c == "#" and depth == 0 and (i == 0 or t[i - 1] == "\n"):
            while True:
                j = t.index("\n", i)
                if t[j - 1] != "\\":
                    break
                i = j + 1
            i = stmt = j + 1
            continue
        if c == "{" and depth == 0:
            head = re.sub(r"/\*.*?\*/|//[^\n]*", "", t[stmt:i], flags=re.S).strip()
            if head.endswith(")") and "=" not in head.split("(")[0]:
                names = [w for w in re.findall(r"(\w+)\s*\(", head) if w != "__attribute__"]
                j, d = i, 0
                while True:
                    if t[j] in "\"'":
                        j = skip_literal(t, j)
                        continue
                    if t.startswith("/*", j):
                        j = t.index("*/", j) + 2
                        continue
                    if t.startswith("//", j):
                        j = t.index("\n", j)
                        continue
                    d += {"{": 1, "}": -1}.get(t[j], 0)
                    if d == 0:
                        break
                    j += 1
                keep = not names or names[0] in used or re.search(r"\binline\b|__inline", head)
                if keep:
                    i = stmt = j + 1
                else:
                    pre = t[:i].rstrip()
                    t = pre + ";\n" + t[j + 1:]
                    i = stmt = len(pre) + 2
                continue
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                stmt = i + 1
        elif c == ";" and depth == 0:
            stmt = i + 1
        i += 1
    return t


def hide_asm(src: str) -> str:
    """Each `__asm__ [volatile] (...)` becomes a permuter b64literal pragma, which
    pycparser accepts and the permuter prints back verbatim."""
    out, i = [], 0
    for m in re.finditer(r"\b__asm__\b", src):
        if m.start() < i:
            continue
        mm = re.match(r"\s*(volatile\s*)?\(", src[m.end():])
        if not mm:
            continue
        k, depth = m.end() + mm.end(), 1
        while depth:
            if src[k] in "\"'":
                k = skip_literal(src, k)
                continue
            depth += {"(": 1, ")": -1}.get(src[k], 0)
            k += 1
        while src[k] in " \t":
            k += 1
        if src[k] == ";":
            k += 1
        text = src[m.start():k].replace("\n", " ")
        out += [src[i:m.start()], "\n#pragma _permuter b64literal " + base64.b64encode(text.encode()).decode() + "\n"]
        i = k
    return "".join(out + [src[i:]])


def strip_other_code(s: str, fn: str) -> str:
    """A unit's cc1 assembly without other functions' code (compiled bodies and
    INCLUDE_ASM includes); data, .extern and section state are kept."""
    out, skip = [], None
    for line in s.split("\n"):
        m = re.fullmatch(r"\s*\.ent\s+(\S+)", line)
        if m and m.group(1) != fn:
            skip = m.group(1)
            continue
        if skip:
            if re.fullmatch(r"\s*\.end\s+" + re.escape(skip), line):
                skip = None
            continue
        m = re.search(r'\.include\s+"build/eeasm/asm/[^"]*/nonmatchings/[^"]*/([^/"]+)\.s"', line)
        if m and m.group(1) != fn and not m.group(1).startswith(("jtbl_", "D_")):
            continue
        out.append(line)
    return "\n".join(out) + "\n"


def asm_site(unit: str, fn: str) -> tuple[int, int, str, str | None]:
    """(start, end, INCLUDE_ASM line, NON_MATCHING draft or None) of fn's fallback in the unit.
    A fallback in `#ifdef NON_MATCHING draft #else INCLUDE_ASM #endif` spans the whole block."""
    m = re.search(r'^[ \t]*INCLUDE_ASM\([^,]+,\s*"[^"]+",\s*' + re.escape(fn) + r"\);[ \t]*\n", unit, re.M)
    if not m:
        raise SystemExit(f"{fn} is not INCLUDE_ASM in the unit")
    line = m.group(0).strip()
    head = unit[:m.start()]
    tail = re.match(r"[ \t]*#\s*endif[^\n]*\n?", unit[m.end():])
    els = re.search(r"^[ \t]*#\s*else[^\n]*\n\Z", head, re.M)
    ifdef = None
    for ifdef in re.finditer(r"^[ \t]*#\s*ifdef\s+NON_MATCHING[^\n]*\n", head, re.M):
        pass
    if tail and els and ifdef:
        return ifdef.start(), m.end() + tail.end(), line, head[ifdef.end():els.start()]
    return m.start(), m.end(), line, None


def splice(unit: str, draft: str | None, fn: str) -> tuple[str, str, str]:
    """(template, full source, the INCLUDE_ASM line) for fn's draft in its unit.
    The template holds @@PERM@@ where a candidate's definition goes."""
    start, end, line, own = asm_site(unit, fn)
    draft = draft if draft is not None else own
    if draft is None:
        raise SystemExit(f"no draft of {fn}: pass one, or keep it as a NON_MATCHING draft in the unit")
    da, db = find_def(draft, fn)
    template = unit[:start] + draft[:da] + "@@PERM@@\n" + draft[db:] + unit[end:]
    return template, unit[:start] + draft + unit[end:], line


def unit_versions(unit: str) -> tuple[str, ...]:
    """The games a unit path (relative to the repository) builds."""
    top = unit.split("/")[1] if unit.startswith("src/") else ""
    if top == "shared":
        return VERSIONS
    if top in VERSIONS:
        return (top,)
    raise SystemExit(f"{unit} is not a unit under src/shared/ or src/<version>/")


def unit_asflags(version: str, unit: str) -> list[str]:
    """Assembler flags cc.sh gives the unit's compiler: -G0 for the 2.9 SDK compilers."""
    rel = unit.split("/", 2)[2].removesuffix(".c")
    compiler = "ee-gcc2.96"
    table = ROOT / "config" / version / "compilers.txt"
    for row in table.read_text().splitlines() if table.exists() else ():
        f = row.split()
        if len(f) >= 2 and f[0] == rel:
            compiler = f[1]
    return ["-EL", "-G8", "-g"] if compiler == "ee-gcc2.96" else ["-EL", "-G0"]


def permuter_dir() -> Path:
    if "DECOMP_PERMUTER" in os.environ:
        return Path(os.environ["DECOMP_PERMUTER"]).resolve()
    near = ROOT.parent / "decomp-permuter"
    return (near if near.exists() else Path.home() / "projects/decomp-permuter").resolve()


def out_root(batch: str | None) -> Path:
    return Path(batch).resolve() / "permuter" if batch else ROOT / "build/permuter"


def cmd_import(a: argparse.Namespace) -> None:
    unit = Path(a.unit).resolve().relative_to(ROOT).as_posix()
    versions = unit_versions(unit)
    if a.version:
        if a.version not in versions:
            raise SystemExit(f"{unit} does not build {a.version}")
        versions = (a.version,)
    fn = a.func
    tag = unit.split("/")[1] if len(versions) > 1 else versions[0]
    out = Path(a.out or out_root(a.batch) / f"{tag}_{fn}").resolve()
    out.mkdir(parents=True, exist_ok=True)
    template, full, include_asm = splice((ROOT / unit).read_text(), Path(a.draft).read_text() if a.draft else None, fn)
    body = full[slice(*find_def(full, fn))]
    if len(versions) > 1 and re.search(r"\bVERSION_RAIDOU", body):
        raise SystemExit(f"{fn} has per-game code; run each game alone with --version")
    (out / "template.c").write_text(template)
    (out / "meta.json").write_text(json.dumps({"versions": versions, "unit": unit, "func": fn,
                                               "include_asm": include_asm}))

    cpp = ["cpp", "-P", "-undef", "-nostdinc", "-DPERMUTER", "-D__GNUC__=2", "-D__GNUC_MINOR__=96",
           "-Dmips", "-D__mips__", "-D_R5900", "-D__R5900__", "-D_LANGUAGE_C", "-DLANGUAGE_C",
           "-D__SIZE_TYPE__=unsigned int", "-D__PTRDIFF_TYPE__=int", f"-DVERSION_{versions[0].upper()}",
           f"-I{ROOT / 'include'}", f"-I{ROOT / 'src'}", f"-I{(ROOT / unit).parent}",
           "-D_Static_assert(x, y)=", "-D__attribute__(x)=", "-D__inline__=inline", "-D__inline=inline",
           "-D__volatile__=volatile", "-D__const=const", "-D__extension__=", "-D__restrict=",
           "-D__signed__=signed", "-Dasm=__asm__", "-"]
    src = hide_asm(subprocess.run(cpp, input=full, capture_output=True, text=True, check=True).stdout)
    perm = permuter_dir()
    spec = importlib.util.spec_from_file_location("permuter_import", perm / "import.py")
    imp = importlib.util.module_from_spec(spec)
    sys.path.insert(0, str(perm))
    spec.loader.exec_module(imp)
    base, _ = imp.prune_source(src, True, fn)
    from src.ast_util import process_pragmas
    (out / "base.c").write_text(base)

    me = Path(__file__).resolve()
    (out / "compile.sh").write_text(f'#!/bin/sh\nexec "{sys.executable}" "{me}" cc "{out}" "$1" "$3"\n')
    (out / "compile.sh").chmod(0o755)
    subprocess.run([sys.executable, me, "cc", out, "TARGET", out / "target.o"], check=True)

    fast = prototypes_only(template, body)
    dumps = []
    # base.c keeps parser-safe asm pragmas; validate the decoded source that
    # the permuter actually sends to the compiler, not ignored GCC pragmas.
    with tempfile.TemporaryDirectory(dir=out) as t:
        check_source = Path(t) / "base.c"
        check_source.write_text(process_pragmas(base))
        for name in ("fast", "full"):
            if name == "fast":
                (out / "template_fast.c").write_text(fast)
            else:
                (out / "template_fast.c").unlink()
            subprocess.run([out / "compile.sh", check_source, "-o", out / f"check_{name}.o"], check=True)
            dumps.append(dump_games(out / f"check_{name}.o")[3:])
    if dumps[0] == dumps[1] and len(dumps[0]) > 3:
        (out / "template_fast.c").write_text(fast)
    print("prototype-only unit:", "same code, used" if dumps[0] == dumps[1] else "differs, full unit used")

    settings = [f'func_name = "{fn}"', 'compiler_type = "gcc"',
                f'objdump_command = "{sys.executable} {me} objdump"', "", "[weight_overrides]"]
    settings += [f"{k} = {v}" for k, v in NEAT_WEIGHTS.items()]
    (out / "settings.toml").write_text("\n".join(settings) + "\n")
    print(out)


def batch_nonmatches(batch: Path, under: int) -> list[tuple[str, str, Path]]:
    """(unit, function, draft) for each results.tsv row of a batch directory recorded "nonmatch N" with N < under."""
    sys.path.insert(0, str(ROOT / "tools" / "batch"))
    import batch_files
    b = batch_files.load(batch)
    return [(row.source, row.name, batch / f"{row.name}.c") for row in b.rows.values()
            if row.nonmatch is not None and row.nonmatch < under]


def cmd_import_all(a: argparse.Namespace) -> None:
    dirs, failed = [], []
    for unit, fn, draft in batch_nonmatches(Path(a.batch).resolve(), a.all_under):
        if not (ROOT / unit).exists() or not draft.exists():
            failed.append(f"{fn}: no {unit if not (ROOT / unit).exists() else draft}")
            continue
        one = argparse.Namespace(unit=str(ROOT / unit), func=fn, draft=str(draft), version=a.version, out=None,
                                 batch=a.batch)
        try:
            try:
                cmd_import(one)
            except ImportError as e:
                raise SystemExit(f"perm_unit: {e}; import needs pycparser and toml") from e
            tag = unit.split("/")[1] if len(unit_versions(unit)) > 1 and not a.version else (a.version or
                                                                                          unit_versions(unit)[0])
            dirs.append(out_root(a.batch) / f"{tag}_{fn}")
        except (SystemExit, subprocess.CalledProcessError, ValueError) as e:
            failed.append(f"{fn}: {e}")
    for d in dirs:
        print(f"timeout 600 {sys.executable} {permuter_dir() / 'permuter.py'} {d} -j4 --best-only "
              "--stop-on-zero --quiet")
    for line in failed:
        print(f"not imported: {line}", file=sys.stderr)
    if failed:
        raise SystemExit(1)


def cmd_cc(a: argparse.Namespace) -> None:
    """Compile the candidate (or TARGET, the INCLUDE_ASM body) for each game: the first
    game's object is the output, the others go beside it as <output>.<game>."""
    d = Path(a.dir)
    meta = json.loads((d / "meta.json").read_text())
    fn = meta["func"]
    if a.input == "TARGET":
        body = meta["include_asm"] + "\n"
        tpl = d / "template.c"
    else:
        c = Path(a.input).read_text()
        ca, cb = find_def(c, fn)
        body = c[ca:cb]
        tpl = d / "template_fast.c" if (d / "template_fast.c").exists() else d / "template.c"
    src = tpl.read_text().replace("@@PERM@@\n", body, 1)
    output = Path(a.output).resolve()
    for i, version in enumerate(meta["versions"]):
        obj = output if i == 0 else Path(f"{output}.{version}")
        with tempfile.TemporaryDirectory(dir=d) as t:
            (Path(t) / "u.c").write_text(src)
            env = {**os.environ, "RAIDOU_VERSION": version, "RAIDOU_AS_UNIT": meta["unit"],
                   "RAIDOU_KEEP_S": f"{t}/u.s"}
            r = subprocess.run([ROOT / "tools/cc.sh", f"{t}/u.c", "-o", f"{t}/u.o"], env=env, cwd=ROOT,
                               capture_output=True, text=True)
            if r.returncode:
                sys.exit(r.stderr[-2000:])
            (Path(t) / "f.s").write_text(strip_other_code((Path(t) / "u.s").read_text(), fn))
            r = subprocess.run([AS, *unit_asflags(version, meta["unit"]), "-Iinclude", "-o", obj, f"{t}/f.s"],
                               cwd=ROOT, capture_output=True, text=True)
            if r.returncode:
                sys.exit(r.stderr[-2000:])


def dump_games(o: Path) -> list[str]:
    """dump() of o and of each other game's object beside it, which is then removed
    unless it is the target's (the permuter dumps a candidate once)."""
    out = dump(o)
    for version in VERSIONS:
        side = Path(f"{o}.{version}")
        if side.exists():
            out += [f"# {version}"] + dump(side)
            if o.name != "target.o":
                side.unlink()
    return out


def dump(o: Path) -> list[str]:
    """objdump -dr with relocations folded into their instructions and symbol
    spelling normalized (retail asm says jtbl_/absolute $gp offsets where cc1
    output has section-relative relocations)."""
    lines = subprocess.run([OBJDUMP, "-dr", o], capture_output=True, text=True, check=True).stdout.split("\n")
    out: list[str] = []
    for line in lines:
        m = re.match(r"\s+[0-9a-f]+: (R_MIPS_\w+)\s+(\S+)", line)
        if m and out:
            kind, sym = m.groups()
            if kind == "R_MIPS_26":
                out[-1] = re.sub(r"[0-9a-f]+ <[^>]*>$|[0-9a-f]+$", sym.split("+")[0], out[-1])
            elif kind in ("R_MIPS_HI16", "R_MIPS_LO16", "R_MIPS_GPREL16"):
                out[-1] = re.sub(r"(,|\t)(-?(0x)?[0-9a-f]+)(\(|$)", r"\1<sym>\4", out[-1])
            continue
        line = re.sub(r"-?\d+\(gp\)", "<sym>(gp)", line)
        # Upper-case sp/s8 so the scorer's stack wildcards never apply: a zero
        # score must mean the frame and local offsets match too.
        out.append(re.sub(r"\b(sp|s8)\b", lambda m: m.group(1).upper(), line))
    return out


DECL = re.compile(r"^[ \t]+(?:register\s+|const\s+|volatile\s+|unsigned\s+|signed\s+)*(?:struct\s+)?[A-Za-z_]\w*"
                  r"[\s*]+([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)?(?:=\s*([^;]+))?;", re.M)
NOT_TYPES = {"return", "goto", "break", "continue", "case", "else", "do"}
CAST = r"(?:\(\s*[A-Za-z_][\w\s*]*\)\s*)*"
CONSTANT = re.compile(rf"^{CAST}[-~]?\s*(?:0x[0-9A-Fa-f]+|\d+(?:\.\d*)?)[uUlLfF]*$")
COPY = re.compile(rf"^{CAST}[A-Za-z_]\w*$")


def local_names(body: str) -> dict[str, list[str]]:
    """Locals declared in a function body, each with its initialiser if it has one."""
    out: dict[str, list[str]] = {}
    for m in DECL.finditer(body):
        first = m.group(0).split()[0]
        if first not in NOT_TYPES:
            out.setdefault(m.group(1), []).extend([m.group(2).strip()] if m.group(2) else [])
    return out


def fake_locals(base: str, cand: str, fn: str) -> list[str]:
    """Locals cand adds to fn that are only ever set to a constant or copied from one variable.

    Such a local changes register allocation without being code anyone would write.
    """
    old = local_names(base[slice(*find_def(base, fn))])
    body = cand[slice(*find_def(cand, fn))]
    out = []
    for name, init in local_names(body).items():
        if name in old:
            continue
        rhs = init + [r.strip() for r in re.findall(rf"\b{re.escape(name)}\s*=(?!=)\s*([^;]+);", body)]
        if rhs and all(CONSTANT.match(r) for r in rhs):
            out.append(f"{name} (constant only: {rhs[0]})")
        elif rhs and all(COPY.match(r) or CONSTANT.match(r) for r in rhs):
            out.append(f"{name} (copy only: {rhs[0]})")
    return out


def cmd_scan(a: argparse.Namespace) -> None:
    """List each output-* of the permuter directories, rejecting those that add a fake local."""
    for d in map(Path, a.dirs):
        fn = re.search(r'func_name = "(\w+)"', (d / "settings.toml").read_text()).group(1)
        base = (d / "base.c").read_text()
        outs = sorted(d.glob("output-*/source.c"), key=lambda p: [int(x) for x in re.findall(r"\d+", p.parent.name)])
        kept = 0
        for src in outs:
            bad = fake_locals(base, src.read_text(), fn)
            kept += not bad
            print(f"{'reject' if bad else 'keep  '} {src.parent}" + (f": {', '.join(bad)}" if bad else ""))
        print(f"# {d.name}: {kept} of {len(outs)} outputs kept", file=sys.stderr)


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("import", help="set up a permuter directory for a draft")
    p.add_argument("unit", nargs="?")
    p.add_argument("func", nargs="?")
    p.add_argument("draft", nargs="?", help="file holding the draft; default the unit's NON_MATCHING draft")
    p.add_argument("--all-under", type=int, metavar="N",
                   help="import every results.tsv nonmatch under N words of --batch, its draft D/NAME.c")
    p.add_argument("--batch", help="batch directory: --all-under reads it, outputs go under its permuter/")
    p.add_argument("--version", choices=VERSIONS, help="only this game of a shared unit")
    p.add_argument("-o", "--out")
    p = sub.add_parser("scan", help="reject permuter outputs that add a constant-only or copy-only local")
    p.add_argument("dirs", nargs="+", help="permuter directories (holding base.c and output-*/)")
    p = sub.add_parser("cc", help="compile hook used by the directory's compile.sh")
    p.add_argument("dir")
    p.add_argument("input")
    p.add_argument("output")
    p = sub.add_parser("objdump", help="dump hook used by the directory's settings.toml")
    p.add_argument("args", nargs="+")
    a = ap.parse_args()
    if a.cmd == "import" and a.all_under is not None:
        if not a.batch or a.unit:
            ap.error("--all-under takes --batch D and no unit or function")
        cmd_import_all(a)
    elif a.cmd == "import":
        if not (a.unit and a.func):
            ap.error("import needs a unit and a function, or --all-under N --batch D")
        cmd_import(a)
    elif a.cmd == "scan":
        cmd_scan(a)
    elif a.cmd == "cc":
        cmd_cc(a)
    else:
        print("\n".join(dump_games(Path(a.args[-1]))))


if __name__ == "__main__":
    main()
