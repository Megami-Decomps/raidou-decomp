#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimLinkF3(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1,
                   s32 aY1, s32 aX2, s32 aY2, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimF3Packet_t *packet;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimF3Packet_t *, aAllocator(0x50));
    packet->dmaTag = 0x20000004;
    packet->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfPrimSetF3Elem(&packet->elem, aColor, aPrim, aX0, aY0, aX1, aY1, aX2, aY2, aZ);
    sdfDrawTagLink(aTag, EE_ADDR_S(packet));
}
