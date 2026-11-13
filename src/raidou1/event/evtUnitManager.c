#include "common.h"

/* Declarations from smt3-decomp src/smt3/event/evtUnitManager.c */
#include "common.h"

#include "event.h"

#include "kernel.h"

#include "ps2asm.h"

#include "sdfFrame.h"

f32 dds3PathBasicControlGetRatio(const dds3PathBasicControl_t *pControl);

void dds3PathBasicControlSetRatio(dds3PathBasicControl_t *pControl, f32 ratio);

void dds3PathBasicControlGetPos(dds3PathBasicControl_t *pControl);

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

/* The fallback ambient vector follows the four source pointers at +0x10. */
typedef struct {
    f32 *sources[4];
    f32 ambient[4];
} evtLightParams_t;

extern evtLightParams_t dds3LightParam;

void evtLinkLightObjectBasic(evtUnit_t *h, dds3Basic_t *light, s32 frame);

void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb);

void evtSetUnitLightDir(evtUnit_t *h, s32 frame);

void evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb);

u32 mdlGetColor(mdlObj_t *aHandle);

void mdlSetColor(mdlObj_t *aHandle, u32 aColor);

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

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

struct dds3Basic_t *dds3GetActiveScene(void);

evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit);

void sdfChipFreeDirect(void *aPtr);

void dds3DeletePathControl(dds3PathBasicControl_t *path);

s32 evtDeleteUnitHandle(evtUnit_t *h);

void evtSetWaitMotion(evtUnit_t *h, s32 motion);

typedef enum {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
} sdfMotionState_t;

void dds3PathBasicControlGetRot(dds3PathBasicControl_t *pControl);

s32 dds3PathBasicControlIsFinished(dds3PathBasicControl_t *path);

#include "basic.h"

#include "dds3obj.h"

void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

void Mult(void);

struct dds3Basic_t *dds3GetControlScene(void);

extern void evtSetUnitMove1(evtUnit_t *work, s32 motionSubmode, s128 *vector, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast);

dds3PathBasicControl_t *dds3AllocPathBasicWorkData(struct dds3Basic_t *object);

void ToMatrix(void);

s32 evtUnitRotationTarget(evtUnit_t *h, f32 *pos, f32 radio);

void dds3GetPosPoseSetMCR(dds3Basic_t *object, u128 *vector);

extern void evtGetUnitDirVector(evtUnit_t *h);

/* vf10 = (a - b) with y replaced by 0 */
void evtVu0SubVectorXZAxis(f32 *a, f32 *b);

/* vf10 = the unit's rotated y axis flattened to the ground plane, negated */
void evtGetUnitDirVector(evtUnit_t *h);

/* vu0 routine: test target range or the selected path endpoint. */
s32 evtCheckMoveEnd(evtUnit_t *h);

s32 evtGetNextMovePos(evtUnit_t *h);

/* Length of the path's vec4 trajectory sampled at 20 steps of the value multiplier. */
f32 GetSplineLength(dds3PathBasicControl_t *path);

extern f32 sdfFaceDivSoften, sdfFaceDivBias;

extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *model, s32 id);

extern void mdlGetPosition(mdlObj_t *model);

extern s32 dds3SceneBasicExistBasicFromType(dds3Basic_t *scene, s32 type, dds3Basic_t *target);

extern void dds3RotationArc(void);

extern void Lerp(f32 ratio);

extern void sdfModelSetFaceDivParm(sdfModelRootNode_t *model, f32 soften, f32 bias);

extern void sdfModelResetFaceDivParm(sdfModelRootNode_t *model);

/* Advance the model, light-colour and directional transitions for one event unit. */
void UnitAnime(evtUnit_t *unit);

/* vu0 routine: load vf10 with the unit's colour (flag 0x100), its target's vector, or the default */
void GetBaseLightRGB(evtUnit_t *h);

/* vu0 routine: load the target info secondColor vector when unit-owned. */
void GetBaseAmbientRGB(evtUnit_t *h);

/* vu0 routine: load vf10 with the unit's vector, or the default one */
void GetBaseLightDir(evtUnit_t *h);

/* The colour inlines read and write a 16-byte slot, one word of it used. */
typedef struct {
    u32 word;
    u8 pad04[12];
} evtColorWork_t;

/* vu0 routine: unpack the unit's colours into a parallel light packet. */
void func_001A5AB8(evtUnit_t *h);

void evtLinkLightObjectBasic(evtUnit_t *h, dds3Basic_t *light, s32 frame);

void evtUnlinkLightObjectBasic(evtUnit_t *h, s32 frame);

void func_001E35B0(evtUnit_t *unit, struct sdf3DLightParallelPacket_t *value);

void evtUnlinkExternalLightPacket(evtUnit_t *h);

/* vu0 routine: pack the base light and ambient vectors into the unit's colours. */
void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb);

/* vu0 routine: normalise the target direction and retain the current source. */
void evtSetUnitLightDir(evtUnit_t *h, s32 frame);

void evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb);

void evtSetUnitColorA(evtUnit_t *h, s32 frame, u32 a);

u8 evtUnitLightAnimeStat(evtUnit_t *h);

void evtUnitLightOn(evtUnit_t *h);

void evtUnitLightOff(evtUnit_t *h, s32 frame);

evtUnit_t *evtGetUnitHandle(s32 serial);

evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit);

void *sdfChipAllocDirect(u32 aSize);

extern const sdfVector4_t D_0053A850;

evtUnit_t *evtCreateUnitHandle(dds3Basic_t *owner, mdlObj_t *model);

s32 evtDeleteUnitHandle(evtUnit_t *h);

s32 evtGetUnitStat(evtUnit_t *h);

/* Callers pass the promoted full-width value (one reads a byte table field);
 * only the low halfword is stored. */
void evtSetWaitMotion(evtUnit_t *h, s32 motion);

void evtSetWaitMotionSpeed(evtUnit_t *h, f32 sp);

void evtSetWaitHokanFrame(evtUnit_t *h, s32 before, s32 smooth);

s32 evtCheckUnitWait(evtUnit_t *h);

void evtSetUnitKutiMotion(evtUnit_t *h, s32 group, s32 motion);

void func_001E3C30(evtUnit_t *h, s32 slot, s32 firstValue, s32 secondValue, s32 thirdValue,
                   s32 mode);

void evtRunUnitKutiMotion(evtUnit_t *h);

void func_001E3C30(evtUnit_t *h, s32 slot, s32 firstValue, s32 secondValue, s32 thirdValue,
                   s32 mode);

void evtSetUnitMotion(evtUnit_t *h, s32 motion, s32 before, s32 smooth, s32 mode);

extern f32 acosf(f32 x);

extern void FromRotation(f32 aAngle);

void dds3AddRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot);

extern const sdfVector4_t evtUnitUpAxis;

/* vu0 routine: turn the unit toward dir, at most a tenth of a radian a frame. */
s32 evtUnitRotationTarget(evtUnit_t *h, f32 *pos, f32 radio);

/* Flat, negated direction of the rotation in quat, offset by the effect object's point, aimed with evtRotateUnitTowardTarget. */
s32 evtUnitRotationQuat(evtUnit_t *h, f32 *rot, f32 radio);

s32 evtUnitMoveType0(evtUnit_t *h);

s32 evtUnitMoveTypePathTrace(evtUnit_t *h);

void func_001E4108(evtUnit_t *work, void *src);

void evtSetUnitRot1(evtUnit_t *h, s128 *rot, s32 para1);

void evtSetUnitRot2(evtUnit_t *h, s32 posID, s32 para1);

void evtSetUnitMove1(evtUnit_t *work, s32 motionSubmode, s128 *vector, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast);

void evtSetUnitMove2(evtUnit_t *work, s32 motionSubmode, s32 worldObjectKey, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast);

void evtSetUnitMove3(evtUnit_t *h, s32 pathID, s32 para1, s32 para2, s32 movesys, s32 movedir, s32 rotsys);

s32 func_001E4510(evtUnit_t *work, s32 (*callback)(evtUnit_t *, s32));

extern void UnitAnime(evtUnit_t *unit);

extern void mdlSmoothLoopMotion(mdlObj_t *model, s32 group, s32 motion, f32 before, f32 smooth);

extern void mdlSmoothSingleMotion(mdlObj_t *model, s32 group, s32 motion, f32 before, f32 smooth);

extern s32 mdlChkMotionEndEx(mdlObj_t *model, s32 group);

#include "sdk_libc.h"

extern const sdfVector4_t D_0053A850;

extern const sdfVector4_t evtUnitUpAxis;

s32 evtUnitManagerPrimaryFunc(dds3PoseSet_t *unused, dds3Basic_t *object);
/* End of declarations from src/smt3/event/evtUnitManager.c */

/* vf10 = (a - b) with y replaced by 0 */
void evtVu0SubVectorXZAxis(f32 *a, f32 *b) {
    SDF_VU0_LOADVECTOR(a);
    SDF_VU0_LOADVECTOR_B(b);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SETVECTOR_Y_V0(0.0f);
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtGetUnitDirVector);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtCheckMoveEnd);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtGetNextMovePos);

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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A4318);

INCLUDE_ASM(const s32, "event/evtUnitManager", GetBaseLightRGB);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A5708);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A57B8);

INCLUDE_ASM(const s32, "event/evtUnitManager", GetBaseAmbientRGB);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A5920);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A59A8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A5A30);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A5AB8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A5CB0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A5D20);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A5D88);

void evtUnlinkExternalLightPacket(evtUnit_t *h) {
    h->flags = h->flags & ~EVT_UNIT_FLAG_VALUE_CHANGED;
    func_001A5AB8(h);
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitLightAndAmbient);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A5E68);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A5F58);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A5FC8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A6038);

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

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitLightAnimeStat);

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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A6390);

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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A65A8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A65B8);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetWaitMotionSpeed);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A65C8);

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

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitKutiMotion);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A6650);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A6680);

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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A67D8);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitRotationQuat);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitMoveType0);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_0044DD40);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_0044DD50);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_0044DD60);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_0044DD70);

INCLUDE_RODATA(const s32, "event/evtUnitManager", D_0044DD80);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A6B08);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitMoveTypePathTrace);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A6DE8);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitRot1);

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

INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitMove1);

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

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A71F0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A7200);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A72A8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A7310);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A7338);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A7A20);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A7AC8);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A7BB0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A7BE0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A7CF0);

INCLUDE_ASM(const s32, "event/evtUnitManager", func_001A7EF0);
