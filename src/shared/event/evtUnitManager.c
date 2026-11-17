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
#include "code_r1_0010DF68.h"

/* vu0 routine: load vf10 with the unit's colour (flag 0x100), its target's vector, or the default */
/* vu0 routine: load vf10 with the unit's vector, or the default one */
extern s32 CheckMoveEndBustup(evtUnit_t *h);
extern void UnitMoveTypeBustup(evtUnit_t *h);
extern void UnitAnime(evtUnit_t *unit);
extern s32 mdlChkMotionEndEx(mdlObj_t *model, s32 group);
extern s32 dds3SceneBasicExistBasicFromType(dds3Basic_t *scene, s32 type, dds3Basic_t *target);
extern void dds3RotationArc(void);
extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *model, s32 id);
extern void sdfModelSetFaceDivParm(sdfModelRootNode_t *model, f32 soften, f32 bias);
extern void sdfModelResetFaceDivParm(sdfModelRootNode_t *model);

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

extern void evtSetUnitMove1(evtUnit_t *work, s32 motionSubmode, s128 *vector, s32 motion, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 data);
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
void evtSetUnitRot1(evtUnit_t *h, f32 *rot, s32 para1);
void evtSetUnitRot2(evtUnit_t *h, s32 posID, s32 para1);
void evtSetUnitMove1(evtUnit_t *work, s32 motionSubmode, s128 *vector, s32 motion, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 data);
void evtSetUnitMove2(evtUnit_t *work, s32 motionSubmode, s32 worldObjectKey, s32 motion, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 data);

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

#ifdef NON_MATCHING
/* 70 of 1232 words differ: same length and structure; registers differ from the colour blend of the second light onward (address and scale temporaries swapped). */
/* Advance the model, light-colour and directional transitions for one event unit. */
void UnitAnime(evtUnit_t *unit) {
    f32 identity[4] __attribute__((aligned(16)));
    f32 targetDirection[4] __attribute__((aligned(16)));
    f32 previousDirection[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 blendedDirection[4] __attribute__((aligned(16)));
    f32 oldDirection[4] __attribute__((aligned(16)));
    u32 rgbTarget[4], rgbSource[4], rgbResult[4];
    u32 alphaTarget[4], alphaSource[4], alphaResult[4];
    u32 colorSource1[4];
    u32 colorTarget1[4];
    u32 colorResult1[4];
    u32 colorSource2[4];
    u32 colorTarget2[4];
    u32 colorResult2[4];
    u32 colorSource3[4];
    u32 colorTarget3[4];
    u32 colorResult3[4];
    u32 colorSource4[4];
    u32 colorTarget4[4];
    u32 colorResult4[4];
    u32 colorResult5[4];
    u32 colorResult6[4];
    u32 colorResult7[4];
    u32 colorResult8[4];
    u32 currentColor1[4];
    u32 previousColor1[4];
    u32 finalColor1[4];
    u32 currentColor2[4];
    u32 previousColor2[4];
    u32 finalColor2[4];
    f32 auxFirst, auxSecond;
    f32 factor;
    f32 transition;
    f32 currentWeight;
    f32 previousWeight;
    evtTargetInfo_t *target;
    evtTargetInfo_t *previous;
    s32 ownVector;
    s32 overrideAux = 0;
    u32 flags;
    u32 originalFlags;
    u32 oldColor;

    memset(identity, 0, sizeof(identity));
    identity[3] = 1.0f;
    previousWeight = 0.0f;
    auxFirst = sdfFaceDivSoften;
    auxSecond = sdfFaceDivBias;
    flags = unit->flags;
    if (flags & EVT_UNIT_FLAG_RGB_TRANSITION) {
        u32 packed;
        factor = unit->rgbDuration ? (f32)unit->rgbElapsed / unit->rgbDuration : 1.0f;
        rgbTarget[0] = unit->color64;
        EE_MMI_RGBA_UNPACK_READONLY(rgbTarget, 0.0078125f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        rgbSource[0] = unit->color60;
        EE_MMI_RGBA_UNPACK_READONLY(rgbSource, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - factor);
        SDF_VU0_SCALEVECTOR_B_BITS(factor);
        SDF_VU0_ADDVECTOR();
        oldColor = mdlGetColor((mdlObj_t *)unit->owner);
        MMI_RGBA_PACK_UNIT(packed, 128.0f);
        rgbResult[0] = packed;
        mdlSetColor((mdlObj_t *)unit->owner, (oldColor & 0xFF000000) | (rgbResult[0] & 0xFFFFFF));
        if (unit->rgbElapsed >= unit->rgbDuration) {
            flags = unit->flags &= ~EVT_UNIT_FLAG_RGB_TRANSITION;
        } else {
            flags = unit->flags;
            unit->rgbElapsed++;
        }
    }
    if (flags & EVT_UNIT_FLAG_ALPHA_TRANSITION) {
        u32 packed;
        factor = unit->alphaDuration ? (f32)unit->alphaElapsed / unit->alphaDuration : 1.0f;
        alphaTarget[0] = unit->color64;
        EE_MMI_RGBA_UNPACK_READONLY(alphaTarget, 0.0078125f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        alphaSource[0] = unit->color60;
        EE_MMI_RGBA_UNPACK_READONLY(alphaSource, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - factor);
        SDF_VU0_SCALEVECTOR_B_BITS(factor);
        SDF_VU0_ADDVECTOR();
        oldColor = mdlGetColor((mdlObj_t *)unit->owner);
        MMI_RGBA_PACK_UNIT(packed, 128.0f);
        alphaResult[0] = packed;
        mdlSetColor((mdlObj_t *)unit->owner, (oldColor & 0xFFFFFF) | (alphaResult[0] & 0xFF000000));
        if (unit->alphaElapsed >= unit->alphaDuration) {
            flags = unit->flags &= ~EVT_UNIT_FLAG_ALPHA_TRANSITION;
        } else {
            flags = unit->flags;
            unit->alphaElapsed++;
        }
    }
    if (flags & EVT_UNIT_FLAG_TARGET_TRANSITION) {
        if (unit->currentTransitionValue == 0) {
            unit->flags = flags & ~EVT_UNIT_FLAG_TARGET_TRANSITION;
        } else if (dds3SceneBasicExistBasicFromType(dds3GetActiveScene(), 9,
                                                   EE_ADDR_AS(dds3Basic_t *, unit->currentTransitionValue)) == 0) {
            unit->currentTransitionValue = 0;
            unit->previousTransitionValue = 0;
            unit->flags &= ~EVT_UNIT_FLAG_TARGET_TRANSITION;
        }
    }
    if (unit->previousTransitionValue &&
        dds3SceneBasicExistBasicFromType(dds3GetActiveScene(), 9,
                                         EE_ADDR_AS(dds3Basic_t *, unit->previousTransitionValue)) == 0) {
        unit->previousTransitionValue = 0;
    }
    transition = 0.0f;
    target = 0;
    previous = 0;
    ownVector = 0;
    currentWeight = 0.0f;
    if (unit->currentTransitionValue) {
        flags = unit->flags;
        if (flags & EVT_UNIT_FLAG_TARGET_TRANSITION) {
            f32 distance;
            target = (EE_ADDR_AS(dds3Basic_t *, unit->currentTransitionValue))->workdata;
            if (unit->previousTransitionValue) {
                previous = (EE_ADDR_AS(dds3Basic_t *, unit->previousTransitionValue))->workdata;
            }
            if (!sdfModelGetMarkPos(unit->owner->inner, 0)) {
                mdlGetPosition(unit->owner);
            }
            SDF_VU0_STOREVECTOR_NOMEM(position);
            SDF_VU0_LOADVECTOR_B(&EE_ADDR_AS(dds3Basic_t *, unit->currentTransitionValue)->poseset->pos);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(targetDirection);
            SDF_VU0_GETLENGTH(distance);
            if (distance <= target->nearDistance) {
                currentWeight = 1.0f;
            } else if (distance >= target->farDistance) {
                currentWeight = 0.0f;
            } else {
                currentWeight = 1.0f - (distance - target->nearDistance) /
                    (target->farDistance - target->nearDistance);
            }
            if (previous) {
                f32 previousDistance;
                SDF_VU0_LOADVECTOR(position);
                SDF_VU0_LOADVECTOR_B(&EE_ADDR_AS(dds3Basic_t *, unit->previousTransitionValue)->poseset->pos);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_STOREVECTOR_NOMEM(previousDirection);
                SDF_VU0_GETLENGTH(previousDistance);
                if (previousDistance <= previous->nearDistance) {
                    previousWeight = 1.0f;
                } else if (previousDistance >= previous->farDistance) {
                    previousWeight = 0.0f;
                } else {
                    previousWeight = 1.0f - (previousDistance - previous->nearDistance) /
                        (previous->farDistance - previous->nearDistance);
                }
            }
            SDF_VU0_LOADVECTOR(targetDirection);
            if (target->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
                SDF_VU0_STOREVECTOR_NOMEM(target->direction);
                flags = unit->flags;
                ownVector = 1;
            } else {
                if (unit->flags & EVT_UNIT_FLAG_TARGET_BLEND_PHASES) {
                    if ((u16)unit->transitionFrameCount) {
                        transition = (f32)(u16)unit->transitionElapsed / (u16)unit->transitionFrameCount;
                    } else {
                        transition = 1.0f;
                    }
                    if (unit->flags & EVT_UNIT_FLAG_TARGET_BLEND_OUT) {
                        transition = 1.0f - transition;
                    }
                    if ((u16)unit->transitionFrameCount) {
                        if ((u16)unit->transitionFrameCount <= (u16)++unit->transitionElapsed) {
                            if (unit->flags & EVT_UNIT_FLAG_TARGET_BLEND_OUT) {
                                unit->currentTransitionValue = 0;
                                unit->flags &= ~EVT_UNIT_FLAG_TARGET_TRANSITION;
                            }
                            unit->previousTransitionValue = 0;
                            unit->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_PHASES;
                        }
                    }
                } else {
                    transition = 1.0f;
                }
                flags = unit->flags;
            }
        }
    } else {
        flags = unit->flags;
    }
    if (flags & 0x6000) {
        if (unit->directionFramesRemaining) {
            factor = 1.0f / unit->directionFramesRemaining;
        } else {
            factor = 1.0f;
        }
        SDF_VU0_LOADVECTOR(unit->vec20);
        if (flags & 0x2000) {
            SDF_VU0_LOADVECTOR_B(unit->vec30);
        } else if (ownVector) {
            SDF_VU0_LOADVECTOR_B(target->direction);
        } else {
            SDF_VU0_LOADVECTOR_B(&dds3LightParam.parallel[0]->v);
        }
        dds3RotationArc();
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(identity);
        Lerp(factor);
        ToMatrix();
        SDF_VU0_LOADVECTOR(unit->vec20);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR_NOMEM(unit->vec40);
        SDF_VU0_STOREVECTOR_NOMEM(unit->vec20);
        SDF_VU0_LOADVECTOR(unit->vec70);
        SDF_VU0_LOADVECTOR_B(unit->vec80);
        dds3RotationArc();
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(identity);
        Lerp(factor);
        ToMatrix();
        SDF_VU0_LOADVECTOR(unit->vec70);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR_NOMEM(unit->vec90);
        SDF_VU0_STOREVECTOR_NOMEM(unit->vec70);
        SDF_VU0_LOADVECTOR(unit->vecC0);
        SDF_VU0_LOADVECTOR_B(unit->vecD0);
        dds3RotationArc();
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(identity);
        Lerp(factor);
        ToMatrix();
        SDF_VU0_LOADVECTOR(unit->vecC0);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR_NOMEM(unit->vecE0);
        SDF_VU0_STOREVECTOR_NOMEM(unit->vecC0);
        if (unit->directionFramesRemaining == 0) {
            flags = unit->flags;
            if (flags & 0x4000) {
                unit->flags &= ~0x400;
                flags = unit->flags;
            }
            unit->flags = flags & ~0x6000;
        } else {
            unit->directionFramesRemaining--;
        }
    } else if (!(flags & 0x400)) {
        if (ownVector) {
            SDF_VU0_LOADVECTOR(target->direction);
        } else {
            SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[0]->v);
        }
        SDF_VU0_STOREVECTOR_NOMEM(unit->vec40);
        if (ownVector) {
            SDF_VU0_LOADVECTOR(target->direction);
        } else {
            SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[1]->v);
        }
        SDF_VU0_STOREVECTOR_NOMEM(unit->vec90);
        if (ownVector) {
            SDF_VU0_LOADVECTOR(target->direction);
        } else {
            SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[2]->v);
        }
        SDF_VU0_STOREVECTOR_NOMEM(unit->vecE0);
    }
    if (target && !ownVector) {
        factor = transition;
        SDF_VU0_LOADVECTOR(unit->vec40);
        SDF_VU0_LOADVECTOR_B(targetDirection);
        dds3RotationArc();
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(identity);
        Lerp(factor);
        ToMatrix();
        SDF_VU0_LOADVECTOR(unit->vec40);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR_NOMEM(blendedDirection);
        if (previous) {
            factor = 1.0f;
            SDF_VU0_LOADVECTOR(unit->vec40);
            SDF_VU0_LOADVECTOR_B(previousDirection);
            dds3RotationArc();
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(identity);
            Lerp(factor);
            ToMatrix();
            SDF_VU0_LOADVECTOR(unit->vec40);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_STOREVECTOR_NOMEM(oldDirection);
            SDF_VU0_LOADVECTOR_B(blendedDirection);
            dds3RotationArc();
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(identity);
            Lerp(transition);
            ToMatrix();
            SDF_VU0_LOADVECTOR(oldDirection);
            SDF_VU0_APPLYMATRIX();
        }
        SDF_VU0_STOREVECTOR_NOMEM(unit->vec10);
    } else {
        SDF_VU0_COPYVECTOR(unit->vec10, unit->vec40);
        SDF_VU0_COPYVECTOR(unit->vec60, unit->vec90);
        SDF_VU0_COPYVECTOR(unit->vecB0, unit->vecE0);
    }
    originalFlags = unit->flags;
    flags = originalFlags;
    if (originalFlags & 0x1800) {
        u32 packed;

        if (unit->colorFramesRemaining) {
            factor = 1.0f / unit->colorFramesRemaining;
        } else {
            factor = 1.0f;
        }
        colorSource1[0] = unit->firstCurrent;
        EE_MMI_RGBA_UNPACK_READONLY(colorSource1, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - factor);
        SDF_VU0_COPYVECTOR_A_TO_B();
        if (unit->flags & 0x800) {
            colorTarget1[0] = unit->color08;
            EE_MMI_RGBA_UNPACK_READONLY(colorTarget1, 0.0078125f);
        } else if (ownVector) {
            SDF_VU0_LOADVECTOR(target);
        } else {
            SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[0]->color);
        }
        SDF_VU0_SCALEVECTOR_BITS(factor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        MMI_RGBA_PACK_UNIT(packed, 128.0f);
        colorResult1[0] = packed;
        unit->firstCurrent = colorResult1[0];
        unit->color0C = unit->firstCurrent;

        colorSource2[0] = unit->color54;
        EE_MMI_RGBA_UNPACK_READONLY(colorSource2, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - factor);
        SDF_VU0_COPYVECTOR_A_TO_B();
        if (unit->flags & 0x800) {
            colorTarget2[0] = unit->color58;
            EE_MMI_RGBA_UNPACK_READONLY(colorTarget2, 0.0078125f);
        } else if (ownVector) {
            SDF_VU0_LOADVECTOR(target);
        } else {
            SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[1]->color);
        }
        SDF_VU0_SCALEVECTOR_BITS(factor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        MMI_RGBA_PACK_UNIT(packed, 128.0f);
        colorResult2[0] = packed;
        unit->color54 = colorResult2[0];
        unit->color5C = unit->color54;

        colorSource3[0] = unit->colorA4;
        EE_MMI_RGBA_UNPACK_READONLY(colorSource3, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - factor);
        SDF_VU0_COPYVECTOR_A_TO_B();
        if (unit->flags & 0x800) {
            colorTarget3[0] = unit->colorA8;
            EE_MMI_RGBA_UNPACK_READONLY(colorTarget3, 0.0078125f);
        } else if (ownVector) {
            SDF_VU0_LOADVECTOR(target);
        } else {
            SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[2]->color);
        }
        SDF_VU0_SCALEVECTOR_BITS(factor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        MMI_RGBA_PACK_UNIT(packed, 128.0f);
        colorResult3[0] = packed;
        unit->colorA4 = colorResult3[0];
        unit->colorAC = unit->colorA4;

        colorSource4[0] = unit->colorF4;
        EE_MMI_RGBA_UNPACK_READONLY(colorSource4, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - factor);
        SDF_VU0_COPYVECTOR_A_TO_B();
        if (unit->flags & 0x800) {
            colorTarget4[0] = unit->colorF8;
            EE_MMI_RGBA_UNPACK_READONLY(colorTarget4, 0.0078125f);
        } else if (ownVector) {
            SDF_VU0_LOADVECTOR(target->secondColor);
        } else {
            SDF_VU0_LOADVECTOR(&dds3LightParam.ambient);
        }
        SDF_VU0_SCALEVECTOR_BITS(factor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_VECTOR_W_SET_ONE();
        MMI_RGBA_PACK_UNIT(packed, 128.0f);
        colorResult4[0] = packed;
        unit->colorF4 = colorResult4[0];
        unit->colorFC = unit->colorF4;
        if (unit->colorFramesRemaining == 0) {
            if (originalFlags & 0x1000) {
                flags = unit->flags = originalFlags & ~EVT_UNIT_FLAG_USE_UNIT_COLOR_CHANNELS;
            }
            unit->flags = flags & ~0x1800;
        } else {
            unit->colorFramesRemaining--;
        }
    } else {
        u32 packed;
        if (!(originalFlags & EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR)) {
            if (ownVector) {
                SDF_VU0_LOADVECTOR(target);
            } else {
                SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[0]->color);
            }
            MMI_RGBA_PACK_UNIT(packed, 128.0f);
            colorResult5[0] = packed;
            unit->color0C = colorResult5[0];
            if (ownVector) {
                SDF_VU0_LOADVECTOR(target);
            } else {
                SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[1]->color);
            }
            MMI_RGBA_PACK_UNIT(packed, 128.0f);
            colorResult6[0] = packed;
            unit->color5C = colorResult6[0];
            if (ownVector) {
                SDF_VU0_LOADVECTOR(target);
            } else {
                SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[2]->color);
            }
            MMI_RGBA_PACK_UNIT(packed, 128.0f);
            colorResult7[0] = packed;
            unit->colorAC = colorResult7[0];
        }
        if (!(originalFlags & EVT_UNIT_FLAG_USE_UNIT_SECOND_COLOR)) {
            if (ownVector) {
                SDF_VU0_LOADVECTOR(target->secondColor);
            } else {
                SDF_VU0_LOADVECTOR(&dds3LightParam.ambient);
            }
            MMI_RGBA_PACK_UNIT(packed, 128.0f);
            colorResult8[0] = packed;
            unit->colorFC = colorResult8[0];
        }
    }
    if (target && !ownVector) {
        u32 packed;
        f32 weightedTransition;
        factor = currentWeight * transition;
        weightedTransition = factor;
        currentColor1[0] = unit->color0C;
        EE_MMI_RGBA_UNPACK_READONLY(currentColor1, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - factor);
        SDF_VU0_LOADVECTOR_B(target);
        SDF_VU0_SCALEVECTOR_B_BITS(factor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(blendedDirection);
        if (previous) {
            factor = previousWeight;
            previousColor1[0] = unit->color0C;
            EE_MMI_RGBA_UNPACK_READONLY(previousColor1, 0.0078125f);
            SDF_VU0_SCALEVECTOR_BITS(1.0f - factor);
            SDF_VU0_LOADVECTOR_B(previous);
            SDF_VU0_SCALEVECTOR_B_BITS(factor);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_SCALEVECTOR_BITS(1.0f - transition);
            SDF_VU0_LOADVECTOR_B(blendedDirection);
            SDF_VU0_SCALEVECTOR_B_BITS(transition);
            SDF_VU0_ADDVECTOR();
        }
        SDF_VU0_VECTOR_W_SET_ZERO();
        MMI_RGBA_PACK_UNIT(packed, 128.0f);
        finalColor1[0] = packed;
        unit->color = finalColor1[0];
        factor = weightedTransition;
        currentColor2[0] = unit->colorFC;
        EE_MMI_RGBA_UNPACK_READONLY(currentColor2, 0.0078125f);
        SDF_VU0_SCALEVECTOR_BITS(1.0f - factor);
        SDF_VU0_LOADVECTOR_B(target->secondColor);
        SDF_VU0_SCALEVECTOR_B_BITS(factor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(blendedDirection);
        if (previous) {
            factor = previousWeight;
            previousColor2[0] = unit->colorFC;
            EE_MMI_RGBA_UNPACK_READONLY(previousColor2, 0.0078125f);
            SDF_VU0_SCALEVECTOR_BITS(1.0f - factor);
            SDF_VU0_LOADVECTOR_B(previous->secondColor);
            SDF_VU0_SCALEVECTOR_B_BITS(factor);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_SCALEVECTOR_BITS(1.0f - transition);
            SDF_VU0_LOADVECTOR_B(blendedDirection);
            SDF_VU0_SCALEVECTOR_B_BITS(transition);
            SDF_VU0_ADDVECTOR();
        }
        SDF_VU0_VECTOR_W_SET_ZERO();
        MMI_RGBA_PACK_UNIT(packed, 128.0f);
        finalColor2[0] = packed;
        unit->colorF0 = finalColor2[0];
    } else {
        unit->color = unit->color0C;
        unit->colorF0 = unit->colorFC;
        unit->color50 = unit->color5C;
        unit->colorA0 = unit->colorAC;
    }
    if (target && !ownVector) {
        auxFirst = target->auxFirst;
        auxSecond = target->auxSecond;
        if (auxFirst >= 1.0f) {
            factor = currentWeight * transition;
            auxFirst = sdfFaceDivSoften * (1.0f - factor) + auxFirst * factor;
            auxSecond = sdfFaceDivBias * (1.0f - factor) + auxSecond * factor;
        }
        overrideAux = 1;
        if (previous) {
            f32 oldFirst = previous->auxFirst;
            f32 oldSecond = previous->auxSecond;
            if (oldFirst >= 1.0f) {
                factor = previousWeight;
                oldFirst = sdfFaceDivSoften * (1.0f - factor) + oldFirst * factor;
                oldSecond = sdfFaceDivBias * (1.0f - factor) + oldSecond * factor;
                auxFirst = auxFirst * transition + oldFirst * (1.0f - transition);
                auxSecond = auxSecond * transition + oldSecond * (1.0f - transition);
            }
        }
    }
    if (!target) {
        if (evtGetCustomSLight_SoftenAndBias(unit, &auxFirst, &auxSecond)) {
            overrideAux = 1;
        }
    }
    if (overrideAux) {
        sdfModelSetFaceDivParm(unit->owner->inner, auxFirst, auxSecond);
    } else {
        sdfModelResetFaceDivParm(unit->owner->inner);
    }
}
#else
INCLUDE_ASM(const s32, "event/evtUnitManager", UnitAnime);
#endif

void GetBaseLightRGB(evtUnit_t *h) {
    evtTargetInfo_t *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (h->currentTransitionValue != 0 && (h->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        info = (EE_ADDR_AS(dds3Basic_t *, h->currentTransitionValue))->workdata;
        if (info->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (h->flags & EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR) {
        scale = 0.0078125f;
        color = h->color;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
        SDF_VU0_LOADVECTOR(info);
    } else {
        SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[0]->color);
    }
}

void GetBaseLightRGB_2(evtUnit_t *h) {
    evtTargetInfo_t *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (h->currentTransitionValue != 0 && (h->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        info = (EE_ADDR_AS(dds3Basic_t *, h->currentTransitionValue))->workdata;
        if (info->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (h->flags & EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR) {
        scale = 0.0078125f;
        color = h->color50;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
        SDF_VU0_LOADVECTOR(info);
    } else {
        SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[1]->color);
    }
}

void GetBaseLightRGB2(evtUnit_t *h) {
    evtTargetInfo_t *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (h->currentTransitionValue != 0 && (h->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        info = (EE_ADDR_AS(dds3Basic_t *, h->currentTransitionValue))->workdata;
        if (info->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (h->flags & EVT_UNIT_FLAG_USE_UNIT_FIRST_COLOR) {
        scale = 0.0078125f;
        color = h->colorA0;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
        SDF_VU0_LOADVECTOR(info);
    } else {
        SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[2]->color);
    }
}

void GetBaseAmbientRGB(evtUnit_t *h) {
    evtTargetInfo_t *info = 0;
    s32 ownVector = 0;
    s32 color;
    f32 scale;

    if (h->currentTransitionValue != 0 && (h->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        info = (EE_ADDR_AS(dds3Basic_t *, h->currentTransitionValue))->workdata;
        if (info->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (h->flags & EVT_UNIT_FLAG_USE_UNIT_SECOND_COLOR) {
        scale = 0.0078125f;
        color = h->colorF0;
        EE_MMI_RGBA_UNPACK(&color, scale);
    } else if (ownVector) {
                SDF_VU0_LOADVECTOR(info->secondColor);
    } else {
                SDF_VU0_LOADVECTOR(&dds3LightParam.ambient);
    }
}

void GetBaseLightDir(evtUnit_t *h) {
    s32 ownVector = 0;

    if (h->currentTransitionValue != 0 && (h->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        if (((evtTargetInfo_t *)(EE_ADDR_AS(dds3Basic_t *, h->currentTransitionValue))->workdata)->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (h->flags & 0x400) {
        SDF_VU0_LOADVECTOR(h->vec10);
    } else if (ownVector) {
        SDF_VU0_LOADVECTOR(h->vec10);
    } else {
        SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[0]->v);
    }
}

void evtSetUnitLightDir1Sub(evtUnit_t *h) {
    s32 ownVector = 0;

    if (h->currentTransitionValue != 0 && (h->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        if (((evtTargetInfo_t *)(EE_ADDR_AS(dds3Basic_t *, h->currentTransitionValue))->workdata)->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (h->flags & 0x400) {
        SDF_VU0_LOADVECTOR(h->vec60);
    } else if (ownVector) {
        SDF_VU0_LOADVECTOR(h->vec60);
    } else {
        SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[1]->v);
    }
}

void GetBaseLightDir2(evtUnit_t *h) {
    s32 ownVector = 0;

    if (h->currentTransitionValue != 0 && (h->flags & EVT_UNIT_FLAG_TARGET_TRANSITION)) {
        if (((evtTargetInfo_t *)(EE_ADDR_AS(dds3Basic_t *, h->currentTransitionValue))->workdata)->flags & EVT_TARGET_INFO_FLAG_UNIT_OWNS_VECTOR) {
            ownVector = 1;
        }
    }
    if (h->flags & 0x400) {
        SDF_VU0_LOADVECTOR(h->vecB0);
    } else if (ownVector) {
        SDF_VU0_LOADVECTOR(h->vecB0);
    } else {
        SDF_VU0_LOADVECTOR(&dds3LightParam.parallel[2]->v);
    }
}

/* NON_MATCHING: 36 of 125 words match.  */
#ifdef NON_MATCHING
void MakeLightPacket(evtUnit_t *h) {
    sdf3DLight_t source[3];
    sdfVector4_t ambient;
    sdf3DLight_t *lights[3] = { &source[0], &source[1], &source[2] };
    evtColorWork_t work[4];
    u32 flags;
    f32 *channel;
    s32 i;
    f32 scale;

    flags = h->flags;
    if (flags & 0x20000) {
        return;
    }
    h->lightPacket = NULL;
    if ((flags & 0x40000) == 0) {
        if ((flags & 0x700) == 0) {
            evtMakeCustomSLightPacket(h);
            return;
        }
    }
    work[0].word = h->color;
    scale = 0.0078125f;
    EE_MMI_RGBA_UNPACK(&work[0], scale);
    SDF_VU0_STOREVECTOR_MEMOPS(source[0].color);
    work[1].word = h->color50;
    scale = 0.0078125f;
    EE_MMI_RGBA_UNPACK(&work[1], scale);
    SDF_VU0_STOREVECTOR(&source[1].color);
    work[2].word = h->colorA0;
    scale = 0.0078125f;
    EE_MMI_RGBA_UNPACK(&work[2], scale);
    SDF_VU0_STOREVECTOR(&source[2].color);
    SDF_VU0_LOADVECTOR(h->vec10);
    SDF_VU0_STOREVECTOR(&source[0].v);
    SDF_VU0_LOADVECTOR(h->vec60);
    SDF_VU0_STOREVECTOR(&source[1].v);
    SDF_VU0_LOADVECTOR(h->vecB0);
    SDF_VU0_STOREVECTOR(&source[2].v);
    work[3].word = h->colorF0;
    scale = 0.0078125f;
    EE_MMI_RGBA_UNPACK(&work[3], scale);
    SDF_VU0_STOREVECTOR(&ambient);
    ambient.w = 1.0f;
    channel = &ambient.x;
    for (i = 2; i >= 0; i--) {
        if (1.0f < *channel) {
            *channel = 1.0f;
        }
        channel++;
    }
    sdf3DMakeLightParallelPacket(h->endpointWork, lights, &ambient);
    h->lightPacket = h->endpointWork;
}
#else
INCLUDE_ASM(const s32, "event/evtUnitManager", MakeLightPacket);
#endif

void evtLinkLightObjectBasic(evtUnit_t *h, dds3Basic_t *light, s32 frame) {
    h->flags |= EVT_UNIT_FLAG_TARGET_TRANSITION;
    h->previousTransitionValue = h->currentTransitionValue;
    h->currentTransitionValue = EE_ADDR_S(light);
    if (frame == 0) {
        h->previousTransitionValue = 0;
        h->transitionElapsed = 0;
        h->transitionDuration = 0;
        h->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_PHASES;
    } else {
        h->flags |= EVT_UNIT_FLAG_TARGET_BLEND_IN;
        h->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_OUT;
        h->transitionDuration = frame;
        h->transitionElapsed = 0;
    }
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtUnitManager.c:798, match, same function 1.00 (words 26/26, calls 1.00, shape 1.00) */
void evtUnlinkLightObjectBasic(evtUnit_t *h, s32 frame) {
    if (h->currentTransitionValue != 0) {
        if (frame == 0) {
            h->flags &= ~EVT_UNIT_FLAG_TARGET_TRANSITION;
            h->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_PHASES;
            h->currentTransitionValue = 0;
        } else {
            h->flags &= ~EVT_UNIT_FLAG_TARGET_BLEND_IN;
            h->transitionDuration = frame;
            h->flags |= EVT_UNIT_FLAG_TARGET_BLEND_OUT;
            h->transitionElapsed = 0;
        }
    }
}

INCLUDE_ASM(const s32, "event/evtUnitManager", evtLinkExternalLightPacket);

void evtUnlinkExternalLightPacket(evtUnit_t *h) {
    h->flags = h->flags & ~EVT_UNIT_FLAG_VALUE_CHANGED;
    MakeLightPacket(h);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtUnitManager.c:825, near, same function 1.00 (words 37/37, calls 1.00, shape 1.00) */
void evtSetUnitLightAndAmbient(evtUnit_t *h, s32 frame, u32 lrgb, u32 argb) {
    evtColorWork_t work[2];
    union {
        f32 value;
        u32 bits;
    } scale;
    u32 light;
    u32 ambient;

    h->colorDuration = frame;
    h->color08 = lrgb;
    h->colorF8 = argb;
    scale.value = 128.0f;
    GetBaseLightRGB(h);
    MMI_RGBA_PACK_UNIT(light, scale.bits);
    /* Retail retains this colour-slot store in the ambient call delay slot,
     * as does DDS evtInitializeUnitColorTransition. */
    work[0].word = light;
    h->firstCurrent = light;
    GetBaseAmbientRGB(h);
    MMI_RGBA_PACK_UNIT(ambient, scale.bits);
    h->colorF4 = ambient;
    h->flags = (h->flags | 0x800) & ~0x1000;
}

void evtSetUnitLightAndAmbient3(evtUnit_t *h, s32 frame, u32 lrgb, u32 lrgb2, u32 lrgb3, u32 argb) {
    union {
        f32 value;
        u32 bits;
    } scale;
    evtColorWork_t work[4];
    u32 light;
    u32 ambient;

    h->colorDuration = frame;
    h->color08 = lrgb;
    h->colorF8 = argb;
    h->color58 = lrgb2;
    scale.value = 128.0f;
    h->colorA8 = lrgb3;
    GetBaseLightRGB(h);
    MMI_RGBA_PACK_UNIT(light, scale.bits);
    work[0].word = light;
    h->firstCurrent = light;
    GetBaseLightRGB_2(h);
    MMI_RGBA_PACK_UNIT(light, scale.bits);
    work[1].word = light;
    h->color54 = light;
    GetBaseLightRGB2(h);
    MMI_RGBA_PACK_UNIT(light, scale.bits);
    work[2].word = light;
    h->colorA4 = light;
    GetBaseAmbientRGB(h);
    MMI_RGBA_PACK_UNIT(ambient, scale.bits);
    h->colorF4 = ambient;
    h->flags = (h->flags | 0x800) & ~0x1000;
}

void evtSetUnitLightDir(evtUnit_t *h, s32 frame) {
    h->directionDuration = frame;
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(h->vec30);
    GetBaseLightDir(h);
    SDF_VU0_STOREVECTOR_NOMEM(h->vec20);
    h->flags = (h->flags | 0x2400) & ~0x4000;
}

s32 evtSetUnitLightDir1(evtUnit_t *h, s32 frame) {
    h->directionDuration = frame;
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(h->vec80);
    evtSetUnitLightDir1Sub(h);
    SDF_VU0_STOREVECTOR_NOMEM(h->vec70);
    return h->flags = (h->flags | 0x2400) & ~0x4000;
}

s32 evtSetUnitLightDir2(evtUnit_t *h, s32 frame) {
    h->directionDuration = frame;
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(h->vecD0);
    GetBaseLightDir2(h);
    SDF_VU0_STOREVECTOR_NOMEM(h->vecC0);
    return h->flags = (h->flags | 0x2400) & ~0x4000;
}

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

evtUnit_t *evtCreateUnitHandle(dds3Basic_t *owner, mdlObj_t *model) {
    evtUnit_t *work;
    sdfVector4_t axis;
    sdfVector4_t axis2;
    sdfVector4_t axis3;

    axis = evtCreateUnitHandleTable;
    axis2 = evtCreateUnitHandleTable2;
    axis3 = evtCreateUnitHandleTable3;
    work = sdfChipAllocDirect(sizeof(*work));
    memset(work, 0, sizeof(*work));
    work->motionState = EVT_UNIT_MOTION_STATE_IDLE;
    work->waitBlendEnd = 10;
    work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE;
    work->transitionMotionScale = 0.5f;
    work->effObj = owner;
    work->owner = model;
    work->motionSetup.endFrame = 10;
    work->waitBlendStart = 0;
    work->motionSetup.startFrame = 0;
    SDF_VU0_LOADVECTOR_MEMOPS(axis);
    SDF_VU0_STOREVECTOR(work->vec10);
    SDF_VU0_STOREVECTOR(work->vec40);
    work->color0C = 0xB2B2B2;
    work->color = 0xB2B2B2;
    work->colorFC = 0x80303030;
    work->colorF0 = 0x80303030;
    SDF_VU0_LOADVECTOR(&axis2);
    SDF_VU0_STOREVECTOR(work->vec60);
    SDF_VU0_STOREVECTOR(work->vec90);
    SDF_VU0_LOADVECTOR(&axis3);
    SDF_VU0_STOREVECTOR(work->vecB0);
    SDF_VU0_STOREVECTOR(work->vecE0);
    work->color5C = 0;
    work->color50 = 0;
    work->colorAC = 0;
    work->colorA0 = 0;
    work->endpointWork = sdfChipAllocDirect(0xE0);
    memset(work->endpointWork, 0, 0xE0);
    work->lightPacket = NULL;
    work->LookRotFlag = 0;
    work->LookRotY = 0;
    work->unk206 = 0;
    return work;
}

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

s16 evtSetUnitStat(evtUnit_t *h, s32 stat) {
    return h->motionState = stat;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtUnitManager.c:994, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void evtSetWaitMotion(evtUnit_t *h, s32 motion) {
    h->waitMotion = motion;
}

void evtSetWaitMotionSpeed(evtUnit_t *h, f32 sp) {
    h->transitionMotionScale = sp;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtUnitManager.c:1002, match, same function 1.00 (words 3/3, calls 1.00, shape 1.00) */
void evtSetWaitHokanFrame(evtUnit_t *h, s32 before, s32 smooth) {
    h->waitBlendStart = before;
    h->waitBlendEnd = smooth;
}

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

void evtRunUnitKutiMotion(addr_t h) {
    evtUnit_t *unit = EE_ADDR_AS(evtUnit_t *, h);

    evtSetUnitExMotion(unit, unit->firstSlot, unit->secondSlot, 0, 0, 2);
}

void evtSetUnitExMotion(evtUnit_t *h, s32 group, s32 motion, s32 before, s32 smooth, s32 mode)
{
    h->slotFlags[group] = 3;
    h->slotA[group] = motion;
    h->slotB[group] = before;
    h->slotC[group] = smooth;
    switch (mode) {
    case 0:
        h->slotFlags[group] |= 4;
        break;
    case 1:
        h->slotFlags[group] |= 8;
        break;
    case 2:
        h->slotFlags[group] |= 0x10;
        break;
    }
}

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

s32 evtUnitRotationTarget(evtUnit_t *h, f32 *pos, f32 radio) {
    sdfVector4_t dir;
    sdfVector4_t axis;
    sdfVector4_t rotation;
    sdfVector4_t flat;
    f32 cross;
    f32 angle;

    dir = evtUnitRotationTarget_evtTable;
    axis = evtUnitUpAxis;
    evtGetUnitDirVector(h);
    SDF_VU0_STOREVECTOR_MEMOPS(dir);
    SDF_VU0_COPYVECTOR_A_TO_C();
    evtVu0SubVectorXZAxis(pos, (f32 *)&h->effObj->poseset->pos);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(&flat);
    SDF_VU0_LOADVECTOR_B(&axis);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_INNERPRODUCT(cross);
    SDF_VU0_LOADVECTOR(&flat);
    SDF_VU0_INNERPRODUCT(angle);
    if (0.9998f <= angle) {
        return 1;
    }
    angle = acosf(angle);
    if (0.0f < cross) {
        angle = -angle;
    }
    angle *= radio;
    if ((h->flags & 0x200000) == 0) {
        if (angle < -0.15707963f) {
            angle = -0.15707963f;
        }
        if (0.15707963f < angle) {
            angle = 0.15707963f;
        }
    }
    SDF_VU0_LOADVECTOR(&axis);
    FromRotation(angle);
    SDF_VU0_STOREVECTOR_NOMEM(&rotation);
    dds3AddRotPoseSet(h->effObj, &rotation);
    return 0;
}

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

/* NON_MATCHING: 18 of 149 words match.  */
#ifdef NON_MATCHING
extern const char evtStrYmoveFrameno0[], evtStrYmoveFramenoAddvalue[];

s32 evtCalcFrame_UnitMoveType0(evtUnit_t *org_h) {
    evtUnit_t h;
    f32 delta[4];
    f32 pos[4];
    f32 rot[4];
    s32 frame = 0;
    s32 i;

    for (i = 0; i < 4; i++) {
        pos[i] = ((f32 *)&org_h->effObj->poseset->pos)[i];
        rot[i] = ((f32 *)&org_h->effObj->poseset->rot)[i];
    }
    h = *org_h;
    while (evtCheckMoveEnd(&h) == 0) {
        frame++;
        evtUnitMoveType0(&h);
    }
    for (i = 0; i < 4; i++) {
        ((f32 *)&org_h->effObj->poseset->pos)[i] = pos[i];
        ((f32 *)&org_h->effObj->poseset->rot)[i] = rot[i];
    }
    if (frame == 0) {
        printf(evtStrYmoveFrameno0);
        org_h->moveYFrames = 0;
        org_h->moveY = 0;
        return 0;
    }
    SDF_VU0_LOADVECTOR(org_h->targetVector);
    SDF_VU0_LOADVECTOR_B(&org_h->effObj->poseset->pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(delta);
    org_h->moveYFrames = frame;
    org_h->moveY = delta[1] / (f32)frame;
    printf(evtStrYmoveFramenoAddvalue, frame, org_h->moveY, delta[1]);
    return frame;
}
#else
INCLUDE_RODATA(const s32, "event/evtUnitManager", evtCreateUnitHandleTable);

INCLUDE_RODATA(const s32, "event/evtUnitManager", evtCreateUnitHandleTable2);

INCLUDE_RODATA(const s32, "event/evtUnitManager", evtCreateUnitHandleTable3);

INCLUDE_RODATA(const s32, "event/evtUnitManager", evtUnitRotationTarget_evtTable);

INCLUDE_RODATA(const s32, "event/evtUnitManager", evtUnitUpAxis);

INCLUDE_ASM(const s32, "event/evtUnitManager", evtCalcFrame_UnitMoveType0);
#endif

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

void evtSetUnitRot1(evtUnit_t *h, f32 *rot, s32 para1) {
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

void evtSetUnitRot2(evtUnit_t *h, s32 posID, s32 para1) {
    dds3Basic_t *worldUnit;
    evtWorldSourceTransformPrefix_t *source;

    worldUnit = dds3SearchSceneBasic(dds3GetControlScene(), posID, 0x10);
    if (worldUnit != NULL) {
        source = worldUnit->workdata;
        evtSetUnitRot1(h, source->rotation, para1);
        h->linkedUnit = worldUnit;
    }
}

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

void evtSetUnitMove2(evtUnit_t *work, s32 motionSubmode, s32 worldObjectKey, s32 unused, s32 frames, s32 directionOffset, s32 (*callback)(evtUnit_t *, s32), s32 unusedLast) {
    dds3Basic_t *worldUnit;
    evtWorldSourceTransformPrefix_t *source;

    worldUnit = dds3SearchSceneBasic(dds3GetControlScene(), worldObjectKey, 0x10);
    if (worldUnit != NULL) {
        source = worldUnit->workdata;
        evtSetUnitMove1(work, motionSubmode, (s128 *)&source->position, unused, frames, directionOffset, callback, unusedLast);
        work->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_WORLD_NODE;
        work->linkedUnit = worldUnit;
    }
}

/* body: matched in smt3 @ 9401f79e: src/smt3/event/evtUnitManager.c:1245, near, same function 1.00 (words 145/145, calls 1.00, shape 1.00) */
void evtSetUnitMove3(evtUnit_t *h, s32 pathID, s32 para1, s32 para2, s32 movesys, s32 movedir, s32 rotsys) {
    dds3Basic_t *pathSource;
    dds3PathBasicControl_t *path;

    pathSource = dds3SearchSceneBasic(dds3GetControlScene(), pathID, BASIC_TYPE_PATH);
    if (pathSource == NULL) {
        return;
    }
    if (h->pathHandle != 0) {
        dds3DeletePathControl(EE_ADDR_AS(dds3PathBasicControl_t *, h->pathHandle));
    }
    path = dds3AllocPathBasicWorkData(pathSource);
    h->pathHandle = EE_ADDR_S(path);
    h->pathSpeed = 40.0f / GetSplineLength(path);
    if (movedir == 0) {
        dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, 0.0f);
        dds3PathBasicControlAlterMode(path, 0);
    } else {
        dds3PathBasicControlSetRatio((dds3PathBasicControl_t *)path, 1.0f);
        dds3PathBasicControlAlterMode(path, 1);
        h->pathSpeed = -h->pathSpeed;
    }
    switch (movesys) {
    case 0:
        h->motionSubmode = 0;
        h->flags &= ~2;
        break;
    case 1:
        h->motionSubmode = 3;
        h->flags |= 2;
        break;
    }
    switch (movedir) {
    case 0:
        h->flags &= ~EVT_UNIT_FLAG_PATH_REVERSE;
        break;
    case 1:
        h->flags |= EVT_UNIT_FLAG_PATH_REVERSE;
        break;
    }
    switch (rotsys) {
    case 0:
        h->flags &= ~8;
        h->flags &= ~0x10;
        break;
    case 1:
        h->flags |= 8;
        h->flags &= ~0x10;
        break;
    case 2:
        h->flags &= ~8;
        h->flags |= 0x10;
        break;
    }
    h->motionState = EVT_UNIT_MOTION_STATE_SOURCE;
    h->transitionSourceKind = EVT_UNIT_TRANSITION_SOURCE_PATH_CURVE;
    h->linkedUnit = pathSource;
    dds3PathBasicControlGetPos(path);
    SDF_VU0_STOREVECTOR(h->targetVector);
    h->motionParameter = para1;
    h->directionOffset = para2;
    h->motionCallback = 0;
    h->motionTicks = 0;
}

INCLUDE_ASM(const s32, "event/evtUnitManager", CheckMoveEndBustup);

INCLUDE_ASM(const s32, "event/evtUnitManager", UnitMoveTypeBustup);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "event/evtUnitManager", evtSetUnitMoveBustup);
#else
INCLUDE_ASM(const s32, "event/evtUnitManager", evtLeaf);
#endif

/* body: matched in smt3 @ 9401f79eb: src/smt3/event/evtUnitManager.c:1310, match, same function 1.00 (words 9/9, calls 1.00, shape 1.00) */
s32 evtSetUnitAction(evtUnit_t *work, s32 (*callback)(evtUnit_t *, s32)) {
    s32 ret = 0;

    if (callback != 0) {
        work->motionCallback = callback;
        work->motionTicks = 0;
        work->motionState = EVT_UNIT_MOTION_STATE_VALUE;
        ret = 1;
    }
    return ret;
}

#ifdef VERSION_RAIDOU1
#ifdef NON_MATCHING
/* 18 of 442 words differ: same length; the case 2 tail orders its flag load and compare differently, and the 0x1000000 test keeps a mask register in retail. */
s32 evtUnitManagerPrimaryFunc(dds3PoseSet_t *unused, dds3Basic_t *object) {
    s32 moving = 0;
    s32 wobble = 0;
    dds3UnitObjectBasicWorkData_t *data = object->workdata;
    evtUnit_t *unit = data->hUnit;
    mdlObj_t *model;
    s32 i;
    u8 enabled;
    s32 (*callback)(evtUnit_t *, s32);

    if (unit == NULL) {
        return 0;
    }
    model = unit->owner;
    UnitAnime(unit);
    switch (unit->motionState) {
    case EVT_UNIT_MOTION_STATE_IDLE:
    case EVT_UNIT_MOTION_STATE_VECTOR:
        if (unit->flags & 0x80) {
            if (unit->flags & 1) {
                mdlSmoothLoopMotion(model, 0, unit->motionSetup.motionId, (f32)unit->motionSetup.startFrame, (f32)unit->motionSetup.endFrame);
            } else {
                mdlSmoothSingleMotion(model, 0, unit->motionSetup.motionId, (f32)unit->motionSetup.startFrame, (f32)unit->motionSetup.endFrame);
            }
            unit->flags &= ~0x80;
        } else if (unit->flags & 0x20) {
            s32 currentMotion = model->current.h.motionIndex;

            if (unit->waitMotion != currentMotion) {
                if (model->first->stat == 5) {
                    mdlSmoothLoopMotion(model, 0, unit->waitMotion, (f32)unit->waitBlendStart, (f32)unit->waitBlendEnd);
                    model->first->speed = unit->transitionMotionScale;
                    unit->motionTicks = 0;
                }
            } else {
                if (model->first->stat == 5) {
                    mdlSmoothLoopMotion(model, 0, currentMotion, (f32)unit->waitBlendStart, (f32)unit->waitBlendEnd);
                    model->first->speed = unit->transitionMotionScale;
                    unit->motionTicks = 0;
                }
            }
        }
        if (unit->flags & 0x40) {
            wobble = 1;
        } else {
            wobble = 0;
        }
        if (unit->motionState == EVT_UNIT_MOTION_STATE_VECTOR) {
            if (evtUnitRotationQuat(unit, unit->targetVector, (f32)unit->motionParameter / 100.0f) != 0) {
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
            }
        }
        break;
    case EVT_UNIT_MOTION_STATE_SOURCE:
        if (evtCheckMoveEnd(unit) != 1 || evtGetNextMovePos(unit) != 0) {
            if (unit->motionSetup.motionId != model->current.h.motionIndex) {
                if (unit->flags & 1) {
                    mdlSmoothLoopMotion(model, 0, unit->motionSetup.motionId, (f32)unit->motionSetup.startFrame, (f32)unit->motionSetup.endFrame);
                } else {
                    mdlSmoothSingleMotion(model, 0, unit->motionSetup.motionId, (f32)unit->motionSetup.startFrame, (f32)unit->motionSetup.endFrame);
                }
            }
            switch (unit->motionSubmode) {
            case 0:
            case 1:
                moving = evtUnitMoveType0(unit);
                if (unit->motionSubmode == 1) {
                    sdfVector4_t v;
                    v.x = 0.0f;
                    v.y = unit->moveY;
                    v.z = 0.0f;
                    v.w = 0.0f;
                    dds3GetPosPoseSetMCR(unit->effObj, &v);
                }
                break;
            case 2:
                break;
            case 3:
                moving = evtUnitMoveTypePathTrace(unit);
                break;
            default:
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
                break;
            }
            callback = unit->motionCallback;
            if (callback != NULL && callback(unit, 0) == 1) {
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
            }
        } else {
            unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        }
        break;
    case EVT_UNIT_MOTION_STATE_MOTION:
        if (unit->motionTicks == 0) {
            datDebugPrintf("set mot %X\n", unit->motionSetup.motionId);
            if (unit->flags & 1) {
                mdlSmoothLoopMotion(model, 0, unit->motionSetup.motionId, (f32)unit->motionSetup.startFrame, (f32)unit->motionSetup.endFrame);
            } else {
                mdlSmoothSingleMotion(model, 0, unit->motionSetup.motionId, (f32)unit->motionSetup.startFrame, (f32)unit->motionSetup.endFrame);
            }
            unit->flags &= ~0x80;
        } else if (model->first->stat == 5) {
            unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        } else if ((unit->flags & 0x800000) && (f32)unit->motionEndFrame <= model->first->frame) {
            s32 currentMotion;

            datDebugPrintf("motion end!!\n");
            unit->motionState = EVT_UNIT_MOTION_STATE_VALUE;
            currentMotion = model->current.h.motionIndex;
            unit->flags &= ~0x800000;
            if (unit->waitMotion != currentMotion) {
                datDebugPrintf("wait motion %X\n", unit->waitMotion);
                mdlSmoothLoopMotion(model, 0, unit->waitMotion, (f32)unit->waitBlendStart, (f32)unit->waitBlendEnd);
                model->first->speed = unit->transitionMotionScale;
                unit->motionTicks = 0;
            }
        }
        break;
    case EVT_UNIT_MOTION_STATE_VALUE:
        callback = unit->motionCallback;
        if (callback != NULL) {
            if (callback(unit, 0) == 1) {
                unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
            }
        } else {
            unit->motionState = EVT_UNIT_MOTION_STATE_IDLE;
        }
        break;
    default:
        return 0;
    }
    if (unit->flags & 0x1000000) {
        if (CheckMoveEndBustup(unit) == 1) {
            unit->flags &= ~0x1000000;
        } else {
            UnitMoveTypeBustup(unit);
        }
    }
    if (unit->flags & 0x400000) {
        wobble = 1;
    }
    if (wobble != 0) {
        unit->wobblePhase += 0.104719755f;
        if (6.2831853f <= unit->wobblePhase) {
            unit->wobblePhase -= 6.2831853f;
        }
        unit->effObj->poseset->flag = (unit->effObj->poseset->flag & ~3) | 1;
    }
    for (i = 1; i < 12; i++) {
        enabled = unit->slotFlags[i] & 1;
        if (enabled != 0) {
            if (unit->slotFlags[i] & 2) {
                unit->slotFlags[i] &= ~2;
                datDebugPrintf("set exmot %d %X\n", i, unit->slotA[i]);
                if (unit->slotFlags[i] & 8) {
                    mdlSmoothLoopMotion(model, i, unit->slotA[i], (f32)unit->slotB[i], (f32)unit->slotC[i]);
                } else {
                    mdlSmoothSingleMotion(model, i, unit->slotA[i], (f32)unit->slotB[i], (f32)unit->slotC[i]);
                }
            }
            if (mdlChkMotionEndEx(model, i) != 0 && (unit->slotFlags[i] & 4)) {
                datDebugPrintf("exmot end!\n");
                mdlSmoothLoopMotion(model, i, unit->tableValues[i], 0.0f, 0.0f);
            }
        }
    }
    MakeLightPacket(unit);
    if (unit->motionTicks < 0x7FFF) {
        unit->motionTicks++;
    }
    if (moving != 0) {
        if (unit->movingTicks < 0x7FFF) {
            unit->movingTicks++;
        }
        unit->idleTicks = 0;
    } else {
        if (unit->idleTicks < 0x7FFF) {
            unit->idleTicks++;
        }
        if (unit->idleTicks >= 6) {
            unit->movingTicks = 0;
        }
    }
    return moving;
}
#else
INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitManagerPrimaryFunc);
#endif
#else
INCLUDE_ASM(const s32, "event/evtUnitManager", evtUnitManagerPrimaryFunc);
#endif

void evtInitCustomSceneLight(void) {
    s32 i;

    for (i = 0; i < 7; i++) {
        evtInitCustomSceneLightSub(CustomSceneLightArray[i].color, CustomSceneLightArray[i].dir,
                                   CustomSceneLightArray[i].ambient, &CustomSceneLightArray[i].soften,
                                   &CustomSceneLightArray[i].bias);
        CustomSceneLightArray[i].lightType = 0;
        CustomSceneLightArray[i].unitHandle = NULL;
    }
}

void evtSetCustomSceneLightData(s32 index, s32 lightType, evtUnit_t *h, f32 (*color)[4], f32 (*dir)[4], f32 *ambient) {
    s32 i;

    if (index >= 7) {
        return;
    }
    for (i = 0; i < 3; i++) {
        CustomSceneLightArray[index].color[i][0] = color[i][0];
        CustomSceneLightArray[index].color[i][1] = color[i][1];
        CustomSceneLightArray[index].color[i][2] = color[i][2];
        CustomSceneLightArray[index].color[i][3] = 0.0f;
        CustomSceneLightArray[index].dir[i][0] = dir[i][0];
        CustomSceneLightArray[index].dir[i][1] = dir[i][1];
        CustomSceneLightArray[index].dir[i][2] = dir[i][2];
        CustomSceneLightArray[index].dir[i][3] = dir[i][3];
    }
    CustomSceneLightArray[index].ambient[0] = ambient[0];
    CustomSceneLightArray[index].lightType = lightType;
    CustomSceneLightArray[index].ambient[1] = ambient[1];
    CustomSceneLightArray[index].ambient[2] = ambient[2];
    CustomSceneLightArray[index].ambient[3] = 1.0f;
    if (lightType == 3) {
        CustomSceneLightArray[index].unitHandle = h;
    } else {
        CustomSceneLightArray[index].unitHandle = NULL;
    }
}

void evtSetCustomSceneLightData_SoftenAndBias(s32 index, f32 soften, f32 bias) {
    if (index < 7) {
        CustomSceneLightArray[index].soften = soften;
        CustomSceneLightArray[index].bias = bias;
    }
}

s32 evtGetCustomSLightData(evtUnit_t *h, f32 *dir) {
    s32 found = -1;
    s32 i;

    for (i = 0; i < 7; i++) {
        if (CustomSceneLightArray[i].lightType == 3 && CustomSceneLightArray[i].unitHandle == h) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        for (i = 0; i < 7; i++) {
            if (CustomSceneLightArray[i].lightType == 2) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            return 0;
        }
    }
    dir[0] = CustomSceneLightArray[found].dir[0][0];
    dir[1] = CustomSceneLightArray[found].dir[0][1];
    dir[2] = CustomSceneLightArray[found].dir[0][2];
    return 1;
}

void evtMakeCustomSLightPacket(evtUnit_t *h) {
    sdf3DLight_t source[3];
    sdfVector4_t ambient;
    sdf3DLight_t *lights[3] = { &source[0], &source[1], &source[2] };
    sdf3DLight_t *light;
    f32 *channel;
    s32 index;
    s32 i;

    index = -1;
    for (i = 0; i < 7; i++) {
        if (CustomSceneLightArray[i].lightType == 3 && CustomSceneLightArray[i].unitHandle == h) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        for (i = 0; i < 7; i++) {
            if (CustomSceneLightArray[i].lightType == 2) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            return;
        }
    }
    light = source;
    for (i = 0; i < 3; i++) {
        light->color.x = CustomSceneLightArray[index].color[i][0];
        light->color.y = CustomSceneLightArray[index].color[i][1];
        light->color.z = CustomSceneLightArray[index].color[i][2];
        light->v.x = CustomSceneLightArray[index].dir[i][0];
        light->v.y = CustomSceneLightArray[index].dir[i][1];
        light->v.z = CustomSceneLightArray[index].dir[i][2];
        light->color.w = 0.0f;
        light->v.w = 0.0f;
        light++;
    }
    ambient.x = CustomSceneLightArray[index].ambient[0];
    ambient.y = CustomSceneLightArray[index].ambient[1];
    ambient.z = CustomSceneLightArray[index].ambient[2];
    ambient.w = 1.0f;
    channel = &ambient.x;
    for (i = 2; i >= 0; i--) {
        if (1.0f < *channel) {
            *channel = 1.0f;
        }
        channel++;
    }
    sdf3DMakeLightParallelPacket(h->endpointWork, lights, &ambient);
    h->lightPacket = h->endpointWork;
}

s32 evtGetCustomSLight_SoftenAndBias(evtUnit_t *h, f32 *soften, f32 *bias) {
    s32 i;

    for (i = 0; i < 7; i++) {
        if (CustomSceneLightArray[i].lightType == 3 && CustomSceneLightArray[i].unitHandle == h) {
            *soften = CustomSceneLightArray[i].soften;
            *bias = CustomSceneLightArray[i].bias;
            return 1;
        }
    }
    for (i = 0; i < 7; i++) {
        if (CustomSceneLightArray[i].lightType == 2) {
            *soften = CustomSceneLightArray[i].soften;
            *bias = CustomSceneLightArray[i].bias;
            return 1;
        }
    }
    return 0;
}
