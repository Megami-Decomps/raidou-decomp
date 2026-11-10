/* Declarations of smt3-decomp src/smt3/sdf/sdfMat.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFMAT_H
#define SHARED_SDF_SDFMAT_H

#include "common.h"

#include "libsdf.h"

#include "eeregs.h"

extern void *sdfChipAllocDirect(s32 aSize);

extern void *sdfChipAllocZeroDirect(s32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern sdfItemArray_t *sdfItemArrayNew(s32 aInitCapa, s32 aElemSize, s32 aGrowStep);

extern void sdfItemArrayFree(sdfItemArray_t *aArray);

extern void sdfItemArrayGrow(sdfItemArray_t *aArray);

extern void func_002BFF98(sdfItemArray_t *aArray, s32 aCapacity);

extern void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);

extern void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

extern sdfDelayedResrcCB_t sdfMatDelayedResrcCB;

extern sdfDelayedResrcCB_t sdfMatDelayedResrcCB2;

extern s32 sdfMatAllocCount;

/* The GS ALPHA_2 value per multi-texture method, indexed by sdfMat_t.multiTexMethod. */
extern u16 sdfMatMultiTexAlphaTable[];

extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

extern sdfTexHandle_t *sdfTexDup(sdfTexHandle_t *aHandle);

extern void sdfTexSetClutColor(sdfTexHandle_t *aHandle, s32 aColor, f32 aRatio);

extern u64 sdfTexGetTex0Reg(sdfTexHandle_t *aHandle);

extern u64 sdfTexGetTex1Reg(sdfTexHandle_t *aHandle);

extern u64 sdfTexGetClampReg(sdfTexHandle_t *aHandle);

extern f32 sdfSin(f32 aRadius);

extern f32 sdfCos(f32 aRadius);

sdfItemArray_t *sdfTexListNew(s32 aInitCapa);

sdfItemArray_t *sdfMatListNew(s32 aInitCapa);

void sdfMatListGrow(sdfItemArray_t *aList);

void sdfMatListFree(sdfItemArray_t *aList);

void sdfMatFree(sdfMat_t *aMat);

sdfMatUvwParm_t *sdfMatAllocTexUvwParm(void);

sdfMatUvwParm_t *sdfMatGetTexUvwParm(sdfMat_t *aMat);

sdfMatUvwParm_t *sdfMatGetMultiTexUvwParm(sdfMat_t *aMat);

void sdfMatCopyStat(sdfMat_t *aDstMat, sdfMat_t *aSrcMat);

void sdfMatListCopyStat(sdfItemArray_t *aDst, sdfItemArray_t *aSrc);

void sdfMatSetUvwParm(sdfMatTexTm_t *aUvwPacket, const sdfMatUvwParm_t *aParm);

void sdfMatUpdatePacketBasic(sdfMat_t *aMat, sdfMatPacket_t *aPacket);

void sdfMatUpdatePacketUvwParm(sdfMat_t *aMat, sdfMatPacket_t *aPacket);

void sdfMatUpdatePacketMultiTex(sdfMat_t *aMat, sdfMatPacket_t *aPacket);

void sdfMatUpdatePacketFaceDiv(sdfMat_t *aMat, sdfMatPacket_t *aPacket);

void sdfMatSetAmbient(sdfMat_t *aMat, u32 aColor);

void sdfMatSetDiffuse(sdfMat_t *aMat, u32 aColor);

void sdfMatSetSpecular(sdfMat_t *aMat, u32 aColor);

void sdfMatSetRefMap(sdfMat_t *aMat, u32 aColor);

void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);

void sdfMatSetTex(sdfMat_t *aMat, sdfTexHandle_t *aTex);

void sdfMatSetTexUvw(sdfMat_t *aMat, f32 aOffsetX, f32 aOffsetY, f32 aTileX, f32 aTileY,
                     f32 aAngle);

void sdfMatSetMultiDiffuse(sdfMat_t *aMat, u32 aColor);

void sdfMatSetMultiMethod(sdfMat_t *aMat, u32 aMethod);

void sdfMatSetMultiTex(sdfMat_t *aMat, sdfTexHandle_t *aTex);

void sdfMatSetMultiTexUvw(sdfMat_t *aMat, f32 aOffsetX, f32 aOffsetY, f32 aTileX, f32 aTileY,
                          f32 aAngle);

void sdfMatSetFaceDivParm(sdfMat_t *aMat, f32 aBias, f32 aSoften);

sdfMat_t *sdfMatNew(void);

u8 *sdfMatSet(sdfMat_t *aMatPtr, sdfItemArray_t *aTexList, u8 *aDef);

void sdfMatListAdd(sdfItemArray_t *aList, sdfMat_t *aMat);

sdfItemArray_t *sdfTexListNew(s32 aInitCapa);

void sdfTexListFree(sdfItemArray_t *aList, s32 aReleaseItems);

void sdfTexListAdd(sdfItemArray_t *aList, sdfTexHandle_t *aTexHandle);

void sdfTexListSetCapacity(sdfItemArray_t *aList, s32 aCapa, s32 aReleaseItems);

sdfItemArray_t *sdfTexListDup(sdfItemArray_t *aList);

void sdfTexListSetClutColor(sdfItemArray_t *aList, s32 aColor, f32 aRatio);

void sdfMatInitSystem(void);

sdfItemArray_t *sdfMatListNew(s32 aInitCapa);

void sdfMatListFree(sdfItemArray_t *aList);

void sdfMatListFreeResrc(void *aList, s32 aDelayed);

void sdfMatListGrow(sdfItemArray_t *aList);

void sdfMatListAdd(sdfItemArray_t *aList, sdfMat_t *aMat);

void sdfMatSetAmbient(sdfMat_t *aMat, u32 aColor);

void sdfMatSetDiffuse(sdfMat_t *aMat, u32 aColor);

void sdfMatSetSpecular(sdfMat_t *aMat, u32 aColor);

void sdfMatSetRefMap(sdfMat_t *aMat, u32 aColor);

void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);

void sdfMatSetTex(sdfMat_t *aMat, sdfTexHandle_t *aTex);

sdfMatUvwParm_t *sdfMatAllocTexUvwParm(void);

sdfMatUvwParm_t *sdfMatGetTexUvwParm(sdfMat_t *aMat);

void sdfMatSetTexUvw(sdfMat_t *aMat, f32 aOffsetX, f32 aOffsetY, f32 aTileX, f32 aTileY,
                     f32 aAngle);

void sdfMatSetTexUvwByPtr(sdfMat_t *aMat, const f32 *aParm);

void sdfMatSetMultiDiffuse(sdfMat_t *aMat, u32 aColor);

void sdfMatSetMultiMethod(sdfMat_t *aMat, u32 aMethod);

void sdfMatSetMultiTex(sdfMat_t *aMat, sdfTexHandle_t *aTex);

sdfMatUvwParm_t *sdfMatGetMultiTexUvwParm(sdfMat_t *aMat);

void sdfMatSetMultiTexUvw(sdfMat_t *aMat, f32 aOffsetX, f32 aOffsetY, f32 aTileX, f32 aTileY,
                          f32 aAngle);

void sdfMatSetMultiTexUvwByPtr(sdfMat_t *aMat, const f32 *aParm);

void sdfMatSetFaceDivParm(sdfMat_t *aMat, f32 aBias, f32 aSoften);

sdfMat_t *sdfMatNew(void);

u8 *sdfMatSet(sdfMat_t *aMatPtr, sdfItemArray_t *aTexList, u8 *aDef);

void sdfMatFree(sdfMat_t *aMat);

void sdfMatFreeDelayed(sdfMat_t *aMat);

sdfDmaTag_t *sdfMatMakePacket(sdfMat_t *aMat, sdfDmaTag_t *aBuffer, s32 aId);

void sdfMatUpdatePacketBasic(sdfMat_t *aMat, sdfMatPacket_t *aPacket);

void sdfMatSetUvwParm(sdfMatTexTm_t *aUvwPacket, const sdfMatUvwParm_t *aParm);

void sdfMatUpdatePacketUvwParm(sdfMat_t *aMat, sdfMatPacket_t *aPacket);

void sdfMatUpdatePacketMultiTex(sdfMat_t *aMat, sdfMatPacket_t *aPacket);

void sdfMatUpdatePacketFaceDiv(sdfMat_t *aMat, sdfMatPacket_t *aPacket);

void sdfMatUpdatePacket(sdfMat_t *aMat, s32 aId);

sdfItemArray_t *sdfMatListMake(sdfItemArray_t *aTexList, u32 *aData);

void sdfMatListUpdatePacket(sdfItemArray_t *aMatList, s32 aId);

void sdfMatCopyStat(sdfMat_t *aDstMat, sdfMat_t *aSrcMat);

void sdfMatListCopyStat(sdfItemArray_t *aDst, sdfItemArray_t *aSrc);

#endif
