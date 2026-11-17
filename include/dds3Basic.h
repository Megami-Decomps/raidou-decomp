#ifndef DDS3BASIC_H
#define DDS3BASIC_H

#include "common.h"
#include "basic.h"
#include "sdf_types.h"

void dds3AddRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);
dds3Basic_t *dds3AllocBasic(u32 type);
void dds3BackupPoseSet(dds3PoseSet_t *pPoseSet);
s32 dds3CallBasicDrawProc(dds3Basic_t *basic);
s32 dds3CallBasicMainProc(dds3Basic_t *pBasic);
s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);
s32 dds3ComparePosPoseSet2(dds3PoseSet_t *left, dds3PoseSet_t *right);
void dds3DeleteBasic(dds3Basic_t *basic);
void dds3FreePoseSet(dds3Basic_t *basic);
void dds3GetTMPoseSet(dds3Basic_t *pBasic);
void dds3GetPosPoseSet(dds3Basic_t *pBasic);
void dds3GetPosPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *offset);
void dds3GetRadiusPoseSet(dds3Basic_t *basic, f32 r);
void dds3GetSclPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *scale);
s32 dds3InitPoseSet(dds3Basic_t *basic);
void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);
void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);
void dds3SetSclPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *scl);

void dds3GetSclPoseSet(dds3Basic_t *pBasic);
void dds3MakeMatrix(const sdfVector4_t *scale, const sdfVector4_t *quaternion, const sdfVector4_t *transrate);
u32 dds3AllocObjectId(void);
void dds3SetPosPoseSet_dds3Basic(dds3Basic_t *pBasic, const sdfVector4_t *pos);
void dds3SetUnitObjectId(dds3Basic_t *basic, char *name);
extern u32 dds3ObjectIdCounter;

#endif
