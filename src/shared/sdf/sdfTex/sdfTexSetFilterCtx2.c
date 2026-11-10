#include "shared/sdf/sdfTex/decls.h"

void sdfTexSetFilterCtx2(sdfTexHandle_t *aHandle, s32 aMagMode, s32 aMinMode)
{
    sdfTexPacket_t *packet;

    packet = aHandle->packetCtx2;
    if (packet == NULL) {
        sdfTexMakeGIFPacketCtx2(aHandle);
        packet = aHandle->packetCtx2;
    }
    packet->tex1.data = (packet->tex1.data & ~0x1E0) | (aMagMode << 5) | (aMinMode << 6);
}
