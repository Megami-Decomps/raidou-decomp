#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetF4Elem(sdfPrimF4Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aX3, s32 aY3, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;

    aElem->gifTag = 0x6400000000008001ULL;
    aElem->gifRegisterList = 0x555510;
    aElem->primitive = (u32)(aPrim | 0x104);
    aElem->rgbaq = (u32)aColor | ((u64)0xFE00 << 46);
    aElem->xyz2[0] = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | depthHigh;
    aElem->xyz2[1] = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | depthHigh;
    aElem->xyz2[2] = (u32)((aX2 & 0xFFFF) | (aY2 << 16)) | depthHigh;
    aElem->xyz2[3] = (u32)((aX3 & 0xFFFF) | (aY3 << 16)) | depthHigh;
}
