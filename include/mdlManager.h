#ifndef MDLMANAGER_H
#define MDLMANAGER_H

#include "common.h"
#include "model.h"
#include "motion.h"

void mdlFree(mdlObj_t *aHandle);
void mdlFreeBlur(mdlObj_t *aHandle);
saddr_t mdlPrepare(s32 aMajor, s32 aMinor, s32 aBlockMode);
u32 mdlGetColor(mdlObj_t *aHandle);
s32 mdlGetMajorNum(mdlObj_t *aHandle);
s32 mdlGetMinorNum(mdlObj_t *aHandle);
sdfMotionCtrlr_t *mdlGetMotionCtrlr(mdlObj_t *aHandle, s32 aGroup);
u16 mdlGetMotionLengthEx(mdlObj_t *aHandle, s32 aGroup);
s32 mdlGetMotionNum(mdlObj_t *aHandle, s32 aGroup);
s32 mdlGetNumOfCut(mdlObj_t *aHandle, s32 aGroup);
void mdlGetPosition(mdlObj_t *aHandle);
void mdlGetScale(mdlObj_t *aHandle);
s32 mdlHasMotionEx(mdlObj_t *aHandle, s32 aGroup);
void mdlLink(mdlObj_t *aHandle, void *aModelOt);
mdlObj_t *mdlLoad(u32 aMajor, u32 aMinor);
void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);
void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng);
void mdlSetBlur(mdlObj_t *aHandle, s32 interval, s32 count, u32 startColor, u32 endColor);
void mdlSetColor(mdlObj_t *aHandle, u32 aColor);
void mdlSetMotionSpeedEx(mdlObj_t *aHandle, s32 aGroup, f32 speed);
void mdlSetPosition(mdlObj_t *aHandle);
void mdlSetRotate(mdlObj_t *aHandle);
void mdlSetScale(mdlObj_t *aHandle);
void mdlSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);
void mdlSmoothLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng, f32 aSmoothLeng);
void mdlSmoothSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng, f32 aSmoothLeng);

#include "sdf_resource.h"

struct dds3ModelHandle_t;

void mdlLink_NoDraw(struct dds3ModelHandle_t *aHandle, sdfModelOt_t *aModelOt);
void mdlLink_Kubi(mdlObj_t *aHandle, void *aModelOt, s32 aNode, f32 aPitch, f32 aYaw);
void mdlRegist(s32 aMajor, s32 aMinor, s32 aFlag, sdfModelTexList_t *aTexList, sdfModelDef_t *aData, sdfMemHandle_t *aHandle);
void mdlRegistMotion(s32 aMajor, s32 aMinor, u32 aFlags, s32 aFirstMotion, s32 aGroup, s32 aPlayGroup, sdfMotionPlayBook_t *aData, sdfMemHandle_t *aHandle);

void mdlCallMemGetInfo(void);
dds3ModelResrc_t *mdlSearchResrc(s32 aMajor, s32 aMinor);
void mdlUnRegist(s32 aMajor, s32 aMinor);
void mdlInitManager(void);

#endif
