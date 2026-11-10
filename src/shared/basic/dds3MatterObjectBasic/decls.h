/* Declarations of smt3-decomp src/smt3/basic/dds3MatterObjectBasic.c, for its bodies in this directory. */
#ifndef SHARED_BASIC_DDS3MATTEROBJECTBASIC_H
#define SHARED_BASIC_DDS3MATTEROBJECTBASIC_H

#include "common.h"

#include "sdfFrame.h"

#include "basic.h"

#include "dds3obj.h"

#include "model.h"

#include "ps2asm.h"

extern u32 dds3GetKernelCount(void);

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);

extern void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);

extern void dds3FreePoseSet(dds3Basic_t *basic);

extern void sdfChipFreeDirect(void *aPtr);

extern void *sdfChipAllocDirect(u32 aSize);

extern void *memset(void *a, s32 b, size_t c);

extern void dds3InitPoseSet(dds3Basic_t *basic);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, u32 flag);

extern dds3Basic_t *dds3GetControlScene(void);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

dds3ObjectBaseControlWorkData_t *dds3MatterObjectBasicGetControlWorkData(const dds3Basic_t *pBasic);

dds3Basic_t *dds3AddMatterObjectBasic(s32 serialID, f32 *pos, f32 *rot);

void dds3DeleteMatterObjectBasic(dds3Basic_t *aPtr);

void dds3MatterObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx);

void dds3MatterObjectBasicSetHojiID(dds3Basic_t *aPtr, u32 hojiID);

u32 dds3MatterObjectBasicGetHojiID(u32 resID);

void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *aPtr, u32 alpmode);

u32 dds3MatterObjectBasicGetAlpMode(const dds3Basic_t *aPtr);

s32 dds3MatterObjectBasicConstructor(dds3Basic_t *aPtr);

void dds3MatterObjectBasicDestructor(dds3Basic_t *aPtr);

extern void *dds3ObjectBaseGetModelHandle(dds3Basic_t *basic);

extern void dds3OffPoseSetFlag(dds3PoseSet_t *pose, u32 flag);

extern void dds3GetRotPoseSet(dds3Basic_t *basic);

extern void dds3GetTMPoseSet(dds3Basic_t *basic);

extern void dds3GetPosPoseSet(dds3Basic_t *basic);

extern s32 fldDoor_Step(s32 matterSerialID, f32 *x, f32 *y, f32 *z);

extern void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);

extern void sdfModelEvalTm(sdfModelRootNode_t *model);

s32 dds3MatterObjectBasicMainProcedure(dds3Basic_t *basic);

extern sdfModelOt_t D_002F69B8[];

extern dds3Basic_t *fldUnitControlHandle;

extern void dds3ObjectBaseModelLinkOt(sdfModelOt_t *ot, dds3Basic_t *basic);

extern u8 dds3ObjectBaseCheckFlag(dds3Basic_t *basic, u32 flag);

extern void rsetOtOfs(s32 offset);

extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);

s32 dds3MatterObjectBasicDrawProcedure(dds3Basic_t *basic);

#endif
