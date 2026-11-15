#include "common.h"
#include "raidou_alloc.h"
#include "kernel_draw.h"
#include "event.h"
#include "kernel.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "sdfChip.h"
#include "mdlManager.h"
#include "sdfQuat.h"
#include "dds3SceneBasic.h"
#include "evtUnitManager.h"
#include "dds3PathBasic.h"
#include "dds3WorldBasic.h"
#include "dds3Basic.h"

f32 GetSplineLength(dds3PathBasicControl_t *path);

typedef struct evtTargetInfo_t {
    f32 firstColor[4];
    f32 direction[4];
    u8 pad20[0x20];
    f32 secondColor[4];
    f32 nearDistance, farDistance;
    f32 auxFirst, auxSecond;
    u32 unk60;
    u32 flags;
} evtTargetInfo_t;

enum EvtTargetInfoFlags {
    EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR = 0x8
};

/* vu0 routine: unpack the unit's colours into a parallel light packet. */
void MakeLightPacket(evtUnit_t *h);
void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb);
void evtSetUnitLightDir(evtUnit_t *h, s32 frame);
void evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb);
void evtSetUnitColorA(evtUnit_t *h, s32 frame, u32 a);
void evtUnitLightOn(evtUnit_t *h);

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

evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit);
s32 evtDeleteUnitHandle(evtUnit_t *h);

typedef enum {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
} sdfMotionState_t;


#include "basic.h"
#include "dds3obj.h"

extern void evtSetUnitMove1(evtUnit_t *work, s32 motionSubmode, s128 *vector, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast);
s32 evtUnitRotationTarget(evtUnit_t *h, f32 *pos, f32 radio);
extern void evtGetUnitDirVector(evtUnit_t *h);
void evtVu0SubVectorXZAxis(f32 *a, f32 *b);
void evtGetUnitDirVector(evtUnit_t *h);
s32 evtCheckMoveEnd(evtUnit_t *h);
s32 evtGetNextMovePos(evtUnit_t *h);
extern f32 sdfFaceDivSoften, sdfFaceDivBias;

/* The colour inlines read and write a 16-byte slot, one word of it used. */
typedef struct {
    u32 word;
    u8 pad04[12];
} evtColorWork_t;

void evtUnlinkExternalLightPacket(evtUnit_t *h);
u8 evtUnitLightAnimeStat(evtUnit_t *h);
void evtUnitLightOff(evtUnit_t *h, s32 frame);
evtUnit_t *evtGetUnitHandle(s32 serial);
s32 evtGetUnitStat(evtUnit_t *h);
void evtSetWaitMotionSpeed(evtUnit_t *h, f32 sp);
s32 evtCheckUnitWait(evtUnit_t *h);
void evtSetUnitKutiMotion(evtUnit_t *h, s32 group, s32 motion);
void evtSetUnitMotion(evtUnit_t *h, s32 motion, s32 before, s32 smooth, s32 mode);
s32 evtUnitRotationQuat(evtUnit_t *h, f32 *rot, f32 radio);
s32 evtUnitMoveType0(evtUnit_t *h);
s32 evtUnitMoveTypePathTrace(evtUnit_t *h);
void evtSetUnitRot1(evtUnit_t *h, s128 *rot, s32 para1);
void evtSetUnitRot2(evtUnit_t *h, s32 posID, s32 para1);
void evtSetUnitMove1(evtUnit_t *work, s32 motionSubmode, s128 *vector, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast);
void evtSetUnitMove2(evtUnit_t *work, s32 motionSubmode, s32 worldObjectKey, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast);

#include "sdk_libc.h"

/* vf10 = (a - b) with y replaced by 0 */
void evtVu0SubVectorXZAxis(f32 *a, f32 *b) {
    SDF_VU0_LOADVECTOR(a);
    SDF_VU0_LOADVECTOR_B(b);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SETVECTOR_Y_V0(0.0f);
}

/* vf10 = the unit's rotated y axis flattened to the ground plane, negated */
void evtGetUnitDirVector(evtUnit_t *h) {
    sdfVector4_t v;

    SDF_VU0_LOADVECTOR(&h->effObj->poseset->rot);
    ToMatrix();
    SDF_VU0_STORE_ROW2(&v);
    v.y = 0.0f;
    v.w = 1.0f;
    SDF_VU0_LOADVECTOR(&v);
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR_V0(-1.0f);
}

/* vu0 routine: test target range or the selected path endpoint. */
s32 evtCheckMoveEnd(evtUnit_t *h) {
    f32 value;

    switch (h->motionSubmode) {
    case 0:
    case 1:
    case 2:
        evtVu0SubVectorXZAxis(h->targetVector, (f32 *)&h->effObj->poseset->pos);
        SDF_VU0_GETLENGTH(value);
        if (value <= 20.0f) {
            return 1;
        }
        break;
    case 3:
        value = dds3PathBasicControlGetRatio(EE_ADDR_AS(void *, h->pathHandle));
        if (h->flags & EVT_UNIT_FLAG_PATH_REVERSE) {
            if (value == 0.0f) {
                return 1;
            }
        } else if (value == 1.0f) {
            return 1;
        }
        break;
    }
    return 0;
}

s32 evtGetNextMovePos(evtUnit_t *h) {
    f32 t;

    if (h->motionState != EVT_UNIT_MOTION_STATE_SOURCE) {
        return 0;
    }
    switch (h->transitionSourceKind) {
    case EVT_UNIT_TRANSITION_SOURCE_COPIED_VECTOR:
    case EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE:
        break;
    case EVT_UNIT_TRANSITION_SOURCE_PATH_CURVE:
        t = dds3PathBasicControlGetRatio(EE_ADDR_AS(void *, h->pathHandle));
        if (h->flags & EVT_UNIT_FLAG_PATH_REVERSE) {
            if (t == 0.0f) {
                return 0;
            }
        } else {
            if (t == 1.0f) {
                return 0;
            }
        }
        t += SDF_FRAME_STEP(h->pathSpeed);
        if (t < 0.0f) {
            t = 0.0f;
        }
        if (1.0f < t) {
            t = 1.0f;
        }
        dds3PathBasicControlSetRatio(EE_ADDR_AS(dds3PathBasicControl_t *, h->pathHandle), t);
        dds3PathBasicControlGetPos(EE_ADDR_AS(dds3PathBasicControl_t *, h->pathHandle));
        SDF_VU0_STOREVECTOR(h->targetVector);
        return 1;
    }
    return 0;
}

/* Length of the path's vec4 trajectory sampled at 20 steps of the value multiplier. */
f32 GetSplineLength(dds3PathBasicControl_t *path) {
    f32 savedScaleFactor;
    f32 length = 0.0f;
    f32 step = 0.05f;
    f32 t = step;
    f32 segmentLength;

    savedScaleFactor = dds3PathBasicControlGetRatio((dds3PathBasicControl_t *)path);
    dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, 0.0f);
    dds3PathBasicControlGetPos(path);
    do {
        SDF_VU0_COPYVECTOR_A_TO_B();
        dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, t);
        dds3PathBasicControlGetPos(path);
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_SUBVECTOR();
        SDF_VU0_GETLENGTH(segmentLength);
        length += segmentLength;
        SDF_VU0_COPYVECTOR_C_TO_A();
        t += step;
    } while (t <= 1.0f);
    dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, savedScaleFactor);
    return length;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", UnitAnime);

INCLUDE_ASM(const s32, "event/evtUnitManager", GetBaseLightRGB);

INCLUDE_ASM(const s32, "event/evtUnitManager", GetBaseLightRGB_2);

INCLUDE_ASM(const s32, "event/evtUnitManager", GetBaseLightRGB2);

INCLUDE_ASM(const s32, "event/evtUnitManager", GetBaseAmbientRGB);

INCLUDE_ASM(const s32, "event/evtUnitManager", GetBaseLightDir);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitLightDir1Sub);

INCLUDE_ASM(const s32, "event/evtUnitManager", GetBaseLightDir2);

INCLUDE_ASM(const s32, "event/evtUnitManager", MakeLightPacket);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtLinkLightObjectBasic);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnlinkLightObjectBasic_evt);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtLinkExternalLightPacket);

void evtUnlinkExternalLightPacket(evtUnit_t *h) {
    h->flags = h->flags & ~EVT_UNIT_FLAG_VALUE_CHANGED;
    MakeLightPacket(h);
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitLightAndAmbient);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitLightAndAmbient3);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitLightDir);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitLightDir1);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitLightDir2);

void evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb) {
    h->rgbDuration = SDF_FRAMES(frame);
    h->rgbElapsed = 0;
    if (frame == 0) {
        mdlSetColor((mdlObj_t *)h->owner,
            (mdlGetColor((mdlObj_t *)h->owner) & 0xFF000000) | (rgb & 0xFFFFFF));
        h->color60 = (h->color60 & 0xFF000000) | (rgb & 0xFFFFFF);
        h->flags &= ~EVT_UNIT_FLAG_RGB_TRANSITION;
    } else {
        u32 currentRgb = mdlGetColor((mdlObj_t *)h->owner) & 0xFFFFFF;

        h->color60 = (h->color60 & 0xFF000000) | currentRgb;
        h->color64 = (h->color64 & 0xFF000000) | (rgb & 0xFFFFFF);
        h->flags |= EVT_UNIT_FLAG_RGB_TRANSITION;
    }
}

void evtSetUnitColorA(evtUnit_t *h, s32 frame, u32 a) {
    h->alphaDuration = SDF_FRAMES(frame);
    h->alphaElapsed = 0;
    if (frame == 0) {
        mdlSetColor((mdlObj_t *)h->owner,
            (mdlGetColor((mdlObj_t *)h->owner) & 0xFFFFFF) | (a & 0xFF000000));
        h->color60 = (h->color60 & 0xFFFFFF) | (a & 0xFF000000);
        h->flags &= ~EVT_UNIT_FLAG_ALPHA_TRANSITION;
    } else {
        u32 currentAlpha = mdlGetColor((mdlObj_t *)h->owner) & 0xFF000000;

        h->color60 = (h->color60 & 0xFFFFFF) | currentAlpha;
        h->color64 = (h->color64 & 0xFFFFFF) | (a & 0xFF000000);
        h->flags |= EVT_UNIT_FLAG_ALPHA_TRANSITION;
    }
}

u8 evtUnitLightAnimeStat(evtUnit_t *h)
{
    return (h->flags & 0x7800) != 0;
}

void evtUnitLightOn(evtUnit_t *h)
{
    h->flags = h->flags | EVT_UNIT_FLAG_USE_UNIT_COLOR_CHANNELS;
}

void evtUnitLightOff(evtUnit_t *h, s32 frame) {
    if (frame == 0) {
        h->flags &= ~0x700;
        h->flags &= ~0x5000;
        h->flags &= ~0x2800;
    } else {
        evtSetUnitLightAndAmbient(h, frame, 0, 0);
        h->flags = (h->flags & ~0x800) | 0x1000;
        SDF_VU0_LOADIDENTITYVECTOR();
        evtSetUnitLightDir(h, frame);
        h->flags = (h->flags & ~0x2000) | 0x4000;
    }
}

evtUnit_t *evtGetUnitHandle(s32 serial) {
    dds3Basic_t *obj = dds3SearchSceneBasic(dds3GetActiveScene(), serial, EFF_WORLD_KIND_FOLLOW_MODEL);

    if (obj != NULL) {
        return ((dds3UnitObjectBasicWorkData_t *)obj->workdata)->hUnit;
    }
    return NULL;
}

evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit) {
    if (unit == NULL) {
        return NULL;
    }
    return ((dds3UnitObjectBasicWorkData_t *)unit->workdata)->hUnit;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtCreateUnitHandle);

s32 evtDeleteUnitHandle(evtUnit_t *h) {
    void *endpointWork;

    if (h == NULL) {
        return 1;
    }
    endpointWork = h->endpointWork;
    h->owner->inner->light = 0;
    if (endpointWork != NULL) {
        sdfChipFreeDirect(endpointWork);
        h->endpointWork = NULL;
    }
    if (h->pathHandle != 0) {
        dds3DeletePathControl(EE_ADDR_AS(dds3PathBasicControl_t *, h->pathHandle));
        h->pathHandle = 0;
    }
    sdfChipFreeDirect(h);
    return 1;
}

s32 evtGetUnitStat(evtUnit_t *h) {
    return h->motionState;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitStat);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetWaitMotion);

void evtSetWaitMotionSpeed(evtUnit_t *h, f32 sp) {
    h->transitionMotionScale = sp;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetWaitHokanFrame);

s32 evtCheckUnitWait(evtUnit_t *h) {
    s32 state = evtGetUnitStat(h);

    if (state == EVT_UNIT_MOTION_STATE_IDLE) {
        return 1;
    }
    if (state == EVT_UNIT_MOTION_STATE_MOTION && h->motionTicks > 0 &&
        h->owner->first->stat == SDF_MOTION_STATE_TERMINAL) {
        return 1;
    }
    return 0;
}

void evtSetUnitKutiMotion(evtUnit_t *h, s32 group, s32 motion) {
    h->firstSlot = group;
    h->secondSlot = motion;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtRunUnitKutiMotion);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitExMotion);

void evtSetUnitMotion(evtUnit_t *h, s32 motion, s32 before, s32 smooth, s32 mode) {
    h->flags &= ~0x1;
    h->flags &= ~0x20;
    h->flags &= ~0x40;
    h->flags &= ~0x400000;
    h->flags &= ~0x800000;
    h->motionState = EVT_UNIT_MOTION_STATE_MOTION;
    (&h->motionSetup)->motionId = motion;
    (&h->motionSetup)->startFrame = before;
    (&h->motionSetup)->endFrame = smooth;
    h->flags |= 0x80;
    h->motionTicks = 0;
    h->motionCallback = 0;
    switch (mode) {
    case 0:
        h->flags |= 0x20;
        break;
    case 1:
        h->flags |= 0x1;
        break;
    case 2:
        break;
    case 3:
        h->flags |= 0x40;
        break;
    }
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitRotationTarget_evt);

/* Flat, negated direction of the rotation in quat, offset by the effect object's point, aimed with evtRotateUnitTowardTarget. */
s32 evtUnitRotationQuat(evtUnit_t *h, f32 *rot, f32 radio) {
    sdfVector4_t v;

    SDF_VU0_LOADVECTOR(rot);
    ToMatrix();
    SDF_VU0_GET_ROW2();
    VU0_SET_VF10_Y_CLEAR_W(0.0f);
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR_V0(-1.0f);
    SDF_VU0_LOADVECTOR_B(&h->effObj->poseset->pos);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&v);
    return evtUnitRotationTarget(h, &v.x, radio);
}

s32 evtUnitMoveType0(evtUnit_t *h) {
    sdfVector4_t v;
    f32 scale;
    dds3Basic_t *obj;

    evtUnitRotationTarget(h, h->targetVector, h->directionOffset * 0.01f);
    if (h->directionOffset != 0) {
        evtGetUnitDirVector(h);
        obj = h->effObj;
    } else {
        SDF_VU0_LOADVECTOR(h->targetVector);
        obj = h->effObj;
        SDF_VU0_LOADVECTOR_B(&obj->poseset->pos);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
    }
    scale = h->motionParameter * 0.1f;
    SDF_VU0_SCALEVECTOR_V0(scale);
    SDF_VU0_STOREVECTOR(&v);
    dds3GetPosPoseSetMCR(obj, &v);
    return 1;
}

INCLUDE_RODATA(const s32, "event/evtUnitManager", evtCreateUnitHandleTable);

INCLUDE_RODATA(const s32, "event/evtUnitManager", evtCreateUnitHandleTable2);

INCLUDE_RODATA(const s32, "event/evtUnitManager", evtCreateUnitHandleTable3);

INCLUDE_RODATA(const s32, "event/evtUnitManager", evtUnitRotationTarget_evtTable);

INCLUDE_RODATA(const s32, "event/evtUnitManager", evtUnitRotationTarget_evtTable2);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtCalcFrame_UnitMoveType0);

s32 evtUnitMoveTypePathTrace(evtUnit_t *h) {
    sdfVector4_t v;

    dds3PathBasicControlIsFinished(EE_ADDR_AS(dds3PathBasicControl_t *, h->pathHandle));
    dds3PathBasicControlGetPos(EE_ADDR_AS(dds3PathBasicControl_t *, h->pathHandle));
    SDF_VU0_STOREVECTOR(&v);
    dds3SetPosPoseSet(h->effObj, (u128 *)&v);
    if (h->flags & 0x10) {
        dds3PathBasicControlGetRot(EE_ADDR_AS(dds3PathBasicControl_t *, h->pathHandle));
        SDF_VU0_COPYVECTOR_A_TO_B();
        FromEuler(0.0f, 3.14159265f, 0.0f);
        Mult();
        SDF_VU0_STOREVECTOR(&v);
        dds3SetRotPoseSet(h->effObj, (u128 *)&v);
    }
    return 1;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitMovePos);

void evtSetUnitRot1(evtUnit_t *h, s128 *rot, s32 para1) {
    if (para1 > 0 && para1 <= 100) {
        h->linkedUnit = NULL;
        h->motionState = EVT_UNIT_MOTION_STATE_VECTOR;
        SDF_VU0_COPYVECTOR(h->targetVector, rot);
        h->motionParameter = para1;
        h->directionOffset = 0;
        h->motionCallback = 0;
        h->motionTicks = 0;
    }
}

#ifdef NON_MATCHING
/* retail codegen differs in the null-check and call order */
void evtSetUnitRot2(evtUnit_t *h, s32 posID, s32 para1) {
    dds3Basic_t *worldUnit;
    evtWorldSourceTransformPrefix_t *source;

    worldUnit = dds3SearchSceneBasic(dds3GetControlScene(), posID, 0x11);
    if (worldUnit != NULL) {
        source = worldUnit->workdata;
        evtSetUnitRot1(h, (s128 *)&source->rotation, para1);
        h->linkedUnit = worldUnit;
    }
}
#else
INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitRot2);
#endif

void evtSetUnitMove1(evtUnit_t *work, s32 motionSubmode, s128 *vector, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast) {
    work->motionSubmode = motionSubmode;
    work->motionState = EVT_UNIT_MOTION_STATE_SOURCE;
    work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_COPIED_VECTOR;
    work->linkedUnit = NULL;
    SDF_VU0_COPYVECTOR(work->targetVector, vector);
    work->motionParameter = frames;
    work->directionOffset = directionOffset;
    work->motionCallback = callback;
    work->motionTicks = 0;
}

#ifdef NON_MATCHING
/* retail codegen differs in the null-check and call order */
void evtSetUnitMove2(evtUnit_t *work, s32 motionSubmode, s32 worldObjectKey, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast) {
    dds3Basic_t *worldUnit;
    evtWorldSourceTransformPrefix_t *source;

    worldUnit = dds3SearchSceneBasic(dds3GetControlScene(), worldObjectKey, 0x11);
    if (worldUnit != NULL) {
        source = worldUnit->workdata;
        evtSetUnitMove1(work, motionSubmode, (s128 *)&source->position, unused, frames, directionOffset, callback, unusedLast);
        work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE;
        work->linkedUnit = worldUnit;
    }
}
#else
INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitMove2);
#endif

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitMove3);

INCLUDE_ASM(const s32, "event/evtUnitManager", CheckMoveEndBustup);

INCLUDE_ASM(const s32, "event/evtUnitManager", UnitMoveTypeBustup);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtLeaf);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitAction);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitManagerPrimaryFunc);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtInitCustomSceneLight);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetCustomSceneLightData);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetCustomSceneLightData_SoftenAndBias);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtLeaf4);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtMakeCustomSLightPacket);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtGetCustomSLight_SoftenAndBias);
