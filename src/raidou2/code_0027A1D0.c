#include "common.h"

#include "newdata.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "kernel.h"
#include "draw3d.h"
#include "code_r1_001CC1D0.h"

#ifdef VERSION_RAIDOU2
/* Battle reward slot: item identity and owned drop count are observed by clients. */
typedef struct SBTLITEM_DROP {
    u16 id; /* 0x00 */
    u8 count; /* 0x02 */
    u8 unknown_03;
} SBTLITEM_DROP;
#endif

#ifdef VERSION_RAIDOU2
typedef char SBTLITEM_DROP_size_must_be_4[
    (sizeof(SBTLITEM_DROP) == 4) ? 1 : -1];
#endif

#ifdef VERSION_RAIDOU2
/* One 0x28-byte battle scene record; datBattleSceneRecords points at the 0x400-entry table. */
typedef struct datBattleSceneRecord_t {
    s8 voice;            /* 0x00: signed ENCOUNT voice selector */
    u8 itemDropId;       /* 0x01: item passed to btlAddItemDrop when nonzero */
    u8 itemDropCount;    /* 0x02: number of scene-entry item drops */
    u8 pad03;
    u16 serialScene;     /* 0x04: forced follow-up encounter, zero selects the weighted policy. */
    u16 unitModes[11];   /* 0x06: enemy unit modes; zero marks an empty slot */
    u16 backgroundMajorId; /* 0x1C: major scene-background ID, paired with the minor ID. */
    u16 backgroundMinorId; /* 0x1E: minor scene-background ID, paired with the major ID. */
    u16 flags;           /* 0x20: 0x8000, 0x800 and 0x400 are tested */
    u8 maxActiveEnemies; /* 0x22: zero defaults to five; six or more blocks spawning. */
    u8 maxEnemySpawns; /* 0x23: zero removes the cumulative spawn limit. */
    u16 unk24;           /* 0x24: overrides the scene's sound selection when nonzero */
    u16 eventId;         /* 0x26: signed event number; zero disables */
} datBattleSceneRecord_t;
#endif

#ifdef VERSION_RAIDOU2
typedef char datBattleSceneRecordSizeCheck[sizeof(datBattleSceneRecord_t) == 0x28 ? 1 : -1];
#endif

#ifdef VERSION_RAIDOU2
/* Selects actors by matching bits in their copied datActorWork_t flags word. */
typedef u32 BtlPartyRecordFlagsMask;
#endif

#ifdef VERSION_RAIDOU2
/* These actor aggregators index the five base-stat slots in party records. */
s32 btlSumOrAverageActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask,
    DatBaseStatIndex baseStatIndex, s8 actorFilterMode);
#endif

typedef struct SBTL_CONTROLLER {
    u8 pad_000[0x1F4];
    u32 flags;
    u8 pad_1F8[0x2C];
    SBTL_TASK *taskHead; /* 0x224 */
    SBTL_UNIT *actors;
    u8 pad_22C[0x20];
    u16 variant;
    u8 pad_24E[2];
    s32 step;
    u8 pad_254[6];
    u16 spawnCount; /* 0x25A: enemies spawned so far */
    u8 pad_25C[0x14];
    s32 adjustmentRecordIndex;
    s32 adjustmentGroupIndex;
    s32 adjustmentEntryIndex;
    s32 mode;
    u8 pad_280[0x1C];
    dds3ProcessID_t *taskParent;
    u8 pad_2A0[8];
    s32 sceneObjectTask;
    s32 spriteObject;
    s32 cleanupTask;
    SBTLITEM_DROP itemDrops[3];
    s32 moneyEarned; /* 0x2C0 */
    u8 pad2C4[4];
    s32 experienceEarned; /* 0x2C8 */
    s32 epEarned; /* 0x2CC */
    u8 pad2D0[4];
    SBTLSCENE_SLOT slots[8];
    SBTL_TASK *groupPrimary[20];
    SBTL_TASK *groupSecondary[45];
    SBTL_TASK *groupTertiary[15];
    u8 pad_42C[0x20];
    SBTLSCENEFADING_RECORD fadingRecords[8]; /* 0x44C */
    u8 pad_48C[0x18];
    sprHandle_t *resA; /* 0x4A4 */
    sprHandle_t *resB;
    sprHandle_t *resC;
    u8 pad_4B0[0x100];
    s32 (*sceneCallback)();
    u8 pad_5B4[0xD4];
    u32 (*enemyLimitOverride)(SBTL_UNIT *, datBattleSceneRecord_t *, u32); /* 0x688 */
} SBTL_CONTROLLER;

s32 btlAverageActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask, DatBaseStatIndex baseStatIndex) {
    return btlSumOrAverageActorAttribute(partyRecordFlagsMask, baseStatIndex, 0);
}
