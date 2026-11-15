#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "effBattle.h"
#include "fileManager.h"
#include "newdata.h"
#include "sdk_libc.h"
#include "dds/btl.h"

extern void *memcpy(void *, const void *, unsigned int);

#include "dds/btl_camera_cursor.h"
#include "dds/btl_effect_table.h"

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
