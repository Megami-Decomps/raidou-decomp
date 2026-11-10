#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimLinkF4(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aX3, s32 aY3, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimF4Packet_t *packet;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimF4Packet_t *, aAllocator(0x50));
    packet->dmaTag = 0x20000004ULL;
    packet->vifCommands = 0x5000000410000000ULL;
    sdfPrimSetF4Elem(&packet->elem, aColor, aPrim, aX0, aY0, aX1, aY1, aX2, aY2, aX3, aY3, aZ);
    sdfDrawTagLink(aTag, EE_ADDR_S(packet));
}
