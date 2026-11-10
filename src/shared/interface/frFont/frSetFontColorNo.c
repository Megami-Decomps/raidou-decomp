#include "shared/interface/frFont/decls.h"

/* Assign the first option byte across all visited children; NULL is a no-op. */
void frSetFontColorNo(frFontHandle_t *pFRQ, u8 ColorNo) {
    frTextHandle_t *childGlyph;

    for (; pFRQ != NULL; pFRQ = pFRQ->previous) {
        for (childGlyph = pFRQ->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->renderValueOrSetupOrShade.setupBytes.firstOption = ColorNo;
        }
    }
}
