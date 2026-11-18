#include "common.h"
#include "draw3d.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "sdfDraw.h"
#include "sdf2D.h"
#include "effPacket.h"
#include "sdfPacketBuff.h"
#include "kernel_draw.h"
#include "dds3KernelMain.h"
#include "dds3KernelDraw.h"
#include "sdf_packet_list.h"

/* The native end message is padded to a 0x20-byte rodata block. */
extern s8 dds3CrossfadeRunFlag;
extern u32 dds3CrossfadeFrame;
extern u32 dds3CrossfadeFrameMax;
extern sdfVector4_t dds3RenderTargetVector;
extern sdfVector4_t dds3RenderEyeVector;
extern u32 dds3RenderDrawEnv;
extern u32 effPktDefaultFaceIndex[];

#include "sdk_libc.h"

void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm);

/* The render draw env's clear sprite, 0x1800 into each 0x1BE0 draw bank: both
 * XYZ corners go to the GS coordinate bias, which clears nothing. */
typedef struct
{
    u8 unk0000[0x1300];
    u8 frameMaskSource[0x500];
    u64 clearXyzA;
    u64 unk1808;
    u64 clearXyzB;
    u8 unk1818[0x3C8];
} effPacketDrawBank_t;

/* One textureless sprite as sdf2D lays it out: the colour, then two corners
 * of 0x20 bytes each. */
typedef struct
{
    s32 u;
    s32 v;
    u8 pad08[8];
    s32 x;
    s32 y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
} effPacketSpriteCorner_t;

typedef struct
{
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    effPacketSpriteCorner_t corner[2];
} effPacketSpriteVertex_t;

void effWriteGouraudTexturedQuadPacket(u64 *packet, s32 primitive,
                  s32 x0, s32 y0, f32 s0, f32 t0, s32 color0,
                  s32 x1, s32 y1, f32 s1, f32 t1, s32 color1,
                  s32 x2, s32 y2, f32 s2, f32 t2, s32 color2,
                  s32 x3, s32 y3, f32 s3, f32 t3, s32 color3, s32 depth);

void effAppendGouraudTexturedQuadPacket(sdfDrawTag_t *chain, s32 primitive,
                 s32 x0, s32 y0, f32 s0, f32 t0, s32 color0,
                 s32 x1, s32 y1, f32 s1, f32 t1, s32 color1,
                 s32 x2, s32 y2, f32 s2, f32 t2, s32 color2,
                 s32 x3, s32 y3, f32 s3, f32 t3, s32 color3, s32 depth);

void billWriteFloatTextureTrianglePacket(u64 *packet, s32 color, s32 primitive,
                  s32 x0, s32 y0, f32 uFirst, f32 vFirst,
                  s32 x1, s32 y1, f32 uSecond, f32 vSecond,
                  s32 x2, s32 y2, f32 uThird, f32 vThird, s32 depth);

void effAppendTexturedTrianglePacket(sdfDrawTag_t *chain, s32 color, s32 primitive,
                 s32 x0, s32 y0, f32 u0, f32 v0,
                 s32 x1, s32 y1, f32 u1, f32 v1,
                 s32 x2, s32 y2, f32 u2, f32 v2, s32 depth);

sdfGrPacketHeader_t *effPkt2DAllocTriStrip(s32 num, s32 prim) {
    sdfGrPacketHeader_t *packet = (sdfGrPacketHeader_t *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(9, num));

    sdf2DMakePrimPacketHeader(packet, prim | 0x54, 9, 0x525252521, num);
    return packet;
}

void effWriteGouraudTexturedQuadPacket(u64 *packet, s32 primitive,
                  s32 x0, s32 y0, f32 s0, f32 t0, s32 color0,
                  s32 x1, s32 y1, f32 s1, f32 t1, s32 color1,
                  s32 x2, s32 y2, f32 s2, f32 t2, s32 color2,
                  s32 x3, s32 y3, f32 s3, f32 t3, s32 color3, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0xE400000000008001ULL;
    packet[1] = 0xF5125125125120ULL;
    packet[2] = (u32)(primitive | 0x1C);
    ((f32 *)packet)[6] = s0;
    ((f32 *)packet)[7] = t0;
    packet[4] = (u32)color0 | ((u64)0xFE00 << 46);
    packet[5] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    ((f32 *)packet)[12] = s1;
    ((f32 *)packet)[13] = t1;
    packet[7] = (u32)color1 | ((u64)0xFE00 << 46);
    packet[8] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    ((f32 *)packet)[18] = s2;
    ((f32 *)packet)[19] = t2;
    packet[10] = (u32)color2 | ((u64)0xFE00 << 46);
    packet[11] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    ((f32 *)packet)[24] = s3;
    ((f32 *)packet)[25] = t3;
    packet[13] = (u32)color3 | ((u64)0xFE00 << 46);
    packet[14] = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void effAppendGouraudTexturedQuadPacket(sdfDrawTag_t *chain, s32 primitive,
                 s32 x0, s32 y0, f32 s0, f32 t0, s32 color0,
                 s32 x1, s32 y1, f32 s1, f32 t1, s32 color1,
                 s32 x2, s32 y2, f32 s2, f32 t2, s32 color2,
                 s32 x3, s32 y3, f32 s3, f32 t3, s32 color3, s32 depth) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x90);

    packet[0] = 0x20000008;
    packet[1] = 0x5000000810000000ULL;
    effWriteGouraudTexturedQuadPacket(packet + 2, primitive, x0, y0, s0, t0, color0,
                 x1, y1, s1, t1, color1, x2, y2, s2, t2, color2,
                 x3, y3, s3, t3, color3, depth);
    sdfDrawTagLink(chain, (u32)packet);
}

void billWriteFloatTextureTrianglePacket(u64 *packet, s32 color, s32 primitive,
                  s32 x0, s32 y0, f32 uFirst, f32 vFirst,
                  s32 x1, s32 y1, f32 uSecond, f32 vSecond,
                  s32 x2, s32 y2, f32 uThird, f32 vThird, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0x8400000000008001ULL;
    packet[1] = 0x52525210;
    packet[2] = (u32)(primitive | 0x14);
    packet[3] = (u32)color | ((u64)0xFE00 << 46);
    ((f32 *)packet)[8] = uFirst;
    ((f32 *)packet)[9] = vFirst;
    packet[5] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    ((f32 *)packet)[12] = uSecond;
    ((f32 *)packet)[13] = vSecond;
    packet[7] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    ((f32 *)packet)[16] = uThird;
    ((f32 *)packet)[17] = vThird;
    packet[9] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void effAppendTexturedTrianglePacket(sdfDrawTag_t *chain, s32 color, s32 primitive,
                 s32 x0, s32 y0, f32 u0, f32 v0,
                 s32 x1, s32 y1, f32 u1, f32 v1,
                 s32 x2, s32 y2, f32 u2, f32 v2, s32 depth) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x60);

    packet[0] = 0x20000005;
    packet[1] = 0x5000000510000000ULL;
    billWriteFloatTextureTrianglePacket(packet + 2, color, primitive, x0, y0, u0, v0,
                 x1, y1, u1, v1, x2, y2, u2, v2, depth);
    sdfDrawTagLink(chain, (u32)packet);
}

u64 *effPkt2DAllocF4(u32 prim) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x80);

    packet[0] = 7;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0xFF515151510ULL;
    packet[4] = prim | 0x14C;
    packet[15] = 0;
    packet[14] = 0;
    packet[13] = 0;
    return packet;
}

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
        allocate = (void *(*)(s32))sdfGrAllocPacketBuff;
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

void dds3CrossfadeStart(s32 frame)
{
    s32 i;

    if (dds3CrossfadeRunFlag != 0) {
        dds3CrossfadeEnd();
    }
    if (frame == 0) {
        return;
    }
    dds3RenderTargetVector.x = 0.0f;
    dds3RenderTargetVector.y = 0.0f;
    dds3RenderTargetVector.z = 0.0f;
    dds3RenderEyeVector.x = 0.0f;
    dds3RenderEyeVector.y = 0.0f;
    dds3RenderEyeVector.z = 0.0f;
    dds3RenderDrawEnv = 0x80808080;
    dds3AllocRenderTexture(0x100, 0xE0, 0.0f);
    dds3RenderTextureON();
    for (i = 0; i != 2; i++) {
        dds3DrawEnv[i].renderEnv.clear.xyzA.data = 0x80008000;
        dds3DrawEnv[i].renderEnv.clear.xyzB.data = 0x80008000;
    }
    dds3CrossfadeFrameMax = frame;
    dds3CrossfadeRunFlag = 1;
    dds3CrossfadeFrame = 0;
    printf("dds3CrossfadeStart\n");
}

void dds3UpdateCrossfade(void) {
    sdfDrawTag_t *list;
    sdfGrRefPacketHeader_t *reference;
    u64 *texturePacket;
    u64 *blendPacket;
    sdfGrPacketHeader_t *sprite;
    effPacketSpriteVertex_t *vertex;
    u32 alpha;

    if (dds3CrossfadeRunFlag == 0) {
        return;
    }
    if (dds3CrossfadeFrame == dds3CrossfadeFrameMax) {
        dds3CrossfadeEnd();
        return;
    }
    if (dds3CrossfadeFrame == 0) {
        list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
        sdfDrawTagInit(list);
        reference = EE_ADDR_AS(sdfGrRefPacketHeader_t *, sdfGrAllocPacketBuff(0x20));
        sdfGrLinkDispFrameTex(list, EE_ADDR(&dds3DrawEnv[dds3GetFrameID()].stdEnv), reference);
        texturePacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
        texturePacket[0] = 3;
        texturePacket[1] = 0x5000000310000000ULL;
        texturePacket[2] = 0x1000000000008002ULL;
        texturePacket[3] = 0xE;
        texturePacket[4] = 0x8000000080ULL;
        texturePacket[5] = 0x3B;
        texturePacket[6] = 0;
        texturePacket[7] = 0x3F;
        sdfDrawTagLink(list, EE_ADDR(texturePacket));
        blendPacket = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
        blendPacket[0] = 3;
        blendPacket[1] = 0x5000000310000000ULL;
        blendPacket[2] = 0x1000000000008002ULL;
        blendPacket[3] = 0xE;
        blendPacket[4] = 0x31001;
        blendPacket[5] = 0x47;
        blendPacket[6] = 0x44;
        blendPacket[7] = 0x42;
        sdfDrawTagLink(list, EE_ADDR(blendPacket));
        sprite = sdf2DAllocSprite(1);
        vertex = sdf2DGetPrimPtr(sprite);
        vertex->r = 0x80;
        vertex->g = 0x80;
        vertex->b = 0x80;
        vertex->a = 0x80;
        vertex->corner[0].u = 0;
        vertex->corner[0].v = 0;
        vertex->corner[0].x = 0x77F8;
        vertex->corner[0].y = 0x78F8;
        vertex->corner[0].mask = 0xFF0000;
        vertex->corner[0].flag = 0;
        vertex->corner[1].u = 0x2000;
        vertex->corner[1].v = 0xE00;
        vertex->corner[1].x = 0x87F8;
        vertex->corner[1].y = 0x86F8;
        vertex->corner[1].mask = 0xFF0000;
        vertex->corner[1].flag = 0;
        sdfDrawTagLink(list, EE_ADDR(sprite));
        dds3UpdateCrossfadeData.regFunc(&dds3UpdateCrossfadeData, list);
    }
    dds3CrossfadeFrame++;
    alpha = (0x80 - (u32)((f32)dds3CrossfadeFrame * 128.0f / (f32)dds3CrossfadeFrameMax)) & 0xFF;
    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(list);
    sdf2DLinkTexPacket(list, dds3GetRenderTexture(), 0);
    sprite = sdf2DAllocSprite(1);
    vertex = sdf2DGetPrimPtr(sprite);
    vertex->r = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->a = alpha;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x7000;
    vertex->corner[0].y = 0x7900;
    vertex->corner[0].mask = 0xFF0000;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x1000;
    vertex->corner[1].v = 0xE00;
    vertex->corner[1].x = 0x9000;
    vertex->corner[1].y = 0x8700;
    vertex->corner[1].mask = 0xFF0000;
    vertex->corner[1].flag = 0;
    sdfDrawTagLink(list, EE_ADDR(sprite));
    dds3UpdateCrossfadeData2.regFunc(&dds3UpdateCrossfadeData2, list);
}
INCLUDE_SDATA(const s32, "effect/effPacket", dds3CrossfadeRunFlag);

INCLUDE_SDATA(const s32, "effect/effPacket", dds3CrossfadeFrame);

INCLUDE_SDATA(const s32, "effect/effPacket", dds3CrossfadeFrameMax);

INCLUDE_SDATA(const s32, "effect/effPacket", effFieldColorFlags);

