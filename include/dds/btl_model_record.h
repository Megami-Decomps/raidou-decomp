#ifndef DDS_BTL_MODEL_RECORD_H

#define DDS_BTL_MODEL_RECORD_H
#include "common.h"

typedef s16 BtlMotionEndBehavior;

typedef u16 BtlActorShadowKind;

#define BTL_MOTION_END_NORMAL 0

#define BTL_MOTION_END_PAUSE 1

#define BTL_MOTION_END_OUT 2

#define BTL_MOTION_END_STOP 3

#define BTL_ACTOR_SHADOW_NONE 0

#define BTL_ACTOR_SHADOW_SMALL 1

#define BTL_ACTOR_SHADOW_MEDIUM 2

#define BTL_ACTOR_SHADOW_LARGE 3

#define BTL_ACTOR_SHADOW_EXTRA_LARGE 4

/* One motion row in the side-indexed model parameter bank. */
typedef struct BtlActorMotionSlot {
    s16 moveKind;            /* 0x00: UV_MV mode */
    u16 frameCount;
    BtlMotionEndBehavior endBehavior; /* 0x04: UV_END mode */
    s16 alphaStartFrame;
    f32 alphaFrameScale;
    f32 muzzleGuideScale; /* 0x0C: scales the muzzle-position debug guide. */
    s16 alphaDuration;
    s16 unk12;
} BtlActorMotionSlot;

/* The bank getter strides by 0x270; the editor copies the complete record.
 * The header is followed by all 29 motion rows, including opaque parameters.
 */
typedef struct BtlActorStatusRecord {
    f32 position[3];
    u8 pad0C[4];
    f32 scale;               /* 0x10: model scale */
    f32 z;
    f32 reach;               /* 0x18: copied into BtlActorTransformParameters. */
    f32 height;              /* 0x1C: copied into BtlActorTransformParameters. */
    f32 shotRadius;         /* 0x20: model-editor label is "1 shot radius". */
    f32 positionGuideScale; /* 0x24: scales the selected position debug guide. */
    BtlActorShadowKind shadowKind;
    u16 model;
    BtlActorMotionSlot motions[29]; /* 0x2C */
} BtlActorStatusRecord;

typedef char BtlActorMotionSlotSizeCheck[sizeof(BtlActorMotionSlot) == 0x14 ? 1 : -1];

typedef char BtlActorStatusRecordSizeCheck[sizeof(BtlActorStatusRecord) == 0x270 ? 1 : -1];

#endif
