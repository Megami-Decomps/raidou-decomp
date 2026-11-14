#!/usr/bin/env python3
"""Names and C from dds-decomp for Raidou functions identical to DDS1/DDS2 code.

    python3 tools/raidou_dds.py names|pairs --dds DIR   (see docs/shared-c.md)
"""

from __future__ import annotations

import argparse
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import raidou_c as rc  # noqa: E402
from retail_elf import VERSIONS  # noqa: E402

MAP_FILE = rc.MAP / "dds_unique_in_raidou.tsv"
SERIAL = {"dds1": "SLUS_209.74", "dds2": "SLUS_211.52"}
PLACEHOLDER = re.compile(r"(func|sub|nullsub|D|jtbl|lbl|L)_[0-9A-Fa-f]+\w*")


class Side:
    """One DDS game: names, C functions, masked bodies and name provenance."""

    def __init__(self, root, game):
        self.game = game
        syms = rc.repo_symbols(root, game)
        self.name_at = {}
        for name, addr in syms.items():
            self.name_at.setdefault(addr, name)
        self.c = rc.c_functions(root, game)
        for addr, (name, _) in self.c.items():
            self.name_at.setdefault(addr, name)
        self.bodies = rc.split_bodies(root, game)
        self.source = {}
        for line in (root / "config" / game / "name_sources.txt").read_text().splitlines():
            cols = line.split("\t")
            if len(cols) >= 4 and not line.startswith("#"):
                self.source[int(cols[0], 16)] = (cols[2], cols[3])

    def provenance(self, addr):
        """(evidence|inferred, how dds-decomp got the name)."""
        kind, note = self.source.get(addr, ("inferred", ""))
        if note.startswith("SMT3HD"):
            how = "SMT3HD name"
        elif "evidence" in kind.split("|")[0]:
            how = "evidence"
        elif note:
            how = "descriptive"
        else:
            how = "no provenance row"
        return kind, how


def map_rows(version):
    """Raidou address -> [(game, DDS address, method)]."""
    out = defaultdict(list)
    lines = MAP_FILE.read_text().splitlines()
    head = lines[0].split("\t")
    for line in lines[1:]:
        row = dict(zip(head, line.split("\t")))
        if row["game"] != version:
            continue
        for game in rc.DDS_GAMES:
            if row.get(f"{game}_ea"):
                out[int(row["ea"], 16)].append((game, int(row[f"{game}_ea"], 16), row[f"{game}_method"]))
    return out


def verified(version, sides):
    """(src, unit, fn, address, words, [(game, DDS address, method)]) for each Raidou
    function whose map partners have the same masked words and slot kinds."""
    partners = map_rows(version)
    out = []
    rows = rc.raidou_asm_functions(version)
    for path in sorted((ROOT / "asm" / version).glob("code_*.s")):  # gaps: asm units without C
        rows += [(None, path.stem, fn, *body) for fn, body in rc.parse_asm(path).items()]
    for src, unit, fn, addr, words, slots in rows:
        good = []
        for game, ea, method in partners.get(addr, ()):
            theirs = sides[game].bodies.get(ea)
            if theirs and theirs[0] == words and [k for k, _ in theirs[1]] == [k for k, _ in slots]:
                good.append((game, ea, method))
        out.append((src, unit, fn, addr, words, good))
    return out


def load_sides(args):
    rc.DDS = args.dds.expanduser().resolve()
    return {g: Side(rc.DDS, g) for g in rc.DDS_GAMES}


def cmd_names(args):
    sides = load_sides(args)
    out = rc.OUT / "dds"
    out.mkdir(parents=True, exist_ok=True)
    for version in VERSIONS:
        taken = {m.group(1) for line in (ROOT / "config" / version / "symbol_addrs.txt").read_text().splitlines()
                 if (m := rc.SYMROW.match(line))}
        counts = Counter()
        want = {}
        compiled = {fn for src in rc.unit_sources(version).values() for fn in rc.definitions(src.read_text())}
        for src, unit, fn, addr, words, good in verified(version, sides):
            if not re.fullmatch(r"func_[0-9A-F]{8}", fn) or fn in compiled:
                continue
            if not good:
                counts["no verified partner"] += 1
                continue
            named = [(g, ea, m) for g, ea, m in good
                     if (n := sides[g].name_at.get(ea)) and not PLACEHOLDER.fullmatch(n)]
            if not named:
                counts["DDS placeholder"] += 1
                continue
            names = {sides[g].name_at[ea] for g, ea, _ in named}
            if len(names) > 1:
                counts["DDS1/DDS2 names differ"] += 1
            # C first, then DDS1.
            game, ea, method = min(named, key=lambda x: (x[1] not in sides[x[0]].c, x[0]))
            want[addr] = (sides[game].name_at[ea], game, ea, method)
        uses = Counter(n for n, *_ in want.values())
        rows = []
        for addr, (name, game, ea, method) in sorted(want.items()):
            if name in taken or uses[name] > 1:
                counts["name taken" if name in taken else "name wanted twice"] += 1
                continue
            kind, how = sides[game].provenance(ea)
            rows.append(f"0x{addr:08X}\t{name}\t{kind}\tdds-decomp {game.upper()} name via {SERIAL[game]} "
                        f"0x{ea:08X}, masked-words-identical ({method} map pair); dds-decomp: {how}")
            counts[f"named ({how})"] += 1
        (out / f"names_{version}.tsv").write_text("\n".join(rows) + "\n")
        print(version, dict(sorted(counts.items())))


def cmd_rename(args):
    """Rename func_ references in the TUs after raidou_names.py --apply of names_<v>.tsv."""
    texts = defaultdict(dict)
    for version in VERSIONS:
        new = {}
        for line in (rc.OUT / "dds" / f"names_{version}.tsv").read_text().splitlines():
            if line:
                addr, name = line.split("\t")[:2]
                new[f"func_{int(addr, 16):08X}"] = name
        for unit, src in rc.unit_sources(version).items():
            # Only INCLUDE_ASM lines: carried C may use SMT3's own func_ names.
            texts[unit][version] = re.sub(r"^(INCLUDE_ASM\([^,]+,[^,]+,\s*)(func_[0-9A-F]{8})\);$",
                                          lambda m: m.group(1) + new.get(m.group(2), m.group(2)) + ");",
                                          src.read_text(), flags=re.M)
    print(f"{rc.write_tus(texts)} files changed; {len(rc.shared_sources())} shared TUs")


def cmd_pairs(args):
    sides = load_sides(args)
    lines = ["version\tsrc\tfunction\taddress\tsmt3_address\tsmt3_name\tsmt3_src\twords\tstatus"]
    for version in VERSIONS:
        every = verified(version, sides)
        rows = [r for r in every if r[0] and r[5]]
        # One DDS game per unit: two VERSION_DDS macros in one TU select conflicting headers.
        votes = defaultdict(Counter)
        for src, unit, fn, addr, words, good in rows:
            for game, ea, _ in good:
                if ea in sides[game].c and sides[game].c[ea][0] == fn:
                    votes[unit][game] += 1
        counts = Counter()
        for src, unit, fn, addr, words, good in rows:
            game = min(votes[unit], key=lambda g: (-votes[unit][g], g)) if votes[unit] else None
            hit = next(((g, ea) for g, ea, _ in good if g == game), None)
            if hit is None:
                status, ea, name, path = "other-game", good[0][1], "", ""
            elif hit[1] not in sides[game].c:
                status, ea, name, path = "dds-asm", hit[1], "", ""
            else:
                ea = hit[1]
                name, path = sides[game].c[ea]
                status = "identical" if name == fn else "name-differs"
            counts[status] += 1
            lines.append(f"{version}\t{src}\t{fn}\t0x{addr:08X}\t0x{ea:08X}\t{name}\t{path}\t{len(words)}\t{status}")
        # A map partner of the same name whose words differ: raidou_c carries it only when
        # smt3_c.tsv marks it as compiling identical with Raidou's headers.
        partners = map_rows(version)
        for src, unit, fn, addr, words, good in every:
            if not src or good:
                continue
            hit = next(((g, ea) for g, ea, _ in partners.get(addr, ()) if sides[g].c.get(ea, ("",))[0] == fn), None)
            if hit:
                name, path = sides[hit[0]].c[hit[1]]
                counts["differs"] += 1
                lines.append(f"{version}\t{src}\t{fn}\t0x{addr:08X}\t0x{hit[1]:08X}\t{name}\t{path}\t{len(words)}\tdiffers")
        print(version, dict(sorted(counts.items())))
    (rc.OUT / "pairs_dds.tsv").write_text("\n".join(lines) + "\n")


def smt3_guard(item):
    """A type definition an SMT3 file also defines is skipped in TUs carrying that file."""
    types = {n for k, n in rc.smt3_source_types()}
    names = sorted({n for k, n in rc.declared_names(item) if k != "macro"} & types)
    if not names or rc.is_object_decl(item):
        return item
    return "#if " + " && ".join(f"!defined(SMT3_DEFINES_{n})" for n in names) + "\n" + item + "\n#endif"


def smt3_header(path):
    """A dds-decomp header that gives way to SMT3's: what SMT3's headers define is dropped
    and those headers included instead; the include guard gets a DDS_ prefix."""
    text = rc.dds_rename(path.read_text(errors="replace"))
    guard = rc.header_guard(text)
    items, need = rc.defer_to_smt3(b for b in rc.blocks(text) if rc.declared_names(b) != {("macro", guard)})
    items = [smt3_guard(i) for i in items]
    text = "\n\n".join(items) + "\n"
    if guard:
        text = re.sub(rf"\b{guard}\b", f"DDS_{guard}", text)
    head = "".join(f'#include "../{h}"\n' for h in need)
    m = re.search(r"^\s*#\s*ifndef\s+\w+\s*\n", text, re.M) if guard else None
    at = m.end() if m else 0
    return text[:at] + (f"#define DDS_{guard}\n" if guard else "") + head + text[at:]


HEADER_MANIFEST = ROOT / "config" / "dds_headers.sha1"


def read_manifest():
    out = {}
    if HEADER_MANIFEST.exists():
        for line in HEADER_MANIFEST.read_text().splitlines():
            if line and not line.startswith("#"):
                digest, name = line.split(None, 1)
                out[name] = digest
    return out


def write_manifest(rows):
    lines = ["# sha1 of each include/dds header as raidou_dds.py headers last wrote it; a header whose"
             " text differs was edited by hand and is never overwritten or removed"]
    lines += [f"{d}  {n}" for n, d in sorted(rows.items())]
    HEADER_MANIFEST.write_text("\n".join(lines) + "\n")


def cmd_headers(args):
    """Copy the dds-decomp headers the TUs' DDS declaration blocks include (and theirs) into include/dds/.
    Only headers this command wrote and nobody edited since are rewritten or removed (config/dds_headers.sha1)."""
    import hashlib
    sha1 = lambda s: hashlib.sha1(s.encode()).hexdigest()
    rc.DDS = args.dds.expanduser().resolve()
    want, todo = set(), []
    for src in rc.SRC.rglob("*.c"):
        todo += re.findall(r'^\s*#\s*include\s+"dds/([^"]+)"', src.read_text(errors="replace"), re.M)
    while todo:
        name = todo.pop()
        if name in want or not (rc.DDS / "include" / name).exists():
            continue
        want.add(name)
        todo += re.findall(r'^\s*#\s*include\s+"([^"]+)"', (rc.DDS / "include" / name).read_text(errors="replace"), re.M)
    out = ROOT / "include" / "dds"
    manifest = read_manifest()
    have = {p.relative_to(out).as_posix() for p in out.rglob("*.h")} if out.exists() else set()

    def owned(name):
        return manifest.get(name) == sha1((out / name).read_text(errors="replace"))

    new = kept = changed = 0
    for name in sorted(want):
        dst = out / name
        text = smt3_header(rc.DDS / "include" / name)
        if dst.exists():
            if dst.read_text(errors="replace") == text:
                manifest[name] = sha1(text)
                continue
            if not owned(name):
                kept += 1
                if args.verbose:
                    print(f"kept {name}: edited by hand or not written by this command")
                continue
            changed += 1
        else:
            new += 1
            dst.parent.mkdir(parents=True, exist_ok=True)
        dst.write_text(text)
        manifest[name] = sha1(text)
    removed = 0
    for name in sorted(have - want):
        if owned(name):
            (out / name).unlink()
            manifest.pop(name)
            removed += 1
    write_manifest({n: d for n, d in manifest.items() if (out / n).exists()})
    print(f"{len(want)} dds-decomp headers wanted: {new} new, {changed} updated, {removed} removed, "
          f"{kept} differ and kept (--verbose lists them)")


def type_sizes(inc, includes, exprs, defines, raw=False):
    """sizeof(expr) (expr itself with raw) for each expr that compiles after includes, with ee-gcc's cc1
    and include dir inc."""
    import shlex
    import subprocess
    import tempfile
    sys.path.insert(0, str(ROOT))
    import configure
    exprs, out = list(exprs), {}
    with tempfile.TemporaryDirectory() as tmp:
        src, asm = Path(tmp) / "probe.c", Path(tmp) / "probe.s"
        while exprs:
            src.write_text("".join(f'#include "{i}"\n' for i in includes)
                           + "".join((f"const int probe_{i} = {e};\n" if raw else f"const int probe_{i} = sizeof({e});\n") for i, e in enumerate(exprs)))
            cmd = ["tools/glibc32/ld-linux.so.2", "--library-path", "tools/glibc32", str(configure.CC1),
                   *shlex.split(configure.CC1_DEFINES), f"-I{inc}", "-DSKIP_ASM", *defines, *shlex.split(configure.CC1_FLAGS),
                   str(src), "-o", str(asm)]
            r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
            if r.returncode == 0:
                vals = re.findall(r"^probe_(\d+):\s*\n\s*(?:\.word\s+(\d+)|\.space\s+4)", asm.read_text(), re.M)
                out.update({exprs[int(i)]: int(v or 0) for i, v in vals})
                break
            bad = {int(n) - len(includes) - 1 for n in re.findall(r"probe\.c:(\d+):", r.stderr)}
            bad = {exprs[i] for i in bad if 0 <= i < len(exprs)}
            if not bad:
                print(f"{includes[-1]}: does not compile", file=sys.stderr)
                break  # an include itself fails
            exprs = [e for e in exprs if e not in bad]
    return out


def cmd_layouts(args):
    """Types dds-decomp and SMT3 both define, with another size in DDS1 or DDS2: the list
    config/dds_types.txt holds (the DDS types the carried code uses as dds_<name>)."""
    dds_inc = args.dds.expanduser().resolve() / "include"
    smt3 = rc.smt3_header_names()
    want = defaultdict(set)
    for h in sorted(dds_inc.rglob("*.h")):
        for b in rc.blocks(h.read_text(errors="replace")):
            cv = rc.code_view(b).strip()
            for kind, name in rc.declared_names(b):
                if (kind, name) not in smt3:
                    continue
                if kind == "tag" and (m := re.search(rf"\b(struct|union)\s+{name}\s*\{{", cv)):
                    want[h.relative_to(dds_inc).as_posix()].add((name, f"{m.group(1)} {name}"))
                elif kind == "ident" and cv.startswith("typedef") and not re.search(r"\(\s*\*", cv):
                    want[h.relative_to(dds_inc).as_posix()].add((name, name))
    exprs = sorted({e for pairs in want.values() for _, e in pairs})
    smt3_inc = ROOT / "include"
    base = type_sizes(smt3_inc, sorted(p.name for p in smt3_inc.glob("*.h")), exprs, ["-DVERSION_RAIDOU1"])
    differ = set()
    for game in ("DDS1", "DDS2"):
        for h, pairs in sorted(want.items()):
            got = type_sizes(dds_inc, [h], sorted(e for _, e in pairs), [f"-DVERSION_{game}=1"])
            differ |= {n for n, e in pairs if e in got and e in base and got[e] != base[e]}
    for name in sorted(differ, key=str.lower):
        print(name)


def cmd_members(args):
    """DDS member names that SMT3 spells differently at the same offset and size, per type both
    define with one layout: config/dds_members.txt, which raidou_c applies to carried DDS code."""
    dds = args.dds.expanduser().resolve()
    smt3 = rc.smt3_header_names()
    renamed = {ln.split("#")[0].strip() for ln in (ROOT / "config/dds_types.txt").read_text().splitlines()}
    structs, _ = rc.dds_structs(dds, sources=False)
    shared = {}  # name -> (type expression, dds members, dds header, smt3 members)
    for name, defs in structs.items():
        for kind, su, members, path in defs:
            if name in renamed or (kind, name) not in smt3:
                continue
            text = (ROOT / "include" / smt3[(kind, name)]).read_text(errors="replace")
            got = next((g for b in rc.blocks(text) if (kind, name) in rc.declared_names(b)
                        and (g := rc.struct_body(b, kind, name))), None)
            if got:
                expr = f"{su} {name}" if kind == "tag" else name
                shared[name] = (expr, [m for m, _ in members], path.relative_to(dds / "include").as_posix(),
                                [m for m, _ in rc.struct_members(got[1], nested=True)])
    probe = lambda expr, m: (f"(int)&((({expr} *)0)->{m})", f"sizeof(((({expr} *)0)->{m}))")
    smt3_inc = ROOT / "include"
    exprs = [e for expr, _, _, sm in shared.values() for m in sm for e in probe(expr, m)]
    base = type_sizes(smt3_inc, sorted(p.name for p in smt3_inc.glob("*.h")), exprs, ["-DVERSION_RAIDOU1"], raw=True)
    by_header = defaultdict(list)
    for name, (expr, dm, h, _) in shared.items():
        by_header[h] += [e for m in dm for e in probe(expr, m)]
    games = [{} for _ in ("DDS1", "DDS2")]
    for h, ex in sorted(by_header.items()):
        for got, game in zip(games, ("DDS1", "DDS2")):
            got.update(type_sizes(dds / "include", [h], ex, [f"-DVERSION_{game}=1"], raw=True))
    lines = ["# type, DDS member, SMT3 member at the same offset and size (raidou_dds.py members).",
             "# raidou_c rewrites carried DDS code's accesses through a variable of that type."]
    for name, (expr, dm, _, sm) in sorted(shared.items()):
        at = defaultdict(list)
        for m in sm:
            if all(e in base for e in probe(expr, m)):
                at[tuple(base[e] for e in probe(expr, m))].append(m)
        found = {}
        for m in dm:
            keys = {tuple(g.get(e) for e in probe(expr, m)) for g in games} - {(None, None)}
            hit = at.get(next(iter(keys)), []) if len(keys) == 1 else []
            same = [h for h in hit if h.rsplit(".", 1)[-1] == m]
            if m not in sm and (len(same) == 1 or len(hit) == 1):
                found[m] = (same or hit)[0]
        # Two DDS members on one SMT3 member are a union SMT3 names as a whole: not a rename.
        targets = Counter(found.values())
        lines += [f"{name}\t{m}\t{s}" for m, s in found.items() if targets[s] == 1]
    (ROOT / "config/dds_members.txt").write_text("\n".join(lines) + "\n")
    print(f"{len(shared)} shared types; {len(lines) - 2} members renamed")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name, func in (("names", cmd_names), ("rename", cmd_rename), ("pairs", cmd_pairs), ("headers", cmd_headers),
                       ("layouts", cmd_layouts), ("members", cmd_members)):
        p = sub.add_parser(name)
        p.add_argument("--dds", type=Path, default=rc.DDS, help="dds-decomp checkout, configured (split)")
        if name == "headers":
            p.add_argument("--verbose", action="store_true")
        p.set_defaults(func=func)
    args = ap.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
