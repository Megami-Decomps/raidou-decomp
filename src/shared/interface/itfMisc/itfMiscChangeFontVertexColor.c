#include "shared/interface/itfMisc/decls.h"

/* Replace each child color; parent color words are not modified. */
void itfMiscChangeFontVertexColor(frFontRequest_t *pfont, u32 color) {
    for (; pfont != NULL; pfont = pfont->prev) {
        frFontChild_t *child;
        for (child = pfont->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = color;
        }
    }
}
