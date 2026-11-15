#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "event.h"
#include "kernel_draw.h"
#include "scr.h"
#include "sdfDevCons.h"
#include "sdf_packet_alloc.h"
#include "sdk_libc.h"
#include "dds/dds3_admin_control_api.h"
#include "dds/dds3_draw_buffer.h"
#include "dds/dds3_process.h"
#include "dds/dds3_task_create.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/dds3_task_priority_query.h"
#include "dds/eff_quaternion_nlerp.h"
#include "dds/fld_scene_sequence_api.h"
#include "dds/fr_font.h"
#include "dds/mnu_camp_state_ack_api.h"
#include "dds/scr.h"
#include "dds/sdf_asset_create.h"
#include "dds/sdf_asset_state.h"
#include "dds/sdf_chip.h"
#include "dds/sdf_dev_cons.h"
#include "dds/sdf_dev_console_node.h"
#include "dds/sdf_general_heap_api.h"
#include "dds/sdf_gs_blend.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdf_line_pair_packet_api.h"
#include "dds/sdf_packet_alloc.h"
#include "dds/sdf_packet_append.h"
#include "dds/sdf_packet_builders.h"
#include "dds/sdf_packet_header.h"
#include "dds/sdf_packet_list.h"
#include "dds/sdf_primitive_packet_api.h"
#include "dds/sdf_sif_command.h"
#include "dds/sdf_texture_draw_packet.h"
#include "dds/sdf_vu_matrix.h"
#include "dds/sdk_libc.h"
#include "dds/sdk_libkernl.h"

void *sdfChipAllocZeroDirect(s32 size);
void sdfChipFreeDirect(void *memory);

#include "dds/sdk_ps2.h"
#include "dds/evt_sel_state.h"
#include "dds/evt_start_event_api.h"
#include "dds/evt_script_event_id.h"
#include "dds/sdf_gs_header.h"
#include "dds/ee_mmi.h"
#include "dds/pcp_vu0.h"
#include "dds/common.h"
#include "dds/file_menu_start_mode.h"
#include "dds/sdf_vu0.h"
#include "dds/dds3_background_fade_api.h"
#include "dds/fld_lmap_task_presence.h"
#include "dds/fld_terminal_scene_state.h"
#include "dds/sdf_heap_overlay_block.h"
#include "dds/dds3_frame_draw_buffer.h"
#include "dds/evt_gradient_rect.h"
#include "dds/sdf_textured_rect.h"
#include "dds/sdf.h"
#include "dds/scr_instruction_stream_api.h"
#include "dds/brs_task.h"
#include "dds/fld_resource_wait_api.h"
#include "dds/itf.h"

enum {
    EVT_PACKET_LIST_BYTES = 0x20,
    EVT_GS_COMMAND_BYTES = 0x30,
    EVT_GS_TEST_PRIMARY = 0x47,
    EVT_GS_TEST_SECONDARY = 0x48,
    EVT_GS_ALPHA_PRIMARY = 0x42,
    EVT_FIXED_ALPHA_INTERPOLATION = 0x64,
    EVT_QUAD_DEFAULT_DEPTH = 0xFFFFFF,
    EVT_FRAME_REFERENCE_PACKET_BYTES = 0x40,
    EVT_FRAME_DRAW_RECORD_BYTES = 0x1F40,
    EVT_BACKGROUND_BLEND_FLAG = 0x100,
    EVT_DRAW_COLOR_BLEND_FLAG = 0x200,
    EVT_DRAW_VECTOR_BLEND_FLAG = 0x400,
    EVT_SAVED_VECTOR_BLEND_FLAG = 0x4000,
    EVT_QUAD_DESCRIPTOR_BYTES = 0x2C,
    EVT_QUAD_VERTEX_COUNT = 4,
    EVT_SELECTION_ALLOCATION_BYTES = 0x28,
    EVT_DEV_CONSOLE_BUFFER_BYTES = 0x200,
    BF_FLW0_MAGIC = 0x30574C46
};

extern s8 evtSelectionStateActive;
extern EvtSelState *evtSelectionState;
extern u8 sdfViewShakeVector[];

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} DrawVec4;

/* The allocated draw state's float at +0x1C is initialized to 1. */
typedef struct EvtDrawState {
    u8 pad00[0x1C];
    f32 value1C;
} EvtDrawState;

extern void evtUpdateSelectionStateInterpolation(void);
extern void evtSelStateDestroy(void);

typedef struct dds3ResourceNode_t {
    s32 unk0;
    struct dds3ResourceNode_t *next;
    s32 *ready;
} dds3ResourceNode_t;

/* SDK float-input unpack: the RGBA word and its 16-byte workspace stay in memory. */
static inline void eeUnpackRGBAWordFloatReadOnly(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

/* GS AD packet payload starts after the 0x20-byte command header. */
typedef struct EvtGsCommand {
    u8 pad00[0x20];
    u64 data;
    u64 registerId;
} EvtGsCommand;

s32 evtSelStateCreate(s32 countLimit, s16 frames, s32 firstValue, s32 secondValue);
u32 evtCheckSelectionState(void);
void evtDestroySelectionState(void);
void evtSelStateDestroy(void);

/* Zero frames leaves the prior state untouched; zero countLimit selects the -1 sentinel. */
s32 evtSelStateCreate(s32 countLimit, s16 frames, s32 firstValue, s32 secondValue) {
    EvtSelState *selectionState;
    if (frames == 0) {
        return 0;
    }
    if (evtSelectionStateActive != 0) {
        evtSelStateDestroy();
    }
    selectionState = sdfChipAllocZeroDirect(EVT_SELECTION_ALLOCATION_BYTES);
    evtSelectionState = selectionState;
    if (countLimit == 0) {
        selectionState->limit = EVT_SEL_STATE_UNBOUNDED_LIMIT;
    } else {
        selectionState->limit = countLimit;
    }
    evtSelectionState->rampUpFrames = firstValue;
    evtSelectionState->rampDownFrames = secondValue;
    evtSelectionState->amplitude[0] = frames;
    evtSelectionState->amplitude[1] = frames;
    evtSelectionState->amplitude[2] = frames;
    evtSelectionStateActive = 1;
    return 1;
}

/* Return zero only when inactive on entry; updating or destroying active state returns one. */
u32 evtCheckSelectionState(void) {
    EvtSelState *selectionState;
    if (evtSelectionStateActive == 0) {
        return 0;
    }
    selectionState = evtSelectionState;
    if (selectionState->limit > selectionState->count ||
        selectionState->limit == EVT_SEL_STATE_UNBOUNDED_LIMIT) {
        evtUpdateSelectionStateInterpolation();
    } else {
        evtSelStateDestroy();
        return 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0010DC38", evtUpdateSelectionStateInterpolation);

void evtDestroySelectionState(void) {
    evtSelStateDestroy();
}

void evtSelStateDestroy(void) {
    if (evtSelectionStateActive != 0) {
        sdfChipFreeDirect(evtSelectionState);
        evtSelectionStateActive = 0;
        SDF_VU0_LOADIDENTITYVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_STOREVECTOR(sdfViewShakeVector);
    }
}
INCLUDE_SDATA(const s32, "code_0010DC38", dds3Data40);

INCLUDE_SDATA(const s32, "code_0010DC38", dds3Data41);

INCLUDE_SDATA(const s32, "code_0010DC38", dds3Data42);

INCLUDE_SDATA(const s32, "code_0010DC38", dds3Data43);

INCLUDE_SDATA(const s32, "code_0010DC38", dds3Data44);

INCLUDE_SDATA(const s32, "code_0010DC38", dds3Data45);

