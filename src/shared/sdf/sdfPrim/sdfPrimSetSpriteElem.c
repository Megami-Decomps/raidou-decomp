#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetSpriteElem(sdfPrimSpriteElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;

    aElem->gifTag = 0x6400000000008001ULL;
    aElem->gifRegisters = 0x535310;
    aElem->primitive = (u32)(aPrim | 0x116);
    aElem->rgbaq = (u32)aColor | ((u64)0xFE00 << 46);
    aElem->uv0 = (aU0 & 0xFFFF) | (aV0 << 16);
    aElem->xy0 = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | depthHigh;
    aElem->uv1 = (aU1 & 0xFFFF) | (aV1 << 16);
    aElem->xy1 = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | depthHigh;
}
