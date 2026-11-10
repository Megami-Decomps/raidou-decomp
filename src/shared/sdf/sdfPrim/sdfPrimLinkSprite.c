#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimLinkSprite(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimSpritePacket_t *packet;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimSpritePacket_t *, aAllocator(0x50));
    packet->dmaTag = 0x20000004;
    packet->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfPrimSetSpriteElem(&packet->spr, aColor, aPrim, aX0, aY0, aU0, aV0, aX1, aY1, aU1, aV1, aZ);
    sdfDrawTagLink(aTag, EE_ADDR_S(packet));
}
