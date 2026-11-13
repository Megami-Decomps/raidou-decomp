#!/usr/bin/env python3
"""Compile the NON_MATCHING drafts of every version.

    python3 tools/check_nonmatching.py [src/<version>/<unit>.c ...]

A draft kept behind `#ifdef NON_MATCHING` is never compiled by the normal
build, so a changed struct or signature can break it silently. This compiles
each unit that has one with NON_MATCHING and SKIP_ASM defined, once per version
that builds it, with implicit function declarations as errors.

A failure means the draft needs updating, not that the repo is broken. Exit
status is non-zero so CI still says so.
"""
import re
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from units import VERSIONS  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
GUARD = re.compile(r"^\s*#\s*ifdef\s+NON_MATCHING\b", re.M)


def versions_of(path: Path) -> list[str]:
    """The versions that compile this file: its own, or all for src/shared."""
    top = path.relative_to(ROOT / "src").parts[0]
    return list(VERSIONS) if top == "shared" else [top]


def compile_unit(path: Path, version: str, nonmatching: bool = True) -> str:
    """Return the compiler's error lines, empty when the unit compiles."""
    with tempfile.TemporaryDirectory() as tmp:
        out = subprocess.run(
            [str(ROOT / "tools/cc.sh"), "-DSKIP_ASM",
             *(["-DNON_MATCHING"] if nonmatching else []),
             "-Werror-implicit-function-declaration", str(path),
             "-o", str(Path(tmp) / "out.o")],
            capture_output=True, text=True, cwd=ROOT,
            env={**__import__("os").environ, "RAIDOU_VERSION": version})
    errors = [l for l in out.stderr.splitlines()
              if re.search(r":\d+: ", l) and "warning:" not in l]
    if out.returncode and not errors:
        errors = [f"compiler exited with status {out.returncode}",
                  *(out.stderr or out.stdout).strip().splitlines()[:6]]
    return "\n".join(errors)


def control() -> None:
    """The check must fail on a broken draft and pass on a good one."""
    src = ROOT / "src/raidou1"
    with tempfile.TemporaryDirectory(dir=src) as tmp:
        for text, name, must_fail in (
                ("#ifdef NON_MATCHING\nint f(void) { return g(); }\n#endif\n", "bad.c", True),
                ("#ifdef NON_MATCHING\nint f(void) { return 1; }\n#endif\n", "good.c", False)):
            unit = Path(tmp) / name
            unit.write_text(text)
            if bool(compile_unit(unit, "raidou1")) != must_fail:
                sys.exit(f"control {name}: wrong result, so this check cannot be trusted")


def main() -> int:
    if len(sys.argv) > 1:
        todo = [Path(a).resolve() for a in sys.argv[1:]]
    else:
        todo = [p for p in sorted((ROOT / "src").rglob("*.c"))
                if GUARD.search(p.read_text())]
    if not todo:
        print("no NON_MATCHING drafts")
        return 0
    control()
    bad = 0
    for path in todo:
        rel = path.relative_to(ROOT)
        for version in versions_of(path):
            err = compile_unit(path, version)
            if err:
                bad += 1
                print(f"{rel} ({version}): DOES NOT COMPILE with NON_MATCHING")
                print("\n".join("    " + l for l in err.splitlines()[:12]))
            else:
                print(f"{rel} ({version}): compiles with NON_MATCHING")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
