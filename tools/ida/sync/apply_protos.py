# IDAPython, GAME and DRY_RUN set by caller. Applies protos-<GAME>.tsv (raidou-decomp prototypes).
# Fills a function with no type or an IDA-guessed one (__fastcall, __int64); a C definition's
# prototype also replaces a type with a different argument count. Otherwise the existing type
# (SMT3/DDS1 semantics, where the decomp carries addresses as u32) stays. Logs to protos-<GAME>-apply.log.
import csv, re, idc, ida_nalt, ida_typeinf, ida_funcs
log = open(D + "protos-%s-apply.log" % GAME, "w", encoding="utf-8")
n = {"set": 0, "replaced": 0, "same": 0, "kept": 0, "failed": 0, "noname": 0, "unspecified": 0}
for r in csv.DictReader(open(D + "protos-%s.tsv" % GAME, encoding="utf-8"), delimiter="\t"):
    ea = int(r["addr"], 16)
    if idc.get_name(ea) != r["name"]:
        n["noname"] += 1
        log.write("noname\t%s\t%s\t%s\n" % (r["addr"], r["name"], idc.get_name(ea)))
        continue
    proto = r["proto"]
    if re.search(r"\b%s\s*\(\s*\)" % r["name"], proto):
        n["unspecified"] += 1
        continue
    tif = ida_typeinf.tinfo_t()
    had = ida_nalt.get_tinfo(tif, ea)
    old = idc.get_type(ea) or ""
    new = ida_typeinf.tinfo_t()
    if ida_typeinf.parse_decl(new, None, proto, ida_typeinf.PT_SIL) is None:
        n["failed"] += 1
        log.write("parse\t%s\t%s\n" % (r["addr"], proto))
        continue
    if had and tif._print() == new._print():
        n["same"] += 1
        continue
    guessed = not had or "__fastcall" in old or "__int64" in old
    if not guessed and (r["source"] != "def" or tif.get_nargs() == new.get_nargs()):
        n["kept"] += 1
        continue
    if DRY_RUN or ida_typeinf.apply_tinfo(ea, new, ida_typeinf.TINFO_DEFINITE):
        n["replaced" if had else "set"] += 1
        if had:
            log.write("replace\t%s\t%s\t%s\t-> %s\n" % (r["addr"], r["source"], old, new._print()))
    else:
        n["failed"] += 1
        log.write("apply\t%s\t%s\n" % (r["addr"], proto))
log.close()
print(GAME, "dry" if DRY_RUN else "applied", n)
