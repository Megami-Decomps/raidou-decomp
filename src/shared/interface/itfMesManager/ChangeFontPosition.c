#include "shared/interface/itfMesManager/decls.h"

/* Translate every linked node in text units; a NULL chain is a no-op. */
void ChangeFontPosition(frFontHandle_t *pfont, s32 dist_x, s32 dist_y) {
    if (pfont == NULL) {
        return;
    }
    do {
        pfont->x += dist_x;
        pfont->y += dist_y;
        pfont = pfont->previous;
    } while (pfont != NULL);
}
