#include "shared/interface/itfPanel/decls.h"

/* Set the base color, one alpha-scaled gradient row, and two rows at full color scale. */
void CreateColorPlain(itfPanelPlainData_t *pwork, u32 red, u32 green, u32 blue, u32 alpha) {
    u32 *colorRow = pwork->colors[0].components;

    SetVec4(colorRow, red, green, blue, alpha);
    itfMiscSetRatioRGB((s32 *)pwork->colors[1].components, red, green, blue, alpha, itfPanelPlainColorRatio, 1);
    itfMiscSetRatioRGB((s32 *)pwork->colors[2].components, red, green, blue, ITF_PANEL_FULL_COLOR_SCALE, itfPanelPlainFullColorRatio, 2);
}
