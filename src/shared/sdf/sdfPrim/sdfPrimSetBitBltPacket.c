#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetBitBltPacket(sdfPrimBitBltPacket_t *aPacket, u32 aDstGsAddr, s32 aDstBuffWidth,
                                    s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                                    u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                                    s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aDirMode) {
    aPacket->header.dmaWords.tag = 5;
    aPacket->header.dmaWords.vif = (((u64)0x50000005 << 16 | 0x1000) << 16);
    aPacket->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8004);
    aPacket->header.giftag.regs = 0xE;
    sdfPrimSetBitBltElem(&aPacket->bitblt, aDstGsAddr, aDstBuffWidth,
                  aDstPixelFormat, aDstX, aDstY, aSrcGsAddr, aSrcBuffWidth,
                  aSrcPixelFormat, aSrcX, aSrcY, aWidth, aHeight, aDirMode);
}
