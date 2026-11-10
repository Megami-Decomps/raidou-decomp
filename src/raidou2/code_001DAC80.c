#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_00235270.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "newdata.h"

#include "sdfDevCons.h"

#include "sdk_libc.h"

#include "sdk_libkernl.h"

#include "dds/btl.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3_task_update_count.h"

#include "dds/dds3_world_primary.h"

#include "dds/dds3_world_secondary.h"

#include "dds/dds3obj.h"

#include "dds/eff_event.h"

#include "dds/eff_event_setup_readiness.h"

#include "dds/evt_picture.h"

#include "dds/evt_polygon_movie_load_api.h"

#include "dds/evt_unit.h"

#include "dds/evt_viewer.h"

#include "dds/evt_world.h"

#include "dds/itf_mes_panel_mode.h"

#include "dds/itf_mes_window.h"

#include "dds/kwln.h"

#include "dds/mdl.h"

#include "dds/mdl_motion_controller_lookup_api.h"

#include "dds/sdf.h"

#include "dds/sdf_delayed_resource_check_api.h"

#include "dds/sdf_file_existence_api.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libkernl.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/snd_track_playback_api.h"

/* The primary object returned by dds3GetActiveScene is distinct from the active
 * world's WorldInfo payload. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * DDS1 and DDS2 use the same observed layout. Node payloads depend on the slot. */
enum {
    EVT_WORLD_SLOT_MOVIE = 3, /* viewer movie-time lists: byte offset 0x28 */
    EVT_WORLD_SLOT_CAMERA = 4, /* linked cameras; active nodes supply the view transform */
    EVT_WORLD_SLOT_UNIT = 5   /* unit/group/lip-sync lists: byte offset 0x40 */
};

void LoadTexture(EvtPictureWork *context, const char *path);

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void ToMatrix(void);

/* Append the common rectangle used to frame a grid of sprite cells. */
void dds3ConsPanelTagLink(struct sdfDrawTag_t *packetList, s32 column, s32 row,
                        s32 columnCount, s32 rowCount);

struct sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

/* Return the borrowed primary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetActiveScene(void);

/* Return the borrowed secondary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetControlScene(void);

u32 dds3GetProcessData(const dds3ProcessID_t *task);

/* The API returns this count; it resets on creation/activation and increments
 * after callback handling, except when an active callback finishes immediately. */
dds3TaskUpdateCount_t dds3GetProcessRunTime(const dds3ProcessID_t *task);

dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *world, const char *name);

dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *world, s32 slotIndex, const char *name);

/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */
dds3ProcessID_t *dds3SearchProcessName(const char *name);

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *world, u32 key, s32 kind);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

void effEveBEDCutSetScale(effEveBEDCutHandle_t *work, f32 scale);

void evtEventViewerShutdown(dds_evtViewerInfo_t *viewer);

PolyMovieWork *evtGetPolygonMovieWorkData2(s32 eventId, s32 sceneId, EvtPolygonMovieResourceMode resourceMode);

evtUnitHandle_t *evtGetUnitHandle2(dds3Basic_t *unit);

void evtPictureShutdown(dds3ProcessID_t *task);

EffEventSetupReadiness evtViewerCheckDirty(void);

void itfMesMngChangeWindowType(ItfMesWindowIndex window, ItfMesPanelMode panelMode,
    ItfMesPanelFadeLimit fadeLimit);

/* Set or clear only bits from the high-half mask. */
void itfMesMngMarkBitStat(ItfMesWindowIndex window, ItfMesHighStatusBits mask);

/* Borrow a controller owned by ctx. The pointer is read-only and expires when
 * that controller is removed or the model context is freed. */
const struct sdfMotionCtrlr_t *mdlGetMotionCtrlr(
    const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);

/* The referenced halfword is promoted to a word-sized SDK result. */
s32 mdlGetNumOfCut(const dds3ModelHandle_t *ctx, s32 searchId);

void mdlSmoothSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                       f32 blendDurationFrames);

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

EffEventSetupReadiness mfilterRun(void);

void p2sdPlayBGM(s32 trackId);

/* Apply volume and pan to a sequence track through the command queue. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

EffEventSetupReadiness rblurProcess(void);

s32 sceClose(s32 fd);

s32 sceSync(const char *device, s32 flag);

SdfDelayedResourceCheckResult sdfDelayedResrcCheck(void);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

SdfFileExistenceResult sdfFileCheckExist(const char *path);

/* Queue the two heap owners separately: the general heap stores descriptors,
 * while the chip heap stores the address of the cell to release. */
void sdfMemFreeDelayedDirect(struct sdfMemHandle_t *allocation);

void sdfTexFreeDelayed(sdfTexHandle_t *texture);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */
/* For a new texture, copy payload bytes into queue-owned storage before return. */
struct sdfTexHandle_t *sdfTexSet(const SdfTextureFileHeader *header);

s32 strcmp(const char *a, const char *b);

u32 strlen(const char *text);

#include "dds/sdk_ps2.h"

#include "dds/snd_packed_track_load_status.h"

#include "dds/common.h"

#include "dds/sdf_file_existence_api.h"

#include "dds/evt_viewer_track_file_format.h"

#include "dds/sdf_delayed_resource_check_api.h"

#include "dds/sdf_vu0.h"

#include "dds/snd_track_playback_api.h"

#include "dds/dds3_background_fade_api.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/itf_mes_window.h"

#include "dds/itf_mes_panel_mode.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_heap_api.h"

#include "dds/evt_viewer.h"

#include "dds/event.h"

#include "dds/evt_menu_table_entry_availability.h"

#include "dds/mnu_camp_option_index_api.h"

#include "dds/evt_reload_viewer_api.h"

#include "dds/evt_viewer_rows.h"

#include "dds/evt_float_panel_row.h"

#include "dds/evt_pad.h"

#include "dds/evt_serialized_child.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/evt_world.h"

#include "dds/evt_picture.h"

#include "dds/evt_event_bgm_readiness.h"

#include "dds/evt_sound_variation.h"

#include "dds/evt_sky_alpha_animation_status.h"

#include "dds/eff_object.h"

#include "dds/eff_event.h"

#include "dds/eff_event_setup_readiness.h"

#include "dds/pcp_vu0.h"

#include "dds/ee_mmi.h"

#include "dds/mdl.h"

#include "dds/mdl_motion_controller_lookup_api.h"

#include "dds/sdf.h"

#include "dds/sdf_sif_command.h"

#include "dds/kwln.h"

#include "dds/dds3_task_runner.h"

#include "dds/dds3_task_update_count.h"

#include "dds/dds3_task_create.h"

#include "dds/evt_task.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_object_key.h"

#include "dds/evt_unit_vector_slot.h"

#include "dds/evt_event_pack.h"

#include "dds/evt_polygon_movie.h"

#include "dds/evt_polygon_movie_load_api.h"

#include "dds/evt_effect_scale_object.h"

#include "dds/fld.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/sdf_texture_file.h"

#include "dds/btl.h"

/* Declared before first use (were implicit int). */
extern void evtViewerPushCommandHistory(s32 actionMode, s32 first, s32 second, dds_evtViewerInfo_t *viewer);

extern s32 evtDrawFrameListColumnHeadings(sdfDrawTag_t *list, s32 x, s32 y, dds_evtViewerInfo_t *ctx);

extern void evtWriteSerializedBytes(s32 output, void *data, s32 size);

extern SndPackedTrackLoadStatus p2sdLoadCheckMidi(s32 sequence);

extern void p2sdLoadSMGFile(s32 sequence);

extern dds3TaskUpdateResult_t evtUpdateFrameVariableTask(dds3ProcessID_t *task);

extern void dds3EffectObjectBasicStatOn(dds3Basic_t *object, u32 flags);

struct SoundMixer;

extern dds3Basic_t *dds3AddBEDEffectObjectBasic2(struct SoundMixer *resource, const void *position, const void *scale);

extern void fldSetFilterAnime(u32 mode);

extern void fldSkySetAlpha(u32 drawState);

extern void fldSkySetAlphaOfs(u32 alpha);

extern s16 fldSkyGetAlphaOfs(void);

extern void fldSetFadeTarget(s32 area, s32 value, s32 duration);

extern void fldSkyDrawFilter(void);

extern void rsetOtOfs(s32 id);

extern void fldSubmitTexturedOverlaySprite(void);

extern void p2sdPlayBGMFade(s32 track);

extern void p2sdSetBgmTrack(s32 sequence);

#include "dds/sdf_debug_api.h"

extern void p2sdStopBGM(u32 sequence);

extern void p2sdStopSe(u32 sequence);

extern void evtApplyViewerTimelineFrame(s32 time, dds_evtViewerInfo_t *viewer);

extern char evtSkyTaskName[];

extern u8 D_003BC360[];

extern char D_003BC058[];

extern char D_003BC0A8[];

extern char D_003BC0B0[];

extern char D_003BC0B8[];

extern char D_003BC0C0[];

extern char D_003BC098[];

extern u16 skyanow;

extern u16 skyamax;

extern s16 skyatarget;

extern s16 skyabase_par;

typedef struct {
    s16 enabled;
    s8 fields[8]; /* Per-row signed lookup values at +0x2. */
} EvtTblEntry; /* 0xA bytes */

extern EvtTblEntry D_00368950[];

extern EvtSkyAlphaAnimationStatus skyanime;

extern u16 D_003BBE78;

extern u16 D_003BBE7A;

extern u32 fldbokasi;

extern char D_003BC0C8[];

/* Create a task with an initialized event payload. */
dds3ProcessID_t *evtStartPictureProcess2(s32 taskId, const char *path);

dds3ProcessID_t *evtStartPictureProcess(s32 taskId, sdfTexHandle_t *texture);

void evtSetBokasi(u32 enabled);

/* Either set the sky parameter immediately or interpolate from its current value. */
void evtSetSkyAlpha(s32 duration, s32 target);

EvtSkyAlphaAnimationStatus evtCheckSkyAlphaAnime(void);

void evtUpdateSkyAlpha(void);

s32 evtSkyProcess1(dds3ProcessID_t *task);

void evtSkyShutdown(dds3ProcessID_t *task);

void evtExitSky(void);

dds3ProcessID_t *evtInitSky(void);

/* Frame-variable task update: polls the parent task's user value each frame. */
dds3TaskUpdateResult_t evtUpdateFrameVariableTask(dds3ProcessID_t *task);

dds3ProcessID_t *evtInitFrameVar(void);

typedef s32 (*EvtMenuHeaderFn)();

typedef void (*EvtMenuRowFn)();

extern u32 D_003BC040;

extern char D_003BC048[];

extern char D_003BC050[];

/* Draw a framed debug menu: the optional header returns how many rows it used, the row callback fills the rest,
 * and blinking scroll markers appear above/below when entries precede `first` or follow the last drawn one. */
void evtDrawMenuFrame(sdfDrawTag_t *list, s32 x, s32 y, s32 width, s32 rows, s32 first, s32 total, void *callbackContext,
                   EvtMenuHeaderFn header, EvtMenuRowFn row);

s32 evtAppendValueChangeDebugLabel(sdfDrawTag_t *list, s32 x, s32 y);

extern char D_003ADE00[];

extern char D_003ADE10[];

extern char D_003ADE28[];

extern char D_003ADE40[];

extern char D_003ADE50[];

void evtDrawFloatValueEditorRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx);

extern sdfGrOt_t sdfDebugGrOt;

extern EvtPad dds3PadButtonState;

extern void evtDrawFloatValueEditorRow();

/* Edit the bounded float only in mode 8. Confirm precedes cancel; coarse steps
 * replace fine steps before the value is clamped to the runtime limits. */
s32 evtViewerFloatValueUpdate(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

s32 evtDrawValueChangeNoticeRow(sdfDrawTag_t *list, s32 x, s32 y);

extern char D_003ADE40[];

extern char D_003ADE50[];

extern char D_003ADE78[];

void evtDrawValueChangeInstructionRow(sdfDrawTag_t *list, s32 x, s32 y, u32 index, dds_evtViewerInfo_t *ctx);

s32 evtUpdateValueChangeDialog(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

s32 evtFrameChangeHead(sdfDrawTag_t *target, s32 x, s32 y);

void evtViewerDrawFrameChangeRow(sdfDrawTag_t *list, s32 x, s32 y, u32 index, dds_evtViewerInfo_t *ctx);

s32 evtViewerFrameChangeUpdate(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

extern const char *D_003BC070[];

extern const char *D_003AE0D8[];

extern char D_003BC080[];

extern char D_003BC088[];

/* Draw a project-menu command, including its current toggle/skip option. */
void evtDrawProjectCommandRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx);

extern void evtDrawEntrySelectionRow();

extern s32 dds3SelectMenu(s32 padSet, s32 secondaryCount, s32 primaryCount, s32 secondaryVisible,
                                 s32 primaryVisible, s32 *secondaryScroll, s32 *primaryScroll,
                                 s32 *secondaryCursor, s32 *primaryCursor);

s32 mnuDrawInfoWindowA(s32 x, s32 y, dds_evtViewerInfo_t *work);

extern char D_003BC090[];

extern char D_003BC0A0[];

extern s8 D_003688B8[][12];

/* Draw the runtime title when present; return two rows used, or zero for no title. */
s32 evtDrawStringEntry(sdfDrawTag_t *output, s32 x, s32 y, dds_evtViewerInfo_t *work);

/* Draw an in-range item name, distinguishing selected active and inactive rows. */
void evtDrawSelectableTextRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *work);

/* Draw a title-sized selection dialog; only action mode 2 advances its cursor. */
s32 evtUpdateTextSelectionDialog(s32 x, s32 y, dds_evtViewerInfo_t *work);

s32 evtDrawInputValueRow(sdfDrawTag_t *list, s32 x, s32 y, u8 *ctx);

/* Draw eleven characters from a twelve-byte keyboard row and highlight the
 * runtime character selection, using mode 3 for the active color. */
void evtDrawKeyboardRow(sdfDrawTag_t *list, s32 xPosition, s32 y, s32 row, dds_evtViewerInfo_t *work);

/* Draw the keyboard and edit the eight-character name when input is active. */
s32 evtUpdateNameEntryDialog(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

s32 evtDrawEventFileNameRow(sdfDrawTag_t *list, s32 x, s32 y);

void evtDrawEventCutSelectRow(sdfDrawTag_t *list, s32 x, s32 y, u32 index, dds_evtViewerInfo_t *ctx);

s32 evtUpdateEventCutSelectDialog(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

void evtDrawSelectedEntryLabel(sdfDrawTag_t *list, EvtRuntimeGroup *selected, s32 x,
                               s32 unused, dds_evtViewerInfo_t *runtime);

s32 evtUpdateEntrySelectionDialog(s32 x, s32 y, dds_evtViewerInfo_t *work);

extern const char *D_003686F8[];

extern s8 D_00368750[];

extern char D_003BC0D0[];

/* Header row of the frame list: one text cell per column of the selected group's table row. */
s32 evtDrawFrameListColumnHeadings(sdfDrawTag_t *list, s32 x, s32 y, dds_evtViewerInfo_t *ctx);

extern char D_003BC160[];

extern char D_003BC168[];

extern char D_003BC170[];

extern char D_003BC178[];

extern char D_003BC180[];

extern char D_003BC188[];

extern char D_003BC190[];

extern char D_003BC198[];

extern char D_003BC1A0[];

extern char D_003BC1A8[];

extern char D_003BC1B0[];

extern char D_003BC1B8[];

extern char D_003BC1C0[];

extern char D_003BC1C8[];

extern char D_003BC1D0[];

extern char D_003BC1D8[];

extern char D_003BC1E0[];

extern char D_003BC1E8[];

extern char D_003BC1F0[];

extern char D_003BC1F8[];

extern char D_003BC200[];

extern char D_003BC208[];

extern char D_003BC210[];

extern char D_003BC218[];

extern char D_003BC220[];

extern char D_003BC228[];

extern char D_003BC230[];

extern char D_003BC238[];

extern s8 D_00368750[];

extern char *D_003688E8[];

extern char *D_003688F8[];

extern char *D_00368918[];

extern char *D_00368938[];

extern char *D_00368940[];

extern char *D_00368948[];

void evtDrawFrameListKeyValues(sdfDrawTag_t *list, s32 x, s32 y, s32 color,
                   EvtRuntimeChild *node, dds_evtViewerInfo_t *ctx);

extern void evtDrawFrameListKeyValues(sdfDrawTag_t *list, s32 x, s32 y, s32 color, EvtRuntimeChild *node, dds_evtViewerInfo_t *ctx);

/* Draw an indexed child of the selected group, or its trailing new-frame row. */
s32 evtFrameListItem(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx);

s32 evtUpdateFrameListDialog(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

#include "dds/dds3_world_selection.h"

void evtViewerDrawWorldNodeRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx);

s32 evtUpdateWorldNodeListDialog(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

extern char D_003BC240[];

extern char D_003BC248[];

void evtViewerDrawPendingNodeRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx);

extern void evtViewerDrawPendingNodeRow();

/* Draw the pending-node selector for group types 20/21; other types return -1. */
s32 mnuDrawInfoWindowB(s32 x, s32 y, dds_evtViewerInfo_t *work);

/* Draw the message count from the runtime window context
 * and its entry handle, returning two rows used. */
s32 mnuDrawMessageMenuLabel(sdfDrawTag_t *list, s32 x, s32 y, dds_evtViewerInfo_t *work);

extern char D_003BC250[];

extern char D_003BC258[];

extern char D_003BC260[];

extern char D_003AE8D0[];

extern char D_003AE8E0[];

extern char D_003AE8F0[];

extern char D_003AE900[];

extern char D_003AE910[];

extern char D_003AE920[];

extern char D_003AE930[];

extern char D_003AE940[];

extern char D_003AE950[];

extern char D_003AE960[];

void evtDrawMessageDataRow(sdfDrawTag_t *list, s32 x, s32 y, u32 kind, dds_evtViewerInfo_t *ctx);

s32 evtUpdateMessageValueDialog(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

s32 mnuDrawCutFlagLabel(sdfDrawTag_t *target, s32 x, s32 y);

/* Draw the packed four-bit option or twelve-bit number and highlight
 * the selected comparison field; later rows provide input instructions. */
void evtDrawComparisonValueRow(sdfDrawTag_t *list, s32 x, s32 y, u32 index, dds_evtViewerInfo_t *ctx);

s32 evtUpdateComparisonValueDialog(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

EvtMenuTableEntryAvailability evtIsMenuTableEntryEnabled(s32 *index);

/* Return the table value selected by this group's type and editor column. */
s32 mnuGetSelectedTableValue(dds_evtViewerInfo_t *runtime);

extern char *D_00368AA0[];

extern char *D_00368AB0[];

extern char D_003BC290[];

extern char D_003BC298[];

extern char D_003BC2A0[];

extern s8 D_003BC268[3];

void evtDrawGroupPropertyTable(sdfDrawTag_t *list, s32 x, s32 y, s32 hidden, dds_evtViewerInfo_t *runtime);

extern char *D_00368AC0[];

s32 evtHandleGroupPropertyMenu(s32 x, s32 y, dds_evtViewerInfo_t *runtime);

s32 mnuDrawMotionChangeLabel(sdfDrawTag_t *target, s32 x, s32 y);

typedef struct EvtMotionBits {
    s32 group : 8;
    s32 motion : 8;
    s32 loop : 8;
    s32 hokan : 8;
} EvtMotionBits;

typedef union EvtMotionValue {
    s32 word;
    EvtMotionBits bits;
} EvtMotionValue;

extern char D_003BC2A8[];

extern char D_003BBFF0[];

void evtDrawMotionChangeRow(sdfDrawTag_t *list, s32 x, s32 y, s32 row, dds_evtViewerInfo_t *ctx);

extern void evtDrawMotionChangeRow();

/* sdfMotionCtrlr_t editor row: ctx->value packs group (byte 0), motion number (byte 1), loop flag (byte 2) and interpolation (byte 3). */
s32 evtUpdateMotionChangeRow(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

extern char D_003BC288[];

extern char D_003BC280[];

extern char D_003AEA80[];

void evtDrawGroupListRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx);

s32 evtUpdateGroupListDialog(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

/* Empty RGB-editor row renderer passed to evtDrawMenuFrame. */
void evtNoOpRGBEditorRow(sdfDrawTag_t *list, s32 x, s32 y, s32 row, void *runtime);

extern f32 sdfViewMatrix[];

extern void FromRotation(f32 angle);

/* vu0 routine: apply the selected camera-axis rotations to a position. */
void evtRotateViewerPositionFromPad(f32 *position);

extern u16 D_00368AC8[];

extern char D_003BC2C0[];

extern char D_003BC2C8[];

extern const char D_003AEB40[];

extern const char D_003AEB50[];

extern const char D_003AEB60[];

extern void *dds3PrimLine3D(s32, s32, s32, s32);

extern void dds3PrimDebugTextBox(void *, s32, s32, s32, f32 *);

extern void mnuShopSavePrimaryTransform(dds_evtViewerInfo_t *);

extern void mnuShopSaveFullTransform(dds_evtViewerInfo_t *);

extern void mnuShopRestoreTransform(dds_evtViewerInfo_t *);

extern void dds3SetLightColor(s32, s32, const f32 *);

extern void dds3SetLightDir(s32, s32, const f32 *);

extern void dds3SetAmbientColor(s32, f32 *);

extern void evtSetSlotVectors(s32, EvtUnitVectorSlotState, evtUnitHandle_t *, const f32 *, const f32 *, const f32 *);

/* Select and draw one light-color bank at the current panel offset. */
static inline void evtDrawLightRgbComponents(sdfDrawTag_t *list, dds_evtViewerInfo_t *runtime,
                                             s32 selected, s32 y, s32 firstComponent) {
    if ((u32)selected >= 3U || runtime->lightEditorEditMode != 1) {
        selected = -1;
    }
    dds3PrimDebugTextBox(list,
                     (s32)(0x8740u + ((u32)runtime->lightPanelOffset << 4)),
                     y, selected, &runtime->commandMatrix[firstComponent]);
}

/* Convert the panel offset into the fixed-point X origin used by captions. */
static inline s32 evtLightCaptionX(s32 panelOffset) {
    return (s32)(0x8740u + ((u32)panelOffset << 4));
}

s32 evtUpdateLightAndBackgroundEditor(s32 x, s32 y, dds_evtViewerInfo_t *runtime);

/* The editor draws its own fields; the menu frame's row callback stays blank. */
void evtNoOpXYSlotVectorEditorRow(void);

/* This coordinate table is supplied by the ordinary data segment. */
extern const u16 D_00368AE0[];

extern const EvtFloatPanelRow D_00368AE8[4];

extern char D_003BC2C0[];

extern char D_003BC2E8[];

extern char D_003BC2F0[];

extern char D_003BC2F8[];

extern char D_003BC188[];

extern u8 corePktSoftenBias;

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

extern void evtSetSlotVector(s32 vectorSlot, f32 x, f32 y);

/* Draw the XY/overlay editor; cancel restores the captured values and flag. */
s32 evtDrawAndHandleXYSlotVectorEditor(s32 x, s32 y, dds_evtViewerInfo_t *runtime);

/* Draw command text for row 0 or 1; null texts and other rows emit nothing. */
void evtDrawOptionalPromptText(sdfDrawTag_t *list, s32 x, s32 y, s32 kind, dds_evtViewerInfo_t *work);

/* In mode 0x14, count down before checking input: zero expires, negative waits
 * indefinitely, and confirm takes precedence over cancel. */
s32 mnuDrawTimedPrompt(s32 x, s32 y, dds_evtViewerInfo_t *work);

/* Store a countdown and the two optional strings used by timed prompts. */
void evtSetRuntimeCommandValues(dds_evtViewerInfo_t *runtime, s32 frames, char *firstText, char *secondText);

extern char D_003BC300[];

void evtViewerDrawGroupRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx);

s32 evtViewerDrawGroupWindow(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

extern char *D_00368B18[];

void evtDrawShadowSettingsRow(sdfDrawTag_t *list, s32 x, s32 y, s32 index, dds_evtViewerInfo_t *ctx);

/* Shadow-configuration menu: draw it, move the row cursor, and step the
 * selected row's mode, alpha or Y offset; confirm returns 1, cancel -1. */
s32 evtViewerEditShadowProperties(s32 x, s32 y, dds_evtViewerInfo_t *ctx);

typedef struct EvtSelectionCache {
    u8 pad00[0x24];
    s32 selectedEntry; /* 0x24 */
} EvtSelectionCache;

extern EvtSelectionCache *D_003BB128;

extern s32 evtActiveEntryFlags;

/* Cache nonzero selections and publish first-tick flags at timer zero. A zero
 * setup status clears runtime control and finishes the polling task. */
dds3TaskUpdateResult_t evtSynchronizeSelectedEntry(dds3ProcessID_t *task);

/* Poll solid-rectangle setup until it clears runtime control and finishes. */
dds3TaskUpdateResult_t evtPollRuntimeControlReady(dds3ProcessID_t *task);

typedef char EvtViewKey_size_must_be_0x38[(sizeof(EvtRuntimeChild) == 0x38) ? 1 : -1];

extern void dds3PrimDebugTextBoxRGB(void *packetList, s32 x, s32 y,
                                   s32 selected, u32 color,
                                   s32 showNormalized);

extern char D_003BC318[];

extern char D_003BC320[];

extern char D_003BC328[];

extern char D_003BC330[];

extern char D_003BC338[];

extern char D_003BC340[];

extern char D_003AEC00[];

extern char D_003AEC10[];

extern char D_003AEC20[];

extern char D_003AEC30[];

s32 evtEditEffectSpriteFrame(dds_evtViewerInfo_t *runtime);

typedef char EvtCameraColorPayload_size_must_be_0x40[
    (sizeof(EvtCameraColorPayload) == 0x40) ? 1 : -1];

s32 evtEditCameraColorKeyFrame(dds_evtViewerInfo_t *runtime);

extern s32 evtEditEffectSpriteFrame(dds_evtViewerInfo_t *runtime);

extern s32 evtEditCameraColorKeyFrame(dds_evtViewerInfo_t *runtime);

extern void *D_003BB0C0;

extern void *D_003BB14C;

extern void *D_003BB130;

s32 evtPollEffectFrameControl(s32 x, s32 y, dds_evtViewerInfo_t *runtime);

extern s32 (*D_00368B48[])(s32, s32, dds_evtViewerInfo_t *);

s32 evtDispatchActionByIndex(s32 actionIndex, s32 x, s32 y, dds_evtViewerInfo_t *runtime);

void evtWriteSerializedBytes(s32 output, void *data, s32 size);

s32 evtAssignRuntimeChildSequenceAndCount(dds_evtViewerInfo_t *runtime);

s32 evtIndexGroupTypeElevenChildren(dds_evtViewerInfo_t *runtime);

s32 evtIndexGroupTypeThirteenChildren(dds_evtViewerInfo_t *runtime);

s32 evtIndexGroupTypeFourteenChildren(dds_evtViewerInfo_t *runtime);

s32 evtIndexGroupTypeFifteenChildren(dds_evtViewerInfo_t *runtime);

s32 evtIndexGroupTypeTwentyThreeChildren(dds_evtViewerInfo_t *runtime);

s32 evtIndexGroupTypeTwentySevenChildren(dds_evtViewerInfo_t *runtime);

s32 evtIndexGroupTypeSixteenChildren(dds_evtViewerInfo_t *runtime);

s32 evtIndexGroupTypeSeventeenChildren(dds_evtViewerInfo_t *runtime);

s32 evtIndexGroupTypeTwentyFiveChildren(dds_evtViewerInfo_t *runtime);

extern s32 evtEventViewerCountEntriesById(s32 id, dds_evtViewerInfo_t *runtime);

extern s32 evtEventViewerCountEntries(dds_evtViewerInfo_t *runtime);

extern s32 evtEventViewerSumNodeCounts(EvtViewerTrackFileFormat format, dds_evtViewerInfo_t *runtime);

/* Fixed prefix shared by PMD directory streams. */
typedef struct EvtPmdFilePrefix {
    u8 reserved00;
    u8 reserved01;
    u16 reserved02;
    s32 byteLength;
    char tag[4];
    s32 reserved0C;
    s32 count;
    s32 kind;
    s32 reserved18;
    s32 reserved1C;
} EvtPmdFilePrefix;

typedef char EvtPmdFilePrefix_size_check[sizeof(EvtPmdFilePrefix) == 0x20 ? 1 : -1];

/* Build the directory after assigning each payload its serialized index. */
void evtViewerWriteTrackDirectory(s32 output, EvtViewerTrackFileFormat format, dds_evtViewerInfo_t *runtime);

/* Emit first, range, third and metadata words in file order. The range word
 * is written whole, not narrowed to the terminal halfword used by child spans. */
void evtWriteRuntimeHeaderValues(s32 output, dds_evtViewerInfo_t *state);

/* Filter groups by file format and serialize each child body. Type-8 spans use the
 * next start or terminal range halfword, unless their body marker disables them. */
void evtViewerWriteChildRecords(s32 output, EvtViewerTrackFileFormat format, dds_evtViewerInfo_t *runtime);

/* Emit entryTotal consecutive thirty-two-byte names; nonpositive totals emit nothing. */
void evtWriteFixedSizeEntries(s32 output, dds_evtViewerInfo_t *table);

void evtWriteGroupHeader(s32 output, dds_evtViewerInfo_t *runtime);

void evtCopyRuntimeChildPayloadsToBuffer(s32 output, dds_evtViewerInfo_t *runtime);

void evtEmitGroupTypeElevenPayloads(s32 output, dds_evtViewerInfo_t *runtime);

void evtEmitGroupTypeThirteenPayloads(s32 output, dds_evtViewerInfo_t *runtime);

void evtEmitGroupTypeFourteenPayloads(s32 output, dds_evtViewerInfo_t *runtime);

void evtEmitGroupTypeFifteenPayloads(s32 output, dds_evtViewerInfo_t *runtime);

void evtEmitGroupTypeTwentyThreePayloads(s32 output, dds_evtViewerInfo_t *runtime);

void evtEmitGroupTypeTwentySevenPayloads(s32 output, dds_evtViewerInfo_t *runtime);

void evtEmitGroupTypeSixteenPayloads(s32 output, dds_evtViewerInfo_t *runtime);

void evtEmitGroupTypeSeventeenPayloads(s32 output, dds_evtViewerInfo_t *runtime);

void evtWriteGroupMetadata(s32 output, dds_evtViewerInfo_t *runtime);

void evtEmitGroupTypeTwentyFivePayloads(s32 output, dds_evtViewerInfo_t *runtime);

extern u8 sdfHddValid;

extern void *sdfFileGetHostRoot(void);

extern char D_003BC350[];

extern char D_003BC358[];

s32 evtViewerSaveTrackFiles(s32 mode, dds_evtViewerInfo_t *runtime);

u16 evtGetRowValue(const PolyMovieWork *work, s32 row);

s16 evtGetRowVariant(const PolyMovieWork *work, s32 row);

u16 evtGetRowParameter(const PolyMovieWork *work, s32 row);

u16 evtGetRowFlags(const PolyMovieWork *work, s32 row);

const u8 * evtGetRowPayloadAddress(const PolyMovieWork *work, s32 row);

void evtResolveLinkGroupIndex(PolyMovieWork *src, dds_evtViewerInfo_t *runtime, EvtGroupMetadata *link);

#include "dds/dds3obj.h"

#include "dds/dat_state.h"

#include "dds/sdf_texture_offset_list.h"

/* PM1 section 2 and section 3 use the same 0x20-byte package record. */
typedef struct EvtViewerPackageRecord {
    s32 nameIndex;
    s32 reserved04;
    s32 packageId;
    s32 partId;
    u32 offset;
    s32 length;
    s32 loadOrder;
    s32 reserved1C;
} EvtViewerPackageRecord;

/* PM1 auxiliary and texture section entries each occupy 0x10 bytes. */
typedef struct EvtViewerResourceRecord {
    s32 nameIndex;
    u32 offset;
    s32 kind;
    s32 reserved0C;
} EvtViewerResourceRecord;

/* The section-21 wire record is also the group's link subobject at +0x18. */
typedef char EvtViewerPackageRecord_size[(sizeof(EvtViewerPackageRecord) == 0x20) ? 1 : -1];

typedef char EvtViewerResourceRecord_size[(sizeof(EvtViewerResourceRecord) == 0x10) ? 1 : -1];

extern s32 evtLoadArea_FromMemory(s32 area, s32 room, u8 *resource0Data, u8 *resource1Data,
                                            const SdfTextureOffsetListHeader *textureOffsets, s32 flags);

extern EvtRuntimeGroup *evtEventViewerCreateEntry(s32 kind, dds_evtViewerInfo_t *runtime);

extern EvtViewerNameIndex evtEventViewerAddName(const char *name, dds_evtViewerInfo_t *runtime);

extern void mdlPackDecode(s32 packageId, s32 partId, s32 flags, const u8 *data, s32 length);

extern s32 mdlSpawnCameraSlotViewerObject(s32 packageId, s32 partId);

extern evtUnitHandle_t *evtGetUnitHandle(EvtUnitObjectKey worldObjectKey);

extern EvtPathLinkResult evtPathLinkAndMake(dds3Basic_t *object, dds3Basic_t *owner);

extern EvtRuntimeChild *evtCreateTrackKeyAtFrame(EvtRuntimeGroup *group, s32 frame, dds_evtViewerInfo_t *runtime);

extern EvtPackedSoundTrackId evtLoadEventBgm(s32 eventId);

extern EvtEventBgmReadiness evtCheckEventBgmLoaded(s32 eventId);

extern u8 fldTitleIsActive(void);

extern void fldStartTitle(s32 field, s32 mode, s32 option);

extern void dds3FadeBackgroundStartIn(s32 frames);

typedef struct { u8 bytes[0x18]; } EvtRowPayload18;

typedef struct { u8 bytes[0x20]; } EvtRowPayload20;

typedef struct { u8 bytes[0x24]; } EvtRowPayload24;

typedef struct { u8 bytes[0x28]; } EvtRowPayload28;

typedef struct { u8 bytes[0x2C]; } EvtRowPayload2C;

typedef struct { u8 bytes[0x30]; } EvtRowPayload30;

typedef struct { u8 bytes[0x40]; } EvtRowPayload40;

extern EvtRuntimeChild *evtCreateTrackKeyAtFrame(EvtRuntimeGroup *, s32, dds_evtViewerInfo_t *);

extern EvtRuntimeGroup *evtEventViewerCreateEntry(s32, dds_evtViewerInfo_t *);

extern EvtViewerNameIndex evtEventViewerAddName(const char *, dds_evtViewerInfo_t *);

extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *, f32);

extern EvtViewerNameIndex mnuCampFindMatchingEntryIndex(PolyMovieWork *, dds_evtViewerInfo_t *, s32);

s32 evtPopulateViewerTimelineFromMovie(PolyMovieWork *work, dds_evtViewerInfo_t *runtime);

extern void mnuReleaseCampSceneRegisteredIds(dds_evtViewerInfo_t *runtime);

extern void movieStop(void);

extern void dds3FreeRenderTexture(void);

extern u32 dds3DrawFlag;

extern s32 evtFormatPolygonMoviePaths(s32 eventId, s32 sceneId, char *path0, char *path1, char *path2);

extern void evtFreeStage(void);

EvtViewerReloadResult evtReloadEventViewer(EvtPolygonMovieResourceMode resourceMode, dds_evtViewerInfo_t *runtime);

EvtPackedSoundTrackId evtGetSoundIndex(s32 eventId, EvtSoundVariation variation);

EvtPackedSoundTrackId evtLoadEventBgm(s32 eventId);

EvtEventBgmReadiness evtCheckEventBgmLoaded(s32 eventId);

EvtPackedSoundTrackId evtPlayEventBgm(s32 eventId, EvtSoundVariation variation);

void evtTransEventBgm(s32 eventId, EvtSoundVariation variation);

EvtPackedSoundTrackId evtFadeInEventBgm(s32 eventId, EvtSoundVariation variation);

EvtPackedSoundTrackId evtStopEventBgm(s32 eventId, EvtSoundVariation variation);

EvtPackedSoundTrackId evtPlaySe(s32 eventId, EvtSoundVariation variation);

EvtPackedSoundTrackId evtStartBgmBySoundIdAndFade(s32 eventId, EvtSoundVariation variation);

void evtMakeEventPackName(EvtPackEventId eventId, char *buffer);

dds3ProcessID_t *evtSearchEventPack(EvtPackEventId eventId);

/* Return its load phase, or EVT_PACK_LOAD_NOT_FOUND when the task is absent. */
EvtPackLoadStatus evtGetEventPackStat(EvtPackEventId eventId);

EvtPackLoadState *evtGetPackProcessData(EvtPackEventId eventId);

void *evtGetEventPackResourceAddress(EvtPackEventId eventId, EvtPackSecondaryResourceId secondaryResourceId);

extern void Eve_DistortionBlurSetTex(sdfTexHandle_t *);

void evtRefreshTaskEffectTexture(EvtPackEventId eventId, EvtPackSecondaryResourceId key);

extern EvtModelScaleEntry *D_003BAA10;

extern EvtModelScaleEntry *D_003BAA20;

/* Spawn from a keyed task resource with two zeroed constructor vectors.
 * A nonnegative index applies the selected model-table scale relative to the base. */
dds3Basic_t *evtSpawnResourceObject(EvtPackEventId eventId, EvtPackSecondaryResourceId key, s32 index);
/* End of declarations from src/dds1/game/code_00235270.c */

INCLUDE_ASM(const s32, "code_001DAC80", func_001DAC80);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DB0E8);

s32 evtDispatchActionByIndex(s32 actionIndex, s32 x, s32 y, dds_evtViewerInfo_t *runtime) {
    s32 mode = runtime->actionMode;
    if (mode == 11 && actionIndex != mode) {
        return 0;
    }
    return D_00368B48[actionIndex](x, y, runtime);
}

void evtWriteSerializedBytes(s32 output, void *data, s32 size) {
    sceWrite(output, data, size);
}

s32 evtAssignRuntimeChildSequenceAndCount(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeAIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeElevenChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->duration = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeThirteenChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeFourteenChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeFifteenChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeTwentyThreeChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeTwentySevenChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeSixteenChildren(dds_evtViewerInfo_t *runtime) {
    s32 childIndex = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = childIndex++;
            }
        }
    }
    return childIndex;
}

s32 evtIndexGroupTypeSeventeenChildren(dds_evtViewerInfo_t *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = index++;
            }
        }
    }
    return index;
}

s32 evtIndexGroupTypeTwentyFiveChildren(dds_evtViewerInfo_t *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeIndex = index++;
            }
        }
    }
    return index;
}

INCLUDE_ASM(const s32, "code_001DAC80", func_001DB6B0);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DBC38);

/* Filter groups by file format and serialize each child body. Type-8 spans use the
 * next start or terminal range halfword, unless their body marker disables them. */
void evtViewerWriteChildRecords(s32 output, EvtViewerTrackFileFormat format, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;

    for (group = runtime->groups; group != NULL; group = group->next) {
        EvtRuntimeChild *child = group->children;

        if (format == EVT_VIEWER_TRACK_FILE_FORMAT_PM2) {
            if (group->type == 5 || group->type == 0x13) {
                continue;
            }
        } else if (format == EVT_VIEWER_TRACK_FILE_FORMAT_PM3) {
            if (group->type != 5 && group->type != 0x13) {
                continue;
            }
        }

        for (; child != NULL; child = child->next) {
            EvtSerializedChild record;
            s32 i;
            u16 value;

            value = (u16)child->serializedValue;
            record.groupType = group->type;
            record.start = child->frame;
            record.span = child->duration;
            record.value = value;
            for (i = 0; i < 8; i++) {
                record.body[i] = child->body.words[i];
            }
            if (group->type == 8) {
                if (child->body.words[0] == 0) {
                    record.span = 0;
                } else {
                    if (child->next != NULL) {
                        record.span = child->next->frame - child->frame;
                    } else {
                        record.span = runtime->frameRange.f.end - child->frame;
                    }
                }
            }
            evtWriteSerializedBytes(output, &record, sizeof(record));
        }
    }
}

/* Emit entryTotal consecutive thirty-two-byte names; nonpositive totals emit nothing. */
void evtWriteFixedSizeEntries(s32 output, dds_evtViewerInfo_t *table) {
    void *entry;
    s32 index;

    index = 0;
    if (0 < table->entryTotal) {
        entry = table->entryName;
        do {
            evtWriteSerializedBytes(output, entry, 0x20);
            index = index + 1;
            entry = (void *)((s32)entry + 0x20);
        } while (index < table->entryTotal);
    }
}

INCLUDE_ASM(const s32, "code_001DAC80", func_001DBE88);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DBF00);

void evtEmitGroupTypeElevenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x20);
            }
        }
    }
}

void evtEmitGroupTypeThirteenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x28);
            }
        }
    }
}

void evtEmitGroupTypeFourteenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x2C);
            }
        }
    }
}

void evtEmitGroupTypeFifteenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x2C);
            }
        }
    }
}

void evtEmitGroupTypeTwentyThreePayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x2C);
            }
        }
    }
}

void evtEmitGroupTypeTwentySevenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x28);
            }
        }
    }
}

void evtEmitGroupTypeSixteenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x18);
            }
        }
    }
}

void evtEmitGroupTypeSeventeenPayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x24);
            }
        }
    }
}

void evtWriteGroupMetadata(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        u8 header[8];
        header[0] = *(u8 *)group;
        header[1] = group->metadataFlag;
        *(u16 *)&header[2] = (u16)group->entryHeader;
        *(u16 *)&header[4] = group->metadata.value;
        header[6] = group->metadata.option;
        header[7] = group->metadata.extra2;
        evtWriteSerializedBytes(output, header, sizeof(header));
    }
}

void evtEmitGroupTypeTwentyFivePayloads(s32 output, dds_evtViewerInfo_t *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                evtWriteSerializedBytes(output, child->payload, 0x40);
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_001DAC80", func_001DC4D0);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DCB88);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DCBD8);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DCC28);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DCC78);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DCCC8);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DCD18);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DCE10);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DD360);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DD440);

INCLUDE_ASM(const s32, "code_001DAC80", func_001DF938);
INCLUDE_SDATA(const s32, "code_001DAC80", D_004DFEF0);

INCLUDE_SDATA(const s32, "code_001DAC80", D_004DFEF8);

INCLUDE_SDATA(const s32, "code_001DAC80", D_004DFF00);

INCLUDE_SDATA(const s32, "code_001DAC80", D_004DFF08);

INCLUDE_SDATA(const s32, "code_001DAC80", D_004DFF10);

INCLUDE_SDATA(const s32, "code_001DAC80", D_004DFF18);

