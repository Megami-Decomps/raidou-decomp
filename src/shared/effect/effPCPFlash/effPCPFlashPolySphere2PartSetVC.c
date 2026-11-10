#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySphere2PartSetVC(effPCPFlashPolySphere2_tag *work, s32 index, s32 color)
{
    u32 *vc;
    s32 rgb1;
    s32 rgb2;

    vc = (u32 *)dds3FlashPolyList2GetVC(work->hPoly, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    vc[0] = effMagaCylinderCalcColor(rgb2, color);
    vc[1] = effMagaCylinderCalcColor(rgb2, color);
    vc[2] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
}
