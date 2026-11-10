#include "shared/sdf/sdfPacketBuff/decls.h"

s32 sdfGrAllocPacketBuff(s32 aSize) {
    s32 packet;

    packet = sdfGrPacketBuffPtr;
    sdfGrPacketBuffPtr = sdfGrPacketBuffPtr + ((aSize + 0xfU) & 0xfffffff0);
    return packet;
}
