#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "effBattle.h"
#include "fileManager.h"
#include "newdata.h"
#include "sdk_libc.h"
#include "dds/btl.h"
#include "dds/btl_effect_position.h"
#include "dds/btl_sound.h"
#include "dds/btl_stage_task_cleanup.h"
#include "dds/dds3_fade.h"
#include "dds/dds3_fade_timing.h"
#include "dds/dds3_world_counter.h"
#include "dds/eff_random.h"
#include "dds/eff_random_mod.h"
#include "dds/evt_unit.h"
#include "dds/evt_unit_color_transition.h"
#include "dds/file_pac_completion_api.h"
#include "dds/file_request_api.h"
#include "dds/fld_area_path.h"
#include "dds/mdl_flag_api.h"
#include "dds/mdl_motion_api.h"
#include "dds/mnu_title_sound.h"
#include "dds/sdf_chip.h"
#include "dds/sdf_delayed_resource_check_api.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdf_map_position_api.h"
#include "dds/sdf_trig.h"
#include "dds/sdf_vu_matrix.h"
#include "dds/sdk_libc.h"
#include "dds/sdk_libm.h"
#include "dds/snd_sequence_volume_pan.h"

extern void *memcpy(void *, const void *, unsigned int);

#include "dds/sdk_ps2.h"
#include "dds/snd_packed_track_load_status.h"
#include "dds/btl_vec3.h"
#include "dds/btl_motion_transform.h"
#include "dds/btl_unit_motion_duration_api.h"
#include "dds/btl_camera_pose_api.h"
#include "dds/btl_camera_snapshots.h"
#include "dds/btl_camera_cursor.h"
#include "dds/btl_camera_path_preset.h"
#include "dds/btl_pair_camera_preset.h"
#include "dds/btl_camera_parameter_record.h"
#include "dds/btl_camera_mode_index.h"
#include "dds/common.h"
#include "dds/snd_battle_sound_file_request_status.h"
#include "dds/btl_action_entry_is_empty_api.h"
#include "dds/btl_horizontal_aim_api.h"
#include "dds/btl_linked_actor_eligibility.h"
#include "dds/btl_target_list_builder.h"
#include "dds/btl_unit_model_progress_api.h"
#include "dds/btl_quarter_health_result.h"
#include "dds/btl_camera_quaternion_pair.h"
#include "dds/btl_aim_horizontal_direction_clamped_api.h"
#include "dds/btl_set_actor_effect_parameter_api.h"
#include "dds/btl_action_camera_preset.h"
#include "dds/btl_actor_resource_bank_api.h"
#include "dds/btl_formation_spacing_cell.h"
#include "dds/btl_rain_effect_api.h"
#include "dds/btl_blend_color_api.h"
#include "dds/mnu_title_stream.h"
#include "dds/mnu_title_sound_buffer_status.h"
#include "dds/btl_tint_transition.h"
#include "dds/btl_task_poll_result.h"
#include "dds/btl_camera_timed_instruction.h"
#include "dds/btl_scene_light_params.h"
#include "dds/btl_scene_lighting_task_args.h"
#include "dds/sdf_motion.h"
#include "dds/sdf_packet_list.h"
#include "dds/sdf_packet_builders.h"
#include "dds/btl_unit_geometry_api.h"
#include "dds/btl_effect_table.h"
#include "dds/snd_slot.h"
#include "dds/snd_track_load_activity.h"
#include "dds/active_sound_node.h"
#include "dds/dds3_process.h"
#include "dds/btl_task_state.h"
#include "dds/btl_task_condition.h"
#include "dds/sdf_model.h"
#include "dds/btl_actor_work.h"
#include "dds/sdf_texture_offset_list.h"
#include "dds/btl_state.h"
#include "dds/btl_model_record.h"
#include "dds/btl_task_args.h"
#include "dds/btl_custom_fade_task_args.h"
#include "dds/btl_field_load_task_args.h"
#include "dds/file_pac.h"
#include "dds/btl_floor_load_task_args.h"
#include "dds/btl_floor_resource_readiness_api.h"
#include "dds/btl_sound_resource_use_result.h"
#include "dds/btl_referenced_battle_effect_task.h"
#include "dds/btl_sound_release_task.h"
#include "dds/actor_effect_task.h"
#include "dds/eff_field_color.h"
#include "dds/dds3obj.h"
#include "dds/pcp_vu0.h"
#include "dds/ee_mmi.h"
#include "dds/fpu.h"
#include "dds/dat_state.h"
#include "dds/mdl.h"
#include "dds/mdl_asset_request.h"
#include "dds/btl_action.h"
#include "dds/btl_unit_tasks.h"
#include "dds/sdf.h"
#include "dds/file.h"
#include "dds/dat_command.h"
#include "dds/btl_camera_setup_policy.h"

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

#include "dds/btl_marked_entry10_api.h"

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

#include "dds/btl_sound_handle_node.h"

typedef struct SoundTaskArgs {
    BtlUnit *actor;
    s32 option;
    u32 slot;
    s32 waitFrames;
} SoundTaskArgs;

#include "dds/sdf_debug_api.h"
#include "dds/btl_sound_load_frame_counter.h"

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
