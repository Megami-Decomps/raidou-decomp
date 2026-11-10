#include "shared/interface/itfMisc/decls.h"

/* Clear each child's low byte, then OR unmasked bits into the full color word. */
void itfMiscChangeFontAlpha(frFontRequest_t *pfont, u32 alpha) {
    for (; pfont != NULL; pfont = pfont->prev) {
        frFontChild_t *child;
        for (child = pfont->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = (child->parentDimensionsOrRenderWord.renderWord & ~ITF_BYTE_MASK) | alpha;
        }
    }
}
