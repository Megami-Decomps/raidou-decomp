#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimLinkBoxLine(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimBoxLinePacket_t *packet;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimBoxLinePacket_t *, aAllocator(0x60));
    packet->dmaTag = 0x20000005ULL;
    packet->vifCommands = 0x5000000510000000ULL;
    sdfPrimSetBoxLineElem(&packet->line, aColor, aPrim, aX0, aY0, aX1, aY1, aZ);
    sdfDrawTagLink(aTag, EE_ADDR_S(packet));
}
