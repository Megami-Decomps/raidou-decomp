#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "basic.h"
#include "dds3obj.h"
#include "billboard.h"
#include "draw.h"
#include "scr.h"
#include "draw3d.h"
#include "field.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "event.h"
#include "effect.h"

extern f32 R360_LR(f32 a, f32 b);
extern f32 fabsf(f32);
f32 R360_LR(f32 a, f32 b);
f32 fldPlayerCalc360_8(f32 aKakudo);
f32 fldPlayerCalc360_4(f32 aKakudo);

/* Native surface-category branch is retained by the localized operand below. */

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "sdk_libc.h"
#include "dds/btl.h"
#include "dds/dds3_basic_array_cursor.h"
#include "dds/dds3_fade.h"
#include "dds/dds3_object_payload.h"
#include "dds/dds3_slot1_handler_api.h"
#include "dds/dds3_task_create.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/dds3_task_priority_query.h"
#include "dds/dds3_world_counter.h"
#include "dds/dds3_world_secondary.h"
#include "dds/dds3_world_selection.h"
#include "dds/dds3_world_value.h"
#include "dds/dds3obj.h"
#include "dds/eff_blur.h"
#include "dds/eff_inner_float_api.h"
#include "dds/evt_solar.h"
#include "dds/evt_world.h"
#include "dds/evt_world_object_api.h"
#include "dds/evt_world_source_transform.h"
#include "dds/dds3_process.h"
#include "dds/mdl_flag_api.h"
#include "dds/mdl_motion_api.h"
#include "dds/mnu_camp_state_ack_api.h"
#include "dds/scr.h"
#include "dds/sdf_delayed_resource_check_api.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdf_message.h"
#include "dds/sdf_model_scalars.h"
#include "dds/sdf_trig.h"
#include "dds/sdf_vu_matrix.h"
#include "dds/sdk_libc.h"
#include "dds/snd_sequence_volume_pan.h"

extern void *memcpy(void *, const void *, unsigned int);

#include "dds/sdk_ps2.h"
#include "dds/fld_area_work.h"
#include "dds/fld_archive_load_state.h"
#include "dds/fld_runtime_reset_api.h"
#include "dds/fld_area_encounter_table.h"
#include "dds/sdf_packet_list.h"
#include "dds/ee_mmi.h"
#include "dds/dds3_task_runner.h"
#include "dds/dds3_admin_control_api.h"
#include "dds/pcp_vu0.h"
#include "dds/common.h"
#include "dds/fld_camera_link_mode.h"
#include "dds/pty_status_query_api.h"
#include "dds/pty_field_hp_attrition_api.h"
#include "dds/dds3_object_flags.h"
#include "dds/sdf_vu0.h"
#include "dds/fld_scene_status_api.h"
#include "dds/fld_scene_ready_api.h"
#include "dds/fld_area_resource_ready_api.h"
#include "dds/fld_location_coordinate_flags.h"
#include "dds/fld_movement_hold_state_api.h"
#include "dds/fld_scene_command_state_api.h"
#include "dds/fld_camp_check_state_api.h"
#include "dds/fld_scene_transition_gate_api.h"
#include "dds/fld_next_scene_request_api.h"
#include "dds/mnu_title_stream.h"
#include "dds/eff_random.h"
#include "dds/dds3_path.h"
#include "dds/sdf_dev_state.h"
#include "dds/field_stage.h"
#include "dds/fpu.h"
#include "dds/fld.h"
#include "dds/fld_scene_sequence_api.h"
#include "dds/evt_random_status_clear_api.h"
#include "dds/mdl.h"
#include "dds/dat_state.h"
#include "dds/dat_new_cycle_state.h"
#include "dds/dat_world_field_flags.h"
#include "dds/fld_resource_wait_api.h"
#include "dds/fld_sequence_controller.h"
#include "dds/file_selection_api.h"
#include "dds/file_slot_flags_change_api.h"
#include "dds/fld_player_scene_state.h"
#include "dds/fld_spark_controller_state.h"

extern char D_0039FCA0[];

struct dds3Basic_t;

#include "dds/fld_encounter_request.h"

/* Return to-minus-from after integer-degree reduction and one wrap adjustment. */
f32 fldAngleDifference(f32 fromAngle, f32 toAngle);
f32 fldSnapAngleToCompassPoint(f32 angle);
extern u64 D_003BAB70[], D_003BAB78[], D_003BAB80[], D_003BAB88[], D_003BAB90[];
extern u64 D_003BAB98[], D_003BABA0[], D_003BABA8[], D_003BABB0[];

#include "dds/sdf_debug_api.h"

typedef struct FieldVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FieldVec4;

/* Area-state consumption markers are separate from shared game-state flags. */
enum {
    FIELD_TRANSITION_CONSUMED_FLAG = 2,
    FIELD_SECONDARY_SCENE_CONSUMED_FLAG = 4,
    FIELD_PRIMARY_SCENE_CONSUMED_FLAG = 8
};

#include "dds/fld_encounter_roll_entry.h"

enum {
    FIELD_ENCOUNTER_ROLL_ENTRY_COUNT = 16
};

#define FIELD_ENCOUNTER_PERCENT_RANGE 100U

#include "dds/dds3_world_primary.h"
#include "dds/btl_conditional_encounter_roll_api.h"

extern char D_0039FCA0[], D_0039FCB0[];
extern void evtDisableMoonDraw(void);

struct dds_dds3ObjectBaseControlWorkData_t;

#include "dds/fld_encounter_runtime_api.h"

extern char D_0039FD30[], D_0039FD40[];
extern char D_0039FD30[], D_0039FD40[];

INCLUDE_ASM(const s32, "field/fldPlayer", R360_LR);

f32 fldSnapAngleToCompassPoint(f32 angle) {
    f32 compass[16] = {0.0f, 22.5f, 45.0f, 67.5f, 90.0f, 112.5f, 135.0f, 157.5f, 180.0f, 202.5f, 225.0f, 247.5f, 270.0f, 292.5f, 315.0f, 337.5f};
    f32 best;
    s32 index;
    f32 diff;

    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(fldAngleDifference(0.0f, angle));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(fldAngleDifference(22.5f, angle));
    if (diff < best) {
        best = diff;
        index = 1;
    }
    diff = fabsf(fldAngleDifference(45.0f, angle));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(fldAngleDifference(67.5f, angle));
    if (diff < best) {
        best = diff;
        index = 3;
    }
    diff = fabsf(fldAngleDifference(90.0f, angle));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(fldAngleDifference(112.5f, angle));
    if (diff < best) {
        best = diff;
        index = 5;
    }
    diff = fabsf(fldAngleDifference(135.0f, angle));
    if (diff < best) {
        best = diff;
        index = 6;
    }
    diff = fabsf(fldAngleDifference(157.5f, angle));
    if (diff < best) {
        best = diff;
        index = 7;
    }
    diff = fabsf(fldAngleDifference(180.0f, angle));
    if (diff < best) {
        best = diff;
        index = 8;
    }
    diff = fabsf(fldAngleDifference(202.5f, angle));
    if (diff < best) {
        best = diff;
        index = 9;
    }
    diff = fabsf(fldAngleDifference(225.0f, angle));
    if (diff < best) {
        best = diff;
        index = 10;
    }
    diff = fabsf(fldAngleDifference(247.5f, angle));
    if (diff < best) {
        best = diff;
        index = 11;
    }
    diff = fabsf(fldAngleDifference(270.0f, angle));
    if (diff < best) {
        best = diff;
        index = 12;
    }
    diff = fabsf(fldAngleDifference(292.5f, angle));
    if (diff < best) {
        best = diff;
        index = 13;
    }
    diff = fabsf(fldAngleDifference(315.0f, angle));
    if (diff < best) {
        best = diff;
        index = 14;
    }
    diff = fabsf(fldAngleDifference(337.5f, angle));
    if (diff < best) {
        index = 15;
    }
    return compass[index];
}

f32 fldPlayerCalc360_8(f32 aKakudo) {
    f32 best;
    s32 index;
    f32 diff;
    while (aKakudo >= 360.0f) {
        aKakudo -= 360.0f;
    }
    while (aKakudo < 0.0f) {
        aKakudo += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(R360_LR(0.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(R360_LR(45.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 1;
    }
    diff = fabsf(R360_LR(90.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(R360_LR(135.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 3;
    }
    diff = fabsf(R360_LR(180.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(R360_LR(225.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 5;
    }
    diff = fabsf(R360_LR(270.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 6;
    }
    diff = fabsf(R360_LR(315.0f, aKakudo));
    if (diff < best) {
        index = 7;
    }
    switch (index) {
    case 0:
        aKakudo = 0.0f;
        break;
    case 1:
        aKakudo = 45.0f;
        break;
    case 2:
        aKakudo = 90.0f;
        break;
    case 3:
        aKakudo = 135.0f;
        break;
    case 4:
        aKakudo = 180.0f;
        break;
    case 5:
        aKakudo = 225.0f;
        break;
    case 6:
        aKakudo = 270.0f;
        break;
    case 7:
        aKakudo = 315.0f;
        break;
    }
    return aKakudo;
}

f32 fldPlayerCalc360_4(f32 aKakudo) {
    f32 best;
    s32 index;
    f32 diff;
    while (aKakudo >= 360.0f) {
        aKakudo -= 360.0f;
    }
    while (aKakudo < 0.0f) {
        aKakudo += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(R360_LR(0.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(R360_LR(90.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(R360_LR(180.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(R360_LR(270.0f, aKakudo));
    if (diff < best) {
        index = 6;
    }
    switch (index) {
    case 0:
        aKakudo = 0.0f;
        break;
    case 2:
        aKakudo = 90.0f;
        break;
    case 4:
        aKakudo = 180.0f;
        break;
    case 6:
        aKakudo = 270.0f;
        break;
    }
    return aKakudo;
}
