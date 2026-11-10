#include "common.h"

/* Declarations from dds-decomp src/dds2/game/code_00107EF8.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "event.h"

#include "kernel_draw.h"

#include "scr.h"

#include "sdfDevCons.h"

#include "sdf_packet_alloc.h"

#include "sdk_libc.h"

#include "dds/dds3_admin_control_api.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3_task_priority_query.h"

#include "dds/eff_quaternion_nlerp.h"

#include "dds/fld_scene_sequence_api.h"

#include "dds/fr_font.h"

#include "dds/kwln.h"

#include "dds/mnu_camp_state_ack_api.h"

#include "dds/scr.h"

#include "dds/sdf_asset_create.h"

#include "dds/sdf_asset_state.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_dev_cons.h"

#include "dds/sdf_dev_console_node.h"

#include "dds/sdf_general_heap_api.h"

#include "dds/sdf_gs_blend.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_line_pair_packet_api.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_append.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_packet_header.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_primitive_packet_api.h"

#include "dds/sdf_sif_command.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libkernl.h"

s32 DIntr();

s32 EIntr(void);

/* Blend the quaternion in vf10 toward vf11 along the shorter arc using
 * blendAmount, normalize the result, and leave it in vf10. Providers:
 * DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void Lerp(f32 blendAmount);

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void ToMatrix(void);

/* Consume CLEANED_UP back to IDLE, then report whether camp remains active. */
MnuCampStateAcknowledgementResult campCheckExitState(void);

/* Set bit 1 in the active administration task's flags; that task must exist. */
void dds3AdminiExitSequence(void);

struct sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

/* Return the current frame draw-buffer slot (0 or 1). */
u32 dds3GetFrameID(void);

dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

dds3TaskKillRequestResult_t dds3KillProcessName(const char *name, s32 delayTicks);

/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */
dds3ProcessID_t *dds3SearchProcessName(const char *name);

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

struct SdfPacket *effDebugDrawWireModel(const f32 vertices[][4], const u32 *colors,
                                                 s32 vertexCount, u32 primitiveFlags);

/* The packet is borrowed for this call. Mode zero copies its fields into
 * persistent scene work before asynchronous tasks start. */
void fldProcTrigSequence(const struct fldSceneParam_t *record, u32 mode);

/* Link a glyph chain into the current draw-buffer slot. */
s32 frDestroyFontRequestAfter(struct _FRQ *glyph);

void *memset(void *dst, s32 value, u32 size);

extern scrProcessWork_s *scrHandleFirst;

extern scrProcessWork_s *scrHandleLast;

SdfTextureDrawPacket *sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 context);

SdfDrawPacket *sdf2DMakePrimPacketHeader(SdfDrawPacket *packet, s32 primitiveFlags,
                                       s32 registerCount, s64 registerList,
                                       s32 loopCount);

void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32));

/* Append standard packets to a DMA packet list. A null allocator selects the
   default packet allocator. Asset packets reference the selected draw entry. */
void sdf3DPrimLinkResetTm(sdfDrawTag_t *packetList,
                              s32 (*allocatePacket)(s32));

/* Build the serialized VIF attribute stream and return its word cursor. */
u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *request);

void *sdfChipAllocDirect(s32 size);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

void sdfChipGetInfo(SdfChipHeapStats *stats);

void sdfDevConsAttribute(sdfDevConsCB_tag *console, u8 attribute);

void sdfDevConsClose(sdfDevConsCB_tag *node);

sdfDevConsCB_tag *sdfDevConsOpen(u32 screenOriginX, u32 screenOriginY, s32 columns, s32 rows);

s32 sdfDevConsPrintf(sdfDevConsCB_tag *console, const char *format, ...);

/* Update the console state stored at the corresponding field offsets. */
void sdfDevConsSetMode(sdfDevConsCB_tag *console, u8 controlByte);

void *sdfDevPrintfEx(SifCommand *packet, const char *format, ...);

void sdfDevSetPrintCB(SifCommand *packet, s32 source, s32 end, s32 argument, s32 index);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

void sdfDrawTagLinkRefGif(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

/* Read the active packet cursor or restore it at a quadword boundary. */
s32 sdfGrOpenPacketBuff(void);

/* Queue an asset owner for the deferred sdfMat_t release callback. */
void sdfMatFreeDelayed(struct sdfMat_t *asset);

struct sdfMat_t *sdfMatNew(void);

/* Store the primary or secondary texture owner and mark its draw state dirty. */
void sdfMatSetTex(struct sdfMat_t *asset, struct sdfTexHandle_t *texture);

void sdfMemGetInfo(SdfGeneralHeapStats *stats);

/* Return NULL when the following link is the end marker. */
const struct sdfMemHandle_t *sdfMemGetNext(const struct sdfMemHandle_t *block);

/* Return the descriptor's payload base as a byte address, not the descriptor. */
u8 *sdfMemGetPtr(const struct sdfMemHandle_t *block);

s32 sdfMemGetSize(const struct sdfMemHandle_t *block);

/* Borrowed read-only descriptors; exclude heap mutation while traversing. */
const struct sdfMemHandle_t *sdfMemGetTop(void);

/* Return free/used/end-sentinel state, not a resource kind. */
SdfMemBlockState sdfMemGetType(const struct sdfMemHandle_t *block);

void sdfPrimLinkGt4(
    sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
    s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2,
    s32 u2, s32 v2, s32 color2, s32 x3, s32 y3, s32 u3, s32 v3, s32 color3,
    s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimLinkLine(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimLinkRect(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *packet);

s32 vsprintf(char *dst, const char *format, void *args);

#include "dds/sdk_ps2.h"

#include "dds/evt_sel_state.h"

#include "dds/evt_start_event_api.h"

#include "dds/evt_script_event_id.h"

#include "dds/sdf_line_pair_packet_api.h"

#include "dds/sdf_gs_header.h"

#include "dds/sdf_dev_cons.h"

#include "dds/sdf_dev_console_node.h"

#include "dds/sdf_gs_blend.h"

#include "dds/common.h"

#include "dds/file_menu_start_mode.h"

#include "dds/sdf_vu0.h"

#include "dds/dds3_background_fade_api.h"

#include "dds/mnu_camp_state_ack_api.h"

#include "dds/fld_lmap_task_presence.h"

#include "dds/fld_terminal_scene_state.h"

#include "dds/dds3_admin_control_api.h"

#include "dds/sdf_heap_overlay_block.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_frame_draw_buffer.h"

#include "dds/evt_gradient_rect.h"

#include "dds/sdf_textured_rect.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_packet_header.h"

#include "dds/sdf_packet_append.h"

#include "dds/fr_font.h"

#include "dds/sdf_chip.h"

#include "dds/kwln.h"

#include "dds/dds3_task_priority_query.h"

#include "dds/dds3_task_create.h"

#include "dds/sdf_primitive_packet_api.h"

#include "dds/pcp_vu0.h"

#include "dds/ee_mmi.h"

#include "dds/eff_quaternion_nlerp.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf.h"

#include "dds/sdf_asset_state.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_general_heap_api.h"

#include "dds/sdf_sif_command.h"

#include "dds/scr.h"

#include "dds/scr_instruction_stream_api.h"

#include "dds/scr.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/brs_task.h"

#include "dds/fld_resource_wait_api.h"

#include "dds/fld_scene_sequence_api.h"

#include "dds/itf.h"

/* Declared before first use (were implicit int). */
extern void btlCreateStageTestTask(void);

extern s32 btlExitWhenAudioAndTasksIdle(void);

extern void evtBattleStageTestStopTask(void);

extern void evtFreeStage(void);

extern void evtInitEventTest(void);

extern void evtViewerCreateTaskWithSky(void);

extern void fileMenuReleaseNoOp(void);

extern void fileMenuWorkCreate(u32 startBranchFlag);

extern void fileReleaseMenuResources(void);

extern void fldProcNextJump(void);

extern void fldProc_CallSisetuReturn(void);

extern void fldRunPendingSceneAction(void);

extern void fldShutdownSceneTasksAndWorld(void);

extern void fldStopLmapTask(void);

extern void fldStopSceneTasks(void);

extern void itfInitializeAdminInterfaceNoop(void);

extern void itfShutdownAdminInterfaceNoop(void);

extern void itfStartFontTestScene(void);

extern void mdlCreateViewerPackageTask(void);

extern u32 mdlStopViewerPackageTask(void);

extern void mnuCampDestroyPanelTasks(void);

extern void dds3ConfigProcessEnd(void);

extern void mnuCreateCampShopPopupTasks(const s32 *stockGroup);

extern void mnuCreateCampTasks(void);

extern void dds3ConfigProcessStart(s32 mode);

extern void InitMovieViewer(void);

extern void mnuCreateTerminalSceneTasks(s32 reduced, s32 slot);

extern void mnuDestroyCampTasks(void);

extern void FinishMovieViewer(void);

extern s32 mnuDestroyTitleMenuTask(void);

extern u32 mnuOpenTitleMenuForMode(s32 mode);

extern dds3ProcessID_t *mnuStaffCreateTasks(void);

extern u32 mnuStaffDestroyTasks(void);

extern u32 mnuStartStaffMovieRequest(void);

extern s32 mnuStopStaffTasks(void);

enum {
    EVT_PACKET_LIST_BYTES = 0x20,
    EVT_GS_COMMAND_BYTES = 0x30,
    EVT_GS_TEST_PRIMARY = 0x47,
    EVT_GS_TEST_SECONDARY = 0x48,
    EVT_GS_ALPHA_PRIMARY = 0x42,
    EVT_FIXED_ALPHA_INTERPOLATION = 0x64,
    EVT_QUAD_DEFAULT_DEPTH = 0xFFFFFF,
    EVT_FRAME_REFERENCE_PACKET_BYTES = 0x40,
    EVT_FRAME_DRAW_RECORD_BYTES = 0x1F40,
    EVT_BACKGROUND_BLEND_FLAG = 0x100,
    EVT_DRAW_COLOR_BLEND_FLAG = 0x200,
    EVT_DRAW_VECTOR_BLEND_FLAG = 0x400,
    EVT_SAVED_VECTOR_BLEND_FLAG = 0x4000,
    EVT_QUAD_DESCRIPTOR_BYTES = 0x2C,
    EVT_QUAD_VERTEX_COUNT = 4,
    EVT_SELECTION_ALLOCATION_BYTES = 0x28,
    EVT_DEV_CONSOLE_BUFFER_BYTES = 0x200,
    BF_FLW0_MAGIC = 0x30574C46
};

extern s32 mdlViewerPackageTaskExists(void);

extern u32 D_00435D28;

extern u32 egl_OT_Offset;

extern s32 fileMenuOperationSucceeded(void);

extern s32 fileMenuTaskExists(void);

extern void fileStartMemoryCardMenuForMode(FileMenuStartMode mode);

extern u32 D_00435BB0;

extern s32 checkConfigState(void);

extern s32 mnuCampConsumePanelTaskCompletion(void);

extern FldTerminalScenePollResult fldPollSceneState(void);

extern u8 fldLmapTaskExists(void);

extern sdfDevConsCB_tag *evtConsoleFontResourceChain;

extern u32 dds3DrawFlag;

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

extern u16 D_00438E06;

extern f32 D_00438E08;

extern f32 D_00438E0C;

extern f32 D_00438E10;

extern f32 D_00438E14;

extern u16 D_00438E04;

#include "dds/sdf_asset_create.h"

extern void *D_00438DB0;

extern char D_00411370[];

extern void fldMainLoadStart(void);

extern char D_00435D20[];

extern void evtEventViewerDestroyTask(void);

extern char D_00411380[];

extern s32 D_00389780[];

extern void fldStartLmapTask(s32 mode);

extern u32 scrHandleCount;

extern sdfGrOt_t sdfDebugGrOt;

extern s8 D_0037F53B[];

extern void evtDrawHeapUsageOverlay(sdfGrOt_t *);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} DrawVec4;

/* The allocated draw state's float at +0x1C is initialized to 1. */
typedef struct EvtDrawState {
    u8 pad00[0x1C];
    f32 value1C;
} EvtDrawState;

extern KwlnDrawVectorParams kwlnDrawVector;

extern DrawVec4 D_0043E3A0;

extern DrawVec4 D_0043E3B0;

extern u16 D_00438DFA;

extern u16 D_00438DF8;

/* GS AD packet payload starts after the 0x20-byte command header. */
typedef struct EvtGsCommand {
    u8 pad00[0x20];
    u64 data;
    u64 registerId;
} EvtGsCommand;

#include "dds/sdf_packet_alloc.h"

extern void *sdf2DMakeStdPacket(void *, s32);

extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);

extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);

extern dds3FrameDrawBuffer_t dds3FrameDrawPacketRecords[];

extern void sdfGrSetFrameMaskRGB(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern void sdfGrSetFrameMaskReset(const u8 *, SdfDmaReferenceChainPacket *, s32);

extern f32 D_0037F5B0[][4];

extern u32 D_0037F5D0[];

extern s8 evtSelectionStateActive;

extern EvtSelState *evtSelectionState;

extern void evtSelStateDestroy(void);

extern void evtUpdateSelectionStateInterpolation(void);

extern u8 sdfViewShakeVector[];

extern s32 dds3TaskNoopCallback(dds3ProcessID_t *task);

extern dds3ProcessID_t *evtInitSky(void);

extern void evtChkEndProcess(void);

extern void evtExitSky(void);

extern void btlStart(s32 mode, s32 pack, s32 encounter);

extern s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *);

extern void *D_00438E64;

extern f32 *D_0037F770[];

extern u16 D_00438DC8[4];

extern u16 D_00438DD0[4];

extern u32 D_0043E320[4];

extern u32 D_0043E330[4];

/* Set an existing light slot's RGB target immediately or save packed blend endpoints. */
void dds3SetLightColor(s32 blendFrames, s32 lightIndex, const f32 *color);

/* Normalize the requested light direction; replace it immediately or prepare its blend. */
extern u16 D_00438DD8[4];

extern u16 D_00438DE0[4];

extern f32 D_0043E340[][4];

extern f32 D_0043E370[][4];

extern void dds3RotationArc(void);

void dds3SetLightDir(s32 blendFrames, s32 lightIndex, const f32 *direction);

extern u16 D_00438DE6;

extern u16 D_00438DE8;

extern u32 D_00438DEC;

extern u32 D_00438DF0;

extern u8 kwlnDefaultColorVector[];

/* vu0 routine: replace the background color immediately or blend over blendFrames frames. */
void dds3SetAmbientColor(s32 blendFrames, f32 *color);

extern u32 D_0037F7A0[];

extern u16 D_00438DF4;

extern u16 D_00438DF6;

extern u32 D_00438DFC;

extern u32 D_00438E00;

/* vu0 routine: set the draw colour target: immediately (mode 0) or interpolate from the previous colour. */
void dds3SetFogColor(s32 blendFrames, f32 *color);

/* Either replace the draw vector immediately or interpolate from its prior value. */
void dds3SetFogPara(s32 blendFrames, f32 x, f32 y, f32 z, f32 w);

/* SDK float-input unpack: the RGBA word and its 16-byte workspace stay in memory. */
static inline void eeUnpackRGBAWordFloatReadOnly(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK_READONLY(rgba, unit);
}

/* The color-blend site passes ratio and the 128.0f pack scale as ordinary GPR operands
   (VU0_SCALE_VF / eePackRGBAWord), so their SF->GPR reloads advance the spill ring
   ($2,$3,$4) and the vector-blend site starts at $4 (cc1probe --reload, DDS1 uid650). */
void dds3SceneLightUpdateFirst(void);

/* Replace the two scalar targets immediately, or retain start/end values for their blend. */
void dds3SetSoftenBias(s32 frames, f32 firstTarget, f32 secondTarget);

void eglInit(void);

void eglSetOt(u32 surfaceIndex);

/* Submit primary-context GS TEST settings. Z testing is always enabled;
   unusedZte is a retained native formal, not the source of the ZTE bit. */
void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);

/* Submit the same TEST bit layout for the secondary GS context, with ZTE forced on. */
void evtSubmitSecondaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);

/* Select a primary-context ALPHA equation by mode; unknown modes use 0x44. */
void evtSubmitPrimaryAlphaBlendMode(s32 blendMode);

/* Submit ALPHA_1 = (Cs - Cd) * FIX / 128 + Cd; retain the native unmasked FIX input. */
void evtSubmitFixedAlphaBlend(s32 fixedAlpha);

void evtSubmitPrimaryFramePacket(void);

void evtSubmitAlternateFramePacket(void);

void evtSubmitTexturedGradientQuad(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3, sdfTexHandle_t *texture);

/* Native rectangle emitter: x/y/width/height, explicit depth, then TL/TR/BR/BL colors. */
void evtSubmitGradientRectAtDepth(s32 x, s32 y, s32 w, s32 h, u32 depth, s32 color0, s32 color1, s32 color2, s32 color3);

/* Submit the four-corner gradient rectangle with the native default depth.
   The inserted 0xFFFFFF is depth, not a white color. */
void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor);

void eglLinkTri(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 color0, s32 color1, s32 color2);

extern u64 D_0037F5A0[];

/* Preserve the native 0,2,3,1 vertex/index order; W components are not initialized here. */
void eglLinkPoly4(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3, f32 y3, f32 z3, u32 i0, u32 i1, u32 i2, u32 i3);

/* Submit the same native vertex permutation with rectangular UVs, then queue asset release. */
void eglLinkTexPoly4(s32 i0, f32 x0, f32 y0, f32 z0, s32 i1, f32 x1, f32 y1, f32 z1, s32 i2, f32 x2, f32 y2, f32 z2, s32 i3, f32 x3, f32 y3, f32 z3, u32 bits, f32 u0, f32 v0, f32 u1, f32 v1);

void eglLinkLine(f32 x, f32 y, f32 z, f32 u, f32 v, f32 w, u32 first, u32 second);

void eglLinkPrint(s32 x, s32 y, s32 packetArg, s32 drawArg);

void eglLinkKanji(s32 width, s32 height, s32 colors, const char *text);

/* Zero frames leaves the prior state untouched; zero countLimit selects the -1 sentinel. */
s32 evtSelStateCreate(s32 countLimit, s16 frames, s32 firstValue, s32 secondValue);

/* Return zero only when inactive on entry; updating or destroying active state returns one. */
u32 evtCheckSelectionState(void);

void evtDestroySelectionState(void);

void evtSelStateDestroy(void);

dds3ProcessID_t *CallDebug(void);

u32 ExitDebug(void);

/* Admin mode 0 cleanup: zero leaves the mode pending. */
u32 CheckDebug(void);

void CallBootMc(void);

u32 ExitBootMc(void);

u8 CheckBootMc(void);

u32 CallTitle(u32 unused, u32 *modeOverride);

u32 ExitTitle(void);

/* Admin mode 2 cleanup: zero leaves the mode pending. */
u32 CheckTitle(void);

void CallLoad(void);

s32 ExitLoad(void);

u8 CheckLoad_dds3SequenceList(void);

u32 CallStaff(void);

u32 ExitStaff(void);

/* Admin mode 12 cleanup: zero leaves the mode pending. */
u32 CheckStaff(void);

void CallEndSave(void);

u32 ExitEndSave(void);

u8 CheckEndSave(void);

void CallField(u32 selection, const struct fldSceneParam_t *record);

u32 ExitField(void);

/* Admin mode 5 cleanup: zero leaves the mode pending. */
u32 CheckField(void);

void evtStartSelectionFadeOut(void);

/* Admin mode 11 destroy callback: no work, zero transition result. */
u32 evtDestroySelectionFadeOutNoOp(void);

/* Admin mode 11 cleanup: zero leaves the mode pending. */
u32 evtKeepSelectionFadeOutPending(void);

/* Source zero takes an explicit event ID; source one uses the pending event ID. */
void CallEvent(s32 selectionSource, EvtScriptEventId *eventParams);

u32 ExitEvent(void);

/* Admin mode 6 cleanup: zero leaves the mode pending. */
u32 CheckEvent(void);

void CallCamp(void);

u32 ExitCamp(void);

u8 CheckCamp(void);

/* Admin mode 8 entry intentionally performs no work. */
void CallRecover(void);

/* Admin mode 8 destroy callback: no work, zero transition result. */
u32 ExitRecover(void);

/* Admin mode 8 cleanup: one immediately reports completion. */
u32 CheckRecover(void);

/* Admin mode 9 entry intentionally performs no work. */
void CallTerminal(void);

/* Admin mode 9 destroy: no work, zero transition result. */
u32 ExitTerminal(void);

/* Admin mode 9 cleanup: one immediately reports completion. */
u32 CheckTerminal(void);

s32 CallResult2(void);

u32 ExitResult2(void);

u8 CheckResult2(void);

void CallConfig(void);

u32 ExitConfig(void);

u8 CheckConfig(void);

/* Source zero forwards an optional parameter pair; other sources resume deferred field work. */
void evtDispatchSelectionCommand(s32 commandSource, s32 *commandParams);

/* Check battle shutdown; the callee exits only after audio and task work become idle. */
s32 evtCheckBattleShutdown(void);

u32 CheckNewBattleTest(void);

void evtCreateBattleStageTestTask(void);

/* Stop the battle-stage test task and return the scene callback's native zero status. */
u32 evtEndBattleStageTest(void);

/* Admin mode 15 cleanup: zero leaves the mode pending. */
u32 evtKeepBattleStageTestModePending(void);

dds3ProcessID_t *evtCreateTestAndSkyTasks(void);

u32 evtInitializeSelectionScene(void);

/* Admin mode 16 cleanup: zero leaves the mode pending. */
u32 evtKeepTestSkyModePending(void);

void evtClearSecondaryWorldAndViewer(void);

u32 evtInitializeSelectionScreen(void);

/* Admin mode 17 cleanup: zero leaves the mode pending. */
u32 evtKeepEventViewerModePending(void);

/* Admin mode 18 entry intentionally performs no work. */
void evtAdminModeEighteenEntryNoOp(void);

/* Admin mode 18 destroy: no work, zero transition result. */
u32 evtAdminModeEighteenDestroyNoOp(void);

/* Admin mode 18 cleanup: zero leaves the mode pending. */
u32 evtKeepAdminModeEighteenPending(void);

void evtCreateCampTasksAlternate(void);

u32 evtExitCampTaskGroup(void);

u8 mnuCheckCampStateAndAcknowledge(void);

void evtCreateSkyAndCampTasks(u32 unused, u32 campMode);

u32 evtDestroySkyAndCampTasks(void);

u8 CheckRecoverTest(void);

void CallTerminalTest(u32 unused, u32 *fieldArgs);

u32 ExitTerminalTest(void);

u8 CheckTerminalTest(void);

s32 evtCreateStaffTasksAlternate(void);

u32 evtDestroyStaffTasksAlternate(void);

u8 evtWaitStaffTaskDoneAlternate(void);

/* Admin mode 23 entry intentionally performs no work. */
void CallSoundTest(void);

/* Admin mode 23 destroy: no work, zero transition result. */
u32 ExitSoundTest(void);

/* Admin mode 23 cleanup: zero leaves the mode pending. */
u32 CheckSoundTest(void);

/* Admin mode 24 entry intentionally performs no work. */
void CallBillboardTest(void);

/* Admin mode 24 destroy: no work, zero transition result. */
u32 ExitBillboardTest(void);

/* Admin mode 24 cleanup: zero leaves the mode pending. */
u32 CheckBillboardTest(void);

void CallFontTest(void);

/* Admin mode 25 destroy callback: no work, zero transition result. */
u32 ExitFontTest(void);

u32 CheckFontTest(void);

void evtCreateMovieViewerTask(void);

u32 evtDestroyMovieViewerTask(void);

/* Admin mode 26 cleanup: zero leaves the mode pending. */
u32 evtKeepMovieViewerModePending(void);

void evtEnterInterfaceMode(void);

u32 evtDestroyInterfaceMode(void);

u32 evtKeepInterfaceModePending(void);

void evtSelectFontResource(s32 unused, s32 *lmapArgs);

u32 evtDestroyLmapTask(void);

u8 evtWaitLmapTaskGone(void);

/* Start the shooting-game task: the callee allocates MnuShootingWork and starts "stgProcess". */
void evtStartShootingGameTask(void);

/* Destroy that shooting-game task hierarchy and invoke its work callback; return native zero status. */
u32 evtStopShootingGameTask(void);

/* Complete the wait only after the named "stgProcess" task is absent. */
u8 evtWaitShootingGameTaskGone(void);

void evtOpenFileMenuFromParams(u32 unused, u32 *menuArgs);

extern void fileReleaseMenuFlowResource();

s32 evtPollFileMenuAndRelease(void);

u8 evtWaitFileMenuTaskFour(void);

u32 evtEnsureFontResourceChain(void);

void evtDestroyFontResourceChain(void);

void datDebugPrintf(const char *format, ...);

extern u16 D_00435D40;

extern u16 D_00435D42;

extern char evtGeneralHeapPercentFormat[];

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *task);

void evtGeneralHeapGraphTaskDestroy(dds3ProcessID_t *task);

void evtToggleDebugTimeGraphTask(s8 mode);

extern u32 D_00435D4C;

/* Render an allocation span as partial rows and complete heap-map rows. */
void sdfDrawHeapSpanOverlay(sdfDrawTag_t *packetList, s32 x, s32 y,
                   u32 heapBase, u32 blockAddress, s32 blockSize, u8 blockState);

void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y);

extern char D_00435D50[];

extern char D_00435D58[];

extern void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y);

/* Format general/chip free-memory statistics into the supplied draw surface.
   Keep the native heap-ratio coordinate calculation and title-specific initial packet. */
void evtDrawHeapUsageOverlay(sdfGrOt_t *surface);

/* Draw the overlay only when the control byte is zero; return zero in either case. */
s32 evtDrawConditionalHeapUsageOverlay(void);

extern s32 frGetFontHandleNum(void);

extern s32 frGetTextHandleNum(void);

extern s32 effGetFontListCount(void);

extern const char D_004113A0[];

s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task);

void evtResourceCountDebugTaskDestroy(dds3ProcessID_t *task);

extern s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task);

extern void evtResourceCountDebugTaskDestroy(dds3ProcessID_t *task);

extern dds3ProcessID_t *D_00438E68;

extern char D_004113B8[];

void evtToggleAlternateDebugTimeGraphTask(s8 mode);

/* Append to the event-work doubly linked list, maintaining both endpoints. */
void scrLinkHandle(scrProcessWork_s *node);

/* Detach from either end or the middle, and clear the old links. */
void scrUnlinkHandle(scrProcessWork_s *node);

extern ItfMesWindowIndex itfMesMngInitialize(ItfMesSub *data);

scrProcessWork_s *scrPrintProcedureStart(const scrHeader_t *header, const scrTypeTable_t *procedureSection,
                        const scrProcTable_t *procedures, const scrLabelTable_t *labels,
                        const scrCode_u *instructions, struct ItfMesSub *auxiliaryData,
                        const char *strings, ScrProcedureIndex procedureIndex);

/* Resolve supported FLW0 sections relative to the header; reject bad magic/unknown kinds.
   Zero-count auxiliary sections are ignored, and absent sections remain null. */
scrProcessWork_s *fldFlowParse(const scrHeader_t *header, ScrProcedureIndex procedureIndex);
/* End of declarations from src/dds2/game/code_00107EF8.c */

u32 evtEnsureFontResourceChain(void) {
    if (evtConsoleFontResourceChain == 0) {
        evtConsoleFontResourceChain = sdfDevConsOpen(0x7100, 0x7A60, 0x28, 0x14);
        sdfDevConsSetMode(evtConsoleFontResourceChain, 2);
        sdfDevConsAttribute(evtConsoleFontResourceChain, 7);
    }
    return 0;
}

void evtDestroyFontResourceChain(void) {
    if (evtConsoleFontResourceChain != 0) {
        sdfDevConsClose(evtConsoleFontResourceChain);
        evtConsoleFontResourceChain = 0;
    }
}

INCLUDE_ASM(const s32, "code_0010F130", datDebugPrintf);

INCLUDE_ASM(const s32, "code_0010F130", func_0010F238);

s32 evtDrawGeneralHeapUsageGraph(struct dds3ProcessID_t *task) {
    SdfGeneralHeapStats heapStats;
    sdfDrawTag_t *list;
    void *packet;
    s32 freePercent;
    s32 originX = 0x7100;
    s32 currentColumn;
    s32 peakColumn;

    sdfMemGetInfo(&heapStats);
    freePercent = heapStats.freeBytes * 100 / heapStats.totalBytes;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, (u32)packet);
    sdfDrawTagLink(list, (u32)dds3PrimFrameRect(0x70D0, 0x85A8, 0xFFFF7F, 0x1260, 0xC0, 0x20000000, 0x40806040));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7100, 0x85C0, 0xFFFF80, 0, evtGeneralHeapPercentFormat, freePercent));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x8080, 0x85C0, 0xFFFF80, 6, evtGeneralHeapPercentFormat, D_00435D40));
    currentColumn = (s32)((f32)freePercent / 100.0f * 200.0f) + 0x2A;
    sdfPrimLinkRect(list, 0x8000C000, 0, 0x73A0, 0x85F0, currentColumn * 16 + originX, 0x8600, 0xFF0000, 0);
    peakColumn = (s32)((f32)D_00435D40 / 100.0f * 200.0f) + 0x2B;
    sdfPrimLinkRect(list, 0x8000FFFF, 0, peakColumn * 16 + originX, 0x85D0, (peakColumn + 2) * 16 + originX, 0x8610, 0xFF0000, 0);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    if (D_00435D42 >= 0x47 || D_00435D40 < freePercent) {
        D_00435D40 = freePercent;
        D_00435D42 = 0;
    } else {
        D_00435D42 = D_00435D42 + 1;
    }
    return 0;
}

void func_0010F570(void) {
}

INCLUDE_ASM(const s32, "code_0010F130", func_0010F578);

/* Render an allocation span as partial rows and complete heap-map rows. */
void sdfDrawHeapSpanOverlay(sdfDrawTag_t *packetList, s32 x, s32 y,
                   u32 heapBase, u32 blockAddress, s32 blockSize, u8 blockState) {
    s32 color;
    u32 byteOffset;
    s32 rowIndex;
    s32 columnIndex;
    s32 leftX;
    s32 topY;
    u32 bytesPerRow;
    u32 bytesPerCell;

    switch (blockState) {
    case SDF_MEM_BLOCK_STATE_FREE:
        color = 0x80000000;
        break;
    case SDF_MEM_BLOCK_STATE_USED:
        color = 0x80008080;
        break;
    case SDF_MEM_BLOCK_STATE_END_SENTINEL:
        color = 0x80000080;
        break;
    default:
        color = 0x80800000;
        break;
    }
    byteOffset = blockAddress - heapBase;
    bytesPerRow = D_00435D4C >> 8;
    bytesPerCell = D_00435D4C >> 14;
    rowIndex = byteOffset / bytesPerRow;
    columnIndex = (byteOffset % bytesPerRow) / bytesPerCell;
    if (D_00435D4C < blockSize || blockSize < 0) {
        return;
    }

    topY = y + rowIndex * 8;
    leftX = x + columnIndex * 16;
    if (columnIndex > 0) {
        s32 remainingCells = 64 - columnIndex;

        if (blockSize >= remainingCells / bytesPerCell) {
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + remainingCells * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            blockSize -= remainingCells / (D_00435D4C >> 14);
        } else {
            s32 cellCount = blockSize / bytesPerCell;

            if (cellCount == 0 && blockSize > 0) {
                cellCount = 1;
            }
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + cellCount * 16, topY + 8,
                                         0x0FFFFF80, NULL);
            blockSize = 0;
        }
        topY += 8;
    }
    while (blockSize >= (D_00435D4C >> 8)) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + 0x400, topY + 8, 0x0FFFFF80, NULL);
        blockSize -= D_00435D4C >> 8;
        topY += 8;
    }
    if (blockSize > 0) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + (blockSize / (D_00435D4C >> 14)) * 16,
                                     topY + 8, 0x0FFFFF80, NULL);
    }
}

void sdfDrawHeapMapOverlay(sdfDrawTag_t *packetList, s32 x, s32 y) {
    SdfHeapOverlayBlock *snapshot;
    SdfHeapOverlayBlock *entry;
    const sdfMemHandle_t *block;
    u32 count;
    u32 i;
    u32 heapBase;
    u32 offset;
    s32 restoreInterrupts;
    s32 markerX;
    s32 markerY;

    snapshot = (SdfHeapOverlayBlock *)sdfGrOpenPacketBuff();
    restoreInterrupts = DIntr();
    block = sdfMemGetTop();
    entry = snapshot;
    heapBase = sdfMemGetPtr(block);
    /* Count only the interrupt-protected snapshot traversal. */
    count = 0;
    do {
        count++;
        entry->address = sdfMemGetPtr(block);
        entry->blockState = (u8)sdfMemGetType(block);
        entry->size = sdfMemGetSize(block);
        entry++;
        block = sdfMemGetNext(block);
    } while (block != NULL);
    if (restoreInterrupts) {
        EIntr();
    }
    sdfGrClosePacketBuff((s32)entry);

    entry = snapshot;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x - 0x20, y - 0x10,
        0x0FFFFF7F, 0x440, (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x20,
        0x80000000, 0x80806020));
    for (i = 0; i < count; i++) {
        sdfDrawHeapSpanOverlay(packetList, x, y, heapBase, entry->address,
            entry->size, entry->blockState);
        entry++;
    }
    markerY = y;
    markerX = x + 0x420;
    sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
        x + 0x4C0, markerY, 0x0FFFFF80, NULL);
    for (offset = 0x100000; offset < D_00435D4C; offset += 0x100000) {
        markerY = y + (offset / (D_00435D4C >> 8)) * 8;
        sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
            x + 0x470, markerY, 0x0FFFFF80, NULL);
    }
    markerY = y + (D_00435D4C / (D_00435D4C >> 8)) * 8;
    sdfPrimLinkLine(packetList, 0x80806020, 0, markerX, markerY,
        markerX + 0xA0, markerY, 0x0FFFFF80, NULL);
}

/* Format general/chip free-memory statistics into the supplied draw surface.
   Keep the native heap-ratio coordinate calculation and title-specific initial packet. */
void evtDrawHeapUsageOverlay(sdfGrOt_t *surface) {
    SdfGeneralHeapStats generalHeapStats;
    SdfChipHeapStats chipHeapStats;
    char statusText[100];
    sdfDrawTag_t *packetList;

    sdfMemGetInfo(&generalHeapStats);
    D_00435D4C = generalHeapStats.totalBytes;
    sdfChipGetInfo(&chipHeapStats);
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(packetList);
    sdfDrawHeapMapOverlay(packetList, 0x8AC0, 0x79C0);
    sprintf(statusText, D_00435D50, generalHeapStats.freeBytes);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x86C0,
        (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x7A00, 0x0FFFFF80, 0, statusText));
    sprintf(statusText, D_00435D58, chipHeapStats.freeBytes);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x86C0,
        (D_00435D4C / (D_00435D4C >> 8)) * 8 + 0x7A60, 0x0FFFFF80, 0, statusText));
    surface->regFunc(surface, packetList);
}

/* Draw the overlay only when the control byte is zero; return zero in either case. */
s32 evtDrawConditionalHeapUsageOverlay(void) {
    if (D_0037F53B[0] != 0) {
        return 0;
    }
    evtDrawHeapUsageOverlay(&sdfDebugGrOt);
    return 0;
}

s32 evtDrawResourceCountDebugTask(dds3ProcessID_t *task) {
    SdfGeneralHeapStats heapStats;
    s32 heapRatio;
    sdfDrawTag_t *list;
    void *packet;
    s32 fontCount;
    s32 textCount;
    s32 gsCount;

    sdfMemGetInfo(&heapStats);
    /* The unused heap ratio retains the native zero-divisor check. */
    heapRatio = heapStats.freeBytes / heapStats.totalBytes;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EVT_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    packet = (void *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(packet);
    sdfDrawTagLink(list, (u32)packet);
    sdfDrawTagLink(list, (u32)dds3PrimFrameRect(0x70d0, 0x7968, 0xffff7f,
                                          0xf70, 0x98, 0x20000000, 0x40806040));
    fontCount = frGetFontHandleNum();
    textCount = frGetTextHandleNum();
    gsCount = effGetFontListCount();
    sdfDrawTagLink(list, (u32)sdfDevPrintf(0x7100, 0x7980,
                    0xffff80, 0, D_004113A0, fontCount, textCount, gsCount));
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    return 0;
}

INCLUDE_RODATA(const s32, "code_0010F130", D_004A7288);

INCLUDE_SDATA(const s32, "code_0010F130", D_004DE410);

INCLUDE_SDATA(const s32, "code_0010F130", D_004DE418);

INCLUDE_SDATA(const s32, "code_0010F130", D_004DE420);

INCLUDE_SDATA(const s32, "code_0010F130", D_004DE428);

INCLUDE_SDATA(const s32, "code_0010F130", D_004DE430);

INCLUDE_SDATA(const s32, "code_0010F130", D_004DE434);

INCLUDE_SDATA(const s32, "code_0010F130", D_004DE438);

INCLUDE_SDATA(const s32, "code_0010F130", D_004DE440);

