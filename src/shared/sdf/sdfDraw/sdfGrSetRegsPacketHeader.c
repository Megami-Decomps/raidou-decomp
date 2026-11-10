#include "shared/sdf/sdfDraw/decls.h"

void sdfGrSetRegsPacketHeader(sdfGrPacketHeader_t *header, s32 payloadQwords) {
    s64 dmaQwords;

    dmaQwords = payloadQwords + 1;
    header->dmaWords.vif = (((dmaQwords | SDF_VIF_DIRECT_WORD) << 32) | SDF_VIF_FLUSHE_WORD);
    header->giftag.parm = (payloadQwords | (((s64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_EOP_BIT));
    header->dmaWords.tag = dmaQwords;
    header->giftag.regs = SDF_GIF_REGISTER_AD;
}
