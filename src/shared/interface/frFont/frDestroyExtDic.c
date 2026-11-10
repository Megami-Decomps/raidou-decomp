#include "shared/interface/frFont/decls.h"

/* Free the reserved temporary font entry. */
void frDestroyExtDic(void) {
    frDelFontStyle(FR_FONT_TEMPORARY_SLOT);
}
