#include "common.h"

/* Declarations from dds-decomp src/dds2/game/code_002C96D0.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "common.h"

#include "fileManager.h"

#include "newdata.h"

#include "sdf_resource.h"

#include "sdk_libc.h"

#include "sdk_libkernl.h"

#include "dds/bill_object_api.h"

#include "dds/btl.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/dds3_fade.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3_world_primary.h"

#include "dds/eff_random.h"

#include "dds/evt_world.h"

#include "dds/file_request_api.h"

#include "dds/file_request_entry.h"

#include "dds/file_slot_flags.h"

#include "dds/fr_font.h"

#include "dds/fr_font_context.h"

#include "dds/kwln.h"

#include "dds/mc_path_api.h"

#include "dds/mc_poll.h"

#include "dds/mc_read_api.h"

#include "dds/mc_write_api.h"

#include "dds/mdl_flag_api.h"

#include "dds/sdf.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_dev_state.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_quaternion.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf_vu_matrix_multiply_api.h"

#include "dds/sdf_vu_transform_api.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libkernl.h"

#include "dds/sdk_libm.h"

#include "dds/snd_sequence_volume_pan.h"

/* Convert three Euler angle components to the VU0 quaternion output.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void FromEuler(f32 x, f32 y, f32 z);

/* Multiply the quaternions in vf10 and vf11, writing the product to vf10. */
void Mult(void);

/* Load matrix into the VU0 scratch bank and postmultiply the primary bank. */
void MultMatrixRev(const struct Matrix4 *matrix);

/* Set vf28-vf31 to vf28-vf31 * vf24-vf27 in place. */
void MultMatrixRevNoArg(void);

/* Rigid-invert the VU matrix in vf28-vf31 in place; translation becomes
 * -(R^T * t). The rotation input is assumed to contain no scale. */
void ReverseMatrix(void);

/* Load axis-angle rotation into VU0 vf28-vf31. The angle-first interface
 * follows the native Flash caller at DDS2 00171FF8..00172000; EE's separate
 * f12/a0 argument registers alone do not distinguish the two source orders. */
void SetRotateMatrixAxis(f32 angle, const RwV3d *axis);

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void ToMatrix(void);

f32 acosf(f32 x);

f32 atan2f(f32 y, f32 x);

struct billHandle_t *dds3BillboardCopy(const struct billHandle_t *source);

/* The selected kind interprets the opaque payload word. */
struct billHandle_t *dds3BillboardCreate(s32 kind, u32 data);

void dds3BillboardDelete(struct billHandle_t *obj);

struct billHandle_t *dds3BillboardGeneral(s32 index);

void dds3BillboardSetAnimColorAdvMode(struct billHandle_t *billboard);

void dds3BillboardSetAnimKey(struct billHandle_t *billboard, u32 frame);

void dds3BillboardSetMode(struct billHandle_t *billboard, s32 mode);

void dds3BillboardSetRGBA(struct billHandle_t *billboard, u32 parameter);

/* Radian rotation of the billboard quad in its plane. */
void dds3BillboardSetRotate(struct billHandle_t *billboard, f32 angle);

void dds3BillboardSetScale(struct billHandle_t *billboard, f32 x, f32 y);

/* Return nonzero while either foreground fade direction is enabled. */
s32 dds3FadeCheck(void);

/* Return the borrowed primary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetActiveScene(void);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(EffRandState *state);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(EffRandState *state);

dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

dds3TaskKillRequestResult_t dds3KillProcessName(const char *name, s32 delayTicks);

/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */
dds3ProcessID_t *dds3SearchProcessName(const char *name);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

/* Preserve the signed-byte drawing gate; only a missing payload is ignored. */
void dds3VisibleSceneBasic(dds3Basic_t *object, s8 enabled);

/* Return whether the selected flag bit is set, as EvtBitTestResult. */
EvtBitTestResult evtBitCheck(s32 flag);

void evtBitOff(s32 flag);

/* Set and clear one flag using the providers' signed-index adjustment. */
void evtBitOn(s32 flag);

/* Return the address of the loaded resource payload as a byte pointer. */
u8 *fileGetLoadPtr(const struct FileRequest *request);

/* Return the signed byte count recorded by the file transfer request. */
s32 fileGetLoadSize(const struct FileRequest *request);

/* Return the payload allocation descriptor. For normal file loads, freeing the
 * request wrapper does not free this allocation; its resource owner must keep
 * or release it separately. */
struct sdfMemHandle_t *fileGetMemHandle(const struct FileRequest *request);

/* Free a completed request wrapper; return one while it remains pending. */
FileRequestCleanupResult fileHandleFree(struct FileRequest *request);

void fileSync(struct FileRequest *request);

/* Link a glyph chain into the current draw-buffer slot. */
s32 frDestroyFontRequestAfter(struct _FRQ *glyph);

void frSetDrawPosition(struct _FRQ *glyph, s32 x, s32 y);

/* Replace each child render word along the borrowed glyph chain. */
void frSetFontColor(struct _FRQ *parentNode, u32 colorWord);

/* Update the first-option byte of every child on this glyph chain. */
void frSetFontColorNo(struct _FRQ *glyph, u8 firstOption);

void frSetFontSpace(struct _FRQ *glyph, s32 spacing);

extern void *memcpy(void *, const void *, unsigned int);

/* Apply volume and pan to a sequence track through the command queue. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

s32 pacFileCheckEnd(const struct FileRequest *request);

/* These title-specific wrappers forward an opaque stage-task owner and its
 * handled/not-handled result. */
s32 pacFileHandleFree_(void *request);

struct FileRequest *pacLoad(const char *requestName);

void pacSync(struct FileRequest *request);

s32 sceClose(s32 fd);

s32 sceSync(const char *device, s32 flag);

/* Allocate from the chip heap through 0x400 bytes; larger requests use the
 * general heap and return its locked payload address. */
void *sdfAllocAuto(s32 size);

void *sdfChipAllocDirect(s32 size);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

f32 sdfCos(f32 angle);

void sdfFileClose(sdfFileCmdHandle_t *state);

u32 sdfFileLength(sdfFileCmdHandle_t *state);

/* Stable state APIs. Callback factories keep caller-specific callback types. */
sdfFileCmdHandle_t *sdfFileOpen(const char *path);

void sdfFileRead(sdfFileCmdHandle_t *state, void *buffer, s32 byteCount);

/* Release the sdfAllocAuto base in its source heap; interior pointers are
 * invalid. */
s32 sdfFreeAuto(void *data);

void sdfMcClearCardStat(s32 request);

void sdfMcClearSlotFlags(s32 request, s32 slot);

void sdfMcFuncChdir(McPort port, const char *path);

McPollResult sdfMcFuncChdir1(void);

McPollResult sdfMcFuncClose1(void);

void sdfMcFuncDelete(McPort port, const char *path);

McPollResult sdfMcFuncDelete1(void);

void sdfMcFuncFormat(u32 port);

McPollResult sdfMcFuncFormat1(void);

void sdfMcFuncGetDir(McPort port, const char *path, sceMcTblGetDir *directoryEntries, s32 entryLimit);

McPollResult sdfMcFuncGetDir1(McDirectoryEntryCount *entryCountOut);

FileReqPollResult sdfMcFuncGetInfo1(void);

void sdfMcFuncMkdir(McPort port, const char *path);

McPollResult sdfMcFuncMkdir1(void);

void sdfMcFuncOpen(McPort port, const char *path, McOpenMode openMode);

McPollResult sdfMcFuncOpen1(McFileDescriptor *fileDescriptorOut);

/* Queue a memory-card read into a caller-owned buffer. The SDK keeps using
 * that buffer asynchronously, so keep it allocated and do not access it until
 * sdfMcFuncRead1 returns a result other than MC_POLL_PENDING. */
void sdfMcFuncRead(McFileDescriptor fileDescriptor, void *buffer, s32 size);

McPollResult sdfMcFuncRead1(void);

/* Queue a memory-card write of a signed byte count, matching sceMcWrite.
 * The SDK reads this caller-owned buffer asynchronously; keep it allocated
 * and unchanged until sdfMcFuncWrite1 returns a result other than
 * MC_POLL_PENDING. */
void sdfMcFuncWrite(McFileDescriptor fileDescriptor, const void *data, s32 byteCount);

McPollResult sdfMcFuncWrite1(void);

FileReqStatus sdfMcGetFileInfoReady(s32 request);

s32 sdfMcGetFreeBytes(s32 request);

u32 sdfMcGetSlotFlags(s32 request, s32 slot);

s8 sdfMcGetSlotIndex(s32 request);

u8 sdfMcGetStatus(s32 request);

void sdfMcRegetInfo(s32 request);

void sdfMcSetCardStat(s32 request);

void sdfMcSetSlotFlags(s32 request, s32 slot, s32 mask);

void sdfMcSetSlotIndex(s32 request, s32 selectedSlot);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

/* Queue the two heap owners separately: the general heap stores descriptors,
 * while the chip heap stores the address of the cell to release. */
void sdfMemFreeDelayedDirect(struct sdfMemHandle_t *allocation);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

void sdfMemFreeHandleDirect(struct sdfMemHandle_t **slot);

/* Return the descriptor's payload base as a byte address, not the descriptor. */
u8 *sdfMemGetPtr(const struct sdfMemHandle_t *block);

void *sdfMemLock(struct sdfMemHandle_t *allocation);

f32 sdfSin(f32 angle);

void sdfTexFreeDelayed(sdfTexHandle_t *texture);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */
/* For a new texture, copy payload bytes into queue-owned storage before return. */
struct sdfTexHandle_t *sdfTexSet(const SdfTextureFileHeader *header);

char *strcpy(char *dst, const char *src);

u32 strlen(const char *text);

#define SDK_SKIP_memset

#include "dds/sdk_ps2.h"

#include "dds/file_menu_start_mode.h"

#include "dds/eff_surface_node.h"

#include "dds/common.h"

#include "dds/pty_active_roster_index_api.h"

#include "dds/file_menu_state_result.h"

#include "dds/file_config_list_node.h"

#include "dds/file_config_option_api.h"

#include "dds/sdf_vu0.h"

#include "dds/file_job_type.h"

#include "dds/file_config_task.h"

#include "dds/eff_emitter_type.h"

#include "dds/file_load_menu_state.h"

#include "dds/eff.h"

#include "dds/cam_aim_record.h"

#include "dds/eff_generated_texture_descriptor.h"

#include "dds/dds3_world_selection.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/sdf_trig.h"

#include "dds/eff_scale_params.h"

#include "dds/ee_mmi.h"

#include "dds/eff_random.h"

#include "dds/sdf_quaternion.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/sdf_textured_rect.h"

#include "dds/itf_mes_window.h"

#include "dds/evt_world.h"

#include "dds/fr_font.h"

#include "dds/fr_font_context.h"

#include "dds/mc_poll.h"

#include "dds/mc_read_api.h"

#include "dds/mc_write_api.h"

#include "dds/mc_path_api.h"

#include "dds/bill_object_api.h"

#include "dds/eff_resource_slots.h"

#include "dds/eff_resource_source_lifetime.h"

#include "dds/eff_update_flags.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_chip.h"

#include "dds/eff_curve.h"

#include "dds/file.h"

#include "dds/file_selection_api.h"

#include "dds/file_prompt_reveal_result.h"

#include "dds/file_request_entry.h"

#include "dds/file_slot_flags.h"

#include "dds/file_slot_flags_change_api.h"

#include "dds/file_scroll_arrow_state.h"

#include "dds/sdf_dev_state.h"

#include "dds/file_slot.h"

#include "dds/file_save_record.h"

#include "dds/dat_state.h"

#include "dds/dat_new_cycle_state.h"

#include "dds/mnu_list.h"

#include "dds/pcp_vu0.h"

#include "dds/sdf_vu_transform_api.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf_vu_matrix_multiply_api.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/eff_expanded_list.h"

#include "dds/file_request_api.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf.h"

#include "dds/mdl_flag_api.h"

#include "dds/btl.h"

#include "dds/fr_font_draw_glyph.h"

#include "dds/itf.h"

typedef s32 (*FileMenuContinuationCallback)(void);

/* Declared before first use (were implicit int). */
extern void effLoadFlashTextures(void);

extern void effLoadScalyTexture(void);

extern void effLoadWindTexture(void);

extern s32 slSaveWriteFile(const char *fileName, const u32 *dataAddressSlot, const u32 *dataSizeSlot, FileMenuContinuationCallback continuation, struct FileRequest * const *pendingRequestStatus);

extern void dds3SoftResetOn(void);

extern s32 slSaveData_chdir(void);

extern void frEnableMode(s32 mask);

extern u8 frDisableMode(u8 mask);

extern void frSetFontDrawOT(u32 renderFlags);

extern void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 drawArg);

struct EffectSlotSet;

extern void effDrawTextureSlotWithBlendedCornerColors(s32, s32, s32, u32, u32, struct EffectSlotSet *, s32, s32);

enum { FILE_CONFIG_SET, FILE_CONFIG_FRAME, FILE_CONFIG_X, FILE_CONFIG_Y };

extern s16 fileConfigSpritePlacements[27][4];

extern s32 D_00437D4C;

extern FilePromptRevealResult fileDrawFileMenuPromptAndCheckDelay(void);

extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, s32, s32, s32, s32);

typedef struct MenuWork {
    FileSavePreviewRecord header;
    s8 applyLoadResult;
    s8 hardInitSelected;
    u16 flowConditionMask;
    u16 startBranchFlag; /* 0x34: selects one of two initial menu flows */
    u16 flowRowCursor;
    u16 unk38;
    u16 unk3A;
    struct sdfMemHandle_t *unk3C;
} MenuWork;

extern FileScrollArrowState fileScrollArrowState;

extern f32 D_00437D24;

extern u8 fileNewCycleLabel[];

extern u8 D_003A41A8[][32];

extern u8 D_003A47E8[][32];

extern char D_00437D98[];

extern char D_00437DA0[];

extern char D_00437DA8[];

extern u32 fileClampPackedColorByte(u32, s32, s8);

extern void fileDrawSlotIcon(s32, s32, s32, s32);

extern void fileCursorStepUp(void);

extern void fileFadeStepDown(void);

extern void fileLoadCtxSlideUpdate(void);

extern u8 fileOriginalTitleClearedGameLabel[];

extern u8 fileOriginalTitleNewCycleClearedGameLabel[];

extern u8 fileOriginalTitleUnusableSaveText[];

extern void *fileSaveReadBuffer;

typedef struct MdlFlagPair {
    s32 flag;
    s32 unk4;
} MdlFlagPair;

/* Eight-byte story-flag rows select a unit and a six-byte bonus record. */
typedef struct FileStatBonus {
    s32 flag;
    u16 unitId;
    u8 bonusIndex;
    u8 pad07;
} FileStatBonus;

extern MdlFlagPair fileRareItemNoticeFlagRows[];

extern FileStatBonus ptyUnlockedUnitStatBonusRows[24];

extern s32 mnuAdvanceTitleStateUnderSemaphore(void);

#include "dds/kwln.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_fade.h"

#include "dds/fpu.h"

struct MenuListNode;

extern void mnuClearListFlagsOneAndTwo(u32 *flags);

extern void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration);

struct sdfMemHandle_t;

extern MenuWork *fileLoadSelectionWork;

extern void fileApplyMenuFlagsToModel(MenuWork *);

extern void fldPrepareDeferredSceneTransition(void);

extern void fileCopySaveHeaderNumbers(FileSavePreviewRecord *);

extern void filePrintAccessoryLoadNotice(void);

extern void fileSaveAndDisplayCurrentMoney(void);

extern s32 ptyApplyUnlockedUnitStatBonusesOnce(DatUnitId unitId);

extern u32 datComputeSkillBoostedMaxHp(dds_datUnitWork_t *);

extern u32 datComputeSkillBoostedMaxMp(dds_datUnitWork_t *);

extern void mtrMantraEventBitReset(void);

extern void ptyResetPartyRecordsAndProfiles(void);

extern u32 dds3DrawFlag;

extern u32 fileWaitTicksRemaining;

extern u32 D_00437CEC;

extern s32 fileSlotSelectionPollCount;

extern u32 D_00437CFC;

extern u32 D_00437D08;

extern u32 D_00437D14;

extern u32 D_00437D18;

extern u32 D_00437D1C;

extern u32 D_00437D20;

extern u32 D_00437D40;

extern u32 D_00437D44;

extern u32 D_00437D3C;

extern u32 itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, s32);

extern struct _FRQ *fileMenuConvertedTextGlyph;

extern struct _FRQ *fileMenuConfiguredTextGlyph;

extern struct _FRQ *fileMenuStringGlyph;

extern dds3ProcessID_t *D_00437CD8;

extern s32 D_00437CE4;

extern s32 fileSavePreviewPortraitTextures[];

extern s32 D_00437D74;

extern s32 D_00437D6C;

extern s32 D_00437D68;

extern s32 D_00437D64;

extern s32 D_00437D60;

extern s32 D_00437D5C;

extern s32 D_00437D38;

extern s32 D_00437D48;

extern u32 fileMemoryCardRequestIndex;

extern u32 D_00437CF4;

extern u32 D_00437CF8;

extern FileSelectionFlowState fileSelectionPending;

extern u32 D_00437D04;

extern u32 fileSavedSlotFlags;

extern u32 effModelUpdateControlFlags;

extern struct EffExpandedList *effCreateTextureAnimationSet(u32);

extern s8 slLoadKillFlag;

extern s32 mcdOriginalTitleFileMode;

extern s32 D_00437D88;

extern s32 fileSlotSelectPoll(void);

extern s32 fileBeginSlotMetadataRefresh(void);

extern s32 slFormatStart(void);

extern s32 mcResetSlotMetadata(void);

extern s32 filePollSlotDetectionAndBranch(void);

extern s32 slSizeChk(void);

extern FileMenuContinuationCallback fileMenuContinuation;

extern s32 mcdFinishFileDetection(void);

extern s32 fileBeginDetectionRequest(FileMenuContinuationCallback callback);

extern s32 filePollSlotRequestAndResumeFlow(void);

extern s32 fileSlotScanIndex;

extern void mcdFormatSaveSlotName(char *buffer, s32 slot);

extern s32 slChooseData_getCardInfo2(void);

extern s32 slChooseData_getCardInfo2_B(void);

extern s32 filePrepareMainBlobWrite(void);

extern s32 fileOnAllWritten(void);

extern void dds3SoftResetOff(void);

extern s8 dds3PadButtonState[];

extern void p2sdStopBGM(u32 command);

extern s32 slExitWait(FileMenuContinuationCallback callback);

extern s32 mcHandleDetectionResult(void);

/* Loader context at D_0037D4A0. */
extern FileLoadMenuState fileLoadMenuState;

/* Far scalar: incomplete array forces non-small-data addressing. */
extern s32 D_003E8008[];

extern sdfGrOt_t *effPacketSubmissionSurfaces[6];

extern void sdfVuMatrixToQuaternion(const f32 matrix[4][4]);

/* Effect parameter-set dispatch tables. Every effect kind owns one 0x28-byte
 * entry per table; the handler lives at +0x0. Slots are declared as separate
 * arrays (D_00353710/14/18/1C/20/24/28/2C/30/34 and D_00353880/84/88/90/94/
 * 98/9C/A0/A4). The family2 create table (D_00353880) additionally carries a
 * fallback selector at +0x0C: nonzero calls the entry handler directly,
 * zero falls back through D_003536A0.
 */
/* 8-byte parameter work (family1): kind id plus one data pointer. */
typedef struct EffDispatchExtra {
    void (*func)(void *, f32 vector[4]); /* 0x00 vector handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchExtra;

typedef struct EffDispatchScale {
    void (*func)(void *, f32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchScale;

typedef struct EffDispatchColor {
    void (*func)(void *, u32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchColor;

extern s8 dds3ConfigState;

extern s32 mcdFinishFileDetectionWithAudio(void);

extern s32 fileResetSlotSelection(void);

extern s32 D_00437D50;

extern s8 fileSavePreviewVersionByteView[];

extern FileMenuStateHandler fileMenuStateHandler;

extern void eglSetOt(u32);

extern void evtSubmitPrimaryAlphaBlendMode(s32);

extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 D_00437D70;

extern void fileCursorPulseUpdate(void);

extern void fileDrawCursorPulse(s32, s32, s32);

extern s32 D_00437D78;

extern s32 fileReadSlotPreviewBegin(void);

extern s32 slLoadProc_readFile(void);

extern McFileDescriptor fileSaveFileDescriptor;

extern struct sdfMemHandle_t *fileSaveReadBufferResource;

extern s32 slHeadLoadProc_readFile(void);

extern s32 fileBeginReadRemainingSlotIcons(void);

extern s32 slHeadLoad(void);

extern s32 fileSlotSelectPollClear(void);

extern s32 slBootRetry(void);

extern s32 fileSlotStatusPoll(void);

extern s32 fileResetSelection(void);

extern FileMenuContinuationCallback fileWaitContinuation;

extern s32 slExitWaitProc(void);

extern s32 slChooseData_getCardInfo3(void);

extern s32 slBootError(void);

extern s32 fileScanSlotStatesAdvance(void);

extern s32 slChooseData_getCardInfo3_B(void);

extern s32 slSaveData(void);

extern s32 slSaveProc_Write_mkdir(void);

extern s32 fileScanSlotStates(void);

extern s32 slLoadProc_openFile(void);

extern s32 fileDrawMenuFrame(s32);

extern u32 fileSaveIconData;

extern u32 fileSaveIconDataSize;

extern char fileConfigInputTaskName[];

extern char fileConfigLoadTaskName[];

extern char fileConfigOwnerTaskName[];

typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;

extern LoadMirror fileSlotFlagMirror;

extern void fileCacheSlotFlagsFromState(void);

extern void fileLoadObjectSetResource(EffectSurfaceNode *node, u32 entryId, void *resource);

extern void fileLoadObjectOpenDevice(EffectSurfaceNode *node, u32 resourceId);

extern void fileLoadObjectOpenAndStartDevice(EffectSurfaceNode *node, u32 resourceId);

extern void fileLoadObjectOpenNamedDevice(EffectSurfaceNode *node, u32 resourceId);

extern void fileReplaceEffectSurfaceJobs(EffectSurfaceNode *node, FileJobPayload *job);

extern void fileReplaceEffectSurfaceQueues(EffectSurfaceNode *node, FileQueue *queue);

extern void fileReplaceReferenceHolder(EffectSurfaceNode *node, u32 resource);

extern void fileJobFreePrimaryBuffer(FileJobPayload *job);

extern void fileJobFreeSecondaryBuffer(FileJobPayload *job);

extern void fileQueueAppend(FileQueue *queue, FileJob *job);

extern void fileQueueInitTransform(void *queue);

extern FileJob *fileJobCreate(void);

extern s32 slEndingExit(void);

extern s32 slEndingTop(void);

extern FileMenuStartMode D_00437D30;

extern s32 slEndingUpd2(void);

extern char D_0042B698[];

extern s32 slErrorWait(void);

extern char D_0042B6B8[];

extern sceMcTblGetDir fileDirectoryEntryScratch[];

extern s32 mcClearSlotMetadata(void);

extern s32 slSaveData_deleteIconSys(void);

extern s32 fileSelectedSlotIndex;

extern s32 fileBuildMainBlobAfterDelete(void);

extern s32 fileBuildMainBlobAndWrite(void);

extern s32 slFormatCancel(void);

extern u32 D_00437D0C;

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_0037F660[];

extern FileJob *fileQueueFindById(FileQueue *queue, u32 id);

extern s32 fileFindQueuedJobIndex(FileQueue *queue, FileJob *target);

extern f32 fileSaveHighlightPhase;

extern u8 sdfHddValid;

extern char D_r1_0046D0E0[];

extern char D_r1_0046D0E8[];

extern char D_r1_0046D0F0[];

extern char *sdfFileGetHostRoot(void);

void slKillLoadProcess(void);

s8 fileMenuTaskIsAlive(void);

void mnuFormatSaveSlotHeaderName(void);

void mcdFormatSaveSlotName(char *buffer, s32 slot);

void fileReqGetSlotCode(void);

#define FILE_MAIN_BLOB_SIZE 0x1E840

u32 fileMainBlobSize(void);

void fileReloadSaveBuffer(void);

u8 fileIsLoadedAndConditionTrue(s32 condition);

u8 fileIsLoadedWithActiveFlow(s32 condition);

void mnuDrawAndStoreTextGlyphHandle(s32 x, s32 y, u32 colors, const u8 *text);

void mcdCreateConfiguredDrawHandle(s32 x, s32 y, u32 colors, const u8 *text);

void mcdCreateFontDrawHandle(s32 x, s32 y, u32 colors, const char *text);

void fileDrawMenuImageAtPoint(s32 x, s32 y, u32 colors, char *text);

/* Animate the save-window highlight's alpha with a sinusoidal phase. */
void fileDrawPulsingSaveHighlight(void);

void fileDrawSaveWindow(void);

s32 slChkWindowOpenDone(void);

/* Draw the expanding prompt panel and report when its input delay has elapsed. */
FilePromptRevealResult fileDrawFileMenuPromptAndCheckDelay(void);

extern s32 D_00437D28;

extern void fileResetLoadContextSlide(void);

extern char fileNoCardStatus1MessageLine1[];

extern char fileNoCardStatus1MessageLine2[];

extern char fileNoCardStatus1MessageLine3[];

extern char fileNoCardStatus1MessageLine4[];

extern char fileNoCardStatus1MessageLine5[];

extern char fileNoCardStatus2MessageLine1[];

extern char fileNoCardStatus2MessageLine2[];

extern char fileNoCardStatus2MessageLine3[];

extern char fileNoCardStatus2MessageLine4[];

extern char fileNoCardStatus2MessageLine5[];

extern char fileLoadFailedText[];

extern char fileSaveFailedText[];

extern char fileFormatFailedText[];

extern char fileInsufficientSpaceMessageLine1[];

extern char fileInsufficientSpaceMessageLine2[];

extern char fileInsufficientSpaceMessageLine3[];

extern char fileInsufficientSpaceMessageLine4[];

extern char fileInsufficientSpaceMessageLine5[];

extern char fileNoSaveDataMessageLine1[];

extern char fileNoSaveDataMessageLine2[];

extern char fileNoSaveDataMessageLine3[];

extern char D_003E8990[];

extern char D_003E89B8[];

extern char fileOriginalTitleSaveMissingText[];

extern char fileNotClearedGameSaveText[];

extern char fileSelectClearedGameSaveMessageLine1[];

extern char fileSelectClearedGameSaveMessageLine2[];

extern char fileCorruptSaveDataMessageLine1[];

extern char fileCorruptSaveDataMessageLine2[];

extern char fileCorruptSaveDataMessageLine3[];

void fileDrawMemoryCardFlowPrompt(void);

void slSetInfo(u32 value);

extern char fileCheckingCardMessageLine1[];

extern char fileCheckingCardMessageLine2[];

extern char fileCheckingCardMessageLine3[];

extern char fileLoadingText[];

extern char fileSavingText[];

extern char fileFormattingText[];

extern char fileCardBusyCardWarningText[];

extern char fileCardBusyConsoleWarningText[];

extern char fileLoadSuccessfulText[];

extern char fileSaveSuccessfulText[];

extern char fileFormatSuccessfulText[];

extern char fileOriginalTitleSaveBenefitsMessageLine1[];

extern char fileOriginalTitleSaveBenefitsMessageLine2[];

extern char fileOriginalTitleSaveBenefitsMessageLine3[];

extern char fileOriginalTitleSaveBenefitsMessageLine4[];

extern char fileRareItemAvailabilityMessageLine1[];

extern char fileRareItemAvailabilityMessageLine2[];

extern char fileCharacterStatBonusMessageLine1[];

extern char fileCharacterStatBonusMessageLine2[];

extern char fileDifficultySelectionAvailableText[];

extern char fileDataTransferCompleteText[];

void fileDrawModeSpecificMemoryCardPrompt(void);

void fileClearAllSlotFlags(void);

s32 slHeadLoad(void);

s32 fileReadSlotPreviewBegin(void);

extern void sdfMcFuncClose(McFileDescriptor fileDescriptor);

extern s32 fileStoreSlotHeader(void);

s32 slHeadLoadProc_readFile(void);

/* Ten contiguous 0x30-byte save-preview records (0x1E0 bytes total). */
extern FileSavePreviewRecord fileSavePreviewRecords[10];

s32 fileStoreSlotHeader(void);

s32 slExitWait(FileMenuContinuationCallback callback);

s32 slEndingTop(void);

s32 slEndingUpd2(void);

extern s32 fileRequestDefaultMode(void);

s32 slEndingExit(void);

s32 slBootTop(void);

s32 slBootRetry(void);

s32 slBootError(void);

extern s32 fileRequestDefaultMode(void);

s32 slBootError2(void);

s32 fileBeginSlotMetadataRefresh(void);

s32 fileResetSelection(void);

extern void dds3AdminiCallSequence(s32, const void *, u32, s32);

extern s32 fileMenuWorkStart(void);

s32 slExit(void);

s32 mcdEnterDefaultFileFlow(void);

s32 fileRequestDefaultMode(void);

s32 mcdEnterSelectedFileFlow(void);

s32 fileRestartSlotScan(void);

extern s32 fileSlotBrowserUpdate(void);

s32 fileScanSlotStates(void);

s32 slCancel(void);

extern struct sdfMemHandle_t *fileSaveIconResourceHandle;

extern struct FileRequest *fileSaveIconRequest;

extern char D_0042B6A8[];

s32 fileLoadIconFileAndResetSelection(void);

s32 fileResetSlotSelection(void);

s32 fileLoadIconFileAndBeginSlotReset(void);

s32 slBootProc1st(void);

s32 fileSlotStatusPoll(void);

s32 fileCountSelectableFiles(void);

s32 slSizeChk(void);

extern s32 slBootError(void);

extern s32 fileRequestDefaultMode(void);

extern s32 slSizeChk(void);

extern s32 slBootRetry(void);

s32 fileSlotSelectPoll(void);

extern s32 slBootError(void);

extern s32 fileRequestDefaultMode(void);

extern s32 slSizeChk(void);

s32 fileSlotSelectPollClear(void);

s32 filePollSlotDetectionAndBranch(void);

s32 slErrorWaitToFileMc(void);

s32 slErrorWait(void);

s32 slExitWaitProc(void);

s32 fileSetMenuCallbackAndClearResult(FileMenuContinuationCallback menuCallback);

s32 mcdFinishFileDetection(void);

s32 fileBeginDetectionRequest(FileMenuContinuationCallback callback);

s32 filePollSlotRequestAndResumeFlow(void);

s32 fileBeginReadSlotIcon(void);

s32 slChooseData_getCardInfo2(void);

s32 slChooseData_getCardInfo3(void);

s32 fileBeginReadRemainingSlotIcons(void);

s32 mcResetSlotMetadata(void);

s32 fileBeginSaveSlotIconScan(void);

s32 slChooseData_getCardInfo2_B(void);

s32 slChooseData_getCardInfo3_B(void);

s32 fileScanSlotStatesAdvance(void);

s32 mcClearSlotMetadata(void);

s32 slSaveData(void);

s32 slSaveData_chdir(void);

s32 slSaveProc_Write_mkdir(void);

s32 filePrepareMainBlobWrite(void);

s32 fileBuildMainBlobAfterDelete(void);

s32 fileOnAllWritten(void);

s32 fileWriteIconSysComplete(void);

extern u8 fileSaveIconSysData[];

extern u32 D_00437D90;

extern u32 D_00437D94;

s32 fileBeginLabeledSlotWrite(void);

extern s32 slSaveWriteFile(const char *fileName, const u32 *dataAddressSlot, const u32 *dataSizeSlot, FileMenuContinuationCallback continuation, struct FileRequest * const *pendingRequestStatus);

extern s32 fileBeginLabeledSlotWrite(void);

s32 slSaveWriteEnd_saveData(void);

extern dds_dds3GlobalWork_t *fileMainSaveBlobAddress;

extern u32 fileMainSaveBlobSize;

s32 fileBuildMainBlobAndWrite(void);

s32 slSaveData_deleteIconSys(void);

extern const u32 *fileWriteDataAddressSlot;

extern const u32 *fileWriteDataSizeSlot;

extern FileMenuContinuationCallback fileWriteContinuation;

extern struct FileRequest * const *fileWritePendingRequestSlot;

extern s32 slSaveProc_openFile(void);

/* Later save callbacks read these borrowed slots without changing them; callers
 * keep the slots alive until the asynchronous save completes. */
s32 slSaveWriteFile(const char *fileName, const u32 *dataAddressSlot, const u32 *dataSizeSlot, FileMenuContinuationCallback continuation, struct FileRequest * const *pendingRequestStatus);

extern s32 slSaveProc_writeFile(void);

extern s32 slSaveProc_waitDataReady(void);

s32 slSaveProc_openFile(void);

extern s32 slSaveProc_closeFile(void);

s32 slSaveProc_writeFile(void);

extern FileMenuContinuationCallback fileWriteContinuation;

s32 slSaveProc_closeFile(void);

s32 slSaveProc_waitDataReady(void);

s32 mcHandleDetectionResult(void);

s32 slFormatStart(void);

s32 slFormatCancel(void);

s32 slLoadData(void);

s32 slLoadProc_openFile(void);

extern s32 mcdHandleSaveSetupDone(void);

s32 slLoadProc_readFile(void);

extern s8 D_00437CD5;

void fileRestoreSlotFlagsToState(void);

s32 mcdHandleSaveSetupDone(void);

extern s32 mcdContinueLoadSelection();

s32 mcdAdvanceToLoadSelection(void);

extern s32 D_00437D00;

extern void fileLoadSetMode(FileLoadMenuSlideMode mode);

extern void fileSetMenuValueAndInitializeFlags(u32);

extern void fileCopyRecordHeader(FileSavePreviewRecord *, const void *);

s32 fileSlotBrowserUpdate(void);

/* Draw save slots and the selected preview; native scrolling snaps to its target. */
s32 fileDrawSlotListAndPreview(s32 work);

s32 slCalc(dds3ProcessID_t *task);

s32 fileDrawMenuFrame(s32 task);

void fileResetMenuFlowState(void);

#include "dds/file_pac.h"

extern void *memset(void *dst, s32 value, s32 size);

struct sdfMemHandle_t;

extern void dds3SoftResetOff(void);

extern s32 D_00437D80;

extern void fileDestroyMemoryCardMenuTask(dds3ProcessID_t *task);

extern void dds3FadeIn(s32 red, s32 green, s32 blue, s32 duration);

void fileInitCursorPulse(void);

void fileStartMemoryCardMenuForMode(FileMenuStartMode mode);

void fileReleaseMenuResources(void);

void fileDestroyMemoryCardMenuTask(dds3ProcessID_t *task);

void fileMenuReleaseNoOp(void);

void fileReleaseMenuResources(void);

u32 fileMenuOperationSucceeded(void);

s32 fileMenuTaskExists(void);

FileSelectionFlowState fileGetSelectionPendingFlag(void);

extern FileSelectionCallback D_00439024, D_00439028;

/* Yes/no cursor input while the card request is idle. */
static inline void filePollPromptChoice(void) {
    if (D_00437D20 == 0 && slChkWindowOpenDone() != 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x27] < 0) != 0) {
            if (D_00437D18 != 1) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_00437D18 = 1;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x26] < 0) != 0) {
            if (D_00437D18 != 0) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_00437D18 = 0;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) != 0) {
            if (D_00437D18 == 0 && (D_00437D1C == 1 || D_00437D1C == 3)) {
                p2sdPlaySE(0x11, 0x7F, 0x3F);
            } else {
                p2sdPlaySE(8, 0x7F, 0x3F);
            }
            if (D_00437D18 != 0) {
                D_00437D20 = 2;
            } else {
                D_00437D20 = 1;
            }
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0) != 0) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            D_00437D20 = 2;
        }
    }
}

s32 fileUpdatePromptDialog(void);

extern s32 fileUpdatePromptDialog(void);

s32 slQueryYesNo(FileSelectionCallback start, FileSelectionCallback finish, s32 mode);

FileMenuOperationResult fileGetSaveLoadResult(void);

void slSetTmnlID(s32 x, s32 y);

void fileSetMenuValueAndInitializeFlags(u32 value);

/* Lower the selected low/alpha byte, retaining the native signed, unmasked bound. */
/* Keep the input word separate from the per-mode result; see docs/idioms.md,
 * "Trace-justified result selection: packed byte clamp". */
u32 fileClampPackedColorByte(u32 color, s32 limit, s8 alpha);

void fileCursorStepUp(void);

void fileFadeStepDown(void);

void fileDrawSlotIcon(s32 index, s32 x, s32 y, s32 alpha);

void fileCursorOffsetLookup(s32 dir, s32 step, s32 *outX, s32 *outY);

/* Init record at fileCursorPulseState. */
typedef struct Init374E0 {
    s8 mode[3];      /* 0x00 */
    u8 pad3;         /* 0x03 */
    s32 pos[3][2];   /* 0x04 */
    s32 counter[3];  /* 0x1C */
    u8 pad28[0xC];   /* 0x28 */
    s32 alpha[3];    /* 0x34 */
} Init374E0;

extern Init374E0 fileCursorPulseState;

extern s32 D_003E7FE4[];

void fileInitCursorPulse(void);

void fileCursorPulseUpdate(void);

extern const s32 D_0042B8F8[3][2];

void fileDrawCursorPulse(s32 x, s32 y, s32 surface);

void fileLoadSetMode(FileLoadMenuSlideMode mode);

void fileResetLoadContextSlide(void);

void fileLoadCtxSlideUpdate(void);

s32 mcdFinishFileDetectionWithAudio(void);

s32 fileCreateDetectionAudioCallback(FileMenuContinuationCallback callback);

extern FileSelectionCallback fileFourWayDialogCancelCallback;

s32 fileFourWayDialogPoll(void);

extern s32 fileFourWayDialogPoll(void);

s32 fileBeginFourWayDialog(FileSelectionCallback ready, FileSelectionCallback completed,
                           FileSelectionCallback cancelled, u32 state);

s32 fileWaitAfterLoadResult(void);

#include "dds/sdf_debug_api.h"

s32 fileApplySelectedLoadResult(void);

s32 fileBeginFadeAndConfirmSound(void);

s32 fileSelectLoadWithoutHardInit(void);

s32 fileSelectLoadWithHardInit(void);

s32 fileBeginLoadBranchDialog(void);

s32 fileStartLoadDetectionAfterBranchDialog(void);

s32 fileRestartSelectionFlow(void);

s32 fileBeginLoadConfirmationDialog(void);

extern s32 fileNextMenuFlowState(void);

s32 fileWaitForLoadStepBeforeDetection(void);

typedef struct FileFlowEntry {
    u16 state;
    u16 mask;
} FileFlowEntry;

extern FileFlowEntry fileMenuFlowStateEntries[];

s32 fileNextMenuFlowState(void);

s32 fileResetPendingRequest(void);

s32 fileFadeBeforeResettingRequest(void);

s32 fileBeginLoadOrAbortDialog(void);

extern void dds3FadeIn2(s32);

extern s32 fileBeginLoadOrAbortDialog(void);

s32 fileMenuWorkStart(void);

void fileMenuWorkCreate(u32 startBranchFlag);

void fileReleaseMenuFlowResource(void);

extern char D_0042B920[];

void filePrintAccessoryLoadNotice(void);

extern char D_0042B938[];

void fileSaveAndDisplayCurrentMoney(void);

typedef struct FileMultiFlagBonus {
    s32 flags[4];
    u8 bonusIndex;
    u8 pad11[3];
} FileMultiFlagBonus;

extern FileMultiFlagBonus ptyUnit7StatBonusFlagGroups[6];

extern char D_0042B950[];

/* Apply each unlocked story bonus once to the active party record. */
s32 ptyApplyUnlockedUnitStatBonusesOnce(DatUnitId unitId);

void fileCopySaveHeaderNumbers(FileSavePreviewRecord *source);

void fileCopyRecordHeader(FileSavePreviewRecord *destination, const void *source);

void fileApplyMenuFlagsToModel(MenuWork *work);

FileSlotFlagsChangeResult fileLoadStateChanged(void);

void fileCacheSlotFlagsFromState(void);

void fileRestoreSlotFlagsToState(void);

extern void dds3PadVibrationSet(s32, u8, s32);

extern s32 fileTestSlotFlagsBit(FileConfigOption option, u32 *flags);

u8 fileToggleSlotFlagsBit(FileConfigOption option, u32 *flags);

s32 fileTestSlotFlagsBit(FileConfigOption option, u32 *flags);

s32 fileTestSavedSlotFlags(FileConfigOption option);

extern FileConfigTask *GBWK;

extern struct MenuList *mnuCreateListState(s32, s32, s32);

extern void fileDrawConfigurationMenuEntry(s32 x, s32 y, s32 depth, struct MenuList *list, FileConfigListNode *node, s32 drawArg);

extern void mnuResetTitleStreamLocked(void);

extern void mnuResetTitleStreamAfterFileIdle(void);

extern void mnuQueueTitleSoundStreamEntry(s32);

extern void effRequestResourceByMode(const char *, const char *, s32, u32 *);

extern char D_00437DF0[];

extern char *fileConfigSpriteResourceNames[];

/* Allocate the save/config task, its five-row list and its effect resource slots. */
FileConfigTask *fileCreateConfigTask(s32 mode);

void fileConfigTaskDestroy(dds3ProcessID_t *task);

extern FileConfigTask *fileCreateConfigTask(s32 mode);

extern s32 fileStartQueuedLoad(dds3ProcessID_t *task);

extern void fileConfigTaskDestroy(dds3ProcessID_t *task);

extern s32 fileUpdateConfigurationMenu(dds3ProcessID_t *task);

extern s32 fileUpdateConfigOwnerTask(dds3ProcessID_t *task);

extern s8 dds3ConfigState;

void dds3ConfigProcessStart(s32 mode);

void dds3ConfigProcessEnd(void);

s32 checkConfigState(void);

u32 fileGetConfigTaskSlot(s32 slot);

s32 fileUpdateConfigurationMenu(dds3ProcessID_t *task);

extern void fileDrawConfigurationMenuBackground(void *);

s32 fileStartQueuedLoad(dds3ProcessID_t *task);

s32 fileUpdateConfigOwnerTask(dds3ProcessID_t *task);

void fileDrawConfigurationMenuEntry(s32 x, s32 y, s32 depth, struct MenuList *list,
                   FileConfigListNode *node, s32 drawArg);

void fileDrawConfigurationMenuBackground(void *unusedTask);

void fileResetRenderFlags(void);

void effLoadCommonTexturesAndResetRenderFlags(void);

void fileSetRenderFlag(u32 mask);

u32 fileClearRenderFlag(u32 mask);

void fileResetRenderFlags(void);

sdfGrOt_t *effGetPacketSubmissionSurface(s32 index);

void mnuProjectViewPoint(void);

extern u8 aViewEye[];

/* vu0 routine: tests whether vf10 projects within the camera's forward cone. */
s32 sdfProjectPointInForwardConeVU(void);

extern u8 aViewDown[];

/* vu0 routine: distance between the projected view point and a second point offset perpendicular to
   the camera axis by rate; 0 when either projection (sdfProjectPointInForwardConeVU) fails. Point comes in vf10. */
f32 mnuMeasureProjectedPerpendicularDistance(f32 rate);

/* vu0 routine: point at t between p[1] and p[2] of a Catmull-Rom (Hermite, 0.5 tangents) spline, left in vf10 */
void vuCatmullRomPoint(f32 (*p)[4], f32 t);

extern u8 aViewEye[];

extern u8 aViewTarget[];

extern u8 aViewDown[];

/* vu0 routine: camera basis rows vf28-vf31 from eye aViewEye, target aViewTarget and up aViewDown */
void vuBuildLookAtBasis(void);

/* The quaternion result is returned in vf10 for the VU transform routines. */
void sdfVuMatrixToQuaternion(const f32 matrix[4][4]);

FileJobPayload *fileCreateJob(FileJobType type);

#define FILE_JOB_LOCATION_AS_JOB_RELATIVE_OFFSET(payload) ((payload).offset)

#define FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(payload) ((payload).offset)

#define FILE_JOB_LOCATION_CLEAR(payload) ((payload).offset = 0)

void *fileResolvePrimaryBuffer(FileJobPayload *job);

void *fileResolveSecondaryBuffer(FileJobPayload *job);

FileJobPayload *fileJobCreateFromJob(FileJobPayload *request);

void fileJobDestroy(FileJobPayload *job);

void fileJobFreePrimaryBuffer(FileJobPayload *job);

void fileJobFreeSecondaryBuffer(FileJobPayload *job);

FileJobPayload *fileJobCreateChild(FileJobPayload *request);

void fileJobNotifyPair(FileJobPayload *left, FileJobPayload *right);

void fileJobNotifyComplete(FileJobPayload *job);

void fileJobInvokeTypeCallback(FileJobPayload *job);

void fileJobInvokePositionCallback(FileJobPayload *job, const f32 position[4]);

void fileJobInvokeRotationCallback(FileJobPayload *job, const f32 rotation[4]);

void fileJobInvokeScaleCallback(FileJobPayload *job, f32 scale);

void fileDispatchJobTypeCallback(FileJobPayload *job, u32 color);

void fileJobSetPrimaryData(FileJobPayload *job, const void *src, s32 size, u16 option);

void fileJobCopyCommandIntoPrimaryData(FileJobPayload *job, const char *commandPath, u16 option);

void fileJobSetSecondaryData(FileJobPayload *job, const void *src, s32 size, u16 selector);

void fileJobCopyCommandIntoSecondaryData(FileJobPayload *job, const char *commandPath, u16 selector);

void fileWriteToPfs(FileJobPayload *job, const char *filePath);

FileJobPayload *fileDuplicateJob(FileJobPayload *request);

/* NULL when the command file does not open: retail's v0 is sdfFileOpen's
 * null result there. */
FileJobPayload *fileJobCreateFromCommandState(const char *entry);

u32 fileJobSerializedSize(FileJobPayload *job);

/* Initialize the shared transform prefix, leaving queue/job links untouched. */
void fileQueueInitTransform(void *queue);

void fileJobResetAndInitTransform(FileJob *job);

void fileQueueAppend(FileQueue *queue, FileJob *job);

void fileQueueInsertAfter(FileQueue *queue, FileJob *after, FileJob *job);

void fileQueueRemove(FileQueue *queue, FileJob *job);

FileQueue *fileQueueCreate(void);

FileJob *fileJobCreate(void);

void fileDestroyJob(FileJob *job);

FileQueue *fileCloneQueueEntries(FileQueue *source);

/* Effect-node creation callback: ignore its option and clone the supplied queue. */
FileQueue *CallShopTest(u32 unused, FileQueue *source);

void camFollowOffsetVec(FileJob *job, void *dst);

struct CamAim;

extern void camAimRotation(struct CamAim *obj, void *dst);

/* Per-frame update: refreshes the queue rotation when an aim flag (0x60) is set, then repositions and re-notifies every job whose start frame (job+0x80) has been reached; the frame counter only advances when the effect-frame pause flag is clear. */
void fileQueueUpdate(FileQueue *queue);

/* A queued job's +0x90 word is a child pointer when flag 1 is clear; it is
 * an ID for other job kinds. Destroy children before their parent jobs. */
void fileQueueDestroy(FileQueue *queue);

FileQueue *fileQueueClone(const FileQueue *source);

void fileQueueNotifyAllJobsComplete(FileQueue *queue);

void fileQueueSetPosition(FileQueue *queue, const f32 vec[4]);

void fileQueueSetRotation(FileQueue *queue, const f32 rot[4]);

void fileQueueSetScale(FileQueue *queue, f32 scale);

/* Store the queue's modulation colour, then send every job its own colour
 * modulated by it ((job / 128) * (queue / 128) * 128 per channel). The tint
 * store is the unclobbered SDK spelling: as a bare volatile asm it flushes the
 * CSE table, so the loop's 1/128 is rematerialized rather than copied from the
 * first unpack's FPR. Reading the payload before the packed store keeps the
 * store from feeding the argument load. */
void fileQueueSetColor(FileQueue *queue, u32 color);

void fileQueueReadPosition(const FileQueue *queue, f32 position[4]);

void fileReadStoredQuaternion(const FileQueue *queue, f32 quaternion[4]);

f32 fileGetQueueScale(FileQueue *queue);

u32 fileGetQueueColor(FileQueue *queue);

void fileQueueCopyRotationFromSource(FileQueue *queue, f32 matrix[4][4]);

extern void fileQueueAppend(FileQueue *queue, FileJob *job);

FileJob *fileAppendJob(FileQueue *queue, u32 id);

FileJob *fileDuplicateAndAppendJob(FileQueue *queue, FileJobPayload *source);

extern u8 D_r1_0046D0F8[];

FileJob *fileAppendJobFromCommandPath(FileQueue *queue, const char *commandPath);

FileJob *fileJobDuplicateAfter(FileQueue *queue, FileJob *src);

extern FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector);

extern FileJob *fileQueueFindFlaggedById(FileQueue *queue, u32 id);

/* Makes the first job chained to owner's sector the leader and rechains the rest to it. */
static inline void fileQueueRechainSectorFollowers(FileQueue *queue, FileJob *owner, FileJob *leader) {
    FileJob *next;

    leader->sector = 0;
    leader->flags &= ~FILE_JOB_FLAG_SECTOR_FOLLOWER;
    fileQueueLinkJobToSectorLeader(queue, leader, leader);
    next = fileQueueFindBySector(queue, owner->id);
    while (next != NULL) {
        fileQueueLinkJobToSectorLeader(queue, next, leader);
        next = fileQueueFindBySector(queue, owner->id);
    }
}

void fileQueueLinkJobToSectorLeader(FileQueue *queue, FileJob *job, FileJob *ref);

void fileQueueDetachSectorFollower(FileQueue *queue, FileJob *job);

void fileQueueRemoveAndDestroyJob(FileQueue *queue, FileJob *job);

void fileJobCopyHeader(FileJob *dst, const FileJob *src);

extern char fileQueueImagePaddingBytes[];

extern s32 fileQueueCountLinkedJobs(FileQueue *queue);

/* Export a queue: the queue header, one relocated record per job, then each
 * owning job's 16-byte-aligned payload. Child jobs store their parent's index. */
void fileQueueSaveImage(FileQueue *queue, const char *filePath);

/* Sixteen-byte preamble of a saved file queue. */
typedef struct FileQueueImageHeader {
    s32 version;
    s32 unk4;
    s32 unk8;
    f32 unkC;
} FileQueueImageHeader;

extern char fileQueueVersionedImagePaddingBytes[];

/* Save a queue image: the queue header, one relocated record per job, then each
 * owning job's 16-byte-aligned payload. Child jobs store their parent's index. */
void fileQueueSaveVersionedImage(FileQueue *queue, const char *filePath);

FileQueue *fileQueueCreateFromCommandState(const char *entry);

FileJob *fileQueueFindById(FileQueue *queue, u32 id);

FileJob *fileQueueFindFlaggedById(FileQueue *queue, u32 id);

FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector);

FileJob *fileQueueGetAt(FileQueue *queue, s32 index);

s32 fileFindQueuedJobIndex(FileQueue *queue, FileJob *target);

s32 fileQueueCountLinkedJobs(FileQueue *queue);

/* vu0 routine: dst = |pos| * unit(aViewEye - aViewTarget); flag 0x10 flattens y, then puts pos.y back */
void camFollowOffsetVec(FileJob *obj, void *dst);

/* dst = rotation facing unit(aViewEye - aViewTarget) (pitch and yaw); flag 0x40 composes it onto obj->quat, no flag 0x60 copies obj->quat */
void camAimRotation(CamAim *obj, void *dst);

EffectSurfaceNode *fileCreateSurfaceLoaderState(s32 capacity);

EffectSurfaceNode *fileCreateGridLoaderRecord(FileKeyBlock *hdr);

EffectSurfaceNode *fileCreateEffectSurfaceFromJob(FileJobPayload *job);

void fileDestroyEffectSurfaceAndChildren(EffectSurfaceNode *node);

void fileCloneEffectSurfaceResources(EffectSurfaceNode *dst, EffectSurfaceNode *src);

EffectSurfaceNode *fileLoadObjectCreateChild(EffectSurfaceNode *owner);

void fileCloneEffectSurfaceResources(EffectSurfaceNode *dst, EffectSurfaceNode *src);

void fileLoadObjectSetResource(EffectSurfaceNode *node, u32 entryId, void *resource);

void fileLoadObjectOpenNamedDevice(EffectSurfaceNode *node, u32 resourceId);

void fileLoadObjectOpenDevice(EffectSurfaceNode *node, u32 resourceId);

void fileLoadObjectOpenAndStartDevice(EffectSurfaceNode *node, u32 resourceId);

void fileReplaceEffectSurfaceJobs(EffectSurfaceNode *node, FileJobPayload *job);

void fileReplaceEffectSurfaceQueues(EffectSurfaceNode *node, FileQueue *queue);

void fileReplaceReferenceHolder(EffectSurfaceNode *obj, u32 resource);

void fileClearLoadObjectReferences(EffectSurfaceNode *obj);

extern void mnuRecordSetVector(void *record, const void *src);

extern void fileSetRecordSecondVector(void *record, const void *src);

/* Original-style implicit int; callers ignore its result. */
fileAcquireLoadObjectRecord(EffectSurfaceNode *obj);

#include "dds/eff_anim.h"

extern void dds3BillboardDraw(struct billHandle_t *);

extern void fileReadRecordSecondVector(u8 *, void *);

extern void fileReadVectorPtr20(u8 *, void *);

extern void effSubmitGeneratedTexturePacket(sdfGrOt_t *, EffGeneratedTextureDescriptor *);

void fileDrawLoadObjectSlots(s32 object);

void fileUpdateLoadObjectSlots(u32 objectAddr);

void fileSendLoadObjectRecordVector(EffectSurfaceNode *obj, const u128 *value);

void fileCopyLoadObjectRecordVector(EffectSurfaceNode *obj, const u128 *value);

void fileSetRecordWordFour(s32 record, u32 value);

void fileSetLoadObjectScale(EffectSurfaceNode *obj, f32 scale);

void fileResetSlotStates(FileSlotTable *table);

/* vu0 routine: preserve the packed RGB snapshot while applying the alpha track. */
u32 effSampleColorAlphaTracks(const SdfColorTrack *colorTrack,
                              const SdfAlphaTrack *alphaTrack,
                              s32 frame, s32 duration);

f32 effSampleScalarCurve(const EffScalarCurve *curve, s32 frame, s32 duration);

/* vu0 routine: samples the colour, scale and heading tracks at frame; in mode 2 the heading is the screen-space direction from out->pos to target (0 when they coincide) */
void fileSampleKeyTracks(FileSlot *out, const FileKeyBlock *block, s32 frame, const f32 *target);

void fileInvalidateSlotGroup(FileSlotTable *table, FileSlot *slot);

void fileCopyAndInvalidateSlotGroup(FileSlotTable *table, FileSlot *source);

/* vu0 routine: shifts one slot's history and interpolates its trailing samples. */
void effUpdateGridRecordSlotHistory(FileSlotTable *record, FileSlot *slot);

extern struct EffRandState effSharedRandomState;

extern f32 fabsf(f32);

extern void effUpdateGridRecordSlotHistory(FileSlotTable *, FileSlot *);

typedef struct FileWaveMotion {
    f32 axis[4];             /* 0x00: xyz only */
    f32 swayDirection[4];    /* 0x10: xyz only */
    f32 phase;
    f32 previousSine;
    f32 amplitude;
    f32 amplitudeStep;
    f32 phaseStep;
    f32 speed;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} FileWaveMotion;            /* 0x44, ordinary word alignment */

extern f32 effWaveEmitterNegativeYAxis[4];

/* vu0 routine: advances axial motion with a quaternion-oriented sine sway. */
void effUpdateWaveEmitterSlots(FileSlotTable *record);

/* Scale selected wave-emitter parameters from source to destination. */
void effScaleWaveEmitterParameters(ScaleOwner *owner, f32 scale);

typedef struct FileSlotMotion {
    f32 direction[4];     /* 0x00; this callback initializes xyz only */
    f32 speed;            /* 0x10 */
    f32 scaleMultiplier;  /* 0x14 */
    f32 angle;            /* 0x18 */
    f32 angleMultiplier;  /* 0x1C */
} FileSlotMotion;

/* vu0 routine: emits and advances randomized radial slots, then samples their key tracks. */
void effUpdateRadialEmitterSlots(FileSlotTable *record);

/* Scale selected radial-emitter parameters from source to destination. */
void effScaleRadialEmitterParameters(ScaleOwner *owner, f32 scale);

typedef struct FileCircularMotion {
    f32 origin[4];          /* 0x00: xyz only */
    f32 orientation[4];     /* 0x10: saved emitter quaternion */
    f32 height;             /* 0x20 */
    f32 radius;
    f32 radiusStep;
    f32 initialPhase;
    f32 angularSpeed;
    f32 axialSpeed;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} FileCircularMotion;       /* 0x44, ordinary word alignment */

/* vu0 routine: advances changing-radius circular motion in the saved emitter frame. */
void effUpdateCircularEmitterSlots(FileSlotTable *record);

/* Scale selected circular-emitter parameters from source to destination. */
void effScaleCircularEmitterParameters(ScaleOwner *owner, f32 scale);

typedef struct FileOrientedRingMotion {
    f32 origin[4];          /* 0x00: xyz only */
    f32 orientationAngle;   /* 0x10 */
    f32 tiltAngle;
    f32 radius;
    f32 radiusStep;
    f32 angularSpeed;
    f32 initialPhase;
    f32 height;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} FileOrientedRingMotion;   /* 0x38, ordinary word alignment */

extern void SetRotateMatrixX(f32 angle);

extern void SetRotateMatrixY_B(f32 angle);

/* vu0 routine: advances a ring in its sampled orientation with vertical acceleration. */
void effUpdateOrientedRingEmitterSlots(FileSlotTable *record);

/* Scale selected oriented-ring parameters; keep the destination-first declarations. */
void effScaleOrientedRingEmitterParameters(ScaleOwner *owner, f32 scale);

/* vu0 routine: advances directed slots through the emitter quaternion frame. */
void effUpdateDirectedEmitterSlots(FileSlotTable *record);

/* Scale selected directed-emitter parameters; this is the DDS1 callback's twin. */
void effScaleDirectedEmitterParameters(ScaleOwner *owner, f32 factor);

/* vu0 routine: advances slots within a bounded azimuth of the emitter frame. */
void effUpdateSectorEmitterSlots(FileSlotTable *record);

/* Scale selected parameters for the bounded-azimuth sector emitter. */
void effScaleSectorEmitterParameters(ScaleOwner *owner, f32 factor);

/* Replace type-seven curve-emitter start xyz; other record types are untouched. */
void effSetCurveEmitterStartPosition(FileSlotTable *record, const f32 *value);

/* Replace type-seven curve-emitter end xyz; other record types are untouched. */
void effSetCurveEmitterEndPosition(FileSlotTable *record, const f32 *value);

/* cubic Bezier point at t through four control points; result is left in vf10 */
void vu0CubicBezierPoint(f32 *controlPoint0, f32 *controlPoint1,
                         f32 *controlPoint2, f32 *controlPoint3, f32 t);

typedef struct FileCurveMotion {
    f32 firstControl[3];
    f32 secondControl[3];
    f32 rotationReference[3]; /* native live-path input; writer unresolved */
    f32 endpointOffset[3];
    f32 angularSpeed;
    f32 travelSpeed;
    f32 pathLength;
    f32 progress;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} FileCurveMotion; /* 0x4C, naturally four-byte aligned */

extern f32 effWorldUpVector[4];

/* vu0 routine: advances randomized cubic-curve slots and their axial rotation. */
void effUpdateCurveEmitterSlots(FileSlotTable *record);

/* Curve-emitter scaling changes only the common scale-track values. */
void effScaleCurveEmitterParameters(ScaleOwner *owner, f32 scale);

FileSlotTable *fileAllocateGridRecordSlots(u16 type, u32 count, const void *data);

void effPCPHelperBEDGetModel(FileSlotTable *record);

void fileClearRecordReferences(FileSlotTable *record);

void fileAcquireRecord(FileSlotTable *record);

void fileReadVectorPtr20(u8 *recordBytes, void *dst);

void mnuRecordSetVector(void *recordBytes, const void *src);

void fileReadRecordSecondVector(u8 *recordBytes, void *dst);

void fileSetRecordSecondVector(void *recordBytes, const void *src);
/* End of declarations from src/dds2/game/code_002C96D0.c */

/* Declarations from dds-decomp src/dds2/game/code_002DC108.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "dds/common.h"

#include "dds/eff_emitter_type.h"

/* Persona 4 func_00492e30 @ 00492E30 (src/promoted/code1_0049.c), recompiled unchanged */
void dds3DispatchIndexedCallback(u16 *record, f32 scale);
/* End of declarations from src/dds2/game/code_002DC108.c */

/* Declarations from dds-decomp src/dds2/game/code_002DC138.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "sdk_libc.h"

#include "dds/btl_grouped_entity_api.h"

#include "dds/mdl.h"

#include "dds/sdf.h"

#include "dds/sdf_chunk.h"

#include "dds/sdf_map_position_api.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_projection.h"

#include "dds/sdf_tex_create.h"

#include "dds/sdf_tex_packet.h"

#include "dds/sdf_texture_format.h"

#include "dds/sdf_texture_submit.h"

#include "dds/sdf_vu_matrix.h"

/* Build a look-at basis from four-float target, eye, and up vectors. */
void LookAtMatrix(const f32 *targetPoint, const f32 *eyePosition, const f32 *upVector);

/* Apply the primary VU0 matrix bank vf28-vf31 to basis vectors vf24-vf27,
 * then replace vf28-vf31 with the four transformed vectors. Provider bodies:
 * DDS1 game/code_002DD8B8.c and DDS2 game/code_00336768.c. */
void MultMatrixNoArg(void);

enum {
    SDF_PSMCT32 = 0x00,
    SDF_PSMCT24 = 0x01,
    SDF_PSMCT16 = 0x02,
    SDF_PSMCT16S = 0x0A,
    SDF_PSMT8 = 0x13,
    SDF_PSMT4 = 0x14,
    SDF_PSMT8H = 0x1B,
};

/* Model-context lifetime, per-frame update, and broadcast accessors. */
/* Destroy one context. If it leaves its resource-owning group empty, group
 * teardown also releases that group's registered assets. */
void mdlFree(dds3ModelHandle_t *ctx);

u32 mdlGetColor(const dds3ModelHandle_t *ctx);

u16 mdlGetMajorNum(const dds3ModelHandle_t *ctx);

u16 mdlGetMinorNum(const dds3ModelHandle_t *ctx);

/* VU helpers transfer vectors through vf10 and basis rows through vf28-vf30. */
void mdlGetPosition(dds3ModelHandle_t *ctx);

void mdlGetScale(dds3ModelHandle_t *ctx);

void mdlLink(dds3ModelHandle_t *ctx, struct sdfGrOt_t **surfaces);

/* Allocate a distinct caller-owned context linked to the registered group.
 * Contexts share the group node and assets; release each context with mdlFree. */
dds3ModelHandle_t *mdlLoad(u32 group, u32 id);

void mdlLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

/* Return the first node of this type in the group, or NULL. */
dds3ModelResrc_t *mdlSearchResrc(s32 group, s32 type);

void mdlSetColor(dds3ModelHandle_t *ctx, u32 value);

void mdlSetPosition(dds3ModelHandle_t *ctx);

void mdlSetRotate(dds3ModelHandle_t *ctx);

void mdlSetScale(dds3ModelHandle_t *ctx);

/* sdfMotionCtrlr_t-record selection and lookup. */
void mdlSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

void sdfLinkLoadImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 destinationX,
    s32 destinationY, s32 width, s32 height, s32 sourceAddress,
    s32 (*allocatePacket)(s32));

u32 sdfModelGetMarkPosCount(const struct dds_sdfModelRootNode_t *model);

SdfMapPositionLookupResult sdfModelGetMarkPosMatrix(
    struct dds_sdfModelRootNode_t *model, s32 id);

void sdfMotionSeek(struct sdfMotionCtrlr_t *motion, f32 frame);

/* Allocate and register a texture. The shared registration helper initializes
 * its ordinary reference count to one; the caller releases that reference with
 * sdfTexReleaseReference or its deferred form. */
struct sdfTexHandle_t *sdfTexAlloc(s32 width, s32 height,
    SdfGsPixelStorageFormat pixelFormat, u32 maxMipLevel,
    SdfGsPixelStorageFormat clutFormat, u32 paletteCount);

u32 sdfTexGetCLUTAddr(sdfTexHandle_t *texture);

/* Texture resource accessors and palette helpers shared by both title providers. */
u8 sdfTexGetCLUTCount(sdfTexHandle_t *texture);

u32 sdfTexGetPixelAddr(sdfTexHandle_t *texture);

/* Store the context-one resource packet on the texture's primary buffer. */
void sdfTexMakeGIFPacket(struct sdfTexHandle_t *texture);

/* Submit a palette using dimensions selected from the texture pixel format. */
const u8 *sdfTexTransCLUT(sdfTexHandle_t *texture, u32 destination,
                    const u8 *pixels,
                    SdfTextureUploadOwnership borrowPixels);

/* Reads the source bytes and returns its cursor advanced past the submitted image. */
const u8 *sdfTexTransData(u32 destination, s32 width, s32 height, u32 format,
                    const u8 *pixels,
                    SdfTextureUploadOwnership borrowPixels);

#include "dds/sdk_ps2.h"

#include "dds/common.h"

#include "dds/sdf_map_position_api.h"

#include "dds/eff_texture_table.h"

#include "dds/sdf_vu0.h"

#include "dds/eff_object_node.h"

#include "dds/eff_model_owner.h"

#include "dds/sdf_motion.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_chip.h"

#include "dds/eff_ref_obj.h"

#include "dds/sdf_heap_api.h"

#include "dds/eff_anim.h"

#include "dds/file.h"

#include "dds/pcp_vu0.h"

#include "dds/ee_mmi.h"

#include "dds/mdl.h"

#include "dds/mdl_broadcast_callback.h"

#include "dds/btl_grouped_entity_api.h"

#include "dds/sdf.h"

#include "dds/sdf_texture_submit.h"

#include "dds/sdf_texture_file.h"

#include "dds/sdf_tex_packet.h"

#include "dds/sdf_tex_create.h"

#include "dds/sdf_chunk.h"

#include "dds/eff_expanded_source.h"

#include "dds/eff_update_flags.h"

#include "dds/btl_runtime.h"

extern u8 btlIsRuntimeAllocated(void);

extern u8 btlIsCurrentActorFullyMarked(void);

extern dds3ModelHandle_t *effLoadViewerModelWithVUState(void *, u32);

extern void mdlPackDecode(s32, s32, s32, const u8 *, s32);

extern u16 D_00437E2C;

extern EffModelOwner *effCreateModelOwner(FileJobPayload *);

extern void effRecreateModelFromSource(EffModelOwner *, EffModelOwner *);

#include "dds/sdf_debug_api.h"

extern EffectObjectNode *effFloorModelListHead;

extern u32 D_00437E38;

extern sdfGrOt_t *D_00437E3C;

extern s32 effComputeLightDirectionVU(dds3ModelHandle_t *, sdf3DLightParallelPacket_t *);

extern void effFloorModelListRemove(EffectObjectNode *);

#include "dds/sdf_packet_alloc.h"

extern sdfGrOt_t *kwlnSceneSurfaceSets[13][4];

extern u64 D_003E9640[];

extern u32 effSharedTextureReferenceCount;

extern sdfTexHandle_t *D_00437E40;

/* Initialize the VU transforms and the first node's float slot, if present. */
void effInitModelVUState(dds3ModelHandle_t *model);

/* Load under an unused group-6 viewer ID and initialize the model's VU state. */
dds3ModelHandle_t *effLoadViewerModelWithVUState(void *modelData, u32 modelDataSize);

/* Clear the lighting attachment before destroying its model context. */
void effDestroyModelContext(dds3ModelHandle_t *owner);

dds3ModelHandle_t *effCloneModelWithVUState(dds3ModelHandle_t *sourceModel);

EffModelOwner *effCreateModelOwner(FileJobPayload *source);

void effDestroyModelOwner(EffModelOwner *owner);

EffModelOwner *effDuplicateEffectHeader(EffModelOwner *source);

void effRecreateModelFromSource(EffModelOwner *owner, EffModelOwner *source);

void effModelAnimationStop(EffModelOwner *owner);

/* Attach the owner's lighting buffer when the direction update succeeds. */
void effRefreshModelLighting(EffModelOwner *work);

void effApplyModelPrimaryVector(EffModelOwner *owner, const f32 *position);

void effApplyModelVecB(EffModelOwner *owner, const f32 *rotation);

void effBroadcastModelMask(EffModelOwner *owner, u32 color);

void effApplyScaledModelTertiaryVector(EffModelOwner *owner, float scale);

/* Battle state: resource headers for texture slots 1 and 2. */
typedef struct EffBattleTexHeaders {
    u8 pad_000[0x5B0];
    u8 *slot1; // 0x5B0
    u8 *slot2; // 0x5B4
} EffBattleTexHeaders;

void effUploadModelTextures(EffModelOwner *owner);

extern void effFloorModelListPush(EffModelOwner *);

/* Track floor models only while battle is active and the current actor is not fully marked. */
EffModelOwner *effCreateFloorModelOwner(FileJobPayload *source);

void effMarkFloorModelForDestruction(EffModelOwner *p);

EffModelOwner *effDuplicateFloorModelOwner(EffModelOwner *source);

/* Render the model owner with staged GS TEST_2 packets, then restore the tests. */
void effRenderModelOwnerWithGsTests(EffModelOwner *owner);

void effMarkFloorModelForUpdate(EffModelOwner *p);

void effFloorModelListPush(EffModelOwner *obj);

void effFloorModelListRemove(EffectObjectNode *node);

void effSweepFloorModelList(void);

void mdlPropagateObjectFlag(void);

void mdlClearListedObjectFlag(void);

void mdlSetListedObjectFlag(void);

void mdlMarkAndProcessObjectNodes(void);

#include "dds/eff_resource_owner.h"

extern EffResourceOwner *effCreateResourceOwner(FileJobPayload *);

/* Decode the type-6 configuration, load its model and clone per-position queues. */
EffResourceOwner *effCreateResourceOwner(FileJobPayload *job);

void effDestroyResourceOwner(EffResourceOwner *owner);

extern void effCopyResourceOwner(EffResourceOwner *, EffResourceOwner *);

EffResourceOwner *effDuplicateModelOwner(EffResourceOwner *source, u16 unusedType);

void effCopyResourceOwner(EffResourceOwner *dst, EffResourceOwner *src);

/* Complete queued jobs, sample the first model motion at zero, and clear the owner word. */
void effResetModelMotionAndOwnerFlag(EffResourceOwner *owner);

#include "dds/eff_curve.h"

#include "dds/sdf_projection.h"

#include "dds/sdf_vu_transform_api.h"

#include "dds/sdf_vu_rotation_axes.h"

#include "dds/sdf_vu_matrix.h"

extern void effGetWorldVector(u32);

extern s32 effResolveRequestPositionIntoVu(EffectVectorRequest *, s16);

extern void Normalize(void);

extern void btlDebugDrawSegment(f32 *, f32 *, u32);

extern void btlDebugDrawPoint(f32 *);

extern f32 D_003E9660[4];

extern f32 D_003E9130[4];

extern f32 D_003E9120[4];

/* Resolve attachment endpoints, advance the model, and update retained effect queues. */
void func_002DD448(EffResourceOwner *owner);

void effLoadModelPrimaryVector(EffResourceOwner *owner, const f32 *position);

void effSetModelRotationQuaternion(EffResourceOwner *owner, const f32 *rotationQuaternion);

void effPropagateResourceModelMask(EffResourceOwner *owner, u32 color);

void effScaleModelVec(EffResourceOwner *owner, float scale);

void effObjectListCountersReset(void);

extern s32 sdfImageDataSize(u32 format, s32 width, s32 height);

RefObj *effCreateSharedTextureReference(SdfTextureFileHeader *source);

RefObj *effCloneSharedReferenceWithValue(SdfTextureFileHeader *source, u32 textureIndex);

void effReleaseSharedReference(RefObj *obj);

RefObj *effRetainSharedReference(RefObj *obj);

extern sdfTexHandle_t *sdfGetOrCreateTextureForReference(sdfGrOt_t *, RefObj *);

#include "dds/sdf_packet_builders.h"

sdfTexHandle_t *sdfGetOrCreateTextureForReference(sdfGrOt_t *surface, RefObj *source);

s32 effCountExpandedEntries(const EffExpandedList *list);

void effReleaseReferenceHolder(EffExpandedList *holder);

EffExpandedList *effReferenceObjectRetain(EffExpandedList *obj);

/* One animation track: segments contribute their length plus a boundary frame;
 * the loop flag wraps the frame by the total track length. */
void effSampleAnimSet(const EffExpandedList *set, u32 frame, EffAnimSample *out);

sdfTexHandle_t *effAssignSampledSegmentReference(const EffExpandedList *set, sdfGrOt_t *target,
                                                 const EffAnimSample *sample);
/* End of declarations from src/dds2/game/code_002DC138.c */

INCLUDE_ASM(const s32, "effect/effPCPMisc_0023C018", effPCPHelperBEDGetModel);

INCLUDE_ASM(const s32, "effect/effPCPMisc_0023C018", func_r1_00224D28);

INCLUDE_ASM(const s32, "effect/effPCPMisc_0023C018", func_r1_00224D40);

void fileReadVectorPtr20(u8 *recordBytes, void *dst) {
    FileSlotTable *record = (FileSlotTable *)recordBytes;
    SDF_VU0_COPYVECTOR(dst, record->data0);
}

void mnuRecordSetVector(void *recordBytes, const void *src) {
    FileSlotTable *record = (FileSlotTable *)recordBytes;
    SDF_VU0_COPYVECTOR(record->data0, src);
}

void fileReadRecordSecondVector(u8 *recordBytes, void *dst) {
    FileSlotTable *record = (FileSlotTable *)recordBytes;
    SDF_VU0_COPYVECTOR(dst, record->data0 + 0x10);
}

void fileSetRecordSecondVector(void *recordBytes, const void *src) {
    FileSlotTable *record = (FileSlotTable *)recordBytes;
    SDF_VU0_COPYVECTOR(record->data0 + 0x10, src);
}

/* Persona 4 func_00492e30 @ 00492E30 (src/promoted/code1_0049.c), recompiled unchanged */
void dds3DispatchIndexedCallback(u16 *record, f32 scale) {
    effEmitterTypeDescriptors[*record].scaleParameters(record, scale);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc_0023C018", effInitModelVUState);

INCLUDE_ASM(const s32, "effect/effPCPMisc_0023C018", func_r1_00224EB0);

/* Clear the lighting attachment before destroying its model context. */
void effDestroyModelContext(dds3ModelHandle_t *owner) {
    owner->inner->lighting = NULL;
    mdlFree(owner);
}

dds3ModelHandle_t *effCloneModelWithVUState(dds3ModelHandle_t *sourceModel) {
    s32 group;
    s32 id;
    dds3ModelHandle_t *model;

    group = mdlGetMajorNum(sourceModel);
    id = mdlGetMinorNum(sourceModel);
    model = mdlLoad(group, id);
    effInitModelVUState(model);
    return model;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc_0023C018", func_r1_00224FB8);

INCLUDE_ASM(const s32, "effect/effPCPMisc_0023C018", func_r1_00225058);

INCLUDE_ASM(const s32, "effect/effPCPMisc_0023C018", func_r1_002250A8);

INCLUDE_SDATA(const s32, "effect/effPCPMisc_0023C018", D_r1_0046D0FC);

