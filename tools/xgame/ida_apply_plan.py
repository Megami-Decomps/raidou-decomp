# IDAPython: apply ops START..START+COUNT of a plan from plan_idb_apply.py, rechecking the live IDB.
# The caller sets PLAN (file name) and START, COUNT. Results append to <PLAN>.result.
import json, re, idc, ida_bytes, ida_funcs, ida_name, ida_nalt, ida_segment, ida_typeinf
DIR = r"\\wsl.localhost\Debian\home\wsl\.local\state\smt3-port-work\raidou-map\apply"
DUMMY = re.compile(r"^start$|^(sub|off|dword|unk|byte|word|qword|xmmword|flt|dbl|asc|stru|jpt|def|loc|nullsub|algn"
                   r"|func|D|jtbl|j)_[0-9A-Fa-f]+(_\d+)?$")
SN = ida_name.SN_NOWARN | ida_name.SN_NOCHECK
ops = json.load(open(DIR + "\\" + PLAN))[START:START + COUNT]
out = []
for o in ops:
    ea, kind = o["ea"], o["op"]
    res = "ok"
    if kind in ("fclear", "fname"):
        now = idc.get_func_name(ea)
        if now != o["expect"] and not DUMMY.match(now):
            res = "skip:now=" + now
        elif not ida_name.set_name(ea, "" if kind == "fclear" else o["name"], SN):
            res = "fail"
    elif kind == "proto":
        tif = ida_typeinf.tinfo_t()
        if ida_nalt.get_tinfo(tif, ea):
            res = "skip:has-type"
        elif not idc.SetType(ea, o["decl"]):
            res = "fail"
    elif kind == "fcmt":
        if idc.get_func_cmt(ea, o["rep"]):
            res = "skip:has-cmt"
        else:
            idc.set_func_cmt(ea, o["text"], o["rep"])
    elif kind == "cmt":
        if idc.get_cmt(ea, o["rep"]):
            res = "skip:has-cmt"
        else:
            idc.set_cmt(ea, o["text"], o["rep"])
    elif kind == "stub":
        w = ida_bytes.get_dword(ea)
        name = o["names"].get(str(w & 0xFFFF))
        if ida_funcs.get_func(ea):
            res = "skip:is-func"
        elif w >> 16 != 0x2403 or ida_bytes.get_dword(ea + 4) != 0xC or not name:
            res = "skip:not-stub"
        elif not ida_funcs.add_func(ea, ea + 0x10):
            res = "fail:func"
        elif not ida_name.set_name(ea, name, SN):
            res = "fail:name=" + name
        else:
            res = "ok:" + name
    elif kind == "dname":
        seg = ida_segment.getseg(ea)
        fl = ida_bytes.get_flags(ea)
        now = ida_name.get_name(ea)
        head = ida_bytes.get_item_head(ea)
        if not seg or ida_bytes.is_code(ida_bytes.get_flags(head)):
            res = "skip:not-data"
        elif now and now != o["name"] and not DUMMY.match(now) and ida_bytes.has_user_name(fl):
            res = "skip:now=" + now
        elif head != ea and ida_name.get_name(head) and not DUMMY.match(ida_name.get_name(head)) \
                and ida_nalt.get_tinfo(ida_typeinf.tinfo_t(), head):
            res = "skip:inside=" + ida_name.get_name(head)
        else:
            if head != ea:
                ida_bytes.del_items(head, ida_bytes.DELIT_SIMPLE, 1)
            have = ida_typeinf.tinfo_t()
            if o["limit"] and head == ea and ida_nalt.get_tinfo(have, ea) and ea + have.get_size() > o["limit"]:
                ida_nalt.del_tinfo(ea)
                ida_bytes.del_items(ea, ida_bytes.DELIT_SIMPLE, 1)
                res = "untyped"
            if not ida_name.set_name(ea, o["name"], SN):
                res = "fail:name"
            elif o["decl"] and not ida_nalt.get_tinfo(ida_typeinf.tinfo_t(), ea):
                p = idc.parse_decl(o["decl"], idc.PT_SILENT)
                tif = ida_typeinf.tinfo_t()
                if not p or not tif.deserialize(None, p[1], p[2]):
                    res = "named:type-parse-fail"
                else:
                    size = tif.get_size()
                    end = ea + max(size, 1)
                    x = ida_bytes.next_head(ea, end)
                    while x != idc.BADADDR and x < end:
                        n = ida_name.get_name(x)
                        if ida_bytes.is_code(ida_bytes.get_flags(x)) or (n and not DUMMY.match(n)):
                            break
                        x = ida_bytes.next_head(x, end)
                    if size == idc.BADADDR or (x != idc.BADADDR and x < end) or (o["limit"] and end > o["limit"]):
                        res = "named:type-overlaps"
                    else:
                        ida_bytes.del_items(ea, ida_bytes.DELIT_SIMPLE, size)
                        res = "ok" if idc.apply_type(ea, p, ida_typeinf.TINFO_DEFINITE) else "named:type-fail"
    out.append("%s\t%#x\t%s\t%s" % (kind, ea, o.get("name", ""), res))
with open(DIR + "\\" + PLAN + ".result", "a") as f:
    f.write("\n".join(out) + "\n")
from collections import Counter
print(len(ops), Counter((l.split("\t")[0], l.split("\t")[3].split(":")[0] + (":" + l.split("\t")[3].split(":")[1].split("=")[0] if ":" in l.split("\t")[3] else "")) for l in out))
