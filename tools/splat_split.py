#!/usr/bin/env python3
"""`splat split` that reads src/shared/<unit>.c (a TU both games share) when
src/<version>/<unit>.c is missing, and sees only the game's side of VERSION_ guards.
It also writes asm/<version>/function_rodata.tsv, splat's rodata migration for every
C unit (units.function_rodata reads it), since C functions get no asm file, and every
unit's data sections to asm/<version>/data/.
Usage: tools/splat_split.py config/<version>/<serial>.yaml"""
import sys
from pathlib import Path

import spimdisasm
from splat.__main__ import splat_main
from splat.segtypes.common.c import CommonSegC
from splat.segtypes.common.data import CommonSegData
from splat.util import options

sys.path.insert(0, str(Path(__file__).resolve().parent))
from version_guard import view  # noqa: E402

_out_path = CommonSegC.out_path


def out_path(self):
    path = _out_path(self)
    src = Path(options.opts.src_path)
    shared = src.parent / "shared" / path.relative_to(src)
    return shared if not path.exists() and shared.exists() else path


CommonSegC.out_path = out_path

_split = CommonSegC.split
_entries = spimdisasm.mips.FunctionRodataEntry.getAllEntriesFromSections
migration = []  # (unit, function or "", rodata symbols)


def split(self, rom_bytes):
    def entries(text, rodata):
        out = _entries(text, rodata)
        unit = (Path(self.dir) / self.name).as_posix()
        for e in out:
            func = e.function.getNameUnquoted() if e.function is not None else ""
            migration.append((unit, func, [s.getNameUnquoted() for s in e.iterRodataSyms()]))
        return out

    spimdisasm.mips.FunctionRodataEntry.getAllEntriesFromSections = entries
    try:
        _split(self, rom_bytes)
    finally:
        spimdisasm.mips.FunctionRodataEntry.getAllEntriesFromSections = _entries


CommonSegC.split = split
# A C unit's .rodata/.sdata still gets asm/<v>/data/<unit>.<section>.s, which
# splat writes for dot sections only under disassemble_all.
CommonSegData.should_self_split = lambda self: options.opts.is_mode_active("data")


class VersionView:
    """A source path whose read_text() is one game's view of the file."""

    def __init__(self, path, version):
        self.path, self.version = path, version

    def read_text(self, encoding="utf-8"):
        return view(self.path.read_text(encoding=encoding), self.version)


def read_view(scan, version):
    return staticmethod(lambda c_file: scan(VersionView(Path(c_file), version)))

if __name__ == "__main__":
    version = Path(sys.argv[1]).resolve().parent.name
    for name in ("get_funcs_defined_in_c", "get_global_asm_funcs", "get_global_asm_rodata_syms"):
        setattr(CommonSegC, name, read_view(getattr(CommonSegC, name), version))
    sys.argv = ["splat", "split", *sys.argv[1:]]
    splat_main()
    lines = [f"{unit}\t{func}\t{' '.join(syms)}\n" for unit, func, syms in migration]
    (Path(options.opts.asm_path) / "function_rodata.tsv").write_text("".join(lines))
