#!/usr/bin/env python3
"""Extract the type definitions a raidou-decomp version's units see.
Preprocesses every src/<v> and src/shared unit with clang (x32: 32-bit pointers, 64-bit long long,
16-byte __int128 like ee-gcc's TImode), keeps top-level typedef/struct/union/enum definitions in
first-seen order, and writes types/<v>.h, types/<v>-conflicts.txt and types/<v>-sizes.tsv."""
import os, re, subprocess, sys
RD = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../.."))
DATA = os.path.expanduser("~/.local/state/raidou-decomp-work/ida-pseudo/sync")
OUT = DATA + "/types"
DEFS = ["-D__GNUC__=2", "-D__GNUC_MINOR__=96", "-Dmips", "-D__mips__", "-D__mips=3", "-D__mips64",
        "-D__mips_eabi", "-D__R5900__", "-D__ee__", "-D_LANGUAGE_C", "-DLANGUAGE_C", "-D__LANGUAGE_C",
        "-D__SIZE_TYPE__=unsigned int", "-D__PTRDIFF_TYPE__=int", "-DSKIP_ASM", '-DASM_ROOT="x/"']
CC = ["clang", "--target=x86_64-linux-gnux32", "-nostdinc", "-undef", "-w", "-I", RD + "/include", "-I", RD + "/src"]


def statements(text, heads=None):
    """Split preprocessed C into top-level statements, skipping function bodies; a function
    definition's head (text before its body) is appended to heads when given."""
    i, n, start, depth = 0, len(text), 0, 0
    while i < n:
        c = text[i]
        if c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            i = j + 1
            continue
        if c in "({[":
            if c == "{" and depth == 0 and re.search(r"\)\s*(__attribute__\s*\(\(.*\)\)\s*)?$", text[start:i]):
                if heads is not None:
                    heads.append(text[start:i].strip())
                d, j = 0, i
                while j < n:
                    if text[j] == "{":
                        d += 1
                    elif text[j] == "}":
                        d -= 1
                        if d == 0:
                            break
                    j += 1
                i = start = j + 1
                continue
            depth += 1
        elif c in ")}]":
            depth -= 1
        elif c == ";" and depth == 0:
            yield start, text[start:i + 1].strip()
            start = i + 1
        i += 1


def strip_attr(s):
    out, i = [], 0
    while True:
        j = s.find("__attribute__", i)
        if j < 0:
            return "".join(out) + s[i:]
        out.append(s[i:j])
        k = s.index("(", j)
        d = 0
        while True:
            d += {"(": 1, ")": -1}.get(s[k], 0)
            k += 1
            if d == 0:
                break
        i = k


def key_of(s):
    t = strip_attr(s)
    m = re.match(r"(?:typedef\s+)?(?:const\s+|volatile\s+)*(struct|union|enum)\s+(\w+)\s*\{", t)
    tag = "%s %s" % (m.group(1), m.group(2)) if m else None
    m = re.match(r"enum\s*\{\s*(\w+)", t)
    if m:
        tag = "enum <anon %s>" % m.group(1)
    if not t.startswith("typedef"):
        return tag, None
    body = re.sub(r"\{.*\}", "{}", t, flags=re.S)
    m2 = re.search(r"\(\s*\*\s*(\w+)\s*\)\s*\(", body) or re.search(r"(\w+)\s*(\[[^\]]*\]\s*)*;$", body.strip())
    return tag, m2.group(1) if m2 else None


def norm(s):
    return re.sub(r"\s+", " ", s)


def preprocess(u, v, strip):
    """Preprocessed text with linemarkers removed, and (offset, origin file) marks."""
    src = open(u).read()
    if strip:
        src = re.sub(r"^\s*#\s*define\s+VERSION_DDS\w*.*$", "", src, flags=re.M)
    r = subprocess.run(CC + DEFS + ["-DVERSION_" + v.upper(), "-iquote", os.path.dirname(u), "-E", "-x", "c", "-"],
                       input=src, capture_output=True, text=True)
    if r.returncode:
        return None, None
    out, marks, pos = [], [], 0
    for line in r.stdout.splitlines(keepends=True):
        m = re.match(r'# \d+ "([^"]*)"', line)
        if m:
            marks.append((pos, os.path.relpath(m.group(1), RD) if m.group(1).startswith("/") else m.group(1)))
            continue
        out.append(line)
        pos += len(line)
    return "".join(out), marks


def origin(marks, off):
    f = None
    for p, name in marks:
        if p > off:
            break
        f = name
    return f


def candidates(text, marks):
    for off, s in statements(text):
        if not s or not re.match(r"(typedef|struct|union|enum)\b", s):
            continue
        if not s.startswith("typedef") and not re.match(r"(struct|union|enum)\s+\w+\s*\{|enum\s*\{", strip_attr(s)):
            continue
        tag, tname = key_of(s)
        if tname and "[" in s and re.search(r"sizeof|offsetof|__builtin|\?", s):
            continue  # size or offset assertion typedef
        if not s.startswith("typedef"):
            s = re.sub(r"\}[^}]*;$", "};", s, flags=re.S)  # drop a variable declarator
        yield origin(marks, off), tag, tname, re.sub(r"\n\s*\n", "\n", s)


def used_keys(src, cands):
    """Keys of the definitions a unit's own source names, closed over the types they mention."""
    src = re.sub(r"/\*.*?\*/|//[^\n]*", " ", src, flags=re.S)
    by_ident = {}
    for _, tag, tname, s in cands:
        if "{" not in s:
            continue
        k = tag or tname
        for ident in {tag.split()[-1] if tag else None, tname} - {None}:
            by_ident.setdefault(ident, set()).add((k, s))
    seen, todo = set(), [w for w in set(re.findall(r"\w+", src)) if w in by_ident]
    while todo:
        w = todo.pop()
        for k, s in by_ident.get(w, ()):
            if k in seen:
                continue
            seen.add(k)
            todo.extend(x for x in set(re.findall(r"\w+", s)) if x in by_ident)
    return seen


def lineage_of(label, cache={}):
    """raidou (no borrowed layouts), dds1, dds2, or stripped (a borrowing unit read without its define)."""
    if label.endswith(" (stripped)"):
        return "stripped"
    if not label.endswith(" (as DDS)"):
        return "raidou"
    path = os.path.join(RD, label[:-len(" (as DDS)")])
    if path not in cache:
        m = re.search(r"^\s*#\s*define\s+VERSION_(DDS\d)", open(path).read(), re.M)
        cache[path] = m.group(1).lower() if m else "dds"
    return cache[path]


def lineage_variants(defs, chosen, stripped, rd):
    """A type gets its members as the units it comes from define them. A member whose definition
    there is another borrowed lineage's (#ifdef VERSION_DDS1/2) is emitted again as
    <name>_dds1/_dds2, and the parent refers to that."""
    def labels(e):
        return (e[4] - stripped) or (e[2] - stripped) or e[2]
    ident = {}
    for k, e in chosen.items():
        names = {k.split()[-1] if " " in k and not k.startswith("enum <") else k}
        if e[3]:
            names.add(e[3])
        for n in names:
            ident.setdefault(n, k)
    variants, vname_of, vlog = [], {}, []
    def suffix(e):
        lins = {lineage_of(l) for l in labels(e)} - {"stripped"}
        if len(lins) == 1 and lins != {"raidou"}:
            return lins.pop()
        return re.sub(r"\W", "_", os.path.splitext(os.path.basename(sorted(labels(e))[0].split(" ")[0]))[0])
    def resolve(text, ctx, own):
        for w in sorted(set(re.findall(r"\b\w+\b", text))):
            m = ident.get(w)
            if m is None or w in own or m.startswith("enum"):
                continue
            cands = [e for e in defs[m].values() if e[2] & ctx]
            if not cands:
                continue
            best = max(cands, key=lambda e: (len(e[2] & ctx), e is chosen[m]))
            if best is chosen[m] or norm(best[0]) == norm(chosen[m][0]) or chosen[m] in cands and len(chosen[m][2] & ctx) == len(best[2] & ctx):
                continue
            if suffix(best) not in ("dds1", "dds2"):
                continue  # a unit's own redefinition: same layout in different text, or an override's job
            key = (m, norm(best[0]))
            if key not in vname_of:
                sfx = suffix(best)
                vnames = {x for x in ident if ident[x] == m}
                vname_of[key] = sfx
                t = best[0]
                for x in vnames:
                    t = re.sub(r"\b%s\b" % x, "%s_%s" % (x, sfx), t)
                t = resolve(t, best[2] & ctx, {"%s_%s" % (x, sfx) for x in vnames})
                variants.append(t)
                vlog.append("%s\t%s_%s\tfrom %s" % (m, w, sfx, sorted(labels(best))[:3]))
            text = re.sub(r"\b%s\b" % w, "%s_%s" % (w, vname_of[key]), text)
        return text
    texts = {}
    for k, e in chosen.items():
        own = {x for x in ident if ident[x] == k}
        texts[k] = resolve(e[0], labels(e), own)
    return texts, variants, vlog


TI = re.compile(r"typedef\s+(signed\s+|unsigned\s+)?int\s+(\w+)\s+__attribute__\s*\(\(\s*(__)?mode(__)?\s*\(\s*(__)?TI(__)?\s*\)\s*\)\)\s*;")


def main(v):
    units = sorted(os.path.join(d, f) for top in (f"{RD}/src/{v}", f"{RD}/src/shared")
                   for d, _, fs in os.walk(top) for f in fs if f.endswith(".c"))
    defs, fwd, first, failed, pp = {}, {}, {}, [], {}
    n = 0
    # A unit that #defines VERSION_DDS1/2 borrows that title's layouts for its own code. Its types
    # are read twice: with the define (as it builds) and without it (what the headers give Raidou).
    borrow = {u for u in units if re.search(r"^\s*#\s*define\s+VERSION_DDS", open(u).read(), re.M)}
    runs = [(u, True) for u in units] + [(u, False) for u in units if u in borrow]
    for u, raidou in runs:
        text, marks = preprocess(u, v, raidou)
        if text is None:
            if raidou and u in borrow:
                continue
            failed.append(u)
            continue
        label = os.path.relpath(u, RD) + (" (as DDS)" if not raidou else " (stripped)" if u in borrow else "")
        # Use only preprocessings whose type definitions compile on their own.
        probe = "\n".join(c[3] for c in candidates(text, marks))
        if subprocess.run(CC + ["-x", "c", "-fsyntax-only", "-"], input=probe, capture_output=True, text=True).returncode:
            if raidou and u in borrow:
                continue
            failed.append(label + " (types do not compile)")
            continue
        pp[label] = (text, marks)
        cands = list(candidates(text, marks))
        used = used_keys(open(u).read(), cands)
        for org, tag, tname, s in cands:
            n += 1
            if "{" not in s:
                if tname:
                    fwd.setdefault(tname, s)
                    first.setdefault(tname, n)
                continue
            k = tag or tname
            first.setdefault(k, n)
            d = defs.setdefault(k, {})
            e = d.setdefault(norm(s), [s, org, set(), tname, set()])
            e[2].add(label)
            if k in used:
                e[4].add(label)
    # Choose: a definition that a unit's own code uses (names it, or names a type that contains
    # it) as it builds, then one seen by units that do not borrow a DDS version's layouts (#define
    # VERSION_DDS1/2), then one a borrowing unit compiles with, then the borrowing unit read
    # without its define (no unit builds that way), then a header's, then the one most units use.
    borrow = {k for k in pp if k.endswith(" (as DDS)")}
    stripped = {k for k in pp if k.endswith(" (stripped)")}
    chosen, conflicts = {}, []
    for k, d in defs.items():
        best = max(d.values(), key=lambda e: (len(e[4] - stripped) > 0, len(e[2] - borrow - stripped) > 0, len(e[2] & borrow) > 0,
                                               len(e[4] - stripped), e[1].startswith("include/"), len(e[2] - borrow - stripped), len(e[2])))
        chosen[k] = best
        if len(d) > 1:
            real = sum(1 for e in d.values() if e[4] - stripped) > 1
            conflicts.append((("USED " if real else "") + k, best[1], "%d units, %d using" % (len(best[2]), len(best[4] - stripped)),
                              [(e[1], len(e[2]), len(e[4] - stripped)) for e in d.values() if e is not best]))
    tnames = {e[3] for e in chosen.values() if e[3]}
    texts, variants, vlog = lineage_variants(defs, chosen, stripped, RD)
    out = []
    for k in sorted(set(chosen) | set(fwd), key=lambda k: first[k]):
        if k in chosen:
            out.append(texts[k])
        elif k not in tnames:
            out.append(fwd[k])
    out.extend(variants)
    with open(f"{OUT}/{v}-variants.txt", "w") as f:
        f.write("\n".join(vlog) + "\n")
    text = TI.sub(lambda m: "typedef %s__int128 %s;" % ("unsigned " if m.group(1) and "un" in m.group(1) else "", m.group(2)),
                  "\n\n".join(out))
    open(f"{OUT}/{v}.h", "w").write(text + "\n")
    with open(f"{OUT}/{v}-conflicts.txt", "w") as f:
        for k, org, nu, others in sorted(conflicts):
            f.write(f"{k}\tchosen {org} ({nu})\tothers (origin, units, using) {others}\n")
        for u in failed:
            f.write(f"preprocess failed\t{u}\n")
    # Sizes as clang lays them out, probed in a unit that sees the chosen definition; a unit whose
    # types do not compile alone is skipped for the next one.
    sizes, bad = {}, set()
    todo = {k: list(dict.fromkeys(sorted(e[4] - stripped) + sorted(e[2] - borrow - stripped) + sorted(e[2] & borrow) + sorted(e[2] & stripped)))
            for k, e in chosen.items()
            if k.startswith(("struct ", "union ")) or (e[3] and not k.startswith("enum "))}
    while todo:
        by_unit = {}
        for k, us in list(todo.items()):
            us = [u for u in us if u not in bad]
            if not us:
                del todo[k]
                continue
            todo[k] = us
            by_unit.setdefault(us[0], []).append(k)
        for u, keys in by_unit.items():
            text, marks = pp[u]
            probe = "\n".join(c[3] for c in candidates(text, marks)) + "\n" + "\n".join("int __sz_%d = sizeof(%s);" % (i, k) for i, k in enumerate(keys))
            r = subprocess.run(CC + ["-x", "c", "-S", "-O0", "-o", "-", "-"], input=probe, capture_output=True, text=True)
            got = dict(re.findall(r"__sz_(\d+):\n\s*\.long\s+(\d+)", r.stdout))
            if not got:
                bad.add(u)
                continue
            for i, k in enumerate(keys):
                if str(i) in got:
                    sizes[k] = int(got[str(i)])
                del todo[k]
    unsized = [k for k in chosen if k not in sizes and (k.startswith(("struct ", "union ")) or (chosen[k][3] and not k.startswith("enum ")))]
    for name in ("sizes", "unit-sizes"):
        with open(f"{OUT}/{v}-{name}.tsv", "w") as f:
            for k in sorted(sizes):
                f.write(f"{k}\t{sizes[k]}\n")
    print(v, "units", len(units), "borrowing", len(borrow), "preprocess failed", len(failed), "definitions", len(chosen), "forward only",
          len(set(fwd) - tnames - set(chosen)), "conflicting keys", len(conflicts), "sized", len(sizes), "unsized", len(unsized))


if __name__ == "__main__":
    import order_types
    for v in sys.argv[1:] or ("raidou1", "raidou2"):
        main(v)
        order_types.main(v)
