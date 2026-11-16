# IDAPython, GAME set by caller. Where a decomp name is held by an address the decomp does not
# name, clear the stale holder and set the name at the decomp address. Prints each move.
import csv, idc, ida_name, ida_funcs
rows = list(csv.DictReader(open(D + "names-%s.tsv" % GAME, encoding="utf-8"), delimiter="\t"))
decomp = {int(r["addr"], 16): r["name"] for r in rows}
for ea, name in sorted(decomp.items()):
    if idc.get_name(ea) == name:
        continue
    other = idc.get_name_ea_simple(name)
    if other != idc.BADADDR and other != ea:
        if other in decomp:
            ida_name.set_name(other, decomp[other], ida_name.SN_NOWARN | ida_name.SN_NOCHECK)
        else:
            f = ida_funcs.get_func(other)
            print("clear 0x%X %s (func start %s)" % (other, name, bool(f and f.start_ea == other)))
            ida_name.set_name(other, "", ida_name.SN_NOWARN)
    ok = ida_name.set_name(ea, name, ida_name.SN_NOWARN | ida_name.SN_NOCHECK)
    print("set 0x%X %s %s" % (ea, name, ok))
