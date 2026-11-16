# IDAPython, GAME and DRY_RUN set by caller. Parses types/<GAME>.h (raidou-decomp definitions,
# from extract_types.py) into a scratch til, checks each aggregate's size against clang's
# (types/<GAME>-sizes.tsv), and copies size-checked types into the IDB, replacing older ones.
import re, ida_typeinf
D = D + "types" + "\\"
text = open(D + GAME + ".h", encoding="utf-8").read()
sizes = dict(l.rstrip("\n").split("\t") for l in open(D + GAME + "-sizes.tsv", encoding="utf-8"))
# gcc applies aligned() after a typedef name to the typedef alone and keeps the struct's size;
# IDA would pad the struct to it.
text = re.sub(r"(\}\s*\w+)\s+__attribute__\s*\(\(\s*aligned\s*\(\s*\d+\s*\)\s*\)\)\s*;", r"\1;", text)
# IDA's parser rejects C++ keywords as member names.
text = re.sub(r"(\w[\w\s\*]*?\s\**)(class|this|new|delete|template|operator|private|public|protected|namespace)(\s*(\[[^\]]*\])?\s*;)", r"\1\2_\3", text)
blocks = text.split("\n\n")
# C keeps tags and typedef names apart, IDA does not: a tag whose name a typedef gives to another
# type becomes <name>_tag.
tags = set(m.group(2) for m in re.finditer(r"\b(struct|union|enum)\s+(\w+)\s*\{", text))
clash = set()
for b in blocks:
    b = b.strip()
    if not b.startswith("typedef"):
        continue
    m = re.search(r"(\w+)\s*(\[[^\]]*\]\s*)*;$", re.sub(r"\{.*\}", "{}", b, flags=re.S))
    if m and m.group(1) in tags and not re.match(r"typedef\s+(const\s+)?(struct|union|enum)\s+%s\b" % m.group(1), b):
        clash.add(m.group(1))
if clash:
    text = re.sub(r"\b(struct|union|enum)\s+(%s)\b" % "|".join(sorted(clash)), r"\1 \2_tag", text)
    blocks = text.split("\n\n")
print("tag clashes", len(clash), " ".join(sorted(clash))[:400])
tmp = ida_typeinf.new_til("raidou_tmp", "")
HTI = ida_typeinf.HTI_DCL | ida_typeinf.HTI_SEMICOLON | ida_typeinf.HTI_NOBASE
fwd = []
for b in blocks:
    for m in re.finditer(r"\b(struct|union)\s+(\w+)", b):
        fwd.append("%s %s;" % m.groups())
ida_typeinf.parse_decls(tmp, "\n".join(sorted(set(fwd))), None, ida_typeinf.HTI_DCL | ida_typeinf.HTI_NOBASE)
pending = blocks
while pending:
    retry = [b for b in pending if ida_typeinf.parse_decls(tmp, b, None, HTI)]
    if len(retry) == len(pending):
        break
    pending = retry
log = open(D + GAME + "-import.log", "w", encoding="utf-8")
for b in pending:
    log.write("unparsed\t" + re.sub(r"\s+", " ", b)[:160] + "\n")
added = replaced = same = badsize = unsized = fwd_only = failed = 0
name = ida_typeinf.first_named_type(tmp, ida_typeinf.NTF_TYPE)
try:
    ONLY
except NameError:
    ONLY = None
while name:
    if ONLY and name not in ONLY:
        name = ida_typeinf.next_named_type(tmp, name, ida_typeinf.NTF_TYPE)
        continue
    tif = ida_typeinf.tinfo_t()
    tif.get_named_type(tmp, name)
    if tif.is_forward_decl():
        fwd_only += 1
        old = ida_typeinf.tinfo_t()
        if not old.get_named_type(None, name) and not DRY_RUN:
            tif.set_named_type(None, name, ida_typeinf.NTF_TYPE | ida_typeinf.NTF_COPY)
        name = ida_typeinf.next_named_type(tmp, name, ida_typeinf.NTF_TYPE)
        continue
    sk = name[:-4] if name.endswith("_tag") and name[:-4] in clash else name
    want = sizes.get(("union " if tif.is_union() else "struct ") + sk) if sk != name or name not in sizes else sizes.get(name)
    if want is None:
        want = sizes.get(("union " if tif.is_union() else "struct ") + name)
    if tif.is_udt() and want is None:
        unsized += 1
    if tif.is_udt() and want is not None and tif.get_size() != int(want):
        badsize += 1
        log.write("size\t%s\tida %d clang %s\n" % (name, tif.get_size(), want))
        udt = ida_typeinf.udt_type_data_t()
        tif.get_udt_details(udt)
        for mb in udt:
            log.write("  member\t%s\t0x%X\t0x%X\t%s\n" % (mb.name, mb.offset // 8, mb.size // 8, mb.type._print()))
        name = ida_typeinf.next_named_type(tmp, name, ida_typeinf.NTF_TYPE)
        continue
    old = ida_typeinf.tinfo_t()
    have = old.get_named_type(None, name)
    if have and not old.is_forward_decl() and old._print(None, ida_typeinf.PRTYPE_DEF | ida_typeinf.PRTYPE_MULTI) == tif._print(None, ida_typeinf.PRTYPE_DEF | ida_typeinf.PRTYPE_MULTI):
        same += 1
    else:
        if have and not old.is_forward_decl():
            log.write("replace\t%s\tsize %d -> %d\n" % (name, old.get_size(), tif.get_size()))
        if DRY_RUN or tif.set_named_type(None, name, ida_typeinf.NTF_TYPE | ida_typeinf.NTF_COPY | ida_typeinf.NTF_REPLACE) == ida_typeinf.TERR_OK:
            if have and not old.is_forward_decl():
                replaced += 1
            else:
                added += 1
        else:
            failed += 1
            log.write("failed\t%s\n" % name)
    name = ida_typeinf.next_named_type(tmp, name, ida_typeinf.NTF_TYPE)
ida_typeinf.free_til(tmp)
log.close()
print(GAME, "dry" if DRY_RUN else "applied", "added", added, "replaced", replaced, "same", same, "size mismatch", badsize,
      "unsized udt", unsized, "forward", fwd_only, "failed", failed, "unparsed", len(pending))
