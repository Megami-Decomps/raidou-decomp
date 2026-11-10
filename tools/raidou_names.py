#!/usr/bin/env python3
"""Carry SMT3 function names to raidou1 and raidou2 through code pairing.

Writes pairs_<v>.tsv (every candidate) and names_<v>.tsv (accepted); `--apply`
puts the latter into config/<v>/. Method and rules: docs/smt3-names.md."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TEXT_START = 0x00100000
DELTA = 0xFF000  # vram minus file offset of .text, the same in all three ELFs
GP = 28
SMT3_TEXT_END = 0x002E6E44
LO_OPS = frozenset((0x09, 0x19, 0x1E, 0x1F, 0x20, 0x21, 0x23, 0x24, 0x25, 0x27, 0x28,
                    0x29, 0x2B, 0x31, 0x35, 0x37, 0x39, 0x3D, 0x3F))
MIN_SIZE = 16
SERIAL = {"raidou1": "SLUS_214.31", "raidou2": "SLUS_218.45"}

PLACEHOLDER = re.compile(
    r"^(sub|nullsub|func|loc|locret|def|unk|j_sub|jpt|code|D)_[0-9A-Za-z]+$")
ADDRESS_TAG = re.compile(r"_[0-9A-Fa-f]{5,}$")
IDENT = re.compile(r"^[A-Za-z_]\w*$")
JUNK = re.compile(r"^gcc2_compiled|_[0-9]+$")
ROW = re.compile(r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;(.*)$")
GLABEL = re.compile(r"^glabel (\S+)")
STRONG = ("unique-masked-hash", "masked-hash-in-gap", "byte-identical", "reloc-identical")
MARKER = "// Names from tools/raidou_names.py (provenance: name_sources.txt)"


def reloc_hash(code):
    """Hash with only the fields that form addresses zeroed (see dds_names.py)."""
    words = bytearray()
    live = set()
    for i in range(0, len(code) - len(code) % 4, 4):
        w = struct.unpack_from("<I", code, i)[0]
        op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
        if op in (2, 3):
            w &= 0xFC000000
        elif op == 0x0F:
            w &= 0xFFFF0000
            live.add(rt)
        elif op in LO_OPS and (rs == GP or rs in live):
            w &= 0xFFFF0000
            if rs in live and op in (0x09, 0x19):
                live.discard(rs)
        elif op == 0x0D and rs in live:
            w &= 0xFFFF0000
            live.discard(rs)
        words += struct.pack("<I", w)
    return hashlib.sha1(bytes(words)).hexdigest()[:16]


def read_pairs(path):
    """pair_build.py output as {from_addr: (to_addr, tier, evidence)}."""
    out = {}
    for line in Path(path).read_text().splitlines():
        if not line or line.startswith("#") or line.startswith("kind\t"):
            continue
        kind, a, _an, _asz, b, _bsz, _d, tier, ev = line.split("\t")
        if kind.startswith("pair"):
            out[int(a, 16)] = (int(b, 16), tier, ev)
    return out


def load_side(cache_file, elf):
    """{addr: (size, masked, raw, idb_name, reloc)} from a pair_build.py cache."""
    image = Path(elf).read_bytes()
    out = {}
    for addr, size, _insns, masked, raw, name in json.loads(Path(cache_file).read_text()):
        code = image[addr - DELTA:addr - DELTA + size] if masked else b""
        out[addr] = (size, masked, raw, name, reloc_hash(code) if code else "")
    return out


FIELD = {"masked": 1, "raw": 2, "reloc": 4}


def unique_index(side, field):
    by = defaultdict(list)
    for addr, row in side.items():
        h = row[FIELD[field]]
        if h and row[0] >= MIN_SIZE:
            by[h].append(addr)
    return {h: addrs[0] for h, addrs in by.items() if len(addrs) == 1}, by


def smt3_repo_names(smt3_decomp):
    """Function names smt3-decomp's symbol_addrs.txt gives, which override the IDB's."""
    out = {}
    for line in (smt3_decomp / "config/smt3/symbol_addrs.txt").read_text().splitlines():
        if (m := ROW.match(line)) and "type:func" in m[3]:
            out.setdefault(int(m[2], 16), m[1])
    return out


def read_map(path):
    """smt3-port's cross-game map as ({raidou_ea: smt3_ea}, {smt3_ea: raidou_ea})."""
    fwd, back = {}, {}
    for line in Path(path).read_text().splitlines()[1:]:
        cols = line.split("\t")
        if len(cols) >= 4 and cols[2] and cols[3]:
            ea, sea = int(cols[0], 16), int(cols[3], 16)
            fwd[ea] = sea
            back[sea] = ea
    return fwd, back


def glabels(version):
    """Function starts splat emitted in the code unit; only these can be renamed."""
    out = {}
    for path in (ROOT / "asm" / version).glob("code_*.s"):
        for line in path.read_text().splitlines():
            if m := GLABEL.match(line):
                name = m[1]
                if name.startswith("func_"):
                    out[int(name[5:], 16)] = name
    return out


def read_symbols(version):
    rows = {}
    for line in (ROOT / "config" / version / "symbol_addrs.txt").read_text().splitlines():
        if m := ROW.match(line):
            rows.setdefault(int(m[2], 16), m[1])
    return rows


class Game:
    def __init__(self, version, pairs_path, cache_dir, smt3_elf, smt3_names, map_path):
        self.version = version
        self.pairs = read_pairs(pairs_path)
        elf = ROOT / "orig" / version / SERIAL[version]
        self.smt3 = load_side(Path(cache_dir) / "from.json", smt3_elf)
        self.target = load_side(Path(cache_dir) / "to.json", elf)
        self.smt3_names = smt3_names
        self.map_fwd, self.map_back = read_map(map_path)
        self.labels = glabels(version)
        self.symbols = read_symbols(version)
        self.rows = {}       # addr -> [current, smt3_addr, smt3_name, status, evidence, map]
        self.accepted = {}   # addr -> (smt3_addr, name, evidence)

    def name_of(self, a):
        """The SMT3 name at a, smt3-decomp's spelling first."""
        return self.smt3_names.get(a) or self.smt3[a][3]

    def name_source(self, a):
        repo = self.smt3_names.get(a)
        return "smt3-decomp name" if repo and repo != self.smt3[a][3] else "SMT3HD name"

    def candidates(self):
        _, masked_all = unique_index(self.smt3, "masked")
        raw_u, _ = unique_index(self.smt3, "raw")
        reloc_u, _ = unique_index(self.smt3, "reloc")
        _, t_masked_all = unique_index(self.target, "masked")
        t_raw_u, _ = unique_index(self.target, "raw")
        t_reloc_u, _ = unique_index(self.target, "reloc")
        forced = {a: t_raw_u[h] for h, a in raw_u.items() if h in t_raw_u}
        proven = {a: t_reloc_u[h] for h, a in reloc_u.items() if h in t_reloc_u}
        out = {}
        for a, (_size, masked, *_rest) in sorted(self.smt3.items()):
            if not TEXT_START <= a < SMT3_TEXT_END:
                continue
            if a in forced:
                out[a] = (forced[a], "byte-identical")
                continue
            if a in proven:
                out[a] = (proven[a], "reloc-identical")
                continue
            pair = self.pairs.get(a)
            if pair is None:
                continue
            b, tier, ev = pair
            if tier != "high":
                out[a] = (b, f"reject:tier-{tier}")
            elif len(masked_all.get(masked, ())) != 1 or len(t_masked_all.get(masked, ())) != 1:
                out[a] = (b, "shared-hash")
            else:
                out[a] = (b, ev)
        # A shared-hash pair is kept only when a proven neighbour has the same delta.
        keys = sorted(out)
        delta = {a: out[a][0] - a for a in keys if not out[a][1].startswith("reject:")}
        strong = {a for a in delta if out[a][1] != "shared-hash"}
        for i, a in enumerate(keys):
            if out[a][1] != "shared-hash":
                continue
            prev = next((keys[j] for j in range(i - 1, -1, -1) if keys[j] in strong), None)
            nxt = next((keys[j] for j in range(i + 1, len(keys)) if keys[j] in strong), None)
            if (prev is not None and delta[prev] == delta[a]) or (nxt is not None and delta[nxt] == delta[a]):
                out[a] = (out[a][0], "shared-hash-in-run")
            else:
                out[a] = (out[a][0], "reject:shared-hash-off-run")
        return out

    def map_verdict(self, a, b):
        """How the independent map stands on SMT3 a at raidou b."""
        at_b = self.map_fwd.get(b)
        at_a = self.map_back.get(a)
        if at_b is None and at_a is None:
            return "map-silent"
        if at_b == a:
            return "map-agrees"
        if at_b is not None:
            return f"map-has-0x{at_b:08X}-at-target"
        return f"map-puts-source-at-0x{at_a:08X}"

    def check(self, a, b, name, dropped):
        """Status of naming raidou b after SMT3 a, short of the cross-game rules."""
        if a in dropped:
            return "reject:transitivity"
        if PLACEHOLDER.match(name):
            return "reject:smt3-placeholder"
        if ADDRESS_TAG.search(name) or not IDENT.match(name) or JUNK.match(name):
            return "reject:address-tag"
        if b not in self.labels:
            return "reject:not-a-splat-function"
        if not self.map_verdict(a, b).startswith(("map-agrees", "map-silent")):
            return "reject:map-disagrees"
        return "accept"

    def decide(self, dropped):
        taken = defaultdict(list)
        for a, (b, ev) in self.candidates().items():
            name = self.name_of(a)
            status = ev if ev.startswith("reject:") else self.check(a, b, name, dropped)
            self.rows[(b, a)] = [self.labels.get(b, "?"), a, name, status, ev,
                                 self.map_verdict(a, b)]
            if status == "accept":
                taken[b].append((a, name, ev))
        by_name = Counter(v[0][1] for v in taken.values() if len(v) == 1)
        for b, got in taken.items():
            if len(got) > 1:
                for a, _n, _e in got:
                    self.rows[(b, a)][3] = "reject:two-smt3-functions"
                continue
            a, name, ev = got[0]
            held = next((s for s, n in self.symbols.items() if n == name and s != b), None)
            if by_name[name] > 1:
                self.rows[(b, a)][3] = "reject:duplicate-target"
            elif held is not None:
                self.rows[(b, a)][3] = f"reject:collision-with-0x{held:08X}"
            else:
                self.accepted[b] = (a, name, ev)

    def drop(self, b, status):
        a = self.accepted.pop(b)[0]
        self.rows[(b, a)][3] = status

    def write(self, out_dir):
        v = self.version
        names = [f"0x{b:08X}\t{name}\tinferred\t{self.name_source(a)} via SLUS_209.11 "
                 f"0x{a:08X}, {ev}; {self.rows[(b, a)][5]}"
                 for b, (a, name, ev) in sorted(self.accepted.items())]
        (out_dir / f"names_{v}.tsv").write_text("\n".join(names) + "\n")
        head = [f"# SMT3 function names for {v} ({SERIAL[v]}); tools/raidou_names.py.",
                "# addr\tcurrent\tsmt3_addr\tsmt3_name\tstatus\tevidence\tmap"]
        body = [f"0x{b:08X}\t{cur}\t0x{a:08X}\t{n}\t{st}\t{ev}\t{mv}"
                for (b, _a), (cur, a, n, st, ev, mv) in sorted(self.rows.items())]
        (out_dir / f"pairs_{v}.tsv").write_text("\n".join(head + body) + "\n")
        return Counter(r[3] for r in self.rows.values())


def transitivity_drops(g1, g2, r1_r2):
    """SMT3 functions whose raidou2 counterpart depends on the route taken."""
    c1, c2 = g1.candidates(), g2.candidates()
    dropped = set()
    for a, (b1, ev1) in c1.items():
        if ev1.startswith("reject:") or a not in c2 or c2[a][1].startswith("reject:"):
            continue
        via = r1_r2.get(b1)
        if via and via[1] == "high" and via[0] != c2[a][0]:
            dropped.add(a)
    return dropped


def propagate_twins(g1, g2, r1_r2, dropped):
    """Give a raidou1/raidou2 twin the name its sibling got; drop both on a conflict."""
    twins = {b1: b2 for b1, (b2, tier, ev) in r1_r2.items()
             if tier == "high" and ev.split("+")[0] in STRONG}
    back = {b2: b1 for b1, b2 in twins.items()}
    carried = Counter()
    for src, dst, fwd in ((g1, g2, twins), (g2, g1, back)):
        for b, (a, name, _ev) in sorted(src.accepted.items()):
            t = fwd.get(b)
            if t is None:
                continue
            got = dst.accepted.get(t)
            if got is not None:
                if got[1] != name:
                    src.drop(b, "reject:twin-conflict")
                    dst.drop(t, "reject:twin-conflict")
                continue
            if any(row[3] == "accept" for (bb, _), row in dst.rows.items() if bb == t):
                continue
            if any(n == name for _a, n, _e in dst.accepted.values()):
                continue
            ev = f"twin-of-{src.version}-0x{b:08X}"
            status = dst.check(a, t, name, dropped)
            dst.rows[(t, a)] = [dst.labels.get(t, "?"), a, name, status, ev,
                                dst.map_verdict(a, t)]
            if status == "accept":
                dst.accepted[t] = (a, name, ev)
                carried[dst.version] += 1
    return carried


def apply(version, names_file):
    """Write accepted names into symbol_addrs.txt and name_sources.txt."""
    sym = ROOT / "config" / version / "symbol_addrs.txt"
    src = ROOT / "config" / version / "name_sources.txt"
    new = {}
    notes = {}
    for line in Path(names_file).read_text().splitlines():
        if line.strip():
            addr, name, kind, note = line.split("\t")
            new[int(addr, 16)] = name
            notes[int(addr, 16)] = f"{addr}\t{name}\t{kind}\t{note}"
    lines = sym.read_text().splitlines()
    keep = [l for l in lines if not ((m := ROW.match(l)) and int(m[2], 16) in new)]
    if MARKER not in keep:
        keep.append(MARKER)
    at = keep.index(MARKER) + 1
    block = [l for l in keep[at:] if ROW.match(l)]
    block += [f"{n} = 0x{a:08X}; // type:func" for a, n in new.items()]
    block.sort(key=lambda l: int(ROW.match(l)[2], 16))
    names = Counter(ROW.match(l)[1] for l in keep[:at] + block if ROW.match(l))
    if dup := [n for n, c in names.items() if c > 1]:
        sys.exit(f"{version}: names used twice: {dup[:10]}")
    sym.write_text("\n".join(keep[:at] + block) + "\n")
    if src.exists():
        for line in src.read_text().splitlines():
            if line.strip() and not line.startswith("#"):
                notes.setdefault(int(line.split("\t")[0], 16), line)
    src.write_text("# addr\tname\tevidence|inferred\tnote\n"
                   + "\n".join(notes[a] for a in sorted(notes)) + "\n")
    print(f"{version}: {len(new)} names written")


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--raidou1", nargs=2, metavar=("PAIRS", "CACHE"))
    ap.add_argument("--raidou2", nargs=2, metavar=("PAIRS", "CACHE"))
    ap.add_argument("--raidou1-raidou2", metavar="PAIRS")
    ap.add_argument("--map", nargs=2, metavar=("RAIDOU1_MAP", "RAIDOU2_MAP"),
                    help="smt3-port's raidou<n>_map.tsv files, the independent cross-check")
    ap.add_argument("--smt3-decomp", type=Path, default=Path.home() / "projects/smt3-decomp")
    ap.add_argument("--out-dir", type=Path, default=ROOT / "build" / "raidou_names")
    ap.add_argument("--apply", action="store_true",
                    help="write out-dir's names_<v>.tsv into config/<v>/")
    args = ap.parse_args()
    if args.apply:
        for v in ("raidou1", "raidou2"):
            apply(v, args.out_dir / f"names_{v}.tsv")
        return 0
    if not (args.raidou1 and args.raidou2 and args.raidou1_raidou2 and args.map):
        ap.error("--raidou1, --raidou2, --raidou1-raidou2 and --map are required")
    smt3_elf = args.smt3_decomp / "orig/smt3/SLUS_209.11"
    repo = smt3_repo_names(args.smt3_decomp)
    g1 = Game("raidou1", *args.raidou1, smt3_elf, repo, args.map[0])
    g2 = Game("raidou2", *args.raidou2, smt3_elf, repo, args.map[1])
    r1_r2 = read_pairs(args.raidou1_raidou2)
    dropped = transitivity_drops(g1, g2, r1_r2)
    for g in (g1, g2):
        g.decide(dropped)
    carried = propagate_twins(g1, g2, r1_r2, dropped)
    args.out_dir.mkdir(parents=True, exist_ok=True)
    for g in (g1, g2):
        counts = g.write(args.out_dir)
        print(f"{g.version}: accepted={len(g.accepted)}  "
              + "  ".join(f"{k}={n}" for k, n in sorted(counts.items()))
              + f"  (carried from twin: {carried[g.version]})")
        verdicts = Counter(r[5].split("-0x")[0] for r in g.rows.values()
                           if not r[4].startswith("reject:"))
        print(f"  map verdicts on non-rejected candidates: {dict(verdicts)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
