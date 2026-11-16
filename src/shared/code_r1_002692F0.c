#include "common.h"

#ifdef VERSION_RAIDOU1
#include "sBtlScene.h"
#include "sBtlUnit.h"
#include "sbtlWork.h"

s32 btlRunCameraMotionResetTask(void) {
    sbtlSetCutScroll(&sbtlRefSbtl38()->pos);
    return 1;
}
#else
#include "dds3obj.h"
#include "effBattle.h"
#include "event.h"
#include "model.h"
#include "newdata.h"
#include "sdf_model_packet_callback.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "kernel.h"
#include "draw3d.h"
#include "basic.h"
#include "code_0027A1D0.h"

#define DDS_ENUM_EvtUnitMotionState

/* Event motion work, not the world-list node which contains its address.
 * Setup opcodes pass this same object to evtSetUnitMotion and
 * evtConfigureUnitMotionSlot; the direction updater passes it to planar aim.
 * targetVector is four floats even when copied as a quadword by VU0/MMI macros.
 * The manager copies the entire object by value.
 * No stronger alignment than the manager's original float layout is implied. */
typedef struct evtUnitHandle_t {
    u32 color;                     /* 0x00 */
    union {
        s32 objectId;               /* Legacy script-facing view at +0x04 */
        u32 firstCurrent;           /* Packed first-colour transition state */
    };
    union {
        u8 pad08[8];
        struct {
            u32 color08;            /* First-colour transition target */
            u32 color0C;            /* First-colour state before target influence */
        };
    };
    f32 vec10[4];                  /* 0x10 */
    f32 vec20[4];                  /* 0x20 */
    f32 vec30[4];                  /* 0x30 */
    union {
        u8 pad40[0x10];
        f32 vec40[4];               /* Direction before target influence */
    };
    u32 color50;                   /* 0x50 */
    union {
        u8 pad54[0xC];
        struct {
            u32 color54;            /* Second-colour transition state */
            u32 color58;            /* Second-colour transition target */
            u32 color5C;            /* Second-colour state before target influence */
        };
    };
    u32 color60;                   /* 0x60: packed color retained during RGB/alpha transitions */
    union {
        u8 pad64[4];
        u32 color64;                /* Destination packed RGB/alpha word */
    };
    struct sdf3DLightParallelPacket_t *endpointWork; /* 0x68: owned lighting packet storage */
    u32 value;                     /* 0x6C */
    f32 targetVector[4];           /* 0x70 */
    dds3Basic_t *effObj;          /* 0x80: kind-5 world node owning this motion work. */
    dds3Basic_t *currentTransitionTarget;  /* 0x84: active target world node */
    dds3Basic_t *previousTransitionTarget; /* 0x88: prior target during blend-in */
    struct mdlObj_t *owner;          /* 0x8C: model context and its SDK motion slots */
    dds3Basic_t *linkedUnit;       /* 0x90: retained world/path transition source */
    s32 (*motionCallback)(struct evtUnitHandle_t *, s32); /* 0x94: returns 1 when motion completes. */
    f32 wobblePhase;               /* 0x98: wrapping radians used by model scale/rotation wobble. */
    s32 unk9C;                     /* 0x9C */
    struct dds3PathBasicControl_t *pathControl; /* 0xA0: owned path-control work */
    f32 pathSpeed;                 /* 0xA4: signed progress increment */
    u32 flags;                     /* 0xA8 */
    s16 motionState;               /* 0xAC */
    s16 transitionSourceKind;      /* 0xAE */
    s16 motionSubmode;             /* 0xB0 */
    s16 motionTicks;               /* 0xB2 */
    s16 motionParameter;           /* 0xB4: frames in setup; direction scale in updater */
    s16 directionOffset;           /* 0xB6: updater multiplies by 0.01 */
    f32 transitionMotionScale;     /* 0xB8: set by evtSetWaitMotionSpeed */
    s16 pendingMotionIndex;        /* 0xBC: script-selected motion for the next blend */
    s16 motionBlendLeadFrames;     /* 0xBE: frames before the motion blend begins */
    s16 motionBlendDurationFrames; /* 0xC0: duration of the motion blend */
    u8 padC2[2];
    s16 motionSetupId;             /* 0xC4: motion selected by evtSetUnitMotion */
    s16 motionSetupStartFrame;     /* 0xC6 */
    s16 motionSetupEndFrame;       /* 0xC8 */
    s16 motionEndFrame;            /* 0xCA: threshold for the timed motion's final phase */
    s16 movingTicks;               /* 0xCC: saturates at 32767 while moving */
    s16 idleTicks;                 /* 0xCE: six idle frames clear movingTicks */
    s8 firstSlot;                  /* 0xD0: consumed by stored-slot activation */
    s8 secondSlot;                 /* 0xD1 */
    u8 padD2;
    u8 shadowLevel;                /* 0xD3: timeline-key value passed to fldDrawShadowDisc. */
    f32 shadowY;                    /* 0xD4: float parameter written by timeline keys */
    u32 referenceAngleCacheFlags;   /* 0xD8: bit 0 marks a valid cached angle. */
    f32 cachedReferenceAngleDegrees; /* 0xDC: cached reference angle in degrees. */
    u8 slotFlags[12];              /* 0xE0 */
    u8 padEC[4];
    s16 tableValues[1];            /* 0xF0: minimum known extent; script index is unchecked */
    u8 padF2[0x16];
    s16 slotA[12];                 /* 0x108: opaque per-slot parameters */
    s16 slotB[12];                 /* 0x120 */
    s16 slotC[12];                 /* 0x138 */
    union {
        s16 unused150;               /* Legacy signed view */
        u16 colorFramesRemaining;  /* 0x150: colour interpolation countdown */
    };
    u16 directionFramesRemaining; /* 0x152: direction interpolation countdown */
    union {
        u8 pad154[8];
        struct {
            u16 rgbElapsed;
            u16 rgbDuration;
            u16 alphaElapsed;
            u16 alphaDuration;
        };
    };
    s16 transitionElapsed;         /* 0x15C */
    union {
        s16 transitionDuration;    /* Legacy setter view, retaining the stored bits */
        u16 transitionFrameCount;  /* 0x15E: duration consumed as unsigned frames */
    };
    f32 speedY;                    /* 0x160 */
    s16 stepCount;                 /* 0x164 */
    u8 pad166[10];
} evtUnitHandle_t;

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

/* Camera pose payload (0x28); command members occupy SDK-aligned storage. */
typedef struct SBTLCAM_STATE {
    f32 position[4];  /* 0x00 */
    f32 direction[4]; /* 0x10 */
    f32 distance;     /* 0x20 */
    f32 fov;          /* 0x24 */
} SBTLCAM_STATE;

/* Exponential interpolation also supplies the actor-panel expansion scale. */
typedef struct SBTLEXPONENTIAL_RANGE {
    f32 start; /* 0x00: span */
    f32 end;   /* 0x04: current progress */
} SBTLEXPONENTIAL_RANGE;

typedef struct SBTLSCALAR_RANGE {
    f32 start;       /* 0x00: initial span */
    f32 end;         /* 0x04: remaining span */
    f32 inverseSpan; /* 0x08: quadratic acceleration */
    f32 velocity;    /* 0x0C */
    f32 value;       /* 0x10: accumulated progress */
} SBTLSCALAR_RANGE;

/* Returns the raw battle-runtime base address; callers choose the needed view. */
void *btlGetRuntime(void);

/* The override rejects on zero; nonzero permits the ordinary checks to run. */
typedef s32 BtlBattleRecordEligibilityOverrideResult;

typedef struct SBTLITEM_DROP {
    u16 id;
    u8 count;
    u8 pad03;
} SBTLITEM_DROP;

/* Embedded debug-list cursor state; the row limit is independent of count. */
typedef struct SBTLDEBUGMENU_CURSOR {
    u32 count;
    u32 cursor;
    u32 top;
    u32 rows;
} SBTLDEBUGMENU_CURSOR;

typedef char BtlDebugMenuCursorExtent[(sizeof(SBTLDEBUGMENU_CURSOR) == 0x10) ? 1 : -1];

/* Generic first word shared by actor-effect and boss-selection allocations. */
typedef struct SBTLEFFECT_HEADER {
    union {
        u32 owner;
        struct SBTL_UNIT *actor;
        u8 statIndex;
        u16 formCount; /* HARI1 reads and advances only this halfword. */
    };
} SBTLEFFECT_HEADER;

typedef char SBTLEFFECT_HEADERSizeCheck[(sizeof(SBTLEFFECT_HEADER) == 4) ? 1 : -1];

typedef struct SBTLLINKED_COMMAND {
    SBTLCAM_STATE camera;       /* 0x00 */
    u8 pad28[8];
    SBTLCAM_STATE frontCamera;  /* 0x30 */
    u8 pad58[0x68];
    SBTLCAM_STATE backCamera;   /* 0xC0 */
    u8 padE8[8];
    u32 flags;               /* 0xF0: includes facing-direction bit 0x200 */
    SBTL_TASK *task;            /* 0xF4: fallback actor is task->unit */
    SBTL_UNIT *linkedA;         /* 0xF8 */
    SBTL_UNIT *linkedB;         /* 0xFC */
    SBTL_UNIT *selectedUnit;    /* 0x100: actor selected for camera setup. */
    u32 status;              /* 0x104: command status used by callback dispatch. */
    s32 actionKind;          /* 0x108: previous camera action kind. */
    u16 stepKind;             /* 0x10C: camera setup and cursor step selector. */
    u8 pad10E[2];
    s32 state;               /* 0x110: aim waits for 0x1E, then resets this */
    s32 actionCode;          /* 0x114 */
    SBTLINDEX_LIST *targetList; /* 0x118: indexed target list */
    s32 motionProgress;      /* 0x11C: one-shot aim latch */
    u8 pad120[8];
    union {
        s32 progressBits;
        f32 progress;
    };                      /* 0x128: initialized as bits, interpolated as float */
    s32 durationFrames;       /* 0x12C */
    f32 motionParameter;     /* 0x130: aim setup stores 10 */
    SBTLEXPONENTIAL_RANGE exponentialRange; /* 0x134 */
    SBTLSCALAR_RANGE quadraticRange;        /* 0x13C */
} SBTLLINKED_COMMAND;

typedef BtlBattleRecordEligibilityOverrideResult (*BtlBattleRecordEligibilityOverrideCallback)(
    struct SBTL_UNIT *actor, struct SBTL_UNIT *target, s32 speciesIndex);

/* Battle model-debug editor: phase, list cursors, selected unit and name tables.
 * Indexed accesses address these
 * string-address tables relative to this nested record. */
typedef struct SBTLDEBUGMENU_WORK {
    s32 phase; /* Model-debug editor initialization, selection, submenus and exit wait. */
    SBTLDEBUGMENU_CURSOR actionMenu; /* 0x04: seven debug actions. */
    SBTLDEBUGMENU_CURSOR modelMenu; /* 0x14: enemy/party model selection. */
    u8 pad24[0x10];
    SBTLDEBUGMENU_CURSOR motionMenu; /* 0x34 */
    SBTLDEBUGMENU_CURSOR gunMenu; /* 0x44 */
    u8 pad54[0x10];
    SBTL_UNIT *selectedUnit; /* 0x64 */
    s32 partyModelNames[0x20]; /* 0x68 */
    s32 enemyModelNames[0x180]; /* 0xE8 */
    s32 gunNames[0x20]; /* 0x6E8 */
} SBTLDEBUGMENU_WORK;

/* Observed fields of the singleton battle work returned by btlGetRuntime.
 * This is one object, not separate script/event/actor-update contexts. Holes
 * remain opaque. */
typedef struct SBTL_STATE {
    f32 position[4]; /* 0x00: battle origin added to the camera offset at 001DC11C. */
    f32 baselineLightDirection[4];
    f32 baselineLightColor[4];
    f32 baselineAmbientColor[4];
    f32 lightDirection[4];
    f32 lightColor[4]; /* 0x50: scene light color used by battle light transitions. */
    f32 ambientColor[4]; /* 0x60: default ambient color used by battle light transitions. */
    /* The same command payload is passed to the action-camera helpers. */
    SBTLLINKED_COMMAND cameraCommand; /* 0x70..0x1BF */
    s16 eventPackId; /* 0x1C0: signed event-pack key, or -1 when absent */
    u8 pad1C2[2];
    u32 scriptFlags; /* 0x1C4 */
    u32 eventFlags; /* 0x1C8 */
    s16 eventActive; /* 0x1CC */
    u8 pad1CE[2];
    s32 eventAction; /* 0x1D0 */
    struct sndMixer_t *eventResult; /* 0x1D4 */
    struct sndMixer_t *eventRequest; /* 0x1D8 */
    void *eventData; /* 0x1DC */
    SBTL_UNIT *eventUnit; /* 0x1E0 */
    s32 sequenceHandle; /* 0x1E4 */
    s32 scriptHandle; /* 0x1E8 */
    struct sdfMemHandle_t *eventAssets; /* 0x1EC */
    s32 frame; /* 0x1F0 */
    u32 battleFlags; /* 0x1F4 */
    u32 commandRestrictFlags; /* 0x1F8: bit 0x10 blocks commands with the +0x30 restriction */
    u32 runtimeFlags; /* 0x1FC: battle runtime and command flags. */
    struct dds3Basic_t *cameraObject; /* 0x200: world camera stored at 001DC1EC. */
    struct dds3Basic_t *sceneLightNode; /* 0x204: action node used for scene-light transitions. */
    s32 currentScene; /* 0x208 */
    s32 queuedScene; /* 0x20C */
    s32 scenePhaseFrame; /* 0x210: scene-phase counter, distinct from the +0x1F0 model-update clock. */
    s32 sceneEndFrame; /* 0x214: frame threshold scheduled by the battle end phase. */
    s16 endDelay; /* 0x218: signed battle-end countdown. */
    u16 endFlags; /* 0x21A: jingle, script-release and stream-reset latches. */
    s32 scriptState; /* 0x21C */
    s32 scriptArg; /* 0x220 */
    SBTL_TASK *tasks; /* 0x224 */
    SBTL_UNIT *units; /* 0x228 */
    u8 pad22C[0x10];
    struct SoundSlotOwner *soundSlotOwners; /* 0x23C: shared category/id motion-SE banks. */
    struct BattleModelEntry *modelEntries; /* 0x240 */
    /* Retail also reads both halves to recognize one actor in preset mode 3. */
    union {
        struct {
            u16 cameraPresetMode; /* 0x244 */
            u16 cameraActorHighWater; /* 0x246 */
        };
        u32 cameraActorConfiguration; /* 0x244 */
    };
    u16 preemptiveOutcome; /* 0x248: 0 awaits selection; 1 is preemptive; 2 is not preemptive. */
    u8 encounterKind; /* 0 normal, 2 surprise */
    u8 pad24B[1];
    u16 mode; /* 0x24C */
    u8 pad24E[2];
    s32 turnCount; /* 0x250 */
    s32 escapeAttemptCount; /* 0x254: incremented before each escape chance test. */
    u8 terminalOutcome; /* 0x258: selected result code; overrides may add values. */
    u8 pad259;
    u16 spawnCount; /* 0x25A: enemies spawned, compared with maxEnemySpawns. */
    u16 phase; /* 0x25C */
    u8 requestMode; /* 0x25E: script sets this to 4 with nextBattleMode */
    u8 pad25F[5];
    u32 motionSeLoadFrame; /* 0x264: all-ones disables signed frame polling. */
    u32 skillSeLoadFrame; /* 0x268: same value disables the unsigned throttle. */
    u16 defeatedLevelCount; /* Number of defeated enemy levels accumulated for the reward average. */
    u16 defeatedLevelTotal; /* Sum of partyRecord.level for defeated enemies. */
    s32 encounterPack; /* 0x270: ENC PACK test selection (btlUpdateDebugEncounterSelector) */
    s32 adjustmentGroupIndex; /* 0x274: encounter reward lookup in code_001A1960 */
    s32 adjustmentEntryIndex; /* 0x278: entry within that encounter group */
    s32 battleMode; /* 0x27C */
    s32 nextBattleMode; /* 0x280: selected by serial encounters or script */
    s32 nextAdjustmentEntryIndex; /* 0x284: entry retained for a follow-up encounter. */
    u16 encounterParamA; /* 0x288: scene record +0x1C, else the test-menu default */
    u16 encounterParamB; /* 0x28A: scene record +0x1E, else the test-menu default */
    u8 pad28C[4];
    struct sdfMemHandle_t *fieldF1ResourceHandle; /* 0x290: retained field archive allocation. */
    struct sdfMemHandle_t *fieldF2ResourceHandle; /* 0x294 */
    struct sdfMemHandle_t *fieldTBResourceHandle; /* 0x298 */
    struct dds3ProcessID_t *scriptOwner; /* 0x29C: parent task; script tasks use its priority minus one */
    struct dds3ProcessID_t *scriptTask; /* 0x2A0: active battle script task. */
    struct dds3ProcessID_t *boundTask; /* 0x2A4: AI actor-slot binding script. */
    struct dds3ProcessID_t *sceneObject; /* 0x2A8: command-panel task. */
    struct dds3ProcessID_t *spriteObject; /* 0x2AC: target-selection task. */
    struct dds3ProcessID_t *cleanupTask; /* 0x2B0: scene-panel cleanup task. */
    SBTLITEM_DROP itemDrops[3]; /* 0x2B4: battle defeat item aggregation in code_001A1960 */
    s32 moneyEarned;
    s32 moneyTotal; /* 0x2C4: accumulated battle money. */
    s32 experienceEarned; /* 0x2C8: defeat experience accumulator */
    s32 epEarned; /* 0x2CC: distinct defeat EP accumulator */
    s32 totalAp; /* 0x2D0: defeat EP accumulated for the result screen. */
    SBTLSCENE_SLOT slots[8]; /* 0x2D4: fldClearSceneSlotsAndGroups resets all eight. */
    SBTL_TASK *groupPrimary[20]; /* 0x2EC: scene-group lists in code_001C48A8 */
    SBTL_TASK *groupSecondary[45]; /* 0x33C */
    SBTL_TASK *groupTertiary[15]; /* 0x3F0 */
    u8 pad42C[0x20];
    SBTLSCENEFADING_RECORD fading[8]; /* 0x44C: fldInitSceneFadeRecords initializes these. */
    SBTL_TASK *currentTask; /* 0x48C: retained current command actor. */
    s8 timingRate; /* 0x490: initialized to 30; scales battle motion and countdowns. */
    u8 pad491[3];
    f32 modelFrameScale; /* 0x494 */
    u8 pad498[0xC];
    struct sprHandle_t *resA; /* 0x4A4: btlLoadResourceBlock stores resA at retail 0x001AC740. */
    struct sprHandle_t *numberGlyphSlots; /* 0x4A8: resource slots used for battle-number glyphs. */
    struct sprHandle_t *resC; /* 0x4AC: btlReleaseResourceBlock clears this at retail 0x001AC7B4. */
    u8 pad4B0[4];
    struct effRefObj_t *buttonTextureHandle; /* 0x4B4: retained button-icon texture reference. */
    struct SoundResourceNode *resources[0x31]; /* 0x4B8: SYSEFF resource slots */
    void *frontFloorResource; /* 0x57C: locked payload address; delayed free resolves its allocation. */
    void *sideFloorResource; /* 0x580: locked payload address; delayed free resolves its allocation. */
    u8 pad584[8];
    struct sdfFlagListWork_t *soundTransitionTask; /* 0x58C */
    u8 pad590[4];
    void (*bossCleanup)(void); /* 0x594 */
    u8 pad598[4];
    s32 (*hitResultOverride)(SBTL_UNIT *, SBTL_UNIT *, s32); /* 0x59C: source, target and command. */
    s32 (*chooseMotion)(SBTL_UNIT *, s32, s32); /* 0x5A0 */
    s32 (*statusMotionOverride)(SBTL_UNIT *); /* 0x5A4: negative keeps the current motion. */
    s32 (*preserveMotionRange)(SBTL_UNIT *, s32); /* 0x5A8: keep the stored motion start/end. */
    void (*actorParameterDeltaCallback)(SBTL_UNIT *, s32 *); /* 0x5AC */
    u8 pad5B0[4];
    s32 (*battleOutcomeOverride)(void); /* 0x5B4: -1 falls through to the unit scan; other results become terminalOutcome (func_001A8188). */
    void (*postPlacementCallback)(void); /* 0x5B8: runs after actor positions and rotations. */
    s32 (*effectParameterCallback)(SBTL_UNIT *, s32); /* 0x5BC: actor record-index override */
    u8 pad5C0[4];
    SBTL_UNIT *(*findReusableUnit)(u32, u32); /* 0x5C4 */
    u64 (*callback5C8)(u64); /* 0x5C8 */
    s32 (*callback5CC)(SBTL_UNIT *); /* 0x5CC */
    void (*cleanup)(void); /* 0x5D0 */
    u64 (*callback5D4)(u64); /* 0x5D4 */
    void (*prepareModelUnit)(SBTL_UNIT *); /* 0x5D8 */
    void (*beforeMotionUpdate)(void); /* 0x5DC */
    void (*finishModelUnit)(SBTL_UNIT *); /* 0x5E0 */
    u8 pad5E4[8];
    s32 (*serialOverride)(void); /* 0x5EC: -1 cancels a scripted follow-up encounter. */
    void (*updateCallback)(void); /* 0x5F0 */
    u8 pad5F4[0x14];
    u8 (*cameraArrangementHook)(SBTLLINKED_COMMAND *, SBTLCAM_STATE *, s32); /* 0x608: boolean mode-specific pose override. */
    s32 (*defeatCameraHook)(SBTLLINKED_COMMAND *, SBTLCAM_STATE *, s32); /* 0x60C: linked-defeat camera override. */
    s32 (*cameraStateChangePredicate)(SBTLLINKED_COMMAND *); /* 0x610 */
    u8 pad614[8];
    s32 (*actionCameraSetupHook)(SBTLLINKED_COMMAND *); /* 0x61C: nonzero handles action-camera setup. */
    u8 pad620[4];
    u32 (*commandSoundDelay)(s32); /* 0x624: action-specific sound delay. */
    u8 (*cameraPoseBlendHook)(SBTLLINKED_COMMAND *, s8, s8); /* 0x628: boolean override with side-presence flags. */
    s32 (*actionCameraStepHook)(SBTLLINKED_COMMAND *); /* 0x62C: nonzero handles the camera step. */
    s32 (*handleActorCategoryCamera)(struct SBTLLINKED_COMMAND *, s32, s32); /* 0x630: linked-list flags 0x200 / 0x400. */
    u8 pad634[4];
    u8 (*summonFadePredicate)(s32); /* 0x638: SUMMON fade option for the command. */
    s32 (*actionHitOverride)(SBTL_UNIT *, SBTL_UNIT *, s32, u32); /* 0x63C */
    s32 (*reflectedHitOverride)(SBTL_UNIT *, SBTL_UNIT *, s32); /* 0x640 */
    u8 pad644[4];
    void (*preActionHook)(SBTL_TASK *, s32, u64, u64, u64); /* 0x648 */
    void (*postTargetHook)(SBTL_TASK *, s32, SBTL_UNIT *, u64, u64, s32); /* 0x64C */
    u8 pad650[4];
    s32 (*allowDefeatCandidate)(SBTL_UNIT *); /* 0x654 */
    s32 (*allowDefeatCandidateClear)(SBTL_UNIT *); /* 0x658: zero vetoes clearing the ordinary defeat flags. */
    u8 pad65C[8];
    s32 (*cameraModeInitializePredicate)(SBTLLINKED_COMMAND *); /* 0x664: permits the mode initializer in 001DB048. */
    s32 (*cameraUpdatePredicate)(SBTLLINKED_COMMAND *); /* 0x668: gates the active camera handler. */
    s32 (*allowPositionEffect)(SBTL_UNIT *); /* 0x66C */
    s32 (*unitValueOverrideHook)(datActorWork_t *, s32); /* 0x670: custom affinity result, or -1 for the normal resolver. */
    s32 (*summonEffectId)(s32); /* 0x674: -1 retains the default summon effect. */
    s32 (*summonEffectDelay)(u32); /* 0x678: delay for the selected summon species. */
    s32 (*actionPointsOverride)(u32, u32, s32); /* 0x67C */
    /* 0x680: zero vetoes the record; nonzero continues ordinary checks. */
    BtlBattleRecordEligibilityOverrideCallback battleRecordEligibilityOverride;
    void (*actionResourceNameHook)(SBTL_TASK *, s32, char *); /* 0x684: formats the action event resource name. */
    u8 pad688[4];
    s32 (*commandAvailabilityOverride)(SBTL_UNIT *, s32); /* 0x68C: gates command availability */
    s32 (*commandRangeOverride)(SBTL_UNIT *, s32); /* 0x690: takes the actor and command */
    SBTLEFFECT_HEADER *effect; /* 0x694: mode-owned allocation begins with its generic header. */
    u8 pad698[0xC];
    SBTLDEBUGMENU_WORK debug; /* 0x6A4 */
    s8 debugModelListKind; /* 0: enemy models; 1: party models. */
    u8 unk_E0D;
    s16 debugMotionCountdown; /* Frames until preview motion; -1 when idle. */
} SBTL_STATE;

typedef char BtlSceneLightDds1Extent[(sizeof(SBTL_STATE) == 0xE10) ? 1 : -1];

/* SYSEFF metadata and runtime registrations share these indices. */
enum { BTL_SELECTED_UNIT_EFFECT_SOUND_SLOT = 0x26 };

extern void btlResetCameraMotion(SBTLLINKED_COMMAND *);

typedef struct SBTLROTATIONTASK_ARGS SBTLROTATIONTASK_ARGS;

struct evtUnitHandle_t;

typedef struct SBTLROTATIONTASK_ARGS {
    s128 from;
    s128 to;
    f32 rate;
    f32 t;
    s32 count;
    SBTL_UNIT *unit;
} SBTLROTATIONTASK_ARGS;

typedef char SBTLROTATIONTASK_ARGSSizeCheck[
    sizeof(SBTLROTATIONTASK_ARGS) == 0x30 ? 1 : -1];

/* Consume one item when its table flag 4 is set, then signal task completion. */
typedef struct {
    SBTL_UNIT *actor;
    s32 itemId;
} BtlCommandItemWork;

/* Kind-5 world-node payload attaches the event unit at +8. */
typedef struct evtUnitData_t {
    u8 pad00[8];
    evtUnitHandle_t *unit;
} evtUnitData_t;

/* Draw the unit's transparency model into its mirror's packet buffer, then draw the mirror from it. Either
 * model is created (and the update ends) on the first frame it is missing. */

/* Complete model loading, cross-fade the retained actor, and release it. */

typedef struct SBTLUNITLIGHTDIRECTIONTASK_ARGS {
    f32 direction[4];
    s32 directionMode;
    SBTL_UNIT *unit;
} SBTLUNITLIGHTDIRECTIONTASK_ARGS;

typedef char SBTLUNITLIGHTDIRECTIONTASK_ARGS_size_check[
    sizeof(SBTLUNITLIGHTDIRECTIONTASK_ARGS) == 0x18 ? 1 : -1];

typedef struct SBTLDAMAGESHAKE_ARGS {
    SBTL_UNIT *unit;
    f32 amplitude;
    s32 tick;
} SBTLDAMAGESHAKE_ARGS;

typedef struct SBTLGUNLOAD_ARGS {
    SBTL_UNIT *unit;
    struct fileRequest *request;
} SBTLGUNLOAD_ARGS;

typedef struct SBTLACTORTRANSPARENCY_ARGS {
    SBTL_UNIT *unit;
} SBTLACTORTRANSPARENCY_ARGS;

u32 btlRunCameraMotionResetTask(void);
extern f32 btlCameraLocalUpVector[4], btlCameraOutputRotation[4];
extern f32 btlWorldCameraStartOffset[4], btlWorldCameraStartRotation[4];
extern f32 btlCreatedCameraTarget[4], btlCreatedCameraRotation[4];
void btlResetCameraMotion(SBTLLINKED_COMMAND *action);

u32 btlRunCameraMotionResetTask(void) {
    SBTL_STATE *work = (SBTL_STATE *)btlGetRuntime();

    btlResetCameraMotion(&work->cameraCommand);
    return 1;
}
#endif
