#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetBitBltElem(sdfPrimBitBltElem_t *aElem, u32 aDstGsAddr, s32 aDstBuffWidth,
                  s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                  u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                  s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aDirMode) {
    aElem->bitbltbuf.data = (aDstPixelFormat << 56) |
        ((u64)(aDstBuffWidth >> 6) << 48) |
        ((u64)(aDstGsAddr >> 6) << 32) |
        (aSrcPixelFormat << 24) | ((aSrcBuffWidth >> 6) << 16) |
        (aSrcGsAddr >> 6);
    aElem->bitbltbuf.reg = SDF_GS_BITBLTBUF;
    aElem->trxpos.data = (aDstY << 48) | (aDstX << 32) |
        (aSrcY << 16) | aSrcX;
    aElem->trxpos.reg = SDF_GS_TRXPOS;
    aElem->trxreg.data = aWidth | ((u64)aHeight << 32);
    aElem->trxreg.reg = SDF_GS_TRXREG;
    aElem->trxdir.data = aDirMode;
    aElem->trxdir.reg = SDF_GS_TRXDIR;
}
