#include "shared/interface/itfMesManager/decls.h"

void ChangeFontColorAll(frFontHandle_t *pfont, u8 color)
{
    frTextHandle_t *c;

    for (; pfont != NULL; pfont = pfont->previous) {
        for (c = pfont->firstChild; c != NULL; c = c->next) {
            c->renderValueOrSetupOrShade.setupBytes.firstOption = color;
        }
    }
}
