/* Declarations of smt3-decomp src/smt3/basic/dds3EventObjectBasic.c, for its bodies in this directory. */
#ifndef SHARED_BASIC_DDS3EVENTOBJECTBASIC_H
#define SHARED_BASIC_DDS3EVENTOBJECTBASIC_H

#include "common.h"

#include "basic.h"

#include "dds3obj.h"

/* The event constructor allocates a complete 16-byte payload. */
typedef struct
{
    dds3ObjectBaseControlWorkData_t *pObjectBaseControlWorkData;
    u8 unk04[12];
} dds3EventObjectBasicWorkData_t;

extern s32 dds3InitPoseSet(dds3Basic_t *basic);

extern void dds3FreePoseSet(dds3Basic_t *basic);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

extern void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

extern void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

u32 EventObjectBasicConstructor(dds3Basic_t *pBasic);

void EventObjectBasicDestructor(dds3Basic_t *pBasic);

s32 EventObjectBasicMainProcuder(dds3Basic_t *pBasic);

dds3Basic_t *dds3AddEventObjectBasic(s32 serialID, const sdfVector4_t *pos,
                                   const sdfVector4_t *rot);

dds3ObjectBaseControlWorkData_t *dds3EventObjectBasicGetControlWorkData(dds3Basic_t *pBasic);

#endif
