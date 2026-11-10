/* Declarations of smt3-decomp src/smt3/sdf/sdfMotion.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFMOTION_H
#define SHARED_SDF_SDFMOTION_H

#include "common.h"

#include "ps2asm.h"

#include "sdfFrame.h"

#include "libsdf.h"

#include "model.h"

#include "motion.h"

typedef struct
{
    sdfMotionNode_t nodeBase;
    sdfModelMorph_t *morphNode;
    sdfModelMorph_t blendSrc;
} sdfMotNodeMorph_t;

extern void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

extern void Lerp(f32 aRatio);

extern void ToMatrix(void);

extern sdfMotionNodeVTBL_t sdfMotMorphTable;

void sdfMotionSeek(sdfMotionCtrlr_t *aCtrlr, f32 aFrame);

typedef sdfMotionNode_t *(*sdfMotGenNodeFunc)(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

extern void *sdfChipAllocDirect(s32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern sdfModelNode_t *sdfModelSearchNode(sdfModelRootNode_t *aRoot, u32 aId);

extern void sdfMotionChangeBlend(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType, f32 aPreFrame,
                                 f32 aBlendLeng);

/* The type id's high half picks one of these group tables, its low half the
 * generator inside the group. */
extern void *sdfChipAllocZeroDirect(s32 aSize);

extern void *sdfItemArrayNew(s32 aInitCapa, s32 aElemSize, s32 aGrowStep);

extern void sdfItemArrayFree(void *aArray);

extern void sdfMatSetDiffuse(sdfMat_t *aMat, u32 aColor);

extern void sdfMatSetAmbient(sdfMat_t *aMat, u32 aColor);

extern void sdfMatSetSpecular(sdfMat_t *aMat, u32 aColor);

extern void sdfMatSetRefMap(sdfMat_t *aMat, u32 aColor);

extern void sdfMatSetMultiDiffuse(sdfMat_t *aMat, u32 aColor);

extern void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);

extern void sdfMatSetTexUvwByPtr(sdfMat_t *aMat, void *aParm);

extern void sdfMatSetMultiTexUvwByPtr(sdfMat_t *aMat, void *aParm);

extern void sdfMotGetKey(sdfMotionNode_t *aThis, f32 aFrame, sdfMotGetKeyIf_t *aItf);

extern /* vu0 routine: blend the two colour keys */
u32 sdfMotInterpolateKeyColor(sdfMotGetKeyIf_t *aItf);

extern f32 sdfMotInterpolateKeyFloat(sdfMotGetKeyIf_t *aItf);

extern void sdfMotInterpolateKeyUvw(sdfMotGetKeyIf_t *aItf, void *aDst);

extern void sdfMotInterpolateKeyUvwSub(f32 *aDst, f32 *aSrc0, f32 *aSrc1, f32 aRate);

extern sdfMotGenNodeFunc sdfMotGenNodeTable[];

extern sdfMotGenNodeFunc sdfMotGenNodeDef[];

extern sdfMotGenNodeFunc sdfMotGenMatTable[];

extern sdfMotGenNodeFunc sdfMotGenMorphTable[];

extern sdfMotionNodeVTBL_t sdfMotModelLinerPosTable;

extern sdfMotionNodeVTBL_t sdfMotModelLinerRotXYZTable;

extern sdfMotionNodeVTBL_t sdfMotModelLinerScaleTable;

extern sdfMotionNodeVTBL_t sdfMotModelLerpRotQuatTable;

typedef struct
{
    sdfMotNodeModelBase_t modelBase;
    s8 blendFrom;
} sdfMotNodeModelFlag_t;

typedef struct
{
    sdfMotNodeMatBase_t base;
    u32 blendSrc;
} sdfMotNodeMatColor_t;

typedef struct
{
    sdfMotNodeMatBase_t matBase;
    f32 blendFrom;
} sdfMotNodeMatFloat_t;

/* Five weights, not the six-word sdfMatUvwParm_t: only offset, tile and angle
 * are keyed. */
typedef struct
{
    f32 parm[5];
} sdfMotUvwKey_t;

typedef struct
{
    sdfMotNodeMatBase_t base;
    sdfMotUvwKey_t blendFrom;
} sdfMotNodeMatUvw_t;

extern sdfMatUvwParm_t *sdfMatGetTexUvwParm(sdfMat_t *aMat);

extern sdfMatUvwParm_t *sdfMatGetMultiTexUvwParm(sdfMat_t *aMat);

extern sdfMotionNodeVTBL_t sdfMotModelVisibleTable;

extern sdfMotionNodeVTBL_t sdfMotMatDiffuseTable;

extern sdfMotionNodeVTBL_t sdfMotMatAmbientTable;

extern sdfMotionNodeVTBL_t sdfMotMatSpecularTable;

extern sdfMotionNodeVTBL_t sdfMotMatRefMapTable;

extern sdfMotionNodeVTBL_t sdfMotMatConstantTable;

extern sdfMotionNodeVTBL_t sdfMotMatUvwTable;

extern sdfMotionNodeVTBL_t sdfMotMatMultiUvwTable;

extern sdfMotionNodeVTBL_t sdfMotMatMultiDiffuseTable;

extern sdfMotionNodeVTBL_t sdfMotMatUvw2Table;

extern sdfMotionNodeVTBL_t sdfMotMatMultiUvw2Table;

sdfMotionNode_t *sdfMotionNodeNew(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotionNodeFree(sdfMotionNode_t *aNode);

void sdfMotionNodeBaseInit(sdfMotionNode_t *aNode, sdfMotionCtrlr_t *aCtrlr, sdfMotionNodeVTBL_t *aVTBL);

sdfMotionCtrlr_t *sdfMotionCtrlrNew(sdfModelRootNode_t *aModel, sdfMotionPlayBook_t *aBook);

void sdfMotionCtrlrFree(sdfMotionCtrlr_t *aCtrlr);

void sdfMotionChangeBlend(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType, f32 aPreFrame,
                          f32 aBlendLeng);

void sdfMotionChange(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType);

void sdfMotionSeek(sdfMotionCtrlr_t *aCtrlr, f32 aFrame);

s32 sdfMotionNext(sdfMotionCtrlr_t *aCtrlr);

void sdfMotionPause(sdfMotionCtrlr_t *aCtrlr);

void sdfMotionPauseOff(sdfMotionCtrlr_t *aCtrlr);

void sdfMotCLBK_Destroy(sdfMotionNode_t *aNode);

void sdfMotCLBK_SetCut(sdfMotionNode_t *aNode, sdfMotionCutChunk_t *aTable);

void sdfMotGetKey(sdfMotionNode_t *aThis, f32 aFrame, sdfMotGetKeyIf_t *aItf);

f32 sdfMotInterpolateKeyFloat(sdfMotGetKeyIf_t *aItf);

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotInterpolateKeyXYZ(sdfMotGetKeyIf_t *aItf);

/* vu0 routine: blend the two colour keys */
u32 sdfMotInterpolateKeyColor(sdfMotGetKeyIf_t *aItf);

void sdfMotInterpolateKeyUvwSub(f32 *aDst, f32 *aSrc0, f32 *aSrc1, f32 aRate);

void sdfMotInterpolateKeyUvw(sdfMotGetKeyIf_t *aItf, void *aDst);

sdfMotionNode_t *sdfMotGenNode(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotGenModelInit(sdfMotNodeModelBase_t *aBase, sdfMotionCtrlr_t *aCtrlr, sdfMotionNodeVTBL_t *aVTBL, u32 aNodeId);

sdfMotionNode_t *sdfMotGenModelLinerPos(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerPosSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame);

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerPosBlendSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame, f32 aBlendRate);

sdfMotionNode_t *sdfMotGenModelLinerRotXYZ(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerRotXYZSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame);

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerRotXYZBlendSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame, f32 aBlendRate);

sdfMotionNode_t *sdfMotGenModelLinerScale(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerScaleSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame);

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLinerScaleBlendSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame, f32 aBlendRate);

sdfMotionNode_t *sdfMotGenModelLerpRotQuat(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLerpRotQuatSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame);

/* vu0 routine: sample and blend the channel using the SDK packed operations. */
void sdfMotCLBK_modelLerpRotQuatBlendSeek(sdfMotNodeModelVec_t *aThis, f32 aFrame, f32 aBlendRate);

sdfMotionNode_t *sdfMotGenModelVisible(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotCLBK_modelVisibleSeek(sdfMotNodeModelBase_t *aThis, f32 aFrame);

void sdfMotCLBK_modelVisibleBlendSeek(sdfMotNodeModelBase_t *aThis, f32 aFrame, f32 aBlendRate);

void sdfMotCLBK_modelVisibleInitBlend(sdfMotNodeModelFlag_t *aThis);

void sdfMotCLBK_modelPosInitBlend(sdfMotNodeModelVec_t *aThis);

void sdfMotCLBK_modelRotInitBlend(sdfMotNodeModelVec_t *aThis);

void sdfMotCLBK_modelScaleInitBlend(sdfMotNodeModelVec_t *aThis);

sdfMotionNode_t *sdfMotGenMatNode(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotGenMatInit(sdfMotNodeMatBase_t *aBase, sdfMotionCtrlr_t *aCtrlr, sdfMotionNodeVTBL_t *aVTBL, u32 aNodeId);

sdfMotionNode_t *sdfMotGenMatDiffuse(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotCLBK_matDiffuseSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame);

void sdfMotCLBK_matDiffuseBlendSeek(sdfMotNodeMatColor_t *aThis, f32 aFrame, f32 aBlendRate);

void sdfMotCLBK_matDiffuseInitBlend(sdfMotNodeMatColor_t *aThis);

sdfMotionNode_t *sdfMotGenMatAmbient(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotCLBK_matAmbientSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame);

void sdfMotCLBK_matAmbientBlendSeek(sdfMotNodeMatColor_t *aThis, f32 aFrame, f32 aBlendRate);

void sdfMotCLBK_matAmbientInitBlend(sdfMotNodeMatColor_t *aThis);

sdfMotionNode_t *sdfMotGenMatSpecular(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotCLBK_matSpecularSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame);

void sdfMotCLBK_matSpecularBlendSeek(sdfMotNodeMatColor_t *aThis, f32 aFrame, f32 aBlendRate);

void sdfMotCLBK_matSpecularInitBlend(sdfMotNodeMatColor_t *aThis);

sdfMotionNode_t *sdfMotGenMatRefMap(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotCLBK_matRefMapSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame);

void sdfMotCLBK_matRefMapBlendSeek(sdfMotNodeMatColor_t *aThis, f32 aFrame, f32 aBlendRate);

void sdfMotCLBK_matRefMapInitBlend(sdfMotNodeMatColor_t *aThis);

sdfMotionNode_t *sdfMotGenMatConstant(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotCLBK_matConstantSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame);

void sdfMotCLBK_matConstantBlendSeek(sdfMotNodeMatFloat_t *aThis, f32 aFrame, f32 aBlendRate);

void sdfMotCLBK_matConstantInitBlend(sdfMotNodeMatFloat_t *aThis);

sdfMotionNode_t *sdfMotGenMatUvw(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotCLBK_matUvwSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame);

void sdfMotCLBK_matUvwBlendSeek(sdfMotNodeMatUvw_t *aThis, f32 aFrame, f32 aBlendRate);

void sdfMotCLBK_matUvwInitBlend(sdfMotNodeMatUvw_t *aThis);

sdfMotionNode_t *sdfMotGenMatMultiUvw(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotCLBK_matMultiUvwSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame);

void sdfMotCLBK_matMultiUvwBlendSeek(sdfMotNodeMatUvw_t *aThis, f32 aFrame, f32 aBlendRate);

void sdfMotCLBK_matMultiUvwInitBlend(sdfMotNodeMatUvw_t *aThis);

sdfMotionNode_t *sdfMotGenMatMultiDiffuse(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotCLBK_matMultiDiffuseSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame);

void sdfMotCLBK_matMultiDiffuseBlendSeek(sdfMotNodeMatColor_t *aThis, f32 aFrame, f32 aBlendRate);

void sdfMotCLBK_matMultiDiffuseInitBlend(sdfMotNodeMatColor_t *aThis);

sdfMotionNode_t *sdfMotGenMatUvw2(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotCLBK_matUvw2Seek(sdfMotNodeMatBase_t *aThis, f32 aFrame);

void sdfMotCLBK_matUvw2BlendSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame, f32 aBlendRate);

void sdfMotCLBK_matUvw2InitBlend(void);

sdfMotionNode_t *sdfMotGenMatMultiUvw2(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotCLBK_matMultiUvw2Seek(sdfMotNodeMatBase_t *aThis, f32 aFrame);

void sdfMotCLBK_matMultiUvw2BlendSeek(sdfMotNodeMatBase_t *aThis, f32 aFrame, f32 aBlendRate);

void sdfMotCLBK_matMultiUvw2InitBlend(void);

sdfMotionNode_t *sdfMotGenMorphNode(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId);

void sdfMotGenMorphInit(sdfMotNodeMorph_t *aBase,
                                 sdfMotionCtrlr_t *aCtrlr, sdfMotionNodeVTBL_t *aVTBL, s32 aNodeId);

sdfMotionNode_t *sdfMotGenMorph(sdfMotionCtrlr_t *aCtrlr,
                                                           s32 aTypeId, s32 aNodeId);

void sdfMotCLBK_morphSeek(sdfMotNodeMorph_t *aThis, f32 aFrame);

void sdfMotCLBK_morphBlendSeek(sdfMotNodeMorph_t *aThis, f32 aFrame, f32 aBlendRate);

void sdfMotCLBK_morphInitBlend(sdfMotNodeMorph_t *aThis);

extern sdfMotGenNodeFunc sdfMotGenNodeTable[4];

extern sdfMotGenNodeFunc sdfMotGenNodeDef[5];

extern sdfMotionNodeVTBL_t sdfMotModelLinerPosTable;

extern sdfMotionNodeVTBL_t sdfMotModelLinerRotXYZTable;

extern sdfMotionNodeVTBL_t sdfMotModelLinerScaleTable;

extern sdfMotionNodeVTBL_t sdfMotModelLerpRotQuatTable;

extern sdfMotionNodeVTBL_t sdfMotModelVisibleTable;

extern sdfMotGenNodeFunc sdfMotGenMatTable[10];

extern sdfMotionNodeVTBL_t sdfMotMatDiffuseTable;

extern sdfMotionNodeVTBL_t sdfMotMatAmbientTable;

extern sdfMotionNodeVTBL_t sdfMotMatSpecularTable;

extern sdfMotionNodeVTBL_t sdfMotMatRefMapTable;

extern sdfMotionNodeVTBL_t sdfMotMatConstantTable;

extern sdfMotionNodeVTBL_t sdfMotMatUvwTable;

extern sdfMotionNodeVTBL_t sdfMotMatMultiUvwTable;

extern sdfMotionNodeVTBL_t sdfMotMatMultiDiffuseTable;

extern sdfMotionNodeVTBL_t sdfMotMatUvw2Table;

extern sdfMotionNodeVTBL_t sdfMotMatMultiUvw2Table;

extern sdfMotGenNodeFunc sdfMotGenMorphTable[2] __attribute__((section(".data")));

extern sdfMotionNodeVTBL_t sdfMotMorphTable;

#endif
