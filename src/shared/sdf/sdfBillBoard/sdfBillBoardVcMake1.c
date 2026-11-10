#include "shared/sdf/sdfBillBoard/decls.h"

void sdfBillBoardVcMake1(u8 *aPacket, const f32 *aCenter, const u32 *aColor, const void *aUVList,
                   const f32 *aXYOffsetList) {
    sdfBillBoardVc1Packet_t *p = (sdfBillBoardVc1Packet_t *)aPacket;

    p->dma_qwc = 8;
    p->vif_unpack1 = 0x6C01C000;
    p->count = 1;
    p->giftag_h = 0xD0000000;
    p->giftag_reg_l = 0x34134130;
    p->giftag_reg_h = 0x41341;
    p->vif_unpack2 = 0x6001C001;
    p->prim = 0x155;
    p->vif_unpack3 = 0x6801C002;
    p->center[0] = aCenter[0];
    p->center[1] = aCenter[1];
    p->center[2] = aCenter[2];
    p->vif_unpack4 = 0x6E04C003;
    p->color[0] = aColor[0];
    p->color[1] = aColor[1];
    p->color[2] = aColor[2];
    p->color[3] = aColor[3];
    p->vif_unpack5 = 0x6504C007;
    EE_LOAD_UNALIGNED_WORD(p->uv[0], (const u8 *)aUVList);
    EE_LOAD_UNALIGNED_WORD(p->uv[1], (const u8 *)aUVList + 4);
    EE_LOAD_UNALIGNED_WORD(p->uv[2], (const u8 *)aUVList + 8);
    EE_LOAD_UNALIGNED_WORD(p->uv[3], (const u8 *)aUVList + 12);
    p->vif_unpack6 = 0x6404C00B;
    p->xyOffset[0] = aXYOffsetList[0];
    p->xyOffset[1] = aXYOffsetList[1];
    p->xyOffset[2] = aXYOffsetList[2];
    p->xyOffset[3] = aXYOffsetList[3];
    p->xyOffset[4] = aXYOffsetList[4];
    p->xyOffset[5] = aXYOffsetList[5];
    p->xyOffset[6] = aXYOffsetList[6];
    p->xyOffset[7] = aXYOffsetList[7];
    p->vif_itop = 0x04000006;
    p->vif_mscal = SDF_VIF_MSCAL_VERTICES;
    p->pad[0] = 0;
    p->pad[1] = 0;
}
