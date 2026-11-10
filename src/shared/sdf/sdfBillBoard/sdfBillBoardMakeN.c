#include "shared/sdf/sdfBillBoard/decls.h"

/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 *sdfBillBoardMakeN(const u128 *aCenter, const void *aColor, const void *aUVList, const void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32)) {
    s32 packetBytes;
    u32 *packet;
    u32 *cursor;
    u32 vuAddress;
    u32 unpackCode;
    s32 n;
    s32 vertexIndex;

    packetBytes = sdfBillBoardGetPacketSize(aNum);
    if (aAllocator == NULL) {
        cursor = EE_ADDR_AS(u32 *, sdfGrAllocPacketBuff(packetBytes));
    } else {
        cursor = aAllocator(packetBytes);
    }
    packet = cursor;
    packet[0] = (packetBytes >> SDF_DMA_QWORD_SHIFT) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = aNum;
    packet[4] = 0xA0000000;
    packet[5] = 0x43434310;
    packet[6] = 0x43;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (aNum << 16) | 0x6800C002;
    cursor = packet + 10;
    vertexIndex = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, aCenter[vertexIndex]);
        cursor += 3;
        vertexIndex++;
    } while (vertexIndex != aNum);
    n = aNum;
    unpackCode = n << 16;
    vuAddress = aNum + 2;
    *cursor++ = unpackCode | vuAddress | 0x6E00C000;
    memcpy(cursor, aColor, n * 4);
    cursor += n;
    vuAddress += n;
    n = aNum * 4;
    unpackCode = n << 16;
    *cursor++ = unpackCode | vuAddress | 0x6500C000;
    vuAddress += n;
    unpackCode |= vuAddress;
    memcpy(cursor, aUVList, n * 4);
    cursor += n;
    n *= 8;
    unpackCode |= 0x6400C000;
    *cursor++ = unpackCode;
    memcpy(cursor, aXYOffsetList, n);
    cursor = (u32 *)((u8 *)cursor + n);
    cursor[0] = SDF_VIF_ITOP_COMPACT_VERTEX;
    cursor[1] = SDF_VIF_MSCAL_VERTICES;
    cursor += 2;
    while ((EE_ADDR(cursor) & SDF_QWORD_ALIGNMENT_MASK) != 0) {
        *cursor++ = 0;
    }
    return packet;
}
