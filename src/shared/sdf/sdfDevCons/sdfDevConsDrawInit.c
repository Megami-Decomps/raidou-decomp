#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsDrawInit(sdfDevConsDrawCB_t *aCB, void *aPacketBuffer, s32 aBufferSize) {
    addr_t ringAddress;
    u8 *ring;
    vu64 *packet;
    u64 textureState;
    s32 halfBufferBytes = aBufferSize >> 1;

    ringAddress = EE_UNCACHED_RING(EE_PHYSICAL(EE_ADDR(aPacketBuffer)));
    ring = EE_ADDR_AS(u8 *, ringAddress);
    aCB->buffSize = halfBufferBytes;
    aCB->buf[0] = ring;
    aCB->buf[1] = ring + halfBufferBytes;
    aCB->start = ring;
    aCB->dblBuffId = 0;
    textureState = sdfTexGetTex0Reg(sdfDebugFontHandle);
    packet = (vu64 *)ring;
    packet[0] = 0x10AB400000008005ULL;
    packet[1] = 0xE;
    packet[2] = 0x44;
    packet[3] = 0x42;
    packet[4] = 0x3000D;
    packet[5] = 0x47;
    packet[6] = 0;
    packet[7] = 8;
    packet[8] = 1;
    packet[9] = 0x14;
    packet[10] = textureState;
    packet[11] = 6;
    aCB->tag = ring + 0x60;
    aCB->cur = aCB->tag + 0x10;
    aCB->dmaCh = sceDmaGetChan(2);
}
