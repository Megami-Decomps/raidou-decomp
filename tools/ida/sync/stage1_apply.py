# IDAPython, GAME and D (the sync data folder, Windows path) set by the caller, shared.py sync.
# Sets raidou-decomp names, then SMT3/DDS1 prototypes where the
# function has no type yet. Idempotent; prints counts. Take a snapshot before the first run.
import csv, idc, ida_name, ida_funcs, ida_nalt, ida_typeinf, ida_bytes
named = same = mkfunc = nfail = 0
fails = []
for r in csv.DictReader(open(D + "names-%s.tsv" % GAME, encoding="utf-8"), delimiter="\t"):
    ea = int(r["addr"], 16)
    if not ida_funcs.get_func(ea):
        if ida_funcs.add_func(ea):
            mkfunc += 1
    if idc.get_name(ea) == r["name"]:
        same += 1
        continue
    other = idc.get_name_ea_simple(r["name"])
    if other != idc.BADADDR and other != ea:
        nfail += 1
        fails.append("%s(taken at 0x%X)" % (r["addr"], other))
        continue
    if ida_name.set_name(ea, r["name"], ida_name.SN_NOWARN | ida_name.SN_NOCHECK):
        named += 1
    else:
        nfail += 1
        fails.append(r["addr"])
typed = kept = pfail = 0
pfails = []
for r in csv.DictReader(open(D + "stage1-protos-%s.tsv" % GAME, encoding="utf-8"), delimiter="\t"):
    ea = int(r["addr"], 16)
    tif = ida_typeinf.tinfo_t()
    if ida_nalt.get_tinfo(tif, ea):
        kept += 1
        continue
    if idc.SetType(ea, r["proto"]):
        typed += 1
    else:
        pfail += 1
        pfails.append(r["addr"])
print("names set", named, "same", same, "funcs made", mkfunc, "failed", nfail, fails[:20])
print("protos set", typed, "kept", kept, "failed", pfail, pfails[:20])
