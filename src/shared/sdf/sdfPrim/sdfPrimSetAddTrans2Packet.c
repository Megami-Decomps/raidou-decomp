#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetAddTrans2Packet(sdfPrimTransPacket_t *aPkt) {
    sdfPrimSetAddTrans2Elem(&aPkt->elem);
    aPkt->header.dmaWords.tag = 3;
    aPkt->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    aPkt->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    aPkt->header.giftag.regs = 0xE;
}
