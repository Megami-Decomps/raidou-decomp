#include "shared/sdf/sdfTex/decls.h"

sdfTexPacket_t *sdfTexGetPacketCtx2(sdfTexHandle_t *aHandle)
{
    sdfTexPacket_t *packet;

    packet = aHandle->packetCtx2;
    if (packet == NULL) {
        sdfTexMakeGIFPacketCtx2(aHandle);
        packet = aHandle->packetCtx2;
    }
    return packet;
}
