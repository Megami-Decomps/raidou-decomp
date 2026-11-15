#ifndef SDFMOTION_H
#define SDFMOTION_H

#include "common.h"
#include "model.h"
#include "motion.h"

void sdfMotionChange(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType);
void sdfMotionChangeBlend(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType, f32 aPreFrame, f32 aBlendLeng);
void sdfMotionCtrlrFree(sdfMotionCtrlr_t *aCtrlr);
sdfMotionCtrlr_t *sdfMotionCtrlrNew(sdfModelRootNode_t *aModel, sdfMotionPlayBook_t *aBook);
s32 sdfMotionNext(sdfMotionCtrlr_t *aCtrlr);
void sdfMotionPause(sdfMotionCtrlr_t *aCtrlr);
void sdfMotionPauseOff(sdfMotionCtrlr_t *aCtrlr);
void sdfMotionSeek(sdfMotionCtrlr_t *aCtrlr, f32 aFrame);

#endif
