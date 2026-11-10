#include "shared/sdf/sdfTex/decls.h"

s32 sdfTexGetPacketLength(sdfTexHandle_t *aHandle)
{
    sdfTexPacket_t *packet;

    packet = aHandle->packet;
    if (packet == NULL) {
        return 0;
    }
    return (((u32)packet->giftag.parm & 0x7FFF) + 1) << 4;
}
