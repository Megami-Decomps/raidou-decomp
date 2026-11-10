#include "shared/interface/itfMesManager/decls.h"

void ChangeFontColor(frFontHandle_t *pfont, s32 index, s32 lines, s32 color)
{
    s32 skip = lines - index - 1;
    s32 y = pfont->y;
    s32 cur = y;
    frTextHandle_t *c;
    u8 flag;

    while (skip > 0) {
        while (y == cur) {
            pfont = pfont->previous;
            if (pfont == NULL) {
                return;
            }
            cur = pfont->y;
        }
        skip--;
        y = cur;
    }
    flag = color;
    do {
        for (c = pfont->firstChild; c != NULL; c = c->next) {
            c->renderValueOrSetupOrShade.setupBytes.firstOption = flag;
        }
        pfont = pfont->previous;
    } while (pfont != NULL && y == pfont->y);
}
