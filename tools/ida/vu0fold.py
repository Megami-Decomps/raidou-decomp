#!/usr/bin/env python3
"""vu0fold.py SRC DST | vu0fold.py -: fold runs of r5900_microcode_v2 helper statements in pc/SRC/*.c into the
ps2asm.h/fpu.h macros of vu0cat.json, writing pc/DST/*.c ('-' filters stdin to stdout). Also usable as a module: fold(text)."""
import json, os, re, sys

W = os.path.expanduser("~/.local/state/raidou-decomp-work/ida-pseudo")  # vu0cat.json and pc/ live in the work dir
NO_DEST = {"qmtc2", "ctc2", "mult1", "multu1", "div1", "divu1", "madd1", "maddu1", "mthi1", "mtlo1",
           "pmthi", "pmtlo", "pmthl", "mtsab", "mtsah", "pmaddw", "pmsubw", "pmadduw", "pdivw",
           "pdivuw", "pdivbw", "pmaddh", "pmsubh", "phmadh", "phmsbh"}
EE_HELPER = {"qmfc2", "qmtc2", "cfc2", "ctc2", "mult1", "multu1", "div1", "divu1", "madd1", "maddu1",
             "mfhi1", "mflo1", "mthi1", "mtlo1", "pmfhi", "pmflo", "pmthi", "pmtlo", "pmfhl", "pmthl",
             "mtsab", "mtsah", "qfsrv", "plzcw"}
SKIP_OPERANDS = {"$ACC", "$Q", "$I", "$R"}
CALL = re.compile(r"(__\w+)\(")
WRAP = re.compile(r"^(?:COERCE_FLOAT|COERCE_INT|LODWORD|SLODWORD)\((.*)\)$", re.S)
CAST = re.compile(r"^\((?:unsigned |signed )?(?:int|__int32|_DWORD|u32|s32|unsigned int)\)\s*")


def split_args(s):
    out, depth, cur = [], 0, ""
    for c in s:
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += c
    if cur.strip():
        out.append(cur.strip())
    return out


def strip_value(t):
    t = t.strip()
    while True:
        m = WRAP.match(t)
        if m and balanced(m.group(1)):
            t = m.group(1).strip()
            continue
        m = CAST.match(t)
        if m:
            t = t[m.end():].strip()
            continue
        if t.startswith("(") and t.endswith(")") and balanced(t[1:-1]):
            t = t[1:-1].strip()
            continue
        return t


def balanced(s):
    d = 0
    for c in s:
        d += c == "("
        d -= c == ")"
        if d < 0:
            return False
    return d == 0


def parse_call(expr):
    """'__x(a, b)' (after strip_value) -> (name, [args]) or None."""
    e = strip_value(expr)
    m = re.match(r"^(__\w+)\((.*)\)$", e, re.S)
    if not m or not balanced(m.group(2)):
        return None
    return m.group(1), split_args(m.group(2))


def flatten(expr, dest, ops, tmp):
    """Append (name, args, dest) for EXPR, nested helper calls first; args are text or temp ids."""
    c = parse_call(expr)
    if c is None:
        return False
    name, args = c
    out = []
    for a in args:
        if parse_call(a) is not None:
            tmp[0] += 1
            t = "@t%d" % tmp[0]
            if not flatten(a, t, ops, tmp):
                return False
            out.append(t)
        else:
            out.append(a)
    ops.append((name, out, dest))
    return True


STMT = re.compile(r"^(\s*)(?:([^=;]+?) = )?(.+);\s*$")


def parse_stmt(line):
    m = STMT.match(line)
    if not m or "__" not in m.group(3):
        return None
    lhs = m.group(2)
    if lhs and (lhs.startswith(("return", "if", "while")) or "(" in lhs and not lhs.startswith("*")):
        return None
    if m.group(3).startswith("return"):
        return None
    ops = []
    if not flatten(m.group(3), lhs.strip() if lhs else None, ops, [0]):
        return None
    return m.group(1), ops, line.strip()


def num(o):
    v = re.search(r"\$v[fi](\d+)", o)
    return int(v.group(1)) if v else None


class Template:
    """One vu0cat macro, ops turned into helper form plus the non-helper ops kept as flow steps."""

    def __init__(self, t):
        self.name, self.params, self.uses = t["name"], t["params"], t["uses"]
        self.operands = t["operands"]
        self.mparams = {e for c, e in self.operands if "m" in c and e}
        self.steps = []          # ("op", name, args, dest) | ("alias", reg, ("val"|"deref"|"out", k))
        self.ok = True
        for op in t["ops"]:
            self._add(op)
        self.nops = sum(1 for s in self.steps if s[0] == "op")
        if not self.nops or all(s[1] == "___word" for s in self.steps if s[0] == "op"):
            self.ok = False  # a .word alone (vsqrt, vclip...) prints as ___word() and is ambiguous

    def _add(self, op):
        m, ops = op[0], op[1:]
        base = m.split(".")[0]
        if m == ".word":
            txt = ", ".join(ops)
            if "#" not in txt:
                self.steps.append(("op", "___word", [], None))
                return
            # the instruction rabbitizer could only print as .word, decoded the way r5900 v2 now names it
            parts = txt.split("#")[1].strip().split(None, 1)
            self._add([parts[0]] + ([o.strip() for o in parts[1].split(",")] if len(parts) > 1 else []))
            return
        if base in ("lqc2", "sqc2"):
            a = re.fullmatch(r"(-?0x[0-9a-fA-F]+|\d+)\((%\d+|\$\w+)\)", ops[1])
            if not a or not a.group(2).startswith("%"):
                self.ok = False
                return
            self.steps.append(("op", "__" + base, [num(ops[0]), ("addr", int(a.group(2)[1:]), int(a.group(1), 0))], None))
            return
        if base.startswith("v"):
            args = []
            for o in ops:
                if o in SKIP_OPERANDS:
                    continue
                n = num(o)
                if n is None:
                    self.ok = False
                    return
                args.append(n)
            self.steps.append(("op", "__" + m.replace(".", "_"), args, None))
            return
        if base in ("mfc1", "mtc1", "lw") and len(ops) == 2:
            reg, src = ops
            if base == "lw":
                a = re.fullmatch(r"0x0\((%\d+)\)", src)
                if not a or not reg.startswith("$"):
                    self.ok = False
                    return
                self.steps.append(("alias", reg, ("deref", int(a.group(1)[1:]))))
            elif base == "mfc1" and src.startswith("%"):
                self.steps.append(("alias", reg, ("val", int(src[1:]))))
            elif base == "mtc1" and src.startswith("%"):
                self.steps.append(("alias", reg, ("out", int(src[1:]))))
            else:
                self.ok = False
            return
        if base in EE_HELPER or base.startswith("p"):
            dest, args = None, []
            for i, o in enumerate(ops):
                if o in SKIP_OPERANDS:
                    continue
                if o.startswith("%") or (o.startswith("$") and num(o) is None):
                    g = "$0" if o == "$zero" else o
                    if i == 0 and base not in NO_DEST:
                        dest = g
                    else:
                        args.append(0 if g == "$0" else ("reg", g))
                elif num(o) is not None:
                    args.append(num(o))
                else:
                    try:
                        args.append(int(o, 0))
                    except ValueError:
                        self.ok = False
                        return
            self.steps.append(("op", "__" + m.replace(".", "_"), args, dest))
            return
        self.ok = False

    def match(self, ops, i):
        """Match the steps against ops[i:]; return (count, {param: text}) or None."""
        env = {}        # register -> current text value or ("val"|"deref", k)
        bind = {}       # operand index -> text
        j = i

        def setk(k, text):
            if text is None:
                return False
            if k in bind:
                return norm(bind[k]) == norm(text)
            bind[k] = text
            return True

        for s in self.steps:
            if s[0] == "alias":
                _, reg, (kind, k) = s
                if kind == "out":
                    if reg not in env or not isinstance(env[reg], str):
                        return None
                    if not setk(k, env[reg]):
                        return None
                else:
                    env[reg] = (kind, k)
                continue
            if j >= len(ops):
                return None
            name, args, dest = ops[j]
            _, tname, targs, tdest = s
            if name != tname or len(args) != len(targs):
                return None
            for a, t in zip(args, targs):
                if isinstance(t, int):
                    if strip_value(a) not in (str(t), hex(t)):
                        return None
                elif t[0] == "addr":
                    k, off = t[1], t[2]
                    v = strip_value(a)
                    if off == 0:
                        if not setk(k, v):
                            return None
                    elif k not in bind:
                        return None
                else:
                    reg = t[1]
                    if reg.startswith("%"):
                        cur = env.get(reg)
                        if isinstance(cur, str):
                            if cur != a:
                                return None
                        elif not setk(int(reg[1:]), strip_value(a)):
                            return None
                    else:
                        cur = env.get(reg)
                        if isinstance(cur, str):
                            if cur != a:
                                return None
                        elif isinstance(cur, tuple):
                            kind, k = cur
                            v = strip_value(a)
                            if kind == "deref":
                                v = re.sub(r"^\*\s*(?:\([^()]*\*\)\s*)?", "", v, count=1) if v.startswith("*") else None
                                v = strip_value(v) if v else None
                            if not setk(k, v):
                                return None
                        else:
                            return None
            if tdest is not None:
                if dest is None:
                    return None
                env[tdest] = dest
            elif dest is not None:
                return None
            j += 1
        for reg, v in env.items():
            if reg.startswith("%") and isinstance(v, str):
                if not setk(int(reg[1:]), v):
                    return None
        res = {}
        for p in self.params:
            ks = [k for k, (c, e) in enumerate(self.operands) if e == p and k in bind]
            if not ks:
                return None
            v = bind[ks[0]]
            if v.startswith("@t"):
                return None
            if p in self.mparams and v.startswith("&"):
                v = v[1:].strip()
            res[p] = v
        named = {k for k, (c, e) in enumerate(self.operands) if e}
        consts = sum(1 for k, v in bind.items() if k not in named and re.fullmatch(r"-?[\d.]+f?", v))
        return j - i, res, consts


def norm(t):
    return re.sub(r"\s+", "", strip_value(t))


def load_templates():
    ts = [Template(t) for t in json.load(open(os.path.join(W, "vu0cat.json")))]
    return [t for t in ts if t.ok]


TEMPLATES = None


QCOPY = re.compile(r"^(\s*)\*\(_OWORD \*\)(.+?) = \*\((?:unsigned )?__int64 \*\)(.+);\s*$")


def fold_qcopy(line):
    """v2 prints lq+sq through a register as an _OWORD store of a 64-bit view; that is COPYVECTOR."""
    m = QCOPY.match(line)
    if not m or not balanced(m.group(2)) or not balanced(m.group(3)):
        return line
    return "%sSDF_VU0_COPYVECTOR(%s, %s);" % (m.group(1), strip_value(m.group(2)), strip_value(m.group(3)))


NEUTRAL = re.compile(r"^\s*[A-Za-z_]\w* (?:[-+*/|&^]|<<|>>)?= [^;*\[\]()]*;\s*$")
MAX_GAP = 3


def idents(text):
    return set(re.findall(r"[A-Za-z_]\w*", text))


def plain_fold(lines):
    out, i = [], 0
    while i < len(lines):
        run, k = [], i
        while k < len(lines) and (p := parse_stmt(lines[k])) is not None:
            run.append(p)
            k += 1
        if not run:
            out.append(lines[i])
            i += 1
            continue
        out.extend(fold_run(run))
        i = k
    return out


def helpers_left(lines):
    return sum(1 for l in lines if parse_stmt(l) is not None)


def fold(text):
    """Fold helper runs. Up to MAX_GAP register-only statements between helpers are hoisted above the
    run when they share no identifier with it and the hoist folds more helper statements."""
    global TEMPLATES
    text = "\n".join(fold_qcopy(l) for l in text.split("\n"))
    if TEMPLATES is None:
        TEMPLATES = load_templates()
    lines = text.split("\n")
    out = []
    i = 0
    while i < len(lines):
        if parse_stmt(lines[i]) is None:
            out.append(lines[i])
            i += 1
            continue
        run, gaps, k, end = [], [], i, i
        while k < len(lines):
            p = parse_stmt(lines[k])
            if p is not None:
                run.append(p)
                k += 1
                end = k
                continue
            j = k
            while j < len(lines) and j - k < MAX_GAP and parse_stmt(lines[j]) is None and NEUTRAL.match(lines[j]):
                j += 1
            if j > k and j < len(lines) and parse_stmt(lines[j]) is not None:
                gaps.extend(lines[k:j])
                k = j
                continue
            break
        plain = plain_fold(lines[i:end])
        if gaps:
            used = set().union(*(idents(r[2]) for r in run))
            hoisted = [] if any(idents(g.split("=")[0]) & used or idents(g) & used for g in gaps) else gaps + fold_run(run)
            if hoisted and helpers_left(hoisted) < helpers_left(plain):
                plain = hoisted
        out.extend(plain)
        i = end
    return "\n".join(out)


def fold_run(run):
    """RUN: [(indent, ops)] for consecutive helper statements. Matches must end on a statement edge."""
    ops, edge, owner = [], set(), []
    for n, (_, o, _) in enumerate(run):
        ops.extend(o)
        owner.extend([n] * len(o))
        edge.add(len(ops))
    lhs = [{d for _, _, d in o if d and not d.startswith("@t")} for _, o, _ in run]
    out = []
    i = 0
    while i < len(ops):
        best = None
        if i == 0 or i in edge:
            for t in TEMPLATES:
                m = t.match(ops, i)
                if not m or (i + m[0]) not in edge:
                    continue
                used = set().union(*(lhs[s] for s in set(owner[i:i + m[0]])))
                if not used <= set(m[1].values()):
                    continue
                key = (m[0], m[2], t.uses)
                if best is None or key > best[0]:
                    best = (key, t, m)
        if best:
            _, t, (n, res, _) = best
            out.append("%s%s(%s);" % (run[owner[i]][0], t.name, ", ".join(res[p] for p in t.params)))
            i += n
        else:
            s = run[owner[i]]
            out.append(s[0] + s[2])
            i = next(e for e in sorted(edge) if e > i)
    return out


if __name__ == "__main__":
    if sys.argv[1] == "-":
        sys.stdout.write(fold(sys.stdin.read()))
        sys.exit(0)
    src, dst = sys.argv[1], sys.argv[2]
    sd, dd = os.path.join(W, "pc", src), os.path.join(W, "pc", dst)
    os.makedirs(dd, exist_ok=True)
    n = 0
    for f in sorted(os.listdir(sd)):
        if f.endswith(".c"):
            t = open(os.path.join(sd, f)).read()
            ft = fold(t)
            n += ft.count("\n") != t.count("\n")
            open(os.path.join(dd, f), "w").write(ft)
    print(n, "files changed")
