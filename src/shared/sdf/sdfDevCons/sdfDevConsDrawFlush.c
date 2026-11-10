#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsDrawFlush(sdfDevConsDrawCB_t *aCB) {
    s32 packetBytes;
    u32 *channelRegisters;
    u32 nextBufferIndex;

    sdfDevConsTagClose(aCB);
    packetBytes = aCB->tag - aCB->start;
    if ((packetBytes >> 3) != 0) {
        channelRegisters = aCB->dmaCh;
        sceDmaSync((sceDmaChan *)channelRegisters, 0, 0);
        HW_WRITE((volatile u32 *)channelRegisters, HW_READ((volatile u32 *)channelRegisters) | 0x40); /* CHCR bit 6: tag-transfer enable */
        EE_SYNC_BARRIER();
        sceDmaSendN(channelRegisters, (void *)EE_PHYSICAL(EE_ADDR(aCB->start)), packetBytes >> 4);
        nextBufferIndex = aCB->dblBuffId ^ 1;
        aCB->dblBuffId = nextBufferIndex;
        aCB->tag = (u8 *)aCB->buf[nextBufferIndex];
        aCB->cur = aCB->tag + 0x10;
        aCB->start = aCB->tag;
        sceGsSyncPath(0, 0);
    }
}
