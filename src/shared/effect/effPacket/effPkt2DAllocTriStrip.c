#include "shared/effect/effPacket/decls.h"

sdfGrPacketHeader_t *effPkt2DAllocTriStrip(s32 num, s32 prim) {
    sdfGrPacketHeader_t *packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(9, num));

    sdf2DMakePrimPacketHeader(packet, prim | 0x54, 9, 0x525252521, num);
    return packet;
}
