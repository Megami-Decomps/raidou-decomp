#include "shared/sdf/sdfBillBoard/decls.h"

void sdfBillBoardMakeTMPacket(u8 *aPacket, const f32 *aTM) {
    sdfBillBoardTMPacket_t *p = (sdfBillBoardTMPacket_t *)aPacket;

    p->dma_qwc = 2;
    p->vif_unpack = 0x6403C000;
    p->tm[0] = aTM[0];
    p->tm[1] = aTM[1];
    p->tm[2] = aTM[4];
    p->tm[3] = aTM[5];
    p->tm[4] = aTM[12];
    p->tm[5] = aTM[13];
    p->vif_itop = 0x04000000;
    p->vif_mscal = SDF_VIF_MSCAL_VERTICES;
    memset(p->pad, 0, sizeof(p->pad));
}
