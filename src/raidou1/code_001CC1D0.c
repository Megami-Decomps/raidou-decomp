#include "common.h"

/* Declarations from dds-decomp src/dds2/game/code_0025DA20.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "event.h"

#include "kernel_draw.h"

#include "newdata.h"

#include "sdf_resource.h"

#include "sdk_libc.h"

#include "dds/dds3_fade.h"

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

#include "dds/kwln.h"

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

/* Return nonzero while either foreground fade direction is enabled. */
s32 dds3FadeCheck(void);

void dds3FadeIn(s32 red, s32 green, s32 blue, s32 duration);

u32 dds3GetProcessData(const dds3ProcessID_t *task);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(EffRandState *state);

dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

dds3TaskKillRequestResult_t dds3KillProcessName(const char *name, s32 delayTicks);

/*
 * Append child to parent's child list, detaching it from any previous parent.
 * This hierarchy controls teardown: parent finalization calls
 * dds3KillProcess(child, 0) for linked children before its destroy callback.
 * It does not change scheduler queue membership.
 */
void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *child);

/* Returns a borrowed task from the scheduler queues, or NULL when none has this priority. */
dds3ProcessID_t *dds3SearchProcessPriority(u32 priority);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

/* Return whether the selected flag bit is set, as EvtBitTestResult. */
EvtBitTestResult evtBitCheck(s32 flag);

void evtBitOff(s32 flag);

EvtSolarCyclePhase evtGetAgeOfMoon16(void);

void evtMakeEventPackName(EvtPackEventId eventId, char *buffer);

/* Event-pack IDs identify camp tasks; lookup returns the scheduler handle. */
dds3ProcessID_t *evtSearchEventPack(EvtPackEventId eventId);

/* Return the signed control-state byte as a query result, not a boolean or a
 * stored phase. NO_ACTIVE_CONTROL (0) covers a missing window and control state
 * two hidden by the gate; stored zero also passes through. Visible stored states
 * one, two and three report ACTIVE, GATED and CLEANUP. Other stored values pass
 * through unchanged. */
EvtMessageWindowControlResult fclChkMessage(void);

/* Close the active display channel and return its status. */
s32 fclDestroyMessage(void);

s32 fclInitMessage(struct ItfMesSub *definition);

/* Start a display entry; return zero if no window is open, one if it starts. */
s32 fclStartMessage(s32 entry);

/* Link a glyph chain into the current draw-buffer slot. */
s32 frDestroyFontRequestAfter(struct _FRQ *glyph);

void frSetDrawPosition(struct _FRQ *glyph, s32 x, s32 y);

/* Replace each child render word along the borrowed glyph chain. */
void frSetFontColor(struct _FRQ *parentNode, u32 colorWord);

/* Bind a texture and emit a sprite from two vertices and packed UV/color pairs. */
void itfMiscDrawSpritePrim(DrawVertex *vertices, DrawColorRec *uv,
                           DrawColorRec *colors, s32 tail,
                           const sdfTexHandle_t *borrowedTexture, s32 flag,
                           sdfDrawTag_t *command);

void *memset(void *dst, s32 value, u32 size);

/* Apply volume and pan to a sequence track through the command queue. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

void *sdfChipAllocDirect(s32 size);

void sdfChipFreeDirect(void *memory);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

void sdfLinkLoadImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 destinationX,
    s32 destinationY, s32 width, s32 height, s32 sourceAddress,
    s32 (*allocatePacket)(s32));

/* resourceAddress is copied as a packet metadata word, not dereferenced. */
void sdfLinkStoreFrameImage(
    sdfDrawTag_t *list, sdfPreDrawTag_t *linkedList, s32 sourceX, s32 sourceY,
    s32 width, s32 height, s32 resourceAddress, s32 arg7, s32 arg8,
    s32 (*allocatePacket)(s32));

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

struct sdfMemHandle_t *sdfMemAllocFromTailDirect(s32 requestedBytes);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

void sdfMemGetInfo(SdfGeneralHeapStats *stats);

void *sdfMemLock(struct sdfMemHandle_t *allocation);

void sdfPreDrawChainLink(SdfPacketChain *chain, sdfPreDrawTag_t *node);

void sdfPreDrawTagInit(sdfPreDrawTag_t *list);

struct sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*allocator)(s32));

s32 strcmp(const char *a, const char *b);

#include "dds/sdk_ps2.h"

#include "dds/evt_message_window_api.h"

#include "dds/dsp_channel.h"

#include "dds/dsp_active_api.h"

#include "dds/dsp_entry_api.h"

#include "dds/common.h"

#include "dds/dds3_task_add_child.h"

#include "dds/mnu_camp_display.h"

#include "dds/mnu_camp_option_index_api.h"

#include "dds/mnu_camp_font_api.h"

#include "dds/evt_solar.h"

#include "dds/eff_random.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/sdf_chip.h"

#include "dds/dat_command.h"

#include "dds/fr_font.h"

#include "dds/fr_font_context.h"

#include "dds/sdf_packet_list.h"

#include "dds/eff_resource_slots.h"

#include "dds/eff_mapped_resource_destroy_result.h"

#include "dds/eff_resource_source_lifetime.h"

#include "dds/sdf_heap_api.h"

#include "dds/evt_viewer.h"

#include "dds/event.h"

#include "dds/evt_viewer_indexed_condition.h"

#include "dds/evt_camera_color_track_api.h"

#include "dds/mnu.h"

#include "dds/mnu_staff.h"

#include "dds/mnu_shop.h"

#include "dds/sdf.h"

#include "dds/sdf_gs_packet.h"

#include "dds/sdf_linked_packet.h"

#include "dds/sdf_packet_builders.h"

#include "dds/itf_panel_draw.h"

#include "dds/kwln.h"

#include "dds/dds3_task_priority_query.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_fade.h"

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

#include "dds/evt_task.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/mdl_flag_api.h"

#include "dds/pty_adjust_item_quantity_api.h"

/* Declared before first use (were implicit int). */
extern void fldDrawCameraColorLayerPass(void);

extern void mnuSetScrollingStripOverlayPosition(DspScrollingStripState *state, s32 vertical, s32 horizontal, s32 unused);

extern void mnuSetScrollingStripSpeedsRandomly(DspScrollingStripState *state, s32 negate, s32 minimum, s32 maximum);

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

extern s32 mnuPreparePopupAndDispatchSelection(dds3ProcessID_t *task);

extern s32 mnuAdvanceCampPopup(dds3ProcessID_t *task);

extern s32 mnuFinishCampPopup(dds3ProcessID_t *task);

extern void mnuInitializeShopStatusBatches(MenuTerminalContext *);

extern void mnuSetCurrentTerminalContext(MenuTerminalContext *);

extern void mnuResetGradientFadeColor(MenuGradientFade *, s32);

extern void p2sdLoadSMGFile(s32);

extern void p2sdPlayBGMFade(s32);

extern s32 mnuCampGetCurrentRowQuantityLimit();

extern s32 mnuCampScalePriceByTier();

#include "dds/pty_bullet_item_count_api.h"

extern s8 mnuCampListedItems[34];

typedef struct CampFlagRow {
    u32 messageSet;  /* 0x00: shop message resource number */
    s16 flag[8];     /* 0x04 */
    u8 value[9];     /* 0x14: [0] default, [i + 1] for flag[i] */
    u8 pad1D[3];
} CampFlagRow;

extern CampFlagRow mnuCampFlagRows[];

extern MenuPopupEntry mnuShopMessageSequencePopupEntry[];

extern sdfTexHandle_t *dds3RenderTexture;

extern EvtPackedSoundTrackId evtStopEventBgm(s32 eventId, EvtSoundVariation variation);

extern s32 D_003C99B8[];

extern void evtViewerCleanupMessageWindow();

extern s32 mnuRebuildCampSaleItemWindow(MenuTerminalContext *);

extern char D_00437838[];

extern char D_00424BC0[];

extern char D_00424BD0[];

extern s8 gProcessStat_0;

extern void mnuShopReleaseWindowSprites();

extern void mnuShopReleaseWindowAndEffectResources();

extern EffMappedResourceDestroyResult mnuShopReleaseSceneObjects(MenuTerminalContext *);

extern void evtReleaseResourcePairHandle(EvtResourcePair *);

extern void mnuDrawAndStepGradientFade(MenuGradientFade *, s32);

extern void mnuSetStateValue(u32 *, u32);

extern u8 D_003CD8D0[];

extern u8 mnuCampWideRewardIdView[];

extern u8 mnuCampCompactEntries[];

extern u8 mnuCampListedItemSolarWeightsView[];

extern u8 mnuIsHighRangeItemId(s32);

extern u8 mnuIsBulletItemId(s32);

extern u8 mnuIsSolarPricedItemId(s32);

extern u8 mnuCampWideRewardEntries[];

extern struct _FRQ *itfDrawBankTextWithLayoutFlags(s32 x, s32 y, s32 depth, u16 textId, struct FrFontTextBank *bank, s32 flags);

extern s32 frFontDrawGlyphChain(struct _FRQ *, s8, u32);

extern f32 mnuShopSavedLastTransformVector[];

extern f32 mnuShopSavedMiddleTransformVector[];

extern f32 mnuShopSavedFirstTransformVector[];

extern s32 mnuShopRestoreMiddleVector;

#include "dds/sdf_packet_alloc.h"

extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *packet, s32 table, s32 mode);

extern void eglSetOt(u32);

extern void evtSubmitPrimaryAlphaBlendMode(s32);

extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 mnuCampQuadBounds[4];

extern s32 mnuCampQuadTexcoords[4];

extern s32 mnuCampQuadColor[4];

extern ShopRankPriceRow mnuShopRankPriceRows[];

/* Schedule the camp task only if no task currently owns this event ID. */
dds3ProcessID_t *evtLoadEventPack(EvtPackEventId eventId);

void evtKillEventPack(EvtPackEventId eventId);

/* Drain every camp task at the scheduler priority used during creation. */
void mnuCampDestroyAllTasks(void);

/* Shift the selected track's frames; values exactly at the upper limit are kept. */
void mnuShopScrollList(dds_evtViewerInfo_t *scene, s32 delta);

/* Shift qualifying keys and their supported payload offsets. Preserve the
 * incoming base/offset/ubase arguments when no key overwrites them: the native
 * cleanup call receives those values even after an empty track traversal. */
void mnuFxWorldScrollDelta(dds_evtViewerInfo_t *scene, s32 delta, s32 threshold, s32 base, s32 offset, s32 ubase);

/* Process keys at or beyond threshold, restarting at the head after each call. */
void mnuFxWorldDropOutOfRange(dds_evtViewerInfo_t *scene, s32 threshold);

void mnuAdjustTimelineDurations(dds_evtViewerInfo_t *world, s32 threshold, s32 delta);

/* Default three-vector slot contents; the trailing two scalars have unknown roles. */
void mnuInitializeCampPanelVisualDefaults(f32 *firstVector, f32 *secondVector, f32 *thirdVector, f32 *scalarA, f32 *scalarB);

extern void evtHokanSceneLight(s32 enable, f32 t, const f32 *first, const f32 *second, f32 *out);

extern f32 evtHokanFloat(s32 enable, f32 t, f32 first, f32 second);

void mnuFindCampKeyTrackNeighbors(EvtRuntimeGroup *track, s32 frame, EvtRuntimeChild **selected, EvtRuntimeChild **future);

void mnuSampleCampPanelVisualTrack(dds_evtViewerInfo_t *runtime, EvtRuntimeGroup *group, s32 frame,
                   f32 (*outVectors)[4], f32 *outValueA, f32 *outValueB);

void mnuCampInitializeDisplayDefaults(CampDisplayDefaults *display);

void mnuFindCampKeyTrackNeighbors(EvtRuntimeGroup *track, s32 frame, EvtRuntimeChild **lowerKey, EvtRuntimeChild **upperKey);

void mnuBlendCampDisplayFields(EvtRuntimeChild *from, EvtRuntimeChild *to, CampDisplayDefaults *display, f32 ratio);

/* Use defaults before the first key; otherwise blend the bounding keys. */
void mnuSampleCampDisplayTrack(dds_evtViewerInfo_t *viewer, EvtRuntimeGroup *track, s32 frame, CampDisplayDefaults *display);

void mnuInitializeCampListLayoutDefaults(EvtBlendKey *layout);

void mnuSampleCampListLayoutTrack(dds_evtViewerInfo_t *viewer, EvtRuntimeGroup *track, EvtBlendKey *out);

/* Blend the display fields; control bytes always come from the lower key. */
void mnuBlendCampDisplayFields(EvtRuntimeChild *from, EvtRuntimeChild *to,
                   CampDisplayDefaults *display, f32 ratio);

void mnuFindCampKeyTrackNeighbors(EvtRuntimeGroup *track, s32 frame, EvtRuntimeChild **lowerKey, EvtRuntimeChild **upperKey);

/* Retail never blocks the event viewer's end countdown through this hook. */
u32 mnuShouldBlockViewerEndCountdown(void);

/* Split the key's packed halfword into its low 12 bits and upper four bits. */
void mnuUnpackNibbleFields(EvtRuntimeChild *key, s32 *lowBitsOut, s32 *highBitsOut);

/* Return the scene's matching name index, or -1 when the track list has no match. */
EvtViewerNameIndex mnuCampFindMatchingEntryIndex(PolyMovieWork *lookup, dds_evtViewerInfo_t *scene, s32 nameIndex);

/* Return the first track with this fixed-width name, or NULL when absent. */
EvtRuntimeGroup *mnuCampFindEntryByName(dds_evtViewerInfo_t *scene, const char *name);

/* Follow zero-coded alternatives, clear on code 1, otherwise resolve a name.
 * Keep the native signed code and separately retained halfword bits distinct. */
void campResolvePendingValue(dds_evtViewerInfo_t *scene, EvtRuntimeChild *cue);

void mnuDrawCampScaledTexture(sdfTexHandle_t *texture, CampDisplayDefaults *display);

/* Clear each track's unknown word and the scene state; do not alter links or values. */
void fldResetCampSceneEntries(dds_evtViewerInfo_t *scene);

/* Blend the first camera-color track at the clamped frame. Lookup includes
 * the track base, but interpolation retains the native unre-based key frames.
 * Without a future key, pass zero blend and the uninitialized fallback block. */
void evtApplyCameraColorTrackAtFrame(dds_evtViewerInfo_t *scene);

/* The genuine three-row typed candidate remains non-matching. */
void fldApplyCameraColorKeyWords(dds_evtViewerInfo_t *viewer, const EvtBlendKey *source);

void evtUpdateCameraColorTrackEffect(dds_evtViewerInfo_t *scene);

void mnuCampInitFontResource(dds_evtViewerInfo_t *scene);

void mnuCampLinkFontGlyph(dds_evtViewerInfo_t *scene);

/* Retail keeps only the divide-by-zero check (break 7) of a division whose result is never used. */
void mnuCampCheckClockDivisor(void);

void mnuEnterCampSceneMenuState(dds_evtViewerInfo_t *scene);

extern s32 dds3AllocRenderTexture(u16 width, u16 height, f32 value);

extern void mnuShopCreateDescriptorSurface(dds_evtViewerInfo_t *scene);

extern void mnuShopSubmitDescriptor(dds_evtViewerInfo_t *scene);

extern void mnuShopQueueAuxiliaryTexture(dds_evtViewerInfo_t *scene);

/* Stages 1, 3, 4 and 5 fall through; stage 0 stops. All other stages,
 * including 2, advance once through the default arm. */
void mnuAdvanceShopMenuState(dds_evtViewerInfo_t *scene);

extern sdfGrOt_t D_00380708;

extern u8 D_00380860[];

void mnuShopCreateDescriptorSurface(dds_evtViewerInfo_t *scene);

void mnuShopSubmitDescriptor(dds_evtViewerInfo_t *scene);

extern sdfGrOt_t D_003805A8;

extern s32 D_003C99F0[4];

extern s32 D_003C9A00[4];

extern s32 D_003C9A10[4];

void mnuShopQueueAuxiliaryTexturePacket(s32 alpha);

void mnuShopQueueAuxiliaryTexture(dds_evtViewerInfo_t *scene);

void func_0025F2C8(void);

void mnuCampSetPrimaryOption(dds_evtViewerInfo_t *scene, MnuCampPrimaryOptionIndex optionIndex);

MnuCampPrimaryOptionIndex mnuCampGetPrimaryOption(dds_evtViewerInfo_t *scene);

void mnuCampSetSecondaryOption(dds_evtViewerInfo_t *scene, MnuCampSecondaryOptionIndex optionIndex);

MnuCampSecondaryOptionIndex mnuCampGetSecondaryOption(dds_evtViewerInfo_t *scene);

/* Clear script registers 200..209 whose type-4 key is not active at this frame. */
void mnuValidateCampTimelineScriptRegisters(dds_evtViewerInfo_t *scene);

/* Save the first and last four-component vectors; leave the middle unsaved. */
void mnuShopSavePrimaryTransform(dds_evtViewerInfo_t *scene);

/* Save all three vectors and enable the middle-vector restore path. */
void mnuShopSaveFullTransform(dds_evtViewerInfo_t *scene);

/* Keep the native restore-mode snapshot and last/middle/first write order. */
void mnuShopRestoreTransform(dds_evtViewerInfo_t *scene);

/* Append a distinct identifier only while the native registry has room. */
void fldRegisterCampSceneId(dds_evtViewerInfo_t *scene, s32 identifier);

/* Queue the scene's BGM ID with each registered variation, then clear the list.
 * The loop bound is read again after each queue call, not cached up front. */
void mnuReleaseCampSceneRegisteredIds(dds_evtViewerInfo_t *scene);

void mnuEvaluateCubicBezierPoint(f32 parameter, const s32 *x, const s32 *y, s32 *outX, s32 *outY);

extern void mnuEvaluateCubicBezierPoint(f32 parameter, const s32 *x, const s32 *y, s32 *outX, s32 *outY);

s32 mnuSolveCubicBezierYForX(s32 target, const s32 *x, const s32 *y);

void mnuInitializeShopStatusBatches(MenuTerminalContext *scene);

/* Destroy both batches and return the second destruction result. */
EffMappedResourceDestroyResult mnuShopReleaseSceneObjects(MenuTerminalContext *scene);

extern const CampMapArguments D_00424A90;

extern const CampEffectRows D_00424AC0;

extern const char D_00424AE0[];

extern void mnuInitializeMapPacket(u32 packetValue, u32 *values, s32 count, MapPacket *packet);

extern void mnuCopyCampEffectRowData(const CampEffectRows *source, MenuEffectResources *resources);

extern void mnuOrEntryFlags(u32 flags, u32 *entryFlags);

void mnuInitializeCampEffectResources(EffectSlotSet *object, MenuEffectResources *resources);

void mnuShopDestroyNestedEffectBatch(MenuEffectResources *resources);

extern s32 mnuFirstPresentMainCharacterIndex(void);

extern const char *mnuShopPrimarySpriteNames[4];

extern const char *mnuShopManWomanSpriteNames[];

extern const char *mnuShopCharacterSpriteNames[4];

void mnuShopLoadSpriteAssets(MenuTerminalContext *scene);

extern void evtLoadResourcePair(const char *, EvtResourcePair *);

void mnuShopLoadMessageResource(MenuTerminalContext *scene);

extern void effDrawTextureSlotWithBlendedCornerColors(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);

extern void mnuDrawCampIconBackdrop(MenuCampEffect *, s32);

/* The shop layouts use slot 1 as their background and slot 3 as the rotated portrait. */
void mnuDrawShopPanelDecorations(MenuTerminalContext *scene);

/* Callers pass their menu context; the check reads only global stock. */
s32 mnuCampHasEligibleOwnedItems(void *context);

extern void mnuDrawMenuItemListRow(void);

MenuWindowContainer *mnuCreateEnabledCampEntryWindow(s32 count, s32 *enabled, MenuTerminalContext *settings);

typedef struct CampEntryEnableSet {
    s32 enabled[7];
} CampEntryEnableSet;

extern const CampEntryEnableSet D_00424BA0;

extern s32 mnuHasCampFlaggedItemByClass(MenuTerminalContext *, s32);

void mnuBuildEnabledCampEntryWindow(MenuTerminalContext *scene);

s32 mnuCampResolveFlagRowValue(s32 row);

/* Scan the twenty-one flag IDs in descending slot order. */
/* Highest matching row wins; rows with nonpositive flags are not queried. */
s32 mnuCampFindActiveSlot(void);

typedef struct ShopSourcePriceEntry {
    u16 itemId;
    u8 type;
    u8 pricePercent;
} ShopSourcePriceEntry;

typedef struct ShopSourcePriceRow {
    u16 pricePercent;
    ShopSourcePriceEntry entries[0x60];
} ShopSourcePriceRow;

extern ShopSourcePriceRow mnuShopSourcePriceRows[];

/* True when the shop's source row or the active rank row stocks an item of the
 * requested class (1: bullets, otherwise the high item range). */
s32 mnuHasCampFlaggedItemByClass(MenuTerminalContext *scene, s32 itemClass);

/* The retail stub always returns zero and has no memory effects. */
u32 __attribute__((const)) mnuCampGetProgressStage(void);

/* Seed or refresh the terminal message option; retail always selects zero. */
u32 mnuGetTerminalMessageOption(void);

/* Sum the active low bit across five entries using the native halfword stride. */
s32 mnuCountActivePartyEntries(void);

typedef struct CampTier {
    u32 threshold;
    s32 value;
} CampTier;

extern CampTier mnuShopCurrencyAwardTiers[];

s32 mnuCampResolveProgressTierValue(void);

void mnuCampClearListedItemCounts(void);

MenuTerminalContext *mnuTerminalCreateContext(void);

void mnuTerminalReleaseContextAndResources(dds3ProcessID_t *task);

s32 mnuTerminalSyncMessageWindowControl(dds3ProcessID_t *task);

void mnuCreateCampShopPopupTasks(const s32 *stockGroup);

void mnuCampDestroyPanelTasks(void);

s32 mnuCampConsumePanelTaskCompletion(void);

s32 mnuPreparePopupAndDispatchSelection(dds3ProcessID_t *callback);

s32 mnuAdvanceCampPopup(dds3ProcessID_t *callback);

s32 mnuFinishCampPopup(dds3ProcessID_t *callback);

typedef struct ShopProgressPrice {
    f32 percent;
    u32 unk4;
} ShopProgressPrice;

extern ShopProgressPrice mnuShopProgressPriceScales[];

s32 mnuCampGetSourceItemPrice(s32 entryIndex, s32 shopRow, s32 halfPrice);

s32 mnuCampGetRankedItemPrice(s32 entryIndex, s32 halfPrice);

s32 mnuCampFindListedItemIndex(s32 itemId);

typedef struct EffItemSlot44 {
    u32 id;
    u8 pad_04[0x40];
} EffItemSlot44;

extern EffItemSlot44 mnuCampCompactEntryClaimFlagView[];

extern void evtBitOn(s32);

s32 itmClaimFreeSlot(s32 row);

/* Convert a camp reward row and its three-way option to a flat index. */
s32 mnuCampGetWideRewardRowIndex(s32 listedItemIndex, s32 option);

s32 mnuCampGetCompactRewardRowIndex(s32 slot, s32 listedItemIndex);

u32 mnuCampChooseSolarWeightedOption(s32 listedItemIndex);

u8 mnuCampChooseWeightedTableValue(void);

s32 mnuCampFindFreeWideEntryIndex(s32 rewardRow);

s32 mnuCampFindFreeCompactEntryIndex(s32 unused, s32 listedItemIndex);

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

extern s32 mtrMantraFindIndex(s32);

s32 mnuCampChooseWideRewardEntry(s32 rewardRow);

extern u8 mnuCampWideRewardKindByteView[];

u8 mnuCampGetWideRewardKind(s32 rewardRow, s32 column);

extern u8 mnuCampCompactRewardKindByteView[];

u8 mnuCampGetCompactRewardKind(s32 rewardRow, s32 column);

s32 mnuCampResolveOwnedItemVariant(s32 rewardRow, s32 column);

s32 mnuCampGetCompactEntryId(s32 rewardRow, s32 column);

s32 mnuCampCountRemainingUses(s32 itemMode, s32 itemId, MenuTerminalContext *record);

s32 mnuCampGetAffordablePurchaseCount(MenuTerminalContext *scene);

/* Mark rows unavailable when neither currency nor inventory capacity permits a use. */
void mnuCampDisableUnavailableItemEntries(MenuTerminalContext *scene);

typedef struct CampPriceTier {
    u8 limit;
    u8 percent;
} CampPriceTier;

extern CampPriceTier mnuShopQuantityPriceTiers[];

typedef struct CampRateRow {
    u16 percent;
    u8 pad02[10];
} CampRateRow;

extern CampRateRow mnuShopProgressSlotPricePercentView[];

s32 mnuCampAdvanceCounter(s32 delta, MenuTerminalContext *scene);

extern u8 *D_00435E5C;

extern u8 mnuShopSourceEntryTypeView[];

extern void mnuSelectFirstListNode();

extern void mnuClearWindowListSelectionFlags(MenuWindowContainer *);

extern s32 mnuDrawCampItemListRow();

extern s32 mnuCampGetSourceItemPrice(s32, s32, s32);

s32 mnuCampAppendRankedItemRows(MenuTerminalContext *scene);

s32 mnuCampAppendRankedFilteredRows(MenuTerminalContext *scene, s32 filterMode);

s32 mnuCampRebuildItemStockWindow(MenuTerminalContext *scene);

s32 mnuCampAppendRankedFilteredRows(MenuTerminalContext *scene, s32 filterMode);

s32 mnuCampRebuildFilteredStockWindow(MenuTerminalContext *scene, s32 filterMode);

extern u32 mnuShopSolarPhaseSalePrices[][16];

s32 mnuRebuildCampSaleItemWindow(MenuTerminalContext *scene);

void mnuQueueCampTextGlyphWithChildColor(s32 fontValue, s32 enabled, s32 unused2, s32 unused3, s32 fontArg, s32 flags);

#include "dds/mnu_input.h"

#include "dds/common.h"

#include "dds/mnu.h"

#include "dds/dat_state.h"

#include "dds/mnu_list.h"

#include "dds/kwln.h"

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

extern s32 evtAdvanceSlotFlags(void);

extern void datAddCurrencyClamped(s32);

extern s32 mnuCampFindListedItemIndex(s32);

extern void mnuDrawCommandPhaseTransition(s32);

extern s32 mnuDrawCampCommandTransition(MenuTerminalContext *);

extern s32 mnuShopProgressSlotRows[];

extern MenuPopupEntry mnuShopRangeFilteredCommandPopupEntries[];

extern void mnuSetQuantityListTransition(struct MenuList *, u32);

extern s32 mnuCampAdvanceCounter(s32, MenuTerminalContext *);

extern MenuPopupEntry mnuShopCommandPopupEntries[];

extern u8 *datUnitNames;

extern char D_00437840[];

extern s32 D_003C9A20[];

extern void evtCopyEntryStringToActiveWindow(s32, const char *);

extern void evtClearActiveFlag();

extern void mnuBuildEnabledCampEntryWindow();

extern s32 mnuCampHasEligibleOwnedItems();

extern s32 mnuShopProgressCompletionFlagView[];

extern MenuPopupEntry mnuShopSelectedRewardPopupEntry[];

extern const EvtFlagGate D_003CE400[];

extern s32 mnuTickExtendedCommandPhase(MenuTerminalContext *);

extern u8 mnuIsSolarPricedItemId(s32);

extern MenuPopupEntry mnuShopProgressGatePopupEntry[];

extern u16 D_003CE3F8[];

extern void mnuDrawSpriteMenuFadeOut();

extern s32 mnuDrawCommandClosePhase(MenuTerminalContext *);

extern void mnuSetCommandPhase(MenuTerminalContext *, u32);

extern void dspRequestMessageWindowModeOne(void);

extern MenuPopupEntry mnuShopCatalogPopupEntries[];

extern void mnuSetValueListTransition(struct MenuList *, u32);

extern void mnuDrawFadingValueListRow();

extern MenuPopupEntry mnuShopFilteredCatalogPopupEntries[];

extern MenuPopupEntry mnuShopSaleCommandPopupEntries[];

extern s32 mnuRebuildCampSaleItemWindow(MenuTerminalContext *);

extern MenuPopupEntry mnuShopReturnFadePopupEntry[];

extern void mnuStorePendingMenuCommandValue(struct MenuList *, u32);

extern void mnuSetSelectionListTransition(struct MenuList *, u32);

extern void mnuPlayInputSound(s32, s32, u32 *);

extern MenuPopupEntry mnuShopQuantityPopupEntry[];

s32 evtIsFadeDispatchIdle(void);

#define MNU_INSTALL_STATE_TABLE(ctx, table, entry) \
    do { \
        (ctx)->popupTable = (table); \
        mnuSetPopupEntry(&(ctx)->popupState, (entry)); \
    } while (0)

void evtInstallStateTable(MenuTerminalContext *state);

/* Mirror the chosen slot into both the active scene record and dispatch state. */
s32 evtInitializeSelectedSlot(dds3ProcessID_t *task);

s32 evtAdvancePhaseOne(dds3ProcessID_t *task);

extern s32 mnuTickCommandPhase(MenuTerminalContext *);

extern MenuPopupEntry mnuShopReturnDelayPopupEntry[];

s32 mnuShopPollCommandSelection(dds3ProcessID_t *task);

s32 evtPrimeDispatchStart(dds3ProcessID_t *callbackContext);

s32 evtSetupDispatchSync(dds3ProcessID_t *callbackContext);

void evtInstallStateTableB(MenuTerminalContext *state);

s32 evtBuildShopCatalogIfActive(dds3ProcessID_t *task);

s32 evtSelectStateAction(dds3ProcessID_t *task);

s32 mnuShopPollBuyCommandSelection(dds3ProcessID_t *task);

s32 evtStageDispatchStart(dds3ProcessID_t *callbackContext);

s32 evtSetupDispatchSyncB(dds3ProcessID_t *callbackContext);

void evtInstallStateTableC(MenuTerminalContext *state);

s32 evtBuildFilteredShopCatalogIfActive(dds3ProcessID_t *task);

s32 evtSelectStateActionB(dds3ProcessID_t *task);

s32 mnuShopPollFilteredCommandSelection(dds3ProcessID_t *task);

s32 evtStageDispatchStartB(dds3ProcessID_t *callbackContext);

s32 evtSetupDispatchSyncC(dds3ProcessID_t *callbackContext);

void mnuInstallNoCapacityStateTable(MenuTerminalContext *state);

s32 evtBuildRangeFilteredShopCatalogIfActive(dds3ProcessID_t *task);

s32 mnuResetCommandStepAndSelectPhase(dds3ProcessID_t *task);

s32 mnuShopPollRangeFilteredCommandSelection(dds3ProcessID_t *callbackContext);

s32 evtStageDispatchStartC(dds3ProcessID_t *callbackContext);

s32 evtSetupDispatchSyncD(dds3ProcessID_t *callbackContext);

void evtInstallStateTableD(MenuTerminalContext *state);

s32 evtEnableStateFlag(dds3ProcessID_t *task);

s32 evtEnterProgressCommandPhase(dds3ProcessID_t *task);

s32 mnuShopPollSaleCommandSelection(dds3ProcessID_t *task);

s32 mnuRunCampCommandPrimaryTransition(dds3ProcessID_t *callbackContext);

s32 mnuRunCampCommandSecondaryTransition(dds3ProcessID_t *callbackContext);

/* Return a slot's remaining threshold, or -1 for an unavailable slot. */
s32 evtGetRemainingSlotThreshold(u32 slotIndex);

s32 evtShowResultText(void);

/* Registered shop progress-result leave hook: no side effects; returns one. */
u32 mnuShopProgressResultLeaveNoOp(void);

s32 evtOpenProgressResultPopupWhenIdle(dds3ProcessID_t *callbackContext);

s32 evtStartProgressResultPrimaryTransition(dds3ProcessID_t *callbackContext);

s32 evtStartProgressResultSecondaryTransition(dds3ProcessID_t *callbackContext);

extern s32 D_003CE420[8];

extern s32 D_003CE440[];

/* Registered in the camp popup table; start the message for the selected shop row.
 * Message-set two picks one of five entries from the first group whose flag is
 * clear; sets one and three pick one of the row's pair by its flag. */
s32 mnuStartShopRowMessage(dds3ProcessID_t *callbackContext);

/* Registered shop row-message leave hook: no side effects; returns one. */
u32 mnuShopRowMessageLeaveNoOp(void);

s32 mnuPollTerminalPanelTransition(dds3ProcessID_t *callbackContext);

s32 mnuStartTerminalPanelPrimaryTransition(dds3ProcessID_t *callbackContext);

s32 mnuStartTerminalPanelSecondaryTransition(dds3ProcessID_t *callbackContext);

struct MenuList;

extern void mnuSelectLastListNode(struct MenuList *list);

u32 evtResetStateProgressTimer(dds3ProcessID_t *task);

/* Delay the next state table until dispatch is idle and 20 progress ticks elapse. */
s32 evtQueryStateProgress(dds3ProcessID_t *callbackContext);

s32 evtDispatchProgressCallback(dds3ProcessID_t *callbackContext);

s32 evtSetupDispatchSyncE(dds3ProcessID_t *callbackContext);

s32 evtApplyBaseRateProgressStep(dds3ProcessID_t *task);

s32 evtAdvanceStateStage(dds3ProcessID_t *task);

extern s32 mnuTickCommandWaitPhase(MenuTerminalContext *);

extern MenuPopupEntry mnuShopTransactionPopupEntry[];

/* Poll command phases, adjust the idle quantity selection, and play UI sounds. */
s32 evtPollQuantitySelection(dds3ProcessID_t *callbackContext);

s32 evtAlignDispatchStart(dds3ProcessID_t *callbackContext);

s32 evtSetupDispatchSyncF(dds3ProcessID_t *callbackContext);

s32 mnuShopBeginTransactionMessage(dds3ProcessID_t *task);

s32 evtUpdateSlotItemCompletionState(dds3ProcessID_t *task);

s32 evtPollStageSelectionAndAdvance(dds3ProcessID_t *task);

/* The stage-selection caller forwards its task to the user-value lookup. */
void evtMarkSceneFollowupReadyAndQueueAction(dds3ProcessID_t *task);

void evtAccumulateEligibleStageMultiplierValue(MenuTerminalContext *state);

s32 evtIsAllowedId(s32 itemId);

/* Add progress, treating zero as one. The unsigned check also clamps negative totals. */
s32 evtApplyShopProgressDelta(s32 progressDelta);

/* Apply the selected shop transaction and preserve its progress baseline. */
s32 evtApplyShopQuantityTransaction(dds3ProcessID_t *task);

extern s8 evtGetCapturedWindowPanelValue(void);

extern MenuPopupEntry mnuShopPendingRewardPopupEntry[];

extern MenuPopupEntry mnuShopSlotAdvancePopupEntry[];

/* Complete an idle shop confirmation and queue its followup state. */
s32 evtDispatchShopSelectionAction(dds3ProcessID_t *task);

s32 evtDispatchSceneReadyFollowup(dds3ProcessID_t *callbackContext);

s32 evtSetupDispatchSyncG(dds3ProcessID_t *callbackContext);

s32 evtPlayDispatchModeCue(dds3ProcessID_t *task);

s32 evtApplyDispatchModeState(dds3ProcessID_t *task);

s32 evtSetPopupEntryWhenMessageWindowIdle(dds3ProcessID_t *callbackContext);

extern void mnuDrawShopPanelWithTypeGraphic(s32, s32, s32, MenuTerminalContext *, s32, s32);

extern void mnuDrawTerminalPanelListRows(s32, s32, s32, void *, s32, s32);

extern void mnuDrawWindowBorderAndScrollBar(s32, s32, s32, MenuTerminalContext *, s32, s32);

extern void mnuDrawTerminalPanelValues(s32, s32, s32, MenuTerminalContext *, s32, s32, s32);

extern void mnuDrawIfActive(s32, s32, s32, MenuWindowContainer *, s32);

extern void mnuDrawShopPricePlaceholder(s32, s32, s32, MenuTerminalContext *, s32);

extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, u8 *, s32);

extern void mnuDrawShopZeroValueIconAndText(s32, s32, s32, s32, s32, s32);

extern void mnuDrawShopCurrencyBalance(MenuTerminalContext *, s32);

extern void brsDrawResultIconPairWithAlpha(MenuTerminalContext *, u32, s32, s32);

/* Draw the active dispatch mode before advancing its callback state. */
s32 mnuShopDrawMenuState(dds3ProcessID_t *callbackContext);

s32 evtSetupDispatchSyncH(dds3ProcessID_t *callbackContext);

extern s32 mnuCampGetWideRewardRowIndex(s32 listedItemIndex, s32 option);

extern u32 mnuCampChooseSolarWeightedOption(s32 listedItemIndex);

extern u8 mnuCampChooseWeightedTableValue(void);

extern s32 mnuCampFindFreeCompactEntryIndex(s32 unused, s32 listedItemIndex);

s32 evtMenuPrepareCampRewardSelection(dds3ProcessID_t *task);

/* Registered camp reward-selection leave hook: no side effects; returns one. */
u32 mnuCampRewardSelectionLeaveNoOp(void);

extern s32 mnuCampChooseWideRewardEntry(s32);

extern u8 mnuCampGetWideRewardKind(s32, s32);

extern u8 mnuCampGetCompactRewardKind(s32, s32);

extern s32 mnuCampResolveOwnedItemVariant(s32, s32);

extern s32 mnuCampGetCompactEntryId(s32, s32);

extern u8 mnuIsHighRangeItemId(s32);

extern MenuPopupEntry mnuShopSlotAdvancePopupEntry[];

extern char D_00437848[];

extern char D_00437850[];

s32 evtAdvancePendingRewards(dds3ProcessID_t *callbackContext);

s32 evtDrawPendingRewardTransition(dds3ProcessID_t *callbackContext);

s32 evtAdvancePendingRewardMessage(dds3ProcessID_t *callbackContext);

/* Mark each newly completed slot and advance the persistent slot index. */
s32 evtAdvanceSlotFlags(void);

u32 evtUpdateSlotAdvanceCount(dds3ProcessID_t *task);

/* Registered slot-advance message leave hook: no side effects; returns one. */
u32 mnuSlotAdvanceMessageLeaveNoOp(void);

s32 evtOpenSlotAdvancePopupWhenIdle(dds3ProcessID_t *callbackContext);

s32 evtDrawSlotAdvanceTransition(dds3ProcessID_t *callbackContext);

s32 evtAdvanceSlotAdvanceMessage(dds3ProcessID_t *callbackContext);

/* Raise a gate's flag and play its cue only on the first threshold crossing. */
s32 evtTriggerProgressFlagGate(s32 unusedContext);

s32 evtShowSlotText(dds3ProcessID_t *task);

/* Registered progress-gate message leave hook: no side effects; returns one. */
u32 mnuProgressGateMessageLeaveNoOp(void);

s32 evtTriggerProgressGateThenOpenPopup(dds3ProcessID_t *callbackContext);

s32 evtDrawProgressGateTransition(dds3ProcessID_t *callbackContext);

s32 evtAdvanceProgressGateMessage(dds3ProcessID_t *callbackContext);

/* Select the highest reached reward tier, retiring earlier tier flags first. */
s32 mnuSelectUnclaimedProgressRewardTier(void);

s32 evtIsLastSlot(s32 slotIndex);

/* Set the model flag owned by the given slot. */
void mnuSetFlagForMenuEntry(s32 slotIndex);

#include "dds/kwln.h"

#include "dds/mnu.h"

#include "dds/dat_state.h"

#include "dds/mnu_list.h"

extern void dspRequestMessageWindowModeOne(void);

extern MenuPopupEntry mnuShopGrantedRewardPopupEntry[];

extern char D_00437850[];

extern s32 mnuSelectUnclaimedProgressRewardTier();

extern void mnuBuildEnabledCampEntryWindow();

extern s32 mnuCampHasEligibleOwnedItems();

extern void mnuShopLoadMessageResource(MenuTerminalContext *);

extern s32 mnuFirstPresentMainCharacterIndex();

extern void evtClearActiveFlag();

extern u32 D_003CE460[];

extern char D_00437840[];

extern void dspAdvanceMessageWindowControl();

extern s32 mnuCampResolveProgressTierValue();

extern void evtCopyEntryStringToActiveWindow(s32, const char *);

extern s32 evtIsLastSlot(s32);

struct dds3ProcessID_t;

s32 evtMenuPopulateSelectedSlotLabels(struct dds3ProcessID_t *task);

/* Registered selected-reward message leave hook: no side effects; returns one. */
u32 mnuSelectedRewardMessageLeaveNoOp(void);

/* Poll the event window; when it closes, install the default window if needed. */
s32 evtMenuPollWindow(dds3ProcessID_t *callback);

s32 evtDrawSelectedRewardTransition(dds3ProcessID_t *callback);

s32 evtFinishPopupAfterMenuConfiguration(dds3ProcessID_t *callback);

/* Hand out the captured slot's reward: an item (named in the window) or a currency amount. */
s32 evtMenuGrantSelectedProgressReward(dds3ProcessID_t *task);

/* Walk the list until its selected id is found, then persist the slot choice. */
s32 evtMenuPersistSelectedSlot(dds3ProcessID_t *task);

extern void mnuSetFlagForMenuEntry(s32);

extern s32 mnuCampRebuildItemStockWindow(MenuTerminalContext *);

extern s32 mnuCampRebuildFilteredStockWindow(MenuTerminalContext *, s32);

extern s32 mnuRebuildCampSaleItemWindow(MenuTerminalContext *);

s32 evtMenuUpdateProgressRewardPopup(dds3ProcessID_t *callback);

s32 evtDrawGrantedRewardTransition(dds3ProcessID_t *callback);

s32 evtAdvanceGrantedRewardMessage(dds3ProcessID_t *callback);

/* Fade out according to the event mode, with a separate flag-dependent case 2. */
s32 evtStartFadeByState(dds3ProcessID_t *task);

/* Registered shop-message leave hook: no side effects; returns one. */
u32 mnuShopMessageLeaveNoOp(void);

u32 evtMenuSetProgressFlag(MenuTerminalContext *context);

void mnuAwardCampProgressCurrency(void);

/* One-shot menu flag: set the object's flag the first time it is not yet set, returning 1 only then. */
s32 evtMenuSetTypeProgressFlagOnce(MenuTerminalContext *object);
/* End of declarations from src/dds2/game/code_0025DA20.c */

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
