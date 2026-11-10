#include "shared/sdf/sdf3DPrim/decls.h"

u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm) {
    s32 stripWords = aParm->faceCount;
    s32 count = aParm->pointCount;
    s32 headerWords = 18;
    s32 wordsPerVertex = 3;
    u32 primitive = aParm->attr | 0x68;
    u32 format = 0x60;
    s32 bytes;
    void *(*allocator)(s32);
    u32 *packet;
    u32 *cursor;
    const u128 *vector;
    const u32 *coordinates;
    const u32 *secondCoordinates;
    s32 i;
    u32 countCode;

    if (aParm->normal != NULL) {
        format = 0xE0;
        headerWords = 19;
        wordsPerVertex = 6;
    }
    if (aParm->uv != NULL) {
        format |= 0x100;
        primitive |= 0x10;
        headerWords++;
        wordsPerVertex += 2;
        if (aParm->multiUv != NULL) {
            primitive |= 0x1000;
            wordsPerVertex += 2;
        }
    }
    if (aParm->vertexColor != NULL) {
        format |= 0x200;
        primitive |= 0x800;
        headerWords++;
        wordsPerVertex++;
    }
    bytes = (headerWords + stripWords + wordsPerVertex * count) * 4;
    bytes = (bytes + 15) & ~15;
    allocator = aParm->allocator;
    if (allocator == NULL) allocator = sdfGrAllocPacketBuff;
    packet = allocator(bytes);
    *(u64 *)packet = (u64)(u16)((bytes >> 4) - 1) | 0x20000000ULL;
    packet[2] = 0x6102C000;
    packet[3] = (~aParm->attrInhibit) & 0xFFFF;
    packet[4] = 0x6E01C002;
    packet[5] = aParm->color;
    packet[6] = 0x6003C003;
    cursor = packet + 7;
    *cursor++ = (u32)aParm->zBias;
    *cursor++ = (u32)sdfFaceDivSoften;
    *cursor++ = (u32)sdfFaceDivBias;
    *cursor++ = 0x04000004;
    *cursor++ = 0x14000000;
    *cursor++ = 0x6D01C000;
    *cursor++ = (u16)stripWords | ((u32)(u16)count << 16);
    *cursor++ = (primitive & 0xFFFF) | (format << 16);
    *cursor++ = (stripWords << 16) | 0x6E00C001;
    if (aParm->faceIndex == NULL) {
        memcpy(cursor, sdf3DPrimDefaultFaceIndex, stripWords * 4);
    } else {
        memcpy(cursor, aParm->faceIndex, stripWords * 4);
    }
    cursor += stripWords;
    stripWords++;
    countCode = count << 16;
    *cursor++ = countCode | stripWords | 0x6800C000;
    vector = aParm->vertex;
    i = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, *vector);
        cursor += 3;
        vector++;
        i++;
    } while (i != count);
    stripWords += count;
    vector = aParm->normal;
    if (vector != NULL) {
        *cursor++ = countCode | stripWords | 0x6800C000;
        i = 0;
        do {
            MMI_STORE_VEC3_VALUE(cursor, *vector);
            cursor += 3;
            vector++;
            i++;
        } while (i != count);
        stripWords += count;
    }
    coordinates = aParm->uv;
    if (coordinates != NULL) {
        secondCoordinates = aParm->multiUv;
        if (secondCoordinates == NULL) {
            *cursor++ = countCode | stripWords | 0x6400C000;
            memcpy(cursor, coordinates, count * 8);
            cursor += count * 2;
        } else {
            *cursor++ = countCode | stripWords | 0x6C00C000;
            i = 0;
            do {
                *cursor++ = *coordinates++;
                *cursor++ = *coordinates++;
                *cursor++ = *secondCoordinates++;
                *cursor++ = *secondCoordinates++;
                i++;
            } while (i != count);
        }
        stripWords += count;
    }
    if (aParm->vertexColor != NULL) {
        *cursor++ = countCode | stripWords | 0x6E00C000;
        memcpy(cursor, aParm->vertexColor, count * 4);
        cursor += count;
    }
    *cursor = 0x1400000C;
    while (EE_ADDR(++cursor) & 15) {
        *cursor = 0;
    }
    return packet;
}
