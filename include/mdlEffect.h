#ifndef MDLEFFECT_H
#define MDLEFFECT_H

#include "common.h"
#include "model.h"

void mdlFreeEffect(dds3ModelEffectCtrlr_t *aCtrlr);
void mdlGenEffect(mdlObj_t *aHandle, s32 aGroup, s32 aPlayGroup);
void mdlLinkEffect(mdlObj_t *owner, dds3ModelEffectCtrlr_t *anchorRecord);
void mdlSetEffectColorSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, u32 aColor);
void mdlSetEffectScaleSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, float aScale);
void mdlStopEffect(mdlObj_t *aHandle, s32 aPlayGroup);

#endif
