/* Declarations of smt3-decomp src/smt3/effect/effPCPBezier.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_EFFPCPBEZIER_H
#define SHARED_EFFECT_EFFPCPBEZIER_H

#include "common.h"

#include "libsdf.h"

#include "ps2asm.h"

#include "effect.h"

#include "sdfFrame.h"

typedef struct effBezierBED_t effBezierBED_t;

typedef struct effBezierCut_t effBezierCut_t;

typedef struct effBezierPlacement_t {
    f32 payload[11];
    u32 color;
} effBezierPlacement_t;

/* One retained fragment-history header follows its point/color buffers. */
typedef struct dds3BezierTrackPoly_tag {
    u32 abgr;                              /* 0x00 */
    s32 blend;                       /* 0x04 */
    s32 point;                      /* 0x08: history point capacity */
    s32 count;                   /* 0x0C */
    s32 current;                /* 0x10: first point of the next three-point sample */
    s32 split;                       /* 0x14: retained input value */
    u128 *pVertex;                           /* 0x18 */
    u32 *pVC;                            /* 0x1C */
    sdfMat_t *pMat;               /* 0x20 */
    sdfMemHandle_t *hMem;                /* 0x24 */
    u128 *pHeadVertex;                        /* 0x28 */
    u32 *pHeadVC;                         /* 0x2C */
} dds3BezierTrackPoly_tag;

/* Copied 0x50-byte input for a group of thunder-fragment ribbons. */
typedef struct effPCPBezierBillParam_t {
    f32 origin[4];                          /* 0x00 */
    u32 mode;                               /* 0x10: retains native selector values */
    f32 spreadX;                            /* 0x14 */
    f32 spreadZ;                            /* 0x18 */
    f32 height;                             /* 0x1C */
    u32 number;                         /* 0x20 */
    s32 curveFrames;                        /* 0x24 */
    s32 startDelayRange;                    /* 0x28 */
    u32 fade_in;                              /* 0x2C */
    s32 fadeFrames;                         /* 0x30 */
    f32 width;                              /* 0x34 */
    s32 historyLength;                      /* 0x38 */
    s32 subdivisions;                       /* 0x3C */
    u32 palette[4];                         /* 0x40: inner-start, outer-start, inner-end, outer-end */
} effPCPBezierBillParam_t;

/* One 0x80-byte moving ribbon slot plus its event work. */
typedef struct effPCPBezierBillPart_tag {
    f32 pos[4];                        /* 0x00: current sampled point */
    s32 frame;                                /* 0x10: negative delay, then elapsed frames */
    f32 alpha;                          /* 0x14 */
    dds3BezierTrackPoly_tag *hBztp;/* 0x18 */
    effBezierCurve_t Bezier7P;           /* 0x1C */
    struct effBezierCut_t *hBEDCut;          /* 0x7C */
} effPCPBezierBillPart_tag;

/* Allocation header followed by groupCount inline 0x80-byte slots. */
typedef struct effPCPBezierBill_tag {
    effPCPBezierBillParam_t param;         /* 0x00 */
    effPCPBezierBillPart_tag *pPart;           /* 0x50 */
    u32 abgr;                           /* 0x54 */
    struct effBezierBED_t *hBED;           /* 0x58 */
    u8 original;                      /* 0x5C */
    u8 pad5D[3];                             /* 0x5D */
    sdfMemHandle_t *hMem;                 /* 0x60 */
} effPCPBezierBill_tag;

/* Vertex-draw descriptor used by the thunder history and end-cap packets.
 * Both title providers clear the first 0x2C bytes and keep these globals in
 * 0x30-byte slots; the final four bytes remain opaque. */
typedef struct effBezierDrawParam_t {
    s16 primitiveCount; /* 0x00 */
    s16 pointCount;     /* 0x02 */
    u16 flags;          /* 0x04 */
    u8 opaque_06[2];
    u32 color;          /* 0x08 */
    const u32 *primitiveIndices; /* 0x0C */
    u128 *points;       /* 0x10 */
    u8 opaque_14[0xC];
    u32 *colors;        /* 0x20 */
    u8 opaque_24[0xC];
} effBezierDrawParam_t;

extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern sdfMat_t *sdfMatNew(void);

extern void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);

extern void sdfMatFreeDelayed(sdfMat_t *aMat);

extern void *memcpy(void *a, const void *b, size_t c);

extern void *memset(void *a, s32 b, size_t c);

extern effBezierBED_t *effEveBEDCreate(effBezierBED_t *pData);

extern void effEveBEDDelete(effBezierBED_t *hEveBED);

extern effBezierCut_t *effEveBEDCutCreate(effBezierBED_t *hEveBED, s32 CutNum, effBezierPlacement_t *pParams);

extern void effEveBEDCutDelete(effBezierCut_t *hEveBEDCut);

extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);

extern s32 sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag, void *aAllocator);

extern void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat, void *aAllocator);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern void *effPkt3DPrimMakePoly(effBezierDrawParam_t *aParm);

extern effBezierDrawParam_t dds3BezierVertexCount, dds3BezierTrackPolyVertex;

extern const f32 dds3BezierTrackPolyInitVertex[8][4];

extern const u32 dds3BezierVertexCountIndices[28], dds3BezierTrackPolyVertexIndices[8];

extern sdfGrOt_t *dds3BezierTrackPolyParam[];

dds3BezierTrackPoly_tag *dds3BezierTrackPolySetVertex(s32 hPoly, s32 pVertex);

void dds3BezierTrackPolyCreate(dds3BezierTrackPoly_tag *face, u32 *split);

void dds3BezierTrackPolyDelete(dds3BezierTrackPoly_tag *hPoly);

effPCPBezierBill_tag *effPCPBezierBillCreate(const effPCPBezierBillParam_t *pParam, effBezierBED_t *pBEDData);

effPCPBezierBill_tag *effPCPBezierBillCreatePCF(void *paramTable);

effPCPBezierBill_tag *effPCPBezierBillCopy(effPCPBezierBill_tag *src);

void effPCPBezierBillDelete(effPCPBezierBill_tag *hData);

extern effRandState_t dds3EffectBase[];

extern sdfVector4_t aViewEye, aViewTarget;

extern u32 effBTLSkillCheckRun(void);

extern s32 effBTLSkillGetTargetFormation(s32 number);

extern s32 effBTLSkillGetInvokeFormation(void);

extern void effBTLModelGetSphereCenter(s32 pFormation);

extern f32 dds3GetRandFloatA(effRandState_t *name);

extern u32 dds3GetRandA(effRandState_t *name);

extern void SetRotateMatrixAxis(f32 aAngle, const f32 *aAxis);

extern s32 effPCPBezierEval(effBezierCurve_t *curve, f32 *out);

extern u32 effMagaCylinderCalcColor(u32 color, u32 scale);

extern f32 sdfAtan2(f32 aY, f32 aX);

extern void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

extern void effEveBEDCutSetParams(effBezierCut_t *hEveBEDCut, const effBezierPlacement_t *pParams);

extern void effEveBEDCutDraw(effBezierCut_t *hEveBEDCut);

void dds3BezierTrackPolyReset(dds3BezierTrackPoly_tag *hPoly);

void dds3BezierTrackPolySetVC(dds3BezierTrackPoly_tag *hPoly, u128 *pABGR);

void dds3BezierTrackPolyDraw(dds3BezierTrackPoly_tag *hPoly);

/* The ribbon and end-cap vectors are full 16-byte-aligned VU objects.
 * This donor-backed SDK store/readback pipeline is audited specifically here;
 * see docs/bezier-matching.md before changing its memory-access boundaries. */
typedef f32 effBezierVector_t[4] __attribute__((aligned(16)));

void effPCPBezierBillDraw(effPCPBezierBill_tag *group);

void effPCPBezierBillSetPosition(void *dst, void *src);

void effPCPBezierBillSetColor(effPCPBezierBill_tag *work, u32 color);

dds3BezierTrackPoly_tag *dds3BezierTrackPolySetVertex(s32 hPoly, s32 pVertex);

void dds3BezierTrackPolyDelete(dds3BezierTrackPoly_tag *hPoly);

void dds3BezierTrackPolyReset(dds3BezierTrackPoly_tag *hPoly);

void dds3BezierTrackPolyCreate(dds3BezierTrackPoly_tag *face, u32 *split);

void dds3BezierTrackPolySetVC(dds3BezierTrackPoly_tag *hPoly, u128 *pABGR);

void dds3BezierTrackPolyDraw(dds3BezierTrackPoly_tag *hPoly);

#endif
