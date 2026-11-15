#include "common.h"

#include "effBattle.h"
#include "fileManager.h"
#include "newdata.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "code_0027A1D0.h"

typedef s8 BtlCameraCursorPosePhase;

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

/* Eight-byte rows from /battle/EFFECT.TBL. */
typedef struct BtlEffectActionRecord {
    u8 unknown00[2];
    u16 kind; /* 0x02 */
    s8 participantSelectors[3]; /* 0x04: -1 omits the corresponding participant. */
    u8 unknown07;
} BtlEffectActionRecord;

typedef char BtlEffectActionRecordSizeCheck[
    sizeof(BtlEffectActionRecord) == 8 ? 1 : -1];

extern void effObjSetOpacityPassEnabled(u32 enabled);
extern BtlCameraCursorView btlCameraCursor;

/* SYSEFF metadata and runtime registrations share these indices. */
enum { BTL_COMMAND_UNIT_EFFECT_SOUND_SLOT = 0x2C };

struct evtUnitHandle_t;

typedef struct BattleVoiceEntry {
    u8 format;
    u8 pad01[3];
    char fileName[0xC];
} BattleVoiceEntry;

#define CURSOR (&btlCameraCursor)

/* Camera framing preset: from and to quaternions, their distance multipliers,
 * the camera parameter copied to the action, and padding. */
typedef struct CameraFramePose {
    f32 fromQuat[4];
    f32 toQuat[4];
    f32 fromDistance;
    f32 toDistance;
    f32 parameter;
    f32 pad2C;
} CameraFramePose;

#define BTL_APPROACH_DIST_START 0.55f

#define BTL_APPROACH_DIST_END 0.6f

#define BTL_APPROACH_PITCH_START 0.6108652f /* 35 degrees */

#define BTL_APPROACH_PITCH_END 0.6108652f

/* The pair preset row with an integer frame count in place of the motion parameter. */
typedef struct BattlePairCameraFramePreset {
    f32 fromQuaternion[4];
    f32 toQuaternion[4];
    f32 fromDistanceScale;
    f32 toDistanceScale;
    f32 fromHeightScale;
    s32 frames;
} BattlePairCameraFramePreset;

/* Four retail flag/boolean sites: 1E5CE4/1E5CF0/1E5D3C/1E5D48; matched DDS2 predicate. */
static inline s32 btlHasFlag(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

typedef struct BtlCursorChoices {
    u16 values[3][3][8];
} BtlCursorChoices;

typedef char CameraCursorExtent[(sizeof(BtlCameraCursorView) == 0x130) ? 1 : -1];

typedef struct TintTransitionState {
    u32 sourceAlpha;
    u32 targetAlpha;
    u16 framesRemaining;
    u16 totalFrames;
} TintTransitionState;

void btlQueueTintTransitionToZero(u16 frames);

struct dds3Effect_Filter_t;

void mapBadEffDrawTask(void);

typedef struct SoundTaskArgs {
    BtlUnit *actor;
    s32 option;
    u32 slot;
    s32 waitFrames;
} SoundTaskArgs;

enum {
    BATTLE_FORMATION_ACTION_FIRST = 0x1AB,
    BATTLE_FORMATION_ACTION_COUNT = 0x55
};

typedef struct BattleFormationActionTable {
    BtlEffectActionRecord rows[BATTLE_FORMATION_ACTION_COUNT];
} BattleFormationActionTable;

typedef char BattleFormationActionTableSizeCheck[(sizeof(BattleFormationActionTable) == 0x2A8) ? 1 : -1];

typedef struct BattleFormationActionArgs {
    BtlTask *link;
    BtlUnit *first;
    BtlUnit *second;
    u32 actionId;
    u32 unk10;
} BattleFormationActionArgs;

typedef char BattleFormationActionArgs_size[(sizeof(BattleFormationActionArgs) == 0x14) ? 1 : -1];

void mapBadEffDrawTask(void) {
    btlQueueTintTransitionToZero(0);
    effObjSetOpacityPassEnabled(1);
}
