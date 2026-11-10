#include "shared/effect/effPacket/decls.h"

u64 *effPkt2DAllocF4(u32 prim) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x80);

    packet[0] = 7;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0xFF515151510ULL;
    packet[4] = prim | 0x14C;
    packet[15] = 0;
    packet[14] = 0;
    packet[13] = 0;
    return packet;
}
