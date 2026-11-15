#include "common.h"

#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "event.h"
#include "kernel_draw.h"
#include "newdata.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "dds/dds3_fade.h"
#include "dds/dds3_process.h"
#include "dds/dds3_task_add_child.h"
#include "dds/dds3_task_create.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/dds3_task_priority_query.h"
#include "dds/dsp_channel.h"
#include "dds/dsp_entry_api.h"
#include "dds/eff_random.h"
#include "dds/evt_message_window_api.h"
#include "dds/evt_solar.h"
#include "dds/evt_task.h"
#include "dds/fr_font.h"
#include "dds/fr_font_context.h"
#include "dds/itf_panel_draw.h"
#include "dds/mdl_flag_api.h"
#include "dds/sdf_chip.h"
#include "dds/sdf_gs_packet.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdf_linked_packet.h"
#include "dds/sdf_packet_alloc.h"
#include "dds/sdf_packet_builders.h"
#include "dds/sdf_packet_list.h"
#include "dds/sdk_libc.h"
#include "dds/snd_sequence_volume_pan.h"

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};

#include "dds/sdk_ps2.h"
#include "dds/dsp_active_api.h"
#include "dds/common.h"
#include "dds/mnu_camp_display.h"
#include "dds/mnu_camp_option_index_api.h"
#include "dds/mnu_camp_font_api.h"
#include "dds/dat_command.h"
#include "dds/eff_resource_slots.h"
#include "dds/eff_mapped_resource_destroy_result.h"
#include "dds/eff_resource_source_lifetime.h"
#include "dds/evt_viewer.h"
#include "dds/event.h"
#include "dds/evt_viewer_indexed_condition.h"
#include "dds/evt_camera_color_track_api.h"
#include "dds/mnu.h"
#include "dds/mnu_staff.h"
#include "dds/mnu_shop.h"
#include "dds/sdf.h"
#include "dds/evt_world.h"
#include "dds/evt_unit.h"
#include "dds/evt_event_pack_task.h"
#include "dds/evt_event_pack.h"
#include "dds/evt_start_event_api.h"
#include "dds/evt_sound_variation.h"
#include "dds/dat_state.h"
#include "dds/mnu_list.h"
#include "dds/eff.h"
#include "dds/eff_resource_records.h"
#include "dds/fld.h"
#include "dds/pty_adjust_item_quantity_api.h"

#define CAMP_TASK_NAME_BYTES 0x20

#define CAMP_TASK_DATA_BYTES 0x48

#define CAMP_TIMELINE_WRAP_FRAME 10

#define CAMP_TRACK_FIRST_OFFSET_TYPE 0x12

#define CAMP_CAMERA_COLOR_TRACK_TYPE 0x19

#define CAMP_KEY_LOW_BITS_MASK 0xFFF

#define CAMP_KEY_HIGH_BITS_SHIFT 12

#define CAMP_ENTRY_VALUE_FOLLOW_ALT 0

#define CAMP_ENTRY_VALUE_CLEAR 1

#define CAMP_ENTRY_NAME_CODE_BASE 2

#define CAMP_FONT_CONTEXT_WIDTH 0x960

#define CAMP_FONT_CONTEXT_HEIGHT 0x70

#define CAMP_DESCRIPTOR_WIDTH 0x200

#define CAMP_DESCRIPTOR_HEIGHT 0xE0

#define CAMP_OPTION_VALUE_MASK 3

#define CAMP_PRIMARY_OPTION_CLEAR_MASK 0xfffffffc

#define CAMP_SECONDARY_OPTION_CLEAR_MASK 0xfffffff3

#define CAMP_SECONDARY_OPTION_SHIFT 2

#define CAMP_SECONDARY_OPTION_MASK 0xc

#define CAMP_TRANSFORM_COMPONENT_COUNT 4

#define CAMP_TRANSFORM_MIDDLE_START 4

#define CAMP_TRANSFORM_LAST_START 8

#define CAMP_REGISTERED_ID_LIMIT 20

#define CAMP_TIMELINE_SCRIPT_REGISTER_BASE 200

#define CAMP_TIMELINE_SCRIPT_SLOT_COUNT 10

#define CAMP_STATUS_BATCH_COUNT 2

#define CAMP_STATUS_INITIAL_PARAMETER 15

#define CAMP_SLOT_LAST_ROW_INDEX 0x14

#define CAMP_PARTY_SCAN_LAST 4

#define CAMP_PARTY_ACTIVE_FLAG 1

#include "dds/pty_bullet_item_count_api.h"

typedef struct CampFlagRow {
    u32 messageSet;  /* 0x00: shop message resource number */
    s16 flag[8];     /* 0x04 */
    u8 value[9];     /* 0x14: [0] default, [i + 1] for flag[i] */
    u8 pad1D[3];
} CampFlagRow;

void mnuEvaluateCubicBezierPoint(f32 parameter, const s32 *x, const s32 *y, s32 *outX, s32 *outY);
extern void mnuEvaluateCubicBezierPoint(f32 parameter, const s32 *x, const s32 *y, s32 *outX, s32 *outY);
s32 mnuSolveCubicBezierYForX(s32 target, const s32 *x, const s32 *y);

typedef struct CampEntryEnableSet {
    s32 enabled[7];
} CampEntryEnableSet;

/* Scan the twenty-one flag IDs in descending slot order. */

typedef struct ShopSourcePriceEntry {
    u16 itemId;
    u8 type;
    u8 pricePercent;
} ShopSourcePriceEntry;

typedef struct ShopSourcePriceRow {
    u16 pricePercent;
    ShopSourcePriceEntry entries[0x60];
} ShopSourcePriceRow;

typedef struct CampTier {
    u32 threshold;
    s32 value;
} CampTier;

static inline s32 campSetHandler(MenuTerminalContext *context, s32 mode, void *callback) {
    return jmsSeqChange(&context->transitionWork, &context->popupState, mode, callback);
}

typedef struct ShopProgressPrice {
    f32 percent;
    u32 unk4;
} ShopProgressPrice;

typedef struct EffItemSlot44 {
    u32 id;
    u8 pad_04[0x40];
} EffItemSlot44;

/* A wide reward record carries its weight, kind, item ID and optional variant. */
typedef struct CampWideRewardEntry {
    u8 weight;
    u8 kind;
    u8 reserved02[2];
    s32 id;
    u16 variant;
    u8 reserved0A[2];
} CampWideRewardEntry;

typedef char CampWideRewardEntry_size_check[
    sizeof(CampWideRewardEntry) == 0xC ? 1 : -1];

typedef struct CampPriceTier {
    u8 limit;
    u8 percent;
} CampPriceTier;

typedef struct CampRateRow {
    u16 percent;
    u8 pad02[10];
} CampRateRow;

#include "dds/mnu_input.h"

#define EVT_DISPATCH_OPERATION_POLL 0

#define EVT_DISPATCH_OPERATION_PRIMARY 1

#define EVT_DISPATCH_OPERATION_SECONDARY 2

#define EVT_PROGRESS_SLOT_COUNT 8

#define EVT_PROGRESS_RECORD_WORDS 3

#define EVT_ALLOWED_ITEM_COUNT 3

#define EVT_PROGRESS_UNSIGNED_LIMIT 999999U

#define EVT_PROGRESS_FLAG_GATE_COUNT 1

typedef struct EvtFlagGate {
    s8 threshold;
    u8 pad1;
    u16 cue;
    u32 flag;
} EvtFlagGate;

#define MNU_INSTALL_STATE_TABLE(ctx, table, entry) \
    do { \
        (ctx)->popupTable = (table); \
        jmsSeqJump(&(ctx)->popupState, (entry)); \
    } while (0)

struct MenuList;

struct dds3ProcessID_t;

void mnuEvaluateCubicBezierPoint(f32 parameter, const s32 *x, const s32 *y, s32 *outX, s32 *outY) {
    f32 inverse = 1.0f - parameter;
    f32 parameterSquared = parameter * parameter;
    f32 inverseSquared = inverse * inverse;
    f32 inverseCubed = inverseSquared * inverse;
    f32 threeInverseSquared = inverseSquared * 3.0f;
    f32 threeInverseParameterSquared = (inverse * 3.0f) * parameterSquared;
    f32 xValue;
    f32 yValue;

    xValue = inverseCubed * (f32)x[0];
    xValue += (threeInverseSquared * parameter) * (f32)x[1];
    xValue += threeInverseParameterSquared * (f32)x[2];
    xValue += (parameterSquared * parameter) * (f32)x[3];
    yValue = inverseCubed * (f32)y[0];
    yValue += (threeInverseSquared * parameter) * (f32)y[1];
    yValue += threeInverseParameterSquared * (f32)y[2];
    yValue += (parameterSquared * parameter) * (f32)y[3];
    *outX = (s32)xValue;
    *outY = (s32)yValue;
}

s32 mnuSolveCubicBezierYForX(s32 target, const s32 *x, const s32 *y) {
    s32 currentX;
    s32 resultY;
    s32 nextX;
    f32 parameter = 0.5f;
    f32 step = 0.25f;

    mnuEvaluateCubicBezierPoint(parameter, x, y, &currentX, &resultY);
    for (;;) {
        if (currentX == target || step < 0.0009999999310821295f) {
            return resultY;
        }
        if (currentX < target) {
            parameter += step;
        }
        if (target < currentX) {
            parameter -= step;
        }
        step *= 0.5f;
        mnuEvaluateCubicBezierPoint(parameter, x, y, &nextX, &resultY);
        currentX = nextX;
    }
}
