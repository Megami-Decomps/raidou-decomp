#ifndef DDS_BTL_CAMERA_CURSOR_H

#define DDS_BTL_CAMERA_CURSOR_H
#include "common.h"

typedef s8 BtlCameraCursorPosePhase;

#define BTL_CAMERA_CURSOR_POSE_PHASE_INITIAL 0

#define BTL_CAMERA_CURSOR_POSE_PHASE_TRANSITION 1

#define BTL_CAMERA_CURSOR_POSE_PHASE_ENDPOINT_CAPTURED 2

/* Modes 1, 3, and 5 use TRANSITION while the desired distance changes. */

/* Shared battle-camera script cursor and path state (0x130 bytes).
 * Both title providers use the same field offsets. */
typedef struct BtlCameraCursorView {
    u16 useTimedScript;     /* 0x00 */
    s16 frame;              /* 0x02 */
    s16 mode;
    s16 index;
    union {
        u16 unk_08;
        s8 category;
    };
    s16 scriptSetIndex;    /* 0x0A */
    s16 scriptIndex;       /* 0x0C */
    u16 unk_0E;
    u16 kind;              /* 0x10: path kind */
    BtlCameraCursorPosePhase posePhase;
    s8 counter;
    s8 busy;                /* 0x14: nonzero suspends camera updates */
    s8 restoreAlternate;   /* 0x15: snapshot restore selector */
    u8 pad16[0xA];
    f32 pathEnd[4];         /* 0x20 */
    f32 pathStart[4];       /* 0x30 */
    /* Where the camera looks and where it stands, from and to, as retail's debug
     * print labels them ("[Target]", "[Camera]"): the pose's target moves along
     * targetFrom..targetTo, and the camera (the target less the pose's direction
     * times its distance) along cameraFrom..cameraTo. */
    f32 targetFrom[4];      /* 0x40 */
    f32 targetTo[4];        /* 0x50 */
    f32 cameraFrom[4];      /* 0x60 */
    f32 cameraTo[4];        /* 0x70 */
    f32 direction[4];       /* 0x80 */
    f32 distance;           /* 0x90 */
    u8 pad94[0xC];
    f32 fov;               /* 0xA0 */
    u8 padA4[0xC];
    f32 startFrame;        /* 0xB0 */
    u8 padB4[0xC];
    f32 duration;          /* 0xC0 */
    u8 padC4[0xC];
    u8 parameterD0;        /* 0xD0: unsigned serialized flag */
    u8 padD1[0xF];
    f32 parameterE0;       /* 0xE0 */
    u8 padE4[0xC];
    f32 blendRate;         /* 0xF0 */
    u8 padF4[0xC];
    f32 rangeStart;        /* 0x100 */
    u8 pad104[0xC];
    f32 rangeEnd;          /* 0x110 */
    s32 parameterIndex114;
    s32 parameterIndex118;
    s32 parameterIndex11C;
    s32 parameterIndex120;
    s32 parameterIndex124;
    s32 parameterIndex128;
    s32 parameterIndex12C;
} BtlCameraCursorView;

typedef char BtlCameraCursorViewSizeCheck[(sizeof(BtlCameraCursorView) == 0x130) ? 1 : -1];

typedef char BtlCameraCursorViewPosePhaseSizeCheck[
    (sizeof(((BtlCameraCursorView *)0)->posePhase) == 1) ? 1 : -1];

#endif /* DDS_BTL_CAMERA_CURSOR_H */
