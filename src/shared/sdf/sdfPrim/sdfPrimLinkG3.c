#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimLinkG3(sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimG3Packet_t *packet;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimG3Packet_t *, aAllocator(0x60));
    packet->dmaTag = 0x20000005ULL;
    packet->vifCommands = 0x5000000510000000ULL;
    sdfPrimSetG3Elem(&packet->elem, aPrim, aX0, aY0, aColor0, aX1, aY1, aColor1, aX2, aY2, aColor2, aZ);
    sdfDrawTagLink(aTag, EE_ADDR_S(packet));
}
