#include "common.h"

/* Declarations from dds-decomp src/dds2/game/code_002BE628.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "datCalc.h"

#include "kernel_draw.h"

#include "newdata.h"

#include "sdfDevCons.h"

#include "sdk_libc.h"

#include "dds/btl.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3_world_primary.h"

#include "dds/dds3obj.h"

#include "dds/dsp_entry_api.h"

#include "dds/eff_random.h"

#include "dds/evt_world.h"

#include "dds/evt_world_object_api.h"

#include "dds/fr_font.h"

#include "dds/kwln.h"

#include "dds/mdl.h"

#include "dds/mdl_asset_request.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_projection.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdk_libc.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/ui_color.h"

/* The primary object returned by dds3GetActiveScene is distinct from the active
 * world's WorldInfo payload. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * DDS1 and DDS2 use the same observed layout. Node payloads depend on the slot. */
enum {
    EVT_WORLD_SLOT_MOVIE = 3, /* viewer movie-time lists: byte offset 0x28 */
    EVT_WORLD_SLOT_CAMERA = 4, /* linked cameras; active nodes supply the view transform */
    EVT_WORLD_SLOT_UNIT = 5   /* unit/group/lip-sync lists: byte offset 0x40 */
};

/* Convert three Euler angle components to the VU0 quaternion output.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void FromEuler(f32 x, f32 y, f32 z);

/* Build a look-at basis from four-float target, eye, and up vectors. */
void LookAtMatrix(const f32 *targetPoint, const f32 *eyePosition, const f32 *upVector);

enum SdfMotionStateValue {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
};

/* Append the common rectangle used to frame a grid of sprite cells. */
void dds3ConsPanelTagLink(struct sdfDrawTag_t *packetList, s32 column, s32 row,
                        s32 columnCount, s32 rowCount);

struct sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *world);

/* Return the borrowed primary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetActiveScene(void);

/* Return the current frame draw-buffer slot (0 or 1). */
u32 dds3GetFrameID(void);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(EffRandState *state);

dds3TaskKillRequestResult_t dds3KillProcessName(const char *name, s32 delayTicks);

extern SdfProjectionRecord dds3ScreenParam;

void dds3SetScreenFovy(f32 value);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

/* Create and select the world object for an area/room pair; returns 1 after setup. */
s32 evtLoadArea(s32 area, s32 room);

/* Blends packed RGBA channels; weights above 0x100 are mirrored from 0x200. */
u32 fclBlendColor(u32 color0, u32 color1, u32 weight);

/* Start a display entry; return zero if no window is open, one if it starts. */
s32 fclStartMessage(s32 entry);

/* Link a glyph chain into the current draw-buffer slot. */
s32 frDestroyFontRequestAfter(struct _FRQ *glyph);

/* Update the first-option byte of every child on this glyph chain. */
void frSetFontColorNo(struct _FRQ *glyph, u8 firstOption);

/* Model-context lifetime, per-frame update, and broadcast accessors. */
/* Destroy one context. If it leaves its resource-owning group empty, group
 * teardown also releases that group's registered assets. */
void mdlFree(dds3ModelHandle_t *ctx);

/* The referenced halfword is promoted to a word-sized SDK result. */
s32 mdlGetNumOfCut(const dds3ModelHandle_t *ctx, s32 searchId);

void mdlLink(dds3ModelHandle_t *ctx, struct sdfGrOt_t **surfaces);

void mdlLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

/* A nonblocking request returns QUEUED when it starts work and
 * ALREADY_PENDING when work for the key already exists. Otherwise the result
 * is the ready dds3ModelResrc_t address. */
MdlAssetRequestResult mdlPrepare(
    s32 group, s32 id, MdlAssetRequestMode requestMode);

void mdlSetPosition(dds3ModelHandle_t *ctx);

void mdlSetRotate(dds3ModelHandle_t *ctx);

void mdlSetScale(dds3ModelHandle_t *ctx);

void mdlSmoothLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                          f32 blendDurationFrames);

void mdlSmoothSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                       f32 blendDurationFrames);

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

/* Apply volume and pan to a sequence track through the command queue. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

void qsort(void *base, u32 count, u32 size, s32 (*compare)(const void *, const void *));

extern void sdf3DMakeScreenParam(SdfCamera *);

extern void sdf3DMakeViewportPacket(struct ConsMatrixPacket *, SdfProjectionRecord *, const Matrix4 *);

extern void sdf3DSetCamera(const SdfProjectionRecord *, const Matrix4 *);

void *sdfChipAllocDirect(s32 size);

void sdfChipFreeDirect(void *memory);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

#include "dds/sdk_ps2.h"

#include "dds/dsp_entry_api.h"

#include "dds/evt_stage_test_api.h"

#include "dds/evt_stage_test_resource_work_result.h"

#include "dds/mnu_input.h"

#include "dds/sdf_packet_list.h"

#include "dds/eff_resource_slots.h"

#include "dds/eff_resource_records.h"

#include "dds/fpu.h"

#include "dds/common.h"

#include "dds/dat_assigned_item_id.h"

#include "dds/pty_record_slot_value_api.h"

#include "dds/sdf_vu0.h"

#include "dds/pty_bullet_item_count_api.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/eff_random.h"

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

#include "dds/dat_command.h"

#include "dds/dds3obj.h"

#include "dds/evt_world.h"

#include "dds/sdf.h"

#include "dds/kwln.h"

#include "dds/dds3_task_create.h"

#include "dds/sdf_pac_state.h"

#include "dds/sdf_projection.h"

#include "dds/sdf_sif_command.h"

#include "dds/pcp_vu0.h"

#include "dds/mnu.h"

#include "dds/mnu_party_row_draw_state.h"

#include "dds/mnu_ability_target_category.h"

#include "dds/mnu_entry_cost_api.h"

#include "dds/mnu_entry_use_status_api.h"

#include "dds/mnu_popup_eligibility.h"

#include "dds/mnu_party_entry_lookup.h"

#include "dds/mnu_panel_item.h"

#include "dds/mnu_shop.h"

#include "dds/mnu_list.h"

#include "dds/mnu_panel_state.h"

#include "dds/mnu_panel_state_prefix.h"

#include "dds/mnu_staff.h"

#include "dds/mdl.h"

#include "dds/mdl_asset_request.h"

#include "dds/eff.h"

#include "dds/dat_state.h"

#include "dds/prf_current_record_view.h"

#include "dds/pty_skill_nibble_state.h"

#include "dds/pty_roster_api.h"

#include "dds/pty_party_unit_presence_api.h"

#include "dds/evt_stage_test.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/btl.h"

#include "dds/btl_party_command_use_result.h"

#include "dds/dds3_world_primary.h"

#include "dds/fld_scene_command_state_api.h"

#include "dds/itf_draw_grid.h"

#include "dds/prf_profile_cap.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/fr_font_draw_glyph.h"

extern s32 datAddMp(dds_datUnitWork_t *unit, s32 delta);

extern void fclSetMessageVar(s32 slotIndex, const char *sourceText);

extern void evtFreeStage(void);

extern void fldActivatePendingSceneCommand(void);

extern s32 itfGridLookupValueOrDefault(EffectSlotSet *resource, s32 resourceIndex);

extern void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *resource, s32 resourceIndex, s32 x, s32 y, s32 width, s32 height);

extern void dds3DebugGraphSetEnabled(s8 enabled);

extern s32 sdfApplyCommandResults(s32 channel, dds_datUnitWork_t *sourceRecord, dds_datUnitWork_t *item);

extern void sndDestroyFileQueueWrapper(struct SoundHandleNode *node);

extern void mnuSetGroupPair(struct MenuPanelItem *, u32, u32);

extern void itfGridStorePosition(MenuGridSlot *, EffectSlotSet *, s32);

extern void effDrawTextureSlotWithBlendedCornerColors(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);

#define MNU_PANEL_ITEM_COUNT 5

#define MNU_PANEL_STATE_BYTES 0x8C

#define MNU_PANEL_GROUP_BYTES 0x2C

#define MNU_SPRITE_STATE_BYTES 0x20

#define MNU_SIMPLE_SPRITE_BYTES 0x20

#define MNU_PANEL_ITEM_BYTES 0xAC

#define MNU_PROFILE_PANEL_BYTES 0x48

#define MNU_TRANSITION_LIMIT 0x100

#define MNU_TRANSITION_STEP 8

#define MNU_GRADIENT_FADE_STEP 0x20

#define MNU_PROFILE_PHASE_PERIOD 0x200

#define MNU_PROFILE_PHASE_STEP 1

#define MNU_PANEL_TEXTURE_COUNT 7

#define MNU_POPUP_STATE_BYTES 0x4c

#define MNU_POPUP_ENTRY_MARK_BITS 0x60000

#define MNU_PARTY_SLOT_COUNT 5

#define MNU_INPUT_PRIORITY_BIT 1

#define MNU_PAD_TRIGGER_BIT 2

#define MNU_COMMAND_RECORD_BYTES 0x38

#define MNU_COMMAND_ID_MASK 0xFFFF

#define MNU_PERCENT_SCALE 100

#define MNU_COMMAND_USE_STATUS_BOUNDARY 0x220

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

extern s32 D_00437C9C;

extern void mnuInitializeQueuedCommandEffect(MenuQueuedCommand *entry);

extern void mnuInitRandomLineEffect(MenuQueuedCommand *entry);

extern s32 mnuLookupRangeEntry(u16);

extern void ptyRecomputeMaxHpMp(dds_datUnitWork_t *);

extern EvtStageTestResourceWorkResult evtStageTestAdvanceResourceWork(void);

extern s8 D_003E7970[];

extern u8 D_003E7978[];

extern void evtStageTestApplyInitialMotion(s32 initialMotionIndex);

extern void evtStageTestQueueMotionSegment(u32, f32, f32);

extern void evtStageTestCreateModelEffect(s32);

extern void evtStageTestUpdateCamera(void);

extern void btlUpdateJobPositionFromModel(struct SoundHandleNode *node);

extern void evtStageTestStop(void);

extern f32 evtStageTestViewEyeVector[4];

extern f32 evtStageTestViewTargetVector[4];

extern void mnuInitializePanelItemGridSlots(MenuPanelItem *, EffectSlotSet *, s32);

extern s8 mnuSkillCategoryRangeMapRows[];

extern u16 mnuSkillCategoryRangeBounds[];

extern u16 mnuSkillCategoryRangeUpperBounds[];

extern s8 mnuSkillCategoryRangeBoundWordIndices[];

extern void evtStageTestAdvanceMotionQueue(void);

extern dds3Basic_t *evtAddCamera(f32 *pos, f32 *rot);

extern char D_00437CB0[];

extern void mnuFreePanelItemWork(MenuPanelItem *);

extern u16 D_003E78D8[];

extern StageTestState evtStageTestState;

extern char D_0042B610[];

extern void itfGridSetQuantizedBounds(EffectSlotSet *, s32, s32, s32, s32, s32);

extern u32 effDestroyResourceSlotSet(EffectSlotSet *);

extern s32 D_00437CB8;

extern s32 D_00437CBC;

extern sdfGrOt_t D_00380708;

extern s8 dds3PadButtonState[];

extern MnuPartyRowDrawState mnuGetPanelWindowTransitionState(MenuPageWindow *window, s32 mode);

extern void mnuBlendPanelSlots(EffectSlotSet *dst, EffectSlotSet *src, s32 amount);

/* Draw the window surface; while the window is in selection state 1, blend its second resource in. */
void mnuDrawPanelWindowSurface(s32 x, s32 y, s32 z, MenuPageWindow *window, s32 mode, EffectSlotSet **resources, u32 alpha, s32 surface);

typedef struct MenuList MenuList;

/* Give both complete commands in each page the same draw resource. */
void mnuSetPanelSlotValues(MenuPageWindow *menu, struct EffectSlotSet *resourceSet);

extern s32 D_0042B048[];

extern s32 D_0042B098[];

void mnuInitializeQueuedCommandEffect(MenuQueuedCommand *entry);

/* Initialize both particle arrays for a queued menu command. */
void mnuInitRandomLineEffect(MenuQueuedCommand *entry);

void mnuInitializeQueuedMenuCommand(MenuQueuedCommand *entry);

/* Queue in the least-valued command bank; use bank zero if neither value
 * is below 0x200. Retail has no explicit return: its last call is a plain jal. */
void mnuQueueListEntry(MenuPageWindow *menu, s32 window, u32 kind, s32 argument);

void mnuClearSpriteRecord(MenuQueuedCommand *entry);

/* Clear only the two command headers, preserving resources and particle arrays. */
void mnuClearPairedSpriteRecords(MenuPageWindow *menu, s32 index);

typedef struct MenuQueuedRenderContext {
    s32 panelIndex;
    MenuPageWindow *menu;
    MenuPoint origin;
    MenuPoint extent;
} MenuQueuedRenderContext;

typedef char MenuQueuedRenderContext_size_check[
    sizeof(MenuQueuedRenderContext) == 0x18 ? 1 : -1];

extern void uiDrawSurfaceAtNearDepth(u32);

extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

void func_002BF000(s32 x, s32 y, s32 z, MenuQueuedCommand *command,
                   MenuQueuedRenderContext *context, s32 surface);

/* Draw queued random lines and their cropped panel. */
void mnuDrawQueuedCommandLineEffect(s32 x, s32 y, s32 z, MenuQueuedCommand *command,
                   MenuQueuedRenderContext *context, s32 surface);

typedef struct MenuCommandScatterLayout {
    MenuPoint points[7];
} MenuCommandScatterLayout;

extern const MenuCommandScatterLayout D_0042B0D0;

typedef char MenuCommandScatterLayout_size_check[
    sizeof(MenuCommandScatterLayout) == 0x38 ? 1 : -1];

void mnuDrawQueuedCommandScatterEffect(s32 unusedX, s32 unusedY, s32 unusedDepth,
                  MenuQueuedCommand *command, s32 surface);

extern void sdfSubmitGsAlphaOneRegisterPacket(s32, s32);

extern void func_002BF000(s32, s32, s32, MenuQueuedCommand *, MenuQueuedRenderContext *, s32);

extern void mnuDrawQueuedCommandLineEffect(s32, s32, s32, MenuQueuedCommand *, MenuQueuedRenderContext *, s32);

extern void mnuDrawPartyStatusValuePanel(s32, s32, s32, MenuPageWindow *, s32, s32, s32, s32);

/* Advance and draw a page's two queued commands. An active command first waits
 * out its delay, then draws by kind and fades its phase toward zero; it is
 * cleared once the phase is spent while the first command is idle. The loop
 * indexes `commands[i]` (each block forms its own address, so GCSE does not
 * forward the first phase load; loop.c later reduces the addresses and
 * reverses the counter). */
void mnuUpdateAndDrawPartyCommandQueue(s32 x, s32 y, s32 z, MenuQueuedCommand *commands,
                  MenuQueuedRenderContext *context, s32 surface);

extern const s32 D_0042B108[];

extern const s32 D_0042B118[];

extern const s32 D_00437C60[];

extern char D_00437C68[];

extern void mnuDrawMenuPageSpriteLayers(s32, s32, s32, MenuPageSlot *, s32, s32, s32);

extern void mnuDrawPartyCommandPage(s32, s32, s32, s32, MenuSprites *, s32);

extern void mnuDrawPartyProfileAndExpOverlay(s32, s32, s32, MenuPageSlot *, s32, u32, s32);

extern s32 mnuClearWindowPendingFlagAfterSelection();

extern void uiDrawActiveSurfaceRegion(s32);

extern void sdfDispatchSurfaceWithPreparedTexturePacket(s32);

extern struct _FRQ *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, struct _FRQ *);

u32 mnuGetPanelRatioColor(s32, s32, s32);

void mnuDrawAndAdvanceRatioPanel(s32, s32, s32, u32, s32, s32, MenuPageBar *, u32);

void mnuDrawPartyMemberCommandPanel(s32 x, s32 y, s32 z, MenuPageWindow *menu,
                                    s32 panelIndex, s32 surface);

extern const s32 D_0042B108[4];

extern const s32 D_0042B130[4];

extern const s32 D_0042B140[4];

extern const s32 D_00437C70[];

extern char D_00437C68[];

extern void mnuUpdateAndDrawPartyCommandQueue(s32, s32, s32, MenuQueuedCommand *, MenuQueuedRenderContext *, s32);

/* Both native page renderers also forward the surface word; the
 * selection helper consumes only its first five arguments. */
extern s32 mnuClearWindowPendingFlagAfterSelection();

typedef struct _FRQ _FRQ;

extern _FRQ *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, _FRQ *);

extern s32 frFontDrawGlyphChain(_FRQ *, s8, u32);

/* Draw one party page: queued commands, level text, selection and HP/MP bars. */
void mnuDrawPartyMemberStatusPanel(s32 x, s32 y, s32 z, MenuPageWindow *menu,
                  s32 panelIndex, s32 surface);

extern void mnuDrawPartyMemberCommandPanel(s32, s32, s32, MenuPageWindow *, s32, s32);

extern void mnuDrawPartyMemberStatusPanel(s32, s32, s32, MenuPageWindow *, s32, s32);

void mnuDispatchListPanel(s32 x, s32 y, s32 z, MenuPageWindow *menu, s32 panelIndex, s32 surface);

typedef struct MenuSpacing {
    s32 step;
    s32 gap;
    s32 tail;
} MenuSpacing;

extern const s32 D_0042B130[4];

extern const s32 D_0042B140[4];

void mnuCalcListEntryOffset(s32 *out, MenuPageWindow *menu, s32 panelIndex);

/* Increment while below the transition limit; the native add is not clamped. */
void mnuAdvancePanelTransition(MenuPageWindow *menu);

/* Apply a temporary override to the selected node while drawing its panel. */
void mnuDrawPanelWithTemporaryOverride(s32 x, s32 y, s32 z, s32 overrideValue, MenuPageWindow *menu, s32 surface);

/* Draw the selected panel or all visible/additional panels, then advance the transition. */
void mnuDrawListPanels(s32 x, s32 y, s32 z, s32 overrideValue, MenuPageWindow *menu, s32 surface);

void mnuDrawPanelListDefault(s32 x, s32 y, s32 depth, MenuPageWindow *window, s32 surface);

typedef struct MenuPanelState {
    MenuPanelStatePrefix common;
    MenuGridSlot backgroundSlot; /* 0x20 */
    s32 corners[5][2]; /* 0x28 */
    MenuGridSlot selectedCenterSlot; /* 0x50 */
    MenuGridSlot selectedCornerSlot; /* 0x58 */
    MenuGridSlot rowSlot; /* 0x60 */
    s32 secondOffsetX; /* 0x68: row position relative to the panel. */
    s32 secondOffsetY; /* 0x6C */
    MenuGridSlot headingSlot; /* 0x70 */
    s32 thirdOffsetX; /* 0x78: heading position relative to the panel. */
    s32 thirdOffsetY; /* 0x7C */
    const s32 *headingIndices; /* 0x80: caller supplies a static sprite-index table. */
    u8 pad84[4];
    struct MenuIconState *resourceHandle; /* 0x88 */
} MenuPanelState;

typedef char MenuPanelStateSizeCheck[(sizeof(MenuPanelState) == MNU_PANEL_STATE_BYTES) ? 1 : -1];

/* Allocate a zeroed native panel state with the requested dimensions. */
MenuPanelState *mnuCreatePanelState(s32 width, s32 height);

void mnuDestroyPanelState(MenuPanelState *panel);

void mnuSetPanelFirstGridSlot(MenuPanelState *panel, u32 valueA, u32 valueB, EffectSlotSet *resource,
                                    s32 resourceIndex);

void mnuSetPanelCornerGeometry(MenuPanelState *panel, s32 x, s32 y, EffectSlotSet *resource, s32 centerIndex, s32 cornerIndex);

void mnuInitializePanelResource(MenuPanelState *panel, s32 resource, s32 material);

void mnuSetPanelSecondGridSlot(MenuPanelState *panel, s32 offsetX, s32 offsetY, EffectSlotSet *resource,
                                    s32 resourceIndex);

void mnuSetPanelThirdGridSlot(MenuPanelState *panel, s32 offsetX, s32 offsetY, EffectSlotSet *resource,
                                    const s32 *indices);

void mnuSetPanelState(MenuPanelState *panel,
                      MenuPanelSelectionIndex selectionIndex);

extern void mnuDrawIconState(u32, u32, u32, u32, struct MenuIconState *, u32, u32);

extern void mnuUpdateWindowPanelHandleStatesKindFourFive(struct MenuIconState *);

void mnuDrawPanelState(s32 x, s32 y, s32 depth, MenuPanelState *panel, s32 drawArg);

extern MenuPanelItem *mnuCreatePanelItem(void);

extern void mnuInitializePanelGroupGridSlots(MenuPanelItem *, EffectSlotSet *,
                                             EffectSlotSet *, EffectSlotSet *, s32);

extern void mnuClearPanelGroupSelection(MenuPanelGroup *);

extern void mnuSetPanelItemSelection(MenuPanelItem *, s32);

extern void mnuSetPanelItemOption(MenuPanelItem *, u32);

extern void mnuStorePanelItemValue(MenuPanelItem *, u32);

/* Create the five panel items owned by this group and clear its selection. */
MenuPanelGroup *mnuCreatePanelGroup(EffectSlotSet *owner, EffectSlotSet *texture,
                                    EffectSlotSet *extraGrid);

/* Release every owned panel item before releasing the group allocation. */
void mnuDestroyPanelGroup(MenuPanelGroup *group);

/* Configure all five panel items against the same grid object. */
void mnuUpdateFiveListEntries(MenuPanelGroup *group, EffectSlotSet *gridObject);

void mnuSetPanelGroupSelection(MenuPanelGroup *group, MenuPanelGroupSelection selection);

/* Store the no-selection sentinel. */
void mnuClearPanelGroupSelection(MenuPanelGroup *group);

MenuPanelGroupSelection mnuGetPanelGroupSelection(MenuPanelGroup *group);

extern void mnuDrawAndAdvancePanelItem(s32, s32, s32, s32, u32, MenuPanelItem *, u32);

/* Draw the five party stats, including the sequel's candidate-value colors. */
void mnuDrawAndAdvancePanelGroup(s32 x, s32 y, s32 depth, dds_datUnitWork_t *partyRecord,
                                 MenuPanelGroup *group, s32 mode, s32 drawFlags);

void mnuSetGroupSelection(MenuPanelGroup *group, s32 index, s32 selection, u32 option);

void mnuSetIndexedPanelGroupValue(MenuPanelGroup *group, s32 index, u32 value);

/* Apply the selected item's five stat values to the panel entries. */
void mnuApplyPackedGroupValues(MenuPanelGroup *group, s32 itemId);

/* Create the sprite-resource state with its native initial value. */
MenuSpriteState *mnuCreateSpriteState(struct EffectSlotSet *resourceSet0,
                                      struct EffectSlotSet *resourceSet1,
                                      struct EffectSlotSet *resourceSet2);

void mnuFreeSpriteStateWork(MenuSpriteState *spriteState);

/* The sequel selects its draw variant from the range index plus eight. */
void mnuDrawRangeSpriteVariant(u32 x, u32 y, u32 depth, u32 color,
                                    u16 rangeId, EffectSlotSet *drawArg, u32 texture);

/* Create the DDS2 simple sprite state from its three resource sets. */
MenuSpriteState *mnuAllocateSimpleSprite(struct EffectSlotSet *resourceSet0,
                                       struct EffectSlotSet *resourceSet1,
                                       struct EffectSlotSet *resourceSet2);

void mnuFreeSimpleSpriteWork(MenuSpriteState *sprite);

void mnuResetGradientFadeColor(MenuGradientFade *state, s32 color);

void mnuSetStateValue(u32 *out, u32 value);

/* Draw two clear corners and two blended corners, then step the bounded blend value. */
void mnuDrawAndStepGradientFade(MenuGradientFade *state, s32 surface);

void mnuDrawRepeatedPanelSprites(s32 x, s32 y, s32 depth, s32 fade, s32 count, EffectSlotSet *drawArg, s32 variant, s32 texture);

/* Set both effect positions; only the first Y comes from the active menu entry. */
void mnuSetPairedEffectPositions(MenuPageBar *pair);

/* Divide a quantized horizontal span into the two effect-grid regions. */
void mnuUpdatePairedEffectBounds(MenuPageBar *pair);

/* Cycle through four indexed settings while refreshing the paired effects. */
void mnuCyclePairedEffectSetting(MenuPageBar *pair);

void mnuDrawAndSwapPageBarEffect(s32 x, s32 y, s32 depth, u32 fade, MenuPageBar *bar, u32 flags);

void mnuCreatePairedEffects(MenuPageBar *pair);

/* The public word-pointer boundary refers to the same complete panel owner. */
void mnuReleasePairedEffectBatches(s32 *objectWords);

extern void mnuInitializePageBar(MenuPageBar *, s32, s32 *, s32, s32,
                        EffectSlotSet *, const s32 *, s32);

void mnuInitializePageBar(MenuPageBar *pair, s32 variant, s32 *settings, s32 positionY, s32 quantizedSpan,
                   EffectSlotSet *source, const s32 *table, s32 count);

void mnuDrawPanelSequenceByRow(MenuPageBar *pair, s32 variant, s32 *settings,
                             s32 positionY, s32 quantizedSpan, EffectSlotSet *source);

/* Release the seven sprite texture handles, then the paired effect batches. */
void mnuReleaseSpriteTextures(u32 *objectWords);

/* Select the packed ratio color; a zero divisor retains the default color. */
u32 mnuGetPanelRatioColor(s32 useDefault, s32 amount, s32 divisor);

extern char D_00437C88[];

extern void mnuDrawRatioPanelContents(s32, s32, s32, u32, u32, s32, s32, MenuPageBar *, u32);

extern void mnuDrawAndSwapPageBarEffect(s32, s32, s32, u32, MenuPageBar *, u32);

void mnuDrawAndAdvanceRatioPanel(s32 x, s32 y, s32 depth, u32 color, s32 value,
                  s32 limit, MenuPageBar *pair, u32 flags);

/* Allocate a zeroed native panel item and initialize its three default values. */
MenuPanelItem *mnuCreatePanelItem(void);

/* Bind the panel item's nine sprite cells to their grid entries (the extra pair only when an extra grid
 * exists) and pick the panel's label entry. */
void mnuInitializePanelGroupGridSlots(MenuPanelItem *item, EffectSlotSet *primaryGrid,
                                      EffectSlotSet *secondaryGrid,
                                      EffectSlotSet *extraGrid, s32 panelIndex);

/* Bind five grid object/index references and initialize their quantized bounds.
 * The x/y members in this path hold object addresses and entry indices, not coordinates. */
void mnuInitializePanelItemGridSlots(MenuPanelItem *item, EffectSlotSet *gridObject,
                                     s32 panelIndex);

void mnuSetPanelItemPixelSpan(MenuPanelItem *item, u32 value);

void mnuSetGroupPair(MenuPanelItem *entry, u32 left, u32 right);

void mnuStorePanelItemValue(MenuPanelItem *item, u32 value);

void mnuSetPanelItemAuxiliaryValue(MenuPanelItem *item, u32 value);

void mnuSetPanelItemSelection(MenuPanelItem *item, s32 selection);

void mnuSetPanelItemOption(MenuPanelItem *item, u32 option);

void mnuFreePanelItemWork(MenuPanelItem *item);

extern void mnuDrawPanelItemDetail(s32, s32, s32, s32, s32, MenuPanelItem *, u32);

void mnuDrawAndAdvancePanelItem(s32 x, s32 y, s32 depth, s32 mode, u32 textMode,
                  MenuPanelItem *item, u32 flags);

void mnuSetProfilePanelValues(MenuProfilePanel *panel, s32 capValue, s32 progressValue);

extern u32 scrGetSelectedScriptEntryId(dds_datUnitWork_t *);

/* Create a profile panel and initialize its five native random words. */
MenuProfilePanel *mnuCreateProfilePanel(dds_datUnitWork_t *selectionState);

void mnuFreeProfilePanelWork(MenuProfilePanel *panel);

void mnuSetGroupProperties(MenuProfilePanel *panel, EffectSlotSet *panelSet,
                           EffectSlotSet *particleSet, s32 trailIndex, s32 particleIndex);

extern void mnuDrawProfilePanel(s32, s32, s32, MenuProfilePanel *, s32);

/* Draw first, then advance the native phase by one with a single period subtraction. */
void mnuDrawAndAdvanceProfilePanel(s32 x, s32 y, s32 z, MenuProfilePanel *panel, s32 option);

/* Clear the complete native popup-transition state, including saved entry addresses. */
void mnuClearPanelTransitionState(MenuPopupState *state);

void mnuApplyPanelTransitionAction(s32 action, MenuPopupEntry *entry, MenuPopupState *state, u32 argument);

/* Pop saved entries with their leave callbacks until the state count reaches zero. */
void mnuDrainPanelTransitions(MenuPopupState *state, struct dds3ProcessID_t *task);

s32 mnuHasPopupSelectionFlag(s32 *flags);

s32 jmsSeqChange(MenuPopupState *work, s32 *entrySlot, s32 mode,
                               void *callback);

/* Compare the popup state's current entry address, not an entry payload value. */
u8 mnuIsPopupEntryValue(s32 stateAddress, s32 entryAddress);

/* Bind the entry address and retain only the entry's low sixteen flag bits. */
void jmsSeqJump(s32 *entrySlot, MenuPopupEntry *entry);

/* Bind the entry with the native insert-before-top bit after preserving low flags. */
void jmsSeqJumpAndFree(s32 *entrySlot, MenuPopupEntry *entry);

/* Bind the entry with both native marking bits after preserving low flags. */
void mnuAttachAndMarkMenuEntry(s32 entrySlotAddress, s32 entryAddress);

/* Bind the current popup entry only when its saved address is nonzero. */
void jmsSeqReturn(MenuPopupState *state, s32 *entrySlot);

void mnuPopulatePartyPanelSlot(dds_datUnitWork_t *partyRecord, s32 unused, u32 index, PartyPanel *panel);

/* Populate occupied party slots; empty slots retain a unit index of -1. */
void mnuInitPartyPanelSlots(PartyPanel *panel);

extern u8 D_0037F530[];

/* Translate requested inputs using native sign-bit/trigger-bit and nonzero tests.
 * Input bit zero has exclusive priority over every other result bit. */
MnuInputActionFlags mnuMapPadMaskToFlags(MnuPadButtonQueryMask requestedButtons);

void mnuHandleListPageJumpInput(const void *resource, MenuWindowContainer *window,
                                MnuInputActionFlags *actionFlags);

void mnuHandlePanelListPageJumpInput(MenuWindowContainer *window,
                                     MnuInputActionFlags *actionFlags);

/* Bits 0x8000 and 0x4000 return early in that order; state bits may suppress navigation SE. */
void mnuPlayInputSound(s32 unused, s32 inputFlags, u32 *stateFlags);

/* Only bit 0x8000 returns early; other flags may play multiple sounds of the selected kind. */
void mnuPlayInputSoundKind(s32 inputFlags, s8 soundKind);

/* Dispatch the input flags with the native default sound kind. */
void mnuPlayDefaultInputSounds(u32 inputFlags);

/* Find the first occupied slot with the same table ID; zero also serves as no-match. */
s32 mnuFindMatchingPartyEntryIndex(const dds_datUnitWork_t *borrowedEntry);

/* Match a half-open range, then return its signed mapping value; unmatched IDs return zero. */
s32 mnuLookupRangeEntry(u16 rangeId);

/* Read a bound word from the primary table or its one-word-shifted alternate view. */
u16 mnuPickPairedTableValue(s32 boundWordIndex, s32 alternate);

/* For nonnegative indices, select among nonzero signed mapping values in row order. */
s32 mnuGetIndexedNonzeroEffect(s32 valueIndex);

/* Accumulate selected mapping values, then read the requested adjacent range bound. */
u16 mnuLookupPartyTableValue(u32 valueCount, s32 baseIndex, s32 alternate);

/* Return the attribute mask only for kind 2, which selects flag-mask semantics. */
u16 mnuGetCommandFlagMask(s32 commandId);

/* Return the native value/cost kind from the low-sixteen-bit command ID. */
u8 mnuGetRangeEntryKind(u32 commandId);

/* HP-kind values use max HP as a percentage basis; other kinds retain the stored value. */
u16 mnuGetAdjustedEntryValue(s32 commandId, dds_datUnitWork_t *actor);

s32 mnuGetRangeEntryFlatValue(s32 id);

/* Compare the stored raw HP/MP cost; equality is affordable and other kinds pass. */
MnuEntryCostAffordability mnuCanAffordEntryCost(
    u16 commandId, const dds_datUnitWork_t *actorEntry);

/* Return the menu's three-way unavailable-marker status. */
MnuEntryUseStatus mnuGetEntryUseStatus(dds_datUnitWork_t *actorEntry, u16 commandId);

/* Report insufficient raw HP/MP cost; equality and unhandled kinds return zero. */
s32 mnuIsEntryCostUnaffordable(u16 commandId, dds_datUnitWork_t *actorEntry);

/* Deduct an affordable stored HP/MP cost; unhandled kinds succeed without a deduction. */
s32 mnuConsumeEntryCost(s32 commandId, dds_datUnitWork_t *actorEntry);

/* Map target_type to the menu's single-entry or party-group selection category. */
MnuAbilityTargetCategory mnuGetAbilityTargetCategory(u16 commandId);

void datDelDevil(u16 ability, dds_datUnitWork_t *target, dds_datUnitWork_t *entry);

u32 ptyCheckFieldUseOverride(MenuPageWindow *context, s32 ability, dds_datUnitWork_t *target, dds_datUnitWork_t *entry);

s32 ptySkillApplyFieldUseEffect(MenuPageWindow *context, u16 ability, dds_datUnitWork_t *target, dds_datUnitWork_t *selectedEntry);

/* Test for the exact signed-byte marker one, not merely a nonzero selector byte. */
u8 mnuIsAbilityValueMarked(u32 commandId);

s32 ptyGetAffinityKind(s32 affinityId, s32 index);

s32 ptyGetAffinityFlagsWithoutOverride(s32 affinityId, s32 index);

u8 mnuIsBulletItemId(s32 id);

u8 mnuIsSolarPricedItemId(s32 id);

u8 mnuIsHighRangeItemId(s32 id);

extern u16 mnuGetPartyEntryMenuValue(dds_datUnitWork_t *);

/* Count inventory plus one for every matching slot value, without an occupancy test.
 * This counter still rejects 0xBF, although DDS2's bullet-ID predicate accepts it. */
PtyBulletItemCount ptyCountBulletItem(PtyBulletItemId bulletItemId);

u32 mnuSetPartyEntryMenuValue(dds_datUnitWork_t *entry, u32 menuValue);

u16 mnuGetPartyEntryMenuValue(dds_datUnitWork_t *entry);

void mnuMarkEntryBlocked(s32 itemId);

u32 mnuSetPartyEntryCurrentId(dds_datUnitWork_t *entry, u32 id);

DatAssignedItemId mnuGetPartyEntryCurrentId(dds_datUnitWork_t *entry);

dds_datUnitWork_t *mnuFindPartySlotByCurrentId(u32 id);

dds_datUnitWork_t *mnuFindReserveSlotByCurrentId(u32 id);

void mnuMarkEntryBlocked(s32 itemId);

void mnuClearEntryBlocked(s32 itemId);

s32 mnuIsEntryBlocked(s32 itemId);

s32 mnuHasOwnedUnblockedItem(void);

/* Match an item ID, select its mode-dependent SE ID, and start it; id changes roles. */
s32 sndPlayPartyItemSe(u32 id, s32 soundMode);

extern u32 datComputeSkillBoostedMaxHp(dds_datUnitWork_t *);

extern u32 datComputeSkillBoostedMaxMp(dds_datUnitWork_t *);

s32 btlItemApplyPermanentBonus(u16 itemId, dds_datUnitWork_t *unit);

s32 ptyChooseFirstAvailableRosterId(void);

extern s32 fldEsutoMaChk(void);

extern void fldEsutoMaStart(void);

extern s32 fldRiberaMaChk(void);

extern void fldRiberaMaStart(void);

extern s32 fldRifutoMaChk(void);

extern void fldRifutoMaStart(void);

extern u8 *datUnitNames;

s32 func_002C5A28(MenuPageWindow *page, u16 ability, struct dds_datUnitWork_t *target,
                  struct dds_datUnitWork_t *selected);

/* Return the first matching status-icon index in native priority order, or none. */
MnuPartyStatusIconIndex mnuGetPartyStatusIconIndex(const dds_datUnitWork_t *actorEntry);

/* Return one bit for the first occupied matching table ID, or zero when absent. */
s32 mnuGetMatchingPartyEntryMask(const dds_datUnitWork_t *borrowedEntry);

/* Classify the truncated signed percentage into default, below-half, and
 * below-quarter bands. A zero divisor returns the default class. */
MnuQuarterHalfPercentClass mnuClassifyQuarterHalfPercent(s32 amount, s32 divisor);

dds_datUnitWork_t *mnuPickBestPartyEntry(u32 *table);

dds_datUnitWork_t *mnuFindPartyEntryBySelection(s32 *out);

s32 mnuTryUseFieldSkill(PartyPanel *partyPanel, MenuPageWindow *page, dds_datUnitWork_t *target, s32 commit);

/* Give flag-bit-1 entries precedence, then compare their 10-bit fixed-point
 * HP/max-HP ratios without converting to floating point. */
s32 mnuComparePartyEntryCostRatio(u32 *left, u32 *right);

extern const u16 D_0042B518[];

s32 mnuTryUseBestPartyHealingSkill(PartyPanel *partyPanel, MenuPageWindow *page,
                                   dds_datUnitWork_t *caster, s32 commit);

s32 mnuUseFieldSkillOnParty(PartyPanel *partyPanel, MenuPageWindow *page, s32 commit);

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

/* Return the motion byte; this twin receives an already-narrowed entry index. */
u8 evtStageTestGetEntryMotionIndex(u16 entryIndex);

/* Return the indexed entry's animation frame. */
f32 evtStageTestGetEntryFrame(u16 entryIndex);

/* Copy exactly three position components; any output W component is untouched. */
void evtStageTestCopyEntryPosition(s32 encodedIndex, f32 *position);

/* Copy exactly three stored rotation components. */
void evtStageTestCopyEntryRotation(s32 encodedIndex, f32 *rotation);

extern f32 evtStageTestViewUpVector[4];

extern f32 aViewEye[4];

extern f32 aViewTarget[4];

extern f32 aViewDown[4];

extern u8 sdfViewShakeVector[];

extern Matrix4 sdfViewMatrix;

extern u8 D_003820F0[];

extern u8 D_0042B528[];

extern u8 D_0042B538[];

extern u8 D_0042B548[];

extern u8 D_0042B558[];

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

void evtStageTestInit(s32 mode);

void evtStageTestDestroyModelEffect(void);

/* Release the viewer's model/effect and discard its pending effect, except in mode 1. */
void evtStageTestStop(void);

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

void evtStageTestApplyInitialMotion(s32 initialMotionIndex);

extern u8 *D_00435DF0;

/* Store uniform model scale through vf10 and return it; useTable selects the model-record factor. */
f32 mnuSetModelScaleVector(dds3ModelHandle_t *model, s32 useTable);

typedef struct MenuWorkCamera {
    s32 x;
    s32 y;
    f32 depth; /* third component uses float in evtStageTestViewTargetVector */
} MenuWorkCamera;

typedef struct MenuWorkPosition {
    s32 x;
    s32 y;
    s32 z;
} MenuWorkPosition;

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
void evtStageTestQueueMotionSegment(u32 motionIndex, f32 blendLeadFrames, f32 blendDurationFrames);

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

extern s32 D_00437CA4;

extern const char *D_00437CA0;

extern char D_00437CA8[];

extern f32 dds3CameraObjectBasicGetFovy(const dds3Basic_t *camera);

void evtStageTestDrawCameraTransformDebug(void);

u32 evtStageTestCameraUpdate(void);

/* Create the fixed-transform camera target and return its update callback. */
void *evtCreateBattleStageTestCamera(void);

/* Draw the battle-stage selector; confirmation creates the selected world object
 * and returns its camera callback before the four trigger-bit ID adjustments. */
s32 evtBattleStageTestScreen(dds3ProcessID_t *task);

void evtDestroyBattleStageTestWorldNode(dds3ProcessID_t *task);

/* Destroy the named battle-stage-test task hierarchy and disable the debug graph. */
void evtBattleStageTestStopTask(void);

/* Enable the debug graph and create the battle-stage selector task with its cleanup callback. */
void btlCreateStageTestTask(void);

extern const s32 D_00437C70[2];
/* End of declarations from src/dds2/game/code_002BE628.c */

void mnuHandleListPageJumpInput(const void *resource, MenuWindowContainer *window,
                                MnuInputActionFlags *actionFlags) {
    struct MenuListNode *top = NULL;
    struct MenuListNode *bottom = NULL;
    MenuList *list = window->list;

    if (resource != NULL) {
        if (list->count >= list->visibleCount) {
            if (*actionFlags & MNU_INPUT_ACTION_PREVIOUS_PAGE) {
                top = mnuScrollListToPreviousPage(list);
            }
            if (*actionFlags & MNU_INPUT_ACTION_LAST_PAGE) {
                bottom = mnuScrollListToEnd(window->list);
            }
            if (top == NULL) {
                *actionFlags &= ~MNU_INPUT_ACTION_PREVIOUS_PAGE;
            }
            if (bottom == NULL) {
                *actionFlags &= ~MNU_INPUT_ACTION_LAST_PAGE;
            }
            mnuClearWindowPanelTransitionFlag(window);
            return;
        }
    }
    *actionFlags &= ~MNU_INPUT_ACTION_PREVIOUS_PAGE;
    *actionFlags &= ~MNU_INPUT_ACTION_LAST_PAGE;
}

void mnuHandlePanelListPageJumpInput(MenuWindowContainer *window,
                                     MnuInputActionFlags *actionFlags) {
    mnuHandleListPageJumpInput(window->resource, window, actionFlags);
}
