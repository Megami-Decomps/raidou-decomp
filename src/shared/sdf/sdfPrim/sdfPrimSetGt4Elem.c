#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetGt4Elem(sdfPrimGt4Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aColor0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1, s32 aX2, s32 aY2, s32 aU2,
                   s32 aV2, s32 aColor2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aColor3, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;

    aElem->giftag = 0xE400000000008001ULL;
    aElem->gifRegisters = 0xF5135135135130ULL;
    aElem->prim = (u32)(aPrim | 0x11C);
    aElem->uv0 = (aU0 & 0xFFFF) | (aV0 << 16);
    aElem->rgbaq0 = (u32)aColor0 | ((u64)0xFE00 << 46);
    aElem->xy0 = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | depthHigh;
    aElem->uv1 = (aU1 & 0xFFFF) | (aV1 << 16);
    aElem->rgbaq1 = (u32)aColor1 | ((u64)0xFE00 << 46);
    aElem->xy1 = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | depthHigh;
    aElem->uv2 = (aU2 & 0xFFFF) | (aV2 << 16);
    aElem->rgbaq2 = (u32)aColor2 | ((u64)0xFE00 << 46);
    aElem->xy2 = (u32)((aX2 & 0xFFFF) | (aY2 << 16)) | depthHigh;
    aElem->uv3 = (aU3 & 0xFFFF) | (aV3 << 16);
    aElem->rgbaq3 = (u32)aColor3 | ((u64)0xFE00 << 46);
    aElem->xy3 = (u32)((aX3 & 0xFFFF) | (aY3 << 16)) | depthHigh;
}
