#ifndef SDFMAT_H
#define SDFMAT_H

#include "common.h"
#include "libsdf.h"
#include "sdf_resource.h"
#include "sdf_types.h"

void sdfMatFreeDelayed(sdfMat_t *aMat);
sdfMatUvwParm_t *sdfMatGetMultiTexUvwParm(sdfMat_t *aMat);
sdfMatUvwParm_t *sdfMatGetTexUvwParm(sdfMat_t *aMat);
void sdfMatInitSystem(void);
void sdfMatListCopyStat(sdfItemArray_t *aDst, sdfItemArray_t *aSrc);
void sdfMatListFreeResrc(void *aList, s32 aDelayed);
sdfItemArray_t *sdfMatListMake(sdfItemArray_t *aTexList, u32 *aData);
sdfDmaTag_t *sdfMatMakePacket(sdfMat_t *aMat, sdfDmaTag_t *aBuffer, s32 aId);
sdfMat_t *sdfMatNew(void);
void sdfMatSetAmbient(sdfMat_t *aMat, u32 aColor);
void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);
void sdfMatSetDiffuse(sdfMat_t *aMat, u32 aColor);
void sdfMatSetMultiDiffuse(sdfMat_t *aMat, u32 aColor);
void sdfMatSetMultiTex(sdfMat_t *aMat, sdfTexHandle_t *aTex);
void sdfMatSetMultiTexUvwByPtr(sdfMat_t *aMat, const f32 *aParm);
void sdfMatSetRefMap(sdfMat_t *aMat, u32 aColor);
void sdfMatSetSpecular(sdfMat_t *aMat, u32 aColor);
void sdfMatSetTex(sdfMat_t *aMat, sdfTexHandle_t *aTex);
void sdfMatSetTexUvwByPtr(sdfMat_t *aMat, const f32 *aParm);
void sdfMatUpdatePacket(sdfMat_t *aMat, s32 aId);
void sdfTexListAdd(sdfItemArray_t *aList, sdfTexHandle_t *aTexHandle);
sdfItemArray_t *sdfTexListDup(sdfItemArray_t *aList);
void sdfTexListFree(sdfItemArray_t *aList, s32 aReleaseItems);
sdfItemArray_t *sdfTexListNew(s32 aInitCapa);

#endif
