#ifndef DDS3PATHBASIC_H
#define DDS3PATHBASIC_H

#include "common.h"
#include "basic.h"

dds3PathBasicControl_t *dds3AllocPathBasicWorkData(dds3Basic_t *basic);
void dds3DeletePathControl(dds3PathBasicControl_t *work);
f32 dds3PathBasicControlGetFovy(dds3PathBasicControl_t *pControl);
void dds3PathBasicControlGetPos(dds3PathBasicControl_t *pControl);
f32 dds3PathBasicControlGetRatio(dds3PathBasicControl_t *pControl);
void dds3PathBasicControlGetRot(dds3PathBasicControl_t *pControl);
s32 dds3PathBasicControlIsFinished(dds3PathBasicControl_t *work);
void dds3PathBasicControlNextStep(dds3PathBasicControl_t *work, fldResourceLightPath_t *output);
void dds3PathBasicControlSetRatio(dds3PathBasicControl_t *pControl, f32 ratio);

dds3Basic_t *dds3AddPathBasic(u32 serialID, void *pData, char *pName);
void dds3PathBasicFreeControl(dds3PathBasicControl_t *pControl);

#endif
