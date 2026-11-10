#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimLinkRect(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimRectPacket_t *packet;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimRectPacket_t *, aAllocator(0x40));
    packet->dmaTag = 0x20000003ULL;
    packet->vifCommands = 0x5000000310000000ULL;
    sdfPrimSetRectElem(&packet->elem, aColor, aPrim, aX0, aY0, aX1, aY1, aZ);
    sdfDrawTagLink(aTag, EE_ADDR(packet));
}
