#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds3obj.h"
#include "effBattle.h"
#include "event.h"
#include "model.h"
#include "newdata.h"
#include "sdf_model_packet_callback.h"
#include "sdk_libc.h"
#include "dds/btl.h"
#include "dds/btl_effect_position.h"
#include "dds/btl_sound.h"
#include "dds/dds3_object_flags.h"
#include "dds/dds3_world_counter.h"
#include "dds/dds3_world_primary.h"
#include "dds/dds3obj.h"
#include "dds/eff_random.h"
#include "dds/eff_random_mod.h"
#include "dds/eff_transform.h"
#include "dds/evt_unit.h"
#include "dds/evt_unit_color_transition.h"
#include "dds/evt_unit_motion_api.h"
#include "dds/file_request_api.h"
#include "dds/mdl.h"
#include "dds/mdl_asset_request.h"
#include "dds/mdl_flag_api.h"
#include "dds/mdl_motion_api.h"
#include "dds/sdf_chip.h"
#include "dds/sdf_dev_slot_api.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdf_map_position_api.h"
#include "dds/sdf_model.h"
#include "dds/sdf_motion.h"
#include "dds/sdf_packet_alloc.h"
#include "dds/sdf_packet_builders.h"
#include "dds/sdf_packet_list.h"
#include "dds/sdf_vu_matrix.h"
#include "dds/sdk_libc.h"
#include "dds/snd_sequence_volume_pan.h"

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

extern void *memcpy(void *, const void *, unsigned int);

#ifndef DDS_ENUM_EvtUnitMotionState
#define DDS_ENUM_EvtUnitMotionState
typedef int EvtUnitMotionState;
#endif

#include "dds/btl_motion_selection_result.h"
#include "dds/sdk_ps2.h"
#include "dds/dds3_path.h"
#include "dds/sdf_model_packet_callback.h"
#include "dds/btl_motion_transform.h"
#include "dds/btl_unit_motion_duration_api.h"
#include "dds/btl_status_motion_override.h"
#include "dds/btl_reusable_model_api.h"
#include "dds/btl_camera_pose_step.h"
#include "dds/btl_actor_transform_parameters.h"
#include "dds/btl_select_transform_parameters_api.h"
#include "dds/common.h"
#include "dds/btl_unit_defeat_by_delta_result.h"
#include "dds/btl_action_entry_is_empty_api.h"
#include "dds/btl_action_state_api.h"
#include "dds/btl_selected_entry_api.h"
#include "dds/btl_unit_model_progress_api.h"
#include "dds/btl_quarter_health_result.h"
#include "dds/btl_horizontal_aim_api.h"
#include "dds/btl_actor_command_selector_value_api.h"
#include "dds/dds3_camera_update_flags.h"
#include "dds/file_config_option_api.h"
#include "dds/btl_command_use_error.h"
#include "dds/btl_command_cost_api.h"
#include "dds/btl_action_reaction_motion.h"
#include "dds/btl_action_press_result.h"
#include "dds/btl_fear_infliction_roll.h"
#include "dds/btl_command_hit_result.h"
#include "dds/btl_command_status_api.h"
#include "dds/btl_hunt_mp_recovery_api.h"
#include "dds/btl_action_state_selection.h"
#include "dds/btl_scene_counter_mode.h"
#include "dds/btl_command_result_kind_api.h"
#include "dds/btl_command_target_policy.h"
#include "dds/btl_command_target_eligibility.h"
#include "dds/btl_actor_roll_eligibility.h"
#include "dds/btl_actor_mode_acceptance.h"
#include "dds/sdf_vu0.h"
#include "dds/btl_aim_horizontal_direction_clamped_api.h"
#include "dds/btl_set_actor_effect_parameter_api.h"
#include "dds/btl_raise_party_record_status_value.h"
#include "dds/btl_actor_resource_bank_api.h"
#include "dds/btl_unit_fade_task_args.h"
#include "dds/btl_unit_color_fade_args.h"
#include "dds/btl_special_ability.h"
#include "dds/evt_world.h"
#include "dds/snd_pad.h"
#include "dds/btl_stage_task_cleanup.h"
#include "dds/btl_unit_geometry_api.h"
#include "dds/btl_actor_entry_code_api.h"
#include "dds/eff_offset_if_owner_flag_clear.h"
#include "dds/snd_slot.h"
#include "dds/dds3_process.h"
#include "dds/btl_task_state.h"
#include "dds/btl_task_poll_result.h"
#include "dds/btl_task_condition.h"
#include "dds/sdf_model_release_api.h"
#include "dds/btl_index_list_search_api.h"
#include "dds/btl_skill_name_index.h"
#include "dds/btl_model_state_ownership.h"
#include "dds/btl_deferred_unit_status_args.h"
#include "dds/btl_position_effect_task_args.h"
#include "dds/btl_position_lerp_task_args.h"
#include "dds/btl_actor_work.h"
#include "dds/btl_find_actor_for_owner_api.h"
#include "dds/sdf_texture_offset_list.h"
#include "dds/btl_state.h"
#include "dds/btl_model_record.h"
#include "dds/btl_task_args.h"
#include "dds/btl_selected_unit_effect_task_args.h"
#include "dds/btl_approach_task_args.h"
#include "dds/btl_unit_base_light_task_args.h"
#include "dds/btl_effect_node.h"
#include "dds/eff_field_color.h"
#include "dds/pcp_vu0.h"
#include "dds/ee_mmi.h"
#include "dds/fpu.h"
#include "dds/dat_state.h"
#include "dds/btl_model_and_sound_readiness_api.h"
#include "dds/btl_action.h"
#include "dds/btl_unit_tasks.h"
#include "dds/sdf.h"
#include "dds/file.h"
#include "dds/dat_command.h"
#include "dds/btl_command_id.h"
#include "dds/btl_command_effect.h"
#include "dds/dat_affinity.h"
#include "dds/pty_adjust_item_quantity_api.h"

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

#include "dds/btl_camera_mode_definition.h"

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
