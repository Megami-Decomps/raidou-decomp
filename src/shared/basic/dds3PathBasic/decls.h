/* Declarations of smt3-decomp src/smt3/basic/dds3PathBasic.c, for its bodies in this directory. */
#ifndef SHARED_BASIC_DDS3PATHBASIC_H
#define SHARED_BASIC_DDS3PATHBASIC_H

#include "common.h"

#include "sdfFrame.h"

#include "basic.h"

#include "ps2asm.h"

#define DDS3_PATH_CURVE_POSITION 0

#define DDS3_PATH_CURVE_ROTATION 2

#define DDS3_PATH_CURVE_SCALAR 4

#define DDS3_PATH_CURVE_WORLD_TRANSFORM 5

#define DDS3_PATH_POSITION_CHANNEL 0x1

#define DDS3_PATH_ROTATION_CHANNEL 0x2

#define DDS3_PATH_SCALAR_CHANNEL 0x4

#define DDS3_PATH_WORLD_TRANSFORM_CHANNEL 0x10

extern void *memset(void *a, s32 b, size_t c);

extern void *sdfChipAllocDirect(u32 aSize);

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern s32 dds3AllocSplineHandle(f32 (*points)[3], u32 count, s32 mode);

extern void dds3PathEvalSpline(u32 *segment, f32 *weight, fldResourcePathData_t *keys, f32 time);

extern void Lerp(f32 aRatio);

extern void dds3FreeSplineHandle(s32 spline);

extern void sdfChipFreeDirect(void *aPtr);

void dds3PathEvalSpline(u32 *segment, f32 *weight, fldResourcePathData_t *keys, f32 frame);

dds3Basic_t *dds3AddPathBasic(s32 serialID, void *pData, const char *pName);

dds3PathBasicControl_t *dds3AllocPathBasicWorkData(dds3Basic_t *basic);

void dds3DeletePathControl(dds3PathBasicControl_t *work);

void dds3PathBasicControlGetPos(dds3PathBasicControl_t *pControl);

void dds3PathBasicControlGetRot(dds3PathBasicControl_t *pControl);

/* vu0 routine: interpolate the light channel's radii and colors. */
void dds3PathBasicControlNextStep(dds3PathBasicControl_t *work, fldResourceLightPath_t *output);

f32 dds3PathBasicControlGetFovy(dds3PathBasicControl_t *pControl);

s32 dds3PathBasicControlIsFinished(dds3PathBasicControl_t *work);

void dds3PathBasicControlAlterMode(dds3PathBasicControl_t *pControl, s32 mode);

s32 dds3PathBasicControlGetMode(dds3PathBasicControl_t *work);

void dds3PathBasicControlSetRatio(dds3PathBasicControl_t *pControl, f32 ratio);

f32 dds3PathBasicControlGetRatio(dds3PathBasicControl_t *pControl);

void dds3PathBasicPauseOn(dds3PathBasicControl_t *work);

void dds3PathBasicPauseOff(dds3PathBasicControl_t *pControl);

void dds3PathBasicLoopOn(dds3PathBasicControl_t *pControl);

void dds3PathBasicLoopOff(dds3PathBasicControl_t *pControl);

#endif
