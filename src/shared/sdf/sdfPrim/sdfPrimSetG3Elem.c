#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetG3Elem(sdfPrimG3Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;

    aElem->gifTag = 0x8400000000008001ULL;
    aElem->gifRegisters = 0xFFFFFFFFF5151510ULL;
    aElem->primitive = (u32)(aPrim | 0x10C);
    aElem->rgbaq0 = (u32)aColor0 | ((u64)0xFE00 << 46);
    aElem->xy0 = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | depthHigh;
    aElem->rgbaq1 = (u32)aColor1 | ((u64)0xFE00 << 46);
    aElem->xy1 = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | depthHigh;
    aElem->rgbaq2 = (u32)aColor2 | ((u64)0xFE00 << 46);
    aElem->xy2 = (u32)((aX2 & 0xFFFF) | (aY2 << 16)) | depthHigh;
}
