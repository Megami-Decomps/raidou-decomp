#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "newdata.h"
#include "sdk_libc.h"
#include "dds/eff_random_mod.h"
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
#include "dds/dds3_process.h"
#include "dds/dat_state.h"
#include "dds/dat_calculated_stat_value_api.h"
#include "dds/btl_actor_attribute_api.h"
#include "dds/pty_roster_api.h"
#include "dds/dat_command.h"
#include "dds/dds3_task_lifecycle.h"

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

#include "dds/sdf_debug_api.h"
#include "dds/btl_command_affinity_api.h"

s32 btlSumOrAverageActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask,
    DatBaseStatIndex baseStatIndex, s8 allowDisabled);

s32 btlAverageActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask, DatBaseStatIndex baseStatIndex);

#include "dds/btl_preemptive_outcome_result.h"

s32 btlAverageActorAttribute(
    BtlPartyRecordFlagsMask partyRecordFlagsMask, DatBaseStatIndex baseStatIndex) {
    return btlSumOrAverageActorAttribute(partyRecordFlagsMask, baseStatIndex, 0);
}
