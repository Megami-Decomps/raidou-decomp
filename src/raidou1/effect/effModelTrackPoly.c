#include "common.h"

/* Declarations from smt3-decomp src/smt3/effect/effModelTrackPoly.c */
#include "common.h"

#include "sdk_libgcc.h"

#include "model.h"

#include "motion.h"

#include "ps2asm.h"

#include "draw3d.h"

#include "draw.h"

#include "effect.h"

#include "sdfFrame.h"

typedef struct
{
    u32 blend;
    u32 abgr;
    s32 point;
    s32 count;
    s32 current;
    s32 split;
    u128 *pVertex;
    u32 *pVC;
    sdfMat_t *pMat;
    sdfMemHandle_t *hMem;
} dds3HermitianTrackPoly_tag;

typedef struct
{
    mdlObj_t *hModel;
    s32 mark0;
    s32 mark1;
    f32 frame_s;
    f32 frame_e;
    u32 record;
    s32 face;
    u32 split;
    u32 blend;
    u32 gradientColors[4];
} Eff_ModelTrackPolyParam_t;

typedef struct
{
    Eff_ModelTrackPolyParam_t params;
    u32 updateCount;
    dds3HermitianTrackPoly_tag *trackPoly;
} effModelTrackPoly_t;

typedef struct
{
    effModelTrackPoly_t **hPolyList;
    u32 number;
    sdfMemHandle_t *hMem;
} effMTPolyListHandle_t;

typedef struct
{
    mdlObj_t *hModel;
    s32 mark0;
    s32 mark1;
    u32 startFrame;
    s32 point;
    s32 historyLength;
    f32 step;
    u32 blend;
    u32 abgr;
    u32 sub_alpha_in;
} effSwordTrackParams_t;

/* Sword trails have their history pointer at +0x30 instead of +0x38. */
typedef struct
{
    effSwordTrackParams_t param_dante;
    u32 frame;
    u32 otIndex;
    dds3HermitianTrackPoly_tag *hPoly;
} Eff_ModelTrackPoly_tag;

extern s32 mdlGetMotionNum(mdlObj_t *model, s32 group);

extern s32 mdlGetMinorNum(mdlObj_t *model);

extern s32 mdlGetMajorNum(mdlObj_t *model);

extern void *memset(void *destination, int value, size_t size);

void pMeshDelete(dds3HermitianTrackPoly_tag *hPoly);

void dds3HermitianTrackPolySetVC(dds3HermitianTrackPoly_tag *hPoly, u32 *colors);

void dds3HermitianTrackPolySetBlend(dds3HermitianTrackPoly_tag *hPoly, u32 blend);

extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void sdfChipFreeDirect(void *aPtr);

extern void *sdfChipAllocDirect(u32 aSize);

extern sdfGrOt_t sdfGrOtTable[];

extern sdfGrOt_t effPolyOt;

extern u32 dds3GetFrameID(void);

extern void sdfGrSetFrameMaskRGB(const u8 *aEnv, void *aHdr, s32 aContext);

extern void sdfGrSetFrameMaskReset(const u8 *aEnv, void *aHdr, s32 aContext);

extern void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, addr_t aPacket);

/* dds3DrawBank_t stride; only the held frame texture block at 0x1300 is used here. */
typedef struct
{
    u8 unk0000[0x1300];
    u8 frameTex[0x8E0];
} effDrawBank_t;

extern effDrawBank_t dds3DrawEnv[];

extern sdfMat_t *sdfMatNew(void);

extern void sdfMatFreeDelayed(sdfMat_t *aMat);

extern void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);

extern s32 sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag, s32 aAllocator);

extern void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat, s32 aAllocator);

extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm);

extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);

extern sdf3DPrimPolyParm_t effModelTrackPolyPrimParm;

extern u32 effModelTrackPolyFaceIndex[];

extern sdfGrOt_t *effModelTrackPolyBlendOt[];

void dds3HermitianTrackPolySetVertex(dds3HermitianTrackPoly_tag *hPoly, const u128 *pVertex);

void dds3HermitianTrackPolyReset(dds3HermitianTrackPoly_tag *hPoly);

void dds3HermitianTrackPolyDraw(dds3HermitianTrackPoly_tag *hPoly);

void Eff_ModelTrackPolyGetMarkPosition(effModelTrackPoly_t *hMTPoly);

void effSwordTrackDraw(Eff_ModelTrackPoly_tag *hSwordTrack);

void effSwordTrackRecord(Eff_ModelTrackPoly_tag *hSwordTrack);

void effSwordTrackPolygonDraw(Eff_ModelTrackPoly_tag *hSwordTrack);

void effSwordTrackPolygon2DDraw(Eff_ModelTrackPoly_tag *hSwordTrack);

extern s32 sdfModelGetMarkPos(void *aRoot, s32 aId);

s32 Eff_ModelTrackPoly_DANTE_Check(mdlObj_t *hModel);

Eff_ModelTrackPoly_tag *effSwordTrackCreate(effSwordTrackParams_t *pParams);

void effSwordTrackDelete(Eff_ModelTrackPoly_tag *hSwordTrack);

void effSwordTrackReset(Eff_ModelTrackPoly_tag *hSwordTrack);

/* Called above its definition, where ee-gcc assumed int. */
dds3HermitianTrackPoly_tag *dds3HermitianTrackPolyCreate(s32 face, s32 split);

effModelTrackPoly_t *Eff_ModelTrackPolyCreate(Eff_ModelTrackPolyParam_t *pParam);

void Eff_ModelTrackPolyDelete(effModelTrackPoly_t *work);

void Eff_ModelTrackPolyReset(effModelTrackPoly_t *hMTPoly);

void Eff_ModelTrackPolyGetMarkPosition(effModelTrackPoly_t *hMTPoly);

void Eff_ModelTrackPolyVertexSet(effModelTrackPoly_t *hMTPoly, const u128 *pVertex);

void Eff_ModelTrackPolyColorSet(effModelTrackPoly_t *hMTPoly, u32 abgr);

void Eff_ModelTrackPolyDrawOnly(effModelTrackPoly_t *hMTPoly);

void Eff_ModelTrackPolyDraw(effModelTrackPoly_t *hMTPoly);

effMTPolyListHandle_t *Eff_ModelTrackPolyListCreate(Eff_ModelTrackPolyParam_t *pParam, u32 number);

void Eff_ModelTrackPolyListDelete(effMTPolyListHandle_t *list);

void Eff_ModelTrackPolyListVertexSet(effMTPolyListHandle_t *hMTPolyList, s32 index, const u128 *pVertex);

void Eff_ModelTrackPolyListReset(effMTPolyListHandle_t *hMTPolyList, s32 index);

void Eff_ModelTrackPolyListColorSet(effMTPolyListHandle_t *hMTPolyList, s32 index, u32 abgr);

void Eff_ModelTrackPolyListDrawOnly(effMTPolyListHandle_t *hMTPolyList);

/* vu0 routine: Catmull-Rom point between p[1] and p[2], left in vf10. */
void Eff_HermitianSpline(sdfVector4_t *p, f32 t);

dds3HermitianTrackPoly_tag *dds3HermitianTrackPolyCreate(s32 face, s32 split);

void pMeshDelete(dds3HermitianTrackPoly_tag *hPoly);

void dds3HermitianTrackPolyReset(dds3HermitianTrackPoly_tag *hPoly);

void dds3HermitianTrackPolySetBlend(dds3HermitianTrackPoly_tag *hPoly, u32 blend);

void dds3HermitianTrackPolySetVC(dds3HermitianTrackPoly_tag *hPoly, u32 *pABGR);

void dds3HermitianTrackPolyGetVertex(dds3HermitianTrackPoly_tag *hPoly, void *pVertex0, void *pVertex1, s32 frame);

void dds3HermitianTrackPolySetCurrent(dds3HermitianTrackPoly_tag *hPoly, s32 frame);

void dds3HermitianTrackPolySetVertexSub(dds3HermitianTrackPoly_tag *hPoly, const u128 *pVertex);

/* Resample the newest history span: cubic interpolation for established
 * tracks, linear extension while the history is still short. */
void dds3HermitianTrackPolySetVertex(dds3HermitianTrackPoly_tag *hPoly, const u128 *pVertex);

/* Submit up to two wrapped ring runs in 18-point strips, overlapping by two. */
void dds3HermitianTrackPolyDraw(dds3HermitianTrackPoly_tag *hPoly);

s32 Eff_ModelTrackPoly_DANTE_Check(mdlObj_t *hModel);

Eff_ModelTrackPoly_tag *effSwordTrackCreate(effSwordTrackParams_t *pParams);

void effSwordTrackDelete(Eff_ModelTrackPoly_tag *hSwordTrack);

void effSwordTrackRecord(Eff_ModelTrackPoly_tag *hSwordTrack);

void effSwordTrackPolygonDraw(Eff_ModelTrackPoly_tag *hSwordTrack);

void effSwordTrackDraw(Eff_ModelTrackPoly_tag *hSwordTrack);

void effSwordTrackReset(Eff_ModelTrackPoly_tag *hSwordTrack);
/* End of declarations from src/smt3/effect/effModelTrackPoly.c */

void Eff_ModelTrackPolyGetMarkPosition(effModelTrackPoly_t *hMTPoly)
{
    sdfVector4_t pair[2];
    mdlObj_t *object = hMTPoly->params.hModel;

    sdfModelGetMarkPos(object->inner, hMTPoly->params.mark0);
    SDF_VU0_STOREVECTOR_MEMOPS(pair[0]);
    sdfModelGetMarkPos(object->inner, hMTPoly->params.mark1);
    SDF_VU0_STOREVECTOR(&pair[1]);
    dds3HermitianTrackPolySetVertex(hMTPoly->trackPoly, (const u128 *)pair);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001F69A0);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001F69B8);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001F69C8);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001F69E0);

effMTPolyListHandle_t *Eff_ModelTrackPolyListCreate(Eff_ModelTrackPolyParam_t *pParam, u32 number) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(number * sizeof(effModelTrackPoly_t *) + sizeof(effMTPolyListHandle_t));
    effMTPolyListHandle_t *list = (effMTPolyListHandle_t *)sdfMemLock(handle);
    u32 workIndex;

    list->hMem = handle;
    list->hPolyList = (effModelTrackPoly_t **)(list + 1);
    list->number = number;
    for (workIndex = 0; workIndex < number; workIndex++) {
        list->hPolyList[workIndex] = Eff_ModelTrackPolyCreate(pParam);
    }
    return list;
}

void Eff_ModelTrackPolyListDelete(effMTPolyListHandle_t *list) {
    u32 count = list->number;
    u32 workIndex = 0;

    while (workIndex < count) {
        Eff_ModelTrackPolyDelete(list->hPolyList[workIndex]);
        workIndex++;
    }
    sdfMemFreeDirect(list->hMem);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001F6BB0);

void Eff_ModelTrackPolyListReset(effMTPolyListHandle_t *hMTPolyList, s32 index) {
    Eff_ModelTrackPolyReset(hMTPolyList->hPolyList[index]);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001F6C00);

void Eff_ModelTrackPolyListDrawOnly(effMTPolyListHandle_t *hMTPolyList) {
    u32 count = hMTPolyList->number;
    u32 workIndex = 0;

    while (workIndex < count) {
        Eff_ModelTrackPolyDrawOnly(hMTPolyList->hPolyList[workIndex]);
        workIndex++;
    }
}

/* vu0 routine: Catmull-Rom point between p[1] and p[2], left in vf10. */
void Eff_HermitianSpline(sdfVector4_t *p, f32 t)
{
    sdfVector4_t tan[2];
    sdfVector4_t half;
    sdfVector4_t h[4];
    f32 t2 = t * t;
    f32 t3 = t2 * t;

    half.x = 0.5f;
    half.y = 0.5f;
    half.z = 0.5f;
    SDF_VU0_LOADVECTOR(&p[1]);
    SDF_VU0_LOADVECTOR_B(&p[0]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&p[2]);
    SDF_VU0_LOADVECTOR_B(&p[1]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B(&half);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&tan[0]);
    SDF_VU0_LOADVECTOR(&p[2]);
    SDF_VU0_LOADVECTOR_B(&p[1]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&p[3]);
    SDF_VU0_LOADVECTOR_B(&p[2]);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B(&half);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&tan[1]);
    h[0].x = 2.0f * t3 - 3.0f * t2 + 1.0f;
    h[0].y = 2.0f * t3 - 3.0f * t2 + 1.0f;
    h[0].z = 2.0f * t3 - 3.0f * t2 + 1.0f;
    h[1].x = t3 - 2.0f * t2 + t;
    h[1].y = t3 - 2.0f * t2 + t;
    h[1].z = t3 - 2.0f * t2 + t;
    h[2].x = t3 - t2;
    h[2].y = t3 - t2;
    h[2].z = t3 - t2;
    h[3].x = -2.0f * t3 + 3.0f * t2;
    h[3].y = -2.0f * t3 + 3.0f * t2;
    h[3].z = -2.0f * t3 + 3.0f * t2;
    SDF_VU0_LOADVECTOR(&h[0]);
    SDF_VU0_LOADVECTOR_B(&p[1]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&h[1]);
    SDF_VU0_LOADVECTOR_B(&tan[0]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_C_A();
    SDF_VU0_LOADVECTOR(&h[2]);
    SDF_VU0_LOADVECTOR_B(&tan[1]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_C_A();
    SDF_VU0_LOADVECTOR(&h[3]);
    SDF_VU0_LOADVECTOR_B(&p[2]);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
}

dds3HermitianTrackPoly_tag *dds3HermitianTrackPolyCreate(s32 face, s32 split) {
    s32 count = face * split * 2 + 4;
    s32 bytes = count * (sizeof(u128) + sizeof(u32));
    sdfMemHandle_t *handle = sdfMemAllocDirect(bytes + sizeof(dds3HermitianTrackPoly_tag));
    u8 *cursor = (u8 *)sdfMemLock(handle);
    dds3HermitianTrackPoly_tag *data = (dds3HermitianTrackPoly_tag *)(cursor + bytes);

    data->pVertex = (u128 *)cursor;
    cursor += count * sizeof(u128);
    data->blend = 2;
    data->abgr = 0x80808080;
    data->point = count;
    data->count = 0;
    data->current = 2;
    data->split = split;
    data->hMem = handle;
    data->pVC = (u32 *)cursor;
    data->pMat = sdfMatNew();
    sdfMatSetConstant((sdfMat_t *)data->pMat, 1.0f);
    memset(&effModelTrackPolyPrimParm, 0, sizeof(effModelTrackPolyPrimParm));
    effModelTrackPolyPrimParm.attr = 0x4000;
    effModelTrackPolyPrimParm.faceIndex = effModelTrackPolyFaceIndex;
    return data;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001F6EE8);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001F6F18);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001F6F38);

void dds3HermitianTrackPolySetVC(dds3HermitianTrackPoly_tag *hPoly, u32 *pABGR) {
    f32 t = 0.0f;
    u32 count = hPoly->point >> 1;
    f32 step = 1.0f / count;
    u32 i = 0;
    u32 *colors = hPoly->pVC;

    if (count != 0) {
        do {
            colors[0] = effColorBlend(pABGR[2], pABGR[0], t);
            colors[1] = effColorBlend(pABGR[3], pABGR[1], t);
            t += step;
            colors += 2;
            i++;
        } while (i < count);
    }
}

void dds3HermitianTrackPolyGetVertex(dds3HermitianTrackPoly_tag *hPoly, void *pVertex0, void *pVertex1, s32 frame)
{
    s32 index = hPoly->current - 2 * (hPoly->split * (frame - 1) + frame);
    u8 *vertex;

    if (index < 2) {
        index += hPoly->point - 2;
    }
    vertex = (u8 *)hPoly->pVertex + index * 16;
    SDF_VU0_COPYVECTOR(pVertex0, vertex);
    SDF_VU0_COPYVECTOR(pVertex1, vertex + 16);
}

void dds3HermitianTrackPolySetCurrent(dds3HermitianTrackPoly_tag *hPoly, s32 frame) {
    s32 pos = hPoly->current + ((hPoly->split - 1) * (frame - 1) + frame) * -2;

    if (pos < 2) {
        pos += hPoly->point - 2;
    }
    hPoly->current = pos;
}

void dds3HermitianTrackPolySetVertexSub(dds3HermitianTrackPoly_tag *hPoly, const u128 *pVertex) {
    s32 position = hPoly->current;
    u128 *points = hPoly->pVertex;
    s32 count;

    SDF_VU0_COPYVECTOR(&points[position], pVertex);
    SDF_VU0_COPYVECTOR(&points[position + 1], pVertex + 1);
    position += 2;
    count = hPoly->point;
    hPoly->current = position;
    if (position == count) {
        SDF_VU0_COPYVECTOR(&points[0], pVertex);
        SDF_VU0_COPYVECTOR(&points[1], pVertex + 1);
        hPoly->current = 2;
    }
    if (hPoly->count < count - 2) {
        hPoly->count += 2;
    }
}

/* Resample the newest history span: cubic interpolation for established
 * tracks, linear extension while the history is still short. */
void dds3HermitianTrackPolySetVertex(dds3HermitianTrackPoly_tag *hPoly, const u128 *pVertex) {
    sdfVector4_t firstHistory[4];
    sdfVector4_t secondHistory[4];
    sdfVector4_t firstBasePoint;
    sdfVector4_t secondBasePoint;
    sdfVector4_t firstPointDelta;
    sdfVector4_t secondPointDelta;
    sdfVector4_t samples[2];
    s32 activePointCount = hPoly->count;
    s32 sampleCount = hPoly->split;
    f32 sampleWeightStep;
    f32 sampleWeight;
    s32 sampleIndex;
    u128 *secondSampleOutput;

    if (activePointCount == 0) {
        dds3HermitianTrackPolySetVertexSub(hPoly, pVertex);
        return;
    }

    if (activePointCount >= sampleCount * 6) {
        s32 savedRingPosition = hPoly->current;
        u128 *firstSampleOutputBase;
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)&firstHistory[0], (u128 *)&secondHistory[0], 3);
        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)&firstHistory[1], (u128 *)&secondHistory[1], 2);
        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)&firstHistory[2], (u128 *)&secondHistory[2], 1);
        SDF_VU0_COPYVECTOR(&firstHistory[3], pVertex);
        SDF_VU0_COPYVECTOR(&secondHistory[3], pVertex + 1);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        dds3HermitianTrackPolySetCurrent(hPoly, 2);
        if (sampleCount > 0) {
            firstSampleOutput = (u128 *)&samples[0];
            secondSampleOutput = (u128 *)&samples[1];
            firstSampleOutputBase = firstSampleOutput;
            sampleIndex = sampleCount;
            do {
                sampleWeight += sampleWeightStep;
                Eff_HermitianSpline(firstHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(firstSampleOutput);
                Eff_HermitianSpline(secondHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(secondSampleOutput);
                firstSampleOutput = firstSampleOutputBase;
                sampleIndex--;
                dds3HermitianTrackPolySetVertexSub(hPoly, firstSampleOutput);
            } while (sampleIndex != 0);
        }
        hPoly->count = activePointCount;
        hPoly->current = savedRingPosition;
    }

    if (activePointCount >= sampleCount * 4) {
        u128 *firstSampleOutputBase;
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)&firstHistory[0], (u128 *)&secondHistory[0], 2);
        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)&firstHistory[1], (u128 *)&secondHistory[1], 1);
        SDF_VU0_COPYVECTOR(&firstHistory[2], pVertex);
        SDF_VU0_COPYVECTOR(&secondHistory[2], pVertex + 1);
        SDF_VU0_COPYVECTOR(&firstHistory[3], pVertex);
        SDF_VU0_COPYVECTOR(&secondHistory[3], pVertex + 1);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        sampleIndex = 0;
        if (sampleCount > 0) {
            firstSampleOutput = (u128 *)&samples[0];
            secondSampleOutput = (u128 *)&samples[1];
            firstSampleOutputBase = firstSampleOutput;
            do {
                sampleWeight += sampleWeightStep;
                Eff_HermitianSpline(firstHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(firstSampleOutput);
                Eff_HermitianSpline(secondHistory, sampleWeight);
                SDF_VU0_STOREVECTOR_NOMEM(secondSampleOutput);
                firstSampleOutput = firstSampleOutputBase;
                dds3HermitianTrackPolySetVertexSub(hPoly, firstSampleOutput);
                sampleIndex++;
            } while (sampleIndex < sampleCount);
        }
    } else {
        u128 *firstSampleOutput;

        dds3HermitianTrackPolyGetVertex(hPoly, (u128 *)&firstBasePoint, (u128 *)&secondBasePoint, 1);
        SDF_VU0_LOADVECTOR(pVertex);
        SDF_VU0_LOADVECTOR_B(&firstBasePoint);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR(&firstPointDelta);
        SDF_VU0_LOADVECTOR(pVertex + 1);
        SDF_VU0_LOADVECTOR_B(&secondBasePoint);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_STOREVECTOR(&secondPointDelta);
        sampleWeight = 0.0f;
        sampleWeightStep = 1.0f / (f32)sampleCount;
        if (sampleCount > 0) {
            sampleIndex = sampleCount;
            firstSampleOutput = (u128 *)&samples[0];
            do {
                sampleWeight += sampleWeightStep;
                sampleIndex--;
                samples[0].x = firstBasePoint.x + firstPointDelta.x * sampleWeight;
                samples[0].y = firstBasePoint.y + firstPointDelta.y * sampleWeight;
                samples[0].z = firstBasePoint.z + firstPointDelta.z * sampleWeight;
                samples[1].x = secondBasePoint.x + secondPointDelta.x * sampleWeight;
                samples[1].y = secondBasePoint.y + secondPointDelta.y * sampleWeight;
                samples[1].z = secondBasePoint.z + secondPointDelta.z * sampleWeight;
                dds3HermitianTrackPolySetVertexSub(hPoly, firstSampleOutput);
            } while (sampleIndex != 0);
        }
    }
}

/* Submit up to two wrapped ring runs in 18-point strips, overlapping by two. */
void dds3HermitianTrackPolyDraw(dds3HermitianTrackPoly_tag *hPoly) {
    sdfDrawTag_t *list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    s32 runStart[4];
    s32 runPointCount[2];
    sdf3DPrimPolyParm_t *draw;
    s32 runIndex;
    s32 pointsRemaining;
    s32 wrappedStartOffset;
    s32 activePointCount;
    sdfDrawTag_t *list2;
    u64 *packet;

    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    sdf3DPrimLinkMat(list, hPoly->pMat, 0);
    activePointCount = hPoly->count;
    runStart[0] = hPoly->current - activePointCount;
    if (runStart[0] < 2) {
        wrappedStartOffset = runStart[0] - 2;
        runStart[1] = 0;
        runStart[0] = wrappedStartOffset + hPoly->point;
        runPointCount[0] = -wrappedStartOffset;
        runPointCount[1] = activePointCount - runPointCount[0] + 2;
    } else {
        runPointCount[0] = activePointCount;
        runPointCount[1] = 0;
    }
    effModelTrackPolyPrimParm.vertexColor = hPoly->pVC;
    for (runIndex = 0; runIndex < 2; runIndex++) {
        draw = &effModelTrackPolyPrimParm;
        draw->vertex = (sdfVector4_t *)&hPoly->pVertex[runStart[runIndex]];
        draw->color = hPoly->abgr;
        draw->faceCount = 0x10;
        draw->pointCount = 0x12;
        pointsRemaining = runPointCount[runIndex];
        while (pointsRemaining >= 0x12) {
            pointsRemaining -= 0x10;
            sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&effModelTrackPolyPrimParm)));
            effModelTrackPolyPrimParm.vertex += 0x10;
            effModelTrackPolyPrimParm.vertexColor += 0x10;
        }
        if (pointsRemaining >= 4) {
            draw->pointCount = pointsRemaining;
            draw->faceCount = pointsRemaining - 2;
            sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(draw)));
            draw->vertexColor += pointsRemaining;
        }
    }
    if (hPoly->blend < 4) {
        ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))effModelTrackPolyBlendOt[hPoly->blend]->regFunc)(effModelTrackPolyBlendOt[hPoly->blend], list);
    } else {
        list2 = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
        sdfDrawTagInit(list2);
        packet = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x30));
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 6;
        packet[5] = 0x42;
        sdfDrawTagLink(list2, EE_ADDR(packet));
        effPolyOt.regFunc(&effPolyOt, list2);
        packet = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x30));
        packet[0] = 2;
        packet[1] = ((u64)0x50000002 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8001;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfDrawTagLink(list, EE_ADDR(packet));
        effPolyOt.regFunc(&effPolyOt, list);
    }
}
