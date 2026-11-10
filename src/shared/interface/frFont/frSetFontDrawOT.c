#include "shared/interface/frFont/decls.h"

/* Replace the complete shared render-flag word used by subsequent drawing. */
void frSetFontDrawOT(u32 OtType) {
    gFontDrawOT = OtType;
}
