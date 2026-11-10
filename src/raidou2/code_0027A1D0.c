#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_001A7180.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/common.h"

#include "dds/btl_unit_defeat_by_delta_result.h"

#include "dds/btl_selected_entry_api.h"

#include "dds/btl_quarter_health_result.h"

#include "dds/btl_actor_command_selector_value_api.h"

#include "dds/evt_run_context_api.h"

#include "dds/btl_scene_counter_mode.h"

#include "dds/btl_action_press_result.h"

#include "dds/btl_flag_to_element_index_api.h"

#include "dds/btl_command_result_kind_api.h"

#include "dds/btl_command_status_api.h"

#include "dds/btl_command_use_error.h"

#include "dds/btl_command_cost_api.h"

#include "dds/btl_option_skill_api.h"

#include "dds/btl_special_ability.h"

#include "dds/eff_random_mod.h"

#include "dds/btl_item_drop.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/fr_font_measure.h"

#include "dds/fr_font.h"

#include "dds/eff_resource_slots.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_heap_api.h"

#include "dds/btl_scene_fade.h"

#include "dds/btl_resource.h"

#include "dds/pcp_vu0.h"

#include "dds/btl_ui.h"

#include "dds/sdf.h"

#include "dds/btl.h"

#include "dds/btl_resolve_unit_value_api.h"

#include "dds/btl_actor_selection_clear_api.h"

#include "dds/btl_state.h"

#include "dds/btl_terminal_outcome.h"

#include "dds/btl_enemy_spawn_capacity_api.h"

#include "dds/btl_hunt_mp_recovery_api.h"

#include "dds/btl_reward_level_scale_kind.h"

#include "dds/btl_boss_debug_api.h"

#include "dds/eff.h"

#include "dds/btl_action.h"

#include "dds/btl_ai_table.h"

#include "dds/kwln.h"

#include "dds/dat_state.h"

#include "dds/dat_calculated_stat_value_api.h"

#include "dds/btl_actor_attribute_api.h"

#include "dds/pty_roster_api.h"

#include "dds/dat_command.h"

#include "dds/dds3_task_lifecycle.h"

/* Declared before first use (were implicit int). */
extern u8 btlTestSelectedItemCategoryMask(BtlUnit *unit, s32 actorIndex);

extern s32 fldCountSceneSlots(void);

extern u32 sdfQueryChannelBits(s32 channel, datUnitWork_t *sourceRecord, datUnitWork_t *item);

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

extern s32 D_00358510[];

extern DatEnemyRecord *datEnemyRecords;

extern s32 btlHasEnemyRecordDefeatExemptionFlag(BtlUnit *);

extern s32 btlAverageLevelsForMaskExcludingStatus20(u32);

extern s32 btlCalculateEnemyExperienceReward(BtlUnit *, BtlUnit *);

extern s32 btlGetEnemyMoney(BtlUnit *, BtlUnit *);

u32 btlEncodeActorIndexAsSelectionMask(u32 id);

s32 btlGetAbilityAttributeMultiplierPercent(BtlUnit *actor, s32 attr);

f32 btlGetActorEntryMultiplier(BtlUnit *unit, u32 index, s8 includeCharge);

s32 btlGetEntryFlagsUnlessDisabled(datUnitWork_t *record);

s32 btlHasEnabledSpecialAbilityForSlot(BtlUnit *unit, u32 slot);

s32 btlHasMappedSpecialAbilityForSlot(BtlUnit *unit, u32 slot);

u32 btlHasSpecialAbilityWhenArgumentUnset(BtlUnit *unit, s64 argumentValue);

extern u32 sdfQueryChannelValue(s32, datUnitWork_t *, datUnitWork_t *);

#include "dds/sdf_debug_api.h"

BtlPartyStatusMask btlGetCommandAddedStatus(
    BtlUnit *attacker, BtlUnit *defender, BtlCommandId commandId,
    BtlActionPressResult pressResult, s32 mode);

BtlPartyStatusMask btlQueryUnitChannelFlags(
    BtlUnit *source, BtlUnit *recipient, BtlCommandId commandId,
    BtlActionPressResult pressResult, s32 kind);

u32 btlGetAdjustedSlotAffinity(BtlUnit *unit, s32 unused, DatAffinitySelector selector);

void func_001A7AD0(void);

extern f32 btlGetRewardLevelMultiplier(BtlUnit *, BtlUnit *, BtlRewardLevelScaleKind);

/* Scale the enemy's normal EP reward; flag 0x2000 multiplies it by 100. */
s32 btlCalculateEnemyExperienceReward(BtlUnit *acquirer, BtlUnit *enemy);

extern char D_003A1B90[];

extern char D_003A1BA8[];

/* Return the normal/hunt EP level multiplier with the native level-difference caps. */
f32 btlGetRewardLevelMultiplier(BtlUnit *acquirer, BtlUnit *enemy, BtlRewardLevelScaleKind rewardKind);

s32 btlGetEnemyMoney(BtlUnit *acquirer, BtlUnit *enemy);

extern f32 btlGetRewardLevelMultiplier(BtlUnit *, BtlUnit *, BtlRewardLevelScaleKind);

/* Hunt EP uses its own table quantity and the ratio calculator's mode 0. */
s32 btlCalculateHuntEpReward(BtlUnit *acquirer, BtlUnit *enemy);

/* Hunt-recovery interface: actor and target are passed; this title returns no HP recovery. */
u32 btlGetHuntHpRecoveryAmount(BtlUnit *actor, BtlUnit *target);

/* Target is part of the hunt-recovery interface; this implementation uses only actor. */
BtlHuntMpRecoveryAmount btlGetHuntMpRecoveryAmount(BtlUnit *actor, BtlUnit *target);

BtlUnitDefeatByDeltaResult btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *actor, s32 delta);

BtlQuarterHealthThresholdResult btlIsCurrentValueBelowQuarterThreshold(BtlUnit *object);

s32 btlWouldUiValueFallBelowQuarter(BtlUnit *object, s32 delta);

s32 btlBothSidesActive(BtlUnit *unit);

s32 btlIsUiObjectIndexAllowed(BtlUnit *object);

s32 btlIsUnitStatusFlagClear(BtlUnit *object);

/* Terminal policies publish and return; see docs/idioms.md,
 * "Outcome classifiers: late publication sharing". */
BtlTerminalOutcomeReadiness btlResolveBattleTerminalOutcome(void);

extern s32 D_00358514[];

extern s32 D_00358518[];

s32 btlSelectedEntryHitsElement(BtlUnit *source, BtlUnit *unit, s32 selectorIndex);

BtlCommandHitResult btlGetActionRecordLookupValue(BtlCommandId commandId);

u8 btlTestSelectedItemCategoryMask(BtlUnit *unit, s32 actorIndex);

BtlCommandHitResult btlGetSelectedActionHitResult(BtlUnit *unit);

s32 btlGetSelectedCommandEffectTint(BtlUnit *unit);

s32 btlCompareSkippedAndActiveTargetCounts(BtlIndexList *targets, BtlOperandGroup *results);

extern const char D_003A1C88[29];

extern s32 btlRollAiBucket(void);

s32 btlRollEscapeChance(BtlUnit *actor);

extern AiSpecies *datEnemyAiRecords;

u8 btlIsSelectedActorStatusAndRecordClear(BtlUnit *unit);

/* Return true when neither actor status nor its entry flags contain bit 0x40. */
s32 btlAreUnitStatusAndEntryFlagsClear(BtlUnit *actor);

s32 btlFindCommandPartnersByAffinity(BtlUnit *unit, BtlCommandId commandId, BtlUnit **first, BtlUnit **second);

extern BtlTask *fldGetSceneGroupEntry(s32 entryIndex);

#include "dds/btl_command_affinity_api.h"

BtlCommandAffinityPartnerAvailability btlHasRequiredCommandAffinityPartners(
    BtlUnit *unit, BtlCommandId commandId);

s32 btlHasAdjacentActorRecordStatus(BtlUnit *object);

/* Return 1 when bit 26 is clear, preserving the original signed word shift. */
u32 btlIsActorHighStateFlagClear(BtlUnit *actor);

/* Return AVAILABLE when scene and cumulative spawn limits permit another enemy. */
BtlEnemySpawnCapacityResult btlCanSpawnAnotherEnemy(BtlUnit *requester);

/* Count eligible option-skill IDs; a non-NULL output receives those IDs. */
s32 btlCollectAvailableOptionSkills(BtlUnit *actor, BtlCommandId *choices);

u8 btlHasAvailableOption(BtlUnit *actor);

BtlCommandId btlChooseRandomAvailableOption(BtlUnit *actor);

s32 btlChooseEligibleSkill(BtlUnit *actor);

f32 btlGetActionCategoryMultiplier(BtlUnit *object, s32 unused, s32 command);

f32 btlGetActionCategoryGateAsFloat(s32 unused0, s32 unused1, s32 commandIndex);

f32 btlGetActionRecordPercentAsFraction(s32 unused0, s32 unused1, s32 commandIndex);

u32 btlAdjustPointsForCombatFlags(u32 flags, u32 secondary, u32 points, s32 index);

BtlSceneCounterMode btlGetCommandResultKindFromFlags(
    u32 flags, BtlActionPressResult secondary);

s32 btlAverageMaximumValueForMask(
    BtlPartyRecordFlagsMask partyRecordFlagsMask, s8 allowDisabled);

void btlAverageFilteredMaximumForMask(BtlPartyRecordFlagsMask partyRecordFlagsMask);

s32 btlAverageAllMaximumForMask(BtlPartyRecordFlagsMask partyRecordFlagsMask);

s32 btlAverageCurrentValueForMask(
    BtlPartyRecordFlagsMask partyRecordFlagsMask, s8 allowDisabled);

void btlAverageFilteredCurrentForMask(BtlPartyRecordFlagsMask partyRecordFlagsMask);

/* Return mean current HP for the mask, including bit-0x20 actors. */
s32 btlAverageAllCurrentForMask(BtlPartyRecordFlagsMask partyRecordFlagsMask);

s32 btlAverageMaskedActorStat(
    BtlPartyRecordFlagsMask partyRecordFlagsMask, s8 allowDisabled);

s32 btlAverageLevelsForMaskExcludingStatus20(BtlPartyRecordFlagsMask partyRecordFlagsMask);

/* Return mean level for the mask, including bit-0x20 actors. */
s32 btlAverageAllLevelsForMask(BtlPartyRecordFlagsMask partyRecordFlagsMask);

s32 btlSumOrAverageActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask,
    DatBaseStatIndex baseStatIndex, s8 allowDisabled);

void btlAverageFilteredActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask, DatBaseStatIndex baseStatIndex);

s32 btlAverageActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask, DatBaseStatIndex baseStatIndex);

#include "dds/btl_preemptive_outcome_result.h"

extern void btlPreemptiveChanceHook();

extern const char D_003A1CA8[33];

/* Select the preemptive outcome using scene overrides and the native chance roll. */
BtlPreemptiveOutcomeResult btlSelectPreemptiveOutcome(void);
/* End of declarations from src/dds1/game/code_001A7180.c */

s32 btlAverageActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask, DatBaseStatIndex baseStatIndex) {
    return btlSumOrAverageActorAttribute(partyRecordFlagsMask, baseStatIndex, 0);
}
