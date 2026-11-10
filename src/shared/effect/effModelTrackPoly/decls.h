/* Declarations of smt3-decomp src/smt3/effect/effModelTrackPoly.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_EFFMODELTRACKPOLY_H
#define SHARED_EFFECT_EFFMODELTRACKPOLY_H

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

SYMBOL_AT(sdfGrOt_t, D_002F64B8, sdfGrOtTable, 0x5A0);

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

#endif
