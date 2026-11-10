#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetOpaque2Packet(sdfPrimTransPacket_t *aPkt) {
    sdfPrimSetOpaque2Elem(&aPkt->elem);
    aPkt->header.dmaWords.tag = SDF_ALPHA_DMA_QWORDS;
    aPkt->header.dmaWords.vif = (((u64)SDF_VIF_DIRECT_THREE_WORD << 16 | SDF_VIF_FLUSHE_HALFWORD) << 16);
    aPkt->header.giftag.parm = (((u64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_TWO_AD_LOOPS_EOP);
    aPkt->header.giftag.regs = SDF_GIF_REGISTER_AD;
}
