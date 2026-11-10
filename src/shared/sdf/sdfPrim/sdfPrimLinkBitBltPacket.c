#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimLinkBitBltPacket(s32 aTag, u32 aDstGsAddr, s32 aDstBuffWidth,
                             s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                             u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                             s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight,
                             s32 aDirMode, s32 (*aAllocator)(s32)) {
    s32 packetAddress;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packetAddress = aAllocator(0x60);
    sdfPrimSetBitBltPacket(EE_ADDR_AS(sdfPrimBitBltPacket_t *, packetAddress), aDstGsAddr, aDstBuffWidth,
                                    aDstPixelFormat, aDstX, aDstY, aSrcGsAddr, aSrcBuffWidth,
                                    aSrcPixelFormat, aSrcX, aSrcY, aWidth, aHeight, aDirMode);
    sdfDrawTagLink(EE_ADDR_PTR(aTag), packetAddress);
}
