#include "shared/interface/itfTestMesManager/decls.h"

void *pbMakeSpritePacket(u64 data, s32 context)
{
    pbRegPacket_t *packet;

    packet = sdf2DMakeStdPacket(sdfGrAllocPacketBuff(0x30), 0x30);
    packet->reg[1].data = data;
    packet->reg[1].reg = context ? 0x48 : 0x47;
    return packet;
}
