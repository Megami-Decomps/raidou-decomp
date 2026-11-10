#include "shared/sdf/sdf3DPrim/decls.h"

void sdf3DPrimLinkTm(sdfDrawTag_t *aTag,
                           s32 (*aAllocator)(s32)) {
    u64 *dmaPacket;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    dmaPacket = EE_ADDR_AS(u64 *, aAllocator(0x90));
    dmaPacket[0] = SDF_DMA_TAG(EE_ADDR(dmaPacket + 2), 0x20000008);
    dmaPacket[1] = 0x6C07C000ULL << 32;
    sdf3DPrimSetTmPacket((sdf3DTmPacketData_t *)(dmaPacket + 2));
    sdfDrawTagLink(aTag, EE_ADDR(dmaPacket));
}
