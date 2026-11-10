#include "shared/interface/itfMesManager/decls.h"

/* Clear each child's low color byte, then OR the unmasked input word.
 * High input bits can therefore also change the upper bytes. */
void ChangeFontAlpha(frFontHandle_t *pfont, u32 alpha) {
    frTextHandle_t *child;

    for (; pfont != NULL; pfont = pfont->previous) {
        for (child = pfont->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = child->parentDimensionsOrRenderWord.renderWord & ITF_MES_COLOR_BYTE_CLEAR_MASK | alpha;
        }
    }
}
