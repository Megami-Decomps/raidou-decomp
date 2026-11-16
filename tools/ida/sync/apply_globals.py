# IDAPython, GAME and DRY_RUN set by caller. Types data at globals-<GAME>.tsv addresses with the
# decomp's declaration when it has a size (no []), skipping a type whose extent would cover another
# listed global. Names an address whose name is generated (IDA dummy or auto name, or a D_<hex>
# placeholder); a user-given IDB name stays. Logs to globals-<GAME>-apply.log.
import csv, re, idc, ida_typeinf, ida_nalt, ida_bytes, ida_name, ida_segment
PLACEHOLDER = re.compile(r"^D_(r[12]_)?[0-9A-Fa-f]{6,8}$")
def generated(ea):
    nm = idc.get_name(ea)
    return nm == "" or not ida_name.is_uname(nm) or not ida_bytes.has_user_name(ida_bytes.get_flags(ea)) or bool(PLACEHOLDER.match(nm))
rows = list(csv.DictReader(open(D + "globals-%s.tsv" % GAME, encoding="utf-8"), delimiter="\t"))
starts = sorted(int(r["addr"], 16) for r in rows)
import bisect
log = open(D + "globals-%s-apply.log" % GAME, "w", encoding="utf-8")
n = dict(typed=0, same=0, named=0, unsized=0, overlap=0, parse=0, noseg=0, failed=0, retyped=0)
try:
    names_only = NAMES_ONLY
except NameError:
    names_only = False
for r in rows:
    ea = int(r["addr"], 16)
    if not ida_segment.getseg(ea):
        n["noseg"] += 1
        continue
    if generated(ea) and idc.get_name(ea) != r["name"]:
        if DRY_RUN or ida_name.set_name(ea, r["name"], ida_name.SN_NOWARN | ida_name.SN_NOCHECK):
            n["named"] += 1
    if names_only:
        continue
    if "[]" in r["decl"]:
        n["unsized"] += 1
        continue
    tif = ida_typeinf.tinfo_t()
    if ida_typeinf.parse_decl(tif, None, r["decl"], ida_typeinf.PT_SIL) is None:
        n["parse"] += 1
        log.write("parse\t%s\t%s\n" % (r["addr"], r["decl"]))
        continue
    size = tif.get_size()
    i = bisect.bisect_right(starts, ea)
    if size == ida_typeinf.BADSIZE or (i < len(starts) and starts[i] < ea + size):
        n["overlap"] += 1
        log.write("overlap\t%s\t%s\tsize 0x%X next 0x%X\n" % (r["addr"], r["decl"], size, starts[i] if i < len(starts) else 0))
        continue
    old = ida_typeinf.tinfo_t()
    had = ida_nalt.get_tinfo(old, ea)
    if had and old._print() == tif._print():
        n["same"] += 1
        continue
    if DRY_RUN or ida_typeinf.apply_tinfo(ea, tif, ida_typeinf.TINFO_DEFINITE):
        n["retyped" if had else "typed"] += 1
        if had:
            log.write("retype\t%s\t%s\t-> %s\n" % (r["addr"], old._print(), tif._print()))
    else:
        n["failed"] += 1
        log.write("apply\t%s\t%s\n" % (r["addr"], r["decl"]))
log.close()
print(GAME, "dry" if DRY_RUN else "applied", n)
