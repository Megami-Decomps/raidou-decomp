#include "common.h"

/* Declarations from smt3-decomp src/smt3/event/evtEventViewer.c */
#include "common.h"

#include "basic.h"

#include "event.h"

#include "evtPolygonMovie.h"

#include "effect.h"

#include "kernel_draw.h"

#include "ps2asm.h"

#include "kernel.h"

#include "model.h"

#include "motion.h"

/* The viewer's current menu, which EnterMain switches on to decide what Enter does
 * from each one. Names from the IDB. */
typedef enum EVT_VIEWER_MENU {
    EVT_VIEWER_MENU_NON = 0,
    EVT_VIEWER_MENU_MAIN = 1,
    EVT_VIEWER_MENU_SELECT = 2,
    EVT_VIEWER_MENU_FILENAME = 3,
    EVT_VIEWER_MENU_OBJLIST = 4,
    EVT_VIEWER_MENU_FRAMELIST = 5,
    EVT_VIEWER_MENU_FRAMECHANGE = 6,
    EVT_VIEWER_MENU_INTCHANGE = 7,
    EVT_VIEWER_MENU_FLOATCHANGE = 8,
    EVT_VIEWER_MENU_FILENAME2 = 9,
    EVT_VIEWER_MENU_BASICLIST = 10,
    EVT_VIEWER_MENU_DATA = 11,
    EVT_VIEWER_MENU_EVENTPOS = 12,
    EVT_VIEWER_MENU_MAX = 13
} EVT_VIEWER_MENU;

extern evtFrameInfo_t *evtAddFrameInfo(evtViewerObject_s *track, s32 frame);

extern void sdfChipFreeDirect(void *aPtr);

extern void dds3DeleteSceneBasicData(void *basic);

extern void evtInitViewerInfo(void);

extern s32 dds3KillProcessName(const char *name, s32 shutdowntimer);

extern evtViewerInfo_t *viewerinfo;

extern void *memset(void *a, s32 b, size_t c);

extern void *sdfChipAllocDirect(s32 aSize);

extern void evtFreePolygonMovieWorkData(void *wdata);

extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);

extern void *dds3AddBEDEffectObjectBasic2(void *data, f32 *pos, f32 *rot);

extern void dds3EffectObjectBasicStatOn(void *basic, s32 s);

extern void dds3StartBEDEffectObjectBasic(void *basic, s32 cutno);

extern void LinkEffectBasic(dds3Basic_t *eff, s32 nameindex, s32 posindex, s32 baseframe);

extern dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name);

extern void dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *target);

extern s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target, s32 index);

extern s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj);

extern dds3ObjectBaseControlWorkData_t *dds3GetBaseControlWorkData(dds3Basic_t *pObjectBasic);

typedef struct polyMovieObject_t polyMovieObject_t;

extern s32 evtStopMover(polyMovieObject_t *mover, s32 sw, s32 baseframe, s32 nowframe);

extern const char evtViewerPathLinkMsg[];

extern s8 evtFrameListFieldTable[];

extern s32 PopMenu(void);

extern s32 strcmp(const char *a, const char *b);

extern char *strcpy(char *dst, const char *src);

extern void evtRunFrame(s32 nowframe);

extern dds3ProcessID_t *evtInitFrameVar(void);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void evtFreeStage(void);

extern s32 sdfDelayedResrcCheck(void);

extern void evtFreeViewerInfo(void);

extern void **dds3GetProcessData(void *id);

extern void sdfMemFreeDirect(void *aHandle);

extern s32 SearchNameTable(const char *name);

extern void evtStopFrame(s32 sw, u32 nowframe);

extern void *dds3SetActiveCameraSceneBasic(void *pSceneBasic, void *pCameraBasic);

extern f32 dds3CameraObjectBasicGetFovy(void *pBasic);

extern void dds3SetScreenFovy(f32 fovy);

extern void evtRunUnitMode(dds3Basic_t *unit, s32 mode);

extern void *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(void *aHandle);

extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
                                         s32 shutdownTimer, dds3ProcessFunc_t process,
                                         dds3ProcessFunc_t shutdown, void *data);

extern dds3ProcessID_t *evtInitSky(void);

extern u32 dds3BackColor;

extern void PushMenu(s32 no, s32 x, s32 y);

extern void DelEffectTable_evtEventViewer(s32 index);

extern void DelBED(evtFrameInfo_t *frame);

extern s8 dds3PadButtonState[];

extern effScreenDrawParams_t *evtGetUnityCalcArea(void);

extern void dds3EffectBlur(void *area);

extern void Eve_PartialMultiBlurON(void);

extern void Eve_PartialMultiBlurOFF(void);

extern void Eve_PartialMultiFilterON(void);

extern void Eve_PartialMultiFilterOFF(void);

extern void Eve_DistortionBlurON(void);

extern void Eve_DistortionBlurOFF(void);

extern void Eve_RippleBlurON(void);

extern void Eve_RippleBlurOFF(void);

extern void Eve_FilterON(void);

extern void Eve_FilterOFF(void);

extern void evtMakeViewerPath(s32 kind);

extern void evtViewerShutdown(s32 kind);

extern u16 evt_file_minor;

extern u16 evtFileMajor;

extern void evtViewerStopFrame(void);

#include "sdk_libc.h"

extern void mdlUnRegist(s32 aMajor, s32 aMinor);

extern s32 evtViewerFrameSetMenuTable[];

extern s32 capwait;

extern void DrawEffect(void);

extern void FrameSelect(void);

extern dds3Basic_t *dds3GetActiveScene(void);

extern char *evtViewerFreeCameraName;

extern char *evtViewerGameCameraName;

extern char evtViewerCameraNameFormat[];

extern sdfGrOt_t sdfGrOtTable[];

extern sdfGrOt_t sdfDebugGrOt;

extern void *dds3FontReqAllocDrawTag(void);

extern s32 sdfDevPrintf(s32 aX, s32 aY, u32 aZ, s32 aAttr, const char *aStr, ...);

extern void sdfDrawTagLink(void *aTag, addr_t aPacket);

extern void SetActiveCamera(void);

extern saddr_t evtEventViewerProcess3(void);

extern saddr_t evtEventViewerProcess4(void);

void LinkViewerObject(evtViewerObject_s *obj);

void UnlinkViewerObject(evtViewerObject_s *obj);

void DelFrameInfo(evtViewerObject_s *parent, evtFrameInfo_t *f);

void DelViewerObject(evtViewerObject_s *obj);

/* Insert after existing equal keys, keeping ascending order and incrementing count. */
void LinkFrameInfo(evtViewerObject_s *parent, evtFrameInfo_t *f);

/* Unlink an attached node, clear its links, and decrement the owning list's count. */
void UnlinkFrameInfo(evtViewerObject_s *parent, evtFrameInfo_t *f);

/* Walk the linked list and reinsert the first out-of-order successor. */
void SortFrameInfo(evtViewerObject_s *parent);

evtFrameInfo_t *AddFrameInfo2(evtViewerObject_s *group, s32 frame, evtViewerInfo_t *viewer);

evtFrameInfo_t *evtGetSelectFrameInfo(void);

/* Release a queued node from its entry, freeing its buffer unless the entry id is 0x12. */
void DelFrameInfo(evtViewerObject_s *parent, evtFrameInfo_t *f);

void DelFrameInfo2(void);

/* Insert the track in ascending type order and bump the viewer's track count. */
void LinkViewerObject(evtViewerObject_s *obj);

void UnlinkViewerObject(evtViewerObject_s *obj);

evtViewerObject_s *evtAddViewerObject(s32 type);

s32 evtGetViewerObjectTypeCount(s32 type);

s32 evtGetAllFrameInfoCount(void);

void DelViewerObject(evtViewerObject_s *obj);

void evtSetFirstFrame(void);

void evtSetLastFrame(void);

void evtInitViewerInfo(void);

void evtFreeViewerInfo(void);

void evtUnRegistUnit(void);

s32 SearchNameTable(const char *name);

s32 evtAddNameTable(const char *name);

void *SearchBasicFromNameIndex_evtEventViewer(s32 index);

extern const char evtViewerPathLinkMsg[40];

void LinkEffectBasic(dds3Basic_t *eff, s32 nameindex, s32 posindex, s32 baseframe);

extern dds3Basic_t *dds3AddD3PEffectObjectBasic2(void *effdata, void *pos, void *rot);

extern dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 type, void *effdata);

extern s32 dds3EffectObjectPresetMG12(dds3Basic_t *eff);

extern s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1,
                                    dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4);

s32 AddEffectTable2(s32 nowframe, evtViewerObject_s *object, evtFrameInfo_t *frame);

void DelEffectTable_evtEventViewer(s32 index);

void *AddBED(void *data, s32 cutno, s32 nameindex, s32 posindex, s32 baseframe);

void DelBED(evtFrameInfo_t *frame);

void DrawEffect(void);

void SetActiveCamera(void);

extern void evtEffectLoopControl(dds3Basic_t *effect, s32 sw);

extern s32 evtPlaySe(s32 eventNo, s32 soundNo);

extern void dds3SetQuake(s32 frame, s32 range);

extern void dds3StopQuake(void);

void RunFrame1(evtViewerObject_s *track, evtFrameInfo_t *frameInfo, s32 nowFrame, s32 length);

void OutFrame1(evtViewerObject_s *track, evtFrameInfo_t *node, s32 nowFrame, s32 length);

extern evtUnit_t *evtGetUnitHandle2(dds3Basic_t *basic);

extern void evtLinkLightObjectBasic(evtUnit_t *unit, dds3Basic_t *light,
                                    s32 frame);

extern void evtUnlinkLightObjectBasic(evtUnit_t *unit, s32 frame);

extern dds3Basic_t *dds3GetActiveScene(void);

void RunFrame3(s32 nowframe);

void RunFrameUnit(evtViewerObject_s *track, s32 nowFrame);

extern void RunFrameUnit(evtViewerObject_s *track, s32 nowFrame);

extern void RunFrame2(evtViewerObject_s *track, evtFrameInfo_t *next,
                      evtFrameInfo_t *previous, s32 nowFrame, s32 *nearestFrame);

extern evtViewerObject_s *evt_camera;

extern s8 dds3ScreenBlurFlag;

void evtRunFrame(s32 nowFrame);

void DrawActiveCameraName(void);

void SelectActiveCamera(void);

extern void sdfMotionSeek(sdfMotionCtrlr_t *motion, f32 frame);

extern void sdfMotionPause(sdfMotionCtrlr_t *motion);

extern void sdfMotionPauseOff(sdfMotionCtrlr_t *motion);

void evtStopFrame(s32 sw, u32 nowFrame);

void evtViewerStopFrame(void);

void FrameSelect(void);

void PushMenu(s32 no, s32 x, s32 y);

s32 PopMenu(void);

u32 evtEventViewerProcess5_0(void);

extern const char D_0053B620[32];

s32 EnterMain(void);

s32 EnterSelect(s32 unused, s32 menuMode);

s32 EnterObjList(void);

extern const char evtFrameFadeBlackOut[16];

extern const char evtFrameFadeBlackIn[16];

extern const char evtFrameFadeWhiteOut[16];

extern const char evtFrameFadeWhiteIn[16];

extern const char evtViewerUnitVshadowOn[16];

extern const char evtViewerUnitMshadowOn[16];

extern const char evtViewerUnitShadowOff[16];

extern const char evtViewerUnitDispOff[16];

s32 EnterFrameChange(void);

s32 EnterIntFloatChange(void);

s32 EnterFilename2(void);

s32 EnterBasicList(s32 action, s32 previousMenu);

s32 EnterEventPos(void);

extern s16 winoff_x;

extern s32 evtViewerDispatch(s32 actionIndex, s32 x, s32 y);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

extern s32 (*evtViewerMenuHandlers[])(s32 action, s32 previousMenu);

saddr_t evtEventViewerProcess4(void);

saddr_t evtEventViewerProcess3(void);

saddr_t evtEventViewerProcess2(void *id);

saddr_t evtEventViewerProcess1(void);

void evtEventViewerShutdown(void *id);

void evtInitEventViewer(void);

s32 evtKillEventViewer(void);

extern evtViewerInfo_t *viewerinfo;

extern u16 evtFileMajor;

extern u16 evt_file_minor;

extern s16 winoff_x;

extern evtViewerObject_s *evt_camera;

extern char *evtViewerFreeCameraName;

extern char *evtViewerGameCameraName;

extern char evtViewerCameraNameFormat[3];

extern s32 capwait;

extern char evtViewerStrNo[3];

extern char evtViewerStrYes[4];

extern char evtViewerStrOn[3];

extern char evtViewerStrOff[4];

extern char evtViewerStrSub[4];

extern char evtViewerStrAdd[4];

extern char evtViewerStrNormal[7];

extern char evtFrameFadeNameAllStop[8];

extern char evtFrameFadeNameVolUp[6];

extern char evtFrameFadeNameVolDown[8];

extern char evtFrameFadeNameTrans[6];

extern char evtFrameFadeNameFadeOut[8];

extern char evtFrameFadeNameFadeIn[7];

extern char evtFrameFadeNamePlay[5];

extern char evtViewerUnitControlNameDispOn[8];

extern char evtViewerProcNameSlight[7];

extern char evtViewerProcNameSfog[5];

extern char evtViewerProcNameFilter[7];

extern char evtViewerProcNameMfilter[8];
/* End of declarations from src/smt3/event/evtEventViewer.c */

/* Declarations from dds-decomp src/dds2/event/evtEventViewer.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#define SMT3_DEFINES_EVT_VIEWER_MENU 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_BASICLIST 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_DATA 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_EVENTPOS 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_FILENAME 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_FILENAME2 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_FLOATCHANGE 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_FRAMECHANGE 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_FRAMELIST 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_INTCHANGE 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_MAIN 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_MAX 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_NON 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_OBJLIST 1

#define SMT3_DEFINES_EVT_VIEWER_MENU_SELECT 1

#define SMT3_DEFINES_polyMovieObject_t 1

#include "dds3obj.h"

#include "newdata.h"

#include "dds/btl.h"

#include "dds/dds3_object_control_slot.h"

#include "dds/dds3obj.h"

#include "dds/eff_event_draw.h"

#include "dds/eff_transform.h"

#include "dds/evt_sky_alpha_animation_status.h"

#include "dds/evt_world.h"

#include "dds/file_request_api.h"

#include "dds/itf_mes_window.h"

#include "dds/mdl_motion_api.h"

#include "dds/mdl_world_object_source_vector_api.h"

#include "dds/sdf_float_counter.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_motion_loop.h"

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

/* The primary object returned by dds3GetActiveScene is distinct from the active
 * world's WorldInfo payload. Event lookup, lip-sync and viewer code share this chain:
 * object +0x18 -> table +0x08 -> 0x0C-byte slot entries, each with head +0x04.
 * DDS1 and DDS2 use the same observed layout. Node payloads depend on the slot. */
enum {
    EVT_WORLD_SLOT_MOVIE = 3, /* viewer movie-time lists: byte offset 0x28 */
    EVT_WORLD_SLOT_CAMERA = 4, /* linked cameras; active nodes supply the view transform */
    EVT_WORLD_SLOT_UNIT = 5   /* unit/group/lip-sync lists: byte offset 0x40 */
};

void Eve_FilterSetParam(dds3Effect_Filter_t *parameters);

void dds3GetPosPoseSet(dds3Basic_t *object);

struct dds3Basic_t *dds3ObjectBaseGetControlBasic(
    struct dds3Basic_t *object, Dds3ObjectControlSlotIndex slotIndex);

void dds3PathBasicPauseOff(SdfFloatCounter *counter);

void dds3PathBasicPauseOn(SdfFloatCounter *counter);

EvtSkyAlphaAnimationStatus evtCheckSkyAlphaAnime(void);

/* Copy the source object's transform to the target object, with its rotation offset. */
void evtSetUnitPos(u32 targetKey, u32 sourceKey);

/* Return the payload allocation descriptor. For normal file loads, freeing the
 * request wrapper does not free this allocation; its resource owner must keep
 * or release it separately. */
struct sdfMemHandle_t *fileGetMemHandle(const struct FileRequest *request);

/* Free a completed request wrapper; return one while it remains pending. */
FileRequestCleanupResult fileHandleFree(struct FileRequest *request);

/* After chooser completion, return the selected mask bit, or -1 if unset. */
ItfMesSelectedOptionBitResult itfMesMngGetSelect(ItfMesWindowIndex window);

ItfMesStatusWord itfMesMngGetStat(ItfMesWindowIndex window);

void itfMesMngMsgWndCls(ItfMesWindowIndex window);

u16 mdlGetMotionLengthEx(const dds3ModelHandle_t *ctx, MdlMotionSearchId searchId);

extern void *memcpy(void *, const void *, unsigned int);

s32 pacFileCheckEnd(const struct FileRequest *request);

/* Queue the two heap owners separately: the general heap stores descriptors,
 * while the chip heap stores the address of the cell to release. */
void sdfMemFreeDelayedDirect(struct sdfMemHandle_t *allocation);

void sdfMotionChangeBlend(struct sdfMotionCtrlr_t *motion, s32 motionIndex, SdfMotionLoopMode loopMode,
                         f32 blendLeadFrames, f32 blendDurationFrames);

#include "dds/sdk_ps2.h"

#include "dds/common.h"

#include "dds/evt_viewer_track_file_format.h"

#include "dds/scr_command_result.h"

#include "dds/sdf_delayed_resource_check_api.h"

#include "dds/sdf_vu0.h"

#include "dds/sdf_dma_tag.h"

#include "dds/evt_window_shade_packet.h"

#include "dds/evt_camera_color_track_api.h"

#include "dds/dds3_background_fade_api.h"

#include "dds/sdf_float_counter.h"

#include "dds/dds3_path.h"

#include "dds/dds3_task_add_child.h"

#include "dds/mnu_title_stream.h"

#include "dds/mnu_title_sound.h"

#include "dds/mnu_movie_playback_api.h"

#include "dds/mnu_camp_font_api.h"

#include "dds/mnu_camp_option_index_api.h"

#include "dds/sdf_chip.h"

#include "dds/evt_viewer.h"

#include "dds/event.h"

#include "dds/evt_viewer_indexed_condition.h"

#include "dds/evt_viewer_object_slot_api.h"

#include "dds/evt_sound_variation.h"

#include "dds/evt_reload_viewer_api.h"

#include "dds/evt_reset_unit_vector_slots_api.h"

#include "dds/dds3obj.h"

#include "dds/dds3_object_control_slot.h"

#include "dds/evt_world.h"

#include "dds/mdl_motion_api.h"

#include "dds/mdl_world_object_source_vector_api.h"

#include "dds/sdf_motion_loop.h"

#include "dds/itf_mes_window.h"

#include "dds/itf_mes_entry_start_api.h"

#include "dds/itf_mes_voice_cue_result.h"

#include "dds/sdf_heap_api.h"

#include "dds/kwln.h"

#include "dds/dds3_fade.h"

#include "dds/dds3_fade_timing.h"

#include "dds/sdf.h"

#include "dds/sdf_draw.h"

#include "dds/pcp_vu0.h"

#include "dds/evt_unit.h"

#include "dds/evt_unit_vector_slot.h"

#include "dds/eff_transform.h"

#include "dds/eff_blur.h"

#include "dds/eff_event_draw.h"

#include "dds/dat_state.h"

#include "dds/eff.h"

#include "dds/eff_node.h"

#include "dds/eff_object.h"

#include "dds/eff_object_dispatch_api.h"

#include "dds/mdl.h"

#include "dds/mdl_motion_controller_lookup_api.h"

#include "dds/evt_polygon_movie.h"

#include "dds/evt_viewer_group_record.h"

#include "dds/evt_sky_alpha_animation_status.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/file_request_api.h"

#include "dds/dds3_task_create.h"

#include "dds/itf.h"

#include "dds/sdf_packet_list.h"

extern s32 mnuGetSelectedTableValue(dds_evtViewerInfo_t *runtime);

#include "dds/dds3_world_primary.h"

void Eve_PartialMultiBlurSetDefTex(void);

void Eve_DistortionBlurSetDefTex(void);

void Eve_RippleBlurSetDefTex(void);

void Eve_PartialMultiFilterSetDefTex(void);

void evtPolygonMovieFreeWork(PolyMovieWork *table);

void kwlnPadResetMotorLevelsAndOutput(void);

EvtRuntimeChild *evtEventViewerGetPendingNode(dds_evtViewerInfo_t *viewer);

void evtEventViewerReleasePendingNode(dds_evtViewerInfo_t *viewer);

void evtEventViewerFreeSlot(s32 index, dds_evtViewerInfo_t *viewer);

extern EvtRuntimeChild *evtCreateViewerTimelineKey(EvtRuntimeGroup *group, s32 frame, dds_evtViewerInfo_t *viewer);

/* Return the pending node at position (count A + count B) in the queue entry, or NULL. */
EvtRuntimeChild *evtEventViewerGetPendingNode(dds_evtViewerInfo_t *viewer);

/* Release the pending node and consume its position in the viewer queue. */
void evtEventViewerReleasePendingNode(dds_evtViewerInfo_t *viewer);

/* Release a supplied queued node, with the same resource and counter handling. */
void evtEventViewerReleaseQueuedNode(dds_evtViewerInfo_t *viewer, EvtRuntimeGroup *entry, EvtRuntimeChild *node);

/* Consume queued viewer events until the pending check reports none. */
void evtEventViewerProcessPending(dds_evtViewerInfo_t *viewer);

/* Insert an entry into the viewer's list, ordered by id. */
void evtEventViewerInsertEntry(EvtRuntimeGroup *entry, dds_evtViewerInfo_t *viewer);

void evtEventViewerUnlinkEntry(EvtRuntimeGroup *entry, dds_evtViewerInfo_t *viewer);

/* Allocate a zeroed 0x84-byte entry for a type and queue it in the viewer. */
EvtRuntimeGroup *evtEventViewerCreateEntry(s32 entryType, dds_evtViewerInfo_t *viewer);

s32 evtEventViewerCountEntriesById(s32 entryType, dds_evtViewerInfo_t *viewer);

s32 evtEventViewerCountEntries(dds_evtViewerInfo_t *viewer);

/* PM2 skips group ids 5/0x13; PM3 serializes only those groups. */
s32 evtEventViewerSumNodeCounts(EvtViewerTrackFileFormat format, dds_evtViewerInfo_t *viewer);

/* Destroy an entry: free its nodes, drop a type-0x18 texture, unlink it, free it. */
void evtEventViewerDestroyEntry(EvtRuntimeGroup *entry, dds_evtViewerInfo_t *viewer);

void evtViewerSetMinimumFromCurrent(dds_evtViewerInfo_t *range);

void evtViewerSetMaximumFromCurrent(dds_evtViewerInfo_t *range);

/* Reset the viewer (0x24BC bytes) but keep its first and 0x2410 fields. */
void evtEventViewerReset(dds_evtViewerInfo_t *viewer);

/* Destroy every grouped entity listed in the viewer's table. */
void evtEventViewerReleaseGroups(dds_evtViewerInfo_t *viewer);

/* Search the fixed-width (0x20-byte) event-name records. */
EvtViewerNameIndex evtEventViewerFindNameIndex(const char *name, dds_evtViewerInfo_t *viewer);

/* Return the index of a name in the table, appending it when missing. */
EvtViewerNameIndex evtEventViewerAddName(const char *name, dds_evtViewerInfo_t *viewer);

/* Look up a named kind-7 effect object by index. */
dds3Basic_t *evtEventViewerGetNameObject(EvtViewerNameIndex index, dds_evtViewerInfo_t *viewer);

/* Billboard entries and polygon movies use distinct owner attachment paths. */
void evtViewerBindNamedOwner(dds3Basic_t *effectObject, EvtViewerNameIndex ownerNameIndex, s32 billboardEntry, u32 startFrame, dds_evtViewerInfo_t *viewer);

struct SoundMixer;

struct EffNodeDescriptor;

extern dds3Basic_t *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *descriptor, void *firstVector, void *secondVector);

extern dds3Basic_t *dds3AddD3PEffectObjectBasic(void *resourceData, void *firstVector, void *secondVector);

/* Create the viewer object for a command in the first free slot, if available. */
EvtViewerObjectSlotResult evtViewerCreateObjectInFreeSlot(s32 unused, EvtRuntimeGroup *track, EvtRuntimeChild *key, dds_evtViewerInfo_t *viewer);

void evtEventViewerFreeSlot(s32 index, dds_evtViewerInfo_t *viewer);

/* Create an event-viewer effect at the origin and attach its active event node. */
dds3Basic_t *evtEventViewerCreateOriginEffect(struct SoundMixer *resource, u32 entryId, EvtViewerNameIndex ownerNameIndex, s32 billboardEntry, u32 startFrame,
                    dds_evtViewerInfo_t *viewer);

extern u8 evtViewerHasUpdateFlag(dds_evtViewerInfo_t *);

typedef struct CampDisplayDefaults CampDisplayDefaults;

extern void mnuBlendCampDisplayFields(EvtRuntimeChild *from, EvtRuntimeChild *to, CampDisplayDefaults *display, f32 ratio);

extern void mnuDrawCampScaledTexture(sdfTexHandle_t *texture, CampDisplayDefaults *display);

/* Interpolates parameter keys at the viewer's current frame, accounting for
 * the track offset. DDS2 subtracts 35 from the second output word before applying it. */
void evtViewerApplyInterpolatedNodeKey(dds_evtViewerInfo_t *viewer, EvtRuntimeGroup *track, u16 *from, u16 *to);

/* Applies kind-24 parameter tracks using bracketing keys, or the pending key
 * when keyMode is 1. Traversal uses the shared timeline-key record. */
void evtViewerApplyParameterKeyTracks(dds_evtViewerInfo_t *viewer);

extern KwlnDrawVectorParams kwlnDrawVector;

extern u128 *D_0037F770[];

extern u128 kwlnDefaultColorVector[];

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

extern dds3Effect_PartialMultiBlurParam_t *effEventGetBlurTemplateSetupParams(void);

extern dds3Effect_DistortionBlurParam_t *effEventGetScatterBlurSetupParams(void);

extern dds3Effect_RippleBlurParam_t *rblurGetParam(void);

extern dds3Effect_PartialMultiFilterParam_t *mfilterGetParam(void);

void evtEventViewerApplyEffectToggles(dds_evtViewerInfo_t *viewer);

/* Select the active entry (or fallback) and sync world selection and camera. */
void evtViewerApplySelectedEntry(dds_evtViewerInfo_t *viewer);

extern u16 D_004372B0;

extern void mnuUnpackNibbleFields(EvtRuntimeChild *key, s32 *lowBitsOut, s32 *highBitsOut);

extern EvtPackedSoundTrackId evtPlayEventBgm(s32 eventId, EvtSoundVariation variation);

extern EvtPackedSoundTrackId evtFadeInEventBgm(s32 eventId, EvtSoundVariation variation);

extern EvtPackedSoundTrackId evtStopEventBgm(s32 eventId, EvtSoundVariation variation);

extern void evtTransEventBgm(s32 eventId, EvtSoundVariation variation);

extern void fldRegisterCampSceneId(dds_evtViewerInfo_t *viewer, s32 identifier);

extern void p2sdBgmVolDown(void);

extern void p2sdBgmVolUp(void);

extern void p2sdStopAllBgm(void);

extern void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

extern void dds3PlayMovieByIndex(s32 value);

extern u8 fldTitleIsActive(void);

extern void fldSetTitleDrawMode(u32 enabled);

extern void nbPanelAnalyzeStart(void);

extern void dds3FadeBackgroundStartIn(s32 duration);

extern void itfMesBuildOptionList(ItfMesWindowIndex window, ItfMesEntryIndex entryIndex);

extern void mnuStopTitleVoicePlayback(void);

extern void datDebugPrintf(const char *format, ...);

extern char D_004224A8[];

void evtEventViewerApplyTimelineKey(EvtRuntimeGroup *track, EvtRuntimeChild *key, s32 currentFrame, s32 unused,
                   dds_evtViewerInfo_t *viewer);

extern u16 D_004372B0;

extern u16 D_004372B2;

extern u8 D_00423050[];

extern s32 evtViewerSaveTrackFiles(s32 mode, dds_evtViewerInfo_t *runtime);

extern u8 evtViewerHasUpdateFlag(dds_evtViewerInfo_t *);

extern void evtEventViewerApplyEffectToggles(dds_evtViewerInfo_t *viewer);

extern void evtViewerApplySelectedEntry(dds_evtViewerInfo_t *viewer);

extern u8 D_004372C0[3];

extern u32 dds3GetFrameID(void);

extern void dds3FadeSet(s32 red, s32 green, s32 blue, s32 alpha);

extern void Eve_PartialMultiBlurSetDefTex(void);

extern void Eve_DistortionBlurSetDefTex(void);

extern void Eve_PartialMultiFilterSetDefTex(void);

extern void Eve_RippleBlurSetDefTex(void);

extern void Eve_PartialMultiBlurSetTex(sdfTexHandle_t *texture);

extern void Eve_DistortionBlurSetTex(sdfTexHandle_t *sourceHandle);

extern void Eve_PartialMultiFilterSetTex(sdfTexHandle_t *texture);

extern void Eve_RippleBlurSetTex(sdfTexHandle_t *sourceHandle);

void evtApplyViewerTimelineFrame(s32 time, dds_evtViewerInfo_t *viewer);

void evtViewerPushCommandHistory(s32 actionMode, s32 first, s32 second, dds_evtViewerInfo_t *viewer);

dds3TaskUpdateResult_t evtViewerUpdateFrame(dds3ProcessID_t *task);

dds3TaskUpdateResult_t evtViewerScheduleFrameVariableTask(dds3ProcessID_t *task);

extern const char *D_003C91C0[];

extern const char *D_003C91C8[];

extern const char *D_003C91D0[];

extern const char *D_003C91E0[];

extern s32 D_004372CC;

extern u32 dds3DrawFlag;

void fldInitializeCameraColorResource(void);

extern void evtEventViewerPollMovieResources(PolyMovieWork *assets);

extern u8 evtViewerSkipsTitleStreamWait(PolyMovieWork *ctx);

struct dds_evtViewerInfo_t;

struct EffNode;

typedef struct EvtViewerObjectData {
    u8 pad00[0xC];
    struct EffNode *parameterNode;
} EvtViewerObjectData;

u16 evtViewerPopHistory(dds_evtViewerInfo_t *viewer);

extern char evtViewerTaskName[];

extern s8 evtViewerFrameColumnCategories[];

typedef struct CampDisplayDefaults CampDisplayDefaults;

extern void mnuBlendCampDisplayFields(EvtRuntimeChild *from, EvtRuntimeChild *to, CampDisplayDefaults *display, f32 ratio);

extern void mnuDrawCampScaledTexture(sdfTexHandle_t *texture, CampDisplayDefaults *display);

/* Timeline payloads store fogFar before fogNear. */
extern KwlnDrawVectorParams kwlnDrawVector;

extern u128 *D_0037F770[];

extern u128 kwlnDefaultColorVector[];

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

extern dds3Effect_PartialMultiBlurParam_t *effEventGetBlurTemplateSetupParams(void);

extern dds3Effect_DistortionBlurParam_t *effEventGetScatterBlurSetupParams(void);

extern dds3Effect_RippleBlurParam_t *rblurGetParam(void);

extern dds3Effect_PartialMultiFilterParam_t *mfilterGetParam(void);

#include "dds/sdf_debug_api.h"

/* Applies duration-scaled alpha to the selected effect for supported track
 * kinds. The key's duration is used directly, without validation here. */
void evtEventViewerApplyDurationScaledEffectAlpha(EvtRuntimeGroup *group, EvtRuntimeChild *key, s32 unused2, s32 unused3, dds_evtViewerInfo_t *viewer);

extern void evtEventViewerFreeSlot(s32 index, dds_evtViewerInfo_t *viewer);

void evtEventViewerFadeEffectSlot(EvtRuntimeGroup *group, EvtRuntimeChild *key, s32 time,
                   s32 interval, dds_evtViewerInfo_t *viewer);

#include "dds/evt_blend_vec_color.h"

extern EvtRuntimeGroup *D_004372B8;

extern f32 D_00453960[3][4];

extern EvtBlendVecColor D_00453990;

extern dds3Effect_Blur_t D_004539B0;

extern dds3Effect_PartialMultiBlurParam_t D_004539E0;

extern dds3Effect_Filter_t D_00453A10;

extern dds3Effect_PartialMultiFilterParam_t D_00453A30;

extern dds3Effect_DistortionBlurParam_t D_00453A60;

extern dds3Effect_RippleBlurParam_t D_00453A90;

extern dds3Effect_Blur_t D_00453AC0;

extern s16 dds3ScreenBlurAlpha;

extern s16 dds3ScreenBlurScale;

extern void evtSetBokasi(u32 enabled);

extern void evtSetSkyAlpha(s32 duration, s32 state);

extern void fldSetFadeTarget(s32 id, s32 mode, s32 duration);

extern u32 fldSkyGetAlpha(void);

extern void dds3FadeClear(void);

extern void dds3SetLightColor(s32, s32, const f32 *);

extern void dds3SetLightDir(s32, s32, const f32 *);

extern void dds3SetAmbientColor(s32, f32 *);

extern void evtSetSlotVectors(s32, EvtUnitVectorSlotState, evtUnitHandle_t *, const f32 *, const f32 *, const f32 *);

extern void evtSetSlotVector(s32, f32, f32);

extern void dds3SetFogPara(s32, f32, f32, f32, f32);

extern f32 evtHokanFloat(s32, f32, f32, f32);

extern void evtHokanSceneLight(s32, f32, const f32 *, const f32 *, f32 *);

extern void evtHokanSceneFog(s32, f32, const EvtBlendVecColor *, const EvtBlendVecColor *, EvtBlendVecColor *);

extern void evtHokanBlur2(s32, f32, const dds3Effect_Blur_t *, const dds3Effect_Blur_t *, dds3Effect_Blur_t *);

extern void evtHokanMultBlur(s32, f32, const dds3Effect_PartialMultiBlurParam_t *, const dds3Effect_PartialMultiBlurParam_t *, dds3Effect_PartialMultiBlurParam_t *);

extern void evtHokanMultFilter(s32, f32, const dds3Effect_PartialMultiFilterParam_t *, const dds3Effect_PartialMultiFilterParam_t *, dds3Effect_PartialMultiFilterParam_t *);

extern void evtHokanDistBlur(s32, f32, const dds3Effect_DistortionBlurParam_t *, const dds3Effect_DistortionBlurParam_t *, dds3Effect_DistortionBlurParam_t *);

extern void evtHokanRipBlur(s32, f32, const dds3Effect_RippleBlurParam_t *, const dds3Effect_RippleBlurParam_t *, dds3Effect_RippleBlurParam_t *);

extern void evtBlendParamsG(s32, f32, const dds3Effect_Blur_t *, const dds3Effect_Blur_t *, dds3Effect_Blur_t *);

extern void evtHokanFilter(s32, f32, const dds3Effect_Filter_t *, const dds3Effect_Filter_t *, dds3Effect_Filter_t *);

extern void effSetLoadDescA(const dds3Effect_Blur_t *);

extern void Eve_PartialMultiBlurSetParam(dds3Effect_PartialMultiBlurParam_t *);

extern void Eve_DistortionBlurSetParam(dds3Effect_DistortionBlurParam_t *);

extern void Eve_RippleBlurSetParam(dds3Effect_RippleBlurParam_t *);

extern void effCopyFramebufferQuadParameters(const dds3Effect_Blur_t *source);

extern void Eve_PartialMultiFilterSetParam(dds3Effect_PartialMultiFilterParam_t *);

extern s32 mnuSolveCubicBezierYForX(s32,const s32 *,const s32 *);

void evtViewerUpdateTimelineGroupState(EvtRuntimeGroup *group, EvtRuntimeChild *future,
        EvtRuntimeChild *previous, s32 time, s32 *bestDistance, dds_evtViewerInfo_t *viewer,
        s32 offset);

extern void evtEndUnitValueTransition(evtUnitHandle_t *unit, s32 duration);

/* At timelineTime, selects each named unit's latest kind-9 transition key and starts,
 * ends or leaves its value transition according to the selected key flags. */
void evtEventViewerUpdateUnitTransitions(s32 timelineTime, dds_evtViewerInfo_t *viewer);

extern f32 evtHokanFloat(s32 enable, f32 t, f32 a, f32 b);

extern s32 evtHokanInt(s32 enable, f32 t, s32 a, s32 b);

/* At time, applies the track's latest enabled mode keys, then interpolates the
 * unit's byte and float parameters from the active kind-3/4 key to the next
 * enabled key of the same kind. */
void evtStepPolygonMovieConditionalKeys(EvtRuntimeGroup *track, s32 time);

extern void evtViewerClampMovieTimes(s32 endTime, dds_evtViewerInfo_t *viewer);

extern void evtViewerSyncWorldGroups(s32 position, dds_evtViewerInfo_t *viewer);

extern void evtViewerApplyGlyphLodChannel(s32 position, dds_evtViewerInfo_t *viewer);

extern void evtEventViewerAttachIndexedObject(s32 position, dds_evtViewerInfo_t *viewer);

extern void evtViewerActivateWindowForGlyphEntry(s32 position, dds_evtViewerInfo_t *viewer);

extern void evtViewerCountFlaggedUpdates(dds_evtViewerInfo_t *viewer);

extern void evtEventViewerApplyEffectChannels(s32 frame, dds_evtViewerInfo_t *viewer);

extern s32 evtViewerUpdateTimedAction(dds_evtViewerInfo_t *viewer);

extern void evtViewerUpdateShopFlagTrack(dds_evtViewerInfo_t *viewer);

extern void evtEventViewerApplyTimelineKey(EvtRuntimeGroup *track, EvtRuntimeChild *key, s32 currentFrame,
                                           s32, dds_evtViewerInfo_t *viewer);

extern void evtEventViewerFadeEffectSlot(EvtRuntimeGroup *group, EvtRuntimeChild *key, s32 time,
                                        s32 interval, dds_evtViewerInfo_t *viewer);

extern void evtViewerUpdateTimelineGroupState(EvtRuntimeGroup *, EvtRuntimeChild *, EvtRuntimeChild *, s32, s32 *, dds_evtViewerInfo_t *, s32);

extern EvtRuntimeGroup *D_004372B8;

void evtApplyViewerTimelineFrame(s32 time, dds_evtViewerInfo_t *viewer);

/* Returns the nearest kind-2 key at/before the current frame, or NULL.
 * Equal-distance ties retain the first key visited. */
EvtRuntimeChild *evtViewFindGlyphAtOrBefore(dds_evtViewerInfo_t *viewer);

extern void evtSetMovieClipPositionClampedToDuration(dds3Basic_t *movie, s32 unused, s32 start, s32 end, s32 offset);

/* Clamps each movie object's playback interval using its linked track's first
 * key frame (or track offset), endTime, and the current key's extra parameter. */
void evtViewerClampMovieTimes(s32 endTime, dds_evtViewerInfo_t *viewer);

extern EvtRuntimeChild *evtViewerFindLatestMatchingMotionKey(EvtRuntimeGroup *group, s32 frame,
                                                             MdlMotionSearchId channel);

void evtViewerSyncModelMotionChannels(s32 frame, dds3Basic_t *object, EvtRuntimeGroup *track,
                   dds_evtViewerInfo_t *viewer, s32 mode);

/* Updates world units at position using the first track whose owner word
 * matches that unit's object-chain node. */
void evtViewerSyncWorldGroups(s32 position, dds_evtViewerInfo_t *viewer);

extern u32 sdfGetLodChunkValue(dds_sdfModelRootNode_t *model);

/* Applies the latest kind-6 key at/before position to a kind-1 track's LOD
 * byte, provided the requested signed-byte level is supported by its chunk. */
void evtViewerApplyGlyphLodChannel(s32 position, dds_evtViewerInfo_t *viewer);

/* At an exact kind-7 key frame, attaches the indexed world object. Index -1
 * restores the saved vectors; losing the active key restores them once too. */
void evtEventViewerAttachIndexedObject(s32 position, dds_evtViewerInfo_t *viewer);

extern void mnuUnpackNibbleFields(EvtRuntimeChild *key, s32 *lowBitsOut, s32 *highBitsOut);

void evtViewerMarkWindowActive(dds_evtViewerInfo_t *);

/* Activates the message window thirty frames before a kind-4 key when its
 * unpacked entry is ready. Only the first kind-4 track is considered. */
void evtViewerActivateWindowForGlyphEntry(s32 timelinePosition, dds_evtViewerInfo_t *viewer);

void evtViewerCountFlaggedUpdates(dds_evtViewerInfo_t *viewer);

extern u32 mnuShouldBlockViewerEndCountdown(void);

extern ScrCommandResult scrCommandIsProcessControlFlagClear(void);

extern void mnuReleaseCampSceneRegisteredIds(dds_evtViewerInfo_t *viewer);

extern void mnuStopTitleVoicePlayback(void);

extern void sdfSndSetChannelCount(u32 channels);

extern u8 fldTitleIsActive(void);

extern void fldSetTitleDrawMode(u32 active);

void evtViewerCleanupMessageWindow(dds_evtViewerInfo_t *viewer);

void evtEventViewerStartEndCountdown(dds_evtViewerInfo_t *viewer);

u8 evtViewerHasUpdateFlag(dds_evtViewerInfo_t *viewer);

/* Fades the viewer's background colour over its update countdown. */
void evtEventViewerFadeBackground(dds_evtViewerInfo_t *viewer);

/* Applies the effect-channel assignments driven by the viewer's tracks. */
void evtEventViewerApplyEffectChannels(s32 frame, dds_evtViewerInfo_t *viewer);

extern void movieStop(void);

/* Advance or stop the timed viewer action according to the current position. */
s32 evtViewerUpdateTimedAction(dds_evtViewerInfo_t *viewer);

void evtViewerPreGlyphTickHookNoOp(dds_evtViewerInfo_t *viewer);

void evtViewerAdvanceGlyphTick(dds_evtViewerInfo_t *viewer);

void evtViewerFlag8ClearFrameHookNoOp(dds_evtViewerInfo_t *viewer);

/* Returns the latest eligible kind-5 motion key at/before frame for channel,
 * or NULL. Its condition must pass; equal-frame ties retain the first key. */
EvtRuntimeChild *evtViewerFindLatestMatchingMotionKey(EvtRuntimeGroup *group, s32 frame,
                                                      MdlMotionSearchId channel);

enum {
    EVT_VIEWER_PLAYBACK_PAUSED = 0,
    EVT_VIEWER_PLAYBACK_RUNNING = 1,
};

/* Apply the viewer playback mode to unit, motion and movie-object tracks. */
void evtEventViewerApplyPlaybackMode(s32 playbackMode, u32 frame, dds_evtViewerInfo_t *viewer);

/* Dispatch one of two viewer modes based on its lowest flag bit. */
void evtViewerDispatchFlagMode(dds_evtViewerInfo_t *viewer);

/* Returns the nearest kind-2 key strictly after the current frame, or NULL.
 * Equal-distance ties retain the first key visited. */
EvtRuntimeChild *evtViewFindNextGlyph(dds_evtViewerInfo_t *viewer);

/* Returns the nearest kind-2 key strictly before the current frame, or NULL.
 * Equal-distance ties retain the first key visited. */
EvtRuntimeChild *evtViewFindPrevGlyph(dds_evtViewerInfo_t *viewer);

void evtViewerPushCommandHistory(s32 actionMode, s32 first, s32 second, dds_evtViewerInfo_t *viewer);

u16 evtViewerPopHistory(dds_evtViewerInfo_t *viewer);

extern s32 itfMesMngGetSelPhase(s32 windowIndex);

extern void itfMesMngSetSelPhase(s32 windowIndex, s16 status);

extern void itfMesMngRequestMessageRelease(s32 window, s32 releasePrimaryBlock);

extern void itfMesMngRequestSelectRelease(s32 window);

void evtViewerMarkWindowInactive(dds_evtViewerInfo_t *);

void evtViewerUpdateWindowShadeFade(dds_evtViewerInfo_t *);

void evtUpdateViewerMessageWindow(dds_evtViewerInfo_t *viewer);

void evtViewerCleanupMessageWindow(dds_evtViewerInfo_t *viewer);

void evtViewerMarkWindowActive(dds_evtViewerInfo_t *viewer);

void evtViewerMarkWindowInactive(dds_evtViewerInfo_t *viewer);

extern EvtWindowShadePacket evtWindowShadePackets[];

extern u8 D_004372C8[4];

extern sdfGrOt_t D_00380448;

/* Ramp the message-window shade toward its target and, while visible, queue
 * the translucent shade sprite for the current draw buffer. */
void evtViewerUpdateWindowShadeFade(dds_evtViewerInfo_t *viewer);

EvtViewerIndexedConditionResult evtViewerTestIndexedCondition(
    EvtViewerIndexedCondition encodedCondition);

u32 evtViewerReturnZero(void);

s32 evtEventViewerOpenInputMenu(s32 arg0, s32 arg1, dds_evtViewerInfo_t *viewer);

/* Commit viewer menu selections and apply typed edits to the current timeline key. */
s32 evtViewerApplyCommandSelection(s32 unused0, s32 mode, dds_evtViewerInfo_t *viewer);

u32 evtViewerPushMode5CommandHistory(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer);

/* Viewer command menus and the existing timeline/control providers. */
extern char *D_003C91E8[];

extern char *D_003C91F8[];

extern char *D_003C9200[];

extern char *D_003C9210[];

extern char *D_003C9230[];

extern char *D_003C9250[];

extern char *D_003C9270[];

extern char *D_003C9278[];

extern char *D_003C9280[];

extern char *D_003C9288[];

extern char *D_003C9290[];

extern char *D_003C9298[];

extern char *D_003C92A0[];

extern char *D_003C92A8[];

extern const char D_004373A0[];

extern const char D_004373A8[];

extern const char D_004373B0[];

extern u8 corePktSoftenBias;

extern void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

extern void effSetLoadDescA(const dds3Effect_Blur_t *parameters);

extern void effSetLoadDescD(const dds3Effect_Blur_t *parameters);

extern void effEventSetBlurTemplateParameters(dds3Effect_PartialMultiBlurParam_t *parameters);

extern void effEventSetScatterBlurParameters(dds3Effect_DistortionBlurParam_t *parameters);

extern void rblurSetParam(dds3Effect_RippleBlurParam_t *parameters);

extern void mfilterSetParam(dds3Effect_PartialMultiFilterParam_t *parameters);

extern void campResolvePendingValue(dds_evtViewerInfo_t *scene, EvtRuntimeChild *cue);

extern s32 mdlDebugPrintEffectScale2(dds3ProcessID_t *task);

extern dds3TaskUpdateResult_t evtPollRuntimeControlReady(dds3ProcessID_t *task);

extern dds3TaskUpdateResult_t evtSynchronizeSelectedEntry(dds3ProcessID_t *task);

s32 evtEventViewerHandleKeyEditCommand(s32 command, s32 unused, dds_evtViewerInfo_t *viewer);

/* Store the edited timing or selector halfword in the pending timeline key. */
s32 evtViewerStoreKeyTimingOrSelector(s32 unused0, s32 unused1, dds_evtViewerInfo_t *viewer);

extern void mnuFxWorldScrollDelta();

extern void mnuAdjustTimelineDurations(dds_evtViewerInfo_t *world, s32 threshold, s32 delta);

/* Store the edited menu value: the option/frame commands commit directly, and
 * otherwise the value goes into the pending key field chosen by the column's
 * command category and the track kind. */
s32 evtViewerStoreMenuValueInPendingKey(s32 unused, s32 command, dds_evtViewerInfo_t *viewer);

/* Store the command value as a halfword and clear its extra halfword when tagged. */
s32 evtViewCmdSetValue(s32 unused0, s32 unused1, dds_evtViewerInfo_t *viewer);

/* Store the command value in the selected halfword of a viewer entry. */
u32 evtViewerStoreCommandInSelectedField(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer);

/* Registered viewer-command handler with no action; return 0. */
u32 evtViewCmdNoOp(void);

/* Copy the current command word into the selected script entry. */
u32 evtViewerStoreCommandInEntryWord(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer);

s32 evtEventViewerCommitGroupSelection(s32 unused0, s32 unused1, dds_evtViewerInfo_t *viewer);

u32 evtBattleCopyMatrix(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer);

/* Transfer a selected two-component viewer position to the command entry. */
s32 evtViewCmdSetPosition(s32 unused0, s32 unused1, dds_evtViewerInfo_t *viewer);

u32 evtViewCmdCancelSelection(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer);

s32 evtViewCmdResolveSlot(s32 operation, void *argument, dds_evtViewerInfo_t *viewer);

/* Copy the selected slot descriptor and numeric value into the script entry. */
s32 evtViewCmdSetSlot(s32 unused0, s32 unused1, dds_evtViewerInfo_t *viewer);

/* Apply one of three viewer selection modes to the selected slots. */
s32 evtViewCmdSelectMode(s32 unused0, s32 unused1, dds_evtViewerInfo_t *viewer);

u32 evtViewerClearPendingNodeAndPushHistory(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer);

s32 evtEventViewerCopyEffectSetupPayload(s32 unused0, s32 unused1, dds_evtViewerInfo_t *viewer);

/* Hand control back to the frame update whether or not the task still owns a viewer. */
dds3TaskUpdateResult_t evtViewerPickNextHandler(dds3ProcessID_t *task);

extern s16 D_004372B4;

extern void datDebugPrintf(const char *, ...);

extern void evtUpdateViewerMessageWindow(dds_evtViewerInfo_t *);

extern void mnuAdvanceShopMenuState();

extern u8 mnuGetMoviePlaybackMode(void);

extern dds3TaskUpdateResult_t evtViewerPickNextHandler(dds3ProcessID_t *task);

/* Advance the viewer timeline, deferred voice and task-update handoff. */
dds3TaskUpdateResult_t evtViewerUpdateFrame(dds3ProcessID_t *task);

/* Update the active viewer, then switch to its frame-variable task. */
dds3TaskUpdateResult_t evtViewerScheduleFrameVariableTask(dds3ProcessID_t *task);

/* Initialize the active viewer and schedule its next update callback. */
dds3TaskUpdateResult_t evtViewerInitializeUpdateSequence(dds3ProcessID_t *task);

u8 evtViewerSkipsTitleStreamWait(PolyMovieWork *ctx);

/* Advance the viewer update: tick the timed action or hand over to the next task. */
dds3TaskUpdateResult_t evtViewerAdvanceUpdate(dds3ProcessID_t *task);

dds3TaskUpdateResult_t evtViewerStartUpdate(dds3ProcessID_t *task);

u8 evtViewerSkipsCampSceneIdRelease(PolyMovieWork *task);

extern f32 sdfViewShakeVector[];

extern void dds3FreeRenderTexture();

extern void fldReleaseCameraColorEffect();

extern void mnuReleaseCampSceneRegisteredIds(dds_evtViewerInfo_t *viewer);

void evtViewerCleanupMessageWindow(dds_evtViewerInfo_t *viewer);

void evtViewerRelease();

void evtEventViewerDestroyActive(dds3ProcessID_t *task);

void evtEventViewerDestroyActiveAlternate(dds3ProcessID_t *task);

extern void mnuCampInitFontResource(dds_evtViewerInfo_t *viewer);

void evtEventViewerInitCampFontResource(dds_evtViewerInfo_t *viewer);

extern u32 D_00435CBC;

void evtViewerCreateTaskWithSky(void);

void evtEventViewerDestroyTask(void);

void evtEventViewerPollMovieResources(PolyMovieWork *assets);
/* End of declarations from src/dds2/event/evtEventViewer.c */

INCLUDE_ASM(const s32, "event/evtEventViewer", LinkFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", UnlinkFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", SortFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001AF498);

evtFrameInfo_t *AddFrameInfo2(evtViewerObject_s *group, s32 frame, evtViewerInfo_t *viewer)
{
    return evtAddFrameInfo(group, frame);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001AFB80);

INCLUDE_ASM(const s32, "event/evtEventViewer", DelFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001AFC40);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001AFD28);

/* Consume queued viewer events until the pending check reports none. */
void evtEventViewerProcessPending(dds_evtViewerInfo_t *viewer) {
    while (evtEventViewerGetPendingNode(viewer) != 0) {
        evtEventViewerReleasePendingNode(viewer);
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001AFE48);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001AFEE8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001AFF38);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001AFFB0);

s32 evtEventViewerCountEntries(dds_evtViewerInfo_t *viewer) {
    EvtRuntimeGroup *entry;
    s32 entryCount;

    entryCount = 0;
    for (entry = viewer->groups; entry != NULL; entry = entry->next) {
        entryCount++;
    }
    return entryCount;
}

/* PM2 skips group ids 5/0x13; PM3 serializes only those groups. */
s32 evtEventViewerSumNodeCounts(EvtViewerTrackFileFormat format, dds_evtViewerInfo_t *viewer) {
    EvtRuntimeGroup *entry;
    s32 total;

    total = 0;
    for (entry = viewer->groups; entry != NULL; entry = entry->next) {
        if (format == EVT_VIEWER_TRACK_FILE_FORMAT_PM2) {
            if (entry->type == 5 || entry->type == 0x13) {
                continue;
            }
        } else if (format == EVT_VIEWER_TRACK_FILE_FORMAT_PM3) {
            if (entry->type != 5 && entry->type != 0x13) {
                continue;
            }
        } else {
            continue;
        }
        total += entry->childCount;
    }
    return total;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B00A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0150);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0170);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0190);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0220);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B02A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0310);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0398);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B03F8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0460);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B05A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B07F8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0838);

INCLUDE_ASM(const s32, "event/evtEventViewer", DelBED);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0950);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B09E0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0A58);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0BD8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0DC8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B0ED0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B1000);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B1068);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B1250);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B19A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B1A78);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B1B90);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B1D30);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B1E78);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B1F98);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B2180);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B2DC8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B2FB8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B3358);

/* Returns the nearest kind-2 key at/before the current frame, or NULL.
 * Equal-distance ties retain the first key visited. */
EvtRuntimeChild *evtViewFindGlyphAtOrBefore(dds_evtViewerInfo_t *viewer) {
    EvtRuntimeChild *result = NULL;
    s32 best = 99999;
    EvtRuntimeGroup *node = viewer->groups;

    while (node != NULL) {
        if (node->type == 2) {
            EvtRuntimeChild *glyph = node->children;

            if (glyph != NULL) {
                do {
                    s32 frame = glyph->frame;

                    if (frame <= viewer->nowframe) {
                        s32 distance = viewer->nowframe - frame;

                        if (distance < best) {
                            best = distance;
                            result = glyph;
                        }
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
        }
        node = node->next;
    }
    return result;
}

/* Clamps each movie object's playback interval using its linked track's first
 * key frame (or track offset), endTime, and the current key's extra parameter. */
void evtViewerClampMovieTimes(s32 endTime, dds_evtViewerInfo_t *viewer) {
    evtWorldTable_t *worldTable;
    evtWorldSlot_t *worldSlots;
    dds3Basic_t *movieObject;
    EvtRuntimeGroup *trackEntry;
    s32 startFrame;
    s32 extra;
    EvtRuntimeChild *glyph;

    if (dds3GetActiveScene() != 0) {
        worldTable = (evtWorldTable_t *)((dds3Basic_t *)dds3GetActiveScene())->workdata;
        if (worldTable != 0) {
            worldSlots = worldTable->slots;
            if (worldSlots != 0) {
                movieObject = worldSlots[EVT_WORLD_SLOT_MOVIE].head;
                if (movieObject != 0) {
                    do {
                        trackEntry = viewer->groups;
                        while (trackEntry != NULL) {
                            if (trackEntry->info != NULL && movieObject == dds3ObjectBaseGetControlBasic(trackEntry->info, DDS3_OBJECT_DATA_SLOT)) {
                                if (trackEntry->type == 2) {
                                    startFrame = 0;
                                    if (trackEntry->childCount != 0) {
                                        startFrame = trackEntry->children->frame;
                                    }
                                    glyph = evtViewFindGlyphAtOrBefore(viewer);
                                    extra = 0;
                                    if (glyph != NULL) {
                                        extra = glyph->p0C.sh[1];
                                    }
                                } else {
                                    startFrame = trackEntry->metadata.value;
                                    extra = 0;
                                }
                                evtSetMovieClipPositionClampedToDuration(movieObject, 0, startFrame, endTime, extra);
                                break;
                            }
                            trackEntry = trackEntry->next;
                        }
                        movieObject = movieObject->next;
                    } while (movieObject != 0);
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B3990);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B3CA8);

/* Applies the latest kind-6 key at/before position to a kind-1 track's LOD
 * byte, provided the requested signed-byte level is supported by its chunk. */
void evtViewerApplyGlyphLodChannel(s32 position, dds_evtViewerInfo_t *viewer) {
    EvtRuntimeGroup *node = viewer->groups;
    EvtRuntimeChild *glyph;
    EvtRuntimeChild *best;
    s32 bestFrame;
    dds_sdfModelRootNode_t *lod;
    s8 level;

    while (node != NULL) {
        if (node->type == 1) {
            glyph = node->children;
            bestFrame = -1;
            best = NULL;
            if (glyph != NULL) {
                do {
                    if (position >= glyph->frame && bestFrame < glyph->frame && glyph->p08.sb[0] == 6) {
                        bestFrame = glyph->frame;
                        best = glyph;
                    }
                    glyph = glyph->next;
                } while (glyph != NULL);
            }
            lod = ((dds3ModelHandle_t *)((EffectObjectData *)node->info->workdata)->modelHolder->resourceHandle)->inner;
            if (best == NULL) {
                lod->lodIndex = 0;
            } else {
                level = best->p0C.sb[0];
                if ((s32)sdfGetLodChunkValue(lod) >= level) {
                    lod->lodIndex = best->p0C.sb[0];
                }
            }
        }
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerAttachIndexedObject);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B3FE0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B40B8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B40E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B4120);

u8 evtViewerHasUpdateFlag(dds_evtViewerInfo_t *viewer) {
    s32 flags = viewer->flags;
    return (flags & 0x10) > 0;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B4278);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B43B0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B45D8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B4608);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B46B0);

void func_r1_001B48A8(void) {
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B48B0);

void func_r1_001B4918(void) {
}

/* Returns the latest eligible kind-5 motion key at/before frame for channel,
 * or NULL. Its condition must pass; equal-frame ties retain the first key. */
EvtRuntimeChild *evtViewerFindLatestMatchingMotionKey(EvtRuntimeGroup *group, s32 frame,
                                                      MdlMotionSearchId channel) {
    s32 bestFrame = -1;
    EvtRuntimeChild *best = NULL;
    EvtRuntimeChild *key = group->children;

    if (key != NULL) {
        do {
            if (frame >= key->frame && bestFrame < key->frame && key->p08.sb[0] == 5 &&
                key->p0C.sb[0] == channel &&
                evtViewerTestIndexedCondition(key->p10.sh[0]) ==
                    EVT_VIEWER_INDEXED_CONDITION_MATCHED) {
                bestFrame = key->frame;
                best = key;
            }
            key = key->next;
        } while (key != NULL);
    }
    return best;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B49E8);

/* Dispatch one of two viewer modes based on its lowest flag bit. */
void evtViewerDispatchFlagMode(dds_evtViewerInfo_t *viewer) {
    if ((viewer->flags & 1) != 0) {
        evtEventViewerApplyPlaybackMode(EVT_VIEWER_PLAYBACK_PAUSED, viewer->nowframe, viewer);
        return;
    }
    evtEventViewerApplyPlaybackMode(EVT_VIEWER_PLAYBACK_RUNNING, viewer->nowframe, viewer);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B4CE8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B4D68);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B4DE8);

u16 evtViewerPopHistory(dds_evtViewerInfo_t *viewer) {
    u16 historyId;
    s32 historyIndex;

    historyIndex = viewer->historyCount - 1;
    if (viewer->historyCount == 0) {
        viewer->actionMode = 0;
        return 0;
    }
    viewer->historyCount = historyIndex;
    historyId = viewer->history[historyIndex].id;
    viewer->actionMode = (u32)historyId;
    return historyId;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B4E80);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B4F00);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B4F38);

u32 evtViewerReturnZero(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F070);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F080);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F090);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F0A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B5100);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B52E8);

u32 evtViewerPushMode5CommandHistory(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer) {
    evtViewerPushCommandHistory(5, 0x90, 0x48, viewer);
    return 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F2D0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F2E0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F2F0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F300);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F310);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F320);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F330);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F340);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F350);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F360);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F370);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F380);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F390);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F3A0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F3B0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F3C0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F3D0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F3E0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F3F0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F400);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F410);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044F420);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B5778);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B67F8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B6928);

/* Store the command value as a halfword and clear its extra halfword when tagged. */
s32 evtViewCmdSetValue(s32 unused0, s32 unused1, dds_evtViewerInfo_t *viewer) {
    s32 commandValue = viewer->value;
    EvtRuntimeChild *entry = evtEventViewerGetPendingNode(viewer);

    if (entry == NULL) {
        return 0;
    }
    entry->p08.h[0] = commandValue;
    if (((u32)(commandValue << 16) >> 28) != 0) {
        entry->p08.h[1] = 0;
    }
    evtApplyViewerTimelineFrame(viewer->nowframe, viewer);
    evtViewerPopHistory(viewer);
    return 0;
}

/* Store the command value in the selected halfword of a viewer entry. */
u32 evtViewerStoreCommandInSelectedField(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer) {
    EvtRuntimeChild *entry;
    u32 commandValue;
    s32 slot;
    commandValue = viewer->value;
    entry = evtEventViewerGetPendingNode(viewer);
    if (entry != 0) {
        slot = viewer->frameGroup->type - 1;
        if ((u32)slot < 0x13u) {
            switch (slot) {
            case 0:
                entry->p10.h[0] = commandValue;
                break;
            case 11:
            case 15:
            case 16:
                entry->p14.h[0] = commandValue;
                break;
            case 3:
                entry->p08.h[1] = commandValue;
                break;
            case 4:
                entry->p08.h[1] = commandValue;
                break;
            case 1:
                entry->p0C.h[0] = commandValue;
                break;
            case 18:
                entry->p0C.h[0] = commandValue;
                break;
            }
        }
        evtApplyViewerTimelineFrame(viewer->nowframe, viewer);
        evtViewerPopHistory(viewer);
        return 0;
    }
    return 0;
}

/* Registered viewer-command handler with no action; return 0. */
u32 evtViewCmdNoOp(void) {
    return 0;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B6CE0);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerCommitGroupSelection);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B6E30);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B6ED0);

u32 evtViewCmdCancelSelection(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer) {
    evtViewerPopHistory(viewer);
    return 0;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewCmdResolveSlot);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B7030);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B7088);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B7158);

u32 evtViewerClearPendingNodeAndPushHistory(u32 unused0, u32 unused1, dds_evtViewerInfo_t *viewer) {
    if (evtEventViewerGetPendingNode(viewer) != 0) {
        viewer->groupFirst = 0;
        viewer->groupCursor = 0;
        evtViewerPushCommandHistory(0xa, 0x9c, 0x54, viewer);
        return 0;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044FAA0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B7488);

/* Hand control back to the frame update whether or not the task still owns a viewer. */
dds3TaskUpdateResult_t evtViewerPickNextHandler(dds3ProcessID_t *task) {
    if (dds3GetProcessData(task) != 0) {
        return (s32)evtViewerUpdateFrame;
    }
    return (s32)evtViewerUpdateFrame;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B7790);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B78C8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B79F0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B7A90);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B7D88);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B7E78);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B7EB8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B7F48);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B7FD8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B7FE0);

void evtEventViewerDestroyActive(dds3ProcessID_t *task) {
    dds_evtViewerInfo_t *viewer;

    viewer = (dds_evtViewerInfo_t *)dds3GetProcessData(task);
    evtViewerRelease(viewer);
}

void evtEventViewerDestroyActiveAlternate(dds3ProcessID_t *task) {
    dds_evtViewerInfo_t *viewer;

    viewer = (dds_evtViewerInfo_t *)dds3GetProcessData(task);
    evtViewerRelease(viewer);
}

void evtEventViewerInitCampFontResource(dds_evtViewerInfo_t *viewer) {
    mnuCampInitFontResource(viewer);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B81F0);

void evtEventViewerDestroyTask(void) {
    dds3KillProcessName(evtViewerTaskName, 1);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B82B0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B82E0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B8340);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B8370);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B83A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B8438);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B8448);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B8450);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B8470);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B84B8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B84E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_r1_001B8598);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_r1_0044FC08);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6A8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6AA);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6AC);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6AE);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6B4);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6B8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6BC);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6C0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6C4);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6C8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6D0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6D8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6E0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6E8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6F0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C6F8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C700);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C708);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C710);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C718);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C720);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C728);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C730);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C738);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C740);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C748);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C750);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C758);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C760);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C768);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C770);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C778);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C780);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C788);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C790);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C798);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C7A0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C7A8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C7B0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C7B8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C7C0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C7C8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C7D0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C7D8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C7E0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_r1_0046C7E8);

