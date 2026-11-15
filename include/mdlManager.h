#ifndef MDLMANAGER_H
#define MDLMANAGER_H

#include "common.h"
#include "model.h"
#include "motion.h"

void mdlFree(mdlObj_t *aHandle);
u32 mdlGetColor(mdlObj_t *aHandle);
s32 mdlGetMajorNum(mdlObj_t *aHandle);
s32 mdlGetMinorNum(mdlObj_t *aHandle);
sdfMotionCtrlr_t *mdlGetMotionCtrlr(mdlObj_t *aHandle, s32 aGroup);
s32 mdlGetMotionNum(mdlObj_t *aHandle, s32 aGroup);
s32 mdlGetNumOfCut(mdlObj_t *aHandle, s32 aGroup);
void mdlGetPosition(mdlObj_t *aHandle);
s32 mdlHasMotionEx(mdlObj_t *aHandle, s32 aGroup);
void mdlLink(mdlObj_t *aHandle, void *aModelOt);
mdlObj_t *mdlLoad(u32 aMajor, u32 aMinor);
void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);
void mdlPackDecode(s32 major, s32 minor, s32 regFlags, const u8 *buff, s32 leng);
void mdlSetBlur(mdlObj_t *aHandle, s32 interval, s32 count, u32 startColor, u32 endColor);
void mdlSetColor(mdlObj_t *aHandle, u32 aColor);
void mdlSetPosition(mdlObj_t *aHandle);
void mdlSetRotate(mdlObj_t *aHandle);
void mdlSetScale(mdlObj_t *aHandle);
void mdlSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);
void mdlSmoothLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng, f32 aSmoothLeng);
void mdlSmoothSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng, f32 aSmoothLeng);

#endif
