#include "shared/sdf/sdf2D/decls.h"

s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx) {
    s32 packetBytes = sdf2DGetTexPacketSize(aTexHndl);
    sdf2DTexPacket_t *packet = EE_ADDR_AS(sdf2DTexPacket_t *, sdfGrAllocPacketBuff(packetBytes));
    saddr_t packetAddress = EE_ADDR_S(sdf2DMakeTexPacket(packet, aTexHndl, aCtx));
    sdfDrawTagLink(aTag, packetAddress);
    return packetAddress;
}
