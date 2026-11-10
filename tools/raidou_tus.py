#!/usr/bin/env python3
"""Assign Raidou functions to translation units from carried evidence.

    python3 tools/raidou_tus.py raidou1 [--write]

Evidence, strongest first:
  * smt3        The function pairs with a function of an SMT3 unit
                (smt3-decomp's yaml), through config/<v>/name_sources.txt or
                the cross-game map (raidou<N>_map.tsv). An SMT3 unit whose
                paired functions form one run, with no other unit's function
                inside, is a Raidou TU from its first to its last paired
                function; unpaired functions inside the run belong to it.
  * dds         The same for DDS-only code, through dds_unique_in_raidou.tsv
                and dds-decomp's units; its code_<vram> files (DDS1's first)
                give TUs named code_<Raidou vram>.
  * literal     find_tus.py's same-TU cores (shared strings, jump tables,
                .lit4 constants) extend a TU over unpaired neighbours, and a
                duplicate string inside a TU marks a conflict.
Pairs of functions under 6 words are not used: tiny bodies recur in many units.
Functions with no evidence stay in code_<vram> gaps (asm units).

Writes build/<v>/tu_plan.tsv, and with --write the .text subsegments of the
yaml and docs/tus-<v>.tsv (unit, range, evidence, SMT3 unit).
"""
from __future__ import annotations

import argparse
import bisect
import collections
import csv
import re
from pathlib import Path

import yaml

from find_tus import VERSIONS, Image, literal_evidence

ROOT = Path(__file__).resolve().parent.parent
SMT3 = Path.home() / "projects/smt3-decomp"
DDS = Path.home() / "projects/dds-decomp"
DDS_CODE = "dds-code:"  # label of a dds-decomp code_<vram> unit; the Raidou TU is named code_<its vram>
MAP = Path.home() / ".local/state/smt3-port-work/raidou-map"
MIN_WORDS = 6
# No TU spans more functions; a longer order inversion is a link-order anomaly.
MAX_SPAN = 400
# g++ puts inline and template functions in .gnu.linkonce.t sections, which link
# after all of .text while their literals stay in each object's .rodata.
LINKONCE_TEXT = {"raidou2": 0x004179A8}
GLABEL = re.compile(r"^glabel (\w+)\n(?:.*\n)*?\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ", re.M)
INSN = re.compile(r"\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ")
SYMREF = re.compile(r"%(?:lo|gp_rel)\(((?:D_|jtbl_)([0-9A-F]{8}))(?:\s*\+\s*(0x[0-9A-F]+|\d+))?\)")


def main_segment(cfg):
    return next(s for s in cfg["segments"] if isinstance(s, dict) and s.get("subsegments"))


def text_units(cfg_path: Path) -> list[tuple[int, str]]:
    """(vram, name) of each code subsegment, ending with the .vutext start."""
    main = main_segment(yaml.safe_load(cfg_path.read_text()))
    delta = main["vram"] - main["start"]
    out = []
    for s in main["subsegments"]:
        if isinstance(s, list) and len(s) >= 3:
            out.append((s[0] + delta, s[1], s[2]))
            if s[1] == "textbin":
                break
    return [(v, k, n) for v, k, n in out]


def functions_and_refs(version: str, text_end: int):
    """Function starts and their literal references, from every split .s file."""
    starts, refs = set(), collections.defaultdict(set)
    asm = ROOT / "asm" / version
    for path in sorted(asm.rglob("*.s")):
        if path.relative_to(asm).parts[0] == "data":
            continue
        cur, pending = None, False
        for line in path.read_text().splitlines():
            if line.startswith("glabel "):
                pending = True
                continue
            m = INSN.match(line)
            if m and pending:
                cur, pending = int(m.group(1), 16), False
                if cur < text_end:
                    starts.add(cur)
            if cur is None or cur >= text_end:
                continue
            for full, addr, off in SYMREF.findall(line):
                refs[cur].add((full.startswith("jtbl_"), int(addr, 16) + (int(off, 0) if off else 0)))
    return sorted(starts), refs


def smt3_unit_of():
    units = [(v, n) for v, k, n in text_units(SMT3 / "config/smt3/SLUS_209.11.yaml") if k in ("c", "asm")]
    addrs = [v for v, _ in units]
    end = next(v for v, k, _ in text_units(SMT3 / "config/smt3/SLUS_209.11.yaml") if k == "textbin")

    def unit(ea: int) -> str | None:
        if not addrs[0] <= ea < end:
            return None
        return units[bisect.bisect_right(addrs, ea) - 1][1]
    return unit


def pairs(version: str, starts: list[int]):
    """Per Raidou function: (unit, source, SMT3 partner address, SMT3 unit)."""
    unit = smt3_unit_of()
    label: dict[int, tuple[str, str]] = {}
    # raidou-decomp's own accepted names first; they carry the SMT3 address.
    for line in (ROOT / "config" / version / "name_sources.txt").read_text().splitlines():
        m = re.match(r"0x([0-9A-F]{8})\t\S+\t\S+\t.*via SLUS_209\.11 0x([0-9A-F]{8})", line)
        if m and (u := unit(int(m.group(2), 16))) and "code_" not in u:
            label[int(m.group(1), 16)] = (u, "smt3:names", int(m.group(2), 16), u)
    words = {}
    with open(MAP / f"{version}_map.tsv") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            ea = int(row["ea"], 16)
            words[ea] = int(row["words"])
            if row["method"] and row["smt3_ea"] and ea not in label:
                if (u := unit(int(row["smt3_ea"], 16))) and "code_" not in u:
                    label[ea] = (u, f"smt3:map-{row['method']}", int(row["smt3_ea"], 16), u)
    with open(MAP / "dds_unique_in_raidou.tsv") as f:
        for row in csv.DictReader(f, delimiter="\t"):
            if row["game"] != version:
                continue
            ea = int(row["ea"], 16)
            words.setdefault(ea, int(row["words"]))
            if ea in label:
                continue
            names = {row[f"{d}_unit"] for d in ("dds1", "dds2")
                     if row[f"{d}_unit"] and row[f"{d}_method"] and "code_" not in row[f"{d}_unit"]}
            if len(names) == 1:
                label[ea] = (names.pop(), "dds", None, None)
            elif not names:
                # dds-decomp's code_<vram> files are its TUs too, named per game: DDS1's when it has one.
                d = next((d for d in ("dds1", "dds2") if row[f"{d}_unit"] and row[f"{d}_method"]), None)
                if d:
                    label[ea] = (f"{DDS_CODE}{d}:{row[f'{d}_unit']}", "dds", None, None)
    for i, ea in enumerate(starts):
        words.setdefault(ea, ((starts[i + 1] if i + 1 < len(starts) else ea) - ea) // 4)
    label = {ea: l for ea, l in label.items() if words.get(ea, 0) >= MIN_WORDS}
    # In the SDK region the archive member is the object; SMT3's units there
    # are whole archives, so they only name the SMT3 unit.
    sdk = [r for r in csv.reader((l for l in (ROOT / "config" / version / "sdk_objects.tsv").open()
                                  if not l.startswith("#")), delimiter="\t")][1:]
    lo, hi = int(sdk[0][0], 16), int(sdk[-1][0], 16)
    for addr, _, _, lib, member, how, *_ in sdk:
        ea = int(addr, 16)
        old = label.pop(ea, None)
        if lib != "-" and member != "?":
            lib = lib.split("[")[0].removesuffix(".a")
            label[ea] = (f"sdk/{lib}/{member.removesuffix('.o')}", f"sdk:{how}", None, old and old[3])
    for ea in [ea for ea in label if lo <= ea <= hi and not label[ea][0].startswith("sdk/lib")]:
        del label[ea]
    return label


def runs_of(seq):
    runs = []
    for i, u in seq:
        if runs and runs[-1][0] == u:
            runs[-1][1].append(i)
        else:
            runs.append([u, [i]])
    return runs


def assign(starts, label, cores, boundaries):
    """Label runs -> TUs as [lo index, hi index, unit, smt3 unit, notes]."""
    seq = [(i, label[ea][0]) for i, ea in enumerate(starts) if ea in label]
    # A short run of one label inside another label's run is noise from a
    # recurring body: drop it, so the surrounding run joins up.
    dropped = []
    while True:
        runs = runs_of(seq)
        k = next((k for k in range(1, len(runs) - 1) if runs[k - 1][0] == runs[k + 1][0] and
                  (len(runs[k][1]) < min(len(runs[k - 1][1]), len(runs[k + 1][1])) or
                   len(runs[k][1]) == 1 and len(runs[k - 1][1]) + len(runs[k + 1][1]) > 2)), None)
        if k is None:
            break
        gone = set(runs[k][1])
        dropped += [(i, runs[k][0], f"inside {runs[k - 1][0]}") for i in sorted(gone)]
        seq = [(i, u) for i, u in seq if i not in gone]
    # Split runs where the unit cannot be one object: a duplicate string between
    # two members, or far more Raidou code between two members than SMT3 has.
    cut = set()
    for a, b, _ in boundaries:
        cut.add((a, b))
    pieces = []
    for u, members in runs_of(seq):
        cur = [members[0]]
        for p, q in zip(members, members[1:]):
            split = any(p <= a and b <= q for a, b in cut)
            pe, qe = label[starts[p]][2], label[starts[q]][2]
            if not split and pe is not None and qe is not None:
                ours, theirs = starts[q] - starts[p], qe - pe
                split = ours > max(theirs, 0) + 0x800 and ours > 2 * theirs
            if split:
                pieces.append([u, cur])
                cur = []
            cur.append(q)
        pieces.append([u, cur])
    count = collections.Counter(u for u, _ in pieces)
    largest = {}
    for u, m in pieces:
        if u not in largest or len(m) > len(largest[u]):
            largest[u] = m
    tus, gapped = [], []
    for u, m in pieces:
        if count[u] > 1 and len(m) < 2:
            gapped.append((m[0], u))
            continue
        name = u if m is largest[u] else f"{u}_{starts[m[0]]:08X}"
        eas = [label[starts[i]][2] for i in m]
        notes = [] if None in eas or eas == sorted(eas) else ["reordered"]
        if name != u:
            notes.append("part")
        tus.append([m[0], m[-1], name, u, notes])
    tus.sort()
    # Literal cores: a core is one object. Extend a TU over unpaired functions
    # of a core it shares; join pieces of one unit that a core spans.
    conflicts = []
    for clo, chi in cores:
        inside = [t for t in tus if t[0] <= chi and t[1] >= clo]
        if len(inside) > 1 and len({t[3] for t in inside}) == 1:
            first = inside[0]
            first[1] = inside[-1][1]
            first[2] = first[3]
            first[4] = sorted(set(sum((t[4] for t in inside), [])) - {"part"}) + ["joined by literal"]
            tus = [t for t in tus if t is first or t not in inside]
            inside = [first]
        if len(inside) == 1:
            t = inside[0]
            prev_hi = max((x[1] for x in tus if x[1] < t[0]), default=-1)
            next_lo = min((x[0] for x in tus if x[0] > t[1]), default=len(starts))
            t[0] = max(min(t[0], clo), prev_hi + 1)
            t[1] = min(max(t[1], chi), next_lo - 1)
        elif len(inside) > 1:
            conflicts.append(("core", clo, chi, ",".join(t[2] for t in inside)))
    for a, b, s in boundaries:
        for t in tus:
            # A boundary lies after function a and at or before function b.
            if t[0] <= a and b <= t[1]:
                conflicts.append(("dup-string", a, b, f"{t[2]} {s[:30]!r}"))
    return tus, conflicts, dropped + [(i, u, "gap: lone member of a split unit") for i, u in gapped]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version", choices=list(VERSIONS))
    ap.add_argument("--write", action="store_true")
    ap.add_argument("--gaps-only", action="store_true",
                    help="keep tu_evidence.tsv's TUs; only split its gaps into new code_<vram> TUs")
    args = ap.parse_args()
    v = args.version
    cfg_path = ROOT / "config" / v / f"{VERSIONS[v]['serial']}.yaml"
    units = text_units(cfg_path)
    text_end = next(a for a, k, _ in units if k == "textbin")
    first = next(a for a, k, n in units if k in ("c", "asm") and n != "hand/crt0")
    starts, refs = functions_and_refs(v, text_end)
    starts = [s for s in starts if s >= first]
    tail = LINKONCE_TEXT.get(v, text_end)
    refs = {f: r for f, r in refs.items() if first <= f < tail}
    img = Image(v)
    cores, boundaries = literal_evidence(img, starts, refs, max_span=MAX_SPAN)
    label = pairs(v, starts)
    tus, conflicts, dropped = assign(starts, label, cores, boundaries)
    for t in tus:
        if t[2].startswith(DDS_CODE):
            t[2] = f"code_{starts[t[0]]:08X}"

    smt3_names = {n for _, k, n in text_units(SMT3 / "config/smt3/SLUS_209.11.yaml")}

    def smt3_unit(lo, hi, base):
        units = collections.Counter(label[s][3] for s in starts[lo:hi + 1] if s in label and label[s][3])
        return units.most_common(1)[0][0] if units else base if base in smt3_names else ""
    rows = []  # (lo addr, hi addr (last function), unit, kind, evidence, smt3 unit)
    pos = 0
    for lo, hi, u, smt3_u, notes in sorted(tus):
        if lo > pos:
            rows.append((starts[pos], starts[lo - 1], f"code_{starts[pos]:08X}", "asm", "none", ""))
        srcs = collections.Counter(label[starts[i]][1].split("-")[0] for i in range(lo, hi + 1) if starts[i] in label)
        ev = "+".join(f"{s}:{c}" for s, c in srcs.most_common())
        ncore = sum(1 for c in cores if c[1] > c[0] and lo <= c[0] and c[1] <= hi)
        if ncore:
            ev += f" literal-cores:{ncore}"
        ev = " ".join([f"{ev} of {hi - lo + 1}"] + notes)
        rows.append((starts[lo], starts[hi], u, "c", ev, smt3_unit(lo, hi, smt3_u)))
        pos = hi + 1
    if pos < len(starts):
        rows.append((starts[pos], starts[-1], f"code_{starts[pos]:08X}", "asm", "none", ""))

    if args.gaps_only:
        rows = gaps_only(v, rows, starts)

    plan = ROOT / "build" / v / "tu_plan.tsv"
    plan.parent.mkdir(parents=True, exist_ok=True)
    with plan.open("w") as f:
        f.write("start\tlast_func\tunit\tkind\tevidence\tsmt3_unit\n")
        for r in rows:
            f.write(f"{r[0]:08X}\t{r[1]:08X}\t{r[2]}\t{r[3]}\t{r[4]}\t{r[5]}\n")
        for c in conflicts:
            f.write(f"# conflict {c[0]} {starts[c[1]]:08X}..{starts[c[2]]:08X} {c[3]}\n")
        for i, b, a in dropped:
            f.write(f"# dropped label {starts[i]:08X} {b}: {a}\n")
    ntu = sum(1 for r in rows if r[3] == "c")
    nfun = sum(1 for i, s in enumerate(starts) for r in rows if r[3] == "c" and r[0] <= s <= r[1])
    print(f"{v}: {len(starts)} functions, {ntu} TUs ({sum(1 for r in rows if r[5])} SMT3 units) "
          f"covering {nfun}, {len(rows) - ntu} gaps, {len(conflicts)} conflicts, {len(dropped)} dropped labels")
    print(f"wrote {plan.relative_to(ROOT)}")
    if args.write:
        write(v, cfg_path, rows)


HEADER = """\
# Translation units of .text, one row per subsegment from code_001001D8 to .vutext.
# NOT a splat config: tools/raidou_tus.py writes it and the yaml together.
# kind c: a TU with evidence; asm: a code_<vram> gap with none (it may hold
# several objects). evidence: per-function sources and their counts, then "of N"
# functions in the range. smt3: paired with smt3-decomp's unit (names =
# name_sources.txt, map = the cross-game map); dds: dds-decomp's unit; sdk:
# archive member by bytes or by name (sdk_objects.tsv); literal-cores: groups
# of functions that share strings, jump tables or .lit4 constants. Notes: part =
# a separate run of a unit that also occurs elsewhere (named <unit>_<vram>);
# reordered = paired functions not in SMT3 order; joined by literal = runs of
# one unit joined by a shared literal.
"""


def gaps_only(v, rows, starts):
    """The current TUs, with each current gap split by the new plan's code_<vram> TUs inside it."""
    old = []
    for line in (ROOT / "config" / v / "tu_evidence.tsv").read_text().splitlines():
        if line and not line.startswith(("#", "start")):
            c = line.split("\t")
            old.append((int(c[0], 16), int(c[1], 16), c[2], c[3], c[4], c[5] if len(c) > 5 else ""))
    index = {ea: i for i, ea in enumerate(starts)}
    out = []
    for lo, hi, unit, kind, ev, smt3_u in old:
        if kind == "c":
            out.append((lo, hi, unit, kind, ev, smt3_u))
            continue
        pos = lo
        for r in rows:
            if r[3] == "c" and r[2].startswith("code_") and lo <= r[0] and r[1] <= hi:
                if r[0] > pos:
                    out.append((pos, starts[index[r[0]] - 1], f"code_{pos:08X}", "asm", "none", ""))
                out.append(r)
                pos = starts[index[r[1]] + 1] if index[r[1]] + 1 < len(starts) else hi + 1
        if pos <= hi:
            out.append((pos, hi, f"code_{pos:08X}", "asm", "none", ""))
    return out


def write(v, cfg_path, rows):
    with (ROOT / "config" / v / "tu_evidence.tsv").open("w") as f:
        f.write(HEADER)
        f.write("start\tlast_func\tunit\tkind\tevidence\tsmt3_unit\n")
        for r in rows:
            f.write(f"{r[0]:08X}\t{r[1]:08X}\t{r[2]}\t{r[3]}\t{r[4]}\t{r[5]}\n")
    main = main_segment(yaml.safe_load(cfg_path.read_text()))
    delta = main["vram"] - main["start"]
    text = cfg_path.read_text()
    lines = text.splitlines(keepends=True)
    first = next(i for i, l in enumerate(lines) if re.match(r"\s+- \[0x[0-9A-F]+, (asm|c), (?!hand/)", l))
    end = next(i for i, l in enumerate(lines) if ", textbin, vutext]" in l)
    while lines[end - 1].lstrip().startswith("#"):
        end -= 1
    body = [f"      - [0x{r[0] - delta:X}, {r[3]}, {r[2]}]\n" for r in rows]
    cfg_path.write_text("".join(lines[:first] + body + lines[end:]))
    print(f"wrote config/{v}/tu_evidence.tsv and {cfg_path.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
