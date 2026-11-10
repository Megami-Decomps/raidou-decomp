#include "shared/sdf/sdfDraw/decls.h"

void sdfGrLinkDrawFrameTex(sdfDrawTag_t *aTag, u32 aEnv, sdfGrRefPacketHeader_t *aPkt) {
    aPkt->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    aPkt->dmaTag = SDF_DMA_TAG(aEnv + 0x1e0, 0x30000004);
    QUAD_ZERO(aPkt->nextTag);
    sdfDrawTagLinkRef(aTag, EE_ADDR(aPkt));
}
