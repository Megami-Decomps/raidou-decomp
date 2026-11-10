/* Declarations of smt3-decomp src/smt3/basic/dds3MoverBasic.c, for its bodies in this directory. */
#ifndef SHARED_BASIC_DDS3MOVERBASIC_H
#define SHARED_BASIC_DDS3MOVERBASIC_H

#include "common.h"

#include "basic.h"

#include "dds3obj.h"

#include "ps2asm.h"

/* The matter object caches its path position at +0x20, independently of pose. */
typedef struct
{
    u8 unk00[0x20];
    sdfVector4_t position;
} dds3MatterPathPosition_t;

extern void dds3PathBasicControlGetPos(dds3PathBasicControl_t *pControl);

extern void dds3PathBasicControlGetRot(dds3PathBasicControl_t *pControl);

extern f32 dds3PathBasicControlGetFovy(dds3PathBasicControl_t *pControl);

extern void dds3PathBasicControlNextStep(dds3PathBasicControl_t *work, fldResourceLightPath_t *output);

extern s32 dds3PathBasicControlIsFinished(dds3PathBasicControl_t *work);

extern void dds3SetLightObjectPathResoruce(dds3Basic_t *basic, fldResourceLightPath_t *resource);

extern void dds3BackupPoseSet(dds3PoseSet_t *pPoseSet);

extern void dds3GetSclPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *scale);

extern void dds3AddRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);

extern void dds3GetPosPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *offset);

typedef struct dds3MoverBasicWorkData_t
{
    dds3Basic_t *pObjectBasic;
    dds3PathBasicControl_t *pPathControl;
    dds3BasicControlProc_t pFunc;
    dds3Basic_t *pPathBasic;
} dds3MoverBasicWorkData_t;

extern s32 dds3MoverCount;

extern char dds3MoverNameTable[10][16];

extern void *memset(void *a, s32 b, size_t c);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern u32 dds3AllocObjectId(void);

extern dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType);

extern dds3PathBasicControl_t *dds3AllocPathBasicWorkData(dds3Basic_t *pathBasic);

extern void dds3DeletePathControl(dds3PathBasicControl_t *control);

void dds3MoverBasicFreePath(dds3Basic_t *pMoverBasic);

s32 MoverBasicMainprocuder(dds3Basic_t *pBasic);

s32 MoverBasicConstructor(dds3Basic_t *pBasic);

void MoverBasicDestructor(dds3Basic_t *pBasic);

/* Paths supply absolute channels; a primary callback supplies relative pose. */
s32 MoverBasicMainprocuder(dds3Basic_t *pBasic);

dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *pObjectBasic);

void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *mover, dds3BasicControlProc_t func);

void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic);

void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic);

void dds3MoverBasicFreePath(dds3Basic_t *pMoverBasic);

dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *pMoverBasic);

s32 dds3MoverBasicUpdate(dds3Basic_t *pBasic);

u32 BasicTypeToItemType(u32 type);

#endif
