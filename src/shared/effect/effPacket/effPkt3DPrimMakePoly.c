#include "shared/effect/effPacket/decls.h"

void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm) {
    s32 parameterCount = aParm->faceCount;
    s32 vertexCount = aParm->pointCount;
    u32 primitive = aParm->attr | 0x48;
    u32 mode = 0x60;
    s32 stride = 3;
    s32 overhead = 16;
    s32 allocationSize;
    s32 i;
    s32 offset;
    void *packet;
    void *(*allocate)(s32);
    u32 *cursor;
    u32 *stream;
    const u128 *vector;
    u32 countCode;
    u32 *stream2;

    if (aParm->normal != NULL) {
        mode = 0xE0;
        overhead = 17;
        stride = 6;
    }
    if (aParm->uv != NULL) {
        mode |= 0x100;
        primitive |= 0x10;
        overhead++;
        stride += 2;
        if (aParm->multiUv != NULL) {
            primitive |= 0x1000;
            stride += 2;
        }
    }
    if (aParm->vertexColor != NULL) {
        mode |= 0x200;
        primitive |= 0x800;
        overhead++;
        stride++;
    }
    allocationSize = (overhead + parameterCount + stride * vertexCount) * 4;
    allocationSize = (allocationSize + 15) & ~15;
    allocate = (void *(*)(s32))aParm->allocator;
    if (allocate == NULL) {
        allocate = sdfGrAllocPacketBuff;
    }
    packet = allocate(allocationSize);

    *(u64 *)packet = (u64)(u16)((allocationSize >> 4) - 1) | 0x20000000ULL;
    ((u32 *)packet)[2] = 0x6102C000;
    ((u32 *)packet)[3] = (~aParm->attrInhibit) & 0xFFFF;
    ((u32 *)packet)[4] = 0x6E01C002;
    ((u32 *)packet)[5] = aParm->color;
    ((u32 *)packet)[6] = 0x6001C003;
    cursor = (u32 *)packet + 7;
    *cursor++ = (u32)aParm->zBias;
    *cursor++ = 0x04000004;
    *cursor++ = 0x14000000;
    *cursor++ = 0x6D01C000;
    *cursor++ = (u16)parameterCount | ((u32)(u16)vertexCount << 16);
    *cursor++ = (primitive & 0xFFFF) | (mode << 16);
    *cursor++ = 0x6E00C001 | ((u32)parameterCount << 16);

    if (aParm->faceIndex == NULL) {
        memcpy(cursor, effPktDefaultFaceIndex, parameterCount * 4);
    } else {
        memcpy(cursor, aParm->faceIndex, parameterCount * 4);
    }
    cursor += parameterCount;
    offset = parameterCount + 1;
    countCode = vertexCount << 16;
    *cursor++ = countCode | offset | 0x6800C000;
    vector = (u128 *)aParm->vertex;
    i = 0;
    do {
        MMI_STORE_VEC3_VALUE(cursor, *vector);
        i++;
        cursor += 3;
        vector++;
    } while (i != vertexCount);
    offset += vertexCount;

    vector = (u128 *)aParm->normal;
    if (vector != NULL) {
        *cursor++ = countCode | offset | 0x6800C000;
        i = 0;
        do {
            MMI_STORE_VEC3_VALUE(cursor, *vector);
            i++;
            cursor += 3;
            vector++;
        } while (i != vertexCount);
        offset += vertexCount;
    }
    stream = (u32 *)aParm->uv;
    if (stream != NULL) {
        stream2 = (u32 *)aParm->multiUv;
        if (stream2 == NULL) {
            *cursor++ = countCode | offset | 0x6400C000;
            memcpy(cursor, stream, vertexCount * 8);
            cursor += vertexCount * 2;
        } else {
            *cursor++ = countCode | offset | 0x6C00C000;
            i = 0;
            do {
                *cursor++ = *stream++;
                *cursor++ = *stream++;
                *cursor++ = *stream2++;
                *cursor++ = *stream2++;
                i++;
            } while (i != vertexCount);
        }
        offset += vertexCount;
    }
    stream = aParm->vertexColor;
    if (stream != NULL) {
        *cursor++ = countCode | offset | 0x6E00C000;
        memcpy(cursor, stream, vertexCount * 4);
        cursor += vertexCount;
    }
    *cursor = 0x1400000C;
    while (EE_ADDR(++cursor) & 15) {
        *cursor = 0;
    }
    return packet;
}
