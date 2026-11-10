#include "common.h"

/* Declarations from smt3-decomp src/smt3/effect/effPCPBezier.c */
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
/* End of declarations from src/smt3/effect/effPCPBezier.c */

effPCPBezierBill_tag *effPCPBezierBillCreate(const effPCPBezierBillParam_t *pParam, effBezierBED_t *pBEDData)
{
    u32 count = pParam->number;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(effPCPBezierBillPart_tag) + sizeof(effPCPBezierBill_tag));
    effPCPBezierBill_tag *work = (effPCPBezierBill_tag *)sdfMemLock(allocation);
    effPCPBezierBillPart_tag *slot = (effPCPBezierBillPart_tag *)(work + 1);
    effBezierPlacement_t place;
    u32 palette[4];
    s32 historyLength, subdivisions;
    u32 i;

    work->param = *pParam;
    work->hMem = allocation;
    work->original = 1;
    work->pPart = slot;
    work->abgr = 0x80808080;
    work->hBED = effEveBEDCreate(pBEDData);
    historyLength = work->param.historyLength;
    if (historyLength == 0) {
        work->param.historyLength = 1;
        historyLength = 1;
    }
    subdivisions = work->param.subdivisions;
    if (subdivisions <= 0) {
        work->param.subdivisions = 1;
        subdivisions = 1;
    }
    if (work->param.startDelayRange <= 0) {
        work->param.startDelayRange = 1;
    }
    palette[0] = work->param.palette[0];
    palette[1] = work->param.palette[1];
    palette[2] = work->param.palette[2];
    palette[3] = work->param.palette[3];
    place.payload[0] = 0;
    place.payload[1] = 0;
    place.payload[2] = 0;
    place.payload[3] = 0;
    place.payload[4] = 0;
    place.payload[5] = 0;
    place.payload[6] = 0;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    place.color = 0x80808080;
    for (i = 0; i < count; i++, slot++) {
        slot->hBztp = dds3BezierTrackPolySetVertex(historyLength, subdivisions);
        dds3BezierTrackPolyCreate(slot->hBztp, palette);
        slot->frame = 0;
        slot->alpha = 1.0f;
        slot->Bezier7P.pointIndex = 0;
        slot->Bezier7P.t = 0;
        slot->Bezier7P.parameterStep = 0;
        slot->hBEDCut = effEveBEDCutCreate(work->hBED, 2, &place);
    }
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPBezier", func_00222800);

effPCPBezierBill_tag *effPCPBezierBillCopy(effPCPBezierBill_tag *src) {
    u32 count = src->param.number;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(effPCPBezierBillPart_tag) + sizeof(effPCPBezierBill_tag));
    effPCPBezierBill_tag *work = (effPCPBezierBill_tag *)sdfMemLock(allocation);
    effPCPBezierBillPart_tag *slot = (effPCPBezierBillPart_tag *)(work + 1);
    effBezierPlacement_t place;
    u32 palette[4];
    s32 historyLength, subdivisions;
    u32 i;

    work->param = src->param;
    work->original = 0;
    work->hMem = allocation;
    work->hBED = src->hBED;
    work->pPart = slot;
    work->abgr = 0x80808080;
    historyLength = work->param.historyLength;
    if (historyLength == 0) {
        work->param.historyLength = 1;
        historyLength = 1;
    }
    subdivisions = work->param.subdivisions;
    if (subdivisions <= 0) {
        work->param.subdivisions = 1;
        subdivisions = 1;
    }
    if (work->param.startDelayRange <= 0) {
        work->param.startDelayRange = 1;
    }
    palette[0] = work->param.palette[0];
    palette[1] = work->param.palette[1];
    palette[2] = work->param.palette[2];
    palette[3] = work->param.palette[3];
    place.payload[0] = 0;
    place.payload[1] = 0;
    place.payload[2] = 0;
    place.payload[3] = 0;
    place.payload[4] = 0;
    place.payload[5] = 0;
    place.payload[6] = 0;
    place.payload[7] = 1.0f;
    place.payload[8] = 100.0f;
    place.payload[9] = 100.0f;
    place.payload[10] = 1.0f;
    place.color = 0x80808080;
    for (i = 0; i < count; i++, slot++) {
        slot->hBztp = dds3BezierTrackPolySetVertex(historyLength, subdivisions);
        dds3BezierTrackPolyCreate(slot->hBztp, palette);
        slot->frame = 0;
        slot->alpha = 1.0f;
        slot->Bezier7P.pointIndex = 0;
        slot->Bezier7P.t = 0;
        slot->Bezier7P.parameterStep = 0;
        slot->hBEDCut = effEveBEDCutCreate(work->hBED, 2, &place);
    }
    return work;
}

void effPCPBezierBillDelete(effPCPBezierBill_tag *hData) {
    u32 i = 0;
    u32 count = hData->param.number;
    effPCPBezierBillPart_tag *slot = hData->pPart;

    if (count != 0) {
        do {
            i++;
            dds3BezierTrackPolyDelete(slot->hBztp);
            effEveBEDCutDelete(slot->hBEDCut);
            slot++;
        } while (i < count);
    }
    if (hData->original != 0) {
        effEveBEDDelete(hData->hBED);
    }
    sdfMemFreeDirect(hData->hMem);
}

void func_00222B48(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPBezier", func_00222B50);

INCLUDE_ASM(const s32, "effect/effPCPBezier", func_00222B60);

dds3BezierTrackPoly_tag *dds3BezierTrackPolySetVertex(s32 hPoly, s32 pVertex) {
    s32 count = hPoly * pVertex * 3 + 6;
    u32 colorBytes = count * sizeof(u32);
    u32 bufferBytes = count * (sizeof(u128) + sizeof(u32)) + 8 * (sizeof(u128) + sizeof(u32));
    sdfMemHandle_t *allocation = sdfMemAllocDirect(bufferBytes + sizeof(dds3BezierTrackPoly_tag));
    u8 *storage = (u8 *)sdfMemLock(allocation);
    dds3BezierTrackPoly_tag *history = (dds3BezierTrackPoly_tag *)(storage + bufferBytes);
    sdfMat_t *asset;

    history->pVertex = (u128 *)storage;
    storage += count * sizeof(u128);
    history->pHeadVertex = (u128 *)storage;
    storage += 8 * sizeof(u128);
    history->pVC = (u32 *)storage;
    storage += colorBytes;
    history->pHeadVC = (u32 *)storage;
    history->blend = 2;
    history->point = count;
    history->current = 3;
    history->split = pVertex;
    history->abgr = 0x80808080;
    history->hMem = allocation;
    history->count = 0;
    memcpy(history->pHeadVertex, dds3BezierTrackPolyInitVertex, sizeof(dds3BezierTrackPolyInitVertex));
    asset = sdfMatNew();
    history->pMat = asset;
    sdfMatSetConstant(asset, 1.0f);
    memset(&dds3BezierVertexCount, 0, 0x2C);
    dds3BezierVertexCount.flags = 0x4000;
    dds3BezierVertexCount.primitiveIndices = dds3BezierVertexCountIndices;
    memset(&dds3BezierTrackPolyVertex, 0, 0x2C);
    dds3BezierTrackPolyVertex.flags = 0x4000;
    dds3BezierTrackPolyVertex.primitiveIndices = dds3BezierTrackPolyVertexIndices;
    dds3BezierTrackPolyVertex.primitiveCount = 6;
    dds3BezierTrackPolyVertex.pointCount = 8;
    return history;
}

INCLUDE_ASM(const s32, "effect/effPCPBezier", func_00222D68);

void dds3BezierTrackPolyCreate(dds3BezierTrackPoly_tag *face, u32 *split) {
    f32 t = 0.0f;
    u32 count = face->point / 3;
    u32 alphaCount = count >> 1;
    f32 step = 1.0f / count;
    f32 alpha = 0.0f;
    f32 alphaStep = 1.0f / alphaCount;
    u32 *colors = face->pVC;
    u32 innerStart = split[0] & 0xFFFFFF;
    u32 innerEnd = split[2] & 0xFFFFFF;
    u32 outerStart = split[1];
    u32 outerEnd = split[3];
    u32 i = 0;

    if (count != 0) {
        do {
            u32 alphaMask = (u32)(alpha * 2147483648.0f) & 0xFF000000;

            colors[0] = effColorBlend(outerEnd, outerStart, t);
            colors[1] = effColorBlend(innerEnd, innerStart, t) | alphaMask;
            colors[2] = colors[0];
            t += step;
            if (i < alphaCount) {
                alpha += alphaStep;
            }
            i++;
            colors += 3;
        } while (i < count);
    }
}
