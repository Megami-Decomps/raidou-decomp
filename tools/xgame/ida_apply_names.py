# IDAPython: name sub_ functions from a <game>_map.tsv; MAP_NAME is set by the caller.
import csv, idc, ida_name
p = r"\\wsl.localhost\Debian\home\wsl\.local\state\smt3-port-work\raidou-map" + "\\" + MAP_NAME
done = skipped = 0
for r in csv.DictReader(open(p), delimiter="\t"):
    if not r["smt3_name"]:
        continue
    ea = int(r["ea"], 16)
    if not idc.get_func_name(ea).startswith("sub_"):
        skipped += 1
        continue
    if ida_name.set_name(ea, r["smt3_name"], ida_name.SN_NOWARN | ida_name.SN_NOCHECK):
        done += 1
    else:
        skipped += 1
print("named", done, "skipped", skipped)
