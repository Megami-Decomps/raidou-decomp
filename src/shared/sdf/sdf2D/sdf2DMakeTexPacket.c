#include "shared/sdf/sdf2D/decls.h"

sdf2DTexPacket_t *sdf2DMakeTexPacket(sdf2DTexPacket_t *aPacket, sdfTexHandle_t *aTexHndl, s32 aCtx) {
    aPacket->header.dmatag.qwc = 4;
    aPacket->header.giftag.parm = 0x1000000000008003ULL;
    aPacket->header.dmatag.dat1 = 0x50000004;
    aPacket->header.dmatag.dat0 = 0;
    aPacket->header.giftag.regs = SDF_GIF_REGISTER_AD;
    aPacket->tex1.data = sdfTexGetTex1Reg(aTexHndl);
    aPacket->tex1.reg = aCtx + 0x14;
    aPacket->tex0.data = sdfTexGetTex0Reg(aTexHndl);
    aPacket->tex0.reg = aCtx + 6;
    aPacket->clamp.data = sdfTexGetClampReg(aTexHndl);
    aPacket->clamp.reg = aCtx + 8;
    return aPacket;
}
