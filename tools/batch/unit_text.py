"""Text edits on C units: splice a final draft at its INCLUDE_ASM, write a gap unit, unwrap a NON_MATCHING draft."""
import re

DRAFT = re.compile(r"^(/\* NON_MATCHING:.*?\*/\n)?#ifdef NON_MATCHING\n(.*)\n#else\n(INCLUDE_ASM\([^;]*\);)\n#endif\n?$",
                   re.S)
IF = re.compile(r"^\s*#\s*if")
ENDIF = re.compile(r"^\s*#\s*endif")


def asm_line(name: str) -> re.Pattern:
    return re.compile(r'^INCLUDE_ASM\(([^,]+),\s*"([^"]+)",\s*%s\);$' % re.escape(name), re.M)


def asm_call(unit: str, name: str) -> str:
    return f'INCLUDE_ASM(const s32, "{unit}", {name});'


def draft_block(lines: list[str], at: int) -> tuple[int, int]:
    """Line span [start, end) of the NON_MATCHING block whose #else holds the INCLUDE_ASM at line `at`,
    with the NON_MATCHING comment above it; (at, at + 1) when the line is not in such a block."""
    if not (0 < at < len(lines) - 1 and lines[at - 1].strip() == "#else" and ENDIF.match(lines[at + 1])):
        return at, at + 1
    depth, i = 0, at - 2
    while i >= 0:
        if ENDIF.match(lines[i]):
            depth += 1
        elif IF.match(lines[i]):
            if depth == 0:
                break
            depth -= 1
        i -= 1
    if i < 0 or not re.match(r"^\s*#\s*ifdef\s+NON_MATCHING\b", lines[i]):
        return at, at + 1
    start = i
    if start > 0 and lines[start - 1].rstrip().endswith("*/"):
        j = start - 1
        while j >= 0 and "/*" not in lines[j]:
            j -= 1
        if j >= 0 and "NON_MATCHING" in lines[j]:
            start = j
    return start, at + 2


def splice(text: str, name: str, body: str, r1only: bool = False) -> str:
    """Replace the function's INCLUDE_ASM (and any NON_MATCHING draft around it) with `body`.
    An R1-only body keeps the INCLUDE_ASM for the other game."""
    lines = text.split("\n")
    hits = [i for i, l in enumerate(lines) if asm_line(name).match(l)]
    body = body.rstrip("\n")
    if len(hits) == 2 and hits[1] == hits[0] + 2 and hits[0] > 0 and hits[1] + 1 < len(lines) and \
            lines[hits[0] - 1].strip() == "#ifdef VERSION_RAIDOU1" and lines[hits[0] + 1].strip() == "#else" and \
            ENDIF.match(lines[hits[1] + 1]):
        # the games' INCLUDE_ASM lines under a version guard, from when their names differed
        if r1only:
            return "\n".join(lines[:hits[0]] + body.split("\n") + lines[hits[0] + 1:])
        return "\n".join(lines[:hits[0] - 1] + body.split("\n") + lines[hits[1] + 2:])
    if len(hits) != 1:
        raise ValueError(f"{name}: {len(hits)} INCLUDE_ASM lines")
    asm = lines[hits[0]]
    start, end = draft_block(lines, hits[0])
    body = body.rstrip("\n")
    if r1only:
        body = f"#ifdef VERSION_RAIDOU1\n{body}\n#else\n{asm}\n#endif"
    return "\n".join(lines[:start] + body.split("\n") + lines[end:])


def gap_unit(pre: str, unit: str, names: list[str], finals: dict[str, str]) -> str:
    """A new C unit: the prelude, then every function of the asm unit in address order, as C when drafted."""
    parts = [pre.rstrip("\n")]
    for name in names:
        parts.append(finals[name].rstrip("\n") if name in finals else asm_call(unit, name))
    return "\n\n".join(parts) + "\n"


def unwrap(final: str) -> str | None:
    """The C of a NON_MATCHING final draft, without the comment and guards; None when it is not one."""
    m = DRAFT.match(final)
    return m.group(2) + "\n" if m else None


def nonmatching_final(body: str, unit: str, name: str, words: int, differ: int, comment: str) -> str:
    """The NON_MATCHING form of a draft that does not match; a draft that is already one is rewrapped, keeping its
    comment when none is given."""
    if m := DRAFT.match(body):
        body = m.group(2)
        if not comment and m.group(1):
            old = m.group(1).strip()[len("/* NON_MATCHING:"):-len("*/")]
            comment = re.sub(r"^\s*\d+ of \d+ words match\.", "", old).strip()
    differ = min(differ, words)
    return (f"/* NON_MATCHING: {words - differ} of {words} words match. {comment} */\n"
            f"#ifdef NON_MATCHING\n{body.rstrip(chr(10))}\n#else\n{asm_call(unit, name)}\n#endif\n")


def substitute(text: str, names: dict[str, str]) -> str:
    """Rename whole words in one pass, so chains (a -> b, c -> a) do not compound."""
    if not names:
        return text
    pat = re.compile(r"\b(%s)\b" % "|".join(map(re.escape, sorted(names, key=len, reverse=True))))
    return pat.sub(lambda m: names[m.group(1)], text)
