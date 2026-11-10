# IDAPython: dump every function's words, calls and string refs as JSON.
# Run through ida.py py <instance>; OUT_NAME is set by the caller.
import json, idautils, idc, ida_funcs, ida_bytes, ida_nalt, ida_segment

OUT_DIR = r"\\wsl.localhost\Debian\home\wsl\.local\state\smt3-port-work\raidou-map"

def strings_of(ea):
    out = []
    for x in idautils.DataRefsFrom(ea):
        t = ida_nalt.get_str_type(x)
        if t is not None and t != ida_nalt.STRTYPE_C - 1 and ida_bytes.is_strlit(ida_bytes.get_flags(x)):
            s = ida_bytes.get_strlit_contents(x, -1, ida_nalt.STRTYPE_C)
            if s:
                out.append(s.decode("latin-1"))
    return out

funcs = []
for f in idautils.Functions():
    fn = ida_funcs.get_func(f)
    words, calls, strs, drefs = [], [], [], []
    for chunk_start, chunk_end in idautils.Chunks(f):
        for ea in range(chunk_start, chunk_end, 4):
            words.append(ida_bytes.get_dword(ea))
            for x in idautils.CodeRefsFrom(ea, 0):
                if ida_funcs.get_func(x) and ida_funcs.get_func(x).start_ea == x and x != f:
                    calls.append(x)
            for x in idautils.DataRefsFrom(ea):
                seg = ida_segment.getseg(x)
                if seg and not (seg.perm & ida_segment.SEGPERM_EXEC):
                    drefs.append(x)
            strs.extend(strings_of(ea))
    seg = ida_segment.getseg(f)
    funcs.append({"ea": f, "name": idc.get_func_name(f), "seg": ida_segment.get_segm_name(seg) if seg else "",
                  "words": words, "calls": calls, "strs": strs, "drefs": drefs})

with open(OUT_DIR + "\\" + OUT_NAME, "w") as fp:
    json.dump(funcs, fp)
print(len(funcs))
