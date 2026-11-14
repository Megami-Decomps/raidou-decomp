#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_001FF0C8.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/btl.h"

#include "dds/sdf_chip.h"

extern void *memcpy(void *, const void *, unsigned int);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

#include "dds/sdk_ps2.h"

#include "dds/common.h"

#include "dds/btl_target_list_builder.h"

#include "dds/btl_element_block_query_result.h"

#include "dds/btl_command_use_error.h"

#include "dds/btl_command_hit_result.h"

#include "dds/btl_resolve_unit_value_api.h"

#include "dds/btl_actor_entry_code_api.h"

#include "dds/btl_actor_selection_clear_api.h"

#include "dds/btl_enemy_spawn_capacity_api.h"

#include "dds/btl_skill_max_api.h"

#include "dds/btl_ai_slot.h"

#include "dds/sdf_chip.h"

#include "dds/btl_task_condition.h"

#include "dds/eff_transform.h"

#include "dds/btl_state.h"

#include "dds/btl_command.h"

#include "dds/btl_model_record.h"

#include "dds/sdf_draw.h"

#include "dds/evt_unit.h"

#include "dds/mdl.h"

#include "dds/ee_mmi.h"

#include "dds/pcp_vu0.h"

#include "dds/btl_action.h"

#include "dds/btl_ai_table.h"

#include "dds/btl_ai_scratch.h"

#include "dds/btl_ai_route_predicate_selection.h"

#include "dds/btl_unit_tasks.h"

#include "dds/dat_state.h"

#include "dds/dat_command.h"

#define BTL_AI_WEIGHT_MASK 0xFFFF

#define BTL_HEALTH_RATE_SCALE 100

#define BTL_PARTY_QUERY_MASK 0x221

#define BTL_PARTY_ACTIVE_FLAGS 0x201

#define BTL_ENEMY_QUERY_MASK 0x421

#define BTL_ENEMY_ACTIVE_MASK 0x401

#define BTL_ENEMY_ACTIVE_FLAGS 0x401

#define BTL_REQUIRED_ACTION_CATEGORY_COUNT 10

#define BTL_ENTRY_CODE_SCAN_COUNT 10

#define BTL_AI_COUNTER_SATURATION 0x100

#define BTL_AI_COUNTER_MAX 0xFF

#define BTL_LOW_HP_ACTION_DELAY 0xF

#define BTL_LOW_HP_PERCENT_LIMIT 0x1E

#define BTL_LOW_HP_BUCKET_LIMIT 0x1E

#define BTL_LOW_HP_ENEMY_COUNT_LIMIT 2

extern s32 btlAreUnitStatusAndEntryFlagsClear(BtlUnit *actor);

extern u8 btlHasAvailableOption(BtlUnit *actor);

extern s32 btlMatchesActorEntryCodeCondition(BtlUnit *actor, s32 conditionIndex);

extern s32 btlUnitMatchesEntryCodeCondition(BtlUnit *, s32, s8);

extern s8 btlHistoryCounter;

extern BtlAiScratchWork *btlActionScratchWork;

extern s8 D_003A5A80[];

extern s8 D_003A5AA8[];

extern void btlBossDebugPrintf(const char *, ...);

extern AiSpecies *datEnemyAiRecords;

extern u8 sdfHddValid;

extern char D_003A5988[];

extern void btlDebugPrintf(const char *, ...);

extern s32 btlCounterReachedLimit(s32, u32);

extern s32 btlAnyUnitHasQueuedQuery(s32, s32, u32);

extern u32 btlAverageLevelsForMaskExcludingStatus20(s32);

extern s32 btlRunAiAction();

extern u32 GetRand(u32);

extern u32 btlPreviousAiCandidateBucket;

extern s32 btlResolveCommandSlotAndKind(BtlTask *, s32, u16 *, s32 *);

extern void (*btlAiActionHandlers[])(BtlTask *, u32, s32);

extern AiSpecies D_00360640[];

extern s32 btlSelectConditionalAiRoute(BtlUnit *, s32, s32 *, s32);

extern s32 btlRollAiBucket(void);

extern s32 btlHasAdjacentActorRecordStatus(BtlUnit *);

extern u32 btlIsActorHighStateFlagClear(BtlUnit *);

extern s32 btlSyncEligibleActorPanelHpMp(BtlTask *);

void btlSelectAndRunModeAiAction(BtlTask *task, u32 mode);

u32 func_001FF558(void);

extern s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction);

s32 btlSelectTierMatrixAiRoute(BtlUnit *context, s32 species, s32 *selected, s32 requestedRow);

s32 btlSelectConditionalAiRoute(BtlUnit *context, s32 species, s32 *selected, s32 requestedRow);

/* Pick the first nonzero-weight slot whose 16-bit cumulative weight reaches the roll.
 * The unit argument is unused; preserve the runtime lookup, debug failure path and zero fallback. */
u32 btlPickWeightedAiSlot(BtlUnit *unusedUnit, s32 speciesId, s32 rowIndex);

/* Sum of two 0..0xFFF rolls, folded into 0..0xFFF; /41 turns it into one of 100 buckets. */
static inline u32 btlRollTwice(void) {
    u32 value = GetRand(0x1000);
    value += GetRand(0x1000);
    return value & 0xFFF;
}

/* Rolls a bucket; if it lands within 3 of the previous one, rolls again. */
s32 btlRollAiBucket(void);

/* Out parameters of the action-id lookup. */
typedef struct BtlActionLookup {
    u16 slot;
    s32 result;
} BtlActionLookup;

extern s32 btlReadCurrentUnitHp(dds_datUnitWork_t *);

extern u16 btlReadUnitStatusMask(dds_datUnitWork_t *);

/* Compare HP against a signed percentage using the native products and unsigned comparison. */
s32 btlIsUnitAtOrBelowHealthRate(BtlUnit *unit, s32 healthPercent);

/* Return whether an active enemy-side unit passes the HP percentage check; bit 0x20 is not filtered here. */
s32 btlHasBossAtOrBelowHealthRate(s32 unused, s32 healthPercent);

extern s32 btlAiCounterReachedLimit(s32 unused, u32 minimumCount);

/* Reset the scratch-context counter only when its query succeeds. */
u32 btlResetAiCounterAtLimit(s32 unused, u32 minimumCount);

/* Check an inclusive upper bound on active enemy-side units with bit 0x20 clear. */
s32 btlIsGroup400CountAtMost(s32 unused, u32 maximumCount);

/* Check an inclusive upper bound on eligible party units, additionally excluding condition bit 0x800. */
s32 btlIsGroup200EligibleCountAtMost(s32 unused, u32 maximumCount);

/* Normalize the scene/status/AI-record gate. */
u8 btlPassesSceneAndActorRecordGate(BtlUnit *unit);

/* Check an inclusive upper bound on active party-side units with bit 0x20 clear. */
s32 btlIsGroup200CountAtMost(s32 unused, u32 maximumCount);

/* Return whether the native unit-status query intersects any requested action-mask bit. */
s32 btlUnitHasAnyStatusInMask(BtlUnit *unit, s32 actionMask);

/* Test active enemy-side units for any requested action bit; do not filter bit 0x20. */
s32 btlAnyGroup400HasActionMask(s32 unused, s32 actionMask);

/* Test party-side units for any requested action bit; DDS1 also requires bit 0x20 clear. */
s32 btlAnyGroup200HasActionMask(s32 unused, s32 actionMask);

/* Require every eligible party unit to intersect the requested mask; an empty selection succeeds. */
s32 btlAllGroup200HaveActionMask(s32 unused, s32 actionMask);

/* Find an eligible party-side unit with the requested mode. */
s32 btlHasGroup200UnitMode(s32 unused, s32 unitMode);

/* Find an eligible enemy-side unit of the requested mode with a different native identity. */
s32 btlHasOtherGroup400UnitMode(BtlUnit *excludedUnit, s32 unitMode);

extern BtlActorEntrySlotIndex btlEntryCodeConditionIndices[];

/* Kinds 0xB/0xC scan the even/odd entry codes; other kinds test their own entry. */
s32 btlMatchesActorEntryCodeCondition(BtlUnit *actor, s32 conditionIndex);

s32 btlUnitMatchesEntryCodeCondition(BtlUnit *unit, s32 conditionIndex, s8 inverse);

extern u8 btlTestSelectedItemCategoryMask(BtlUnit *, s32);

extern const s32 btlRequiredActionCategories[10];

/* Require all ten category queries to succeed; retain the complete local copy of the native category table. */
s32 btlUnitHasAllTenActions(BtlUnit *actor);

/* Return whether any eligible party unit meets the requested entry-code condition. */
s32 btlAnyPartyMeetsEntryCodeCondition(s32 unused, s32 conditionIndex);

/* Return whether any eligible enemy unit meets the requested entry-code condition. */
s32 btlAnyEnemyMeetsEntryCodeCondition(s32 unused, s32 conditionIndex);

/* Normalize the mode-zero action query to a byte boolean; preserve its native result width. */
u8 btlCheckUnitActionModeZero(u32 unitAddress, u32 actionQuery);

/* Normalize the mode-one action query to a byte boolean; preserve its native result width. */
u8 btlCheckUnitActionModeOne(u32 unitAddress, u32 actionQuery);

/* Return whether an eligible party unit has a nonzero mode-zero query result. */
s32 btlAnyPartyPassesEntryCheck(s32 unused, u32 actionQuery);

/* Return whether an eligible party unit has a nonzero mode-one query result. */
s32 btlAnyPartyPassesInverseEntryCheck(s32 unused, u32 actionQuery);

/* Return whether an eligible enemy unit has a nonzero mode-zero query result. */
s32 btlAnyEnemyPassesEntryCheck(s32 unused, u32 actionQuery);

/* Return whether an eligible enemy unit has a nonzero mode-one query result. */
s32 btlAnyEnemyPassesInverseEntryCheck(s32 unused, u32 actionQuery);

/* Return whether an eligible party unit has a zero mode-zero query result. */
s32 btlAnyPartyFailsEntryCheck(s32 unused, u32 actionQuery);

/* Return whether an eligible enemy unit has a zero mode-zero query result. */
s32 btlAnyEnemyFailsEntryCheck(s32 unused, u32 actionQuery);

/* Look for an eligible party unit with native flag 0x1000 clear; its meaning is not established here. */
s32 btlAnyGroup200LacksFlag1000(void);

/* Normalize entry-code condition 10 to a byte boolean without changing the native short-arity calls. */
u8 btlUnitPassesActionTenCheck(u32 unitAddress);

/* Normalize the entry-code query. */
s32 btlQueryActorEntryCondition(s32 actorAddress, s32 conditionIndex);

/* Test record bit 0x2000 only outside actor group 0x200; its meaning is unknown. */
s32 btlTestRecordFlagOutsidePartyGroup(BtlUnit *unit);

/* Test scratch-context bit 1; no gameplay meaning for this flag is established here. */
s32 btlHasContextFlagTwo(void);

s32 btlCheckCounterLimit(s32 unused, u32 minimumCount);

/* Return whether counter query 4 has reached the requested inclusive lower bound. */
s32 btlCounterReachedLimit(s32 unused, u32 minimumCount);

/* Increment the stored counter, retain native wrap/clamp behavior, then reread it for the limit test. */
s32 btlAiCounterReachedLimit(s32 unused, u32 minimumCount);

/* Compare the battle turn count with an inclusive lower bound. */
s32 btlTurnReachedLimit(s32 unused, u32 minimumTurns);

#include "dds/btl_preemptive_outcome_result.h"

/* Return true only after a non-preemptive outcome with no counted turns. */
s32 btlIsReadyWithoutTurns(void);

extern s32 btlReadCurrentUnitMp(dds_datUnitWork_t *);

/* Compare a unit stat with a percentage of its maximum.
 * The stat's identity is not established by these two accessors. */
s32 btlIsUnitStatAtOrBelowRate(BtlUnit *unit, s32 percentage);

/* Normalize option-skill availability. */
u8 btlQueryActorOptionAvailability(BtlUnit *actor);

/* Query the context's index list, freeing it on both the first success and exhausted-list paths. */
s32 btlAnyIndexedUnitPassesQuery(BtlUnit *unit);

/* Gate the low-HP action on hold, delay, HP ratio, enemy count, bucket roll, and selected-actor status.
 * Preserve native unsigned timing and division behavior, including the absence of a max-HP guard. */
s32 btlIsLowHpActionReady(BtlUnit *unit, s32 unused);

/* Test native unit flag 0x1000 without assigning it an unverified gameplay meaning. */
s32 btlUnitHasFlag1000(const BtlUnit *unit);

u8 btlIsCommandAvailable(BtlUnit *unit, s32 command);

/* Normalize the gate requiring status and entry flag 0x40 to be clear. */
u8 btlPassesStatusEntryGate(BtlUnit *actor);

extern const s32 btlElementMasks[20];

s32 btlElementToBitIndex(s32 mask, s32 elementIndex);

s32 btlUnitHasNegativeActionQueryResult(BtlUnit *unit, s32 mask);

extern s32 btlUnitHasNegativeActionQueryResult(BtlUnit *unit, s32 mask);

s32 btlAnyEnemyHasNegativeActionResult(s32 unused, s32 action);

s32 btlAnyPartyUnitHasNegativeActionResult(s32 unused, s32 action);

s32 btlHasGroup200DifferentUnitMode(s32 unused, s32 kind);

s32 btlHasOtherGroup400DifferentUnitMode(BtlUnit *actor, s32 kind);

s32 btlAnyUnitPassesCheck200(s32 unused, s32 action);

s32 btlCanQueryElementAgainstParty(s32 unused, s32 action);

s32 btlAnyUnitPassesCheck400(s32 unused, s32 action);

s32 btlCanQueryElementAgainstEnemies(s32 unused, s32 action);

extern s32 btlWouldUiValueFallBelowQuarter(BtlUnit *, s32);

extern u8 sdfHddValid;

s32 btlCheckActorEligibilityWithDebug(BtlUnit *actor);

extern s32 btlActionMatchesStatMask(s32, s16, s8);

/* Match a selector stat or encoded mask; force bypasses only the options 1..2 gate. */
s32 btlActionMatchesStatMask(s32 mask, s16 actionId, s8 force);

/* Return whether an eligible unit has a queued action in the requested category. */
s32 btlAnyUnitHasActionInSlots(s32 mask, s32 action);

s32 btlAnyGroup200HasAction(s32 unused, s32 action);

s32 btlAnyGroup400HasAction(s32 unused, s32 action);

/* Search eligible group-0x400 actors' stored action IDs with the options gate enabled. */
s32 btlAnyEnemyActionMatchesSelector(s32 unused, s32 action);

/* DDS1 searches eligible party actor records, unlike DDS2's linked-action chain. */
s32 btlAnyPartyActionMatchesSelector(s32 unused, s32 action);

s32 btlAnyPartyUnitHasFullActionSet(void);

s32 btlAnyEnemyHasFullActionSet(void);

s32 btlAreUnitsMissingStatusFlag(void);

s32 btlAreUnitsHoldingStatusFlag(void);

s32 btlIsHistoryCounterEmpty(void);

s32 btlAnyUnitBlocksGroup200Element(s32 unused, s32 action);

s32 btlAnyGroupUnitHasZeroStat(void);

s32 btlAnyGroup400HasQuery(s32 unused, s32 query);

s32 btlAnyGroup200HasQuery(s32 unused, s32 query);

s32 btlAnyUnitHasQueuedQuery(s32 unused, s32 query, u32 mask);

extern s32 btlHasEnabledSpecialAbilityForSlot(BtlUnit *, u32);

extern BtlCommandHitResult btlGetSelectedActionHitResult(BtlUnit *);

BtlUnitElementBlockQueryResult btlUnitBlocksElementQuery(BtlUnit *unit, s32 action, s32 mask);

/* Query active/cumulative enemy capacity with a null limit-override requester. */
BtlEnemySpawnCapacityResult btlCheckEnemySpawnCapacity(void);
/* End of declarations from src/dds1/game/code_001FF0C8.c */

/* Rolls a bucket; if it lands within 3 of the previous one, rolls again. */
s32 btlRollAiBucket(void) {
    u32 slot = btlRollTwice() / 0x29;
    if (btlPreviousAiCandidateBucket >= slot - 3 && btlPreviousAiCandidateBucket <= slot + 3) {
        u32 reroll = btlRollTwice();
        btlPreviousAiCandidateBucket = slot;
        return reroll / 0x29;
    }
    btlPreviousAiCandidateBucket = slot;
    return slot;
}
INCLUDE_SDATA(const s32, "code_0027CC80", D_r1_0046DB00);

INCLUDE_SDATA(const s32, "code_0027CC80", D_r1_0046DB04);

INCLUDE_SDATA(const s32, "code_0027CC80", D_r1_0046DB08);

INCLUDE_SDATA(const s32, "code_0027CC80", D_r1_0046DB0C);

INCLUDE_SDATA(const s32, "code_0027CC80", D_r1_0046DB10);

