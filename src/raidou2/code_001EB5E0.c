#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_0028A150.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "fileManager.h"

#include "newdata.h"

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

#include "dds/sdf_texture_file.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf_vu_matrix_multiply_api.h"

#include "dds/sdf_vu_rotation_axes.h"

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

/* Load the X-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixX(f32 angle);

/* Load the secondary Y-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixY_B(f32 angle);

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

void dds3FadeIn(s32 red, s32 green, s32 blue, s32 duration);

/* Foreground alpha transitions, stepped by dds3FadeUpdate. */
void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration);

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

void *sdfChipAllocDirect(s32 size);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

f32 sdfCos(f32 angle);

void sdfFileClose(sdfFileCmdHandle_t *state);

u32 sdfFileLength(sdfFileCmdHandle_t *state);

/* Stable state APIs. Callback factories keep caller-specific callback types. */
sdfFileCmdHandle_t *sdfFileOpen(const char *path);

void sdfFileRead(sdfFileCmdHandle_t *state, void *buffer, s32 byteCount);

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

#include "dds/common.h"

#include "dds/file_menu_start_mode.h"

#include "dds/file_menu_state_result.h"

#include "dds/file_config_list_node.h"

#include "dds/file_config_option_api.h"

#include "dds/sdf_vu0.h"

#include "dds/file_job_type.h"

#include "dds/file_config_task.h"

#include "dds/sdf_vu_rotation_axes.h"

#include "dds/file_load_menu_state.h"

#include "dds/eff.h"

#include "dds/cam_aim_record.h"

#include "dds/eff_generated_texture_descriptor.h"

#include "dds/dds3_world_selection.h"

#include "dds/mnu_title_stream.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/sdf_trig.h"

#include "dds/eff_scale_params.h"

#include "dds/ee_mmi.h"

#include "dds/eff_random.h"

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

#include "dds/pcp_vu0.h"

#include "dds/sdf_vu_transform_api.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf_vu_matrix_multiply_api.h"

#include "dds/kwln.h"

#include "dds/dds3_task_create.h"

#include "dds/fpu.h"

#include "dds/mnu_list.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/eff_expanded_list.h"

#include "dds/file_request_api.h"

#include "dds/mdl_flag_api.h"

#include "dds/sdf.h"

#include "dds/dds3_fade.h"

#include "dds/file_pac.h"

#include "dds/sdf_pac_work.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/file_request_api.h"

#include "dds/sdf_texture_file.h"

#include "dds/mdl_flag_api.h"

#include "dds/btl.h"

#include "dds/fr_font.h"

#include "dds/fr_font_draw_glyph.h"

#include "dds/itf.h"

typedef s32 (*FileMenuContinuationCallback)(void);

/* Declared before first use (were implicit int). */
extern void effLoadFlashTextures(void);

extern void effLoadScalyTexture(void);

extern void effLoadWindTexture(void);

extern void dds3SoftResetOn(void);

extern void frEnableMode(s32 mask);

extern u8 frDisableMode(u8 mask);

extern void frSetFontDrawOT(u32 renderFlags);

extern s32 mcResetSlotMetadata(void);

extern void p2sdStopBGM(u32 trackHandle);

extern void *memset(void *dst, s32 value, s32 size);

typedef struct MenuResourceWork MenuResourceWork;

struct MenuListNode;

extern void mnuClearListFlagsOneAndTwo(u32 *flags);

struct EffectSlotSet;

extern void effDrawSpriteWithCornerFade(s32, s32, s32, u32, u32, struct EffectSlotSet *, s32, s32);

enum {
    FILE_CONFIG_SET,
    FILE_CONFIG_FRAME,
    FILE_CONFIG_X,
    FILE_CONFIG_Y
};

extern s16 fileConfigSpritePlacements[27][4];

extern FileScrollArrowState fileScrollArrowState;

extern f32 D_003BC83C;

extern u8 fileNewCycleLabel[];

extern u8 D_00347C68[][32];

extern u8 D_003482A8[][32];

extern char D_003BC898[];

extern char D_003BC8A0[];

extern char D_003BC8A8[];

extern u32 fileClampPackedColorByte(u32, s32, s8);

extern void fileDrawSlotIcon(s32, s32, s32, s32);

extern void fileCursorStepUp(void);

extern void fileFadeStepDown(void);

extern void fileLoadCtxSlideUpdate(void);

extern void dds3PadVibrationSet(s32, u8, s32);

extern struct sdfTexHandle_t *fileSavePreviewPortraitTextures[5];

extern struct sdfTexHandle_t *D_003BC868;

extern struct sdfTexHandle_t *D_003BC86C;

extern struct sdfTexHandle_t *D_003BC870;

extern struct sdfTexHandle_t *D_003BC874;

extern struct sdfTexHandle_t *D_003BC878;

extern struct sdfTexHandle_t *D_003BC87C;

extern struct sdfTexHandle_t *D_003BC880;

extern struct sdfTexHandle_t *D_003BC884;

extern u8 aViewEye[];

extern u8 aViewTarget[];

extern u8 aViewDown[];

extern s32 slFormatStart(void);

extern s32 slFormatCancel(void);

/* Ten contiguous 0x30-byte save-preview records (0x1E0 bytes total). */
extern FileSavePreviewRecord fileSavePreviewRecords[10];

extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 fileReadSlotPreviewBegin(void);

extern s32 fileAdvanceSlotScan(void);

extern FileMenuStateHandler fileMenuStateHandler;

extern s32 D_003BC814;

extern s32 fileDrawMenuFrame(s32);

#include "dds/eff_emitter_type.h"

extern struct EffExpandedList *effCreateTextureAnimationSet(u32);

struct sdfMemHandle_t;

extern u32 effModelUpdateControlFlags;

extern u32 D_003BC888;

extern void *fileSaveReadBuffer;

extern struct _FRQ *fileMenuConvertedTextGlyph;

extern FileMenuContinuationCallback fileWaitContinuation;

extern s32 fileWaitTicksRemaining;

extern s32 slExitWaitProc(void);

extern s32 fileSlotStatusPoll(void);

extern void fileResetSlotStates(FileSlotTable *record);

extern char D_003BC940[];

extern char D_003B2688[];

extern u32 fileSaveIconData;

extern u32 fileSaveIconDataSize;

extern const u32 *fileWriteDataAddressSlot;

extern const u32 *fileWriteDataSizeSlot;

extern FileMenuContinuationCallback fileWriteContinuation;

extern struct FileRequest * const *fileWritePendingRequestSlot;

extern u32 D_003BC824;

extern s32 slSaveWriteFile(const char *fileName, const u32 *dataAddressSlot, const u32 *dataSizeSlot, FileMenuContinuationCallback continuation, struct FileRequest * const *pendingRequestStatus);

extern s32 slSaveProc_openFile(void);

extern s32 slSaveProc_writeFile(void);

extern s32 fileBeginLabeledSlotWrite(void);

extern s32 fileBuildMainBlobAfterDelete(void);

extern McFileDescriptor fileSaveFileDescriptor;

extern struct sdfMemHandle_t *fileSaveReadBufferResource;

extern void sdfMcFuncClose(McFileDescriptor fileDescriptor);

extern s32 fileStoreSlotHeader(void);

extern s32 slSaveData_chdir(void);

extern s32 slEndingExit(void);

extern s32 slSaveData(void);

extern s32 fileSlotSelectPollClear(void);

extern s32 slEndingUpd2(void);

typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;

extern u32 fileMainSaveBlobAddress;

extern u32 fileMainSaveBlobSize;

extern LoadMirror fileSlotFlagMirror;

extern u32 fileSavedSlotFlags;

extern char fileConfigInputTaskName[];

extern char fileConfigLoadTaskName[];

extern char fileConfigOwnerTaskName[];

#include "dds/sdf_debug_api.h"

extern char D_003BC900[];

extern char D_003BC908[];

extern char D_003BC910[];

extern char D_003BC918[];

extern char D_003BC920[];

extern u32 D_003BC81C;

extern FileSelectionFlowState fileSelectionPending;

extern u32 D_003BC854;

extern u32 D_003BC80C;

extern u32 D_003BC810;

extern u32 fileMemoryCardRequestIndex;

extern s32 D_003BC850;

extern s32 D_003BC860;

extern u32 D_003BC858;

extern s32 D_003BC864;

extern s8 fileSavePreviewVersionByteView[];

extern FileMenuContinuationCallback fileMenuContinuation;

extern struct sdfMemHandle_t *fileSaveIconResourceHandle;

extern struct FileRequest *fileSaveIconRequest;

extern struct _FRQ *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, struct _FRQ *);

extern struct _FRQ *fileMenuStringGlyph;

extern s32 D_003BC7FC;

extern s8 slLoadKillFlag;

extern dds3ProcessID_t *D_003BC7F0;

/* Loader context at fileLoadMenuState. */
extern FileLoadMenuState fileLoadMenuState;

/* Far scalar: incomplete array forces non-small-data addressing. */
extern s32 D_0037D4D0[];

extern sdfGrOt_t *effPacketSubmissionSurfaces[6];

extern char D_003B2668[];

extern void fileDrawLoadObjectSlots(s32 object);

extern s32 fileResetSelection(void);

extern s32 D_003BC818;

extern s32 D_003BC820;

extern void fileSetMenuValueAndInitializeFlags(u32 value);

extern void fileLoadSetMode(FileLoadMenuSlideMode mode);

extern s32 slEndingTop(void);

extern s32 fileSlotSelectPoll(void);

extern s32 fileOnAllWritten(void);

extern s32 mcdFinishFileDetection(void);

extern s32 fileSlotSelectionPollCount;

extern s32 fileRequestDefaultMode(void);

extern s32 slBootRetry(void);

extern s32 slBootError(void);

extern s32 slSizeChk(void);

extern s32 fileSelectedSlotIndex;

extern s32 fileBeginDetectionRequest(FileMenuContinuationCallback callback);

extern s32 fileScanSlotStates(void);

extern s32 filePollSlotRequestAndResumeFlow(void);

extern u32 D_003BC804;

extern void dds3SoftResetOff(void);

extern s32 mcHandleDetectionResult(void);

extern void fileQueueInitTransform(void *queue);

extern void sdfVuMatrixToQuaternion(const f32 matrix[4][4]);

extern void mnuRecordSetVector(FileSlotTable *record, const void *src);

extern void fileSetRecordSecondVector(FileSlotTable *record, const void *src);

extern void dds3AdminiCallSequence(s32 kind, const void *data, u32 size, s32 flags);

extern FileMenuStartMode D_003BC848;

extern s32 fileSlotScanIndex;

extern char D_003B2658[];

extern s32 slExitWait(FileMenuContinuationCallback callback);

extern void mcFormatSaveFilename(char *dst, s32 number);

extern s32 slChooseData_getCardInfo2(void);

extern char D_003B2678[];

extern sceMcTblGetDir fileDirectoryEntryScratch[];

extern s32 slChooseData_getCardInfo3(void);

extern s32 fileBeginSlotMetadataRefresh(void);

extern s32 slHeadLoad(void);

extern s32 fileScanSlotStatesAdvance(void);

extern s32 slChooseData_getCardInfo3_B(void);

extern s32 slChooseData_getCardInfo2_B(void);

extern s32 slBootError2(void);

extern u32 D_003BC834;

extern s32 filePollSlotDetectionAndBranch(void);

extern s32 slLoadProc_openFile(void);

extern s32 slLoadProc_readFile(void);

extern s32 filePrepareMainBlobWrite(void);

extern s32 slSaveData_deleteIconSys(void);

extern s32 slErrorWait(void);

extern s32 slSaveProc_closeFile(void);

extern s32 slSaveProc_Write_mkdir(void);

extern s32 fileBuildMainBlobAndWrite(void);

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

extern u32 D_0037D4AC[];

/* Object with loader sub-objects (+0x40...). */
typedef struct LoadObj {
    void *owner;        /* 0x00 */
    u32 color;          /* 0x04 */
    f32 scale;          /* 0x08 */
    u32 selector;       /* 0x0C: secondary job buffer operation */
    u8 pad10[0x24];
    void *deviceHandle; /* 0x34 */
    FileJobPayload **jobs; /* 0x38: child file-job payloads */
    struct sdfMemHandle_t *jobAllocation;          /* 0x3C */
    struct EffExpandedList *referenceHolder; /* 0x40 */
    FileSlotTable *recordWork; /* 0x44: created by fileAllocateGridRecordSlots */
    s16 unk48;          /* 0x48 */
    u16 unk4A;
} LoadObj;

extern s32 mcdHandleSaveSetupDone(void);

extern void fileDrawConfigurationMenuBackground(void *);

extern void mnuCallInitWide(s32, s32, s32, u32, s32);

extern LoadObj *fileLoadObjectCreate(void *owner);

extern void fileCloneEffectSurfaceResources(LoadObj *result, LoadObj *owner);

extern void fileJobFreePrimaryBuffer(FileJobPayload *job);

extern void fileJobFreeSecondaryBuffer(FileJobPayload *job);

extern FileJob *fileJobCreate(void);

extern FileJob *fileQueueFindById(FileQueue *, u32);

extern s32 fileFindQueuedJobIndex(FileQueue *, FileJob *);

extern void fileQueueAppend(FileQueue *queue, FileJob *job);

extern s8 dds3ConfigState;

void slKillLoadProcess(void);

s8 fileMenuTaskIsAlive(void);

void mnuFormatSaveSlotHeaderName(void);

void mcFormatSaveFilename(char *dst, s32 number);

void fileReqGetSlotCode(void);

#define FILE_MAIN_BLOB_SIZE 0x33600

u32 fileMainBlobSize(void);

void fileReloadSaveBuffer(void);

u8 fileIsLoadedAndConditionTrue(s32 condition);

u8 fileIsLoadedWithActiveFlow(s32 loaded);

void mnuDrawAndStoreTextGlyphHandle(s32 x, s32 y, u32 colors, const u8 *text);

extern _FRQ * frFontBuildGlyphChain(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *existingGlyph);

void mcdCreateFontDrawHandle(s32 x, s32 y, u32 color, const char *text);

void fileDrawMenuImageAtPoint(s32 x, s32 y, u32 colors, char *text);

extern f32 fileSaveHighlightPhase;

extern void eglSetOt(u32);

extern void evtSubmitPrimaryAlphaBlendMode(s32);

/* Animate the save-window highlight's alpha with a sinusoidal phase. */
void fileDrawPulsingSaveHighlight(void);

extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, s32, s32, s32, s32);

extern void fileCursorPulseUpdate(void);

extern void fileDrawCursorPulse(s32, s32, s32);

extern s32 D_003BC85C;

void fileDrawSaveWindow(void);

s32 slChkWindowOpenDone(void);

FilePromptRevealResult fileDrawStatusDialogFrame(void);

extern char fileCheckingCardMessageLine1[];

extern char fileCheckingCardMessageLine2[];

extern char fileCheckingCardMessageLine3[];

extern char fileSavingText[];

extern char fileCardBusyCardWarningText[];

extern char fileCardBusyControllerWarningText[];

extern char fileCardBusyConsoleWarningText[];

extern char fileLoadingText[];

extern char fileSaveSuccessfulText[];

extern char fileFormattingText[];

extern char fileFormatSuccessfulText[];

extern char fileLoadSuccessfulText[];

extern s32 D_003BC840;

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

static inline void fileDrawDialogLine(s32 line, const char *glyphSource) {
    mcdCreateFontDrawHandle(86, 114 + line * 24, 0x89FEFF80U, glyphSource);
}

void fileShowStatusDialog(void);

void slSetInfo(u32 state);

void fileShowFlowDialog(void);

void fileClearAllSlotFlags(void);

s32 slHeadLoad(void);

extern s32 slHeadLoadProc_readFile(void);

s32 fileReadSlotPreviewBegin(void);

s32 slHeadLoadProc_readFile(void);

s32 fileStoreSlotHeader(void);

s32 slExitWait(FileMenuContinuationCallback callback);

s32 slEndingTop(void);

s32 slEndingUpd2(void);

s32 slEndingExit(void);

s32 slBootTop(void);

s32 slBootRetry(void);

s32 slBootError(void);

s32 slBootError2(void);

s32 fileBeginSlotMetadataRefresh(void);

s32 fileResetSelection(void);

s32 slExit(void);

s32 mcdEnterDefaultFileFlow(void);

s32 fileRequestDefaultMode(void);

s32 mcdEnterSelectedFileFlow(void);

s32 fileRestartSlotScan(void);

extern s32 D_003BC82C;

extern s32 fileSlotBrowserUpdate(void);

s32 fileScanSlotStates(void);

s32 slCancel(void);

s32 fileLoadIconFileAndResetSelection(void);

s32 fileResetSlotSelection(void);

s32 fileLoadIconFileAndBeginSlotReset(void);

s32 slBootProc1st(void);

s32 fileSlotStatusPoll(void);

s32 fileCountSelectableFiles(void);

s32 slSizeChk(void);

s32 fileSlotSelectPoll(void);

extern s32 mcClearSlotMetadata(void);

s32 fileSlotSelectPollClear(void);

extern s8 dds3PadButtonState[];

s32 filePollSlotDetectionAndBranch(void);

s32 slErrorWaitToFileMc(void);

s32 slErrorWait(void);

s32 slExitWaitProc(void);

s32 fileSetMenuCallbackAndClearResult(FileMenuContinuationCallback callback);

s32 mcdFinishFileDetection(void);

s32 fileBeginDetectionRequest(FileMenuContinuationCallback callback);

s32 filePollSlotRequestAndResumeFlow(void);

s32 fileBeginReadSlotIcon(void);

s32 slChooseData_getCardInfo2(void);

s32 slChooseData_getCardInfo3(void);

s32 fileAdvanceSlotScan(void);

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

extern u32 D_003BC890;

extern u32 D_003BC894;

s32 fileBeginLabeledSlotWrite(void);

s32 slSaveWriteEnd_saveData(void);

s32 fileBuildMainBlobAndWrite(void);

s32 slSaveData_deleteIconSys(void);

/* Later save callbacks read these borrowed slots without changing them; callers
 * keep the slots alive until the asynchronous save completes. */
s32 slSaveWriteFile(const char *fileName, const u32 *dataAddressSlot, const u32 *dataSizeSlot, FileMenuContinuationCallback continuation, struct FileRequest * const *pendingRequestStatus);

extern s32 slSaveProc_waitDataReady(void);

s32 slSaveProc_openFile(void);

s32 slSaveProc_writeFile(void);

s32 slSaveProc_closeFile(void);

s32 slSaveProc_waitDataReady(void);

s32 mcHandleDetectionResult(void);

s32 slFormatStart(void);

s32 slFormatCancel(void);

s32 slLoadData(void);

s32 slLoadProc_openFile(void);

s32 slLoadProc_readFile(void);

extern s8 D_003BC7ED;

extern void fileCacheSlotFlagsFromState(void);

extern void fileRestoreSlotFlagsToState(void);

s32 mcdHandleSaveSetupDone(void);

s32 fileSlotBrowserUpdate(void);

s32 fileDrawSlotListAndPreview(s32 work);

s32 slCalc(dds3ProcessID_t *task);

s32 fileDrawMenuFrame(s32 work);

extern s32 D_003BC830;

extern s32 D_003BC838;

void fileDestroyMemoryCardMenuTask(void);

void fileInitCursorPulse(void);

/* Load the memory-card pack textures, pick the menu entry state for the
 * requested mode (load, slot select, new-cycle save, or boot) and
 * start the file-menu task with every flow counter cleared. */
void fileEnterMcPackScene(FileMenuStartMode mode);

void fileReleaseMenuResources(void);

void fileDestroyMemoryCardMenuTask(void);

void fileMenuReleaseNoOp(void);

void fileReleaseMenuResources(void);

u32 fileMenuOperationSucceeded(void);

s32 fileMenuTaskExists(void);

FileSelectionFlowState fileGetSelectionPendingFlag(void);

extern FileSelectionCallback D_003BD908, D_003BD90C;

/* Yes/no cursor input while the card request is idle. */
static inline void filePollPromptChoice(void) {
    if (D_003BC838 == 0 && slChkWindowOpenDone() != 0) {
        if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x27] < 0) != 0) {
            if (D_003BC830 != 1) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_003BC830 = 1;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x26] < 0) != 0) {
            if (D_003BC830 != 0) {
                p2sdPlaySE(0, 0x7F, 0x3F);
            }
            D_003BC830 = 0;
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x21] < 0) != 0) {
            if (D_003BC830 == 0 && (D_003BC834 == 1 || D_003BC834 == 3)) {
                p2sdPlaySE(0x11, 0x7F, 0x3F);
            } else {
                p2sdPlaySE(8, 0x7F, 0x3F);
            }
            if (D_003BC830 != 0) {
                D_003BC838 = 2;
            } else {
                D_003BC838 = 1;
            }
        } else if (fileIsLoadedAndConditionTrue(dds3PadButtonState[0x23] < 0) != 0) {
            p2sdPlaySE(0xA, 0x7F, 0x3F);
            D_003BC838 = 2;
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

void fileInitCursorPulse(void);

void fileCursorPulseUpdate(void);

extern const s32 D_003B28A0[3][2];

void fileDrawCursorPulse(s32 x, s32 y, s32 surface);

void fileLoadSetMode(FileLoadMenuSlideMode mode);

void fileResetLoadContextSlide(void);

void fileLoadCtxSlideUpdate(void);

FileSlotFlagsChangeResult fileLoadStateChanged(void);

void fileCacheSlotFlagsFromState(void);

void fileRestoreSlotFlagsToState(void);

extern s32 fileTestSlotFlagsBit(FileConfigOption option, u32 *flags);

u8 fileToggleSlotFlagsBit(FileConfigOption option, u32 *flags);

s32 fileTestSlotFlagsBit(FileConfigOption option, u32 *flags);

s32 fileTestSavedSlotFlags(FileConfigOption option);

extern FileConfigTask *GBWK;

extern struct MenuList *mnuCreateListState(s32, s32, s32);

extern MenuResourceWork *mnuRequestEffectResource(const char *, const char *);

extern u8 mnuHasEffectResourceHandle(MenuResourceWork *);

extern void mnuTitleStreamUpdateAndLogBgm(void);

extern s32 effRequestResourceByMode(const char *, const char *, s32, u32 *);

extern void fileDrawConfigurationMenuEntry(s32 x, s32 y, s32 depth,
                                           struct MenuList *list,
                                           FileConfigListNode *node,
                                           s32 drawArg);

extern char D_003BC8E0[];

/* Allocate the save/config task, its five-row list and its effect resource slots. */
FileConfigTask *fileCreateConfigTask(s32 mode);

extern void mnuReleaseEffectResource(MenuResourceWork *);

extern s32 mnuAdvanceTitleStateUnderSemaphore(void);

void fileConfigTaskDestroy(dds3ProcessID_t *task);

extern FileConfigTask *fileCreateConfigTask(s32 mode);

extern s32 fileUpdateConfigurationMenu(dds3ProcessID_t *task);

extern s32 fileStartQueuedLoad(dds3ProcessID_t *task);

extern s32 fileUpdateConfigOwnerTask(dds3ProcessID_t *task);

extern void fileConfigTaskDestroy(dds3ProcessID_t *task);

void dds3ConfigProcessStart(s32 mode);

void dds3ConfigProcessEnd(void);

s32 checkConfigState(void);

u32 fileGetConfigTaskSlot(s32 slot);

s32 fileUpdateConfigurationMenu(dds3ProcessID_t *task);

s32 fileStartQueuedLoad(dds3ProcessID_t *task);

s32 fileUpdateConfigOwnerTask(dds3ProcessID_t *task);

void fileDrawConfigurationMenuEntry(s32 x, s32 y, s32 depth, struct MenuList *list,
                   FileConfigListNode *node, s32 drawArg);

extern void mnuDrawMantraPulseAndSparks(void *resource, s32 alpha, s32 drawArg);

void fileDrawConfigurationMenuBackground(void *unused);

void fileResetRenderFlags(void);

void fileManagerResetSubsystems(void);

void fileSetRenderFlag(u32 bits);

void fileClearRenderFlag(u32 bits);

void fileResetRenderFlags(void);

sdfGrOt_t *effGetPacketSubmissionSurface(s32 index);

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_00324660[];

/* vu0 routine: project the point in vf10 through view/projection and screen scale/bias. */
void mnuProjectViewPoint(void);

/* vu0 routine: tests whether vf10 projects within the camera's forward cone. */
s32 sdfProjectPointInForwardConeVU(void);

extern s32 sdfProjectPointInForwardConeVU();

/* vu0 routine: distance between the projected view point and a second point offset perpendicular to
   the camera axis by rate; 0 when either projection (sdfProjectPointInForwardConeVU) fails. Point comes in vf10. */
f32 mnuMeasureProjectedPerpendicularDistance(f32 rate);

/* vu0 routine: point at t between p[1] and p[2] of a Catmull-Rom (Hermite, 0.5 tangents) spline, left in vf10 */
void vuCatmullRomPoint(f32 (*p)[4], f32 t);

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

extern u8 sdfHddValid;

extern char D_003BC928[];

extern char D_003BC930[];

extern char D_003BC938[];

extern char *sdfFileGetHostRoot(void);

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

void camAimRotation(CamAim *obj, void *dst);

/* Per-frame update: refreshes the queue rotation when an aim flag (0x60) is set, then repositions and re-notifies every job whose start time (job+0x80) has been reached. */
void fileQueueUpdate(FileQueue *queue);

void fileQueueDestroy(FileQueue *queue);

FileQueue *fileQueueClone(const FileQueue *source);

void fileQueueNotifyAllJobsComplete(FileQueue *queue);

/* Moves the queue to *vec and repositions every job: position = *vec + offset, plus each job's offset rotated by the queue quaternion (scaled when flag 0x80, y lowered by 5 for flag 0x04). */
void fileQueueSetPosition(FileQueue *queue, const f32 vec[4]);

/* Sets the queue rotation to *rot: stores it as queue->quat, rotates queue->axis into queue->offset, notifies each job with its quat multiplied by *rot, then repositions the jobs at queue->position. */
void fileQueueSetRotation(FileQueue *queue, const f32 rot[4]);

void fileQueueSetScale(FileQueue *queue, f32 scale);

/* Store the queue's modulation colour, then send every job its own colour
 * modulated by it ((job / 128) * (queue / 128) * 128 per channel). The tint
 * store is the unclobbered SDK spelling: as a bare volatile asm it flushes the
 * CSE table, so the loop's 1/128 is rematerialized rather than copied from the
 * first unpack's FPR. Reading the payload before the packed store keeps the
 * store from feeding the argument load. */
void fileQueueSetColor(FileQueue *queue, u32 color);

void fileQueueCopyRotationFromSource(FileQueue *queue, f32 matrix[4][4]);

FileJob *fileAppendJob(FileQueue *queue, u32 id);

FileJob *fileDuplicateAndAppendJob(FileQueue *queue, FileJobPayload *source);

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

LoadObj *fileLoadObjectCreate(void *owner);

/* Creates a load object sized by the record's cell count, capped at 0x12C. */
typedef struct FileCellGrid {
    u8 pad00[0x20];
    u32 cols;
    u32 rows;
    u8 pad28[0x90];
    u32 layers;
} FileCellGrid;

LoadObj *fileCreateGridLoaderRecord(FileCellGrid *grid);

void fileLoadObjectSetResource(LoadObj *obj, u32 type, void *data);

void fileLoadObjectOpenDevice(LoadObj *obj, void *name);

void fileLoadObjectOpenAndStartDevice(LoadObj *obj, void *name);

void fileLoadObjectOpenNamedDevice(LoadObj *obj, s32 resourceIndex);

void fileReplaceEffectSurfaceJobs(LoadObj *obj, FileJobPayload *job);

void fileReplaceReferenceHolder(LoadObj *obj, u32 resource);

LoadObj *effLoadObjectCreateFromJob(FileJobPayload *job);

void effLoadObjectDestroy(LoadObj *obj);

LoadObj *fileLoadObjectCreateChild(LoadObj *owner);

void fileCloneEffectSurfaceResources(LoadObj *dst, LoadObj *src);

void fileLoadObjectSetResource(LoadObj *obj, u32 type, void *data);

void fileLoadObjectOpenNamedDevice(LoadObj *obj, s32 resourceIndex);

void fileLoadObjectOpenDevice(LoadObj *obj, void *name);

void fileLoadObjectOpenAndStartDevice(LoadObj *obj, void *name);

void fileReplaceEffectSurfaceJobs(LoadObj *obj, FileJobPayload *job);

void fileReplaceReferenceHolder(LoadObj *obj, u32 resource);

void fileClearLoadObjectReferences(LoadObj *obj);

void fileAcquireLoadObjectRecord(LoadObj *obj);

#include "dds/eff_anim.h"

extern void dds3BillboardDraw(struct billHandle_t *);

extern void fileReadRecordSecondVector(FileSlotTable *, u128 *);

extern void fileReadVectorPtr20(FileSlotTable *, u128 *);

extern void effSubmitGeneratedTexturePacket(sdfGrOt_t *, EffGeneratedTextureDescriptor *);

void fileDrawLoadObjectSlots(s32 object);

void fileUpdateLoadObjectSlots(s32 object);

void fileSendLoadObjectRecordVector(LoadObj *obj, u128 *vector);

void fileCopyLoadObjectRecordVector(LoadObj *obj, u128 *vector);

void fileSetRecordWordFour(s32 record, u32 value);

void fileSetLoadObjectScale(LoadObj *obj, f32 scale);

void fileResetSlotStates(FileSlotTable *record);

/* vu0 routine: preserve the packed RGB snapshot while applying the alpha track. */
u32 effSampleColorAlphaTracks(const SdfColorTrack *colorTrack,
                              const SdfAlphaTrack *alphaTrack,
                              s32 frame, s32 duration);

f32 effSampleScalarCurve(const EffScalarCurve *curve, s32 frame, s32 duration);

/* vu0 routine: samples the colour, scale and heading tracks at frame; in mode 2 the heading is the screen-space direction from out->pos to target */
void fileSampleKeyTracks(FileSlot *out, FileKeyBlock *block, s32 frame, f32 *target);

/* Invalidates the cells x rows slots of the group that starts at `slot`. */
void fileInvalidateSlotGroup(FileSlotTable *group, FileSlot *slot);

/* Same, but first copies the slot at `slot` over the group's first slot. */
void fileCopyAndInvalidateSlotGroup(FileSlotTable *group, FileSlot *slot);

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

/* vu0 routine: advances a ring in its sampled orientation with vertical acceleration. */
void effUpdateOrientedRingEmitterSlots(FileSlotTable *record);

/* Scale selected oriented-ring parameters; keep the destination-first declarations. */
void effScaleOrientedRingEmitterParameters(ScaleOwner *owner, f32 scale);

/* vu0 routine: advances directed slots through the emitter quaternion frame. */
void effUpdateDirectedEmitterSlots(FileSlotTable *record);

/* Scale selected directed-emitter parameters from source to destination. */
void effScaleDirectedEmitterParameters(ScaleOwner *owner, f32 scale);

FileSlotTable *fileAllocateGridRecordSlots(u16 type, u32 count, const void *data);

void effPCPHelperBEDGetModel(FileSlotTable *record);

void fileClearRecordReferences(FileSlotTable *record);

void fileAcquireRecord(FileSlotTable *record);

void fileReadVectorPtr20(FileSlotTable *record, u128 *out);

void mnuRecordSetVector(FileSlotTable *record, const void *src);

void fileReadRecordSecondVector(FileSlotTable *record, u128 *out);

void fileSetRecordSecondVector(FileSlotTable *record, const void *src);
/* End of declarations from src/dds1/game/code_0028A150.c */

INCLUDE_ASM(const s32, "code_001EB5E0", func_001EB5E0);

INCLUDE_ASM(const s32, "code_001EB5E0", func_001EB618);

INCLUDE_ASM(const s32, "code_001EB5E0", func_001EB620);

u8 fileIsLoadedAndConditionTrue(s32 condition) {
    return condition != 0 && D_003BC7FC == 1;
}

u8 fileIsLoadedWithActiveFlow(s32 loaded) {
    return loaded != 0 && D_003BC7FC == 1;
}
