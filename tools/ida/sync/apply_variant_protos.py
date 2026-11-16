# IDAPython, GAME set by caller. Applies protos-<GAME>-variant.tsv (borrowed-lineage type variants).
import csv, idc, ida_typeinf
def main():
    n = f = 0
    for r in csv.DictReader(open(D + "protos-%s-variant.tsv" % GAME, encoding="utf-8"), delimiter="\t"):
        ea = int(r["addr"], 16)
        t = ida_typeinf.tinfo_t()
        if idc.get_name(ea) == r["name"] and ida_typeinf.parse_decl(t, None, r["proto"], ida_typeinf.PT_SIL) is not None and ida_typeinf.apply_tinfo(ea, t, ida_typeinf.TINFO_DEFINITE):
            n += 1
        else:
            f += 1
    print(GAME, "variant protos set", n, "failed", f)
main()
