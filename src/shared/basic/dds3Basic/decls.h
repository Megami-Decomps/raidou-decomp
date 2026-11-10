/* Declarations of smt3-decomp src/smt3/basic/dds3Basic.c, for its bodies in this directory. */
#ifndef SHARED_BASIC_DDS3BASIC_H
#define SHARED_BASIC_DDS3BASIC_H

#include "common.h"

#include "basic.h"

#include "ps2asm.h"

extern void *sdfChipAllocDirect(u32 aSize);

extern void Normalize(void);

extern void Mult(void);

extern void ToMatrix(void);

extern f32 fabsf(f32 value);

void dds3BackupPoseSet(dds3PoseSet_t *pPoseSet);

extern u32 dds3ObjectIdCounter;

void sdfChipFreeDirect(void *aPtr);

extern void dds3DeleteBasic(dds3Basic_t *basic);

extern dds3BasicFunction_t *dds3BasicFuncTablePtr[];

extern char dds3BasicPosLabel[];

extern char dds3BasicRotLabel[];

extern char dds3BasicSclLabel[];

dds3Basic_t *dds3AllocBasic(u32 type);

void dds3DeleteBasic(dds3Basic_t *basic);

s32 dds3InitPoseSet(dds3Basic_t *basic);

void dds3FreePoseSet(dds3Basic_t *basic);

void dds3OnPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

/* vu0 routine: reset translation/rotation and set unit scale. */
void dds3BackupPoseSet(dds3PoseSet_t *pPoseSet);

void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);

void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 r);

f32 func_0010DE98(dds3Basic_t *basic);

void dds3SetPosPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *pos);

void dds3SetRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);

void dds3SetSclPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *scl);

/* vu0 routine: return the position in vf10 with homogeneous w = 1. */
void dds3GetPosPoseSet(dds3Basic_t *pBasic);

void dds3GetRotPoseSet(dds3Basic_t *pBasic);

/* vu0 routine: return scale in vf10. */
void func_0010DF70(dds3Basic_t *basic);

void dds3GetPosPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *offset);

void dds3AddRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);

void dds3GetSclPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *scale);

/* vu0 routine: compose scale, quaternion rotation and translation in vf28-31. */
void dds3MakeMatrix(const sdfVector4_t *scale, const sdfVector4_t *quaternion,
                    const sdfVector4_t *transrate);

/* Load the cached matrix, rebuilding it only when a pose setter invalidated it. */
void dds3GetTMPoseSet(dds3Basic_t *pBasic);

s32 dds3ComparePosPoseSet(dds3PoseSet_t *left, dds3PoseSet_t *right);

/* vu0 routine: distance between the two positions, against the right radius. */
s32 dds3ComparePosPoseSet2(dds3PoseSet_t *left, dds3PoseSet_t *right);

s32 dds3CallBasicMainProc(dds3Basic_t *pBasic);

s32 dds3CallBasicDrawProc(dds3Basic_t *basic);

u32 dds3AllocObjectId(void);

void dds3SetUnitObjectId(dds3Basic_t *basic, char *name);

char *func_0010E270(dds3Basic_t *basic);

#endif
