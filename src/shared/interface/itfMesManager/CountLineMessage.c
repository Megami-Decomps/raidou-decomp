#include "shared/interface/itfMesManager/decls.h"

s32 CountLineMessage(frFontHandle_t *pfont_top, frFontHandle_t *pfont)
{
    s16 h = pfont->parentDimensionsOrRenderWord.parentDimensions.cellHeight;

    return ((pfont->y - pfont_top->y) >> 3) / h + 1;
}
