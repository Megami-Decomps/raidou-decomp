"""Per-game views of C with #ifdef VERSION_<GAME> blocks, and the merge back.

view() keeps what one game compiles, minus the VERSION_ directives; merge() builds one
text whose view for each game is that game's text, with differing lines under guards.
"""
import difflib
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = list(json.loads((ROOT / "config" / "versions.json").read_text()))

DIRECTIVE = re.compile(r"^\s*#\s*(ifdef|ifndef|if|elif|else|endif)\b(.*)$")
DEFINED = re.compile(r"\bdefined\s*(?:\(\s*(\w+)\s*\)|(\w+))")
GAME = re.compile(r"\b(?:%s)\b" % "|".join(f"VERSION_{v.upper()}" for v in VERSIONS))


def _macro(version):
    return f"VERSION_{version.upper()}"


def condition(kind, arg):
    """A function of the version for a VERSION_* condition, None for any other condition."""
    arg = re.sub(r"/\*.*?\*/|//.*$", "", arg).strip()
    if not GAME.search(arg):
        return None
    if kind in ("ifdef", "ifndef"):
        if not GAME.fullmatch(arg):
            raise ValueError(f"#{kind} {arg}: expected one game macro")
        want = kind == "ifdef"
        return lambda v: (_macro(v) == arg) == want
    py = []
    for tok in re.findall(r"defined\s*\(\s*\w+\s*\)|defined\s+\w+|&&|\|\||\S", arg):
        name = DEFINED.fullmatch(tok)
        if name and GAME.fullmatch(name.group(1) or name.group(2)):
            py.append(f"({(name.group(1) or name.group(2))!r} == M)")
        elif tok in ("&&", "||", "!", "(", ")"):
            py.append({"&&": " and ", "||": " or ", "!": " not "}.get(tok, tok))
        else:
            raise ValueError(f"#{kind} {arg}: only defined(<game macro>) with !, && and || is supported")
    expr = "".join(py)
    return lambda v: bool(eval(expr, {}, {"M": _macro(v)}))


def lines_for(text, version):
    """(line, kept) for each line of text: kept when the version compiles it and it is not
    itself a VERSION_* directive. Raises ValueError on unbalanced or mixed conditionals."""
    out = []
    stack = []  # (is_version, active, taken)
    active = True
    for number, line in enumerate(text.splitlines(keepends=True), 1):
        m = DIRECTIVE.match(line)
        if not m:
            out.append((line, active))
            continue
        kind, arg = m.groups()
        try:
            if kind in ("if", "ifdef", "ifndef"):
                cond = condition(kind, arg)
                if cond is None:
                    stack.append((False, active, False))
                    out.append((line, active))
                else:
                    on = cond(version)
                    stack.append((True, active, on))
                    active = active and on
                    out.append((line, False))
            elif kind in ("elif", "else"):
                if not stack:
                    raise ValueError(f"#{kind} without #if")
                is_version, outer, taken = stack[-1]
                cond = condition("if", arg) if kind == "elif" else None
                if not is_version:
                    if cond is not None:
                        raise ValueError("#elif on VERSION_ inside a non-version #if")
                    out.append((line, active))
                    continue
                if kind == "elif" and cond is None:
                    raise ValueError("#elif on a non-version condition inside a VERSION_ #if")
                on = not taken and (cond(version) if cond else True)
                stack[-1] = (True, outer, taken or on)
                active = outer and on
                out.append((line, False))
            else:
                if not stack:
                    raise ValueError("#endif without #if")
                is_version, outer, _ = stack.pop()
                if is_version:
                    active = outer
                out.append((line, active if not is_version else False))
        except ValueError as e:
            raise ValueError(f"line {number}: {e}") from None
    if stack:
        raise ValueError("unterminated #if")
    return out


def view(text, version, blank=False):
    """The text a version compiles. blank=True keeps dropped lines as empty lines, so line
    numbers stay those of the file."""
    if not GAME.search(text):
        return text
    return "".join(l if kept else ("\n" if blank and l.endswith("\n") else "")
                   for l, kept in lines_for(text, version))


def _trim(lines):
    """Blank lines at the edges of a differing run, which stay outside its guard."""
    i, j = 0, len(lines)
    while i < j and not lines[i].strip():
        i += 1
    while j > i and not lines[j - 1].strip():
        j -= 1
    return lines[:i], lines[i:j], lines[j:]


def _depth(lines):
    """Conditionals lines leave open, or None when they close, continue or #else an outer
    one: a guard around them would split a #if from its #else or #endif."""
    depth = 0
    for line in lines:
        m = DIRECTIVE.match(line)
        if not m:
            continue
        if m.group(1) in ("if", "ifdef", "ifndef"):
            depth += 1
        elif depth == 0:
            return None
        elif m.group(1) == "endif":
            depth -= 1
    return depth


def _hunks(a, b):
    """difflib opcodes, with differing runs widened over equal lines until each side of
    every run is balanced."""
    ops = [list(op) for op in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes()]
    i = 0
    while i < len(ops):
        op, i1, i2, j1, j2 = ops[i]
        depths = (_depth(a[i1:i2]), _depth(b[j1:j2]))
        if op == "equal" or depths == (0, 0):
            i += 1
            continue
        # Widen backwards when a side closes or continues an outer conditional.
        back = None in depths
        lo, hi = (max(i - 1, 0), i + 1) if back else (i, min(i + 2, len(ops)))
        if (lo, hi) == (i, i + 1):
            raise ValueError("cannot balance a differing run")
        part = ops[lo:hi]
        ops[lo:hi] = [["replace", part[0][1], part[-1][2], part[0][3], part[-1][4]]]
        i = lo
    return ops


def merge(views):
    """One text whose view for each version is views[version]. Two versions."""
    if len(views) != 2 or set(views) != set(VERSIONS):
        raise ValueError(f"merge needs one text for each of {VERSIONS}")
    a_name, b_name = VERSIONS
    a, b = views[a_name].splitlines(keepends=True), views[b_name].splitlines(keepends=True)
    for ls in (a, b):
        if ls and not ls[-1].endswith("\n"):
            ls[-1] += "\n"
    out = []
    for op, i1, i2, j1, j2 in _hunks(a, b):
        if op == "equal":
            out += a[i1:i2]
            continue
        # Blank lines at the edges stay unguarded only when both sides have them.
        a_lead, a_body, a_tail = _trim(a[i1:i2])
        b_lead, b_body, b_tail = _trim(b[j1:j2])
        lead = min(len(a_lead), len(b_lead)) if a_body and b_body else 0
        tail = min(len(a_tail), len(b_tail)) if a_body and b_body else 0
        a_mid, b_mid = a[i1 + lead:i2 - tail], b[j1 + lead:j2 - tail]
        out += a[i1:i1 + lead]
        if a_mid and b_mid:
            out += [f"#ifdef {_macro(a_name)}\n", *a_mid, "#else\n", *b_mid, "#endif\n"]
        elif a_mid:
            out += [f"#ifdef {_macro(a_name)}\n", *a_mid, "#endif\n"]
        elif b_mid:
            out += [f"#ifdef {_macro(b_name)}\n", *b_mid, "#endif\n"]
        out += a[i2 - tail:i2]
    text = "".join(out)
    for v, want in views.items():
        if view(text, v) != (want if want.endswith("\n") or not want else want + "\n"):
            raise AssertionError(f"merge does not give {v} its own text back")
    return text


def check(text):
    """Every VERSION_ conditional in text is one view() can evaluate (ValueError if not)."""
    for v in VERSIONS:
        lines_for(text, v)
