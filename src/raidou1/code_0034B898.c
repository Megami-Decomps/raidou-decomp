#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_00282850.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/sdk_ps2.h"

#include "dds/dsp_entry_api.h"

#include "dds/brs_level_progress_api.h"

#include "dds/mnu_input.h"

#include "dds/sdf_packet_list.h"

#include "dds/itf_draw_grid.h"

#include "dds/eff_resource_slots.h"

#include "dds/eff_resource_records.h"

#include "dds/common.h"

#include "dds/sdf_delayed_resource_check_api.h"

#include "dds/sdf_vu0.h"

#include "dds/evt_stage_test_api.h"

#include "dds/evt_stage_test_resource_work_result.h"

#include "dds/pty_bullet_item_count_api.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/evt_world_object_api.h"

#include "dds/evt_stage_model_scale.h"

#include "dds/ui_color.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/file_pac.h"

#include "dds/file_pac_cleanup_api.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_chip.h"

#include "dds/fr_font.h"

#include "dds/sdf_dev_state.h"

#include "dds/dds3obj.h"

#include "dds/sdf.h"

#include "dds/sdf_pac_state.h"

#include "dds/sdf_projection.h"

#include "dds/sdf_sif_command.h"

#include "dds/kwln.h"

#include "dds/dds3_task_create.h"

#include "dds/pcp_vu0.h"

#include "dds/mnu.h"

#include "dds/mnu_entry_cost_api.h"

#include "dds/mnu_entry_use_status_api.h"

#include "dds/mnu_popup_eligibility.h"

#include "dds/mnu_party_entry_lookup.h"

#include "dds/mnu_panel_state.h"

#include "dds/mnu_panel_state_prefix.h"

#include "dds/mnu_staff.h"

#include "dds/mnu_ability_target_category.h"

#include "dds/eff.h"

#include "dds/mnu_shop.h"

#include "dds/mdl.h"

#include "dds/mdl_asset_request.h"

#include "dds/dat_state.h"

#include "dds/prf_current_record_view.h"

#include "dds/evt_stage_test.h"

#include "dds/dat_command.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/btl.h"

#include "dds/btl_party_command_use_result.h"

#include "dds/dds3_world_primary.h"

#include "dds/prf_profile_cap.h"

#include "dds/sdf_packet_alloc.h"

/* Declared before first use (were implicit int). */
extern s32 datAddHp(datUnitWork_t *unit, s32 delta);

extern s32 datAddMp(datUnitWork_t *unit, s32 delta);

extern void evtFreeStage(void);

extern void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *resource, s32 resourceIndex, s32 x, s32 y, s32 width, s32 height);

extern void dds3DebugGraphSetEnabled(s8 enabled);

extern void mnuDrawIconPanelDefaultFlag(s32 x, s32 y, s32 depth, s32 fade, MenuPanelHandles *list, s32 drawArg);

extern void mnuUpdateWindowPanelHandleStates(MenuPanelHandles *panel);

extern s32 sdfApplyCommandResults(s32 channel, datUnitWork_t *sourceRecord, datUnitWork_t *item);

extern void sndDestroyFileQueueWrapper(struct SoundHandleNode *node);

struct _FRQ;

extern struct _FRQ *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, struct _FRQ *);

extern s32 frFontDrawGlyphChain(struct _FRQ *, s8, u32);

extern void effDrawSpriteWithCornerFade(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);

extern char D_003BC7A0[];

#define MNU_PANEL_ITEM_COUNT 5

#define MNU_PANEL_STATE_BYTES 0x64

#define MNU_SPRITE_STATE_BYTES 0x20

#define MNU_SIMPLE_SPRITE_BYTES 0x28

#define MNU_PANEL_ITEM_BYTES 0x90

#define MNU_TRANSITION_LIMIT 0x100

#define MNU_TRANSITION_STEP 8

#define MNU_GRADIENT_FADE_STEP 0x20

#define MNU_PROFILE_PHASE_PERIOD 0x200

#define MNU_PROFILE_PHASE_STEP 12

#define MNU_PANEL_TEXTURE_COUNT 9

#define MNU_POPUP_STATE_BYTES 0x4c

#define MNU_POPUP_ENTRY_MARK_BITS 0x60000

#define MNU_PARTY_SLOT_COUNT 5

#define MNU_INPUT_PRIORITY_BIT 1

#define MNU_PAD_TRIGGER_BIT 2

#define MNU_COMMAND_RECORD_BYTES 0x38

#define MNU_COMMAND_ID_MASK 0xFFFF

#define MNU_PERCENT_SCALE 100

#define MNU_COMMAND_USE_STATUS_BOUNDARY 0x200

#define MNU_RATIO_QUARTER_PERCENT 25

#define MNU_RATIO_HALF_PERCENT 50

#define EVT_STAGE_ENTRY_INDEX_MASK 0xFFFF

#define EVT_STAGE_ENTRY_BYTES 60

#define EVT_STAGE_MODEL_RECORD_BYTES 624

#define EVT_STAGE_COLUMN_VALUE_COUNT 8

#define EVT_STAGE_MOTION_SUPPRESS_FALLBACK 1

#define EVT_STAGE_DEFAULT_BLEND_FRAMES 15.0f

#define EVT_STAGE_PROJECTION_BIAS 2048.0f

#define EVT_STAGE_DEFAULT_TARGET_Z -400.0f

#define EVT_STAGE_SCREEN_WORK_BYTES 0x20

#define EVT_STAGE_USE_ENTRY_MOTION 0xffffffffffffffff

extern void evtStageTestQueueMotionSegment(s32, f32, f32);

extern void mnuInitPairedPanelResources(MenuEffectPair *, s32, s32 *, s32, s32, EffectSlotSet *, const s32 *);

extern void evtStageTestCreateModelEffect(s32);

extern void btlUpdateJobPositionFromModel(struct SoundHandleNode *node);

extern void mnuApplyModelCamera(dds3ModelHandle_t *);

extern void evtStageTestApplyEntryRotation(dds3ModelHandle_t *);

extern void evtStageTestUpdateCamera(void);

extern f32 aViewEye[4];

extern f32 aViewTarget[4];

extern f32 aViewDown[4];

extern u8 sdfViewShakeVector[];

extern u8 evtStageTestViewUpVector[];

extern Matrix4 sdfViewMatrix;

extern u8 D_003270F0[];

extern void evtStageTestAdvanceMotionQueue(void);

extern void mnuQueuePartyPanelSlotBlend(MenuPageWindow *, s32, u32, s32);

extern u8 fldEsutoMaChk(void);

extern u8 fldRiberaMaChk(void);

extern void fldEsutoMaStart(void);

extern void fldRiberaMaStart(void);

extern void mnuPopulatePartyPanelSlot(datUnitWork_t *entry, s32 unused, u32 index, PartyPanel *panel);

extern dds3Basic_t *evtAddCamera(f32 *pos, f32 *rot);

extern char D_003BC7C8[];

extern void evtStageTestApplyInitialMotion(s32 initialMotionIndex);

extern s32 D_003BC7B4;

extern s32 mnuLookupRangeEntry(u16);

extern void mnuReleaseResourceList(MenuPanelHandles *);

extern EvtStageTestResourceWorkResult evtStageTestPollModelRequest(void);

typedef struct MenuPanelItem MenuPanelItem;

extern void mnuClearPanelGroupSelection(MenuPanelGroup *);

extern MenuPanelItem *mnuCreatePanelItem(void);

extern void mnuInitializePanelGroupGridSlots(MenuPanelItem *, EffectSlotSet *, s32);

extern void mnuInitializePanelItemGridSlots(MenuPanelItem *, EffectSlotSet *, s32);

extern void mnuFreePanelItemWork(MenuPanelItem *);

extern void mnuStorePanelItemValue(MenuPanelItem *, u32);

extern void mnuSetPanelItemSelection(MenuPanelItem *, s32);

extern void mnuSetPanelItemOption(MenuPanelItem *, u32);

extern char D_003B2608[];

extern u8 evtStageTestViewEyeVector[];

extern u8 evtStageTestViewTargetVector[];

/* Battle stage test viewer state. Retail addresses it partly through
 * &evtStageTestState.queue.slot[0], hence the negative offsets. */
extern StageTestState evtStageTestState;

extern void btlStopStage(void);

extern s32 evtBattleStageTestScreen(dds3ProcessID_t *task);

extern void mnuDrawPartyMemberCommandPanel(s32, s32, s32, MenuPageWindow *, s32, s32);

extern void mnuDrawPartyMemberStatusPanel(s32, s32, s32, MenuPageWindow *, s32, s32);

extern s32 mnuReservePartyPanelRowParameters[];

void mnuDispatchListPanel(s32 x, s32 y, s32 z, MenuPageWindow *menu, s32 panelIndex, s32 surface);

void mnuCalcListEntryOffset(s32 *position, MenuPageWindow *menu, s32 panelIndex);

/* Increment while below the transition limit; the native add is not clamped. */
void mnuAdvancePanelTransition(MenuPageWindow *menu);

/* Apply a temporary override to the selected node while drawing its panel. */
void mnuDrawPanelWithTemporaryOverride(s32 x, s32 y, s32 z, s32 overrideValue, MenuPageWindow *menu, s32 surface);

/* Draw the selected panel or all visible/additional panels, then advance the transition. */
void mnuDrawStageTestList(s32 x, s32 y, s32 z, s32 overrideValue, MenuPageWindow *menu, s32 surface);

/* Same as mnuDrawStageTestList with no override value. */
void mnuDrawPanelListDefault(s32 x, s32 y, s32 depth, MenuPageWindow *menu, s32 surface);

typedef struct MenuPanelState {
    MenuPanelStatePrefix common;
    MenuGridSlot firstSlot; /* 0x20 */
    s32 secondOffsetX; /* 0x28: second row's position relative to the panel. */
    s32 secondOffsetY; /* 0x2C */
    MenuGridSlot secondSlot; /* 0x30 */
    MenuGridSlot thirdSlot; /* 0x38 */
    s32 thirdOffsetX; /* 0x40: third row's position relative to the panel. */
    s32 thirdOffsetY; /* 0x44 */
    MenuGridSlot fourthSlot; /* 0x48 */
    s32 fourthOffsetX; /* 0x50: fourth row's position relative to the panel. */
    s32 fourthOffsetY; /* 0x54 */
    u32 fourthValueC; /* 0x58 */
    u8 pad5C[4];
    MenuPanelHandles *resourceHandle; /* 0x60 */
} MenuPanelState;

typedef char MenuPanelStateSizeCheck[(sizeof(MenuPanelState) == MNU_PANEL_STATE_BYTES) ? 1 : -1];

typedef char MenuPanelStateResourceHandleOffsetCheck[((u32)&((MenuPanelState *)0)->resourceHandle == 0x60) ? 1 : -1];

/* Allocate a zeroed native panel state with the requested dimensions. */
MenuPanelState *mnuCreatePanelState(s32 width, s32 height);

void mnuDestroyPanelState(MenuPanelState *panel);

void mnuSetPanelFirstGridSlot(MenuPanelState *panel, u32 valueA, u32 valueB,
                               EffectSlotSet *resource, u32 index);

void mnuSetPanelSecondGridSlot(MenuPanelState *panel, s32 offsetX, s32 offsetY,
                                EffectSlotSet *resource, u32 index);

void mnuInitializePanelResource(MenuPanelState *panel, EffectSlotSet *resource,
                                EffMappedResource *target);

void mnuSetPanelThirdGridSlot(MenuPanelState *panel, s32 offsetX, s32 offsetY,
                               EffectSlotSet *resource, u32 index);

void mnuSetPanelFourthGridSlot(MenuPanelState *panel, s32 offsetX, s32 offsetY,
                                EffectSlotSet *resource, u32 index, u32 additionalValue);

void mnuSetPanelState(MenuPanelState *panel,
                      MenuPanelSelectionIndex selectionIndex);

/* Paired sprites derive from their row; see docs/idioms.md,
 * "Row-derived paired panel sprites". */
void mnuDrawPanelState(s32 x, s32 y, s32 depth, MenuPanelState *panel, s32 drawArg);

/* Create the five panel items owned by this group and clear its selection. */
MenuPanelGroup *mnuCreatePanelGroup(EffectSlotSet *parent);

/* Release every owned panel item before releasing the group allocation. */
void mnuDestroyPanelGroup(MenuPanelGroup *group);

/* Configure all five panel items against the same grid object. */
void mnuUpdateFiveListEntries(MenuPanelGroup *group, EffectSlotSet *gridObject);

void mnuSetPanelGroupSelection(MenuPanelGroup *group, MenuPanelGroupSelection selection);

/* Store the no-selection sentinel. */
void mnuClearPanelGroupSelection(MenuPanelGroup *group);

MenuPanelGroupSelection mnuGetPanelGroupSelection(MenuPanelGroup *group);

struct MenuPanelItem {
    u8 pad00[0x10];
    u32 value10;
    s32 value14;
    s32 value18;
    u32 option;
    s32 selection;
    MenuGridSlot groupGridSlots[7]; /* 0x24 */
    MenuGridSlot gridSlots[5]; /* 0x5C */
    u8 pad84[4];
    u32 initialValue; /* 0x88 */
    s32 selectionRamp; /* 0x8C */
};

extern void mnuSetPanelItemValueAndOption(MenuPanelItem *item, s32 value, s32 option);

extern void mnuDrawPanelItemValue(s32 x, s32 y, s32 depth, s32 mode, MenuPanelItem *item, s32 layer);

/* Draw the five stat panels of a group (values from the party record, if any), then ramp the
 * group's fade value up to 0x100. */
void mnuDrawAndAdvancePanelGroup(s32 x, s32 y, s32 depth, datUnitWork_t *partyRecord, MenuPanelGroup *group, s32 drawFlags);

void mnuSetGroupSelection(MenuPanelGroup *group, s32 index, s32 selection, u32 option);

/* Create a zeroed sprite-resource state with its native initial value. */
MenuSpriteState *mnuCreateSpriteState(struct EffectSlotSet *resourceSet0,
                                      struct EffectSlotSet *resourceSet1,
                                      struct EffectSlotSet *resourceSet2);

void mnuFreeSpriteStateWork(MenuSpriteState *spriteState);

/* Select the range entry's sprite variant before submitting its draw request. */
void mnuDrawRangeSpriteVariant(u32 x, u32 y, u32 depth, u32 color,
                                    u16 rangeId, s32 alternate, EffectSlotSet *drawArg, u32 texture);

extern const u8 *D_003BAA8C;

/* The source table holds text and empty-slot XY banks, each with two columns. */
void mnuDrawPartyInfoSprites(s32 x, s32 y, s32 depth, datUnitWork_t *entry,
                             MenuSpriteState *spriteState, s32 surface);

/* Create the DDS1 sprite record, including its five resource sets and blend weight. */
MenuSimpleSpriteState *mnuAllocateSimpleSprite(
    struct EffectSlotSet *resourceSet0, struct EffectSlotSet *resourceSet1,
    struct EffectSlotSet *resourceSet2, struct EffectSlotSet *resourceSet3,
    struct EffectSlotSet *resourceSet4);

void mnuFreeSimpleSpriteWork(MenuSimpleSpriteState *sprite);

extern s32 prfBuildRawSkillList(u16, PrfSkillList *);

extern const MenuPoint D_003BC780[];

extern const MenuPoint D_003B2450[];

extern const MenuPoint D_003B2468[];

extern void mnuDrawRangeCostAndIcon(s32, s32, s32, s32, u32, datUnitWork_t *, u16, s32, s32, EffectSlotSet *, EffectSlotSet *, u32);

/* Reuse the frame-coordinate workspace for the next phase's row count.
 * See docs/idioms.md, "Profile contents: coordinate-to-counter workspace". */
void mnuDrawPartyProfilePanelContents(s32 x, s32 y, s32 depth, datUnitWork_t *actor, s32 profile, MenuSimpleSpriteState *panel, s32 surface);

void mnuResetGradientFadeColor(MenuGradientFade *state, s32 color);

void mnuSetStateValue(u32 *out, u32 value);

extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

/* Draw two clear corners and two blended corners, then step the bounded blend value. */
void mnuDrawAndStepGradientFade(MenuGradientFade *state, s32 surface);

/* Set both effect positions; only the first Y comes from the active menu entry. */
void mnuSetPairedEffectPositions(MenuEffectPair *pair);

s32 mnuRateByThreshold(MenuEffectPair *owner);

extern void itfGridSetQuantizedBounds(EffectSlotSet *, s32, s32, s32, s32, s32);

/* Split the scaled grid width by its rate category and clear the paired grid. */
void mnuUpdatePairedEffectBounds(MenuEffectPair *owner);

/* Cycle through four indexed settings while refreshing the paired effects. */
void mnuCyclePairedEffectSetting(MenuEffectPair *pair);

extern s32 itfGridLookupValueOrDefault(EffectSlotSet *object, s32 key);

/* Draw the paired effect at its current rate and initialize its alternate on demand. */
void mnuDrawAlternatingRatioBarEffects(s32 x, s32 y, s32 z, u32 opacity, MenuEffectPair *owner,
                   s32 surface);

void mnuCreatePairedEffects(MenuEffectPair *pair);

/* Release the two effect-batch handles in the native object-word layout. */
void mnuReleasePairedEffectBatches(s32 *objectWords);

void mnuInitPairedPanelResources(MenuEffectPair *pair, s32 initialValue, s32 *settings,
                   s32 positionY, s32 span, EffectSlotSet *source, const s32 *table);

void mnuDrawPanelSequenceByRow(s32 x, s32 y, s32 depth, s32 color, s32 variant, s32 texture);

/* Release the nine sprite texture handles, then the paired effect batches. */
void mnuReleaseSpriteTextures(s32 *objectWords);

/* Select the packed ratio color; a zero divisor retains the default color. */
u32 mnuGetPanelRatioColor(s32 useDefault, s32 amount, s32 divisor);

extern char D_003BC788[];

extern char D_003BC790[];

void mnuDrawRatioPanelContents(s32 x, s32 y, s32 depth, u32 color, u32 fade,
                  s32 value, s32 total, const MenuEffectPair *style,
                  u32 drawFlags);

void func_00284880(void);

/* Allocate a zeroed native panel item and initialize its three default values. */
MenuPanelItem *mnuCreatePanelItem(void);

void mnuInitializePanelGroupGridSlots(MenuPanelItem *item, EffectSlotSet *gridObject, s32 panelIndex);

/* Bind five grid object/index references and initialize their quantized bounds.
 * The x/y members in this path hold object addresses and entry indices, not coordinates. */
void mnuInitializePanelItemGridSlots(MenuPanelItem *item, EffectSlotSet *gridObject,
                                     s32 panelIndex);

void mnuStorePanelItemValue(MenuPanelItem *item, u32 value);

void mnuSetPanelItemValueAndOption(MenuPanelItem *item, s32 value, s32 option);

void mnuSetPanelItemSelection(MenuPanelItem *item, s32 selection);

void mnuSetPanelItemOption(MenuPanelItem *item, u32 option);

void mnuFreePanelItemWork(MenuPanelItem *item);

extern void mnuDrawPanelItemDetail(s32, s32, s32, u32, s32, MenuPanelItem *, s32);

extern char D_003BC798[];

void mnuDrawPanelItemDetail(s32 x, s32 y, s32 depth, u32 opacity, s32 mode,
                            MenuPanelItem *item, s32 layer);

void mnuDrawPanelItemValue(s32 x, s32 y, s32 depth, s32 mode, MenuPanelItem *item, s32 layer);

void mnuSetProfilePanelValues(MenuProfilePanel *panel, s32 capValue, s32 progressValue);

extern s8 scrGetSelectedOperandIndex(datUnitWork_t *);

/* Create a profile panel from the selection state's current profile ID and record. */
MenuProfilePanel *mnuCreateProfilePanel(datUnitWork_t *selectionState);

void mnuFreeProfilePanelWork(MenuProfilePanel *panel);

void mnuCacheProfilePanelGridPositions(MenuProfilePanel *panel, u32 grid, u32 fillIndex,
                                     u32 backgroundIndex, u32 completedIndex);

extern void uiDrawTexturedSurfaceAtFarDepth(s32);

extern void uiPrepareMenuBackgroundSurface(s32, s32);

extern void uiDrawSurfaceAtNearDepth(u32);

/* Draw the filled profile bar and its animated background strips. */
void mnuDrawProfilePanel(s32 x, s32 y, s32 z, MenuProfilePanel *panel, s32 surface);

/* Draw first, then advance the native phase by twelve with a single period subtraction. */
void mnuDrawAndAdvanceProfilePanel(s32 x, s32 y, s32 z, MenuProfilePanel *panel, s32 option);

/* Clear the complete native popup-transition state, including saved entry addresses. */
void mnuClearPanelTransitionState(MenuPopupState *state);

void mnuApplyPanelTransitionAction(s32 action, MenuPopupEntry *entry, MenuPopupState *state, u32 argument);

/* Pop saved entries with their leave callbacks until the state count reaches zero. */
void mnuDrainPanelTransitions(MenuPopupState *state, struct dds3ProcessID_t *task);

s32 mnuHasPopupSelectionFlag(s32 *flags);

s32 mnuDispatchPopupTransition(MenuPopupState *work, s32 *entrySlot, s32 mode,
                               void *callback);

u8 mnuIsPopupEntryValue(s32 stateAddress, s32 entryAddress);

/* Bind the entry address and retain only the entry's low sixteen flag bits. */
void mnuSetPopupEntry(s32 *entrySlot, MenuPopupEntry *entry);

/* Bind the entry with the native insert-before-top bit after preserving low flags. */
void mnuSetPopupEntryFlagged(s32 *entrySlot, MenuPopupEntry *entry);

/* Bind the entry with both native marking bits after preserving low flags. */
void mnuAttachAndMarkMenuEntry(s32 entrySlotAddress, s32 entryAddress);

/* Bind the current popup entry only when its saved address is nonzero. */
void mnuBindPresentMenuEntry(MenuPopupState *state, s32 *entrySlot);

void mnuPopulatePartyPanelSlot(datUnitWork_t *partyRecord, s32 unused, u32 index, PartyPanel *panel);

/* Populate occupied party slots; empty slots retain a unit index of -1. */
void mnuInitPartyPanelSlots(PartyPanel *panel);

extern s8 dds3PadButtonState[];

extern u8 D_00324530[];

/* Translate requested inputs using native sign-bit/trigger-bit and nonzero tests.
 * Input bit zero has exclusive priority over every other result bit. */
MnuInputActionFlags mnuMapPadMaskToFlags(MnuPadButtonQueryMask requestedButtons);

/* Bits 0x8000 and 0x4000 return early in that order; state bits may suppress navigation SE. */
void mnuPlayInputSound(s32 unused, s32 inputFlags, u32 *stateFlags);

/* Only bit 0x8000 returns early; other flags may play multiple sounds of the selected kind. */
void mnuPlayInputSoundKind(s32 inputFlags, s8 soundKind);

/* Dispatch the input flags with the native default sound kind. */
void mnuPlayDefaultInputSounds(u32 inputFlags);

/* Find the first occupied slot with the same table ID; zero also serves as no-match. */
s32 mnuFindMatchingPartyEntryIndex(const datUnitWork_t *borrowedEntry);

extern s8 mnuSkillCategoryRangeMapRows[];

extern u16 mnuSkillCategoryRangeBounds[];

extern u16 mnuSkillCategoryRangeUpperBounds[];

/* Match a half-open range, then return its signed mapping value; unmatched IDs return zero. */
s32 mnuLookupRangeEntry(u16 rangeId);

/* Read a bound word from the primary table or its one-word-shifted alternate view. */
u16 mnuPickPairedTableValue(s32 boundWordIndex, s32 alternate);

/* For nonnegative indices, select among nonzero signed mapping values in row order. */
s32 mnuGetIndexedNonzeroEffect(s32 valueIndex);

extern s8 mnuSkillCategoryRangeBoundWordIndices[];

/* Accumulate selected mapping values, then read the requested adjacent range bound. */
u16 mnuLookupPartyTableValue(u32 valueCount, s32 baseIndex, s32 alternate);

/* Return the attribute mask only for kind 2, which selects flag-mask semantics. */
u16 mnuGetCommandFlagMask(s32 commandId);

/* Return the native value/cost kind from the low-sixteen-bit command ID. */
u8 mnuGetRangeEntryKind(u32 commandId);

/* HP-kind values use max HP as a percentage basis; other kinds retain the stored value. */
u16 mnuGetAdjustedEntryValue(s32 commandId, datUnitWork_t *actor);

u16 mnuGetAdjustedPartyRangeValue(s32 id);

/* Compare the stored raw HP/MP cost; equality is affordable and other kinds pass. */
MnuEntryCostAffordability mnuCanAffordEntryCost(
    u16 commandId, const datUnitWork_t *actorEntry);

/* Return the menu's three-way unavailable-marker status. */
MnuEntryUseStatus mnuGetEntryUseStatus(datUnitWork_t *actorEntry, u16 commandId);

/* Report insufficient raw HP/MP cost; equality and unhandled kinds return zero. */
s32 mnuIsEntryCostUnaffordable(u16 commandId, datUnitWork_t *actor);

/* Deduct an affordable stored HP/MP cost; unhandled kinds succeed without a deduction. */
s32 mnuConsumeEntryCost(s32 commandId, datUnitWork_t *actorEntry);

/* Map target_type to the menu's single-entry or party-group selection category. */
MnuAbilityTargetCategory mnuGetAbilityTargetCategory(u16 commandId);

void datDelDevil(u16 ability, datUnitWork_t *target, datUnitWork_t *entry);

u32 ptyCheckFieldUseOverride(s32 context, s32 ability, s32 target, datUnitWork_t *entry);

s32 ptySkillApplyFieldUseEffect(MenuPageWindow *context, u16 ability, datUnitWork_t *target, datUnitWork_t *selectedEntry);

/* Test for the exact signed-byte marker one, not merely a nonzero selector byte. */
u8 mnuIsAbilityValueMarked(u32 commandId);

s32 ptyGetAffinityKind(s32 affinityId, s32 index);

s32 ptyGetAffinityFlagsWithoutOverride(s32 affinityId, s32 index);

u8 mnuIsBulletItemId(s32 id);

u8 mnuIsSolarPricedItemId(s32 id);

u16 mnuGetPartyEntryMenuValue(datUnitWork_t *entry);

/* Count inventory plus one for every matching slot value, without an occupancy test.
 * This counter accepts only IDs 0xA0..0xBE. */
PtyBulletItemCount ptyCountBulletItem(PtyBulletItemId bulletItemId);

u32 mnuSetPartyEntryMenuValue(datUnitWork_t *entry, u32 menuValue);

u16 mnuGetPartyEntryMenuValue(datUnitWork_t *entry);

extern u16 D_0037CE00[];

/* Match an item ID, select its mode-dependent SE ID, and start it; id changes roles. */
s32 sndPlayPartyItemSe(u32 id, s32 soundMode);

extern u32 datComputeSkillBoostedMaxHp(datUnitWork_t *);

extern u32 datComputeSkillBoostedMaxMp(datUnitWork_t *);

/* Apply a permanent stat/capacity item and refill eligible vitals.
 * Returns 0 for other items, 1 when accepted, or 2 when capped and already full. */
s32 btlItemApplyPermanentBonus(u16 itemId, datUnitWork_t *unit);

s32 btlItemApplyDirectEffect(MenuPageWindow *context, u16 item, s32 mode,
                             datUnitWork_t *unit);

/* Return the first matching status-icon index in native priority order, or none. */
MnuPartyStatusIconIndex mnuGetPartyStatusIconIndex(const datUnitWork_t *partyRecord);

/* Return one bit for the first occupied matching table ID, or zero when absent. */
s32 mnuGetMatchingPartyEntryMask(const datUnitWork_t *borrowedEntry);

/* Classify the truncated signed percentage into default, below-half, and
 * below-quarter bands. A zero divisor returns the default class. */
MnuQuarterHalfPercentClass mnuClassifyQuarterHalfPercent(s32 amount, s32 divisor);

u8 mnuIsStaffMenuInputBlocked(void);

void evtStageTestSetModelScalingEnabled(s32 enabled);

dds3ModelHandle_t *evtStageTestGetActiveModel(void);

u32 evtStageTestGetSlotModelId(void);

/* Apply the native motion-index bounds and refresh the entry pose.
 * An empty loaded motion table can still select -1 after the upper-bound check. */
void evtStageTestSetEntryIndex(s32 encodedIndex, s32 motionIndex);

/* Advance the indexed entry's animation frame and request a stage refresh.
 * The native negative-delta guard tests frame - delta, not frame + delta. */
void evtStageTestAddEntryValue(s32 encodedIndex, f32 delta);

/* Add integer XYZ deltas to the indexed entry's floating-point position. */
void mnuOffsetPanelPosition(s32 encodedIndex, s32 dx, s32 dy, s32 dz);

/* Add integer XYZ deltas to the indexed entry's stored rotation angles. */
void mnuOffsetPanelTarget(s32 encodedIndex, s32 dx, s32 dy, s32 dz);

/* Return the motion byte selected by the low half of the encoded entry index. */
u8 evtStageTestGetEntryMotionIndex(s32 encodedIndex);

/* Return the indexed entry's animation frame. */
f32 evtStageTestGetEntryFrame(s32 encodedIndex);

/* Copy exactly three position components; any output W component is untouched. */
void evtStageTestCopyEntryPosition(s32 encodedIndex, f32 *position);

/* Copy exactly three stored rotation components. */
void evtStageTestCopyEntryRotation(s32 encodedIndex, f32 *rotation);

extern u8 D_003B2520[];

extern u8 D_003B2530[];

extern u8 D_003B2540[];

extern u8 D_003B2550[];

extern void dds3SetSoftenBias(s32 frames, f32 first, f32 second);

extern void dds3SetLightColor(s32 blendFrames, s32 lightIndex, const f32 *color);

extern void dds3SetLightDir(s32 blendFrames, s32 lightIndex, const f32 *direction);

extern void dds3SetAmbientColor(s32 mode, f32 *color);

extern void dds3SetFogColor(s32 mode, f32 *color);

extern void dds3SetFogPara(s32 mode, f32 x, f32 y, f32 z, f32 w);

/* Restore color targets, projection settings and the stage viewer's view vectors. */
void evtStageTestResetViewAndLighting(void);

/* Set the horizontal projection offset while retaining the native GS-coordinate bias. */
void mnuSetStageTestCameraOffset(f32 offset);

extern s8 D_0037CE90[];

extern u8 D_0037CE98[];

void evtStageTestInit(s32 mode);

void evtStageTestDestroyModelEffect(void);

/* Release the viewer's model/effect and discard its pending effect, except in mode 1. */
void btlStopStage(void);

/* Stop the viewer and reset both projection offsets to the native coordinate bias. */
void mnuResetWorkFloats(void);

/* Remember the model asset request result for the stage viewer. */
MdlAssetRequestResult evtStageTestRequestModelAsset(
    s32 resource, s32 modelId, MdlAssetRequestMode requestMode);

/* Request an indexed entry's model using the active slot's resource and option zero. */
void mnuForwardTableByte(s32 encodedIndex);

/* Test the stage queue's pending-selection bit; the poller passes its flags word. */
u32 mnuHasPendingBlockFlag(u32 *queueFlags);

/* Promote the staged selection and clear its pending flag. Returns 1 only
 * when a slot was committed; the poller supplies &evtStageTestState.queue. */
s32 mnuCommitPendingBlock(StageTestQueue *queue);

/* Discard the pending-selection marker without copying either selection slot. */
void evtStageTestClearPendingFlag(void);

/* Return UNCHANGED for the active model; otherwise stage a selection.
 * An in-progress request uses the pending slot rather than replacing the active slot. */
EvtStageTestSelectionResult evtStageTestSelectEntry(
    s32 encodedIndex, s32 initialMotionIndex,
    MdlAssetRequestMode assetRequestMode);

/* Select an entry using its own stored motion byte; retain the native all-bits-set literal type. */
void evtStageTestSelectEntryWithoutInitialValue(
    u16 entryIndex, MdlAssetRequestMode assetRequestMode);

/* Poll model requests: 0 for an empty queue, 1 while handling a request, 2 otherwise. */
EvtStageTestResourceWorkResult evtStageTestPollModelRequest(void);

void evtStageTestApplyInitialMotion(s32 initialMotionIndex);

extern u8 *D_003BAA20;

/* Store uniform model scale through vf10 and return it; useTable selects the model-record factor. */
f32 mnuSetModelScaleVector(dds3ModelHandle_t *model, s32 useTable);

/* Reset target XYZ to (0, 0, -400) and eye XYZ to zero; leave their fourth words untouched. */
void cmpResetStatusModelPos(void);

/* Apply the active entry's model position and scale-dependent view depth.
 * Retain the post-call entry rereads and subtraction-based scaling expressions. */
void mnuApplyModelCamera(dds3ModelHandle_t *model);

/* Convert the active entry's degree angles to radians and update the model rotation basis. */
void evtStageTestApplyEntryRotation(dds3ModelHandle_t *model);

/* vu0 routine: copies the stage-test camera vectors into the view work area, builds the look-at basis for eye 600 units along the view direction, and hands the matrix to the model packet at the current slot */
void evtStageTestUpdateCamera(void);

s8 evtStageTestUpdate(struct sdfGrOt_t **surfaces);

/* Count nonzero bytes in the active entry's motion column; any nonzero kind selects column 1. */
EvtStageMotionColumnCount evtStageTestCountFlags(s32 columnKind);

/* Index a motion column directly and queue a 15-frame blend.
 * Kinds 1/2 use column 1 with zero lead and clear/set fallback suppression;
 * all other kinds use column 0 with a 15-frame lead and retain that flag. */
void evtStageTestQueueMotion(EvtStageMotionKind motionKind, u32 columnIndex);

/* Queue a motion and its blend timing; inputs are truncated to whole frames.
 * The lead shifts initial motion time backwards, not to a start-frame endpoint. */
void evtStageTestQueueMotionSegment(s32 motionIndex, f32 blendLeadFrames, f32 blendDurationFrames);

/* Request the fallback path without waiting for the active motion's end-state byte. */
void evtStageTestForceDefaultMotion(void);

/* Idle and fallback-started states are settled; every other state is reported as pending. */
EvtStageMotionQueueStatus evtStageTestHasPendingMotion(void);

/* Start a queued motion, then the entry's fallback unless suppressed.
 * The native upper-bound checks do not reject negative motion indices.
 * Fallback is permitted by model-state byte 5 or an explicit force request. */
void evtStageTestAdvanceMotionQueue(void);

struct SoundHandleNode;

extern struct SoundHandleNode * sndCreateSystemEffectHandle(struct dds3ModelHandle_t *actor, s32 index);

/* Replace the attached model effect with the native fixed request; argument is unused. */
void evtStageTestCreateModelEffect(s32 unused);

/* Destroy a nonzero model-effect handle and clear it; zero is already released. */
void evtStageTestDestroyModelEffect(void);

/* Store a request value: the update path tests its signed field for nonnegative.
 * The model-effect creator itself ignores the forwarded value. */
void evtStageTestSetPendingEffect(u32 requestValue);

/* Report whether a model-effect handle is present. */
s32 evtStageTestHasModelEffect(void);

extern s32 D_003BC7BC;

extern const char *D_003BC7B8;

extern char D_003BC7C0[];

extern sdfGrOt_t sdfGrOtTable[];

extern f32 dds3CameraObjectBasicGetFovy(const dds3Basic_t *camera);

void evtStageTestDrawCameraTransformDebug(void);

u32 evtStageTestCameraUpdate(void);

/* Create the fixed-transform camera target and return its update callback. */
void *evtCreateBattleStageTestCamera(void);

extern sdfGrOt_t D_00325708;

extern s32 D_003BC7D0;

extern s32 D_003BC7D4;

/* Draw the battle-stage selector; confirmation creates the selected world object
 * and returns its camera callback before the four trigger-bit ID adjustments. */
s32 evtBattleStageTestScreen(dds3ProcessID_t *task);

void evtDestroyBattleStageTestWorldNode(dds3ProcessID_t *task);

/* Destroy the named battle-stage-test task hierarchy and disable the debug graph. */
void evtBattleStageTestStopTask(void);

/* Enable the debug graph and create the battle-stage selector task with its cleanup callback. */
void btlCreateStageTestTask(void);
/* End of declarations from src/dds1/game/code_00282850.c */

/* Clear the complete native popup-transition state, including saved entry addresses. */
void mnuClearPanelTransitionState(MenuPopupState *state) {
    memset(state, 0, MNU_POPUP_STATE_BYTES);
}

void mnuApplyPanelTransitionAction(s32 action, MenuPopupEntry *entry, MenuPopupState *state, u32 argument) {
    MenuPopupCallback callback;
    MenuPopupEntry *saved;
    s32 i;

    if (state == NULL) {
        return;
    }
    switch (action) {
        case 0:
            callback = entry->enter;
            if (callback != NULL) {
                if (entry->leave != NULL) {
                    for (i = 0; i < state->count; i++) {
                        if (entry->enter == state->entries[i]->enter) {
                            return;
                        }
                    }
                    state->entries[state->count++] = entry;
                    if ((entry->flags & MNU_POPUP_INSERT_BEFORE_TOP) && state->count >= 2) {
                        saved = state->entries[state->count - 1];
                        state->entries[state->count - 1] = state->entries[state->count - 2];
                        state->entries[state->count - 2] = saved;
                    }
                }
                callback(argument);
            }
            break;
        case 1:
        case 2:
            if (state->count != 0) {
                saved = state->entries[state->count - 1];
                if (saved->leave != NULL && action == 1) {
                    saved->leave(argument);
                }
                state->count--;
            }
            break;
    }
}

/* Pop saved entries with their leave callbacks until the state count reaches zero. */
void mnuDrainPanelTransitions(MenuPopupState *state, struct dds3ProcessID_t *task) {
    s32 entryCount;

    entryCount = state->count;
    while (entryCount != 0) {
        mnuApplyPanelTransitionAction(1, 0, state, (u32)task);
        entryCount = state->count;
    }
}

INCLUDE_ASM(const s32, "code_0034B898", func_0034BA60);

s32 mnuDispatchPopupTransition(MenuPopupState *work, s32 *entrySlot, s32 mode,
                               void *callback) {
    MenuPopupState *state = work;
    MenuPopupEntry **slot = (MenuPopupEntry **)entrySlot;
    MenuPopupEntry *entry;
    s32 accepted = 1;
    s32 result = 0;
    u32 flags;

    if (callback == NULL) {
        return 0;
    }
    if (slot == NULL) {
        return 0;
    }
    entry = *slot;
    if (entry == NULL) {
        return 0;
    }
    flags = entry->flags;
    if (flags & 0x400000) {
        return 0;
    }
    switch (mode) {
    case 0:
        entry->flags = flags | 0x100000;
        if (entry->canEnter != NULL) {
            accepted = ((MenuPopupCanEnterCallback)entry->canEnter)(callback) != 0;
        }
        if (accepted || ((*slot)->flags & 1)) {
            if (!((*slot)->flags & 0x10000)) {
                mnuApplyPanelTransitionAction(0, *slot, state, (u32)callback);
                (*slot)->flags |= 0x10000;
            }
        }
        if (accepted) {
            result = (s32)(*slot)->start;
            (*slot)->flags |= 0x200000;
        } else {
            flags = (*slot)->flags;
            if (flags & 1) {
                (*slot)->flags = flags | 0x400000;
                (*slot)->start(callback);
                (*slot)->flags &= ~0x400000;
            }
        }
        break;
    case 1:
        if (flags & 0x100000) {
            if (flags & 0x200000) {
                result = (s32)entry->update;
            } else if (flags & 1) {
                entry->flags = flags | 0x400000;
                entry->update(callback);
                (*slot)->flags &= ~0x400000;
            }
        }
        break;
    case 2:
        if (flags & 0x100000) {
            if (flags & 0x200000) {
                if (flags & 0x20000) {
                    if (flags & 0x40000) {
                        mnuApplyPanelTransitionAction(2, NULL, state, (u32)callback);
                    } else {
                        mnuApplyPanelTransitionAction(1, NULL, state, (u32)callback);
                    }
                }
                /* Keep only the low 16 flag bits, read as a halfword. */
                (*slot)->flags = *(u16 *)*slot;
                state->entryAddress = state->lastEntryAddress;
                state->lastEntryAddress = *slot;
                result = (s32)(*slot)->finish;
                *slot = NULL;
            } else if (flags & 1) {
                entry->flags = flags | 0x400000;
                entry->finish(callback);
                (*slot)->flags &= ~0x400000;
            }
        }
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "code_0034B898", func_0034BCE0);

/* Bind the entry address and retain only the entry's low sixteen flag bits. */
void mnuSetPopupEntry(s32 *entrySlot, MenuPopupEntry *entry) {
    u16 retainedFlags;

    retainedFlags = entry->flags;
    *entrySlot = (s32)entry;
    entry->flags = retainedFlags;
}

/* Bind the entry with the native insert-before-top bit after preserving low flags. */
void mnuSetPopupEntryFlagged(s32 *entrySlot, MenuPopupEntry *entry) {
    u16 retainedFlags;

    retainedFlags = entry->flags;
    *entrySlot = (s32)entry;
    entry->flags = retainedFlags | MNU_POPUP_INSERT_BEFORE_TOP;
}

INCLUDE_ASM(const s32, "code_0034B898", func_0034BD18);

/* Bind the current popup entry only when its saved address is nonzero. */
void mnuBindPresentMenuEntry(MenuPopupState *state, s32 *entrySlot) {
    if (state->entryAddress != NULL) {
        mnuSetPopupEntryFlagged(entrySlot, state->entryAddress);
        return;
    }
}

INCLUDE_ASM(const s32, "code_0034B898", mnuMapPadMaskToFlags);
