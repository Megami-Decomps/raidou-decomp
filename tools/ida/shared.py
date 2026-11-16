#!/usr/bin/env python3
# shared.py: apply the ida-pseudo changes to an IDA instance chosen by port. Data and changes.log are in ip.WORK.
# Every command first checks that PORT serves an IDB whose path contains IDB.
#   shared.py check  PORT IDB
#   shared.py sync   PORT IDB VER [--dry] [--defs]   sync/scripts in replay order (+ apply_protos_defs)
#   shared.py plugin PORT IDB                        load r5900_microcode_v2 with VU + generic helpers, clear cached cfuncs
#   shared.py save   PORT IDB
#   shared.py names  PORT IDB VER [--dry]           save, back up to IDB.pre-names.i64, apply_globals names only
#   shared.py protos PORT IDB VER TSV [--dry] [--protos JSON]
#                                                    save, back up to IDB.pre-protos.i64 (kept if present), apply hdm,sim!,call
#                                                    prototypes (function comment "proto proposed: SRC") to TSV rows (name, addr) with no
#                                                    decomp prototype; --protos takes prebuilt propose_protos.py output
#   shared.py ee32   PORT IDB                        save, back up to IDB.pre-ee32.i64, load ee32.py, save
# A non-dry command on 13337/13338 refuses unless SHARED_APPROVED=1 (the user's decision).
import csv, os, shutil, subprocess, sys, time
import ip

DATA = ip.winpath(ip.WORK + "/sync") + "\\"

STEPS = ["stage1_apply.py", "stage1_conflicts.py", "stage2_import.py", "stage2_import.py",
         "apply_protos.py", "apply_variant_protos.py", "apply_globals.py"]


def main():
    cmd, port, idb = sys.argv[1], int(sys.argv[2]), sys.argv[3]
    dry = "--dry" in sys.argv or cmd == "check"
    insts = {i["port"]: i for i in ip.ida.discover_instances([port])}
    if port not in insts or idb not in insts[port]["idb_path"]:
        raise SystemExit(f"port {port} does not serve {idb}: {insts.get(port, {}).get('idb_path')}")
    print(f"port {port}: {insts[port]['idb_path']}")
    if not dry and port in ip.SHARED_PORTS and os.environ.get("SHARED_APPROVED") != "1":
        raise SystemExit("shared instance: set SHARED_APPROVED=1 only with the user's approval")
    if cmd == "sync":
        ver = sys.argv[4]
        game = {"r1": "raidou1", "r2": "raidou2"}[ver]
        steps = STEPS + (["apply_protos_defs.py"] if "--defs" in sys.argv else [])
        for s in steps:
            src = open(f"{ip.TOOLS}/sync/{s}").read()
            if dry and "DRY_RUN" not in src:
                print("==", s, "skipped: no dry mode")
                continue
            code = f"GAME = {game!r}\nDRY_RUN = {dry}\nD = {DATA!r}\n" + src
            code = "def __main():\n" + "".join("    " + l + "\n" for l in code.split("\n")) + "__main()\n"
            r = ip.ida.run_python(port, code)
            out = (r["stdout"] + r["stderr"]).strip()
            print("==", s, out.splitlines()[-1] if out else "")
            if not dry:
                log(port, idb, f"sync/{s}", out.splitlines()[-1] if out else "")
    elif cmd == "plugin":
        path = ip.winpath(ip.TOOLS + "/r5900_microcode_v2.py")
        code = f"""
import importlib.util, sys
old = sys.modules.get("r5900_microcode_v2")
if old: old._remove()
spec = importlib.util.spec_from_file_location("r5900_microcode_v2", {path!r})
m = importlib.util.module_from_spec(spec)
sys.modules["r5900_microcode_v2"] = m
spec.loader.exec_module(m)
m._install()
m.enable_vu()
m.enable_generic()
import ida_hexrays
ida_hexrays.clear_cached_cfuncs()
print('cleared cached cfuncs')
"""
        if not dry:
            r = ip.ida.run_python(port, code)
            print((r["stdout"] + r["stderr"]).strip())
    elif cmd == "names":
        game = {"r1": "raidou1", "r2": "raidou2"}[sys.argv[4]]
        if not dry:
            save(port, idb)
            src = subprocess.check_output(["wslpath", "-u", insts[port]["idb_path"]], text=True).strip()
            dst = src[:-len(".i64")] + ".pre-names.i64"
            shutil.copyfile(src, dst)
            log(port, idb, "backup", dst, "pre-names")
        code = f"GAME = {game!r}\nDRY_RUN = {dry}\nNAMES_ONLY = True\nD = {DATA!r}\n" + open(f"{ip.TOOLS}/sync/apply_globals.py").read()
        code = "def __main():\n" + "".join("    " + l + "\n" for l in code.split("\n")) + "__main()\n"
        r = ip.ida.run_python(port, code)
        out = (r["stdout"] + r["stderr"]).strip()
        print(out.splitlines()[-1] if out else "")
        if not dry:
            log(port, idb, "apply_globals names only", out.splitlines()[-1] if out else "", "pre-names")
            save(port, idb)
    elif cmd == "protos":
        ver, tsv = sys.argv[4], sys.argv[5]
        game = {"r1": "raidou1", "r2": "raidou2"}[ver]
        dec = {r["addr"][2:].upper() for r in csv.DictReader(open(f"{ip.WORK}/sync/protos-{game}.tsv"), delimiter="\t")}
        rows = list(csv.DictReader(open(tsv), delimiter="\t"))
        todo = [r for r in rows if r["addr"].upper().zfill(8) not in dec]
        sel = f"{ip.WORK}/pc/_protos-{ver}.tsv"
        with open(sel, "w") as f:
            f.write("name\taddr\n" + "".join(f"{r['name']}\t{r['addr']}\n" for r in todo))
        if "--protos" in sys.argv:
            pj = sys.argv[sys.argv.index("--protos") + 1]
        else:
            pj = f"{ip.WORK}/pc/_protos-{ver}.json"
            subprocess.run([sys.executable, ip.TOOLS + "/propose_protos.py", sel, pj, "--version", game], check=True)
        import json
        props = json.load(open(pj))
        print(f"{len(todo)} of {len(rows)} rows have no decomp prototype; proposals: hd "
              f"{sum(bool(props.get(r['name'], {}).get('hd')) for r in todo)}, sim {sum(bool(props.get(r['name'], {}).get('sim')) for r in todo)}")
        if dry:
            return
        save(port, idb)
        src = subprocess.check_output(["wslpath", "-u", insts[port]["idb_path"]], text=True).strip()
        dst = src[:-len(".i64")] + ".pre-protos.i64"
        if os.path.exists(dst):
            print("backup kept", dst)
        else:
            shutil.copyfile(src, dst)
            log(port, idb, "backup", dst, "pre-protos")
        env = dict(os.environ, SAMPLE=os.path.relpath(sel, ip.WORK), KEEP="1", PORT=str(port), APPLIED=f"_dumpp_applied-{ver}.json")
        if os.path.exists(f"{ip.WORK}/pc/_dumpp_applied-{ver}.json"):
            os.remove(f"{ip.WORK}/pc/_dumpp_applied-{ver}.json")
        r = subprocess.run([sys.executable, ip.TOOLS + "/dumpp.py", ver, pj, "protos_kept=hdm,sim!,call"],
                           env=env, stdout=subprocess.PIPE, text=True)
        out = r.stdout.strip()
        print(out[-1500:], "exit", r.returncode)
        applied = json.load(open(f"{ip.WORK}/pc/_dumpp_applied-{ver}.json"))
        marks = {n: o["applied"]["protos_kept"][0] for n, o in applied.items()
                 if o.get("applied", {}).get("protos_kept", ["none"])[0] not in ("none", "own", "slow")}
        addrs = {r["name"]: int(r["addr"], 16) for r in todo}
        detail = {"hdm": "hd_from", "sim": "sim_from"}
        jobs = [(addrs[n], f"proto proposed: {src} {props.get(n, {}).get(detail.get(src), '') or ''}".rstrip() + " (shared.py protos)")
                for n, src in marks.items()]
        code = ("import idc\nn = 0\nfor ea, c in %r:\n    old = idc.get_func_cmt(ea, 0)\n"
                "    if 'proto proposed' not in (old or ''):\n        idc.set_func_cmt(ea, ((old + chr(10)) if old else '') + c, 0)\n"
                "        n += 1\nprint('marked', n)\n") % (jobs,)
        r = ip.ida.run_python(port, code)
        mout = (r["stdout"] + r["stderr"]).strip()
        print(mout)
        log(port, idb, "proto markers", mout, "pre-protos")
        log(port, idb, f"proposed prototypes for {os.path.basename(tsv)}", out.splitlines()[-1] if out else "", "pre-protos")
        save(port, idb)
    elif cmd == "ee32" and not dry:
        save(port, idb)
        src = subprocess.check_output(["wslpath", "-u", insts[port]["idb_path"]], text=True).strip()
        dst = src[:-len(".i64")] + ".pre-ee32.i64"
        shutil.copyfile(src, dst)
        print("backup", dst)
        log(port, idb, "backup", dst, "pre-ee32")
        win = ip.winpath(ip.TOOLS + "/ee32.py")
        code = f"""
import importlib.util, sys, ida_hexrays
old = sys.modules.get("ee32")
if old: old._remove()
spec = importlib.util.spec_from_file_location("ee32", {win!r})
m = importlib.util.module_from_spec(spec)
sys.modules["ee32"] = m
spec.loader.exec_module(m)
m._install()
ida_hexrays.clear_cached_cfuncs()
"""
        r = ip.ida.run_python(port, code)
        out = (r["stdout"] + r["stderr"]).strip()
        print(out)
        log(port, idb, "load ee32.py (session only)", out.splitlines()[-1] if out else "", "pre-ee32")
        save(port, idb)
    elif cmd == "save" and not dry:
        save(port, idb)


def save(port, idb):
    r = ip.ida.run_python(port, "import ida_loader\nprint(ida_loader.save_database(None, 0))")
    print(r["stdout"].strip())
    log(port, idb, "idb_save", r["stdout"].strip())


def log(port, idb, what, result, revert="pre-sync"):
    with open(ip.WORK + "/changes.log", "a") as f:
        f.write(f"{time.strftime('%F %T')}\tport {port} {idb}\t{what}\t{result}\trevert: restore {idb}.{revert}.i64\n")


main()
