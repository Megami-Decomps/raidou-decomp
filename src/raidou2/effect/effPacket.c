#include "common.h"

/* Declarations from smt3-decomp src/smt3/effect/effPacket.c */
#include "common.h"

#include "draw3d.h"

#include "ps2asm.h"

#include "sdfFrame.h"

/* The native end message is padded to a 0x20-byte rodata block. */
extern s8 dds3CrossfadeRunFlag;

extern u32 dds3CrossfadeFrame;

extern u32 dds3CrossfadeFrameMax;

extern sdfGrOt_t sdfGrOtTable[];

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern void sdfGrLinkDispFrameTex(sdfDrawTag_t *aTag, addr_t aEnv, void *aPkt);

extern void sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);

extern void *sdf2DAllocSprite(s32 aNumOfSpr);

extern void *sdf2DGetPrimPtr(void *aPacketPtr);

extern u32 dds3GetFrameID(void);

extern sdfTexHandle_t *dds3GetRenderTexture(void);

extern u32 dds3RenderDrawEnv;

extern f32 dds3RenderTargetVector[];

extern f32 dds3RenderEyeVector[];

extern void dds3CrossfadeEnd(void);

extern s32 dds3AllocRenderTexture(u16 sx, u16 sy, f32 aspect);

extern s32 dds3RenderTextureON(void);

extern u32 effPktDefaultFaceIndex[];

extern s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim);

extern void *sdfGrAllocPacketBuff(s32 aSize);

extern sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *aPacketPtr, s32 aPrimReg, s32 aNumOfRegs, s64 aGifRegs, s32 aNumOfPrim);

extern void dds3RenderTextureOFF(void);

extern void dds3FreeRenderTexture(void);

extern void *memcpy(void *a, const void *b, size_t c);

#include "sdk_libc.h"

void *effPkt2DGetPrimPtr(void *pPkt);

sdfGrPacketHeader_t *effPkt2DAllocTriStrip(s32 num, s32 prim);

u64 *effPkt2DAllocF4(u32 prim);

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

extern effPacketDrawBank_t dds3DrawEnv[];

void dds3CrossfadeStart(s32 frame);

void dds3CrossfadeRun(void);

void dds3CrossfadeEnd(void);

extern s8 dds3CrossfadeRunFlag;

extern u32 dds3CrossfadeFrame;

extern u32 dds3CrossfadeFrameMax;
/* End of declarations from src/smt3/effect/effPacket.c */

/* Declarations from dds-decomp src/dds1/effect/effPacket.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_effPacketDrawBank_t 1

#define SMT3_DEFINES_effPacketSpriteCorner_t 1

#define SMT3_DEFINES_effPacketSpriteVertex_t 1

#include "dds/common.h"

#include "dds/draw3d.h"

#include "dds/ps2asm.h"

#include "dds/sdfFrame.h"

/* DDS keeps the end message as data: its rodata block has 8 more bytes than
 * the literal emits. */
extern const char dds3CrossfadeEndMessage[];

#include "dds/sdk_libc.h"

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
/* End of declarations from src/dds1/effect/effPacket.c */

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

INCLUDE_ASM(const s32, "effect/effPacket", func_001FB860);

INCLUDE_ASM(const s32, "effect/effPacket", func_001FB948);
INCLUDE_SDATA(const s32, "effect/effPacket", D_004E16BC);

INCLUDE_SDATA(const s32, "effect/effPacket", D_004E16C0);

INCLUDE_SDATA(const s32, "effect/effPacket", D_004E16C4);

INCLUDE_SDATA(const s32, "effect/effPacket", D_004E16C8);

