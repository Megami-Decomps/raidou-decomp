#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_001624D0.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/evt_unit.h"

#include "dds/evt_unit_color_transition.h"

void evtSetUnitColorRGB(evtUnitHandle_t *unit, s32 duration, u32 color);

/* Initialize an event unit's color transition for the requested duration. */
void evtSetUnitLightAndAmbient(evtUnitHandle_t *unit, s32 duration, u32 firstColor, u32 secondColor);

void evtSetUnitLightDir(evtUnitHandle_t *unit, EvtUnitDirectionTransitionFrames durationFrames);

void evtUnitLightOn(evtUnitHandle_t *unit);

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

#include "dds/evt_unit.h"

#include "dds/evt_unit_color_transition.h"

#include "dds/pcp_vu0.h"

#include "dds/eff_matrix4.h"

#include "dds/mdl.h"

#include "dds/bill_object_api.h"

extern u32 effBTLSkillGetInvokeFormation(void);

extern u32 effBTLFieldColorGetVariantSelector(void);

extern void effBattleMiscDirectionTo(BtlUnit *, EffectVectorRequest *, f32 *);

extern u32 btlCameraVectorHasNaN(void);

extern u32 btlBlendColorVec(f32 *, f32 *, f32);

/* Apply and restore a selected group's two-color and direction keyframe. */
void effBattleApplyUnitColorDirection(BtlUnit *unit, EffBattleUnitColorCommand *command, u32 frame);

void effBattleApplyUnitRgbKeyframe(BtlUnit *unit, EffBattleUnitRgbCommand *command, s32 frame);

extern void dds3AdminiCallSequence(s32 requestedMode, const void *requestData, u32 dataBytes, s32 markHistory);

void dds3AdminSubmitEmptyModeRequest(void);
/* End of declarations from src/dds1/game/code_001624D0.c */

void dds3AdminSubmitEmptyModeRequest(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}
