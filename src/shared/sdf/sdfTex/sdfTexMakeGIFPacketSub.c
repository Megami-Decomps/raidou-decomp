#include "shared/sdf/sdfTex/decls.h"

sdfTexPacket_t *sdfTexMakeGIFPacketSub(sdfTexHandle_t *aHandle, s32 aContext)
{
    sdfTexPacket_t *packet;
    u32 pixelAddr;
    u32 clutAddr;

    packet = sdfChipAllocDirect(64);
    pixelAddr = sdfTexGetPixelAddr(aHandle);
    clutAddr = sdfTexGetCLUTAddr(aHandle);
    sdfTexSetPacket(packet, aHandle->width, aHandle->height, pixelAddr, aHandle->pixelFormat,
                    clutAddr, aHandle->CLUTFormat, 1, aHandle->numOfMipmap, aHandle->mipmapParam,
                    aHandle->texWrapMode, aContext);
    return packet;
}
