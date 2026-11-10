#include "shared/interface/frFont/decls.h"

/* Replace each child's full color word across the parent glyph chain. */
void frSetFontColor(frFontHandle_t *pFRQ, u32 Color) {
    for (; pFRQ != NULL; pFRQ = pFRQ->previous) {
        frTextHandle_t *childNode;
        for (childNode = pFRQ->firstChild; childNode != NULL; childNode = childNode->next) {
            childNode->parentDimensionsOrRenderWord.renderWord = Color;
        }
    }
}
