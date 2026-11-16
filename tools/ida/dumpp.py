#!/usr/bin/env python3
"""dumpp.py VER PROTOS.json LABEL=CHAIN [LABEL=CHAIN]: decompile sample rows with the own prototype hidden,
then with the first CHAIN source that applies (none, hd, sim, call; "!" rejects an arity change), into
pc/LABEL/NAME.c, restoring every type. SAMPLE picks the TSV. OFF=MOD removes that loaded Hex-Rays pass
for the run; HEXOPTIONS=0x... sets that Hex-Rays option word for the run (0x100831FD hides casts).
KEEP=1 leaves the chosen prototype applied (one variant); PORT=N targets that instance instead of VER's copy."""
import csv, json, os, sys, time
import ip, vu0fold

ver, protos_path = sys.argv[1], sys.argv[2]
variants = dict(a.split("=", 1) for a in sys.argv[3:])
rows = list(csv.DictReader(open(ip.WORK + "/" + os.environ.get("SAMPLE", "sample.tsv")), delimiter="\t"))
protos = json.load(open(protos_path))
addr_key = "addr_r2" if ver == "r2" and "addr_r2" in rows[0] else "addr"
jobs = []
for r in rows:
    p = protos.get(r["name"], {})
    jobs.append((r["name"], int(r[addr_key], 16), p))
outfile = ip.winpath(ip.WORK + f"/pc/_dumpp-{ver}.json")

CODE = r'''
import ida_hexrays, ida_lines, ida_nalt, ida_typeinf, ida_funcs, idautils, idc, json, re

JOBS = %r
VARIANTS = %r
KEEP = %r
TIL = ida_typeinf.get_idati()
KEYWORDS = set("const volatile struct union enum unsigned signed void char short int long float double bool".split())

def text(ea):
    hf = ida_hexrays.hexrays_failure_t()
    cf = ida_hexrays.decompile(ea, hf, ida_hexrays.DECOMP_NO_CACHE | ida_hexrays.DECOMP_NO_WAIT)
    if cf is None:
        if hf.code == ida_hexrays.MERR_BUSY:
            raise SystemExit("MERR_BUSY")
        raise Exception("decompile failed %%d %%s" %% (hf.code, hf.str))
    return cf, "\n".join(ida_lines.tag_remove(l.line) for l in cf.get_pseudocode())

def known(name):
    t = ida_typeinf.tinfo_t()
    return t.get_named_type(TIL, name)

def sanitize(decl):
    """Unknown type names become void (behind a pointer) or int, so a decl from another tree parses."""
    head, _, params = decl.partition("(")
    params = params.rsplit(")", 1)[0]
    def fix(part, keep_last):
        toks = re.findall(r"\w+|\*|\S", part)
        ids = [i for i, t in enumerate(toks) if re.match(r"[A-Za-z_]\w*$", t)]
        last = ids[-1] if keep_last and len(ids) > 1 else None
        for i in ids:
            t = toks[i]
            if i == last or t in KEYWORDS or known(t):
                continue
            ptr = any(x == "*" for x in toks[i + 1:])
            toks[i] = "void" if ptr else "int"
        return " ".join(toks)
    ps = [] if params.strip() in ("", "void") else [fix(p, True) for p in params.split(",")]
    return fix(head, True) + "(" + (", ".join(ps) if ps else "void") + ")"

def apply(ea, decl):
    for d in (decl, sanitize(decl)):
        tif = ida_typeinf.tinfo_t()
        if ida_typeinf.parse_decl(tif, TIL, d + ";", ida_typeinf.PT_SIL) is not None and tif.is_func():
            if ida_typeinf.apply_tinfo(ea, tif, ida_typeinf.TINFO_DEFINITE):
                return d, tif
    return None, None

def strip_casts(e):
    while e.op == ida_hexrays.cot_cast:
        e = e.x
    return e

def callsite_types(ea):
    """Per argument index, the types callers pass (casts removed), counted."""
    votes = {}
    callers = sorted({ida_funcs.get_func(x).start_ea for x in idautils.CodeRefsTo(ea, 0) if ida_funcs.get_func(x)})
    for c in callers[:12]:
        try:
            cf = ida_hexrays.decompile(c, None, ida_hexrays.DECOMP_NO_CACHE | ida_hexrays.DECOMP_NO_WAIT)
        except Exception:
            continue
        if cf is None:
            continue
        class V(ida_hexrays.ctree_visitor_t):
            def __init__(self):
                ida_hexrays.ctree_visitor_t.__init__(self, ida_hexrays.CV_PARENTS)
            def visit_expr(self, e):
                if e.op == ida_hexrays.cot_call and e.x.op == ida_hexrays.cot_obj and e.x.obj_ea == ea:
                    par = self.parent_expr()
                    if par is not None and par.op == ida_hexrays.cot_cast:
                        rt = par.type.dstr()
                    elif par is not None and par.op == ida_hexrays.cot_asg and strip_casts(par.y).op == ida_hexrays.cot_call:
                        rt = par.x.type.dstr()
                    else:
                        rt = None
                    if rt:
                        votes.setdefault("ret", {})
                        votes["ret"][rt] = votes["ret"].get(rt, 0) + 1
                    for i in range(e.a.size()):
                        t = strip_casts(e.a[i]).type
                        votes.setdefault(i, {})
                        s = t.dstr()
                        votes[i][s] = votes[i].get(s, 0) + 1
                return 0
        V().apply_to(cf.body, None)
    return votes

def informative(s):
    return "*" in s and not re.match(r"^(void|char|_BYTE|_WORD|_DWORD|int|unsigned \w+|__int\d+) \*$", s) or s in ("float", "f32")

def w32(s):
    """Hex-Rays guesses 64-bit for every GPR; ee-gcc ints are 32-bit."""
    return {"__int64": "int", "unsigned __int64": "unsigned int", "signed __int64": "int"}.get(s, s)

def call_decl(name, guess, votes, narrow=True, use_ret=True):
    fi = ida_typeinf.func_type_data_t()
    if not guess.get_func_details(fi):
        return None
    ps = []
    for i, a in enumerate(fi):
        t = w32(a.type.dstr()) if narrow else a.type.dstr()
        v = votes.get(i, {})
        if v:
            best = max(v.items(), key=lambda kv: kv[1])[0]
            if informative(best):
                t = best
        ps.append("%%s a%%d" %% (t, i + 1))
    rt = w32(fi.rettype.dstr()) if narrow else fi.rettype.dstr()
    v = votes.get("ret", {})
    if v and rt != "void" and use_ret:
        best = max(v.items(), key=lambda kv: kv[1])[0]
        if informative(best):
            rt = best
    return "%%s %%s(%%s)" %% (rt, name, ", ".join(ps) or "void")

def best_vote(v):
    return max(v.items(), key=lambda kv: kv[1])[0] if v else None

def hd_merge(name, guess, votes, hd):
    """HD types over the guess at the guess's arity; a weak HD type yields to an informative call-site type."""
    tif = ida_typeinf.tinfo_t()
    hfi = ida_typeinf.func_type_data_t()
    if not hd or ida_typeinf.parse_decl(tif, TIL, sanitize(hd) + ";", ida_typeinf.PT_SIL) is None or not tif.get_func_details(hfi):
        return None
    fi = ida_typeinf.func_type_data_t()
    if not guess.get_func_details(fi):
        return None
    ps = []
    for i, a in enumerate(fi):
        t = w32(a.type.dstr())
        if i < hfi.size():
            t = hfi[i].type.dstr()
        b = best_vote(votes.get(i, {}))
        if b and informative(b) and not informative(t):
            t = b
        nm = hfi[i].name if i < hfi.size() and hfi[i].name else "a%%d" %% (i + 1)
        ps.append("%%s %%s" %% (t, nm))
    return "%%s %%s(%%s)" %% (hfi.rettype.dstr(), name, ", ".join(ps) or "void")

def nargs_of(decl_tif):
    return decl_tif.get_nargs() if decl_tif is not None else -1

res = {}
for name, ea, p in JOBS:
    saved = ida_typeinf.tinfo_t()
    had = ida_nalt.get_tinfo(saved, ea)
    ida_nalt.del_tinfo(ea)
    out = {"texts": {}, "applied": {}}
    keep = None
    try:
        cf, base = text(ea)
        guess = cf.type
        out["guess"] = guess.dstr()
        need_call = any("call" in ch or "hdm" in ch for ch in VARIANTS.values())
        votes = callsite_types(ea) if need_call else {}
        out["call"] = {str(k): v for k, v in votes.items()}
        for label, chain in VARIANTS.items():
            got, src = None, "none"
            for s in chain.split(","):
                strict = s.endswith("!")
                s = s.rstrip("!")
                if s == "none":
                    break
                if s == "call":
                    d = call_decl(name, guess, votes)
                elif s == "call64":
                    d = call_decl(name, guess, votes, False)
                elif s == "callnr":
                    d = call_decl(name, guess, votes, True, False)
                elif s == "hdm":
                    d = hd_merge(name, guess, votes, p.get("hd"))
                elif s == "g32":
                    d = call_decl(name, guess, {})
                else:
                    d = p.get(s)
                if s == "own":
                    d = saved.dstr() if had else None
                if not d:
                    continue
                if s == "own":
                    applied, tif = (d, saved) if ida_typeinf.apply_tinfo(ea, saved, ida_typeinf.TINFO_DEFINITE) else (None, None)
                else:
                    applied, tif = apply(ea, d)
                if applied is None:
                    continue
                if strict and tif.get_nargs() != guess.get_nargs():
                    ida_nalt.del_tinfo(ea)
                    continue
                got, src = applied, s
                if KEEP and s != "own":
                    keep = tif
                break
            try:
                out["texts"][label] = base if got is None else text(ea)[1]
            except Exception as e:
                out["texts"][label] = "// DECOMPILE FAILED: %%s" %% e
            out["applied"][label] = [src, got]
            ida_nalt.del_tinfo(ea)
    except Exception as e:
        out["error"] = str(e)
    finally:
        ida_nalt.del_tinfo(ea)
        if keep is not None:
            ida_typeinf.apply_tinfo(ea, keep, ida_typeinf.TINFO_DEFINITE)
        elif had:
            ida_typeinf.apply_tinfo(ea, saved, ida_typeinf.TINFO_DEFINITE)
    res[name] = out
ida_hexrays.clear_cached_cfuncs()
open(%r, "w", encoding="utf-8").write(json.dumps(res))
print(len(res))
'''

OFF = os.environ.get("OFF")
KEEP = os.environ.get("KEEP") == "1"
PORT = int(os.environ.get("PORT", "0"))
if KEEP and len(variants) != 1:
    raise SystemExit("KEEP=1 takes one LABEL=CHAIN")
if PORT in ip.SHARED_PORTS and os.environ.get("SHARED_APPROVED") != "1":
    raise SystemExit("shared instance: set SHARED_APPROVED=1 only with the user's approval")


def run(code):
    return ip.ida.run_python(PORT, code) if PORT else ip.run(ver, code)
HEXOPT = os.environ.get("HEXOPTIONS")


def setup(on):
    code = "import sys, ida_hexrays\n"
    if OFF:
        code += f"sys.modules[{OFF!r}]._{'remove' if on else 'install'}()\n"
    if HEXOPT:
        code += f"print(ida_hexrays.change_hexrays_config('HEXOPTIONS = {HEXOPT if on else '0x100831FF'}'))\n"
    code += "ida_hexrays.clear_cached_cfuncs()\n"
    r = run(code)
    if r["stderr"].strip():
        raise SystemExit("setup failed: " + r["stderr"][-800:])


def save_applied(res):
    json.dump({n: {"guess": o.get("guess"), "call": o.get("call"), "applied": o.get("applied")} for n, o in res.items()},
              open(ip.WORK + "/pc/" + os.environ.get("APPLIED", "_dumpp_applied.json"), "w"), indent=1)


res = {}
setup(True)
try:
    for i in range(0, len(jobs), 8):
        code = CODE % (jobs[i:i + 8], variants, KEEP, outfile)
        for attempt in range(20):
            r = run("def __m():\n" + "".join("    " + l + "\n" for l in code.split("\n")) + "__m()\n")
            out = (r["stdout"] + r["stderr"]).strip()
            if "MERR_BUSY" not in out:
                break
            time.sleep(3)  # another client is decompiling; the chunk restores types before it exits
        if not out.endswith(str(len(jobs[i:i + 8]))):
            raise SystemExit("IDA side failed at chunk %d: %s" % (i, out[-1500:]))
        res.update(json.load(open(ip.WORK + f"/pc/_dumpp-{ver}.json")))
        save_applied(res)
        print(f"chunk {i + 8}/{len(jobs)}", file=sys.stderr, flush=True)
finally:
    setup(False)
if KEEP:
    print("kept", sum(1 for o in res.values() if o.get("applied", {}).get(next(iter(variants)), ["none"])[0] not in ("none", "own")))
for label in variants:
    d = os.path.join(ip.WORK, "pc", label)
    os.makedirs(d, exist_ok=True)
    for name, o in res.items():
        t = o.get("texts", {}).get(label, "// ERROR " + o.get("error", ""))
        open(f"{d}/{name}.c", "w").write(vu0fold.fold(t) + "\n")
save_applied(res)
