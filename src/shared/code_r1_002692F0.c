#include "common.h"

#ifdef VERSION_RAIDOU1
extern void *sbtlRefSbtl38(void);
extern void sbtlSetCutScroll(void *work);
#else
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif
#endif

#ifdef VERSION_RAIDOU1
#ifdef NON_MATCHING
/* Same bytes as retail, but +0x40 is a raw offset into the work block that
 * sbtlRefSbtl38 returns, whose struct is not defined yet. */
s32 btlRunCameraMotionResetTask(void) {
    void *work = sbtlRefSbtl38();

    sbtlSetCutScroll((u8 *)work + 0x40);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_002692F0", btlRunCameraMotionResetTask);
#endif
#else
#include "dds3obj.h"
#include "effBattle.h"
#include "event.h"
#include "model.h"
#include "newdata.h"
#include "sdf_model_packet_callback.h"
#include "sdk_libc.h"
#include "dds/evt_unit.h"

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

enum SdfMotionStateValue {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
};


#ifndef DDS_ENUM_EvtUnitMotionState
#define DDS_ENUM_EvtUnitMotionState
typedef int EvtUnitMotionState;
#endif

#include "dds/btl_state.h"

/* SYSEFF metadata and runtime registrations share these indices. */
enum { BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT = 0x26 };

extern void btlResetCameraMotion(BtlLinkedCommand *);

typedef struct BtlRotationTaskArgs BtlRotationTaskArgs;

struct evtUnitHandle_t;

typedef struct BtlRotationTaskArgs {
    s128 from;
    s128 to;
    f32 rate;
    f32 t;
    s32 count;
    BtlUnit *unit;
} BtlRotationTaskArgs;

typedef char BtlRotationTaskArgsSizeCheck[
    sizeof(BtlRotationTaskArgs) == 0x30 ? 1 : -1];

/* Consume one item when its table flag 4 is set, then signal task completion. */
typedef struct {
    BtlUnit *actor;
    s32 itemId;
} BtlCommandItemWork;

/* Kind-5 world-node payload attaches the event unit at +8. */
typedef struct EventUnitData {
    u8 pad00[8];
    evtUnitHandle_t *unit;
} EventUnitData;

/* Draw the unit's transparency model into its mirror's packet buffer, then draw the mirror from it. Either
 * model is created (and the update ends) on the first frame it is missing. */

/* Complete model loading, cross-fade the retained actor, and release it. */

typedef struct BtlUnitLightDirectionTaskArgs {
    f32 direction[4];
    s32 directionMode;
    BtlUnit *unit;
} BtlUnitLightDirectionTaskArgs;

typedef char BtlUnitLightDirectionTaskArgs_size_check[
    sizeof(BtlUnitLightDirectionTaskArgs) == 0x18 ? 1 : -1];

typedef struct BtlDamageShakeArgs {
    BtlUnit *unit;
    f32 amplitude;
    s32 tick;
} BtlDamageShakeArgs;

typedef struct GunLoadArgs {
    BtlUnit *unit;
    struct FileRequest *request;
} GunLoadArgs;

typedef struct BtlActorTransparencyArgs {
    BtlUnit *unit;
} BtlActorTransparencyArgs;

u32 btlRunCameraMotionResetTask(void);
extern f32 btlCameraLocalUpVector[4], btlCameraOutputRotation[4];
extern f32 btlWorldCameraStartOffset[4], btlWorldCameraStartRotation[4];
extern f32 btlCreatedCameraTarget[4], btlCreatedCameraRotation[4];
void btlResetCameraMotion(BtlLinkedCommand *action);

u32 btlRunCameraMotionResetTask(void) {
    BtlState *work = (BtlState *)btlGetRuntime();

    btlResetCameraMotion(&work->cameraCommand);
    return 1;
}
#endif
