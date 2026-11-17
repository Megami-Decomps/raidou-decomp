#!/usr/bin/env python3
"""Extract the retail ELF and IRX modules from a disc image you own into orig/<version>/.

    python tools/extract.py [--iso path/to.iso raidou1]
"""
# Without --iso the image is found by config/versions.json's `iso_glob` in the
# repository root and orig/. An extracted file with the recorded SHA-1 is kept.
from __future__ import annotations

import argparse
import hashlib
import io
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def load_versions() -> dict:
    path = ROOT / "config" / "versions.json"
    return json.loads(path.read_text()) if path.exists() else {}


VERSIONS = load_versions()


def find_iso(glob: str) -> Path | None:
    for base in (ROOT, ROOT / "orig"):
        hits = sorted(base.glob(glob))
        if hits:
            return hits[0]
    return None


def sha1(path: Path) -> str:
    digest = hashlib.sha1()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


def already_extracted(version: str) -> bool:
    """True when orig/<v>/<serial> is present and has the recorded SHA-1."""
    elf = ROOT / "orig" / version / VERSIONS[version]["serial"]
    if not elf.is_file():
        return False
    digest = sha1(elf)
    if digest != VERSIONS[version]["elf_sha1"]:
        raise SystemExit(
            f"{version}: {elf.relative_to(ROOT)} SHA-1 {digest} != expected "
            f"{VERSIONS[version]['elf_sha1']}; remove it to re-extract"
        )
    print(f"{version}: {elf.relative_to(ROOT)} already present ({digest})")
    return True


SECTOR = 2048


def read_archive_file(cd, iso: Path, path: str) -> bytes:
    """A file inside DDS3.IMG, found through the DDS3.DDT tree of 12-byte entries
    (name offset, location, size), the root first; a negative size is a directory
    of that many entries at that DDT offset."""
    buf = io.BytesIO()
    cd.get_file_from_iso_fp(buf, iso_path="/DDS3.DDT;1")
    ddt = buf.getvalue()
    _name, location, size = struct.unpack_from("<IIi", ddt, 0)
    for part in (p for p in path.split("/") if p):
        if size >= 0:
            raise SystemExit(f"DDS3.DDT: {path}: {part!r} follows a file")
        for k in range(-size):
            name_off, loc, sz = struct.unpack_from("<IIi", ddt, location + k * 12)
            if ddt[name_off:ddt.index(b"\0", name_off)].decode("ascii") == part:
                location, size = loc, sz
                break
        else:
            raise SystemExit(f"DDS3.DDT: {path}: {part!r} not found")
    if size < 0:
        raise SystemExit(f"DDS3.DDT: {path} is a directory")
    lsn = cd.get_record(iso_path="/DDS3.IMG;1").extent_location() + location
    with iso.open("rb") as f:
        f.seek(lsn * SECTOR)
        return f.read(size)


def missing_irx(version: str) -> list[dict]:
    """The version's IRX modules (config/<v>/irx) not yet in orig/<v>/irx."""
    sys.path.insert(0, str(ROOT / "tools"))
    import irx_config

    out = []
    for key in irx_config.modules(version):
        cfg = irx_config.load(key)
        target = irx_config.path(cfg, "target")
        if not target.is_file():
            out.append(cfg)
        elif sha1(target) != cfg["sha1"]:
            raise SystemExit(f"{key}: {cfg['paths']['target']} SHA-1 != expected {cfg['sha1']}; remove it to re-extract")
    return out


def extract(version: str, iso: Path, elf_too: bool = True) -> None:
    import pycdlib

    info = VERSIONS[version]
    serial = info["serial"]
    out_dir = ROOT / "orig" / version
    out_dir.mkdir(parents=True, exist_ok=True)

    cd = pycdlib.PyCdlib()
    cd.open(str(iso))
    try:
        for name in (serial, "SYSTEM.CNF") if elf_too else ():
            buf = io.BytesIO()
            cd.get_file_from_iso_fp(buf, iso_path=f"/{name};1")
            (out_dir / name).write_bytes(buf.getvalue())
        for cfg in missing_irx(version):
            if "archive" in cfg:
                where = f"DDS3.IMG:{cfg['archive']}"
                data = read_archive_file(cd, iso, cfg["archive"])
            else:
                where = cfg["disc"]
                buf = io.BytesIO()
                cd.get_file_from_iso_fp(buf, iso_path=f"{cfg['disc']};1")
                data = buf.getvalue()
            target = ROOT / cfg["paths"]["target"]
            target.parent.mkdir(parents=True, exist_ok=True)
            if hashlib.sha1(data).hexdigest() != cfg["sha1"]:
                raise SystemExit(f"{cfg['key']}: {where} SHA-1 != expected {cfg['sha1']}")
            target.write_bytes(data)
            print(f"{cfg['key']}: {cfg['paths']['target']} OK")
    finally:
        cd.close()
    if not elf_too:
        return

    elf = out_dir / serial
    digest = sha1(elf)
    if digest != info["elf_sha1"]:
        elf.unlink()
        raise SystemExit(f"{version}: {serial} SHA-1 {digest} != expected {info['elf_sha1']}; wrong or modified disc")
    print(f"{version}: {elf.relative_to(ROOT)} OK ({digest})")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("versions", nargs="*", help=f"any of: {', '.join(VERSIONS)} (default: all)")
    ap.add_argument("--iso", type=Path, help="disc image (requires exactly one version)")
    ap.add_argument("--force", action="store_true", help="extract even if orig/ already matches")
    args = ap.parse_args()
    if not VERSIONS:
        ap.error("config/versions.json is missing or empty")
    unknown = set(args.versions) - VERSIONS.keys()
    if unknown:
        ap.error(f"unknown version(s): {', '.join(sorted(unknown))}")

    if args.iso:
        if len(args.versions) != 1:
            ap.error("--iso needs exactly one version")
        version = args.versions[0]
        have = not args.force and already_extracted(version)
        if not have or missing_irx(version):
            extract(version, args.iso, elf_too=not have)
        return

    missing = []
    for version in args.versions or VERSIONS:
        have = not args.force and already_extracted(version)
        if have and not missing_irx(version):
            continue
        iso = find_iso(VERSIONS[version]["iso_glob"])
        if iso is None:
            print(f"{version}: no ISO matching {VERSIONS[version]['iso_glob']!r}; skipped", file=sys.stderr)
            missing.append(version)
            continue
        extract(version, iso, elf_too=not have)
    if missing:
        raise SystemExit(f"no disc image for {', '.join(missing)}; pass --iso")


if __name__ == "__main__":
    main()
