#include "common.h"
#include "raidou_alloc.h"
#include "sdfFrame.h"
#include "basic.h"
#include "model.h"
#include "motion.h"
#include "event.h"
#include "field.h"
#include "ps2asm.h"
#include "dds3EffectObjectBasic.h"
#include "dds3LightObjectBasic.h"
#include "dds3CameraObjectBasic.h"
#include "dds3ObjectBase.h"
#include "dds3ModelBasic.h"
#include "dds3MoverBasic.h"
#include "mdlManager.h"
#include "sdfModel.h"
#include "sdfQuat.h"
#include "evtUnitManager.h"
#include "dds3Basic.h"
#include "dds3AreaBasic.h"
#include "dds3WorldBasic.h"
#include "sdfChip.h"
#include "sdfMotion.h"
#include "dds3SceneBasic.h"
#include "kernel_draw.h"

extern dds3ObjectBaseControlWorkData_t *dds3EventObjectBasicGetControlWorkData(dds3Basic_t *pBasic);

typedef struct
{
    char name[20];
    char parts[17][20];
} dds3UnitPartTable_t;

#include "sdk_libc.h"

typedef struct
{
    dds3ObjectBaseControlWorkData_t *control;
    s32 hojiID;
} dds3ObjectBaseMatterWork_t;

s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, dds3BasicControlProc_t func);
u32 BasicTypeToItemType(u32 type);
dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *pMoverBasic);
dds3Basic_t *dds3ObjectBaseGetControlBasic(dds3Basic_t *pObjectBasic, s32 itemType);
dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic);
dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType);
void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);
void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, s32 flag);
u8 dds3ObjectBaseCheckFlag(dds3Basic_t *pObjectBasic, u32 flag);
dds3BasicArrayHandle_t *dds3ObjectBaseGetEventArray(dds3Basic_t *pObjectBasic);
void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);
void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);
void dds3ObjectBaseSetPadControl(dds3Basic_t *pObjectBasic, dds3BasicControlProc_t pFuncPadControl);
void dds3ObjectBaseMakePath(dds3Basic_t *pObjectBasic, dds3Basic_t *pPathBasic);
void dds3ObjectBaseFreePath(dds3Basic_t *pObjectBasic);
dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(dds3Basic_t *pObjectBasic);

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3ObjectBase.c:110, near, same function 0.97 (words 37/35, calls 1.00, shape 0.97) */
dds3ObjectBaseControlWorkData_t *dds3ObjectBaseInitControlWorkData(dds3Basic_t *pObjectBasic)
{
    dds3ObjectBaseControlWorkData_t *control;
    s32 index;

    control = sdfChipAllocDirect(sizeof(dds3ObjectBaseControlWorkData_t));
    memset(control, 0, sizeof(dds3ObjectBaseControlWorkData_t));
    control->modelType = 3;
    control->hEventBasicArray = dds3GetBasicArrayHandle(0);
    control->pBasic[0] = pObjectBasic;
    for (index = 0; index < ARRAY_COUNT(control->modelParts); index++) {
        control->modelParts[index] = -1;
    }
    control->kubinodeidx = -1;
    return control;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3ObjectBase.c:126, near, same function 1.00 (words 49/49, calls 1.00, shape 1.00) */
void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *pControl)
{
    dds3Basic_t *owner;
    dds3Basic_t *area;
    s32 index;

    owner = pControl->pBasic[0];
    area = dds3ObjectBaseGetControlBasic(owner, 4);
    if (area != NULL) {
        dds3AreaBasicGetItemArray(area, owner);
    }
    for (index = 0; index < ARRAY_COUNT(pControl->pBasic); index++) {
        if (index < 3) {
            if (index > 0) {
                if (pControl->pBasic[index] != NULL) {
                    dds3DeleteSceneBasicData(pControl->pBasic[index]);
                }
            }
        }
    }
    dds3ObjectBaseFreeModelHandle(owner);
    if (pControl->pModelRangeRootNode != NULL) {
        sdfModelRootFree(pControl->pModelRangeRootNode, 1, 1);
    }
    dds3FreeBasicArrayHandle(pControl->hEventBasicArray);
    sdfChipFreeDirect(pControl);
}

void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(basic);
    control->flag |= mask;
}

void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, s32 flag)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(pObjectBasic);
    control->flag &= ~flag;
}

u8 dds3ObjectBaseCheckFlag(dds3Basic_t *pObjectBasic, u32 flag)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(pObjectBasic);
    return (control->flag & flag) != 0;
}

void *dds3ObjectBaseSetUserValue(dds3Basic_t *pObjectBasic, void *pUserValue)
{
    dds3ObjectBaseControlWorkData_t *control;
    void *previous;

    control = dds3GetBaseControlWorkData(pObjectBasic);
    previous = dds3ObjectBaseGetUserValue(pObjectBasic);
    control->pUserValue = pUserValue;
    return previous;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3ObjectBase.c:187, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
void *dds3ObjectBaseGetUserValue(dds3Basic_t *pObjectBasic)
{
    return dds3GetBaseControlWorkData(pObjectBasic)->pUserValue;
}

dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic)
{
    if (pItemBasic == NULL) {
        return NULL;
    }
    return dds3ObjectBaseSetControlBasicType(pObjectBasic, pItemBasic,
                                             BasicTypeToItemType(pItemBasic->type >> 24));
}

dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType)
{
    dds3Basic_t *previous;

    previous = dds3ObjectBaseGetControlBasic(pObjectBasic, itemType);
    dds3GetBaseControlWorkData(pObjectBasic)->pBasic[itemType] = pItemBasic;
    return previous;
}

dds3Basic_t *dds3ObjectBaseGetControlBasic(dds3Basic_t *pObjectBasic, s32 itemType)
{
    return dds3GetBaseControlWorkData(pObjectBasic)->pBasic[itemType];
}

dds3BasicArrayHandle_t *dds3ObjectBaseGetEventArray(dds3Basic_t *pObjectBasic)
{
    return dds3GetBaseControlWorkData(pObjectBasic)->hEventBasicArray;
}

void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic)
{
    return dds3GetBaseControlWorkData(pObjectBasic)->pModel;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3ObjectBase.c:226, match, same function 1.00 (words 38/38, calls 1.00, shape 1.00) */
void dds3ObjectBaseFreeModelHandle(dds3Basic_t *pObjectBasic)
{
    dds3ObjectBaseControlWorkData_t *control;
    dds3UnitObjectBasicWorkData_t *work;

    control = dds3GetBaseControlWorkData(pObjectBasic);
    if (control->pModel != NULL) {
        if (control->modelType != 1) {
            if (control->modelType == 0) {
                mdlFree(control->pModel);
                work = pObjectBasic->workdata;
                if (work->hUnit != NULL) {
                    evtDeleteUnitHandle(work->hUnit);
                    work->hUnit = NULL;
                }
            }
        } else {
            sdfModelRootFree(control->pModel, 1, 1);
            sdfMotionCtrlrFree(control->pMot);
        }
        control->pModel = NULL;
        control->modelType = 3;
    }
}

/* NON_MATCHING: 59 of 65 words match. The address register for the two lqc2 loads is v1, retail uses v0. */
void dds3ObjectBaseMakeUnit(dds3Basic_t *pObjectBasic, MODELTYPE major, s32 minor)
{
    sdfVector4_t scale;
    dds3ObjectBaseControlWorkData_t *control;
    dds3UnitObjectBasicWorkData_t *work;
    mdlObj_t *obj;

    control = dds3GetBaseControlWorkData(pObjectBasic);
    if (control->pModel != NULL) {
        mdlFree(control->pModel);
    }
    obj = mdlLoad(major, minor);
    control->kubinodeidx = sdfModelGetNodeIndexByName(obj->inner, "Bip01 Head");
    mdlGetScale(obj);
    SDF_VU0_STOREVECTOR(&scale);
    dds3SetSclPoseSet(pObjectBasic, &scale);
    SDF_VU0_LOADVECTOR(&pObjectBasic->poseset->rot);
    Normalize();
    mdlSetRotate(obj);
    SDF_VU0_LOADVECTOR(&pObjectBasic->poseset->pos);
    SDF_VU0_VECTOR_W_SET_ONE();
    mdlSetPosition(obj);
    pObjectBasic->poseset->r = obj->objectRadius;
    control->pModel = obj;
    work = pObjectBasic->workdata;
    control->modelType = 0;
    control->cam_len = 0.0f;
    if (work->hUnit == NULL) {
        work->hUnit = evtCreateUnitHandle(pObjectBasic, obj);
    }
}

/* NON_MATCHING: 35 of 49 words match.  */
#ifdef NON_MATCHING
/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3ObjectBase.c:855, rework, same function 1.00 (words 49/49, calls 1.00, shape 1.00) */
void dds3ObjectBaseMakeMatter(dds3Basic_t *pObjectBasic)
{
    sdfMotionCtrlr_t *motion;
    sdfModelRootNode_t *model;
    dds3Basic_t *modelBasic;
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(pObjectBasic);
    modelBasic = dds3ObjectBaseGetControlBasic(pObjectBasic, 3);
    model = dds3ModelBasicMakeModel(modelBasic);
    motion = dds3ModelBasicSetMotion(modelBasic, model);
    dds3GetTMPoseSet(pObjectBasic);
    SDF_VU0_STOREMATRIX(&model->tm);
    sdfModelEvalTm2(model, 0);
    sdfModelEvalTm2(model, 1);
    control->pModel = model;
    control->pMot = motion;
    control->modelType = 1;
    control->MotSpeed = 1.0f;
    control->cam_len = 0.0f;
    control->MotMode = 0;
}
#else
INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseMakeMatter);
#endif

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3ObjectBase.c:879, match, same function 1.00 (words 45/45, calls 1.00, shape 1.00) */
void dds3ObjectBaseMatterAnimeSpeed(dds3Basic_t *pObjectBasic, u32 mode)
{
    dds3ObjectBaseControlWorkData_t *control = dds3GetBaseControlWorkData(pObjectBasic);

    switch (mode) {
    case 0:
        control->MotMode = 0;
        control->MotSpeed = 1.0f;
        break;
    case 1:
        control->MotMode = 1;
        control->MotSpeed = 0.0f;
        break;
    case 2:
        control->MotMode = 2;
        control->MotSpeed = 0.0f;
        break;
    case 3:
        control->MotMode = 3;
        control->MotSpeed = 0.0f;
        break;
    case 4:
        control->MotMode = 4;
        control->MotSpeed = 1.0f;
        break;
    case 5:
        control->MotMode = 5;
        control->MotSpeed = 1.0f;
        break;
    case 6:
        control->MotMode = 6;
        control->MotSpeed = 0.0f;
        break;
    }
}

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3ObjectBase.c:915, near, same function 1.00 (words 16/16, calls 1.00, shape 1.00) */
s32 dds3ObjectBaseRegistArea(dds3Basic_t *pObjectBasic)
{
    dds3Basic_t *area;

    area = dds3ObjectBaseGetControlBasic(pObjectBasic, 4);
    if (area == NULL) {
        return 0;
    }
    dds3AreaBasicRegistItem(area, pObjectBasic);
    return 1;
}

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseModelLinkOtSvs);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3CallSetOt2);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", itfCampStartProcess);

#endif
/* NON_MATCHING: 44 of 56 words match.  */
#ifdef NON_MATCHING
f32 dds3CallMemset(dds3Basic_t *pObjectBasic, sdfModelRootNode_t *model_root)
{
    sdfVector4_t mark = { 0.0f, 0.0f, 0.0f, 1.0f };
    f32 dx, dy, dz;

    if (sdfModelGetMarkPos_sdfModel(model_root, 0) != 0) {
        SDF_VU0_STOREVECTOR(&mark);
        dx = aViewEye.x - mark.x;
        dy = aViewEye.y - mark.y;
        dz = aViewEye.z - mark.z;
    } else {
        dx = aViewEye.x - pObjectBasic->poseset->pos.x;
        dy = aViewEye.y - (pObjectBasic->poseset->pos.y - 150.0f);
        dz = aViewEye.z - pObjectBasic->poseset->pos.z;
    }
    return sdfSqrt(dx * dx + dy * dy + dz * dz);
}
#else
INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3CallMemset);
#endif

#ifdef VERSION_RAIDOU1
void dds3ObjectBaseModelLinkOt(sdfModelOt_t *pModelOt, dds3Basic_t *pObjectBasic)
{
    dds3ObjectBaseControlWorkData_t *control;
    dds3UnitObjectBasicWorkData_t *work;
    struct sdf3DLightParallelPacket_t *light;
    s32 minor;

    control = dds3GetBaseControlWorkData(pObjectBasic);
    if (dds3ObjectBaseCheckFlag(pObjectBasic, 1) != 0) {
        return;
    }
    if (control->pModel == NULL) {
        return;
    }
    if (control->modelType != 1) {
        if (control->modelType != 0) {
            return;
        }
        if (dds3ObjectBaseCheckFlag(pObjectBasic, 0x800) != 0) {
            dds3ObjectBaseSetControlBasicCamLen(pObjectBasic);
            if (control->cam_len < 100.0f) {
                mdlLink_NoDraw(control->pModel, pModelOt);
                return;
            }
        }
        if (dds3ObjectBaseCheckFlag(pObjectBasic, 0x100000) != 0) {
            mdlLink_NoDraw(control->pModel, pModelOt);
            return;
        }
        light = ((dds3UnitObjectBasicWorkData_t *)pObjectBasic->workdata)->hUnit->lightPacket;
        if (light != NULL) {
            ((mdlObj_t *)control->pModel)->inner->light = light;
        } else {
            ((mdlObj_t *)control->pModel)->inner->light = NULL;
        }
        if (dds3ObjectBaseCheckFlag(pObjectBasic, 0x2000) != 0) {
            work = pObjectBasic->workdata;
            mdlLink_Kubi(control->pModel, pModelOt, control->kubinodeidx, work->kubiPitch, work->kubiYaw);
            return;
        }
        if (pModelOt != &ModelOt[13]) {
            sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
        } else {
            sdf3DSetCamera(&DrawEnv, &aProjMatrix);
        }
        if (pModelOt == &ModelOt[0] && mdlGetMajorNum(control->pModel) == 1) {
            minor = mdlGetMinorNum(control->pModel);
            if (minor == 19 || minor == 99) {
                mdlLink(control->pModel, &pModelOt[1]);
                return;
            }
        }
        if (mdlGetMajorNum(control->pModel) == 2) {
            minor = mdlGetMinorNum(control->pModel);
            if (minor == 13 || minor == 29) {
                mdlLink(control->pModel, &ModelOt[8]);
                return;
            }
        }
        mdlLink(control->pModel, pModelOt);
        return;
    }
    if ((pObjectBasic->color >> 24) != 0) {
        if (control->pMot != NULL) {
            switch (control->MotMode) {
            case 0:
                control->MotSpeed = 1.0f;
                break;
            case 1:
                control->MotSpeed += SDF_FRAME_STEP(0.05f);
                if (1.0f < control->MotSpeed) {
                    control->MotSpeed = 1.0f;
                }
                break;
            case 2:
                control->MotSpeed += SDF_FRAME_STEP(0.025f);
                if (1.0f < control->MotSpeed) {
                    control->MotSpeed = 1.0f;
                }
                break;
            case 3:
                control->MotSpeed = 0.0f;
                break;
            case 4:
                control->MotSpeed -= SDF_FRAME_STEP(0.05f);
                if (control->MotSpeed < 0.0f) {
                    control->MotSpeed = 0.0f;
                }
                /* fallthrough */
            case 5:
                control->MotSpeed -= SDF_FRAME_STEP(0.02f);
                if (control->MotSpeed < 0.0f) {
                    control->MotSpeed = 0.0f;
                }
                break;
            }
            if (0.0f < control->MotSpeed) {
                control->pMot->speed = control->MotSpeed;
                sdfMotionNext(control->pMot);
            }
            sdfModelEvalTm(control->pModel);
        }
        ((sdfModelRootNode_t *)control->pModel)->color = pObjectBasic->color;
        sdfModelLinkOt_sdfModel(pModelOt->ot, control->pModel);
    }
}
#else
INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseModelLinkOt);
#endif

void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic)
{
    if (dds3ObjectBaseGetControlBasic(pObjectBasic, 1) == NULL) {
        dds3ObjectBaseSetControlBasic(pObjectBasic, dds3AddMoverBasic(pObjectBasic));
    }
}

void dds3ObjectBaseSetPadControl(dds3Basic_t *pObjectBasic, dds3BasicControlProc_t pFuncPadControl)
{
    dds3ObjectBaseSetPrimaryControl(pObjectBasic, pFuncPadControl);
    /* Retail dds3ObjectBaseSetPadControl keeps JAL plus epilogue.
     * This consumed-callback reset is removed in 03.cse,
     * after the sibling-call decision (01.sibling / 02.jump). */
    pFuncPadControl = NULL;
}

s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, dds3BasicControlProc_t func)
{
    dds3Basic_t *mover;

    mover = dds3ObjectBaseGetControlBasic(basic, 1);
    if (mover == NULL) {
        return 0;
    }
    dds3MoverBasicSetCallBackPrimaryControl(mover, func);
    return 1;
}

void dds3ObjectBaseMakePath(dds3Basic_t *pObjectBasic, dds3Basic_t *pPathBasic)
{
    dds3MoverBasicSetPath(dds3ObjectBaseGetControlBasic(pObjectBasic, 1), pPathBasic);
    dds3MoverBasicMakePath(pObjectBasic);
}

void dds3ObjectBaseFreePath(dds3Basic_t *pObjectBasic)
{
    dds3MoverBasicFreePath(dds3ObjectBaseGetControlBasic(pObjectBasic, 1));
}

dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(dds3Basic_t *pObjectBasic)
{
    return dds3MoverBasicGetPathControl(dds3ObjectBaseGetControlBasic(pObjectBasic, 1));
}

dds3ObjectBaseControlWorkData_t *dds3GetBaseControlWorkData(dds3Basic_t *pObjectBasic)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = NULL;
    switch ((pObjectBasic->type >> 24) - BASIC_TYPE_CAMERAOBJECT) {
    case 0: control = dds3GetCameraObjectBaseControlWorkData(pObjectBasic); break;
    case 1: control = dds3UnitObjectBasicGetControlWorkData(pObjectBasic); break;
    case 2: control = dds3MatterObjectBasicGetControlWorkData(pObjectBasic); break;
    case 3: control = dds3EffectObjectBasicGetControlWorkData(pObjectBasic); break;
    case 4: control = dds3EventObjectBasicGetControlWorkData(pObjectBasic); break;
    case 5: control = dds3LightObjectBasicGetControlWorkData(pObjectBasic); break;
    }
    return control;
}
