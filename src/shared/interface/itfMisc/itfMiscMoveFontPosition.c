#include "shared/interface/itfMisc/decls.h"

/* Translate parent entries only; child coordinates are left unchanged. */
void itfMiscMoveFontPosition(frFontRequest_t *pfont, u32 ofsx, u32 ofsy) {
    for (; pfont != NULL; pfont = pfont->prev) {
        pfont->x += ofsx;
        pfont->y += ofsy;
    }
}
