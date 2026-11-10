#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetBoxLineElem(sdfPrimBoxLineElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ) {
    u64 depthHigh = (u64)(u32)aZ << 32;
    u32 x0 = (u32)aX0 & 0xFFFF;
    u32 x1 = (u32)aX1 & 0xFFFF;
    u32 y0 = (u32)aY0 << 16;
    u32 y1 = (u32)aY1 << 16;

    aElem->giftag = 0x8400000000008001ULL;
    aElem->gifRegisterList = (s32)0xF5555510;
    aElem->prim = (u32)aPrim | 0x102;
    aElem->rgbaq = (u32)aColor | 0x3F80000000000000ULL;
    aElem->xyz2[0] = (x0 | y0) | depthHigh;
    aElem->xyz2[1] = (x1 | y0) | depthHigh;
    aElem->xyz2[2] = (x1 | y1) | depthHigh;
    aElem->xyz2[3] = (x0 | y1) | depthHigh;
    aElem->xyz2[4] = (x0 | y0) | depthHigh;
}
