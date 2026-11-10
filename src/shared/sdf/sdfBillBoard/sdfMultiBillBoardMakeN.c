#include "shared/sdf/sdfBillBoard/decls.h"

u32 *sdfMultiBillBoardMakeN(u128 *aCenter, void *aColor, void *aUVList, void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfMultiBillBoardGetPacketSize(aNum);
    if (aAllocator == NULL) {
        cursor = EE_ADDR_AS(u32 *, sdfGrAllocPacketBuff(bytes));
    } else {
        cursor = aAllocator(bytes);
    }
    packet = cursor;
    packet[0] = (bytes >> 4) - 1;
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
    i = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, aCenter[i]);
        cursor += 3;
        i++;
    } while (i != aNum);
    n = aNum * 2;
    index = aNum + 2;
    *cursor++ = (n << 16) | index | 0x6E00C000;
    memcpy(cursor, aColor, n * 4);
    cursor += n;
    index += n;
    n = aNum * 4;
    code = n << 16;
    *cursor++ = code | index | 0x6D00C000;
    memcpy(cursor, aUVList, n * 8);
    cursor += n * 2;
    index += n;
    code |= index;
    *cursor++ = code | 0x6400C000;
    memcpy(cursor, aXYOffsetList, n * 8);
    cursor += n * 2;
    cursor[0] = 0x04000004;
    cursor[1] = 0x14000008;
    cursor += 2;
    while ((EE_ADDR(cursor) & 0xF) != 0) {
        *cursor++ = 0;
    }
    return packet;
}
