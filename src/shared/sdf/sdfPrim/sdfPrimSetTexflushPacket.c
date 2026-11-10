#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetTexflushPacket(sdfPrimTexflushPacket_t *aPkt) {
    sdfPrimSetTexflushElem(&aPkt->texflush);
    aPkt->header.dmaWords.tag = 2;
    aPkt->header.dmaWords.vif = (((u64)0x50000002 << 16 | 0x1000) << 16);
    aPkt->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8001);
    aPkt->header.giftag.regs = 0xE;
}
