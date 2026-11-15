#include "common.h"
#include "raidou_alloc.h"
#include "basic.h"
#include "dds3obj.h"
#include "sdfChip.h"
#include "dds3SceneBasic.h"
#include "dds3Basic.h"

/* The event constructor allocates a complete 16-byte payload. */
typedef struct
{
    dds3ObjectBaseControlWorkData_t *pObjectBaseControlWorkData;
    u8 unk04[12];
} dds3EventObjectBasicWorkData_t;

u32 EventObjectBasicConstructor(dds3Basic_t *pBasic);
void EventObjectBasicDestructor(dds3Basic_t *pBasic);
s32 EventObjectBasicMainProcuder(dds3Basic_t *pBasic);

dds3Basic_t *dds3AddEventObjectBasic(s32 serialID, const sdfVector4_t *pos,
                                   const sdfVector4_t *rot);

u32 EventObjectBasicConstructor(dds3Basic_t *pBasic)
{
    dds3EventObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;

    dds3InitPoseSet(pBasic);
    work = sdfChipAllocDirect(sizeof(dds3EventObjectBasicWorkData_t));
    pBasic->workdata = work;
    control = dds3ObjectBaseInitControlWorkData(pBasic);
    work->pObjectBaseControlWorkData = control;
    return 1;
}

void EventObjectBasicDestructor(dds3Basic_t *pBasic)
{
    dds3EventObjectBasicWorkData_t *work;

    dds3FreePoseSet(pBasic);
    work = pBasic->workdata;
    dds3ObjectBaseFreeControlWorkData(work->pObjectBaseControlWorkData);
    sdfChipFreeDirect(work);
}

s32 EventObjectBasicMainProcuder(dds3Basic_t *pBasic)
{
    dds3PoseSet_t *pose;

    pose = pBasic->poseset;
    if (dds3CheckPoseSetFlag(pose, POSESET_FLAG_UPDATEPOSE) == 1) {
        dds3OffPoseSetFlag(pose, POSESET_FLAG_UPDATEPOSE);
        dds3SetRadiusPoseSet(pose);
    }
    return 1;
}

dds3Basic_t *dds3AddEventObjectBasic(s32 serialID, const sdfVector4_t *pos,
                                   const sdfVector4_t *rot)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_EVENTOBJECT);

    basic->serialID = serialID;
    dds3SetPosPoseSet(basic, pos);
    dds3SetRotPoseSet(basic, rot);
    dds3SetRadiusPoseSet(basic->poseset);
    return basic;
}
