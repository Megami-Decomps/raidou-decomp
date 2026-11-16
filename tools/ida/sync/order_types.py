#!/usr/bin/env python3
"""Reorder types/<v>.h until it compiles alone with clang (move a definition ahead of its first
by-value use, add forward typedefs, drop a duplicate typedef), then write <v>-sizes.tsv from
that one compile, so every size is the layout of the header IDA imports."""
import os, re, subprocess, sys
OUT = os.path.expanduser("~/.local/state/raidou-decomp-work/ida-pseudo/sync/types")
CC = ["clang", "--target=x86_64-linux-gnux32", "-w", "-ferror-limit=0", "-x", "c"]


def blocks_of(text):
    return [b for b in text.split("\n\n") if b.strip()]


def compile_errors(blocks):
    text = "\n\n".join(blocks) + "\n"
    r = subprocess.run(CC + ["-fsyntax-only", "-"], input=text, capture_output=True, text=True)
    starts, line = [], 1
    for b in blocks:
        starts.append(line)
        line += b.count("\n") + 2
    errs = []
    for m in re.finditer(r"<stdin>:(\d+):\d+: error: (.*)", r.stderr):
        ln = int(m.group(1))
        bi = max(i for i, s in enumerate(starts) if s <= ln)
        errs.append((bi, m.group(2)))
    return errs


def defines(b, name):
    t = re.sub(r"\{.*\}", "{}", b, flags=re.S).strip()
    if re.search(r"\b(struct|union|enum)\s+%s\s*\{" % name, b):
        return True
    if not t.startswith("typedef"):
        return False
    if re.search(r"\(\s*\*\s*%s\s*\)\s*\(" % name, t):
        return True
    return bool(re.search(r"\b%s\s*(\[[^\]]*\]\s*)*;$" % name, t)) and ("{" in b or not re.match(r"typedef\s+(struct|union)\s+\w+\s+%s\s*;$" % name, t))


def main(v):
    path = f"{OUT}/{v}.h"
    blocks = blocks_of(open(path).read())
    log = []
    ov = os.path.join(os.path.dirname(OUT), "types-overrides", v + ".h")
    if os.path.exists(ov):
        for b in blocks_of(re.sub(r"^/\*.*?\*/\s*", "", open(ov).read(), flags=re.S)):
            m = re.match(r"(?:typedef\s+)?(?:struct|union)\s+(\w+)\s*\{", b)
            name = m.group(1)
            hit = [i for i, x in enumerate(blocks) if defines(x, name) and "{" in x]
            for i in hit[1:]:
                blocks[i] = None
            if hit:
                blocks[hit[0]] = b
            else:
                blocks.append(b)
            blocks = [x for x in blocks if x is not None]
            log.append("override %s (replaced %d)" % (name, len(hit)))
    for it in range(400):
        errs = compile_errors(blocks)
        if not errs:
            break
        bi, msg = errs[0]
        m = re.match(r"(?:unknown type name|must use '(?:struct|union)' tag to refer to type) '(\w+)'", msg) or \
            re.match(r"field has incomplete type '(?:struct|union) (\w+)'", msg) or re.match(r"variable has incomplete type '(?:struct|union) (\w+)'", msg)
        if m:
            name = m.group(1)
            di = next((i for i, b in enumerate(blocks) if defines(b, name)), None)
            if di is not None and di > bi:
                blocks.insert(bi, blocks.pop(di))
                log.append("moved %s before block %d" % (name, bi))
                continue
            if "must use" in msg and di is not None and di < bi:
                tag = re.search(r"\b(struct|union)\s+%s\b" % name, blocks[di])
                blocks.insert(0, "typedef %s %s %s;" % (tag.group(1) if tag else "struct", name, name))
                log.append("forward typedef %s" % name)
                continue
            if "must use" in msg:
                blocks.insert(0, "typedef struct %s %s;" % (name, name))
                log.append("forward typedef %s" % name)
                continue
            log.append("undefined %s at block %d: dropped %s" % (name, bi, re.sub(r"\s+", " ", blocks[bi])[:100]))
            blocks.pop(bi)
            continue
        m = re.match(r"use of undeclared identifier '(\w+)'", msg)
        if m:
            di = next((i for i, b in enumerate(blocks) if re.match(r"(typedef\s+)?enum\b", b) and re.search(r"\b%s\s*[=,}]" % m.group(1), b)), None)
            if di is not None and di > bi:
                blocks.insert(bi, blocks.pop(di))
                log.append("moved enum with %s before block %d" % (m.group(1), bi))
                continue
        m = re.match(r"use of '(\w+)' with tag type that does not match previous declaration", msg)
        if m:
            fi = next((i for i, b in enumerate(blocks) if re.match(r"typedef\s+(struct|union)\s+%s\s+\w+\s*;$" % m.group(1), b.strip())), None)
            kind = re.match(r"(?:typedef\s+)?(struct|union)", blocks[bi])
            other = next((i for i, b in enumerate(blocks) if i != bi and re.search(r"\b(struct|union)\s+%s\s*\{" % m.group(1), b)), None)
            if other is not None and other < bi:
                log.append("duplicate dropped (other tag kind): %s" % re.sub(r"\s+", " ", blocks[bi])[:120])
                blocks.pop(bi)
                continue
            if fi is not None and fi < bi and kind and not blocks[fi].strip().startswith("typedef " + kind.group(1)):
                blocks[fi] = re.sub(r"^typedef\s+(struct|union)", "typedef " + kind.group(1), blocks[fi].strip())
                log.append("forward tag kind fixed for %s" % m.group(1))
                continue
        if msg.startswith("typedef redefinition") or msg.startswith("redefinition of"):
            log.append("duplicate dropped: %s" % re.sub(r"\s+", " ", blocks[bi])[:120])
            blocks.pop(bi)
            continue
        log.append("unhandled at block %d: %s: dropped %s" % (bi, msg, re.sub(r"\s+", " ", blocks[bi])[:100]))
        blocks.pop(bi)
    text = "\n\n".join(blocks) + "\n"
    open(path, "w").write(text)
    keys = []
    for b in blocks:
        t = re.sub(r"\{.*\}", "{}", b, flags=re.S)
        m = re.match(r"(?:typedef\s+)?(?:const\s+)?(struct|union)\s+(\w+)\s*\{", b)
        if m:
            keys.append("%s %s" % m.groups())
        m2 = re.search(r"(\w+)\s*;\s*$", t.strip()) if t.startswith("typedef") and "{" in b and not re.match(r"typedef\s+enum", b) else None
        if m2:
            keys.append(m2.group(1))
    keys = list(dict.fromkeys(keys))
    probe = text + "\n".join("int __sz_%d = sizeof(%s);" % (i, k) for i, k in enumerate(keys))
    r = subprocess.run(CC + ["-S", "-O0", "-o", "-", "-"], input=probe, capture_output=True, text=True)
    got = dict(re.findall(r"__sz_(\d+):\n\s*\.long\s+(\d+)", r.stdout))
    with open(f"{OUT}/{v}-sizes.tsv", "w") as f:
        for i, k in sorted(enumerate(keys), key=lambda x: x[1]):
            if str(i) in got:
                f.write("%s\t%s\n" % (k, got[str(i)]))
    open(f"{OUT}/{v}-order.log", "w").write("\n".join(log) + "\n")
    print(v, "iterations", it, "remaining errors", len(compile_errors(blocks)), "moves", sum(1 for l in log if l.startswith("moved")),
          "dropped", sum(1 for l in log if "dropped" in l), "sized", len(got), "of", len(keys))


if __name__ == "__main__":
    for v in sys.argv[1:] or ("raidou1", "raidou2"):
        main(v)
