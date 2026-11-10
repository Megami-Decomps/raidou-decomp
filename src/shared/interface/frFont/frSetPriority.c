#include "shared/interface/frFont/decls.h"

/* Store the glyph render value in sixteenths. */
void frSetPriority(frFontHandle_t *pFRQ, u32 Pri) {
    pFRQ->renderValueOrSetupOrShade.renderValue = Pri >> FR_FONT_CONTEXT_VALUE_SHIFT;
}
