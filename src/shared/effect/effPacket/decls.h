/* Declarations of smt3-decomp src/smt3/effect/effPacket.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_EFFPACKET_H
#define SHARED_EFFECT_EFFPACKET_H

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

extern f32 D_002F5C20[];

extern f32 D_002F5C30[];

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

#endif
