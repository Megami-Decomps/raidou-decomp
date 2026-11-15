#ifndef CMPMODEL_H
#define CMPMODEL_H

#include "common.h"
#include "enums.h"
#include "model.h"

struct dds3ModelHandle_t;

void End_cmpModel(void);
void Init_cmpModel(void);
void cmpAddPos(f32 a_x, f32 a_y, f32 a_z);
void cmpAddRot(f32 a_x, f32 a_y, f32 a_z);
s32 cmpCallGetFlagScreenShot4(void);
dds3ModelResrc_t *cmpCallPrepare(MODELTYPE MajorNo, s32 MinorNo, s32 BlockMode);
void cmpCampEFFTES(void);
void cmpDeleteBstEff(void);
void cmpDeleteEffect(void);
void cmpDrawBstEff(void);
void cmpDrawEffect(void);
s8 cmpDrawStatusModel(sdfModelOt_t *pModelOt);
void cmpEffectSetRGBA(u32 a_rgba);
s32 cmpFreeResource(void);
s32 cmpGetMotionNum(void);
f32 cmpGetMotionSpeed(void);
s8 cmpGetPosLimit(void);
s8 cmpGetViewerLimit(void);
void cmpInitCmpModelSub(void);
void cmpInitCore2(void);
void cmpInitDstEff(void);
void cmpSetLight(void);
void cmpSetLightDef(void);
void cmpSetLightUnit(s32 a_modelID, s32 a_lightNo);
void cmpSetModelMotion(struct dds3ModelHandle_t *a_hModel);
f32 cmpSetModelScale(struct dds3ModelHandle_t *Handle, s32 Mode);
void cmpSetPosLimit(s8 a_bool);
void cmpSetSmoothLoopMotion(s32 a_motNo, f32 a_beforeLeng, f32 a_smoothLeng);
void cmpSetStatusModelParam(void);
void cmpSetViewerLimit(s8 a_bool);
void cmpWaitLoadModel(void);

#endif
