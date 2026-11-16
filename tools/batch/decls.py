"""Top-level C declarations: split a file or a list of lines into statements and say what each declares."""
import re
from dataclasses import dataclass

INCLUDE = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]')
MACRO_CALL = re.compile(r"^(INCLUDE_ASM|INCLUDE_RODATA|INCLUDE_SDATA|SIZE_CHECK|STATIC_ASSERT)\b")
TAG = re.compile(r"\b(struct|union|enum)\s+(\w+)")
TYPE_WORDS = {"struct", "union", "enum", "const", "volatile", "signed", "unsigned", "static", "extern", "inline",
              "register", "char", "short", "int", "long", "float", "double", "void"}


@dataclass
class Decl:
    kind: str  # include, type, tag, proto, extern, data, define, other
    name: str
    text: str
    line: int = 0

    def norm(self) -> str:
        """The declaration without parameter names, `extern` and spacing, for comparing two spellings."""
        if self.kind == "include":
            return f"#include {self.name}"
        text = re.sub(r"\bextern\s+", "", self.text)
        if self.kind == "proto":
            head, _, rest = text.partition(f"{self.name}")
            args = rest[rest.index("(") + 1:rest.rindex(")")] if "(" in rest and ")" in rest else rest
            text = f"{head}{self.name}({','.join(strip_param_name(p) for p in split_top(args, ','))})"
        return squash(text)


def squash(text: str) -> str:
    text = re.sub(r"\s+", " ", text).strip().rstrip(";").strip()
    return re.sub(r"\s*([*(),\[\]{};])\s*", r"\1", text)


def strip_param_name(param: str) -> str:
    p = param.strip()
    m = re.match(r"^(.*?[\w*\s])\b([A-Za-z_]\w*)\s*((\[[^\]]*\]\s*)*)$", p)
    if not m or not m.group(1).strip() or m.group(2) in TYPE_WORDS:
        return squash(p)
    head = m.group(1).strip()
    if head in ("struct", "union", "enum") or (head.split()[-1] in ("struct", "union", "enum")):
        return squash(p)
    return squash(head + ("*" if m.group(3) else ""))


def split_top(text: str, sep: str) -> list[str]:
    out, depth, cur = [], 0, []
    for c in text:
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == sep and depth == 0:
            out.append("".join(cur))
            cur = []
        else:
            cur.append(c)
    if "".join(cur).strip():
        out.append("".join(cur))
    return out


def strip_comments(text: str) -> str:
    """Comments become spaces (newlines kept, so line numbers hold); string literals are left alone."""
    out, i = [], 0
    while i < len(text):
        if text.startswith("//", i):
            j = text.find("\n", i)
            j = len(text) if j < 0 else j
            out.append(" " * (j - i))
            i = j
        elif text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = len(text) if j < 0 else j + 2
            out.append(re.sub(r"[^\n]", " ", text[i:j]))
            i = j
        elif text[i] in "\"'":
            q, j = text[i], i + 1
            while j < len(text) and text[j] != q:
                j += 2 if text[j] == "\\" else 1
            out.append(text[i:j + 1])
            i = j + 1
        else:
            out.append(text[i])
            i += 1
    return "".join(out)


def statements(text: str) -> list[tuple[int, str]]:
    """(line, text) of each top-level statement; a preprocessor line is one statement, a function body ends one."""
    text = strip_comments(text)
    out, cur, start, depth, line, blank = [], [], 0, 0, 1, True
    i = 0
    while i < len(text):
        c = text[i]
        if c == "#" and depth == 0 and blank:
            j = i
            while True:
                k = text.find("\n", j)
                k = len(text) if k < 0 else k
                if text[k - 1:k] == "\\":
                    j = k + 1
                    continue
                break
            out.append((line, text[i:k].strip()))
            line += text[i:k].count("\n")
            i = k
            continue
        if c == "\n":
            line += 1
        if blank and not c.isspace():
            start, blank = line, False
        if c in "\"'":
            q, j = c, i + 1
            while j < len(text) and text[j] != q:
                j += 2 if text[j] == "\\" else 1
            cur.append(text[i:j + 1])
            i = j + 1
            continue
        cur.append(c)
        if c in "({[":
            depth += 1
        elif c in ")}]":
            depth -= 1
            if c == "}" and depth == 0:
                stmt = "".join(cur)
                head = stmt[:stmt.index("{")]
                # a function definition ends at its brace; a struct or initializer goes on to its semicolon
                if head.rstrip().endswith(")") and "=" not in head:
                    out.append((start, stmt.strip()))
                    cur, blank = [], True
        elif c == ";" and depth == 0:
            out.append((start, "".join(cur).strip()))
            cur, blank = [], True
        i += 1
    if "".join(cur).strip():
        out.append((start, "".join(cur).strip()))
    return out


def classify(stmt: str, line: int = 0) -> Decl:
    s = stmt.strip()
    if s.startswith("#"):
        m = INCLUDE.match(s)
        if m:
            return Decl("include", m.group(1), s, line)
        m = re.match(r"#\s*define\s+(\w+)", s)
        return Decl("define" if m else "other", m.group(1) if m else "", s, line)
    if MACRO_CALL.match(s):
        return Decl("other", "", s, line)
    flat = s
    while "{" in flat and (n := re.sub(r"\{[^{}]*\}", "\x00", flat)) != flat:
        flat = n
    flat = flat.replace("\x00", "{}")
    if re.match(r"^typedef\b", flat):
        m = re.search(r"\(\s*\*\s*(\w+)\s*\)", flat) or re.search(r"(\w+)\s*(\[[^\]]*\]\s*)*;?\s*$", flat)
        return Decl("type", m.group(1) if m else "", s, line)
    m = re.match(r"^(struct|union|enum)\s+(\w+)\s*(\{\})?\s*;?$", flat)
    if m:
        return Decl("tag", m.group(2), s, line)
    if s.endswith("}") and "{" in s and ")" in s[:s.index("{")]:
        name = re.findall(r"(\w+)\s*\(", s[:s.index("{")])
        return Decl("def", name[0] if name else "", s, line)
    if re.match(r"^(struct|union|enum)\s*\{\}\s*;?$", flat):
        return Decl("other", "", s, line)
    m = re.match(r"^[^(]*\(\s*\*\s*(\w+)\s*\)\s*[(\[]", flat)
    extern = flat.startswith("extern ") or flat.startswith("extern\n")
    if m:
        return Decl("extern" if extern else "data", m.group(1), s, line)
    if "(" in flat and "=" not in flat.split("(")[0]:
        m = re.search(r"(\w+)\s*\(", flat)
        if m and m.group(1) not in ("__attribute__",):
            return Decl("proto", m.group(1), s, line)
    m = re.search(r"(\w+)\s*(\[[^\]]*\]\s*)*(=.*)?;?\s*$", flat, re.S)
    name = m.group(1) if m else ""
    return Decl("extern" if extern else "data", name, s, line)


def parse(text: str) -> list[Decl]:
    return [classify(s, n) for n, s in statements(text)]


def parse_lines(lines: list[str]) -> list[Decl]:
    """Declarations given one per entry (a draft store entry's decls)."""
    out = []
    for i, line in enumerate(lines):
        out += [classify(s, i + 1) for _, s in statements(line)]
    return out


def type_names(d: Decl) -> set[str]:
    """Typedef names and `struct X` tags a declaration's text uses, own name excluded."""
    words = set(re.findall(r"\b[A-Za-z_]\w*\b", strip_comments(d.text)))
    tags = {f"{k} {n}" for k, n in TAG.findall(d.text)}
    return (words | tags) - {d.name} - defines(d)


def defines(d: Decl) -> set[str]:
    """Type names a declaration makes visible: a typedef name, struct tags it defines or forward-declares."""
    out = set()
    if d.kind == "type":
        out.add(d.name)
    for k, n in re.findall(r"\b(struct|union|enum)\s+(\w+)\s*\{", d.text):
        out.add(f"{k} {n}")
    if d.kind == "tag":
        out |= {f"{k} {n}" for k, n in TAG.findall(d.text)[:1]}
    return out
