#include "shared/sdf/sdfDraw/decls.h"

void sdfGrLinkDispFrameTex(sdfDrawTag_t *aTag, addr_t aEnv, sdfGrRefPacketHeader_t *aPkt) {
    aPkt->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    aPkt->dmaTag = SDF_DMA_TAG(aEnv + 0x1a0, 0x30000004);
    QUAD_ZERO(aPkt->nextTag);
    sdfDrawTagLinkRef(aTag, EE_ADDR(aPkt));
}
