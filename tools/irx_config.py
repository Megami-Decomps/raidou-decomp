#!/usr/bin/env python3
"""Load the per-module config in config/<version>/irx/<module>/<module>.yaml.

A module is named by its key, `<version>/<module>` (`raidou1/SDFSDMAN`). Its
paths follow from the key; the C file is src/irx/<version>/<module>.c when the
game has its own, else the src/irx/shared/<module>.c both games compile. They
sit apart from the ELF's src/<version> and src/shared, whose tools read every .c
there as an ELF unit.
"""
from __future__ import annotations

from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[1]


def versions() -> list[str]:
    return sorted(p.parent.name for p in (ROOT / "config").glob("*/irx") if p.is_dir())


def modules(version: str | None = None) -> list[str]:
    """Every module key that has a config, in a stable order."""
    out = []
    for v in [version] if version else versions():
        base = ROOT / "config" / v / "irx"
        if not base.is_dir():
            continue
        for path in sorted(base.iterdir()):
            if path.is_dir() and (path / f"{path.name}.yaml").exists():
                out.append(f"{v}/{path.name}")
    return out


def resolve(name: str) -> list[str]:
    """Keys for a command-line name: a key, a bare module (every game), or a version."""
    if "/" in name:
        return [name]
    if name in versions():
        return modules(name)
    keys = [k for k in modules() if k.split("/")[1] == name]
    if not keys:
        raise SystemExit(f"no IRX module {name!r}")
    return keys


def source(version: str, module: str) -> Path:
    own = Path("src") / "irx" / version / f"{module}.c"
    if (ROOT / own).exists():
        return own
    return Path("src") / "irx" / "shared" / f"{module}.c"


def load(key: str) -> dict:
    version, module = key.split("/")
    path = ROOT / "config" / version / "irx" / module / f"{module}.yaml"
    cfg = yaml.safe_load(path.read_text())
    for field in ("name", "sha1", "module"):
        if field not in cfg:
            raise SystemExit(f"{path.relative_to(ROOT)}: missing {field!r}")
    cfg["key"] = key
    cfg["version"] = version
    cfg["module_dir"] = module
    cfg["paths"] = {
        "target": f"orig/{version}/irx/{module}.IRX",
        "asm": f"asm/{version}/irx/{module}",
        "asm_root": f"asm/{version}/irx/",
        "src": str(source(version, module)),
        "build": f"build/{version}/irx/{module}",
        "symbols": f"config/{version}/irx/{module}/symbol_addrs.txt",
    }
    return cfg


def path(cfg: dict, key: str) -> Path:
    """A path from the config, relative to the repository root."""
    return ROOT / cfg["paths"][key]
