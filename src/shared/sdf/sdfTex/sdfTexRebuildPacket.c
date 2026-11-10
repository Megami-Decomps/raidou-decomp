#include "shared/sdf/sdfTex/decls.h"

void sdfTexRebuildPacket(sdfTexHandle_t *aHandle)
{
    sdfTexPacket_t *packet;

    packet = aHandle->packet;
    if (packet != NULL) {
        sdfTexSetPacket(packet, aHandle->width, aHandle->height, sdfTexGetPixelAddr(aHandle),
                        aHandle->pixelFormat, sdfTexGetCLUTAddr(aHandle), aHandle->CLUTFormat,
                        (packet->tex0.data >> 34) & 1, aHandle->numOfMipmap, aHandle->mipmapParam,
                        aHandle->texWrapMode, 0);
    }

    packet = aHandle->packetCtx2;
    if (packet != NULL) {
        sdfTexSetPacket(packet, aHandle->width, aHandle->height, sdfTexGetPixelAddr(aHandle),
                        aHandle->pixelFormat, sdfTexGetCLUTAddr(aHandle), aHandle->CLUTFormat,
                        (packet->tex0.data >> 34) & 1, aHandle->numOfMipmap, aHandle->mipmapParam,
                        aHandle->texWrapMode, 1);
    }
}
