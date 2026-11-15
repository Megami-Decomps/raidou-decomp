#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "newdata.h"
#include "sdk_libc.h"
#include "dds/btl_item_drop.h"
#include "dds/btl_state.h"
#include "dds/eff.h"
#include "dds/dds3_process.h"
#include "dds/btl_actor_attribute_api.h"

typedef struct BattleController {
    u8 pad_000[0x1F4];
    u32 flags;
    u8 pad_1F8[0x2C];
    BtlTask *taskHead; /* 0x224 */
    BtlUnit *actors;
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
    BattleItemDrop itemDrops[3];
    s32 moneyEarned; /* 0x2C0 */
    u8 pad2C4[4];
    s32 experienceEarned; /* 0x2C8 */
    s32 epEarned; /* 0x2CC */
    u8 pad2D0[4];
    BtlSceneSlot slots[8];
    BtlTask *groupPrimary[20];
    BtlTask *groupSecondary[45];
    BtlTask *groupTertiary[15];
    u8 pad_42C[0x20];
    BtlSceneFadingRecord fadingRecords[8]; /* 0x44C */
    u8 pad_48C[0x18];
    EffectSlotSet *resA; /* 0x4A4 */
    EffectSlotSet *resB;
    EffectSlotSet *resC;
    u8 pad_4B0[0x100];
    s32 (*sceneCallback)();
    u8 pad_5B4[0xD4];
    u32 (*enemyLimitOverride)(BtlUnit *, DatBattleSceneRecord *, u32); /* 0x688 */
} BattleController;



s32 btlAverageActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask, DatBaseStatIndex baseStatIndex) {
    return btlSumOrAverageActorAttribute(partyRecordFlagsMask, baseStatIndex, 0);
}
