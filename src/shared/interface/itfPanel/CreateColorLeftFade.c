#include "shared/interface/itfPanel/decls.h"

/* Populate RGBA rows at bytes 0x30/0x40; the first row is transparent. */
void CreateColorLeftFade(itfPanelLeftFadeData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = pwork->colors[0].components;

    SetVec4(colorRow, red, green, blue, 0);
    colorRow = pwork->colors[1].components;
    SetVec4(colorRow, red, green, blue, alpha);
}
