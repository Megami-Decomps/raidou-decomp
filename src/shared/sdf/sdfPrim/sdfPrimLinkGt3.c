#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimLinkGt3(s32 aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aColor0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1,
                   s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aColor2, s32 aZ,
                   s32 (*aAllocator)(s32)) {
    sdfPrimGt3Packet_t *packet;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimGt3Packet_t *, aAllocator(0x70));
    packet->dmatag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetGt3Elem(&packet->elem, aPrim, aX0, aY0, aU0, aV0, aColor0, aX1, aY1, aU1, aV1, aColor1, aX2, aY2, aU2, aV2, aColor2, aZ);
    sdfDrawTagLink(EE_ADDR_PTR(aTag), EE_ADDR_S(packet));
}
