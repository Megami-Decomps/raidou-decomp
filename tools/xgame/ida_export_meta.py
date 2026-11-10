# IDAPython: dump function prototypes and comments, user-named data with types, and $gp as JSON.
# Run through ida.py py <instance>; OUT_NAME is set by the caller.
import json, idautils, idc, ida_bytes, ida_funcs, ida_nalt, ida_typeinf

OUT_DIR = r"\\wsl.localhost\Debian\home\wsl\.local\state\smt3-port-work\raidou-map\apply"
LINE = ida_typeinf.PRTYPE_1LINE | ida_typeinf.PRTYPE_SEMI


def decl(ea, name, ida_nalt=ida_nalt, ida_typeinf=ida_typeinf, LINE=LINE):  # py_eval runs with separate locals
    tif = ida_typeinf.tinfo_t()
    if not ida_nalt.get_tinfo(tif, ea):
        return None
    return ida_typeinf.print_tinfo("", 0, 0, LINE, tif, name, "")


funcs = {}
for f in idautils.Functions():
    name = idc.get_func_name(f)
    cmts = {}
    for h in idautils.FuncItems(f):
        c = [idc.get_cmt(h, 0), idc.get_cmt(h, 1)]
        if any(c):
            cmts[h - f] = c
    funcs[f] = {"name": name, "user": ida_bytes.has_user_name(ida_bytes.get_flags(f)), "proto": decl(f, name),
                "fcmt": [idc.get_func_cmt(f, 0), idc.get_func_cmt(f, 1)], "cmts": cmts}
data = {}
for ea, name in idautils.Names():
    fl = ida_bytes.get_flags(ea)
    if ida_bytes.is_code(fl) or not ida_bytes.has_user_name(fl):
        continue
    data[ea] = {"name": name, "decl": decl(ea, name), "size": ida_bytes.get_item_size(ea) if ida_bytes.is_head(fl) else 0}
json.dump({"gp": idc.get_name_ea_simple("_gp"), "funcs": funcs, "data": data},
          open(OUT_DIR + "\\" + OUT_NAME, "w"))
print(len(funcs), len(data))
