#include "shared/sdf/sdfVu1/decls.h"

u64 *corePktClosePacket(sdfCorePacketCb_t *cb) {
    addr_t cursor;
    addr_t start;
    s32 quadwords;
    u32 descriptor;
    u64 gifTag;
    u64 *header;

    if (cb->packetOpenPtr == 0) {
        return NULL;
    }

    cursor = EE_ADDR(cb->packetPtr);
    start = EE_ADDR(cb->packetStartPtr);
    if (cursor == start) {
        sdfGrClosePacketBuff(cb->packetOpenPtr);
        return NULL;
    }

    while (((u32)cursor & 0xC) != 0) {
        *EE_ADDR_AS(u32 *, cursor) = 0;
        cursor += 4;
    }
    quadwords = (s32)(cursor - start) >> 4;

    while (((u32)cursor & 0x30) != 0) {
        QUAD_ZERO(*EE_ADDR_AS(u128 *, cursor));
        cursor += 0x10;
    }

    sdfGrClosePacketBuff(EE_PHYSICAL(cursor));
    header = EE_ADDR_AS(u64 *, cb->packetTopPtr);
    descriptor = 0x20000000 | (quadwords & 0xFFFF);
    gifTag = 0x1100000000000000ULL;
    header[0] = descriptor;
    header[1] = gifTag;
    return header;
}
