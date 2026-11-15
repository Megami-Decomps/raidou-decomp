#include "common.h"
#include "raidou_alloc.h"
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

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);
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
void dds3DeleteUnitObjectBasic(dds3Basic_t *aPtr);
s32 dds3GetStatusUnitObjectBasic(dds3Basic_t *aPtr);
struct evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *b);
void dds3UnitObjectBasicSetOt(dds3Basic_t *aPtr, s32 idx);
extern void dds3InitPoseSet(dds3Basic_t *basic);
extern void *sdfChipAllocDirect(u32 size);
extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);
void dds3UnitObjectBasicDestructor(dds3Basic_t *aPtr);
extern void dds3UnitObjectBasicEL_Link(dds3Basic_t *basic);
extern const f32 dds3UnitAdditionalRotation[4];
extern const f32 dds3UnitWobbleAxis[4];
void dds3UnitObjectBasicEL_Link(dds3Basic_t *unitBasic);
extern const f32 dds3UnitAdditionalRotation[4] __attribute__((aligned(16)));

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
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

#include "dds/eff_object.h"

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

typedef struct EffVec4 {
    f32 v[4];
} EffVec4;

struct WorldIndexNode;

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

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3UnitObjectBasicSetKubiModori);

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3ClaimSoundSlot);

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3CtnGetSub);

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

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3UnitObjectBasicMainProcedure);

INCLUDE_ASM(const s32, "basic/dds3UnitObjectBasic", dds3CallMemset2);

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

