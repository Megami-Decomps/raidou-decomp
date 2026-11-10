#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetF3Elem(sdfPrimF3Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;
    u32 first = ((u32)aX0 & 0xFFFF) | ((u32)aY0 << 16);
    u32 second = ((u32)aX1 & 0xFFFF) | ((u32)aY1 << 16);
    u32 third = ((u32)aX2 & 0xFFFF) | ((u32)aY2 << 16);

    aElem->gifTag = 0x6400000000008001ULL;
    aElem->gifRegisterList = 0xF55510;
    aElem->primitive = (u32)aPrim | 0x104;
    aElem->rgbaq = (u32)aColor | 0x3F80000000000000ULL;
    aElem->xyz2[0] = first | depthHigh;
    aElem->xyz2[1] = second | depthHigh;
    aElem->xyz2[2] = third | depthHigh;
}
