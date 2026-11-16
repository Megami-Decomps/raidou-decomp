#!/usr/bin/env python3
"""propose_protos.py TSV OUT.json [--version raidou1] [--floor 0.8]: prototype candidates per row from
the HD sources and matched shape neighbours, never from the row's own or other-game definition."""
import csv, json, os, re, sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
HD = Path.home() / "projects/raidouhd-reconstructed-sources"
HDMAP = Path.home() / ".local/state/raidou-decomp-work/hdnames/functions.tsv"
sys.path.insert(0, str(REPO / "tools"))

DEF = re.compile(r"^((?:static\s+|inline\s+)*)([A-Za-z_][\w\s\*]*?[\s\*])(\w+)\s*\(([^;{}()]*(?:\([^()]*\)[^;{}()]*)*)\)\s*\{", re.M)


def hd_index():
    out = {}
    for f in sorted((HD / "src").glob("*.c")):
        text = f.read_text(errors="replace")
        for m in DEF.finditer(text):
            if "inline" in m.group(1):
                continue  # inline helpers are macros in the PS2 source
            ret, name, params = " ".join(m.group(2).split()), m.group(3), " ".join(m.group(4).split())
            out.setdefault(name, (ret, params, f.name))
    return out


def hd_names(col):
    """addr -> HD name from the HD name map (col 0 for raidou1, 1 for raidou2)."""
    out = {}
    for r in csv.reader(open(HDMAP), delimiter="\t"):
        if r and not r[0].startswith("#") and r[4] and r[col]:
            out[int(r[col], 16)] = r[4]
    return out


def lower_index(idx):
    return {k.lower(): k for k in idx}


def decomp_types(decl):
    """HD's C# port says bool where the PS2 source returns and passes s32."""
    return re.sub(r"\bbool\b", "s32", decl)


def hd_proto(idx, low, names, name, addr):
    for cand in (names.get(addr), name):
        if cand and cand in idx:
            ret, params, f = idx[cand]
            return decomp_types(f"{ret} {name}({params})"), f"{f}:{cand}"
    # module prefix spelt differently (sbtl/sBtl)
    k = low.get(name.lower())
    if k:
        ret, params, f = idx[k]
        return decomp_types(f"{ret} {name}({params})"), f"{f}:{k}"
    return None, None


def sim_proto(funcs, index, fs, name, version, floor):
    t = funcs.get(f"{version}:{name}")
    if t is None:
        return None, None
    import similar_index
    for ratio, c in fs.templates(funcs, index, t, top=8, cached=False):
        if c["name"] == name and c["version"] in ("raidou1", "raidou2"):
            continue
        if ratio < floor:
            break
        got = fs.definition(c)
        if not got:
            continue
        text = got[2]
        head = text[:text.index("{")]
        head = re.sub(r"/\*.*?\*/|//[^\n]*", " ", head, flags=re.S)
        head = " ".join(head.split())
        head = re.sub(r"^(static|inline|extern)\s+", "", head)
        head = re.sub(r"\b" + re.escape(c["name"]) + r"\s*\(", name + "(", head, count=1)
        return head, f"{c['version']}:{c['name']} {ratio:.3f}"
    return None, None


def main():
    tsv, out = sys.argv[1], sys.argv[2]
    version = sys.argv[sys.argv.index("--version") + 1] if "--version" in sys.argv else "raidou1"
    floor = float(sys.argv[sys.argv.index("--floor") + 1]) if "--floor" in sys.argv else 0.8
    rows = list(csv.DictReader(open(tsv), delimiter="\t"))
    idx = hd_index()
    low = lower_index(idx)
    names = hd_names(0 if version == "raidou1" else 1)
    import find_similar as fs
    funcs = fs.load()
    index = fs.Index(funcs)
    res = {}
    for r in rows:
        a = int(r["addr"], 16)
        hd, hd_from = hd_proto(idx, low, names, r["name"], a)
        sim, sim_from = sim_proto(funcs, index, fs, r["name"], version, floor)
        res[r["name"]] = dict(hd=hd, hd_from=hd_from, sim=sim, sim_from=sim_from)
    json.dump(res, open(out, "w"), indent=1)
    print(f"{len(res)} rows: hd {sum(bool(v['hd']) for v in res.values())}, sim {sum(bool(v['sim']) for v in res.values())}")


if __name__ == "__main__":
    main()
