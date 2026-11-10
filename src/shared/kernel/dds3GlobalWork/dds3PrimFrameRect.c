#include "shared/kernel/dds3GlobalWork/decls.h"

u64 *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol) {
    u64 *packet;
    s32 right;
    s32 bottom;
    u64 depthHigh;

    packet = (u64 *)sdfGrAllocPacketBuff(0x80);
    packet[0] = 7;
    right = aX + aW;
    bottom = aY + aH;
    depthHigh = (u64)(u32)aZ << 32;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0x55555105510ULL;
    packet[4] = 0x146;
    packet[5] = aBgCol | 0x3F80000000000000ULL;
    packet[6] = (u32)(((u32)aX & 0xFFFF) | ((u32)aY << 16)) | depthHigh;
    packet[7] = (u32)(((u32)right & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[8] = 0x142;
    packet[9] = aFrameCol | 0x3F80000000000000ULL;
    packet[14] = (u32)(((u32)(aX - 0x10) & 0xFFFF) | ((u32)(aY - 8) << 16)) | depthHigh;
    packet[10] = packet[14];
    packet[11] = (u32)(((u32)right & 0xFFFF) | ((u32)(aY - 8) << 16)) | depthHigh;
    packet[12] = (u32)(((u32)right & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[13] = (u32)(((u32)(aX - 0x10) & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[15] = 0;
    return packet;
}
