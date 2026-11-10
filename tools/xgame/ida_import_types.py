# IDAPython: add local types from C declarations (ida_export_types.py output) whose names this IDB lacks.
# The caller sets IN_NAME and DRY_RUN. Each block parses alone, retried until no progress.
import re, ida_typeinf
IN_DIR = r"\\wsl.localhost\Debian\home\wsl\.local\state\smt3-port-work\raidou-map\apply"
text = open(IN_DIR + "\\" + IN_NAME, encoding="utf-8").read()
# IDA prints a packed anonymous struct with a stray trailing attribute, and dangling ordinals as "#N".
text = re.sub(r"\n(\s*)__attribute__\(\(packed\)\) \};", r"\n\1};", text)
text = re.sub(r"#\d+ \*", "void *", text)
blocks = re.split(r"\n/\* \d+ \*/\n", text)
tmp = ida_typeinf.new_til("xgame_tmp", "")
HTI = ida_typeinf.HTI_DCL | ida_typeinf.HTI_SEMICOLON
fwd = []
for b in blocks:
    m = re.match(r"(?:#pragma[^\n]*\n)?\s*(struct|union)\s+(?:__\w+(?:\([^)]*\))?\s+)*(\w+)\s*\n?\{", b)
    if m:
        fwd.append("%s %s;" % (m.group(1), m.group(2)))
ida_typeinf.parse_decls(tmp, "\n".join(fwd), None, ida_typeinf.HTI_DCL)
pending = blocks
while pending:
    retry = [b for b in pending if ida_typeinf.parse_decls(tmp, b, None, HTI)]
    if len(retry) == len(pending):
        break
    pending = retry
unparsed = [re.sub(r"\s+", " ", b)[:80] for b in pending]
added, kept, failed, fwd_only, completed = [], 0, [], 0, 0
name = ida_typeinf.first_named_type(tmp, ida_typeinf.NTF_TYPE)
while name:
    tif = ida_typeinf.tinfo_t()
    old = ida_typeinf.tinfo_t()
    have = old.get_named_type(None, name)
    if have and old.is_forward_decl() and tif.get_named_type(tmp, name) and not tif.is_forward_decl():
        if DRY_RUN or tif.set_named_type(None, name, ida_typeinf.NTF_TYPE | ida_typeinf.NTF_COPY | ida_typeinf.NTF_REPLACE) == ida_typeinf.TERR_OK:
            completed += 1
        else:
            failed.append(name)
    elif have:
        kept += 1
    elif not tif.get_named_type(tmp, name):
        failed.append(name)
    elif DRY_RUN or tif.set_named_type(None, name, ida_typeinf.NTF_TYPE | ida_typeinf.NTF_COPY) == ida_typeinf.TERR_OK:
        added.append(name)
        fwd_only += tif.is_forward_decl()
    else:
        failed.append(name)
    name = ida_typeinf.next_named_type(tmp, name, ida_typeinf.NTF_TYPE)
ida_typeinf.free_til(tmp)
print("added", len(added), "forward_only", fwd_only, "completed_forward", completed, "kept", kept, "failed", len(failed), "unparsed", len(unparsed))
print("failed:", " ".join(failed))
print("unparsed:", "\n".join(unparsed))
