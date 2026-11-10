#include "shared/kernel/dds3GlobalWork/decls.h"

u64 *dds3PrimLine(s32 aX0, s32 aY0, s32 aZ0, s32 aColor0, s32 aX1, s32 aY1, s32 aZ1, s32 aColor1, s32 aMode) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x50);

    packet[0] = 4;
    packet[1] = 0x5000000400000000ULL;
    packet[2] = 0x5400000000008001ULL;
    packet[3] = 0x51510;
    packet[4] = aMode | 0x109;
    packet[5] = (u32)aColor0 | ((u64)0xFE00 << 46);
    packet[6] = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | ((u64)((u32)aZ0 >> 4) << 32);
    packet[7] = (u32)aColor1 | ((u64)0xFE00 << 46);
    packet[8] = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | ((u64)((u32)aZ1 >> 4) << 32);
    packet[9] = 0;
    return packet;
}
