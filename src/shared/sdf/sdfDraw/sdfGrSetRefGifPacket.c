#include "shared/sdf/sdfDraw/decls.h"

void sdfGrSetRefGifPacket(sdfGrRefGifPacketHeader_t *aPacket, addr_t aRegs, s32 aNumRegs) {
    aPacket->dmatag.data = SDF_DMA_CNT_ONE_WORD;
    aPacket->dmatag.reg = SDF_VIF_DIRECT_ONE_FLUSHE;
    aPacket->giftag.data = (u64)aNumRegs | SDF_GIF_PACKED_AD_BITS;
    aPacket->giftag.reg = SDF_GIF_REGISTER_AD;
    aPacket->reftag.data = SDF_DMA_TAG_WORD_FIRST(
        (u32)((aNumRegs & SDF_DMA_QWC_MASK) | SDF_DMA_TAG_REF_WORD), aRegs);
    aPacket->reftag.reg = SDF_VIF_DIRECT_ONE_NOP;
    aPacket->linktag.data = SDF_DMA_TAG_NEXT_WORD;
    aPacket->linktag.reg = 0;
}
