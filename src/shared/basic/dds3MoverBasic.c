#include "common.h"
#include "basic.h"
#include "dds3obj.h"
#include "ps2asm.h"
#include "sdfChip.h"
#include "sdk_libc.h"
#include "dds3SceneBasic.h"
#include "dds3Basic.h"
#include "dds3PathBasic.h"
#include "dds3LightObjectBasic.h"
#include "dds3ObjectBase.h"
#include "dds3MoverBasic.h"

/* The matter object caches its path position at +0x20, independently of pose. */
typedef struct
{
    u8 unk00[0x20];
    sdfVector4_t position;
} dds3MatterPathPosition_t;


extern s32 dds3MoverCount;
extern char dds3MoverNameTable[10][16];
extern u32 dds3AllocObjectId(void);
void dds3MoverBasicFreePath(dds3Basic_t *pMoverBasic);
s32 MoverBasicMainprocuder(dds3Basic_t *pBasic);
s32 MoverBasicConstructor(dds3Basic_t *pBasic);
void MoverBasicDestructor(dds3Basic_t *pBasic);
dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *pObjectBasic);
void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *mover, dds3BasicControlProc_t func);
void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic);
void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic);

s32 MoverBasicConstructor(dds3Basic_t *pBasic)
{
    dds3MoverBasicWorkData_t *work;

    work = sdfChipAllocDirect(sizeof(dds3MoverBasicWorkData_t));
    pBasic->workdata = work;
    memset(work, 0, sizeof(dds3MoverBasicWorkData_t));
    return 1;
}

void MoverBasicDestructor(dds3Basic_t *pBasic)
{
    dds3MoverBasicWorkData_t *work;

    work = pBasic->workdata;
    dds3MoverBasicFreePath(pBasic);
    dds3ObjectBaseSetControlBasicType(work->pObjectBasic, NULL, 1);
    sdfChipFreeDirect(work);
}

/* Paths supply absolute channels; a primary callback supplies relative pose. */
s32 MoverBasicMainprocuder(dds3Basic_t *pBasic)
{
    sdfVector4_t pathVector;
    fldResourceLightPath_t light;
    dds3PoseSet_t relativePose;
    dds3MoverBasicWorkData_t *work = pBasic->workdata;
    dds3Basic_t *object = work->pObjectBasic;
    dds3PoseSet_t *pose = object->poseset;
    dds3BasicControlProc_t callback;
    dds3CameraObjectBasicWorkData_t *camera;
    f32 fovy;

    if (work->pPathControl != NULL) {
        if (work->pPathControl->flag & DDS3_PATHBASIC_FLAG_POS) {
            dds3PathBasicControlGetPos(work->pPathControl);
            SDF_VU0_STOREVECTOR(&pathVector);
            dds3SetPosPoseSet(object, &pathVector);
            if (object->type >> 24 == BASIC_TYPE_MATTEROBJECT) {
                SDF_VU0_COPYVECTOR(&((dds3MatterPathPosition_t *)object->workdata)->position, &pathVector);
            }
        }
        if (work->pPathControl->flag & DDS3_PATHBASIC_FLAG_ROT) {
            dds3PathBasicControlGetRot(work->pPathControl);
            SDF_VU0_STOREVECTOR(&pathVector);
            dds3SetRotPoseSet(object, &pathVector);
        }
        if (work->pPathControl->flag & DDS3_PATHBASIC_FLAG_FOVY) {
            if (object->type >> 24 == BASIC_TYPE_CAMERAOBJECT) {
                fovy = dds3PathBasicControlGetFovy(work->pPathControl);
                camera = object->workdata;
                camera->fovy = fovy;
                camera->flag |= 1;
            }
        }
        if (work->pPathControl->flag & DDS3_PATHBASIC_FLAG_LIGHT) {
            if (object->type >> 24 == BASIC_TYPE_LIGHTOBJECT) {
                dds3PathBasicControlNextStep(work->pPathControl, &light);
                dds3SetLightObjectPathResoruce(object, &light);
            }
        }
        dds3PathBasicControlIsFinished(work->pPathControl);
    } else {
        callback = work->pFunc;
        pose->frameFall = 0;
        if (callback != NULL) {
            dds3BackupPoseSet(&relativePose);
            if (callback(&relativePose, object) == 1) {
                dds3GetSclPoseSetMCR(object, &relativePose.scl);
                dds3AddRotPoseSet(object, &relativePose.rot);
                dds3GetPosPoseSetMCR(object, &relativePose.pos);
            }
        }
    }
    return 1;
}

dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *pObjectBasic)
{
    dds3Basic_t *basic;
    dds3MoverBasicWorkData_t *work;

    basic = dds3AddSceneBasicData(BASIC_TYPE_MOVER);
    work = basic->workdata;
    basic->serialID = dds3AllocObjectId();
    work->pObjectBasic = pObjectBasic;
    basic->name = dds3MoverNameTable[dds3MoverCount];
    dds3MoverCount++;
    dds3MoverCount %= 10;
    return basic;
}

void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *mover, dds3BasicControlProc_t func)
{
    dds3MoverBasicWorkData_t *work = mover->workdata;

    work->pFunc = func;
}

void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic)
{
    dds3MoverBasicWorkData_t *work = pMoverBasic->workdata;

    work->pPathBasic = pPathBasic;
}

void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic)
{
    dds3MoverBasicWorkData_t *work = pMoverBasic->workdata;

    if (work->pPathControl != NULL) {
        dds3DeletePathControl(work->pPathControl);
    }
    work->pPathControl = dds3AllocPathBasicWorkData(work->pPathBasic);
}

void dds3MoverBasicFreePath(dds3Basic_t *pMoverBasic)
{
    dds3MoverBasicWorkData_t *work = pMoverBasic->workdata;

    if (work->pPathControl != NULL) {
        dds3DeletePathControl(work->pPathControl);
        work->pPathControl = NULL;
    }
}

s32 dds3MoverCount = 0;
