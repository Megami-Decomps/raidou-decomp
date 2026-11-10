#include "shared/sdf/sdfBillBoard/decls.h"

void sdfMultiBillBoardVcMake1(u8 *aPacket, const f32 *aCenter, const void *aColor, const void *aUVList0,
                   const void *aUVList1, const void *aXYOffsetList) {
    sdfMultiBillBoardVc1Packet_t *p = (sdfMultiBillBoardVc1Packet_t *)aPacket;

    p->dma_qwc = 10;
    p->vif_unpack1 = 0x6C01C000;
    p->count = 1;
    p->giftag_h = 0xD0000000;
    p->giftag_reg_l = 0x34134130;
    p->giftag_reg_h = 0x41341;
    p->vif_unpack2 = 0x6001C001;
    p->prim = 0x15D;
    p->vif_unpack3 = 0x6801C002;
    p->center[0] = aCenter[0];
    p->center[1] = aCenter[1];
    p->center[2] = aCenter[2];
    p->vif_unpack4 = 0x6E08C003;
    memcpy(aPacket + 56, aColor, 32);
    p->vif_unpack5 = 0x6D04C00B;
    EE_LOAD_UNALIGNED_WORD(p->uv[0], (const u8 *)aUVList0);
    EE_LOAD_UNALIGNED_WORD(p->uv[1], (const u8 *)aUVList1);
    EE_LOAD_UNALIGNED_WORD(p->uv[2], (const u8 *)aUVList0 + 4);
    EE_LOAD_UNALIGNED_WORD(p->uv[3], (const u8 *)aUVList1 + 4);
    EE_LOAD_UNALIGNED_WORD(p->uv[4], (const u8 *)aUVList0 + 8);
    EE_LOAD_UNALIGNED_WORD(p->uv[5], (const u8 *)aUVList1 + 8);
    EE_LOAD_UNALIGNED_WORD(p->uv[6], (const u8 *)aUVList0 + 12);
    EE_LOAD_UNALIGNED_WORD(p->uv[7], (const u8 *)aUVList1 + 12);
    p->vif_unpack6 = 0x6404C00F;
    memcpy(aPacket + 128, aXYOffsetList, 32);
    p->vif_itop = 0x04000008;
    p->vif_mscal = SDF_VIF_MSCAL_VERTICES;
    p->pad[0] = 0;
    p->pad[1] = 0;
}
