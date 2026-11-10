#include "shared/effect/effPCPFlash/decls.h"

void effPCPFlashPolySphere3PartSetVC(effPCPFlashPolySphere3_tag *work, s32 index, u32 colorMultiplier) {
    u32 *colors;
    u32 colorA;
    u32 colorB;
    u32 *mirror;

    colors = (u32 *)dds3FlashPolyList3GetVC(work->hPoly, index * 2);
    colorA = work->centerColor & 0xFFFFFF;
    colorB = work->edgeColor & 0xFFFFFF;
    colors[0] = effMagaCylinderCalcColor(colorB | 0x80000000, colorMultiplier);
    colors[1] = effMagaCylinderCalcColor(colorB, colorMultiplier);
    colors[2] = effMagaCylinderCalcColor(colorA | 0x80000000, colorMultiplier);
    colors[3] = effMagaCylinderCalcColor(colorB, colorMultiplier);
    mirror = (u32 *)dds3FlashPolyList3GetVC(work->hPoly, index * 2 + 1);
    mirror[0] = colors[0];
    mirror[1] = colors[1];
    mirror[2] = colors[2];
    mirror[3] = colors[3];
}
