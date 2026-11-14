#include "common.h"
#include "raidou_alloc.h"

/* Declarations from smt3-decomp src/smt3/basic/dds3UnitObjectBasic.c */
#include "common.h"

#include "basic.h"

#include "dds3obj.h"

#include "event.h"

#include "model.h"

#include "ps2asm.h"

/* Declared here because no header carries these. */
extern void *memset(void *a, s32 b, size_t c);

typedef struct {
    u8 unk00[0x64];
    u32 flag;
} dds3LinkedLightWork_t;

extern dds3Basic_t *dds3AddSceneBasicData(s32 type);

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);

extern void dds3SetRadiusPoseSet(dds3PoseSet_t *pose);

extern dds3Basic_t *dds3AreaBasicGetFastArea(void);

extern void dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType);

extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *pObjectBasic);

extern void dds3FreePoseSet(dds3Basic_t *basic);

extern void evtDeleteUnitHandle(struct evtUnit_t *h);

extern void dds3ObjectBaseFreeModelHandle(dds3Basic_t *pObjectBasic);

extern void sdfChipFreeDirect(void *aPtr);

extern void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *unitbasic, s32 frame);

extern void dds3LinkLightObjectToUnitObject(dds3Basic_t *lightbasic, dds3Basic_t *unitbasic, s32 frame);

extern dds3Basic_t *dds3GetControlScene(void);

extern dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *pBasic, s32 type);

extern u16 dds3GetBasicArrayCount(dds3BasicArrayHandle_t *hndl);

extern void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *hndl);

extern s32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *hndl);

extern dds3Basic_t *dds3GetBasicArray(dds3BasicArrayHandle_t *hndl);

extern s32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *hndl);

extern s32 dds3ComparePosPoseSet2(dds3PoseSet_t *first, dds3PoseSet_t *second);

void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *unitBasic);

dds3ObjectBaseControlWorkData_t *dds3UnitObjectBasicGetControlWorkData(dds3Basic_t *pBasic);

dds3Basic_t *dds3AddUnitObjectBasic(s32 serialID, void *aPos, void *aRot);

void dds3DeleteUnitObjectBasic(dds3Basic_t *aPtr);

s32 dds3GetStatusUnitObjectBasic(dds3Basic_t *aPtr);

struct evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *b);

void dds3UnitObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx);

extern void dds3InitPoseSet(dds3Basic_t *basic);

extern void *sdfChipAllocDirect(u32 size);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);

s32 dds3UnitObjectBasicConstructor(dds3Basic_t *basic);

void dds3UnitObjectBasicDestructor(dds3Basic_t *aPtr);

extern s32 evtUnitManagerPrimaryFunc(dds3PoseSet_t *pose, dds3Basic_t *basic);

extern void dds3BackupPoseSet(dds3PoseSet_t *pose);

extern void dds3GetSclPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *scale);

extern void dds3AddRotPoseSet(dds3Basic_t *basic, const sdfVector4_t *rotation);

extern void dds3GetPosPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *position);

extern s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pose, u32 flag);

extern void dds3OffPoseSetFlag(dds3PoseSet_t *pose, u32 flag);

extern s32 mdlGetMajorNum(mdlObj_t *model);

extern s32 mdlGetMinorNum(mdlObj_t *model);

extern void mdlSetScale(mdlObj_t *model);

extern void mdlSetRotate(mdlObj_t *model);

extern void mdlSetPosition(mdlObj_t *model);

extern f32 sdfSin(f32 angle);

extern void FromRotation(f32 angle);

extern void Mult(void);

extern void Normalize(void);

extern u8 dds3ObjectBaseCheckFlag(dds3Basic_t *basic, u32 flag);

extern void dds3UnitObjectBasicEL_Link(dds3Basic_t *basic);

extern const f32 dds3UnitAdditionalRotation[4];

extern const f32 dds3UnitWobbleAxis[4];

s32 dds3UnitObjectBasicMainProcedure(dds3Basic_t *basic);

extern sdfModelOt_t sdfModelOtTable[];

extern u8 dds3ObjectBaseCheckFlag(dds3Basic_t *basic, u32 flag);

extern void dds3ObjectBaseModelLinkOt(sdfModelOt_t *ot, dds3Basic_t *basic);

extern void fldSdwDraw(dds3Basic_t *basic);

extern void dds3DrawUnitObjectShadow(s32 alpha, f32 x, f32 y, f32 z, f32 radius);

extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *root, s32 mark);

extern s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pose, u32 flag);

s32 dds3UnitObjectBasicDrawProcedure(dds3Basic_t *basic);

void dds3UnitObjectBasicEL_LinkFree(dds3Basic_t *unitBasic);

/* Detach a light when leaving its radius, then select the first eligible light. */
void dds3UnitObjectBasicEL_Link(dds3Basic_t *unitBasic);

extern const f32 dds3UnitAdditionalRotation[4] __attribute__((aligned(16)));

extern const f32 dds3UnitWobbleAxis[4];
/* End of declarations from src/smt3/basic/dds3UnitObjectBasic.c */

/* Declarations from dds-decomp src/dds1/game/code_001130E0.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_dds3LinkedLightWork_t 1

#include "basic.h"

#include "dds/bill_object_api.h"

#include "dds/dds3_object_flags.h"

#include "dds/dds3obj.h"

#include "dds/eff_event.h"

#include "dds/eff_event_sound.h"

#include "dds/eff_node.h"

#include "dds/eff_transform.h"

#include "dds/sdf_model.h"

#include "dds/sdf_trig.h"

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

void dds3BillboardDelete(struct billHandle_t *obj);

void dds3EffectDelete(EffNode *node);

void dds3GetPosPoseSet(dds3Basic_t *object);

void dds3GetRotPoseSet(dds3Basic_t *object);

void *dds3ObjectBaseGetModelHandle(dds3Basic_t *object);

void dds3ObjectBaseOffFlag(struct dds3Basic_t *object, u32 mask);

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *world, u32 key, s32 kind);

void effEveBEDCutDelete(effEveBEDCutHandle_t *work);

void effEveBEDDelete(struct SoundMixer *mixer);

/* Angles are in radians; cosine applies a pi/2 shift to the sine input. */
f32 sdfAtan2(f32 y, f32 x);

/* Refresh the current per-node transforms from the model's selected frame. */
void sdfModelEvalTm(dds_sdfModelRootNode_t *model);

#include "dds/sdk_ps2.h"

#include "dds/mdl.h"

#include "dds/common.h"

#include "dds/sdf_map_position_api.h"

#include "dds/sdf_vu0.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_model.h"

#include "dds/sdf_chunk.h"

#include "dds/sdf_chip.h"

#include "dds/dds3obj.h"

#include "dds/dds3_basic_array_cursor.h"

#include "dds/dds3_object_flags.h"

#include "dds/eff_dependency.h"

#include "dds/pcp_vu0.h"

#include "dds/eff_object.h"

#include "dds/evt_world_source_transform.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_motion_step_result.h"

#include "dds/eff.h"

#include "dds/eff_node.h"

#include "dds/eff_event.h"

#include "dds/eff_event_sound.h"

#include "dds/btl_sound.h"

#include "dds/bill_object_api.h"

#include "dds/effect_value_object.h"

#include "dds/fld_actor_motion_step_result_api.h"

extern u32 effObjOpacityPassEnabled;

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

#include "dds/dds3_world_secondary.h"

/* Return the signed shortest turn from one degree angle to another.
 * The cast before modulo intentionally discards fractional degrees. */
f32 dds3ShortestAngleDelta(f32 fromDegrees, f32 toDegrees);

extern f32 EulerAngleAxisY(void);

#define EFFECT_HEADING_DEGREES_PER_RADIAN_APPROX 57.32484055f

#define EFFECT_HEADING_RADIANS_TO_DEGREES 57.29577637f

void effObjStepFollowAngleTowardPosition(dds3Basic_t *obj, const f32 *targetPosition);

void effObjStepFollowAngleTowardZero(dds3Basic_t *object);

void evtArmEffectObjectPendingValue(dds3Basic_t *obj, s32 value);

void evtResetObjectPendingValue(dds3Basic_t *obj);

s32 effObjInitializeFollowModelData(dds3Basic_t *object);

typedef struct EffVec4 {
    f32 v[4];
} EffVec4;

extern EffVec4 D_0039F720;

extern EffVec4 D_0039F730;

extern EvtUnitMotionStepResult evtUpdateUnitMotion(dds3PoseSet_t *, dds3Basic_t *);

extern void effObjStepFollowAngleTowardZero(dds3Basic_t *object);

extern void effObjStepFollowAngleTowardPosition(dds3Basic_t *obj, const f32 *targetPosition);

extern void *dds3GetWorldNodeData(dds3Basic_t *node);

/* Per-frame refresh of a model effect object: rebuild the child transform from the follow record (a tilt that wobbles with its angle), then run the timed callbacks. */
s32 effUpdateFollowModelTransform(dds3Basic_t *obj);

extern void effObjApplyTransformParameters(sdfGrOt_t **, dds3Basic_t *);

extern void fldDrawEffectObjectShadow(dds3Basic_t *);

extern sdfGrOt_t *kwlnSceneSurfaceSets[13][4];

struct WorldIndexNode;

dds3Basic_t *dds3AddMatterObjectBasic(s32 key, f32 *offset, f32 *rotation);

void dds3DeleteMatterObjectBasic(dds3Basic_t *node);

void dds3MatterObjectBasicSetOt(dds3Basic_t *object, s32 activeId);

void dds3MatterObjectBasicSetHojiID(dds3Basic_t *obj, u32 roomNumber);

u32 dds3MatterObjectBasicGetHojiID(u32 id);

#include "dds/dds3_object_payload.h"

/* Payload word 8 is interpreted by object kind, not always as a pointer. */
void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *object, u32 value);

u32 dds3MatterObjectBasicGetAlpMode(const dds3Basic_t *object);

void effObjSetOpacityPassEnabled(u32 value);

s32 dds3MatterObjectBasicConstructor(dds3Basic_t *object);

void dds3MatterObjectBasicDestructor(dds3Basic_t *object);

extern void dds3GetTMPoseSet(dds3Basic_t *object);

s32 effUpdateTransformOpacityMode(dds3Basic_t *object);

extern void rsetOtOfs(s32 index);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern dds3Basic_t *fldPlayerObject;

/* Submit the normal pass and the opacity-mode pass, temporarily neutralizing
 * the tint while the player is hidden. */
s32 effObjSubmitTransformOpacityPasses(dds3Basic_t *object);

/* Refresh the object's stored xyz from the source vector. */
void dds3RefreshStoredVec3(dds3Basic_t *object);

/* Release each dependency according to the active state, clearing ownership
 * before releasing the next dependency. State 4 only borrows its handle. */
void FreeEffectData(EffectDependencyState *state);

s32 dds3EffectObjectBasicConstructor(dds3Basic_t *obj);
/* End of declarations from src/dds1/game/code_001130E0.c */

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3AddUnitObjectBasic);

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

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", func_r1_0011A970);

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", func_r1_0011AA70);

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", func_r1_0011AAB8);

s32 effObjInitializeFollowModelData(dds3Basic_t *object) {
    EffectObjectData *data;
    void *work;

    dds3InitPoseSet(object);
    work = sdfChipAllocDirect(sizeof(EffectObjectData));
    object->workdata = work;
    memset(work, 0, sizeof(EffectObjectData));
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

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", func_0011ABF8);

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", func_r1_0011AED0);

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

INCLUDE_RODATA(const s32, "basic/dds3UnitObjectBasic", D_r1_00446450);

INCLUDE_RODATA(const s32, "basic/dds3UnitObjectBasic", D_r1_00446460);

