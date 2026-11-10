#include "shared/sdf/sdfBillBoard/decls.h"

addr_t sdfBillBoardVcMakeN(const u128 *aCenter, const void *aColor,
                  const void *aUVList, const void *aXYOffsetList,
                  s32 aNum, void *(*aAllocator)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfBillBoardGetPacketSize(aNum);
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
    packet[4] = 0xD0000000;
    packet[5] = 0x34134130;
    packet[6] = 0x41341;
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

    code = aNum << 18;
    index = aNum + 2;
    n = aNum * 4;
    *cursor++ = code | index | 0x6E00C000;
    memcpy(cursor, aColor, n * 4);
    cursor += n;
    index += n;
    *cursor++ = code | index | 0x6500C000;
    memcpy(cursor, aUVList, n * 4);
    cursor += n;
    index += n;
    n = aNum * 8;
    *cursor++ = code | index | 0x6400C000;
    memcpy(cursor, aXYOffsetList, n * 4);
    cursor += n;
    cursor[0] = 0x04000006;
    cursor[1] = 0x14000008;
    cursor += 2;
    while ((EE_ADDR(cursor) & 0xF) != 0) {
        *cursor++ = 0;
    }
    return EE_ADDR(packet);
}
