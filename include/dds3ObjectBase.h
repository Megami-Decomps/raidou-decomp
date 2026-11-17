#ifndef DDS3OBJECTBASE_H
#define DDS3OBJECTBASE_H

#include "common.h"
#include "basic.h"

struct dds3Basic_t;

u8 dds3ObjectBaseCheckFlag(dds3Basic_t *pObjectBasic, u32 flag);
void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);
void dds3ObjectBaseFreeModelHandle(dds3Basic_t *pObjectBasic);
dds3Basic_t *dds3ObjectBaseGetControlBasic(dds3Basic_t *pObjectBasic, s32 itemType);
dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(dds3Basic_t *pObjectBasic);
void dds3ObjectBaseMatterAnimeSpeed(struct dds3Basic_t *pObjectBasic, u32 mode);
void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, s32 flag);
void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);
dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType);
void dds3ObjectBaseSetPadControl(dds3Basic_t *pObjectBasic, dds3BasicControlProc_t pFuncPadControl);
s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, dds3BasicControlProc_t func);

#include "enums.h"
#include "model.h"

f32 dds3CallMemset(dds3Basic_t *pObjectBasic, sdfModelRootNode_t *model_root);
f32 dds3CallGetBaseControlWorkData(dds3Basic_t *pObjectBasic);
void *dds3ObjectBaseGetUserValue(dds3Basic_t *pObjectBasic);
void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);
void dds3ObjectBaseMakeMatter(dds3Basic_t *pObjectBasic);
void dds3ObjectBaseMakeUnit(dds3Basic_t *pObjectBasic, MODELTYPE major, s32 minor);
void dds3ObjectBaseModelLinkOt(sdfModelOt_t *pModelOt, dds3Basic_t *pObjectBasic);
#ifdef VERSION_RAIDOU2
void itfCampStartProcess(dds3Basic_t *pObjectBasic);
#endif
s32 dds3ObjectBaseRegistArea(dds3Basic_t *pObjectBasic);
void dds3ObjectBaseSetControlBasicCamLen(dds3Basic_t *pObjectBasic);
void *dds3ObjectBaseSetUserValue(dds3Basic_t *pObjectBasic, void *pUserValue);
dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic);

#endif
