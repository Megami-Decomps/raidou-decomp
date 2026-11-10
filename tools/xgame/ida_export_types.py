# IDAPython: write every local type as C declarations, dependency-ordered; OUT_NAME is set by the caller.
import idc, ida_typeinf
OUT_DIR = r"\\wsl.localhost\Debian\home\wsl\.local\state\smt3-port-work\raidou-map\apply"
ords = [i for i in range(1, ida_typeinf.get_ordinal_limit(None)) if ida_typeinf.get_numbered_type_name(None, i)]
text = idc.print_decls(",".join(map(str, ords)), idc.PDF_DEF_FWD | idc.PDF_DEF_BASE)
open(OUT_DIR + "\\" + OUT_NAME, "w", encoding="utf-8").write(text)
print(len(ords), len(text))
