#include "shared/interface/itfTestMesManager/decls.h"

void *pbMakePrimPacket(u64 data, s32 context)
{
    pbRegPacket_t *packet;

    packet = sdf2DMakeStdPacket(sdfGrAllocPacketBuff(0x30), 0x30);
    packet->reg[1].data = data;
    packet->reg[1].reg = context ? 0x43 : 0x42;
    return packet;
}
