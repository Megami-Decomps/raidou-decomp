#include "shared/interface/frFont/decls.h"

/* Submit glyph-relative coordinates and the cache node's owned UV rectangle. */
void frCreateUserFontPacket(s32 X, s32 Y, s32 Z, frTextHandle_t *pText,
                  s32 Ot) {
    frCreateDrawFontPacket_20550848(X + pText->x, Y + pText->y,
                 pText->cellDimensions.cellWidth,
                 pText->cellDimensions.cellHeight >> FR_FONT_GLYPH_HEIGHT_SHIFT,
                 pText->renderValueOrSetupOrShade.setupBytes.firstOption, pText->parentDimensionsOrRenderWord.renderWord, Z, 1,
                 &pText->cachedItem->font_mem->uv, &gFT.atlas, Ot);
}
