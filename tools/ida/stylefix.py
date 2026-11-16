#!/usr/bin/env python3
"""stylefix.py [-|FILE...]: rewrite Hex-Rays pseudocode into raidou-decomp spelling: s32/u8/f32 types,
NULL, no __fastcall or LL suffixes, and `struct X` where the decomp's own sources mostly write it so."""
import json, os, re, sys, glob

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
CACHE = os.path.expanduser("~/.cache/raidou-decomp/stylefix-tags.json")

# Longest spelling first so "unsigned __int8" wins over "__int8".
TYPES = [
    ("unsigned __int64", "u64"), ("unsigned __int32", "u32"), ("unsigned __int16", "u16"),
    ("unsigned __int8", "u8"), ("unsigned int", "u32"), ("unsigned short", "u16"),
    ("unsigned char", "u8"), ("signed int", "s32"), ("signed __int64", "s64"),
    ("signed __int16", "s16"), ("signed __int8", "s8"), ("signed char", "s8"),
    ("__int64", "s64"), ("__int32", "s32"), ("__int16", "s16"), ("__int8", "s8"),
    ("_QWORD", "u64"), ("_DWORD", "u32"), ("_WORD", "u16"), ("_BYTE", "u8"), ("_OWORD", "u128"),
    ("_BOOL4", "s32"), ("_BOOL2", "s16"), ("_BOOL1", "u8"), ("unsigned", "u32"),
    ("int", "s32"), ("short", "s16"), ("float", "f32"), ("double", "f64"),
]
TYPE_RE = re.compile(r"(?<![\w$])(" + "|".join(re.escape(a) for a, _ in TYPES) + r")(?![\w$])")
TYPE_MAP = dict(TYPES)


def struct_tags():
    """Tag -> True when the decomp's sources write `struct TAG` more often than bare TAG."""
    stamp = max(os.path.getmtime(p) for p in glob.glob(REPO + "/include/**/*.h", recursive=True))
    if os.path.exists(CACHE):
        c = json.load(open(CACHE))
        if c.get("stamp") == stamp:
            return c["tags"]
    tags = set()
    for f in glob.glob(REPO + "/include/**/*.h", recursive=True):
        tags.update(re.findall(r"\bstruct\s+(\w+)\s*\{", open(f, errors="replace").read()))
    text = "\n".join(open(f, errors="replace").read() for f in glob.glob(REPO + "/src/**/*.c", recursive=True)
                     + glob.glob(REPO + "/include/**/*.h", recursive=True))
    with_kw = {}
    for m in re.finditer(r"\bstruct\s+(\w+)\s*\*", text):
        with_kw[m.group(1)] = with_kw.get(m.group(1), 0) + 1
    bare = {}
    for m in re.finditer(r"(?<!struct )\b(\w+)\s*\*", text):
        if m.group(1) in tags:
            bare[m.group(1)] = bare.get(m.group(1), 0) + 1
    out = {t: with_kw.get(t, 0) > bare.get(t, 0) for t in tags}
    os.makedirs(os.path.dirname(CACHE), exist_ok=True)
    json.dump({"stamp": stamp, "tags": out}, open(CACHE, "w"))
    return out


TAGS = None


def fix_line(ln):
    # Leave comments alone: register and stack-slot hints stay as Hex-Rays wrote them.
    code, sep, comment = ln.partition("//")
    code = TYPE_RE.sub(lambda m: TYPE_MAP[m.group(1)], code)
    code = re.sub(r"\bnullptr\b", "NULL", code)
    code = re.sub(r"\b__fastcall\s+", "", code)
    code = re.sub(r"\b(0x[0-9A-Fa-f]+|\d+)(?:i64|ui64|LL|ULL|uLL)\b", r"\1", code)
    code = re.sub(r"(?<![\w$])(?<!struct )([A-Za-z_]\w*)(?=\s*\*|\s+\w+\s*[;,)=\[])",
                  lambda m: ("struct " + m.group(1)) if TAGS.get(m.group(1)) else m.group(1), code)
    return code + sep + comment


def fix(text):
    global TAGS
    if TAGS is None:
        TAGS = struct_tags()
    return "\n".join(fix_line(l) for l in text.split("\n"))


def main():
    args = sys.argv[1:] or ["-"]
    if args == ["-"]:
        sys.stdout.write(fix(sys.stdin.read()))
        return
    for p in args:
        t = open(p).read()
        open(p, "w").write(fix(t))


if __name__ == "__main__":
    main()
