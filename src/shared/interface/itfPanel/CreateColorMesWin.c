#include "shared/interface/itfPanel/decls.h"

/* Write the two color rows, then scale six template rows; the first fourth component is one. */
void CreateColorMesWin(itfPanelMesWinData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = pwork->colors[0].components;

    SetVec4(colorRow, red, green, blue, 1);
    colorRow = pwork->colors[1].components;
    SetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB((s32 *)pwork->colors[2].components, red, green, blue, alpha, itfPanelMesWinColorRatio, 6);
}
