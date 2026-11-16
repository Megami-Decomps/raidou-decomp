"""Shared helpers: find the ida-pseudo IDA instances and run IDAPython. Tools live here, data in WORK."""
import json, os, subprocess, sys

sys.path.insert(0, os.path.expanduser("~/.claude/skills/ida-instances/scripts"))
import ida  # noqa: E402

TOOLS = os.path.dirname(os.path.abspath(__file__))
WORK = os.path.expanduser("~/.local/state/raidou-decomp-work/ida-pseudo")
IDB = {"r1": "SLUS_214.31", "r2": "SLUS_218.45"}
SHARED_PORTS = {13337, 13338}


def winpath(p):
    return subprocess.check_output(["wslpath", "-w", p], text=True).strip()


def find_port(ver):
    """Port of the IDA serving <game dir>/ida-pseudo/<IDB>.i64; never a shared instance."""
    for inst in ida.discover_instances(ida.parse_ports("13337-13437")):
        path = inst["idb_path"]
        if inst["port"] not in SHARED_PORTS and "ida-pseudo" in path and IDB[ver] in path:
            return inst["port"]
    raise SystemExit(f"no ida-pseudo instance for {ver}; run launch.sh {ver}")


def run(ver, code):
    port = find_port(ver)
    assert port not in SHARED_PORTS
    return ida.run_python(port, code)


def load_plugin(ver, path, calls=()):
    """Import a plugin file into the IDA instance as a module and call _install() plus CALLS."""
    win = winpath(path)
    mod = os.path.splitext(os.path.basename(path))[0]
    code = f"""
import importlib.util, sys
old = sys.modules.get({mod!r})
if old and hasattr(old, "_remove"):
    old._remove()
spec = importlib.util.spec_from_file_location({mod!r}, {win!r})
m = importlib.util.module_from_spec(spec)
sys.modules[{mod!r}] = m
spec.loader.exec_module(m)
m._install()
for c in {list(calls)!r}:
    eval("m." + c)
"""
    r = run(ver, code)
    return (r["stdout"] + r["stderr"]).strip()


def unload_plugin(ver, path):
    mod = os.path.splitext(os.path.basename(path))[0]
    r = run(ver, f"import sys\nm = sys.modules.get({mod!r})\nif m: m._remove(); print('removed')")
    return (r["stdout"] + r["stderr"]).strip()
