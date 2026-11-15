#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/evt_unit.h"
#include "dds/evt_unit_color_transition.h"
#include "dds/common.h"
#include "dds/sdf_vu0.h"
#include "dds/sdf_chip.h"
#include "dds/eff_param.h"
#include "dds/eff_param_operations.h"
#include "dds/eff_thunder_vector.h"
#include "dds/par_cell_api.h"
#include "dds/sdf_heap_api.h"
#include "dds/btl_state.h"
#include "dds/eff.h"
#include "dds/eff_battle_color_keyframe.h"
#include "dds/eff_battle_transition_duration.h"
#include "dds/ee_mmi.h"
#include "dds/pcp_vu0.h"
#include "dds/eff_matrix4.h"
#include "dds/mdl.h"
#include "dds/bill_object_api.h"

extern void dds3AdminiCallSequence(s32 requestedMode, const void *requestData, u32 dataBytes, s32 markHistory);
void dds3AdminSubmitEmptyModeRequest(void);

void dds3AdminSubmitEmptyModeRequest(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}
