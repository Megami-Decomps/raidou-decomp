#!/usr/bin/env python3
"""Rank matched functions on both games by asm shape similarity to a target function.

Usage: find_similar.py --for NAME [--version raidou2] [--body] | [--unit DIR] | --clusters | --stats.
Read `sub` (instructions the template does not supply), not the ratio; below 0.9 it is a rewrite.
Tiers: exact (mnemonic, registers, immediates), noimm, noreg, mnem; callees compare by name.
"""
from __future__ import annotations

import argparse
import bisect
import collections
import difflib
import hashlib
import json
import math
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from units import VERSIONS, unit_sources  # noqa: E402
from version_guard import view  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
CACHE = ROOT / "build" / "find_similar"
TIERS = ("exact", "noimm", "noreg", "mnem")
OBJDUMP = ROOT / "tools/bin/mips-ps2-decompals-objdump"

GPR = {
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8",
    "k0", "k1", "gp", "sp", "ra", "fp",
}
DIS_LINE = re.compile(r"^\s*([0-9a-f]+):\t([0-9a-f]{8}) \t(\S+)\s*(.*)$")
INCLUDE_ASM = re.compile(r"^INCLUDE_ASM\([^\n]*\b(\w+)\);", re.M)
FUNC_ASM = re.compile(r"^glabel (\w+)\n(.*?)^endlabel \1", re.M | re.S)
ASM_INSN = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/", re.M)
NUM = re.compile(r"^-?(?:0x[0-9a-fA-F]+|\d+)$")
SKIP = ("hand/", "data/", "matchings/", "nonmatchings/", "vu/")
CALLS = {"jal", "jalx"}
# Below this an instruction sequence carries no shape: a prologue and a jr $ra.
MIN_INSNS = 8
# A template more than this much longer or shorter is not one worth reading.
SIZE_BAND = 0.15
# Sequence comparison is the expensive step; only the best of the cheap score run it.
RERANK = 40
# Shortest shared name prefix worth offering as a same-family candidate.
FAMILY_PREFIX = 8
UNNAMED = ("func_", "D_", "code_", "sub_")


def text_range(elf: Path) -> tuple[int, int]:
    out = subprocess.run([str(OBJDUMP), "-h", str(elf)], capture_output=True, text=True,
                         check=True).stdout
    m = re.search(r"\.text\s+([0-9a-f]+)\s+([0-9a-f]+)", out)
    return int(m.group(2), 16), int(m.group(2), 16) + int(m.group(1), 16)


def disassemble(elf: Path) -> list[tuple[int, str, str]]:
    """(address, mnemonic, operand text) for every instruction of .text.

    -z is required: without it objdump elides a run of zero words and drops the nops.
    """
    out = subprocess.run([str(OBJDUMP), "-d", "-z", "--section=.text", str(elf)],
                         capture_output=True, text=True, check=True).stdout
    return [(int(m.group(1), 16), m.group(3), m.group(4))
            for line in out.splitlines() if (m := DIS_LINE.match(line))]


def operands(text: str) -> tuple[list[str], list[int]]:
    """Registers in order and numeric immediates in order."""
    regs, imms = [], []
    for tok in re.split(r"[,()\s]+", text.split("<")[0].strip()):
        if not tok:
            continue
        if tok.startswith("$") or tok in GPR:
            regs.append(tok)
        elif NUM.match(tok):
            imms.append(int(tok, 0))
    return regs, imms


def tokens(addr: int, mnem: str, text: str, lo: int, hi: int) -> tuple[str, str, str, str]:
    """One instruction at each of the four tiers."""
    regs, imms = operands(text)
    if mnem in CALLS:
        imms = []
    elif imms and (mnem[0] == "b" or mnem == "j") and lo <= imms[-1] < hi:
        imms = imms[:-1] + [imms[-1] - addr]
    r, i = ",".join(regs), ",".join(str(v) for v in imms)
    return f"{mnem} {r} {i}", f"{mnem} {r}", f"{mnem}  {i}", mnem


def asm_files(version: str) -> list[Path]:
    base = ROOT / "asm" / version
    return [p for p in sorted(base.rglob("*.s"))
            if not p.relative_to(base).as_posix().startswith(SKIP)]


def inventory(version: str) -> list[dict]:
    """Every function of every unit: name, address, instruction count, unit, state.

    The inventory tools/progress.py counts: a function is assembly when its unit
    has no source (a gap unit) or the version's view of the source holds its INCLUDE_ASM.
    """
    sources = unit_sources(version)
    base = ROOT / "asm" / version
    out = []
    for path in asm_files(version):
        unit = path.relative_to(base).as_posix()[:-2]
        src = sources.get(unit)
        asm = set(INCLUDE_ASM.findall(view(src.read_text(errors="replace"), version))) if src else None
        for fname, body_text in FUNC_ASM.findall(path.read_text()):
            addrs = ASM_INSN.findall(body_text)
            if addrs:
                state = "c" if asm is not None and fname not in asm else "asm"
                out.append({"name": fname, "addr": int(addrs[0], 16), "n": len(addrs),
                            "unit": unit, "state": state, "version": version})
    out.sort(key=lambda f: f["addr"])
    return out


def stamp(version: str, elf: Path) -> str:
    srcs = list(unit_sources(version).values()) + asm_files(version)
    return str(max([elf.stat().st_mtime_ns] + [p.stat().st_mtime_ns for p in srcs]))


def corpus(version: str, force: bool = False) -> dict[str, dict]:
    """Every .text function of one game, by name, with its four sequences and its callees."""
    elf = ROOT / "orig" / version / VERSIONS[version]["serial"]
    cache = CACHE / f"{version}.json"
    key = stamp(version, elf)
    if cache.exists() and not force:
        got = json.loads(cache.read_text())
        if got.get("stamp") == key:
            return got["funcs"]
    lo, hi = text_range(elf)
    inv = inventory(version)
    starts = {f["addr"]: f for f in inv}
    names = {f["addr"]: f["name"] for f in inv}
    funcs, cur, left = {}, None, 0
    for addr, mnem, text in disassemble(elf):
        if addr in starts:
            f = starts[addr]
            cur = funcs[f["name"]] = dict(f, seq=[[], [], [], []], calls=[])
            left = f["n"]
        if cur is None or left == 0:
            continue
        t = tokens(addr, mnem, text, lo, hi)
        for k in range(4):
            cur["seq"][k].append(t[k])
        left -= 1
        if mnem in CALLS:
            _, imms = operands(text)
            cur["calls"].append(names.get(imms[0], hex(imms[0])) if imms else "?")
    short = [f["name"] for f in funcs.values() if len(f["seq"][0]) != f["n"]]
    if short or len(funcs) != len({f["name"] for f in inv}):
        raise SystemExit(f"{version}: disassembly does not cover the split asm: {short[:5]} "
                         f"{len(funcs)} of {len(inv)} functions")
    for f in funcs.values():
        f["hash"] = [hashlib.blake2b("\n".join(s).encode(), digest_size=8).hexdigest()
                     for s in f["seq"]]
    CACHE.mkdir(parents=True, exist_ok=True)
    cache.write_text(json.dumps({"stamp": key, "funcs": funcs}))
    return funcs


def load(versions=None, force: bool = False) -> dict[str, dict]:
    """Functions of every game keyed VERSION:NAME."""
    out = {}
    for v in versions or VERSIONS:
        for name, f in corpus(v, force).items():
            out[f"{v}:{name}"] = f
    return out


def key(f: dict) -> str:
    return f"{f['version']}:{f['name']}"


def lookup(funcs: dict[str, dict], name: str, version: str | None) -> dict | None:
    """The target by name, on the given game first."""
    order = [version] if version else []
    order += [v for v in VERSIONS if v not in order]
    for v in order:
        if f"{v}:{name}" in funcs:
            return funcs[f"{v}:{name}"]
    return None


# --- similarity ------------------------------------------------------------------

class Index:
    """Nearest matched functions for a target, by instruction sequence, over both games."""

    def __init__(self, funcs: dict[str, dict], tier: int = 3):
        self.funcs, self.tier = funcs, tier
        pool: dict[str, int] = {}
        for f in funcs.values():
            f["tok"] = tuple(pool.setdefault(s, len(pool)) for s in f["seq"][tier])
            f["bag"] = collections.Counter(f["tok"])
            f["len"] = math.sqrt(sum(v * v for v in f["bag"].values())) or 1.0
        self.by_size: dict[int, list[dict]] = collections.defaultdict(list)
        for f in funcs.values():
            if f["state"] == "c" and f["n"] >= MIN_INSNS:
                self.by_size[f["n"]].append(f)
        self.sizes = sorted(self.by_size)

    def ratio(self, a: dict, b: dict) -> float:
        return difflib.SequenceMatcher(None, a["tok"], b["tok"], autojunk=False).ratio()

    def nearest(self, target: dict, top: int = 3, state: str = "c") -> list[tuple[float, dict]]:
        """The closest functions in `state`, best first, as (ratio, function)."""
        if target["n"] < MIN_INSNS:
            return []
        lo, hi = target["n"] * (1 - SIZE_BAND), target["n"] * (1 + SIZE_BAND)
        if state == "c":
            pool = [f for s in self.sizes[bisect.bisect_left(self.sizes, lo):
                                          bisect.bisect_right(self.sizes, hi)]
                    for f in self.by_size[s]]
        else:
            pool = [f for f in self.funcs.values()
                    if f["state"] == state and lo <= f["n"] <= hi and f["n"] >= MIN_INSNS]
        bag, norm, me = target["bag"], target["len"], key(target)
        cheap = sorted(
            ((sum(v * c["bag"].get(k, 0) for k, v in bag.items()) / (norm * c["len"]), c)
             for c in pool if key(c) != me), key=lambda x: -x[0])
        scored = [(self.ratio(target, c), c) for _, c in cheap[:RERANK]]
        return sorted(scored, key=lambda x: -x[0])[:top]


def family(funcs: dict[str, dict], name: str, me: str | None = None) -> list[dict]:
    """Matched functions sharing a long name prefix with this one, on either game.

    A size band hides the variant that grew; a shared prefix does not care about
    size. Numbered wrappers (sbtlCallBtlGetSkilltbl8/10/16) come out here. It is only
    a candidate generator: rank what comes back by sequence ratio.
    """
    if len(name) < FAMILY_PREFIX or name.startswith(UNNAMED):
        return []
    out = []
    for k, g in funcs.items():
        if g["state"] != "c" or k == me:
            continue
        i = 0
        while i < min(len(name), len(g["name"])) and name[i] == g["name"][i]:
            i += 1
        if i >= FAMILY_PREFIX:
            out.append(g)
    return out


def same_name(funcs: dict[str, dict], t: dict) -> list[dict]:
    """The other game's matched copy of the target, whatever its size."""
    return [g for v in VERSIONS if v != t["version"]
            if (g := funcs.get(f"{v}:{t['name']}")) and g["state"] == "c"]


def templates(funcs: dict[str, dict], index: Index, t: dict, top: int = 5) -> list[tuple[float, dict]]:
    """Matched candidates for t, best ratio first: size-banded neighbours, name family, other game."""
    seen, out = {key(t)}, []
    cands = [c for _, c in index.nearest(t, top=top)] + family(funcs, t["name"], key(t)) \
        + same_name(funcs, t)
    for c in cands:
        if key(c) not in seen:
            seen.add(key(c))
            out.append((index.ratio(t, c), c))
    return sorted(out, key=lambda x: -x[0])[:top]


def clusters(funcs: dict[str, dict], tier: int = 3) -> list[list[dict]]:
    """Groups of two or more functions with the same normalised sequence."""
    by = collections.defaultdict(list)
    for f in funcs.values():
        by[f["hash"][tier]].append(f)
    out = [sorted(v, key=lambda f: (f["version"], f["addr"])) for v in by.values() if len(v) > 1]
    return sorted(out, key=lambda v: -sum(f["n"] * 4 for f in v if f["state"] == "asm"))


def definition(f: dict) -> tuple[Path, int, str] | None:
    """(source path, line, text) of a matched function's C definition as its game compiles it."""
    if f is None or f["state"] != "c":
        return None
    path = unit_sources(f["version"]).get(f["unit"])
    if path is None:
        return None
    text = view(path.read_text(errors="replace"), f["version"])
    m = re.search(rf"^[A-Za-z_][^;#\n]*\b{re.escape(f['name'])}\s*\([^;{{]*\)\s*\{{", text, re.M)
    if m is None:
        return None
    i, depth = m.end() - 1, 0
    while i < len(text):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                break
        i += 1
    return path, text[:m.start()].count("\n") + 1, text[m.start():i + 1]


def body(f: dict) -> str | None:
    """The C definition of a matched function, with its file and line."""
    got = definition(f)
    if got is None:
        return None
    path, line, text = got
    return f"// {path.relative_to(ROOT)}:{line} ({f['version']} view)\n{text}"


def differences(a: dict, b: dict) -> list[str]:
    """What substituting b's C for a would have to change, at the exact tier."""
    out = []
    for op, i0, i1, j0, j1 in difflib.SequenceMatcher(
            None, a["seq"][0], b["seq"][0], autojunk=False).get_opcodes():
        if op == "equal":
            continue
        for k in range(max(i1 - i0, j1 - j0)):
            mine = a["seq"][0][i0 + k] if i0 + k < i1 else "-"
            theirs = b["seq"][0][j0 + k] if j0 + k < j1 else "-"
            out.append(f"    {mine:<40} template {theirs}")
    for x, y in zip(a["calls"], b["calls"]):
        if x != y:
            out.append(f"    call {x:<35} template {y}")
    if len(a["calls"]) != len(b["calls"]):
        out.append(f"    {len(a['calls'])} calls, template has {len(b['calls'])}")
    return out


# --- reporting -------------------------------------------------------------------

def label(f: dict) -> str:
    return f"{f['name']} [{'R1' if f['version'] == 'raidou1' else 'R2'}]"


def row(ratio: float, t: dict, c: dict, diffs: int | None = None) -> str:
    left = f"{diffs:>4} sub" if diffs is not None else f"{ratio:.3f}   "
    return (f"{left} {label(t):<40} {t['n'] * 4:>5}B {t['unit']:<28} "
            f"<- {label(c):<40} {c['n'] * 4:>5}B {c['unit']}")


def worklist(funcs, tier, version, unit, count, floor) -> None:
    """Unmatched functions of a game ranked by retail bytes per substitution left to make."""
    index = Index(funcs, tier)
    targets = [f for f in funcs.values() if f["state"] == "asm" and f["n"] >= MIN_INSNS
               and (version is None or f["version"] == version)
               and (unit is None or f["unit"].startswith(unit))]
    rows = []
    for t in targets:
        best = templates(funcs, index, t, top=1)
        if not best or best[0][0] < floor:
            continue
        diffs = len(differences(t, best[0][1]))
        rows.append((t["n"] * 4 / (diffs + 1), diffs, best[0][0], t, best[0][1]))
    rows.sort(key=lambda r: -r[0])
    print(f"{len(rows)} of {len(targets)} unmatched functions have a matched {TIERS[tier]} "
          f"template at ratio {floor:.2f} or better ({sum(r[3]['n'] * 4 for r in rows)} retail bytes). "
          f"'sub' is how many instructions the template does not already supply.")
    for _, diffs, ratio, t, c in rows[:count]:
        print(row(ratio, t, c, diffs))


def stats(funcs: dict[str, dict]) -> None:
    asm = [f for f in funcs.values() if f["state"] == "asm"]
    print(f"{len(funcs)} functions over {len(VERSIONS)} games, {len(asm)} still assembly")
    for k, tier in enumerate(TIERS):
        by = collections.defaultdict(list)
        for f in funcs.values():
            by[f["hash"][k]].append(f)
        twin = [f for f in asm if any(g["state"] == "c" for g in by[f["hash"][k]])]
        print(f"  {tier:6} {len(twin):5} unmatched with an identical matched twin "
              f"({sum(f['n'] * 4 for f in twin)}B)")


def show_clusters(funcs, tier, version, unit, count) -> None:
    out = []
    for group in clusters(funcs, tier):
        rest = [f for f in group if f["state"] == "asm" and (version is None or f["version"] == version)]
        if len(rest) < 2 or group[0]["n"] < MIN_INSNS:
            continue
        if unit and not any(f["unit"].startswith(unit) for f in group):
            continue
        out.append(group)
    print(f"{len(out)} clusters with two or more unmatched members")
    for group in out[:count]:
        shown = ", ".join(f"{label(f)}{'' if f['state'] == 'asm' else ' C'}" for f in group[:8])
        more = f", +{len(group) - 8} more" if len(group) > 8 else ""
        print(f"  {group[0]['n'] * 4}B x{len(group)} {shown}{more}")


def show_one(funcs, tier, name, version, want_body, top) -> None:
    t = lookup(funcs, name, version)
    if t is None:
        raise SystemExit(f"{name}: not a function of any unit")
    index = Index(funcs, tier)
    print(f"{label(t)} {t['n'] * 4}B {t['unit']} ({t['state']})")
    near = templates(funcs, index, t, top)
    print("  matched templates:" if near else "  no matched templates")
    for ratio, c in near:
        print("  " + row(ratio, t, c))
    sib = index.nearest(t, top=3, state="asm")
    if sib:
        print("  unmatched siblings:")
        for ratio, c in sib:
            print("  " + row(ratio, t, c))
    if not near:
        return
    best = near[0][1]
    diff = differences(t, best)
    print(f"  against {label(best)}: {len(diff)} exact-tier differences" if diff
          else f"  against {label(best)}: identical beyond the callees")
    for line in diff[:40]:
        print(line)
    if len(diff) > 40:
        print(f"    ... {len(diff) - 40} more")
    if want_body:
        print()
        print(body(best) or f"// no C definition found for {label(best)}")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--for", dest="target", help="one function to find templates for")
    ap.add_argument("--version", choices=list(VERSIONS),
                    help="game of the target or worklist (templates come from both)")
    ap.add_argument("--body", action="store_true", help="print the best template's C")
    ap.add_argument("--clusters", action="store_true", help="groups of unmatched functions with the same shape")
    ap.add_argument("--unit", help="restrict to units whose path starts with this")
    ap.add_argument("--tier", choices=TIERS, default="mnem")
    ap.add_argument("--floor", type=float, default=0.9, help="lowest sequence ratio in the worklist")
    ap.add_argument("-n", "--count", type=int, default=40)
    ap.add_argument("--top", type=int, default=5, help="templates shown with --for")
    ap.add_argument("--force", action="store_true", help="rebuild the disassembly cache")
    ap.add_argument("--stats", action="store_true", help="per-tier identical-twin counts")
    args = ap.parse_args()

    funcs = load(force=args.force)
    tier = TIERS.index(args.tier)
    if args.stats:
        stats(funcs)
    elif args.clusters:
        show_clusters(funcs, tier, args.version, args.unit, args.count)
    elif args.target:
        show_one(funcs, tier, args.target, args.version, args.body, args.top)
    else:
        worklist(funcs, tier, args.version, args.unit, args.count, args.floor)


if __name__ == "__main__":
    main()
