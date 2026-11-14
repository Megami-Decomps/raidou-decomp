#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_001FC7D8.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "sdk_libc.h"

#include "dds/btl_effect_position.h"

#include "dds/dds3obj.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_map_position_api.h"

void dds3GetPosPoseSet(dds3Basic_t *object);

/* Leaves the unit's transformed muzzle position in vf10. */
void effBTLModelGetSphereCenter(struct BtlUnit *unit);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

/* On success, write the requested MPOS record's position or matrix to VU state. */
SdfMapPositionLookupResult sdfModelGetMarkPos(
    struct dds_sdfModelRootNode_t *model, s32 id);

#include "dds/common.h"

#include "dds/sdf_map_position_api.h"

#include "dds/sdf_vu0.h"

#include "dds/btl_set_actor_effect_parameter_api.h"

#include "dds/btl_actor_resource_bank_api.h"

#include "dds/btl_effect_position.h"

#include "dds/eff_offset_if_owner_flag_clear.h"

#include "dds/btl_command_effect.h"

#include "dds/btl_command_required_entry_match.h"

#include "dds/btl_ai_slot.h"

#include "dds/btl_packed_action_api.h"

#include "dds/dds3obj.h"

#include "dds/btl_resource_name.h"

#include "dds/sdf_chip.h"

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

/* Declared before first use (were implicit int). */
extern s32 btlCreateAnalysisPanelTask(BtlUnit *unit, s32 duration);

extern s32 btlGetRegisteredTaskValueOrDefault(void);

extern void dds3DrawUnitObjectShadow(f32, f32, f32, f32, s32);

extern void btlSetCommandPanelFadeMode(s8, s32);

extern s8 btlHistoryCounter;

extern BtlAiScratchWork *btlActionScratchWork;

extern s32 btlIsLowHpActionReady(BtlUnit *unit, s32 input);

extern u32 btlRandomState;

extern s32 func_00202668(BtlTask *, s32);

extern s32 btlSelectLowestNonzeroHpTarget(BtlTask *, s32);

extern void btlSelectLowestHealthRateTarget(BtlTask *, s32);

extern void btlSelectLowestHealthElementBlockTarget(BtlTask *, s32);

extern s32 btlSelectLowestRankTarget(BtlTask *, s32);

extern void btlSelectHighestCurrentMpTarget(BtlTask *, s32);

extern s32 btlSelectTargetsPassingCheck(BtlTask *, s32);

extern void btlSelectTargetsByActionMask(BtlTask *, s32);

extern void btlSelectTargetsWithoutActionMask(BtlTask *, s32);

extern void btlSelectTargetsByMode(BtlTask *, DatUnitId targetUnitId);

extern void btlPickUnlinkedPartyTarget(BtlTask *, u32);

extern void btlPickLinkedPartyTarget(BtlTask *, u32);

extern void btlAppendSelfAfterTargetScan(BtlTask *);

extern s32 btlSelectTargetsExcludingActorUnit(BtlTask *);

extern void btlAppendEffectActorToCommandIndices(BtlTask *, s32);

extern void btlSelectTargetsBlockingElement(BtlTask *, s32);

extern s32 (*btlPackedEffectHandlers[])(s32, u32);

extern s32 btlWaitEffectTask();

extern s32 func_001FDA78();

extern s32 btlPollEffectWaitTask();

extern u32 btlHasRegisteredAphNamePanelTask(void);

extern void btlReplaceDialogTasksAndQueueMessage(s32, s32);

extern void itfMesSetTextSlotFromValue(s32, s32, s32, s32);

#include "dds/sdf_debug_api.h"

extern s8 dds3PadButtonState[];

void func_001FC990(void);

/* Actor-status providers select one complete 0x270-byte model record. */
/* Use a map anchor for X/Z while retaining the actor's own height. */
void btlDrawUnitStatusShadowDisc(BtlUnit *unit);

extern s32 btlRollAiBucket(void);

/* Requires task bit 6 clear and a matching 0x2A49 status bit.
 * The masked-status switch assigns 25-bucket tests to cases 0x40/0x400. */
BtlActionEligibilityResult btlCheckStatusActionEligibility(BtlTask *task);

/* Accepts a task's unit only when status bit 5 is set and task bit 6 is clear.
 * If bit 5 is the sole low status bit, the final check uses an external value. */
BtlActionEligibilityResult btlAiCheckStatusRollEligibility(BtlTask *task);

extern f32 btlUnitGetTopY(BtlUnit *unit);

extern s32 nbClipEffectNum(s32 screen[4]);

extern void btlBossDebugPrintf(const char *format, ...);

void btlSelectHpMpDisplayPositionVU(BtlUnit *unit);

extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);

void btlDrawCenteredUnsignedNumber(s32 x, s32 y, u32 number, u32 color, s32 *offsets);

/* Select a side-adjusted effect ID from the current status/action; -1 means none. */
s32 btlGetStatusActionEffectId(BtlTask *task);

extern BtlCommandEffect btlCommandEffectMappings[];

/* Retained operand groups can suppress the command's effect. */
BtlCommandEffectId btlGetCommandEffectId(BtlTask *task, BtlCommandId command);

s32 effOffsetIfOwnerFlagClear(BtlUnit *owner, s32 base);

/* Number display: initialize its saved anchor, then project it with the
 * DDS1-specific display gate and twelve fixed screen offsets. */
extern f32 btlLinkedNumberScreenOffsets[12][2];

extern s32 btlWouldUiValueFallBelowQuarter(BtlUnit *, s32);

s32 btlUpdateLinkedNumberDisplay(BtlLinkedEffectArgs *args);

/* Decrement the linked-number task's unit counter without underflowing zero. */
void effDecrementFirstCountdown(BtlLinkedEffectArgs *args);

typedef struct BtlObjLink {
    void *owner; /* 0x00: effect owner */
    union {
        s32 arg;
        u16 shortArg; /* Message/wait variants store only the low halfword. */
    };
    s32 elapsedTicks; /* 0x08: elapsed callback updates */
} BtlObjLink;

extern BtlRuntimeTask *btlAllocTask(s32);

/* Create the selected numbered-display task with its frame count starting at zero. */
BtlRuntimeTask *btlCreateLinkedEffectTask(BtlUnit *owner, s32 value, u8 kind);

/* Decrement the counter-display task's unit counter without underflowing zero. */
void effDecrementSecondCountdown(BtlLinkedEffectArgs *args);

/* Create the counter-display task; its kind is stored as a byte. */
BtlRuntimeTask *btlCreateEffectCounterTask(BtlUnit *owner, s32 kind);

extern s32 btlCreateSkillNamePanel(const u8 *text);

extern s32 btlHasRegisteredSkillNamePanelTask(void);

extern u8 *D_003BAA8C;

extern const u8 *D_00360458[];

extern const u8 *D_00360460[];

s32 btlPollActorOrEntryLabelTask(BtlObjLink *link);

extern s32 btlPollActorOrEntryLabelTask();

BtlRuntimeTask *btlCreateEffObjD(BtlUnit *owner, BtlSkillNameIndex skillNameIndex);

extern u8 *D_003BAA84;

extern s32 btlCreateSkillNamePanel(const u8 *text);

extern s32 btlHasRegisteredSkillNamePanelTask(void);

s32 btlPollTimedTaskLink(BtlObjLink *link);

extern s32 btlPollTimedTaskLink();

BtlRuntimeTask *btlCreateOwnerLinkedTimedTask(BtlUnit *owner, s32 skillNameIndex);

extern u8 btlActionPassLabel[];

extern u8 btlActionRetreatLabel[];

extern const u8 *D_00360438[];

extern const u8 *D_00360448[];

extern const u8 *D_0036044C[];

extern const u8 *D_00360450[];

s32 btlPollCategoryLabelTask(BtlObjLink *link);

extern s32 btlPollCategoryLabelTask();

/* Preserve the owner's effect data in the new task. */
BtlRuntimeTask *btlCreateEffObjA(BtlUnit *owner, s32 arg);

typedef struct BtlJyokyoState {
    u8 pad00[0x498];
    s32 dialogId;          /* 0x498 */
    s32 alternateDialogId; /* 0x49C */
    s32 thirdDialogId;     /* 0x4A0 */
} BtlJyokyoState;

s32 btlJyokyoEffectUpdate(BtlObjLink *link);

BtlRuntimeTask *btlCreateEffObjB(BtlUnit *owner, s32 messageId);

s32 btlUpdateLinkedDialogueEffect(BtlObjLink *link);

BtlRuntimeTask *btlCreateEffObjC(BtlUnit *owner, s32 messageId);

/* Return completion on the first poll; this effect task has no per-frame work. */
s32 btlCompleteEffectTaskImmediately(void *unused);

BtlRuntimeTask *btlCreateEffectTask3E(BtlUnit *owner, u16 messageId);

s32 btlPollTimedPresentationTask(BtlObjLink *link);

extern s32 btlPollTimedPresentationTask();

BtlRuntimeTask *btlCreateOwnerLinkedTimedPresentation(BtlUnit *owner, s32 messageId);

BtlRuntimeTask *btlCreateEffectWaitTask(BtlUnit *owner, u16 mode);

extern u32 btlRequestAnalysisPanelClose(void);

BtlRuntimeTask *btlCreateEffectTask44(BtlUnit *owner);

u32 btlNextScaledRandom(u32 limit);

extern u32 btlNextScaledRandom(u32 limit);

s32 btlRandomInclusiveRange(s32 lower, s32 upper);

s32 btlAllocAndCheck(BtlTask *task);

u32 btlAssignTaskResultAndArgument(BtlTask *task);

extern void btlShiftActorStateHistory(BtlTask *task, s8 flag);

void btlAdvanceHistoryCounter(BtlTask *task);

void btlClearNodeFlags(void);

extern s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction);

s32 btlDispatchPackedActionWithScratch(BtlAiScratchActor actorTask, BtlUnit *owner, s32 mask);

/* Top ten bits select the callback; the lower 22 bits are its argument. */
s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction);

extern AiSpecies *datEnemyAiRecords;

extern void btlSelectTierMatrixAiRoute(BtlUnit *unit, s32 species, s32 *row, s32 requestedRow);

extern s32 btlRunAiAction(BtlTask *task, s32 id, u32 packedAction);

/* Choose a row and a weighted slot of the unit's species AI table and run that action. */
s32 btlRunRandomWeightedAiTableAction(BtlTask *task);

void btlShiftActorStateHistory(BtlTask *task, s8 flag);

/* Choose command-record-driven targets; retain the native zero mode argument. */
void btlChooseCommandActionTargets(BtlTask *context);

/* Delegate minimum nonzero-current-HP selection for this command, with mode zero. */
void btlPickCommandLowestHpTarget(BtlTask *context);

/* Delegate minimum HP-percentage selection without changing native fallback modes. */
void btlPickCommandLowestHpRatioTarget(BtlTask *context);

/* Pick a lowest-HP element-block candidate for the command's actor and action. */
void btlPickCommandLowestHpBlockTarget(BtlTask *actor, s32 action);

/* Forward command selection with its native zero second argument. */
void btlPickCommandLowestLevelTarget(BtlTask *context);

/* Select the highest-current-MP candidate through the native mode-zero entry. */
void btlPickCommandHighestMpTarget(BtlTask *context);

/* Select by group element checks; action is the query input, not a status mask. */
void btlPickCommandElementCheckedTarget(BtlTask *commandContext, s32 action);

/* Select by status-mask presence. */
void btlPickCommandTargetWithStatusMask(BtlTask *actor, s32 mask);

/* Select by status-mask absence. */
void btlPickCommandTargetWithoutStatusMask(BtlTask *actor, s32 mask);

/* Forward mode selection. */
void btlSelectCommandTargetsByMode(BtlTask *actor, s32 mode);

/* Current-command adapter for the unlinked-party selector. */
void btlPickUnlinkedCommandTarget(BtlTask *task, u32 input);

/* Current-command adapter for the linked-party selector. */
void btlPickLinkedCommandTarget(BtlTask *task, u32 input);

/* Append the command's own actor after the native scan. */
void btlAppendActorToCommandTargets(BtlTask *actor, s32 unused);

/* Default selection excludes the actor, with native self fallback; unused stays unused. */
void btlPickCommandTargetExcludingActor(BtlTask *context, s32 unused);

/* Append the global effect actor, distinct from this command's own actor. */
void btlAddEffectActorTargetToCommand(BtlTask *context);

/* Select element-block candidates. */
void btlPickCommandElementBlockTarget(BtlTask *actor, s32 mask);
/* End of declarations from src/dds1/game/code_001FC7D8.c */

u32 btlNextScaledRandom(u32 limit) {
    btlRandomState = btlRandomState * 0x41c64e6d + 0x3039;
    return (btlRandomState >> 0x10) * (limit & 0xffff) >> 0x10;
}

s32 btlRandomInclusiveRange(s32 lower, s32 upper) {
    s32 lo = upper;
    s32 hi = lower;

    if (lo < hi) {
        s32 t = lo;
        lo = hi;
        hi = t;
    }
    lo = lo - hi + 1;
    return (s32)btlNextScaledRandom(0xFFFF) % lo + hi;
}

INCLUDE_SDATA(const s32, "code_0027CBE8", D_r1_0046DAFC);

