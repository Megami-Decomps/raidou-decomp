#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySphereExpPartSetVC(effPCPFlashPolySphereExp_tag *work, s32 index, s32 color)
{
    u32 *vc;
    s32 rgb1;
    s32 rgb2;

    vc = (u32 *)dds3FlashPolyListGetVC(work->hPoly, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    vc[0] = effMagaCylinderCalcColor(rgb2, color);
    vc[1] = effMagaCylinderCalcColor(rgb2, color);
    if (index & 1) {
        vc[2] = effMagaCylinderCalcColor(0x80000000, color);
        vc[3] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
        vc[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        vc[2] = effMagaCylinderCalcColor(0xFF000000, color);
        vc[3] = effMagaCylinderCalcColor(rgb1 | 0x40000000, color);
        vc[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}
