#include "common.h"
#include "sdk_libgcc.h"
#include "datCalc.h"
#include "ps2asm.h"

extern s32 evtBitCheck(s32 no);

typedef struct datTakaraSagasiItem_t {
    u16 item;
    u16 par;
} datTakaraSagasiItem_t;

typedef struct datTakaraSagasiTable_t {
    u32 flag;
    datTakaraSagasiItem_t itemtbl[8];
} datTakaraSagasiTable_t;

/* FLD_SAGASI_AKUMA reads four inline 0x24-byte groups and their weights. */
typedef struct datTakaraSagasi_t {
    u32 flag;
    datTakaraSagasiTable_t tbl[4];
} datTakaraSagasi_t;

extern u32 datGetAisyo(datUnitWork_t *work, s32 attr);

typedef struct
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 skill;
    /* 0x04 */ u8 price[4];
} datItem_t;

#include "sdk_libc.h"

extern datSkill_t attr[];
extern u32 dds3GetRandIntA(void *state, u32 limit);

struct sdfRandCB_t;

u32 datGetAisyo(datUnitWork_t *work, s32 attr);
u32 datGetAisyoValue(datUnitWork_t *work, s32 attr);
u32 datGetAisyoFlag(datUnitWork_t *work, s32 attr);

/* Partial view of the persistent party and stock records. */
typedef struct {
    u8 unknown00[0xA50];
    /* 0xA50 */ s32 maxstock;
    /* 0xA54 */ datUnitWork_t unitwork[16];
    /* 0x1914 */ s8 stocklist[16];
    /* 0x1924 */ s32 stockcnt;
    /* 0x1928 */ u8 unknown1928[0xA0];
} datPartyWork_t;

/* The same persistent-work pointer, through its recovered party/stock view. */

/* NON_MATCHING: 7 of 75 words differ (first at +0xE4). */

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_datItem_t 1

#define SMT3_DEFINES_datPartyWork_t 1

#define SMT3_DEFINES_datTakaraSagasiItem_t 1

#define SMT3_DEFINES_datTakaraSagasiTable_t 1

#define SMT3_DEFINES_datTakaraSagasi_t 1

#include "basic.h"
#include "dds/btl.h"
#include "dds/dds3_path.h"
#include "dds/dds3_task_create.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/dds3obj.h"
#include "dds/eff_transform.h"
#include "dds/sdf_dev_state.h"
#include "dds/sdf_heap_api.h"

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

extern void *memcpy(void *, const void *, unsigned int);

#include "dds/sdk_ps2.h"
#include "dds/evt_run_context_api.h"
#include "dds/common.h"
#include "dds/scr.h"
#include "dds/sdf_vu0.h"
#include "dds/sdf_channel.h"
#include "dds/scr_work_task_api.h"
#include "dds/dat_save_words.h"
#include "dds/sdf_float_counter.h"
#include "dds/eff_random_mod.h"
#include "dds/mnu_flag_snapshot.h"
#include "dds/evt_world_source_transform.h"
#include "dds/pcp_vu0.h"
#include "dds/btl_action.h"
#include "dds/dat_state.h"
#include "dds/dat_affinity_selector.h"
#include "dds/dat_effective_affinity_api.h"
#include "dds/dat_world_field_flags.h"
#include "dds/dat_unit_has_skill_api.h"
#include "dds/evt_randomize_entry_api.h"
#include "dds/dat_command.h"
#include "dds/sdf.h"
#include "dds/fld_runtime_reset_api.h"
#include "dds/mdl_flag_api.h"

#define SDF_PACKED_STATUS_BIT 0x8000

#define SDF_PACKED_CHANNEL_MASK 0x7fff

#define SDF_UNIT_ENEMY 0x20

#define SDF_FLOAT_COUNTER_FROZEN_FLAG 0x00000008

#define SDF_FLOAT_COUNTER_WRAP_FLAG 0x00000020

#include "dds/sdf_debug_api.h"

/* These unused-result legacy mutator calls had no prototype in scope. */
void sdfRaisePackedChannelValue(dds_datUnitWork_t *item, u32 minimumValue);

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_EFF_WORLD_KIND_ACTION_10 1

#define SMT3_DEFINES_EFF_WORLD_KIND_ACTION_B 1

#define SMT3_DEFINES_EFF_WORLD_KIND_ACTION_D 1

#define SMT3_DEFINES_EFF_WORLD_KIND_CAMERA 1

#define SMT3_DEFINES_EFF_WORLD_KIND_COUNT 1

#define SMT3_DEFINES_EFF_WORLD_KIND_EFFECT_OBJECT 1

#define SMT3_DEFINES_EFF_WORLD_KIND_EFFECT_TRANSFORM 1

#define SMT3_DEFINES_EFF_WORLD_KIND_EVENT_TABLE 1

#define SMT3_DEFINES_EFF_WORLD_KIND_FOLLOW_MODEL 1

#define SMT3_DEFINES_EFF_WORLD_KIND_LIGHT 1

#define SMT3_DEFINES_EFF_WORLD_KIND_RESOURCE_OWNER 1

#define SMT3_DEFINES_EFF_WORLD_KIND_SCRIPT_OBJECT 1

#define SMT3_DEFINES_EFF_WORLD_KIND_SLOT_RING 1

#define SMT3_DEFINES_EFF_WORLD_KIND_TRANSFORM_SOURCE 1

#define SMT3_DEFINES_EFF_WORLD_KIND_WORLD_INDEX_OWNER 1

#define SMT3_DEFINES_EFF_WORLD_KIND_WORLD_INFO 1

#include "dds/scr_integer_parameter_api.h"
#include "dds/scr_procedure_start_api.h"
#include "dds/pty_experience_threshold_type.h"
#include "dds/evt_script_context.h"
#include "dds/evt_solar.h"
#include "dds/evt_item_value_threshold_api.h"
#include "dds/evt_random_status_clear_api.h"
#include "dds/pty_roster_value_record.h"
#include "dds/pty_adjust_item_quantity_api.h"
#include "dds/dat_battle_adjustment_condition.h"
#include "dds/dat_currency_requirement_api.h"
#include "dds/dat_calculated_stat_value_api.h"
#include "dds/dat_calculate_command_base_value_api.h"
#include "dds/dat_unit_dying_api.h"
#include "dds/btl_scene_party_level_threshold_api.h"
#include "dds/btl_actor_attribute_api.h"
#include "dds/dat_affinity_value_api.h"
#include "dds/btl_conditional_encounter_roll_api.h"
#include "dds/btl_affinity_row.h"
#include "dds/btl_resolve_unit_value_api.h"
#include "dds/pty_active_roster_index_api.h"
#include "dds/pty_roster_api.h"
#include "dds/pty_party_unit_presence_api.h"
#include "dds/pty_frontline_promotion_api.h"
#include "dds/pty_status_query_api.h"
#include "dds/pty_event_penalty_api.h"
#include "dds/pty_field_hp_attrition_api.h"
#include "dds/pty_read_roster_stat_api.h"
#include "dds/pty_skill_recovery_api.h"
#include "dds/pty_item_quantity_update_view.h"
#include "dds/pty_accumulate_stat_gains_api.h"

#define DAT_EXTERNAL_SKILL_TABLE 0x20

#define DAT_INLINE_SKILL_COUNT 0x18

#define DAT_TABLE_SKILL_COUNT 8

#define DAT_SKILL_HP_BONUS_SMALL 0x200

#define DAT_SKILL_HP_BONUS_MEDIUM 0x201

#define DAT_SKILL_HP_BONUS_LARGE 0x202

#define DAT_SKILL_MP_BONUS_SMALL 0x203

#define DAT_SKILL_MP_BONUS_MEDIUM 0x204

#define DAT_SKILL_MP_BONUS_LARGE 0x205

#define DAT_PERCENT_SCALE 100

#define DAT_BONUS_SMALL_PERCENT 10

#define DAT_BONUS_MEDIUM_PERCENT 20

#define DAT_BONUS_LARGE_PERCENT 30

#define DAT_BOOSTED_RESOURCE_LIMIT 1000

#define DAT_BOOSTED_RESOURCE_MAX 999

#define DAT_STATUS_VALUE_MASK 0x7FFF

#define DAT_STATUS_STAT_OVERRIDE 0x1000

#define DAT_PROFILE_STAT_LIMIT 100

#define DAT_PROFILE_STAT_MAX 99

#define DAT_CALC_HIGH_MASK 0xFFFF0000

#define DAT_CALC_SKIP_STATUS_MASK 0x70000

#define DAT_QUARTER_PERCENT 25

#define DAT_CURRENCY_MAX 0x98967F

typedef struct UiObject {
    u8 unk_00[0x110];
    u32 flags;
    u8 unk_114[0x10];
    u16 index;
    u16 currentValue;
    u16 maximumValue;
    u8 unk_12A[4];
    u16 statusFlags;
} UiObject;

/* Add the signed per-unit stat byte to its profile adjustment and clamp
 * the existing signed result to 1..99. Stat index is not checked here. */
DatCalculatedStatValue datGetClampedProfileAdjustedStat(
    const dds_datUnitWork_t *unit, DatBaseStatIndex statIndex);

DatCalculatedStatValue datGetStatWithStatusOverride(
    const dds_datUnitWork_t *unit, DatBaseStatIndex statIndex);

#include "dds/dat_command_record_index.h"

enum {
    PTY_ACTIVE_ROSTER_COUNT = 5,
    PTY_RECOVERY_DELTA = 9999,
    PTY_MAX_LEVEL = 99,
    PTY_LEVEL_LIMIT = 100
};

enum {
    EVT_DEFAULT_STAT_VALUE = 100,
    EVT_ROSTER_DETAIL_KIND = 5,
    EVT_DEFAULT_PARTY_RECORD_FLAGS_MASK = 4,
    EVT_ALTERNATE_PARTY_RECORD_FLAGS_MASK = 0x20,
    EVT_CONTEXT_BYTES = 0x18
};

enum {
    PTY_ITEM_MAX_QUANTITY = 99,
    PTY_ITEM_QUANTITY_LIMIT = 100,
    PTY_ITEM_SINGLE_MAX = 1,
    PTY_ITEM_SINGLE_LIMIT = 2
};

enum {
    BTL_SCENE_RECORD_BYTES = 0x28,
    BTL_SCENE_ENEMY_SLOT_COUNT = 11,
    BTL_ENEMY_RECORD_BYTES = 0x4C,
    BTL_ENEMY_LEVEL_OFFSET = 5
};

extern s32 btlConditionalEncounterCachedInterval;
void evtRandomizeEntryValue(dds_datUnitWork_t *entry);

/* Step the script with these context values; clear the written flag, not the stored result. */
ScrVmInteger evtRunContext(ScrProcedureIndex procedureIndex, s32 firstArgument, s32 secondArgument, s32 commandIndex, u16 options);
s32 evtPushFirstRosterLevel(void);
s32 evtPushSecondRosterLevel(void);
s32 evtPushFirstRosterCurrentHp(void);
s32 evtPushSecondRosterCurrentHp(void);
s32 evtPushFirstRosterMaximumHp(void);
s32 evtPushSecondRosterMaximumHp(void);

/* Push the first entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushFirstRosterSelectedStat(void);

/* Push the second entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushSecondRosterSelectedStat(void);

/* Push the first entry's option-selected stat, retaining the same default selectors. */
s32 evtPushFirstRosterOptionStat(void);

/* Push the second entry's option-selected stat, retaining the same default selectors. */
s32 evtPushSecondRosterOptionStat(void);

/* Push the first entry's profile-adjusted stat, including its native status override. */
s32 evtPushFirstRosterStatEligibility(void);

/* Push the second entry's profile-adjusted stat, including its native status override. */
s32 evtPushSecondRosterStatEligibility(void);

/* Kind 5 selects the first entry's roster detail instead of the command-table stat. */
s32 evtPushSelectedStatOrRosterLowValue(void);

/* Kind 5 scales the command stat by the first entry's roster multiplier, then truncates. */
s32 evtPushSelectedScaledStat(void);
s32 evtPushSelectedStatGrade(void);

/* Options 1 and 2 select alternate command halfwords; other options push zero. */
s32 evtSelectScriptStatValue(void);

/* Select the alternate stat through the first entry's table value, not the context selector. */
s32 evtPushEntryIndexedStatOption(void);

/* Kind 5 selects the first entry's high roster detail instead of the command total. */
s32 evtPushSelectedTotalOrRosterHighValue(void);
s32 evtPushSelectedStatMaximum(void);

/* Store the script argument as the context result and mark it written. */
s32 evtStoreScriptParameterResult(void);
s32 evtPushScriptContextResult(void);
s32 evtPushEntryFlagBitInverted(void);

/* For positive ranges, return 1.0 plus a percentage jitter; the upper endpoint is excluded. */
s32 evtRollRandomScale(void);

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageCurrentValueForGroup(void);

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageMaximumValueForGroup(void);

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageActorStatForGroup(void);

/* Push the selected actor-group average for the requested attribute. */
s32 evtPushAvailableChoiceRatio(void);

/* Push the native battle float when a runtime exists, otherwise zero. */
s32 evtPushAvailableFloatValue(void);

/* Push the native battle integer when a runtime exists, otherwise zero. */
s32 evtPushAvailableIntegerValue(void);
s32 evtPushLevelMaxHpGrowth(void);
s32 evtPushLevelMaxMpGrowth(void);

/* Push the coarse HP-percentage table value; only exactly 100 percent uses index zero. */
s32 evtSelectStatGrade(void);
s32 evtPushLevelValueA(void);
s32 evtPushLevelValueB(void);
s32 evtPushLevelValueCCommand18D(void);
s32 evtPushLevelValueCCommand18E(void);
s32 evtScriptSelectRandomValue(void);
s32 evtPushRosterBaseValue(void);

/* Push the finer HP-percentage table value; only exactly 100 percent uses index zero. */
s32 evtSelectFineStatGrade(void);
u8 evtTestSolarPhaseOrModelFlag(DatBattleAdjustmentCondition condition);
u8 btlSelectConditionalEncounterGroup(s32 encounterIndex);

enum {
    BTL_LEVEL_GATE_PARTY_FLAG_ELIGIBLE = 4
};

BtlConditionalEncounterGaugeThreshold btlGetConditionalEncounterInterval(s32 encounterIndex);
s32 evtPushRequestedRosterPresence(void);

INCLUDE_ASM(const s32, "newdata/datCalc", datAddMaka);

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckMoney);

INCLUDE_ASM(const s32, "newdata/datCalc", datAddItem);

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckItem);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetItemMaxNum);

INCLUDE_ASM(const s32, "newdata/datCalc", datAddMag);

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckMag);

INCLUDE_ASM(const s32, "newdata/datCalc", datPartyRecovery);

INCLUDE_ASM(const s32, "newdata/datCalc", datPartyDevilRecovery);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetTotalCyuusei);

INCLUDE_ASM(const s32, "newdata/datCalc", datAddTotalCyuusei);

INCLUDE_ASM(const s32, "newdata/datCalc", datCallCalcGetBadStatus);

INCLUDE_ASM(const s32, "newdata/datCalc", datCallCalcGetBadStatus2);

INCLUDE_ASM(const s32, "newdata/datCalc", datResetBadStat);

INCLUDE_ASM(const s32, "newdata/datCalc", datRefDDS3GBWK5);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf46);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf47);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf48);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf49);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf50);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf51);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf52);

/* Raise the packed value to `minimumValue` (never lower it); reaching 0x40, 0x80 or 0x400 re-randomizes the entry. */
void sdfRaisePackedChannelValue(dds_datUnitWork_t *item, u32 minimumValue) {
    s32 channel;

    if ((item->status & SDF_PACKED_CHANNEL_MASK) < minimumValue) {
        item->status = (item->status & SDF_PACKED_STATUS_BIT) | (minimumValue & SDF_PACKED_CHANNEL_MASK);
        channel = item->status & SDF_PACKED_CHANNEL_MASK;
        switch (channel) {
        case 0x40:
        case 0x80:
        case 0x400:
            evtRandomizeEntryValue(item);
            break;
        }
    }
}

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf53);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf54);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf55);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf56);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf57);

/* Only an exact masked status of 0x1000 forces one; combinations of low
 * status bits do not take this path. Otherwise use the profile-adjusted stat. */
DatCalculatedStatValue datGetStatWithStatusOverride(
    const dds_datUnitWork_t *unit, DatBaseStatIndex statIndex) {
    if ((unit->status & DAT_STATUS_VALUE_MASK) == DAT_STATUS_STAT_OVERRIDE) {
        return 1;
    }
    return datGetClampedProfileAdjustedStat(unit, statIndex);
}

INCLUDE_ASM(const s32, "newdata/datCalc", datCheckSyojiSkill);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf58);

INCLUDE_ASM(const s32, "newdata/datCalc", datGetAisyoValueSub);

u32 datGetAisyoValue(datUnitWork_t *work, s32 attr)
{
    return (u16)datGetAisyo(work, attr);
}

u32 datGetAisyoFlag(datUnitWork_t *work, s32 attr)
{
    return datGetAisyo(work, attr) & 0xFFFF0000;
}

INCLUDE_ASM(const s32, "newdata/datCalc", datGetBadStatusAttr);

INCLUDE_ASM(const s32, "newdata/datCalc", datRefDDS3GBWK23);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf59);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf60);

void datLeaf8(void) {
}

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf61);

void datLeaf62(void) {
}

void datLeaf63(void) {
}

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf64);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf65);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf66);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf67);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf68);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf69);

INCLUDE_ASM(const s32, "newdata/datCalc", datFunc25);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf70);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf71);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf72);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf73);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf74);

void datLeaf75(void) {
}

void datLeaf76(void) {
}

void datLeaf77(void) {
}

void datLeaf78(void) {
}

void datLeaf79(void) {
}

INCLUDE_ASM(const s32, "newdata/datCalc", datCallGetRandIntA);

INCLUDE_ASM(const s32, "newdata/datCalc", datLeaf80);

u8 evtTestSolarPhaseOrModelFlag(DatBattleAdjustmentCondition condition) {
    u32 conditionType = condition >> 16;
    switch (conditionType) {
    case DAT_BATTLE_ADJUSTMENT_CONDITION_NONE:
        break;
    case DAT_BATTLE_ADJUSTMENT_CONDITION_SOLAR_PHASE_MASK:
        if (condition & (1 << evtGetMirroredSolarPhase()) & 0xFFFF) {
            return 1;
        }
        break;
    case DAT_BATTLE_ADJUSTMENT_CONDITION_MODEL_FLAG:
        if (evtBitCheck(condition & 0xFFFF)) {
            return 1;
        }
        break;
    }
    return 0;
}

u8 btlSelectConditionalEncounterGroup(s32 encounterIndex) {
    s8 enabled[3];
    s32 conditionIndex;
    u8 code;

    for (conditionIndex = 0; conditionIndex < 3; conditionIndex++) {
        enabled[conditionIndex] = evtTestSolarPhaseOrModelFlag(datBattleAdjustmentRecords[encounterIndex].conditions[conditionIndex]);
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[0];
    if (enabled[0] && enabled[1] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[1];
    if (enabled[0] && enabled[1]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[2];
    if (enabled[0] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[3];
    if (enabled[1] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[4];
    if (enabled[0]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[5];
    if (enabled[1]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[6];
    if (enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[7];
    return code == 8 ? 0 : code;
}

INCLUDE_ASM(const s32, "newdata/datCalc", datEncountCalcSub);

/* Reuse an active interval, or seed one for a nonempty encounter group. */
BtlConditionalEncounterGaugeThreshold btlGetConditionalEncounterInterval(s32 encounterIndex) {
    s32 variant;
    s32 total;
    s32 i;
    s32 interval;
    if (encounterIndex == 0) {
        return BTL_CONDITIONAL_ENCOUNTER_GAUGE_THRESHOLD_UNAVAILABLE;
    }
    variant = btlSelectConditionalEncounterGroup(encounterIndex);
    total = 0;
    for (i = 0; i < 20; i++) {
        total += datBattleAdjustmentRecords[encounterIndex].groups[variant].entries[i].weight;
    }
    if (total == 0) {
        return BTL_CONDITIONAL_ENCOUNTER_GAUGE_THRESHOLD_UNAVAILABLE;
    }
    if (btlConditionalEncounterCachedInterval > 0) {
        return btlConditionalEncounterCachedInterval;
    }
    interval = datBattleAdjustmentRecords[encounterIndex].groups[variant].interval;
    if (interval <= 0) {
        return BTL_CONDITIONAL_ENCOUNTER_GAUGE_THRESHOLD_UNAVAILABLE;
    }
    btlConditionalEncounterCachedInterval = dds3GetRandIntA(0, interval * 2 - 32) + 16;
    return btlConditionalEncounterCachedInterval;
}

INCLUDE_ASM(const s32, "newdata/datCalc", nbEncountCalc);
INCLUDE_SDATA(const s32, "newdata/datCalc", btlGetConditionalEncounterIntervalData);

INCLUDE_SDATA(const s32, "newdata/datCalc", nbEncountCalcData);

INCLUDE_SDATA(const s32, "newdata/datCalc", nbEncountCalcData2);

