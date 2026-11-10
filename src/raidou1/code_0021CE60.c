#include "common.h"

/* Declarations from smt3-decomp src/smt3/kernel/dds3SequenceList.c */
#include "common.h"

#include "kernel.h"

#include "scr.h"

#include "event.h"

#include "field.h"

extern u32 abcd;

extern u32 dds3KernelDebugSign;

extern s32 ChkPipe(void);

extern void EndPipe(void);

extern void FinishMovieViewer(void);

extern void InitMovieViewer(void);

extern s32 campCheckExitState(void);

extern void campDebugRegistAll(void);

extern void campKillProcesses(void);

extern s32 checkConfigState(void);

extern s32 checkResultStat(void);

extern void cmpProcessStart(void);

extern void dds3ConfigProcessEnd(void);

extern void dds3ConfigProcessStart(s32 Mode);

extern void evtChkEndProcess(void);

extern void evtExitSky(void);

extern void evtFreeStage(void);

extern void evtInitEventTest(void);

extern void evtInitEventViewer(void);

extern void fclCombineProcessEnd(void);

extern void fclCombineProcessStart(void);

extern void fclRagProcessEnd(void);

extern void fclRecoverProcessEnd(void);

extern void fclRecoverProcessStart(s32 Place);

extern void fclShopProcessEnd(void);

extern void fclShopProcessStart(void *pPlace);

extern void fclTerminalProcessEnd(void);

extern void fldProc_CallSisetuReturn(void);

extern void fmProcTrigSequence(s32 mode);

extern s32 fmProcessChk(void);

extern void frTestFontInit(void);

extern void func_0022C658(void);

extern void itfDebugMesMngInitialize(void);

extern s32 mselExit(u32 mode);

extern void mselInit(void);

extern void nbExitBattleTest(void);

extern s32 nmeCheckProcessStat(void);

extern void nmeNameProcessEnd(void);

extern s32 ragCheckProcessStat(void);

extern s32 rcvCheckProcessStat(void);

extern void rstResult2ProcessEnd(void);

extern void rstResult2ProcessStart(void);

extern void rstSetTestParam(void);

extern s32 shpCheckProcessStat(void);

extern void stfExit(void);

extern void stfInit(void);

extern void titleExit(void);

extern void titleStart(u32 mode);

extern s32 trmCheckProcessStat(void);

#include "sdk_libc.h"

extern const char dds3CallEventReturnMsg[];

extern void fldProcTrigSequence(void *srcParam, u32 mode);

extern void nmeStartProcesses(s32 arg0, s32 arg1, s32 arg2);

extern void fclRagProcessStart(s32 Place);

extern void pbStartMapEditor(s32 mode, s32 stage);

extern void evtKillEventViewer(void);

extern s32 cmbCheckProcessStat(void);

extern void InitPipe(s32 data, s32 prevData);

extern void dds3AdminiExitSequence(void);

extern void nbInitBattleTest(void);

extern void nbCallNewBattle(s32 encpack, s32 data);

extern s32 evtLoadArea(s32 fld, s32 area);

extern s32 evtLoadUnit(s32 major, s32 minor);

extern evtUnit_t *evtGetUnitHandle(s32 serial);

extern void evtSetUnitMotion(evtUnit_t *h, s32 motion, s32 before, s32 smooth, s32 mode);

extern void mdlSmoothLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber, f32 aBeforeLeng,
                                f32 aSmoothLeng);

extern s32 evtNextEventNo;

extern void *sdfChipAllocDirect(u32 aSize);

extern s32 itfMesMngInitialize(void *sub);

extern scrProcessWork_s *scrHandleFirst;

extern scrProcessWork_s *scrHandleLast;

extern s32 scrHandleCount;

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);

extern dds3ProcessID_t *dds3SearchProcessPriority(u32 priority);

extern s32 dds3StartMenuProcess(dds3ProcessID_t *id);

extern void func_00157050(void);

extern void func_00157058(void);

extern s32 fmShutDown2(void);

extern void fldMainLoadStart(void);

extern void fldAllShutDown(void);

extern s32 fldProcShutDown2(void);

extern void fldProcNextJump(void);

extern dds3ProcessID_t *evtInitSky(void);

extern void fclTerminalProcessStart(s32 TerminalType, s32 TerminalNo);

extern scrProcessWork_s *scrPrintProcedureStart(scrHeader_t *header, scrTypeTable_t *typetable,
                                                scrProcTable_t *proctable,
                                                scrLabelTable_t *labeltable, scrCode_u *code,
                                                u8 *mesdata, char *strdata, s32 procno);

dds3ProcessID_t *CallDebug(void);

s32 ExitDebug(void);

s32 CheckDebug(void);

void CallBootMc(void);

s32 ExitBootMc(void);

s32 CheckBootMc(void);

void CallTitle(u32 flg, u32 *data);

s32 ExitTitle(void);

s32 CheckTitle(void);

void CallLoad(void);

s32 ExitLoad(void);

s32 CheckLoad_dds3SequenceList(void);

void CallLoad2(void);

s32 ExitLoad2(void);

s32 CheckLoad2(void);

void CallStaff(void);

s32 ExitStaff(void);

s32 CheckStaff(void);

void CallEndSave(void);

s32 ExitEndSave(void);

s32 CheckEndSave(void);

void CallField(u32 flg, void *data);

s32 ExitField(void);

s32 CheckField(void);

void CallNewBattle(void);

s32 ExitNewBattle(void);

s32 CheckNewBattle(void);

void CallEvent(u32 flg, s32 *data);

extern const char dds3CallEventReturnMsg[16];

s32 ExitEvent(void);

s32 CheckEvent(void);

void CallCamp(void);

s32 ExitCamp(void);

s32 CheckCamp(void);

void CallCombine(void);

s32 ExitCombine(void);

s32 CheckCombine(void);

void CallShop(void);

s32 ExitShop(void);

s32 CheckShop(void);

void CallRag(void);

s32 ExitRag(void);

s32 CheckRag(void);

void CallRecover(void);

s32 ExitRecover(void);

s32 CheckRecover(void);

void CallTerminal(void);

s32 ExitTerminal(void);

s32 CheckTerminal(void);

void CallResult2(void);

s32 ExitResult2(void);

s32 CheckResult2(void);

void CallConfig(void);

s32 ExitConfig(void);

s32 CheckConfig(void);

void CallNewBattleTest(u32 flg, s32 *data);

s32 ExitNewBattleTest(void);

s32 CheckNewBattleTest(void);

void CallBattleFieldTest(void);

s32 ExitBattleFieldTest(void);

s32 CheckBattleFieldTest(void);

void CallKernelTest(void);

s32 ExitKernelTest(void);

s32 CheckKernelTest(void);

void CallEventTest(void);

s32 ExitEventTest(void);

s32 CheckEventTest(void);

void CallEventViewer(void);

s32 ExitEventViewer(void);

s32 CheckEventViewer(void);

void CallInterfaceTest(void);

s32 ExitInterfaceTest(void);

s32 CheckInterfaceTest(void);

void CallMessageScriptTest(void);

s32 ExitMessageScriptTest(void);

s32 CheckMessageScriptTest(void);

void CallCampTest(void);

s32 ExitCampTest(void);

s32 CheckCampTest(void);

void CallCombineTest(void);

s32 ExitCombineTest(void);

s32 CheckCombineTest(void);

void CallShopTest(s32 flg, void *data);

s32 ExitShopTest(void);

s32 CheckShopTest(void);

void CallRagTest(void);

s32 ExitRagTest(void);

s32 CheckRagTest(void);

void CallRecoverTest(void);

s32 ExitRecoverTest(void);

s32 CheckRecoverTest(void);

void CallTerminalTest(dds3ProcessID_t *flg, s32 *data);

s32 ExitTerminalTest(void);

s32 CheckTerminalTest(void);

void CallResult2Test(void);

s32 ExitResult2Test(void);

s32 CheckResult2Test(void);

void CallNameTest(void);

s32 ExitNameTest(void);

s32 CheckNameTest(void);

void CallModelViewerTest(void);

s32 ExitModelViewerTest(void);

s32 CheckModelViewerTest(void);

void CallSoundTest(void);

s32 ExitSoundTest(void);

s32 CheckSoundTest(void);

void CallBillboardTest(void);

s32 ExitBillboardTest(void);

s32 CheckBillboardTest(void);

void CallFontTest(void);

s32 ExitFontTest(void);

s32 CheckFontTest(void);

void CallMapEditor(void);

s32 ExitMapEditor(void);

s32 CheckMapEditor(void);

void CallPuzzleBoy(u32 flg, s32 *data);

s32 ExitPuzzleBoy(void);

s32 CheckPuzzleBoy(void);

void CallMovieViewerTest(void);

s32 ExitMovieViewerTest(void);

s32 CheckMovieViewerTest(void);

void CallPipe(u32 flg, s32 *data);

s32 ExitPipe(void);

s32 CheckPipe(void);

void CallModeSelect(u32 flg, u32 *data);

s32 ExitModeSelect(void);

s32 CheckModeSelect(void);

void CallTgs(void);

s32 ExitTgs(void);

s32 CheckTgs(void);

s32 dat_1097D8(void);

void func_001097E0(void);

void datDebugPrintf(const char *format, ...);

s32 scr_109830(void);

void func_00109838(void);

void func_00109840(void);

void scrLinkHandle(scrProcessWork_s *work);

void scrUnlinkHandle(scrProcessWork_s *work);

scrProcessWork_s *scrPrintProcedureStart(scrHeader_t *header, scrTypeTable_t *typetable,
                                         scrProcTable_t *proctable, scrLabelTable_t *labeltable,
                                         scrCode_u *code, u8 *mesdata, char *strdata, s32 procno);

scrProcessWork_s *fldFlowParse(scrHeader_t *header, s32 procno);

extern u32 abcd;

extern s32 scrHandleCount;

extern scrProcessWork_s *scrHandleFirst;

extern scrProcessWork_s *scrHandleLast;

extern u32 D_0054E9A0[2];
/* End of declarations from src/smt3/kernel/dds3SequenceList.c */

/* Declarations from dds-decomp src/dds2/game/code_002C96D0.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "common.h"

#include "fileManager.h"

#include "newdata.h"

#include "sdk_libkernl.h"

#include "dds/bill_object_api.h"

#include "dds/btl.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/dds3_fade.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3_world_primary.h"

#include "dds/eff_random.h"

#include "dds/evt_world.h"

#include "dds/file_request_api.h"

#include "dds/file_request_entry.h"

#include "dds/file_slot_flags.h"

#include "dds/fr_font.h"

#include "dds/fr_font_context.h"

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

extern char D_00437E10[];

extern char D_00437E18[];

extern char D_00437E20[];

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

extern u8 D_00437E28[];

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

void fileJobFreeSecondaryBuffer(FileJobPayload *job) {
    struct sdfMemHandle_t *allocation = job->secondary.allocation;
    if (allocation != NULL) {
        sdfMemFreeDirect(allocation);
        FILE_JOB_LOCATION_CLEAR(job->secondary);
        job->secondary.size = 0;
        job->secondary.allocation = NULL;
    }
}

FileJobPayload *fileJobCreateChild(FileJobPayload *request) {
    FileJobPayload *job = fileCreateJob(request->type);
    FileTypeCallbacks *cb = &fileJobTypeOperations[job->type];

    job->option = request->option;
    job->primary.selector = request->primary.selector;
    job->data = cb->createChild(request->data, job->type);
    return job;
}

void fileJobNotifyPair(FileJobPayload *left, FileJobPayload *right) {
    void (*cb)(void *, void *) = fileJobTypeOperations[right->type].notifyPair;

    if (cb != NULL) {
        cb(left->data, right->data);
    }
}

void fileJobNotifyComplete(FileJobPayload *job) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].reset != NULL) {
        fileJobTypeOperations[id].reset(job->data);
    }
}

void fileJobInvokeTypeCallback(FileJobPayload *job) {
    u16 idx = job->type;
    void *data = job->data;

    fileJobTypeOperations[idx].invoke(data);
}

void fileJobInvokePositionCallback(FileJobPayload *job, const f32 position[4]) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setPosition != NULL) {
        fileJobTypeOperations[id].setPosition(job->data, position);
    }
}

void fileJobInvokeRotationCallback(FileJobPayload *job, const f32 rotation[4]) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setRotation != NULL) {
        fileJobTypeOperations[id].setRotation(job->data, rotation);
    }
}

void fileJobInvokeScaleCallback(FileJobPayload *job, f32 scale) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setScale != NULL) {
        fileJobTypeOperations[id].setScale(job->data, scale);
    }
}

void fileDispatchJobTypeCallback(FileJobPayload *job, u32 color) {
    u16 id = job->type;

    if (fileJobTypeOperations[id].setColor != NULL) {
        fileJobTypeOperations[id].setColor(job->data, color);
    }
}

void fileJobSetPrimaryData(FileJobPayload *job, const void *src, s32 size, u16 option) {
    fileJobFreePrimaryBuffer(job);
    if (src != NULL && size > 0) {
        job->primary.allocation = sdfMemAllocDirect(size);
        FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->primary) = sdfMemLock(job->primary.allocation);
        job->primary.size = size;
        job->option = option;
        memcpy((void *)FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->primary), src, size);
    }
}

void fileJobCopyCommandIntoPrimaryData(FileJobPayload *job, const char *commandPath, u16 option) {
    sdfFileCmdHandle_t *command;
    s32 size;
    struct sdfMemHandle_t *allocation;
    void *buffer;

    command = sdfFileOpen(commandPath);
    if (command != 0) {
        size = sdfFileLength(command);
        allocation = sdfMemAllocDirect(size);
        buffer = (void *)sdfMemLock(allocation);
        sdfFileRead(command, buffer, size);
        sdfFileClose(command);
        fileJobSetPrimaryData(job, buffer, size, option);
        sdfMemFreeDirect(allocation);
        return;
    }
}

void fileJobSetSecondaryData(FileJobPayload *job, const void *src, s32 size, u16 selector) {
    fileJobFreeSecondaryBuffer(job);
    if (src != NULL && size > 0) {
        job->secondary.allocation = sdfMemAllocDirect(size);
        FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->secondary) = sdfMemLock(job->secondary.allocation);
        job->secondary.size = size;
        job->primary.selector = selector;
        memcpy((void *)FILE_JOB_LOCATION_AS_LOCKED_ADDRESS_WORD(job->secondary), src, size);
    }
}

void fileJobCopyCommandIntoSecondaryData(FileJobPayload *job, const char *commandPath, u16 selector) {
    sdfFileCmdHandle_t *command;
    s32 size;
    struct sdfMemHandle_t *allocation;
    void *buffer;

    command = sdfFileOpen(commandPath);
    if (command != 0) {
        size = sdfFileLength(command);
        allocation = sdfMemAllocDirect(size);
        buffer = (void *)sdfMemLock(allocation);
        sdfFileRead(command, buffer, size);
        sdfFileClose(command);
        fileJobSetSecondaryData(job, buffer, size, selector);
        sdfMemFreeDirect(allocation);
        return;
    }
}

INCLUDE_ASM(const s32, "code_0021CE60", fileJobWriteSerializedPayload);

void fileWriteToPfs(FileJobPayload *job, const char *filePath) {
    char path[0xD0];
    s32 fd;

    if (sdfHddValid != 0) {
        sprintf(path, D_00437E10, filePath);
        fd = sceOpen(path, 0x602, 0x1B6);
    } else {
        sprintf(path, D_00437E18, sdfFileGetHostRoot(), filePath);
        fd = sceOpen(path, 0x602);
    }
    fileJobWriteSerializedPayload(fd, job);
    sceClose(fd);
    sceSync(D_00437E20, 0);
}

INCLUDE_ASM(const s32, "code_0021CE60", fileDuplicateJob);

/* NULL when the command file does not open: retail's v0 is sdfFileOpen's
 * null result there. */
FileJobPayload *fileJobCreateFromCommandState(const char *entry) {
    sdfFileCmdHandle_t *command;
    s32 size;
    struct sdfMemHandle_t *allocation;
    void *buffer;
    FileJobPayload *job;

    command = sdfFileOpen(entry);
    if (command != 0) {
        size = sdfFileLength(command);
        allocation = sdfMemAllocDirect(size);
        buffer = (void *)sdfMemLock(allocation);
        sdfFileRead(command, buffer, size);
        sdfFileClose(command);
        job = fileDuplicateJob((FileJobPayload *)buffer);
        sdfMemFreeDirect(allocation);
        return job;
    }
    return NULL;
}

u32 fileJobSerializedSize(FileJobPayload *job) {
    u32 size = 0x2C;

    if (fileResolvePrimaryBuffer(job) != NULL) {
        size = job->primary.size + 0x2C;
    }
    if (fileResolveSecondaryBuffer(job) != NULL) {
        u32 aligned = size >> 4;
        if ((size & 0xF) != 0) {
            aligned = (aligned + 1) << 4;
        } else {
            aligned = aligned << 4;
        }
        size = aligned + job->secondary.size;
    }
    return size;
}

INCLUDE_ASM(const s32, "code_0021CE60", func_0021D798);

INCLUDE_ASM(const s32, "code_0021CE60", func_0021D7F0);

void fileQueueAppend(FileQueue *queue, FileJob *job) {
    job->next = NULL;
    if (queue->last != NULL) {
        queue->last->next = job;
        job->prev = queue->last;
    } else {
        queue->first = job;
        job->prev = NULL;
    }
    queue->last = job;
    queue->count++;
}

void fileQueueInsertAfter(FileQueue *queue, FileJob *after, FileJob *job) {
    if (after->next != NULL) {
        after->next->prev = job;
        job->next = after->next;
    } else {
        job->next = NULL;
        queue->last = job;
    }
    after->next = job;
    job->prev = after;
    queue->count++;
}

void fileQueueRemove(FileQueue *queue, FileJob *job) {
    if (job->prev != NULL) {
        job->prev->next = job->next;
    } else {
        queue->first = job->next;
    }
    if (job->next != NULL) {
        job->next->prev = job->prev;
    } else {
        queue->last = job->prev;
    }
    queue->count--;
}

INCLUDE_ASM(const s32, "code_0021CE60", func_0021D8E8);

INCLUDE_ASM(const s32, "code_0021CE60", func_0021D920);

void fileDestroyJob(FileJob *job) {
    sdfChipFreeDirect(job);
}

INCLUDE_ASM(const s32, "code_0021CE60", fileCloneQueueEntries);

void CallShopTest(s32 flg, void *data)
{
    fclShopProcessStart(data);
}

INCLUDE_ASM(const s32, "code_0021CE60", func_0021DBD0);

/* A queued job's +0x90 word is a child pointer when flag 1 is clear; it is
 * an ID for other job kinds. Destroy children before their parent jobs. */
void fileQueueDestroy(FileQueue *queue) {
    FileJob *job = queue->first;
    while (job != NULL) {
        FileJob *next = job->next;
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) == 0) {
            fileJobDestroy((FileJobPayload *)job->id);
        }
        fileDestroyJob(job);
        job = next;
    }
    sdfChipFreeDirect(queue);
}

FileQueue *fileQueueClone(const FileQueue *source) {
    FileQueue *queue = fileQueueCreate();
    const FileJob *src;
    f32 vec[4] __attribute__((aligned(16)));

    SDF_VU0_COPYVECTOR(queue, source);
    SDF_VU0_COPYVECTOR(queue->axis, source->axis);
    queue->scaleMultiplier = source->scaleMultiplier;
    queue->transformFlags = source->transformFlags;
    for (src = source->first; src != NULL; src = src->next) {
        FileJob *job = fileJobCreate();
        job->id = (u32)fileJobCreateChild((FileJobPayload *)src->id);
        fileJobCopyHeader(job, src);
        fileQueueAppend(queue, job);
    }
    VU0_STORE_VF(vf0, vec);
    fileQueueSetPosition(queue, vec);
    fileQueueSetRotation(queue, vec);
    fileQueueSetScale(queue, 1.0f);
    fileQueueSetColor(queue, 0x80808080);
    return queue;
}

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueNotifyAllJobsComplete);

void fileQueueSetPosition(FileQueue *queue, const f32 vec[4])
{
    f32 rot[16];
    f32 base[4];
    f32 pos[4];
    FileJob *job;
    f32 scale;

    SDF_VU0_LOADVECTOR(vec);
    SDF_VU0_STOREVECTOR(queue->position);
    SDF_VU0_LOADVECTOR_B(queue->offset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(base);
    SDF_VU0_LOADVECTOR(queue->quat);
    ToMatrix();
    VU0_STORE_MATRIX(rot);
    scale = queue->scale * queue->scaleMultiplier;
    for (job = queue->first; job != NULL; job = job->next) {
        SDF_VU0_LOADVECTOR(base);
        if (job->xformFlags & 4) {
            VU0_SCALAR_OP(-5.0f, "vaddx.y vf10, vf0, vf2x");
        }
        SDF_VU0_LOADVECTOR_B(job->offset);
        if (job->xformFlags & 0x80) {
            VU0_SCALE_VF(vf11, scale);
        }
        VU0_LOAD_MATRIX(rot);
        VU0_ROTATE_VEC(vf11, vf11);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        fileJobInvokePositionCallback((FileJobPayload *)job->id, pos);
    }
}

void fileQueueSetRotation(FileQueue *queue, const f32 rot[4])
{
    f32 quat[4];
    f32 pos[4];
    FileJob *job;

    SDF_VU0_LOADVECTOR(rot);
    SDF_VU0_STOREVECTOR(queue->quat);
    SDF_VU0_STOREVECTOR(quat);
    ToMatrix();
    SDF_VU0_LOADVECTOR(queue->axis);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(queue->offset);
    for (job = queue->first; job != NULL; job = job->next) {
        SDF_VU0_LOADVECTOR(job->quat);
        SDF_VU0_LOADVECTOR_B(quat);
        Mult();
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        fileJobInvokeRotationCallback((FileJobPayload *)job->id, pos);
    }
    fileQueueSetPosition(queue, queue->position);
}

void fileQueueSetScale(FileQueue *queue, f32 scale)
{
    s128 pos;
    FileJob *job;
    f32 total;
    f32 jobScale;

    queue->scale = scale;
    total = scale * queue->scaleMultiplier;
    for (job = queue->first; job != NULL; job = job->next) {
        jobScale = job->scale;
        if (job->updateFlags & FILE_JOB_UPDATE_FLAG_APPLY_QUEUE_SCALE) {
            jobScale = jobScale * total;
        }
        fileJobInvokeScaleCallback((FileJobPayload *)job->id, jobScale);
        if (job->xformFlags & 0x80) {
            SDF_VU0_LOADVECTOR(queue->position);
            SDF_VU0_LOADVECTOR_B(queue->offset);
            SDF_VU0_ADDVECTOR();
            if (job->xformFlags & 4) {
                VU0_SCALAR_OP(-5.0f, "vaddx.y vf10, vf0, vf2x");
            }
            SDF_VU0_LOADVECTOR_B(job->offset);
            VU0_SCALAR_OP(total, "vmulx.xyzw vf11, vf11, vf2x");
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(&pos);
            fileJobInvokePositionCallback((FileJobPayload *)job->id, (f32 *)&pos);
        }
    }
}

/* Store the queue's modulation colour, then send every job its own colour
 * modulated by it ((job / 128) * (queue / 128) * 128 per channel). The tint
 * store is the unclobbered SDK spelling: as a bare volatile asm it flushes the
 * CSE table, so the loop's 1/128 is rematerialized rather than copied from the
 * first unpack's FPR. Reading the payload before the packed store keeps the
 * store from feeding the argument load. */
void fileQueueSetColor(FileQueue *queue, u32 color) {
    f32 tint[4] __attribute__((aligned(16)));
    u32 queueColor[4] __attribute__((aligned(16)));
    u32 jobColor[4] __attribute__((aligned(16)));
    u32 packed[4] __attribute__((aligned(16)));
    FileJob *job;

    queue->color = color;
    queueColor[0] = color;
    EE_MMI_RGBA_UNPACK(queueColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(tint);
    for (job = queue->first; job != NULL; job = job->next) {
        u32 result;
        FileJobPayload *payload;

        jobColor[0] = job->color;
        EE_MMI_RGBA_UNPACK(jobColor, 1.0f / 128.0f);
        SDF_VU0_LOADVECTOR_B(tint);
        SDF_VU0_MULVECTOR();
        EE_MMI_RGBA_PACK_UNIT(result, 0x43000000);
        payload = (FileJobPayload *)job->id;
        packed[0] = result;
        fileDispatchJobTypeCallback(payload, result);
    }
}

void fileQueueCopyRotationFromSource(FileQueue *queue, f32 matrix[4][4]) {
    f32 vec[4] __attribute__((aligned(16)));
    sdfVuMatrixToQuaternion(matrix);
    SDF_VU0_STOREVECTOR(vec);
    fileQueueSetRotation(queue, vec);
}

FileJob *fileAppendJob(FileQueue *queue, u32 id) {
    FileJob *job = fileJobCreate();

    job->id = id;
    fileQueueAppend(queue, job);
    return job;
}

FileJob *fileDuplicateAndAppendJob(FileQueue *queue, FileJobPayload *source) {
    FileJobPayload *job = fileDuplicateJob(source);
    return fileAppendJob(queue, (u32)job);
}

FileJob *fileAppendJobFromCommandPath(FileQueue *queue, const char *commandPath) {
    printf(D_00437E28);
    return fileAppendJob(queue, (u32)fileJobCreateFromCommandState(commandPath));
}

FileJob *fileJobDuplicateAfter(FileQueue *queue, FileJob *src) {
    FileJob *job = fileJobCreate();

    memcpy(job, src, 0xC0);
    fileJobResetAndInitTransform(job);
    job->flags |= FILE_JOB_FLAG_SHARED_PAYLOAD;
    job->id = src->id;
    fileQueueInsertAfter(queue, src, job);
    return job;
}

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueLinkJobToSectorLeader);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueDetachSectorFollower);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueRemoveAndDestroyJob);

INCLUDE_ASM(const s32, "code_0021CE60", func_0021E800);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueSaveImage);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueSaveVersionedImage);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueCreateFromCommandState);

FileJob *fileQueueFindById(FileQueue *queue, u32 id) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if (job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueFindFlaggedById(FileQueue *queue, u32 id) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if ((job->flags & FILE_JOB_FLAG_SHARED_PAYLOAD) != 0 && job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if ((job->flags & (FILE_JOB_FLAG_SHARED_PAYLOAD | FILE_JOB_FLAG_SECTOR_FOLLOWER)) ==
            FILE_JOB_FLAG_SECTOR_FOLLOWER && job->sector == sector) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueGetAt(FileQueue *queue, s32 index) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if (index-- == 0) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

s32 fileFindQueuedJobIndex(FileQueue *queue, FileJob *target) {
    FileJob *job = queue->first;
    s32 index = 0;
    while (job != NULL) {
        if (job == target) {
            return index;
        }
        job = job->next;
        index++;
    }
    return 0;
}

s32 fileQueueCountLinkedJobs(FileQueue *queue) {
    FileJob *job;
    s32 count;

    count = 0;
    for (job = queue->first; job != 0; job = job->next) {
        count = count + 1;
    }
    return count;
}

/* vu0 routine: dst = |pos| * unit(aViewEye - aViewTarget); flag 0x10 flattens y, then puts pos.y back */
void camFollowOffsetVec(FileJob *obj, void *dst)
{
    f32 len[4];
    f32 length;
    u32 flags = obj->xformFlags;

    if ((flags & 0x18) == 0) {
        SDF_VU0_COPYVECTOR(dst, obj->offset);
        return;
    }
    SDF_VU0_LOADVECTOR(obj->offset);
    if (obj->xformFlags & 0x10) {
        VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
    }
    SDF_VU0_GETLENGTH(length);
    VEC3_SPLAT(len, length);
    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    if (obj->xformFlags & 0x10) {
        VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
    }
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(len);
    SDF_VU0_MULVECTOR();
    if (obj->xformFlags & 0x10) {
        VU0_SCALAR_OP(obj->offset[1], "vaddx.y vf10, vf0, vf2x");
    }
    SDF_VU0_STOREVECTOR(dst);
}

/* dst = rotation facing unit(aViewEye - aViewTarget) (pitch and yaw); flag 0x40 composes it onto obj->quat, no flag 0x60 copies obj->quat */
void camAimRotation(CamAim *obj, void *dst)
{
    f32 v[4];
    f32 pitch;
    u32 flags = obj->flags;

    if ((flags & 0x60) == 0) {
        SDF_VU0_COPYVECTOR(dst, obj->quat);
        return;
    }
    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(v);
    pitch = 0.0f;
    if ((flags & 0x40) == 0) {
        pitch = -acosf(-v[1]);
    }
    FromEuler(pitch, atan2f(v[0], v[2]), 0.0f);
    if (obj->flags & 0x40) {
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(obj->quat);
        Mult();
    }
    SDF_VU0_STOREVECTOR(dst);
}

EffectSurfaceNode *fileCreateSurfaceLoaderState(s32 capacity) {
    EffectSurfaceNode *rec = (EffectSurfaceNode *)sdfChipAllocZeroDirect(sizeof(EffectSurfaceNode));
    u32 color = 0x80808080;

    rec->handleCount = capacity;
    rec->active = 1;
    rec->color = color;
    rec->opacity = 1.0f;
    rec->resource = NULL;
    rec->jobs = NULL;
    rec->jobAllocation = 0;
    rec->queues = NULL;
    rec->queueAllocation = 0;
    rec->record = 0;
    return rec;
}

EffectSurfaceNode *fileCreateGridLoaderRecord(FileKeyBlock *hdr) {
    u32 rows = hdr->emissionDuration;
    u32 count = (rows != 0 ? rows : hdr->spawnRate) * (rows != 0 ? hdr->spawnRate : hdr->length);

    return fileCreateSurfaceLoaderState(count <= 0x12C ? count : 0x12C);
}

INCLUDE_ASM(const s32, "code_0021CE60", func_0021F848);

INCLUDE_ASM(const s32, "code_0021CE60", fileDestroyEffectSurfaceAndChildren);

EffectSurfaceNode *fileLoadObjectCreateChild(EffectSurfaceNode *owner) {
    FileKeyBlock *source = (FileKeyBlock *)owner->record->data1;
    EffectSurfaceNode *result = fileCreateGridLoaderRecord(source);

    fileLoadObjectSetResource(result, owner->record->type, source);
    fileCloneEffectSurfaceResources(result, owner);
    return result;
}

INCLUDE_ASM(const s32, "code_0021CE60", func_0021FAC8);

void fileLoadObjectSetResource(EffectSurfaceNode *node, u32 entryId, void *resource) {
    if (node->record != 0) {
        effPCPHelperBEDGetModel(node->record);
    }
    node->record = fileAllocateGridRecordSlots((u16)entryId, node->handleCount, resource);
}

void fileLoadObjectOpenNamedDevice(EffectSurfaceNode *node, u32 resourceId) {
    struct billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardGeneral(resourceId);
    node->resource = resource;
    if (node->record != 0) {
        dds3BillboardSetMode(resource, (s16)((FileKeyBlock *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void fileLoadObjectOpenDevice(EffectSurfaceNode *node, u32 resourceId) {
    struct billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardCreate(0, resourceId);
    node->resource = resource;
    if (node->record != 0) {
        dds3BillboardSetMode(resource, (s16)((FileKeyBlock *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void fileLoadObjectOpenAndStartDevice(EffectSurfaceNode *node, u32 resourceId) {
    struct billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardCreate(1, resourceId);
    node->resource = resource;
    dds3BillboardSetAnimColorAdvMode(resource);
    if (node->record != 0) {
        dds3BillboardSetMode(node->resource, (s16)((FileKeyBlock *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

INCLUDE_ASM(const s32, "code_0021CE60", fileReplaceEffectSurfaceJobs);

INCLUDE_ASM(const s32, "code_0021CE60", fileReplaceEffectSurfaceQueues);

void fileReplaceReferenceHolder(EffectSurfaceNode *obj, u32 resource) {
    if (obj->resourceHolder != NULL) {
        effReleaseReferenceHolder(obj->resourceHolder);
    }
    obj->resourceHolder = effCreateTextureAnimationSet(resource);
}

void fileClearLoadObjectReferences(EffectSurfaceNode *obj) {
    if (obj->record != 0) {
        fileClearRecordReferences(obj->record);
        return;
    }
}

INCLUDE_ASM(const s32, "code_0021CE60", func_00220170);

INCLUDE_ASM(const s32, "code_0021CE60", func_002201A0);

INCLUDE_ASM(const s32, "code_0021CE60", func_00220B40);

void fileSendLoadObjectRecordVector(EffectSurfaceNode *obj, const u128 *value) {
    mnuRecordSetVector(obj->record, value);
}

void fileCopyLoadObjectRecordVector(EffectSurfaceNode *obj, const u128 *value) {
    fileSetRecordSecondVector(obj->record, value);
}

void fileSetRecordWordFour(s32 record, u32 value) {
    *(u32 *)(record + 4) = value;
}

void fileSetLoadObjectScale(EffectSurfaceNode *obj, f32 scale) {
    obj->opacity = scale;
    dds3DispatchIndexedCallback(obj->record, scale);
}

void fileResetSlotStates(FileSlotTable *table) {
    u32 count;
    FileSlot *slot;
    u32 index;

    count = table->count;
    index = 0;
    slot = table->slots;
    if (count != 0) {
        do {
            index = index + 1;
            FILE_SLOT_FRAME(slot) = FILE_SLOT_FRAME_NONE;
            slot++;
        } while (index < count);
    }
}

/* vu0 routine: preserve the packed RGB snapshot while applying the alpha track. */
u32 effSampleColorAlphaTracks(const SdfColorTrack *colorTrack,
                              const SdfAlphaTrack *alphaTrack,
                              s32 frame, s32 duration) {
    u32 color0;
    u32 color1;
    u32 alpha;
    u32 packedColor;
    u32 unitBits;
    f32 durationFloat;
    f32 factor;
    f32 inverseFactor;
    f32 alphaFactor;
    f32 alphaValue;
    s32 firstFrame;
    s32 secondFrame;
    s32 fadeInFrame;
    s32 fadeOutFrame;

    if (duration == 0) {
        return (colorTrack->initialColor & 0x00FFFFFF) |
               (alphaTrack->alpha << 24);
    }

    durationFloat = (f32)duration;
    switch (colorTrack->segmentMode) {
    case SDF_COLOR_TRACK_MODE_ENDPOINTS:
        factor = (f32)frame / durationFloat;
        color1 = colorTrack->initialColor;
        color0 = colorTrack->finalColor;
        break;
    case SDF_COLOR_TRACK_MODE_ONE_INTERMEDIATE_KEY:
        firstFrame = (s32)(colorTrack->keys[0].fraction * durationFloat);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            color1 = colorTrack->initialColor;
            color0 = colorTrack->keys[0].color;
        } else {
            f32 span = duration - firstFrame;
            factor = (f32)(frame - firstFrame) / span;
            color1 = colorTrack->keys[0].color;
            color0 = colorTrack->finalColor;
        }
        break;
    case SDF_COLOR_TRACK_MODE_TWO_INTERMEDIATE_KEYS:
        firstFrame = (s32)(colorTrack->keys[0].fraction * durationFloat);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            color1 = colorTrack->initialColor;
            color0 = colorTrack->keys[0].color;
        } else {
            secondFrame = (s32)(colorTrack->keys[1].fraction * durationFloat);
            if (frame < secondFrame) {
                f32 span = secondFrame - firstFrame;
                factor = (f32)(frame - firstFrame) / span;
                color1 = colorTrack->keys[0].color;
                color0 = colorTrack->keys[1].color;
            } else {
                f32 span = duration - secondFrame;
                factor = (f32)(frame - secondFrame) / span;
                color1 = colorTrack->keys[1].color;
                color0 = colorTrack->finalColor;
            }
        }
        break;
    default:
        color1 = colorTrack->initialColor;
        color0 = colorTrack->finalColor;
        factor = 0.0f;
        break;
    }

    unitBits = 0x3C000000;
    eeUnpackRGBAWord(color0, unitBits);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(color1, unitBits);
    inverseFactor = 1.0f - factor;
    VU0_SCALE_VF(vf10, inverseFactor);
    VU0_SCALE_VF(vf11, factor);
    SDF_VU0_ADDVECTOR();
    packedColor = eePackRGBAWord(128.0f);

    fadeInFrame = (s32)(alphaTrack->fadeInFraction * durationFloat);
    fadeOutFrame = (s32)(alphaTrack->fadeOutFraction * durationFloat);
    if (frame < fadeInFrame) {
        alphaFactor = (f32)frame / (f32)fadeInFrame;
    } else if (frame > fadeOutFrame) {
        alphaFactor = (f32)(duration - frame) / (f32)(duration - fadeOutFrame);
    } else {
        alphaFactor = 1.0f;
    }

    alphaValue = (f32)alphaTrack->alpha * alphaFactor;
    alpha = (u32)alphaValue;
    return (packedColor & 0x00FFFFFF) | (alpha << 24);
}

f32 effSampleScalarCurve(const EffScalarCurve *curve, s32 frame, s32 duration) {
    f32 from, to, factor, durationFloat;
    s32 firstFrame, secondFrame;
    if (duration == 0) {
        return curve->initialValue;
    }
    durationFloat = duration;
    switch (curve->mode) {
    case EFF_SCALAR_CURVE_MODE_ENDPOINTS:
        factor = (f32)frame / durationFloat;
        from = curve->initialValue;
        to = curve->finalValue;
        break;
    case EFF_SCALAR_CURVE_MODE_ONE_INTERMEDIATE_KEY:
        firstFrame = (s32)(curve->keys[0].fraction * durationFloat);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            from = curve->initialValue;
            to = curve->keys[0].value;
        } else {
            f32 span = duration - firstFrame;
            factor = (f32)(frame - firstFrame) / span;
            from = curve->keys[0].value;
            to = curve->finalValue;
        }
        break;
    case EFF_SCALAR_CURVE_MODE_TWO_INTERMEDIATE_KEYS:
        firstFrame = (s32)(curve->keys[0].fraction * durationFloat);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            from = curve->initialValue;
            to = curve->keys[0].value;
        } else {
            secondFrame = (s32)(curve->keys[1].fraction * durationFloat);
            if (frame < secondFrame) {
                f32 span = secondFrame - firstFrame;
                factor = (f32)(frame - firstFrame) / span;
                from = curve->keys[0].value;
                to = curve->keys[1].value;
            } else {
                f32 span = duration - secondFrame;
                factor = (f32)(frame - secondFrame) / span;
                from = curve->keys[1].value;
                to = curve->finalValue;
            }
        }
        break;
    default:
        from = curve->initialValue;
        to = curve->finalValue;
        factor = 0.0f;
        break;
    }
    return from + (to - from) * factor;
}

INCLUDE_ASM(const s32, "code_0021CE60", func_002210A8);

INCLUDE_ASM(const s32, "code_0021CE60", func_00221218);

void fileInvalidateSlotGroup(FileSlotTable *table, FileSlot *slot) {
    FileKeyBlock *grid = (FileKeyBlock *)table->data0;
    s32 rows = grid->rows;
    s32 columns = grid->columns;
    s32 n = columns * rows;
    u32 index;
    FileSlot *groupSlot;
    s32 i;

    if (n != 0) {
        index = ((u32)slot - (u32)table->slots) >> 5;
        groupSlot = table->slots + (table->instances + index * n);
        for (i = 0; i < n; i++) {
            FILE_SLOT_FRAME(groupSlot) = FILE_SLOT_FRAME_NONE;
            groupSlot++;
        }
    }
}

void fileCopyAndInvalidateSlotGroup(FileSlotTable *table, FileSlot *source) {
    FileKeyBlock *grid = (FileKeyBlock *)table->data0;
    s32 rows = grid->rows;
    s32 columns = grid->columns;
    s32 n = columns * rows;
    u32 index;
    FileSlot *slot;

    if (n != 0) {
        index = ((u32)source - (u32)table->slots) >> 5;
        slot = table->slots + (table->instances + index * n);
        *slot = *source;
        FILE_SLOT_FRAME(slot) = FILE_SLOT_FRAME_NONE;
    }
}

/* vu0 routine: shifts one slot's history and interpolates its trailing samples. */
void effUpdateGridRecordSlotHistory(FileSlotTable *record, FileSlot *slot) {
    f32 points[4][4];
    f32 color[4];
    f32 colorStep[4];
    u32 curveStartColor[4];
    u32 curveEndColor[4];
    u32 curveResultColor[4];
    u32 linearStartColor[4];
    u32 linearEndColor[4];
    u32 linearResultColor[4];
    FileKeyBlock *dims = (FileKeyBlock *)record->data0;
    u32 rows = (u32)dims->rows;
    u32 columns = (u32)dims->columns;
    u32 count = columns * rows;
    u32 remaining;
    u32 i;
    u32 historyIndex;
    s32 frame;
    FileSlot *group;
    FileSlot *shiftTarget;
    FileSlot *cursor;
    f32 fraction;
    f32 step;
    f32 alpha;
    f32 scale;
    f32 angle;
    f32 scaleDelta;
    f32 angleDelta;

    if (count != 0) {
        u8 *base = (u8 *)record->slots;
        frame = FILE_SLOT_FRAME(slot);
        group = (FileSlot *)(base +
            ((record->instances + ((u32)((u8 *)slot - base) >> 5) * count) << 5));
        if (frame == 0) {
            *group = *slot;
            cursor = group;
            for (historyIndex = 0; historyIndex < count; historyIndex++) {
                FILE_SLOT_FRAME(cursor) = FILE_SLOT_FRAME_NONE;
                cursor++;
            }
        } else {
            FILE_SLOT_FRAME(group) = (s32)((u32)frame - 1u);
            step = 1.0f / (f32)count;
            fraction = 0.0f;
            alpha = (f32)(slot->color >> 24);
            remaining = count - rows;
            shiftTarget = group + count - 1;
            cursor = shiftTarget - rows;
            for (historyIndex = 0; historyIndex < remaining; historyIndex++) {
                u32 fadedAlpha;
                *shiftTarget = *cursor;
                fadedAlpha = (u32)(alpha * fraction);
                shiftTarget->color = (cursor->color & 0xFFFFFF) |
                    (fadedAlpha << 24);
                fraction += step;
                cursor--;
                shiftTarget--;
            }
            *group = *slot;
            FILE_SLOT_FRAME(group) = FILE_SLOT_FRAME_NONE;
            if (rows != 1 && columns >= 2) {
                if (frame >= 2 && columns >= 3) {
                    if (FILE_SLOT_FRAME(&group[rows * 2]) >= 0) {
                        FileSlot *previous;
                        SDF_VU0_COPYVECTOR(points[0], group[rows * 2].pos);
                        previous = group + rows;
                        SDF_VU0_COPYVECTOR(points[1], previous->pos);
                        SDF_VU0_COPYVECTOR(points[2], group->pos);
                        SDF_VU0_COPYVECTOR(points[3], group->pos);
                        fraction = 0.0f;
                        cursor = previous - 1;
                        step = 1.0f / (f32)(rows + 1);
                        scale = previous->scale;
                        scaleDelta = group->scale - scale;
                        angle = previous->angle;
                        angleDelta = group->angle - angle;
                        curveStartColor[0] = group->color;
                        EE_MMI_RGBA_UNPACK(curveStartColor, 0.0078125f);
                        SDF_VU0_COPYVECTOR_A_TO_B();
                        curveEndColor[0] = previous->color;
                        EE_MMI_RGBA_UNPACK(curveEndColor, 0.0078125f);
                        SDF_VU0_STOREVECTOR_NOMEM(color);
                        SDF_VU0_SUBVECTOR_B_A();
                        SDF_VU0_SETSCALE1VECTOR();
                        VU0_SCALE_VF(vf10, step);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_STOREVECTOR_NOMEM(colorStep);
                        for (i = 0; i < rows - 1; i++) {
                            fraction += step;
                            vuCatmullRomPoint(points, fraction);
                            SDF_VU0_STOREVECTOR_NOMEM(cursor->pos);
                            cursor->scale = scale + scaleDelta * fraction;
                            cursor->angle = angle + angleDelta * fraction;
                            SDF_VU0_LOADVECTOR(color);
                            SDF_VU0_LOADVECTOR_B(colorStep);
                            VU0_ADD_EXTENDED(vf10, vf10, vf11);
                            SDF_VU0_STOREVECTOR_NOMEM(color);
                            EE_MMI_RGBA_PACK_UNIT(curveResultColor[0], 128.0f);
                            cursor->color = curveResultColor[0];
                            FILE_SLOT_FRAME(cursor) = frame;
                            cursor--;
                        }
                    }
                } else if (frame > 0) {
                    if (FILE_SLOT_FRAME(&group[rows]) >= 0) {
                        FileSlot *previous = group + rows;
                        shiftTarget = previous;
                        fraction = 0.0f;
                        cursor = previous - 1;
                        step = 1.0f / (f32)(rows + 1);
                        scale = previous->scale;
                        scaleDelta = group->scale - scale;
                        angle = previous->angle;
                        angleDelta = group->angle - angle;
                        linearStartColor[0] = group->color;
                        EE_MMI_RGBA_UNPACK(linearStartColor, 0.0078125f);
                        SDF_VU0_COPYVECTOR_A_TO_B();
                        linearEndColor[0] = previous->color;
                        EE_MMI_RGBA_UNPACK(linearEndColor, 0.0078125f);
                        SDF_VU0_STOREVECTOR_NOMEM(color);
                        SDF_VU0_SUBVECTOR_B_A();
                        SDF_VU0_SETSCALE1VECTOR();
                        VU0_SCALE_VF(vf10, step);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_STOREVECTOR_NOMEM(colorStep);
                        {
                            u32 *__restrict packedOutput = linearResultColor;

                            for (i = 0; i < rows - 1; i++) {
                                fraction += step;
                                SDF_VU0_LOADVECTOR(shiftTarget->pos);
                                SDF_VU0_LOADVECTOR_B(group->pos);
                                VU0_LERP_VF10(fraction);
                                SDF_VU0_STOREVECTOR_NOMEM(cursor->pos);
                                cursor->scale = scale + scaleDelta * fraction;
                                cursor->angle = angle + angleDelta * fraction;
                                SDF_VU0_LOADVECTOR(color);
                                SDF_VU0_LOADVECTOR_B(colorStep);
                                VU0_ADD_EXTENDED(vf10, vf10, vf11);
                                SDF_VU0_STOREVECTOR_NOMEM(color);
                                {
                                    u8 *__restrict slotBytes;

                                    EE_MMI_RGBA_PACK_UNIT(packedOutput[0], 128.0f);
                                    slotBytes = (u8 *)cursor;
                                    *(u32 *)(slotBytes + (u32)&((FileSlot *)0)->color) = packedOutput[0];
                                    *(s32 *)(slotBytes + (u32)&FILE_SLOT_FRAME((FileSlot *)0)) = frame;
                                }
                                cursor--;
                            }
                        }
                    }
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "code_0021CE60", effUpdateWaveEmitterSlots);

/* Scale selected wave-emitter parameters from source to destination. */
void effScaleWaveEmitterParameters(ScaleOwner *owner, f32 scale) {
    ScaleSet *src = owner->src;
    ScaleSet *dst = owner->dst;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
    dst->unkC8 = src->unkC8 * scale;
    dst->unkCC = src->unkCC * scale;
    dst->unkD4 = src->unkD4 * scale;
    dst->unkD8 = src->unkD8 * scale;
    dst->unkE0 = src->unkE0 * scale;
}

INCLUDE_ASM(const s32, "code_0021CE60", effUpdateRadialEmitterSlots);

INCLUDE_ASM(const s32, "code_0021CE60", func_00222AD0);

INCLUDE_ASM(const s32, "code_0021CE60", effUpdateCircularEmitterSlots);

INCLUDE_ASM(const s32, "code_0021CE60", func_00223390);

INCLUDE_ASM(const s32, "code_0021CE60", effUpdateOrientedRingEmitterSlots);

INCLUDE_ASM(const s32, "code_0021CE60", func_00223C08);

INCLUDE_ASM(const s32, "code_0021CE60", effUpdateDirectedEmitterSlots);

INCLUDE_ASM(const s32, "code_0021CE60", func_00224388);

INCLUDE_ASM(const s32, "code_0021CE60", effUpdateSectorEmitterSlots);

INCLUDE_ASM(const s32, "code_0021CE60", func_00224B30);

INCLUDE_ASM(const s32, "code_0021CE60", func_00224BB8);
INCLUDE_SDATA(const s32, "code_0021CE60", D_0046D0E0);

INCLUDE_SDATA(const s32, "code_0021CE60", D_0046D0E8);

INCLUDE_SDATA(const s32, "code_0021CE60", D_0046D0F0);

INCLUDE_SDATA(const s32, "code_0021CE60", D_0046D0F8);

