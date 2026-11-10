#include "shared/interface/frFont/decls.h"

/* Bind the caller's font data into the reserved temporary entry. */
void frSetExtDic(u8 *pExtFontData) {
    frAddFontStyle(FR_FONT_TEMPORARY_SLOT, pExtFontData, 0);
}
