#include "shared/sdf/sdfIpu/decls.h"

/* Build both frame buffers' GIF upload packets: a BITBLTBUF header followed
 * by one transfer per 16x16 tile. */
void sdfIpuMakeBltPacket(sdfIpuDecodeCb_t *cb)
{
    s32 pixelMode;
    u32 tileQwords;
    s32 width;
    s32 height;
    s32 bufferWidth;
    s32 remainingTiles;
    s32 packetBytes;
    s32 sourceStride;
    s32 bufferIndex;
    s32 x;
    s32 y;
    s32 endOfPacket;
    addr_t sourceAddress;
    u64 bitblt;
    u32 sourceTag;
    u64 imageTag;
    u64 *packet;

    if (cb->rgbMode == 0) {
        pixelMode = 0;
        tileQwords = 64;
    } else {
        pixelMode = 2;
        tileQwords = 32;
    }
    width = cb->width;
    height = cb->height;
    /* Retail shares this count across both packet buffers. */
    remainingTiles = (cb->width >> 4) * (cb->height >> 4);
    bufferWidth = width >> 6;
    if (bufferWidth == 0) {
        bufferWidth = 1;
    }
    bitblt = ((u64)pixelMode << 56) | ((u64)bufferWidth << 48);
    sourceTag = tileQwords | 0x30000000;
    imageTag = (u64)(tileQwords | 0x50000000) << 32;
    sourceStride = tileQwords << 4;
    packetBytes = remainingTiles * 0x60 + 0x50;
    cb->transferPacketBytes = packetBytes;

    bufferIndex = 0;
    do {
        packet = sdfAllocAuto(packetBytes);
        cb->transferPacketBuffers[bufferIndex] = packet;
        sourceAddress = EE_PHYSICAL(EE_ADDR(cb->frameBuffers[bufferIndex]));

        packet[0] = 0x10000003;
        packet[1] = (u64)0x50000003 << 32;
        packet[4] = bitblt | ((u64)((u32)cb->resourceWord >> 6) << 32);
        packet[5] = 0x50;
        packet[6] = ((u64)16 << 32) | 16;
        packet[7] = 0x52;
        packet[2] = ((u64)0x10000000 << 32) | 2;
        packet[3] = 0xE;
        packet += 8;

        endOfPacket = 0;
        y = 0;
        do {
            x = 0;
            do {
                remainingTiles--;
                if (remainingTiles == 0) {
                    endOfPacket = 1;
                }
                packet[0] = 0x10000004;
                packet[1] = (u64)0x50000004 << 32;
                packet[5] = 0x51;
                packet[4] = ((u64)y << 48) | ((u64)x << 32);
                packet[8] = (u64)(endOfPacket << 15) | (u64)tileQwords | ((u64)2 << 58);
                packet[10] = sourceTag | ((u64)sourceAddress << 32);
                sourceAddress += sourceStride;
                packet[2] = ((u64)0x10000000 << 32) | 2;
                packet[3] = 0xE;
                packet[6] = 0;
                packet[7] = 0x53;
                packet[9] = 0;
                packet[11] = imageTag;
                x += 16;
                packet += 12;
            } while (x < width);
            y += 16;
        } while (y < height);
        bufferIndex++;
    } while (bufferIndex != 2);
}
