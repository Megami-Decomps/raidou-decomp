#include "common.h"
#include "raidou_alloc.h"
#include "basic.h"
#include "dds3obj.h"
#include "event.h"
#include "model.h"
#include "ps2asm.h"
#include "sdfChip.h"
#include "sdk_libc.h"
#include "dds3SceneBasic.h"
#include "dds3Basic.h"
#include "evtUnitManager.h"
#include "dds3LightObjectBasic.h"
#include "dds3WorldBasic.h"
#include "dds3ObjectBase.h"
#include "dds3AreaBasic.h"
#include "dds3PointBasic.h"
#include "mdlManager.h"
#include "sdfQuat.h"
#include "sdfMath.h"
#include "fldDraw.h"

/* Declared here because no header carries these. */

typedef struct {
    u8 unk00[0x64];
    u32 flag;
} dds3LinkedLightWork_t;

void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *unitBasic);
void dds3DeleteUnitObjectBasic(dds3Basic_t *aPtr);
s32 dds3GetStatusUnitObjectBasic(dds3Basic_t *aPtr);
struct evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *b);
void dds3UnitObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx);
void dds3UnitObjectBasicDestructor(dds3Basic_t *aPtr);
extern void dds3UnitObjectBasicEL_Link(dds3Basic_t *basic);
extern const f32 dds3UnitAdditionalRotation[4];
extern const f32 dds3UnitWobbleAxis[4];
void dds3UnitObjectBasicEL_Link(dds3Basic_t *unitBasic);
extern const f32 dds3UnitAdditionalRotation[4] __attribute__((aligned(16)));
#ifdef VERSION_RAIDOU1

#endif

/* Sparse world-node identities recovered from the paired operation tables and
 * concrete creators. Unlisted values stay unnamed; COUNT remains the full
 * directory extent rather than an enum-completeness claim. */
enum {
    EFF_WORLD_KIND_WORLD_INFO = 0,
    EFF_WORLD_KIND_EVENT_TABLE = 1,
    EFF_WORLD_KIND_WORLD_INDEX_OWNER = 2,
    EFF_WORLD_KIND_SLOT_RING = 3,
    EFF_WORLD_KIND_CAMERA = 4,
    EFF_WORLD_KIND_FOLLOW_MODEL = 5,
    EFF_WORLD_KIND_EFFECT_TRANSFORM = 6,
    EFF_WORLD_KIND_EFFECT_OBJECT = 7,
    EFF_WORLD_KIND_RESOURCE_OWNER = 8,
    EFF_WORLD_KIND_LIGHT = 9,
    EFF_WORLD_KIND_SCRIPT_OBJECT = 10,
    EFF_WORLD_KIND_ACTION_B = 11,
    EFF_WORLD_KIND_ACTION_D = 13,
    EFF_WORLD_KIND_ACTION_10 = 16,
    EFF_WORLD_KIND_TRANSFORM_SOURCE = 17,
    EFF_WORLD_KIND_COUNT = 18
};

enum {
    OBJECT_TRANSFORM_FLAG_UPDATE_PENDING = 1,
    OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID = 2,
    OBJECT_TRANSFORM_FLAG_USE_SMOOTHED_POSITION = 8
};

#include "include_asm.h"
#include "dds3UnitObjectBasic.h"
#include "sdfModel.h"

dds3ObjectBaseControlWorkData_t *dds3CreateSlotResourceState(dds3Basic_t *owner);

/* Mode 0 is the initialized neutral tint; other values describe the native
 * alpha updates performed by the paired transform renderer. */
enum {
    EFFECT_OPACITY_MODE_INITIAL = 0,
    EFFECT_OPACITY_MODE_ALPHA_CLEAR = 2,
    EFFECT_OPACITY_MODE_ALPHA_RISE_8 = 3,
    EFFECT_OPACITY_MODE_ALPHA_FALL_8_TO_32 = 4,
    EFFECT_OPACITY_MODE_ALPHA_RISE_4 = 5,
    EFFECT_OPACITY_MODE_ALPHA_FALL_4 = 6,
    EFFECT_OPACITY_MODE_ALPHA_RISE_2 = 7,
    EFFECT_OPACITY_MODE_ALPHA_FALL_2 = 8,
    EFFECT_OPACITY_MODE_ALPHA_FALL_11 = 9,
    EFFECT_OPACITY_MODE_ALPHA_FADE_START = EFFECT_OPACITY_MODE_ALPHA_RISE_8,
    EFFECT_OPACITY_MODE_ALPHA_FADE_END = EFFECT_OPACITY_MODE_ALPHA_RISE_4,
    EFFECT_OPACITY_ALPHA_MAX = 0x80,
    EFFECT_OPACITY_FALL_8_FLOOR = 0x20,
    EFFECT_OPACITY_COLOR_NEUTRAL = 0x80808080,
    EFFECT_OPACITY_COLOR_TRANSPARENT = 0x00808080
};

f32 dds3ShortestAngleDelta(f32 fromDegrees, f32 toDegrees);

#define EFFECT_HEADING_DEGREES_PER_RADIAN_APPROX 57.32484055f

#define EFFECT_HEADING_RADIANS_TO_DEGREES 57.29577637f

s32 effObjInitializeFollowModelData(dds3Basic_t *object);

typedef struct effVec4_t {
    f32 v[4];
} effVec4_t;


dds3Basic_t *dds3AddUnitObjectBasic(u32 serialID, const sdfVector4_t *aPos, const sdfVector4_t *aRot)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_UNITOBJECT);
    dds3UnitObjectBasicWorkData_t *work;

    if (basic != NULL) {
        basic->serialID = serialID;
        dds3ObjectBaseCreateMover(basic);
        dds3SetPosPoseSet(basic, aPos);
        dds3SetRotPoseSet(basic, aRot);
        dds3SetRadiusPoseSet(basic->poseset);
        work = basic->workdata;
        work->status = -1;
        work->pWorkData = 0;
        dds3ObjectBaseSetControlBasicType(basic, dds3AreaBasicGetFastArea(), 4);
        dds3ObjectBaseRegistArea(basic);
        return basic;
    }
    return NULL;
}

void dds3DeleteUnitObjectBasic(dds3Basic_t *aPtr) {
    dds3DeleteSceneBasicData(aPtr);
}

s32 dds3GetStatusUnitObjectBasic(dds3Basic_t *aPtr) {
    return ((dds3UnitObjectBasicWorkData_t *)aPtr->workdata)->status;
}

struct evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *b) {
    return ((dds3UnitObjectBasicWorkData_t *)b->workdata)->hUnit;
}

void dds3UnitObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx) {
    ((dds3UnitObjectBasicWorkData_t *)aPtr->workdata)->ot = idx;
}

/* Return the signed shortest turn from one degree angle to another.
 * The cast before modulo intentionally discards fractional degrees. */
f32 dds3ShortestAngleDelta(f32 fromDegrees, f32 toDegrees) {
    f32 diff;

    if (fromDegrees < 0.0f || toDegrees < 0.0f) {
        fromDegrees += 360.0f;
        toDegrees += 360.0f;
    }
    fromDegrees = (s32)fromDegrees % 360;
    toDegrees = (s32)toDegrees % 360;
    diff = fromDegrees - toDegrees;
    if (diff > 180.0f || diff < -180.0f) {
        if (fromDegrees < toDegrees) {
            fromDegrees += 360.0f;
        } else {
            toDegrees += 360.0f;
        }
    }
    return toDegrees - fromDegrees;
}

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3UnitObjectBasicSetKubiTerget);

void dds3UnitObjectBasicSetKubiModori(dds3Basic_t *object) {
    effObjectData_t *data = object->workdata;
    f32 angle;
    f32 value;
    f32 step;

    if (data->angleReturnDelayFrames > 0) {
        data->angleReturnDelayFrames--;
    } else {
        angle = data->angle;
        if (angle < 0.1f && -0.1f < angle) {
            dds3ObjectBaseOffFlag(object, 0x2000);
        } else {
            value = dds3ShortestAngleDelta(angle, 0.0f);
            step = 0.5f;
            if ((0.0f <= value && value <= step) ||
                (value <= 0.0f && -step <= value)) {
                value = 0.0f;
            } else {
                if (value < 0.0f) {
                    value = angle - step;
                } else {
                    value = angle + step;
                }
            }
            data->angle = value;
            data->word34 = 0;
        }
    }
}

void dds3UnitObjectBasicKubiOn(dds3Basic_t *pBasic, s32 posid)
{
    dds3UnitObjectBasicWorkData_t *work = pBasic->workdata;

    dds3ObjectBaseOnFlag(pBasic, 0x2000);
    if (work->kubiTargetID != posid) {
        work->kubiTargetID = posid;
        work->word20 = 0;
    }
}

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3UnitObjectBasicKubiOff);

#ifdef VERSION_RAIDOU1
s32 effObjInitializeFollowModelData(dds3Basic_t *object) {
    effObjectData_t *data;
    void *work;

    dds3InitPoseSet(object);
    work = sdfChipAllocDirect(sizeof(effObjectData_t));
    object->workdata = work;
    memset(work, 0, sizeof(effObjectData_t));
    data = object->workdata;
    data->modelHolder = dds3CreateSlotResourceState(object);
    data->transitionWork = NULL;
    data->activeId = -1;
    data->followParameterIndex = -1;
    data->pendingTargetKey = 0;
    data->angleReturnDelayFrames = 0;
    data->angle = 0.0f;
    data->limitMin2C = -45.0f;
    data->limitMax30 = 45.0f;
    data->word34 = 0;
    data->limitMin38 = -45.0f;
    data->limitMax3C = 45.0f;
    dds3ObjectBaseOnFlag(object, 0x42);
    return 1;
}
#else
INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3UnitMVSBtlUnitSub);

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", Constructor_dds3UnitObjectBasic);
#endif

void dds3UnitObjectBasicDestructor(dds3Basic_t *aPtr) {
    dds3UnitObjectBasicWorkData_t *work;

    dds3UnitObjectBasicEL_LinkFree(aPtr);
    dds3FreePoseSet(aPtr);
    work = aPtr->workdata;
    if (work->status != -1) {
        work->status = -1;
    }
    if (work->hUnit != NULL) {
        evtDeleteUnitHandle(work->hUnit);
        work->hUnit = NULL;
    }
    dds3ObjectBaseFreeModelHandle(aPtr);
    dds3ObjectBaseFreeControlWorkData(work->pObjectBaseControlWorkData);
    sdfChipFreeDirect(aPtr->workdata);
}

#ifdef VERSION_RAIDOU1
s32 dds3UnitObjectBasicMainProcedure(dds3Basic_t *basic)
{
    dds3UnitObjectBasicWorkData_t *work;
    mdlObj_t *model;
    sdfVector4_t rotationAxis = *(const sdfVector4_t *)dds3UnitWobbleAxis;
    dds3PoseSet_t callbackPose;
    s32 deform = 0;

    work = basic->workdata;
    if (work->hUnit != NULL) {
        dds3BackupPoseSet(&callbackPose);
        if (evtUnitManagerPrimaryFunc(&callbackPose, basic) != 0) {
            dds3GetSclPoseSetMCR(basic, &callbackPose.scl);
            dds3AddRotPoseSet(basic, &callbackPose.rot);
            dds3GetPosPoseSetMCR(basic, &callbackPose.pos);
        }
    }

    model = work->pObjectBaseControlWorkData->pModel;
    if (model != NULL && dds3CheckPoseSetFlag(basic->poseset, 1) == 1) {
        dds3OffPoseSetFlag(basic->poseset, 1);

        if (work->hUnit != NULL) {
            if (work->hUnit->motionState == 0 || work->hUnit->motionState == 3) {
                u32 motionFlag = work->hUnit->flags & 0x40;
                deform = motionFlag != 0;
            }
            if (work->hUnit->flags & 0x400000) {
                deform = 1;
            }
        }

        SDF_VU0_LOADVECTOR(&basic->poseset->scl);
        if (work->hUnit != NULL && deform) {
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADIDENTITYVECTOR();
            SDF_VU0_VECTOR_W_SET_ZERO();
            SDF_VU0_SETVECTOR_X(sdfSin(work->hUnit->wobblePhase) * 0.01f);
            SDF_VU0_SETVECTOR_Y(sdfSin(work->hUnit->wobblePhase) * 0.004f);
            SDF_VU0_MULVECTOR();
            SDF_VU0_ADDVECTOR();
        }
        mdlSetScale(model);

        SDF_VU0_LOADVECTOR(&basic->poseset->rot);
        Normalize();
        if (work->hUnit != NULL && deform) {
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(&rotationAxis);
            FromRotation(sdfSin(work->hUnit->wobblePhase) * 0.006f);
            Mult();
        }
        if (work->rotationFlags & 1) {
            SDF_VU0_LOADVECTOR_B(dds3UnitAdditionalRotation);
            Mult();
        }
        mdlSetRotate(model);

        if (dds3CheckPoseSetFlag(basic->poseset, 8) != 0) {
            SDF_VU0_LOADVECTOR(&basic->poseset->pos2);
            SDF_VU0_VECTOR_W_SET_ONE();
        } else {
            SDF_VU0_LOADVECTOR(&basic->poseset->pos);
            SDF_VU0_VECTOR_W_SET_ONE();
        }
        mdlSetPosition(model);
        dds3SetRadiusPoseSet(basic->poseset);
    }

    if (!dds3ObjectBaseCheckFlag(basic, 0x100)) {
        dds3UnitObjectBasicEL_Link(basic);
    }
    if (dds3ObjectBaseCheckFlag(basic, 0x2000)) {
        if (work->kubiTargetID != 0) {
            dds3UnitObjectBasicSetKubiTerget(basic, dds3GetWorldNodeData(dds3SearchSceneBasic(dds3GetControlScene(), work->kubiTargetID, 16)));
        } else {
            dds3UnitObjectBasicSetKubiModori(basic);
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", MainProcedure_dds3UnitObjectBasic);
#endif

s32 dds3UnitObjectBasicDrawProcedure(dds3Basic_t *basic) {
    sdfVector4_t worldPosition;
    dds3UnitObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;
    struct evtUnit_t *unit;
    mdlObj_t *model;
    s32 alpha;
    s32 shadow;

    memset(&worldPosition, 0, sizeof(worldPosition));
    worldPosition.w = 1.0f;
    if (dds3ObjectBaseCheckFlag(basic, 1)) {
        return 1;
    }

    unit = dds3GetUnitHandleObjectBasic(basic);
    work = basic->workdata;
    if (work->ot == -1) {
        dds3ObjectBaseModelLinkOt(&ModelOt[0], basic);
    } else {
        dds3ObjectBaseModelLinkOt(&ModelOt[work->ot], basic);
    }

    if (!dds3ObjectBaseCheckFlag(basic, 0x400)) {
        return 1;
    }
    if (dds3ObjectBaseCheckFlag(basic, 0x1000)) {
        return 1;
    }
    if (unit == NULL) {
        return 1;
    }

    control = dds3UnitObjectBasicGetControlWorkData(basic);
    model = control->pModel;
    if (dds3ObjectBaseCheckFlag(basic, 0x4000)) {
        alpha = unit->ShadowAlpha;
    } else {
        alpha = ((const u8 *)&model->inner->color)[3];
    }
    shadow = dds3ObjectBaseCheckFlag(basic, 0x8000) != 0;

    if (!dds3ObjectBaseCheckFlag(basic, 0x40000)) {
        if (!dds3ObjectBaseCheckFlag(basic, 0x80000)) {
            if (sdfModelGetMarkPos_sdfModel(unit->owner->inner, 0)) {
                SDF_VU0_STOREVECTOR(&worldPosition);
                worldPosition.y = shadow == 1 ? unit->ShadowY : basic->poseset->pos.y;
                if (dds3ObjectBaseCheckFlag(basic, 0x20000)) {
                    nbCallGetY(worldPosition.x, worldPosition.y, worldPosition.z, basic->poseset->r, alpha * 2 / 3);
                } else {
                    nbCallSetOt4(worldPosition.x, worldPosition.y, worldPosition.z, basic->poseset->r, alpha * 2 / 3);
                }
            } else {
                if (dds3CheckPoseSetFlag(basic->poseset, 8)) {
                    worldPosition.x = basic->poseset->pos2.x;
                    worldPosition.y = basic->poseset->pos2.y;
                    worldPosition.z = basic->poseset->pos2.z;
                } else {
                    worldPosition.x = basic->poseset->pos.x;
                    worldPosition.y = basic->poseset->pos.y;
                    worldPosition.z = basic->poseset->pos.z;
                }
                if (shadow == 1) {
                    worldPosition.y = unit->ShadowY;
                }
                nbCallSetOt4(worldPosition.x, worldPosition.y, worldPosition.z, basic->poseset->r, alpha * 2 / 3);
            }
        } else {
            if (dds3CallGetBaseControlWorkData(basic) < 1700.0f) {
                if (sdfModelGetMarkPos_sdfModel(unit->owner->inner, 601)) {
                    SDF_VU0_STOREVECTOR(&worldPosition);
                    worldPosition.y = shadow == 1 ? unit->ShadowY : basic->poseset->pos.y;
                    if (dds3ObjectBaseCheckFlag(basic, 0x20000)) {
                        nbCallGetY(worldPosition.x, worldPosition.y, worldPosition.z, 25.0f, 48);
                    } else {
                        nbCallSetOt4(worldPosition.x, worldPosition.y, worldPosition.z, 25.0f, 48);
                    }
                }
                if (sdfCallModelMakeMarkPos(unit->owner->inner, 601)) {
                    SDF_VU0_STOREVECTOR(&worldPosition);
                    worldPosition.y = shadow == 1 ? unit->ShadowY : basic->poseset->pos.y;
                    if (dds3ObjectBaseCheckFlag(basic, 0x20000)) {
                        nbCallGetY(worldPosition.x, worldPosition.y, worldPosition.z, 25.0f, 48);
                    } else {
                        nbCallSetOt4(worldPosition.x, worldPosition.y, worldPosition.z, 25.0f, 48);
                    }
                }
            }
            if (sdfModelGetMarkPos_sdfModel(unit->owner->inner, 600)) {
                SDF_VU0_STOREVECTOR(&worldPosition);
                worldPosition.y = shadow == 1 ? unit->ShadowY : basic->poseset->pos.y;
                if (dds3ObjectBaseCheckFlag(basic, 0x20000)) {
                    nbCallGetY(worldPosition.x, worldPosition.y, worldPosition.z, basic->poseset->r, 80);
                } else {
                    nbCallSetOt4(worldPosition.x, worldPosition.y, worldPosition.z, basic->poseset->r, 80);
                }
            } else {
                if (dds3CheckPoseSetFlag(basic->poseset, 8)) {
                    worldPosition.x = basic->poseset->pos2.x;
                    worldPosition.y = basic->poseset->pos2.y;
                    worldPosition.z = basic->poseset->pos2.z;
                } else {
                    worldPosition.x = basic->poseset->pos.x;
                    worldPosition.y = basic->poseset->pos.y;
                    worldPosition.z = basic->poseset->pos.z;
                }
                if (shadow == 1) {
                    worldPosition.y = unit->ShadowY;
                }
                nbCallSetOt4(worldPosition.x, worldPosition.y, worldPosition.z, basic->poseset->r, 80);
            }
        }
    } else {
        if (dds3CheckPoseSetFlag(basic->poseset, 8)) {
            worldPosition.x = basic->poseset->pos2.x;
            worldPosition.y = basic->poseset->pos2.y;
            worldPosition.z = basic->poseset->pos2.z;
        } else {
            worldPosition.x = basic->poseset->pos.x;
            worldPosition.y = basic->poseset->pos.y;
            worldPosition.z = basic->poseset->pos.z;
        }
        if (shadow == 1) {
            worldPosition.y = unit->ShadowY;
        }
        nbCallSetOt4(worldPosition.x, worldPosition.y, worldPosition.z, basic->poseset->r, alpha * 2 / 3);
    }
    return 1;
}

void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *unitBasic) {
    dds3UnitObjectBasicWorkData_t *work;

    work = unitBasic->workdata;
    if (work->EL_serialID != -1) {
        dds3UnlinkLightObjectFromUnitObject(unitBasic, 10);
        work->EL_serialID = -1;
    }
}

/* Detach a light when leaving its radius, then select the first eligible light. */
void dds3UnitObjectBasicEL_Link(dds3Basic_t *unitBasic) {
    dds3UnitObjectBasicWorkData_t *work = unitBasic->workdata;
    dds3Basic_t *scene = dds3GetControlScene();
    dds3BasicArrayHandle_t *list;
    dds3Basic_t *light;
    dds3LinkedLightWork_t *lightWork;

    if (scene == NULL) {
        return;
    }
    list = dds3SceneBasicGetList(scene, BASIC_TYPE_LIGHTOBJECT);
    if (list == NULL) {
        return;
    }
    if (dds3GetBasicArrayCount(list) == 0) {
        dds3FreeBasicArrayHandle(list);
        return;
    }
    if (dds3SetFirstBasicArray(list) != 0) {
        do {
            light = dds3GetBasicArray(list);
            lightWork = light->workdata;
            if (!(lightWork->flag & 4) &&
                dds3ComparePosPoseSet2(unitBasic->poseset, light->poseset) == 0 &&
                work->EL_serialID == light->serialID) {
                dds3UnlinkLightObjectFromUnitObject(unitBasic, 10);
                work->EL_serialID = -1;
            }
        } while (dds3SetNextBasicArray(list) != 0);
    }
    dds3FreeBasicArrayHandle(list);

    if (work->EL_serialID == -1) {
        list = dds3SceneBasicGetList(scene, BASIC_TYPE_LIGHTOBJECT);
        if (dds3SetFirstBasicArray(list) == 0) {
            goto freeList;
        }
        do {
            light = dds3GetBasicArray(list);
            lightWork = light->workdata;
            if (!(lightWork->flag & 4) &&
                dds3ComparePosPoseSet2(unitBasic->poseset, light->poseset) != 0) {
                goto linkLight;
            }
        } while (dds3SetNextBasicArray(list) != 0);

freeList:
        dds3FreeBasicArrayHandle(list);
        return;

linkLight:
        dds3LinkLightObjectToUnitObject(light, unitBasic, 10);
        work->EL_serialID = light->serialID;
        dds3FreeBasicArrayHandle(list);
    }
}

INCLUDE_RODATA(const s32, "basic/dds3UnitObjectBasic", dds3UnitAdditionalRotation);

INCLUDE_RODATA(const s32, "basic/dds3UnitObjectBasic", dds3UnitWobbleAxis);

