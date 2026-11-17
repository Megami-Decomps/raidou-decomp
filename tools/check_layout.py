#!/usr/bin/env python3
"""Refuse tracked files outside LAYOUT, and scripts in tools/ that no other
tracked file names (import, command, docs entry) and no test covers.

    python3 tools/check_layout.py      # one-off scripts belong in local/
"""
from __future__ import annotations

import fnmatch
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

# Where things go; docs/CONTRIBUTING.md#where-things-go explains each entry.
LAYOUT = (
    ".gitignore", "README.md", "configure.py", "requirements.txt",
    "diff_settings.py",            # asm-differ reads it from the root
    "permuter_settings.toml",      # decomp-permuter reads it from the root
    ".github/workflows/*.yml",
    ".claude/agents/*.md",
    "orig/.gitkeep",
    "docs/*.md",
    "tools/*.py", "tools/*.sh", "tools/*.inc",
    "tools/batch/*.py",
    "tools/ida/*.py", "tools/ida/*.sh", "tools/ida/*.json", "tools/ida/sync/*.py",
    "tools/xgame/*.py",
    "tools/tests/test_*.py",
)
# Whole trees, any depth.
TREES = ("config/", "docs/matching/", "include/", "src/")
SCRIPT = re.compile(r"^tools/(?!tests/).*\.(py|sh)$")


def tracked() -> list[str]:
    return subprocess.run(["git", "ls-files"], cwd=ROOT, capture_output=True, text=True,
                          check=True).stdout.splitlines()


def matches(path: str, pattern: str) -> bool:
    """fnmatch per path segment, so * never crosses a /."""
    parts, want = path.split("/"), pattern.split("/")
    return len(parts) == len(want) and all(fnmatch.fnmatchcase(p, w) for p, w in zip(parts, want))


def stray(files: list[str]) -> list[str]:
    """Paths outside LAYOUT, and tests outside tools/tests."""
    def misplaced_test(f: str) -> bool:
        return f.startswith("tools/") and fnmatch.fnmatchcase(Path(f).name, "test_*.py") \
            and not matches(f, "tools/tests/test_*.py")
    return [f for f in files if misplaced_test(f)
            or not f.startswith(TREES) and not any(matches(f, p) for p in LAYOUT)]


def unused(root: Path, files: list[str]) -> list[str]:
    """Scripts in tools/ that no other tracked file names and no test covers."""
    texts = {}
    for f in files:
        if f.startswith(("src/", "include/", "docs/matching/")) or not (root / f).is_file():
            continue
        texts[f] = set(re.findall(r"\w+", (root / f).read_text(errors="replace")))
    tests = {Path(f).stem for f in files if f.startswith("tools/tests/")}
    out = []
    for f in files:
        if not SCRIPT.match(f):
            continue
        stem = Path(f).stem
        if f"test_{stem}" in tests:
            continue
        if not any(stem in words for other, words in texts.items() if other != f):
            out.append(f)
    return out


def main() -> int:
    files = tracked()
    problems = [f"{f}: outside the layout (tools/check_layout.py LAYOUT)" for f in stray(files)]
    problems += [f"{f}: nothing names it and no test covers it; move it to local/" for f in unused(ROOT, files)]
    for line in problems:
        print(line, file=sys.stderr)
    print(f"check_layout: {len(files)} tracked files, {len(problems)} problems")
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
