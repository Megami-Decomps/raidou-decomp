#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "newdata.h"
#include "scr.h"
#include "sdf_resource.h"
#include "dds/dds3_area_room_key.h"
#include "dds/dds3_fade.h"
#include "dds/dds3_process.h"
#include "dds/dds3_task_lifecycle.h"
#include "dds/dds3_task_state_query.h"
#include "dds/dds3_world_secondary.h"
#include "dds/dds3obj.h"
#include "dds/eff_event_draw.h"
#include "dds/eff_random.h"
#include "dds/eff_transform.h"
#include "dds/evt_world_object_api.h"
#include "dds/itf_mes_panel_mode.h"
#include "dds/itf_mes_select_def_key_result.h"
#include "dds/itf_mes_window.h"
#include "dds/mdl_flag_api.h"
#include "dds/scr_integer_parameter_api.h"
#include "dds/scr_integer_return_value_api.h"
#include "dds/sdf_heap_api.h"
#include "dds/sdf_texture_file.h"

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

u32 dds3GetProcessData(const dds3ProcessID_t *task);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#include "dds/dsp_entry_api.h"
#include "dds/scr.h"
#include "dds/dsp_message_window_api.h"
#include "dds/dsp_channel.h"
#include "dds/dsp_active_api.h"
#include "dds/evt_message_window_api.h"
#include "dds/evt_active_entry_flag_api.h"
#include "dds/dds3_task_state.h"
#include "dds/itf_mes_entry_start_api.h"
#include "dds/mnu.h"
#include "dds/mnu_list.h"
#include "dds/mnu_terminal_work.h"
#include "dds/dat_state.h"
#include "dds/evt_world.h"
#include "dds/eff_light.h"
#include "dds/mnu_event_flag_entries.h"

struct sdfTexHandle_t;

struct MenuTerminalWork;

#define EVT_ACTIVE_ENTRY_LIMIT 0xC0

#define EVT_DISPLAY_VALUE_COUNT 0x10

#define EVT_LAST_DISPLAY_VALUE 0xF

#define EVT_SCRIPT_PROCESS_PRIORITY 0x7D0

#define DSP_WINDOW_ENTRY_FLAG 0x200000

#define DSP_WINDOW_DISABLE_FLAG_MAIN 0x800000

#define DSP_WINDOW_DISABLE_FLAG_SECONDARY 0x100000

#define DSP_WINDOW_CONTROL_ACTIVE 1

#define DSP_WINDOW_CONTROL_GATED 2

#define DSP_WINDOW_PANEL_BASE_Y 0x15F

#define EVT_PANEL_DRAW_COMMAND 0x53

#define EVT_PANEL_BACKGROUND_COLOR 0x30303040

#define EVT_SCROLL_TOP_FLAG 1

#define EVT_SCROLL_BOTTOM_FLAG 2

#define DSP_FLAG_EVENT_REQUIRED 0x902

#define DSP_FLAG_EVENT_HANDLED 0x907

#define DSP_FLAG_EVENT_SPECIAL_ENTRY 5

#define DSP_FLAG_EVENT_SCENE_ENTRY 3

#define DSP_FLAG_EVENT_PARTY_ENTRY 4

#define EVT_PARTY_SLOT_COUNT 5

#define DSP_AREA_TEXT_BYTES 0x19

#define DSP_NAME_TEXT_BYTES 0x13

#define DSP_DIALOG_TEXT_BYTES 0x11

typedef struct EvtActiveFlagTable {
    s32 unk0;
    s32 unk4;
    s8 flags[0];
} EvtActiveFlagTable;

typedef struct {
    u8 entryCount;
    u8 pad;
    u16 entryIndices[0];
} ActiveList;

u32 mnuClearTerminalPanelSelection(dds3ProcessID_t *task);

struct MenuTerminalWork;

extern s32 evtCommand_FIELD_CLS(void);

typedef struct QuadRecord {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s16 unk0C;
    s16 unk0E;
    s32 unk10;
} QuadRecord;

struct ItfMesSub;

/* Script opcode: set the supplied active-entry flag without bounds checking. */
ScrCommandResult evtOpSetActiveEntryFlag(void);

/* Script opcode: return a display-table value, clamping only its upper index.
 * It does not activate or test an active-entry flag. */
ScrCommandResult evtOpReadDisplayValue(void);

/* The outer panel owner is unresolved; its list pointer is shared by
 * the viewport flag and height consumers. */
typedef struct EvtListPanelRecord {
    u8 pad0[0x14];
    struct MenuList *list;
} EvtListPanelRecord;

/* Clear the terminal task's stored selection word and report completion. */
u32 mnuClearTerminalPanelSelection(dds3ProcessID_t *task) {
    s32 workAddress;

    workAddress = dds3GetProcessData(task);
    *(u32 *)(workAddress + 0x90) = 0;
    return 1;
}
