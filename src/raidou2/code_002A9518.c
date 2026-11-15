#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "sdk_libc.h"
#include "dds/btl_effect_position.h"
#include "dds/dds3obj.h"
#include "dds/sdf_chip.h"
#include "dds/sdf_map_position_api.h"
#include "dds/common.h"
#include "dds/sdf_vu0.h"
#include "dds/btl_set_actor_effect_parameter_api.h"
#include "dds/btl_actor_resource_bank_api.h"
#include "dds/eff_offset_if_owner_flag_clear.h"
#include "dds/btl_command_effect.h"
#include "dds/btl_command_required_entry_match.h"
#include "dds/btl_ai_slot.h"
#include "dds/btl_packed_action_api.h"
#include "dds/btl_resource_name.h"
#include "dds/btl_task_state.h"
#include "dds/btl_task_condition.h"
#include "dds/btl_action.h"
#include "dds/btl_action_eligibility.h"
#include "dds/btl_ai_table.h"
#include "dds/btl_indexed_ui_resource_api.h"
#include "dds/btl.h"
#include "dds/btl_skill_name_index.h"
#include "dds/btl_model_state_ownership.h"
#include "dds/btl_analysis_panel_work.h"
#include "dds/btl_analysis_panel_wait_args.h"
#include "dds/btl_model_record.h"
#include "dds/sdf_chunk.h"
#include "dds/evt_unit.h"
#include "dds/mdl.h"
#include "dds/btl_state.h"
#include "dds/btl_task_args.h"
#include "dds/pcp_vu0.h"
#include "dds/dat_command.h"
#include "dds/dat_unit_id.h"
#include "dds/eff.h"

extern u32 btlRandomState;
extern s32 (*btlPackedEffectHandlers[])(s32, u32);

#include "dds/sdf_debug_api.h"

/* Actor-status providers select one complete 0x270-byte model record. */

typedef struct BtlObjLink {
    void *owner; /* 0x00: effect owner */
    union {
        s32 arg;
        u16 shortArg; /* Message/wait variants store only the low halfword. */
    };
    s32 elapsedTicks; /* 0x08: elapsed callback updates */
} BtlObjLink;

typedef struct BtlJyokyoState {
    u8 pad00[0x498];
    s32 dialogId;          /* 0x498 */
    s32 alternateDialogId; /* 0x49C */
    s32 thirdDialogId;     /* 0x4A0 */
} BtlJyokyoState;

u32 GetRand(u32 limit);
extern u32 GetRand(u32 limit);
s32 GetIntervalRand(s32 lower, s32 upper);

u32 GetRand(u32 limit) {
    btlRandomState = btlRandomState * 0x41c64e6d + 0x3039;
    return (btlRandomState >> 0x10) * (limit & 0xffff) >> 0x10;
}

s32 GetIntervalRand(s32 lower, s32 upper) {
    s32 lo = upper;
    s32 hi = lower;

    if (lo < hi) {
        s32 t = lo;
        lo = hi;
        hi = t;
    }
    lo = lo - hi + 1;
    return (s32)GetRand(0xFFFF) % lo + hi;
}

INCLUDE_SDATA(const s32, "code_002A9518", GetRandData);

