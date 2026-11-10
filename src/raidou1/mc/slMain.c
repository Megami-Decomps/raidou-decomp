#include "common.h"

/* Declarations from smt3-decomp src/smt3/mc/slMain.c */
#include "common.h"

#include "sdf_resource.h"

#include "basic.h"

#include "model.h"

#include "newdata.h"

#include "scr.h"

#include "field.h"

#include "ps2asm.h"

#include "sdfFrame.h"

typedef struct fileRequest fileRequest_t;

enum sl_mc_result
{
    SL_MC_RESULT_PENDING = 0,
    SL_MC_RESULT_SUCCESS = 1,
    SL_MC_RESULT_ERROR = -1,
    SL_MC_RESULT_SPECIAL_ERROR = -2
};

typedef enum sl_mc_result slMcResult_t;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void rsetOtOfs(u32);

extern void rsetAlpha(s32);

dds3ProcessID_t *dds3SearchProcessName(const char *namestr);

extern s32 slProcShutDownFlg;

extern s32 gSlWindowCnt;

extern s32 gslInfoCode;

extern s32 slChooseSaveIndex;

extern u32 mccb;

void sdfMcSetSlotIndex(s32 request, s32 selectedSlot);

s32 sdfMcGetSlotIndex(s32 request);

u32 sdfMcGetSlotFlags(s32 request, s32 slot);

extern saddr_t slChooseData_getCardInfoNext(void);

extern void mcMakeFileName(char *path, s32 num);

extern u32 strlen(const char *s);

extern void *memcpy(void *, const void *, size_t);

void sdfMcFuncOpen(u32 context, const char *path, s32 openFlags);

saddr_t slHeadLoadProc_openFile(void);

extern s32 slMcFd;

slMcResult_t sdfMcFuncOpen1(s32 *resultOut);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

void *sdfMemLock(sdfMemHandle_t *aHandle);

extern sdfMemHandle_t *slLoadFileFd;

extern void sdfMcFuncRead(s32, addr_t, s32);

extern void *slLoadFileHandle;

saddr_t slHeadLoadProc_readFile(void);

extern saddr_t slNoCard(void);

slMcResult_t sdfMcFuncRead1(void);

extern void sdfMcFuncClose(s32);

extern s32 slHeadLoadProc_closeFile(void);

void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

extern void *slExitWaitNextFunc;

s32 slExitWaitProc(void);

extern s32 slExitWaitCnt;

extern s32 gslMode;

extern saddr_t slTop(void);

s32 slEndingExit(void);

extern u32 gslErrorCode;

extern u32 slQueryYesNoMsg;

extern u32 gslDrawFlagFile;

extern saddr_t slQueryYesNo(s32 (*yesFunc)(void), s32 (*noFunc)(void), s32 yesNoDefault);

saddr_t slSaveData(void);

extern saddr_t slFileSel(void);

s32 slExitToTitle(void);

s32 slBootProc_CheckCard(void);

void sdfMcRegetInfo(s32 request);

extern s32 gslCheckCardWait;

extern u32 gSlNowCardCheck;

s32 slBootProc_CheckCardR(void);

extern u32 gslDrawFlagBg;

s32 slExitToTitleSub(void);

extern const char slFldProcSequenceName[];

s32 slExitToTitle2(void);

extern const char slIconPath[];

fileRequest_t *fileLoad(const char *filename);

extern sdfMemHandle_t *slBaseIconMemHandle;

extern addr_t baseIconFileHandle;

s32 sdfMcGetFreeBytes(s32 request);

s32 sdfMcFuncGetInfo1(void);

s32 sdfMcGetFileInfoReady(s32 request);

extern s32 slChooseData_B(void);

extern u32 gslErrorKeyFlag;

extern s8 dds3PadButtonState[];

void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

/* One step of the save and load state machine, which this unit holds as an
 * address and calls through a cast. */
typedef s32 (*slProcFunc_t)(void);

extern u32 slHitAnyKeyNextFunc;

extern s32 slHitAnyKeyNoChkProc(void);

extern s32 slHitAnyKeyProc(void);

void sdfMcFuncChdir(u32 context, const char *path);

saddr_t slChooseData_getCardInfo2(void);

slMcResult_t sdfMcFuncChdir1(void);

void sdfMcSetSlotFlags(s32 request, s32 slot, s32 mask);

extern const char slIconSysName[];

extern u8 slDirEntries[];

void sdfMcFuncGetDir(u32 context, const char *path, void *directoryEntries, s32 entryLimit);

s32 slChooseData_getCardInfo3(void);

slMcResult_t sdfMcFuncGetDir1(s32 *resultOut);

saddr_t slChooseData_getCardInfo2_B(void);

s32 slChooseData_getCardInfo3_B(void);

s32 slChooseData_getCardInfoNext_B(void);

void sdfMcSetCardStat(s32 request);

saddr_t slSaveData_chdir(void);

void sdfMcFuncMkdir(u32 mccb, const char *cb);

saddr_t slSaveProc_Write_mkdir(void);

saddr_t slSaveProc_Write_chdir(void);

slMcResult_t sdfMcFuncMkdir1(void);

extern s32 slFileSelError;

saddr_t slSaveData_deleteIconSys(void);

slMcResult_t sdfMcFuncDelete1(void);

extern s32 slSaveData_writeFile(void);

void slKillLoadProcess(void);

extern const char slBaseIconName[];

extern s32 slSaveWriteEnd_baseIcon(void);

extern addr_t slBaseIconBuff;

extern u32 slBaseIconBuffSize;

saddr_t slSaveWriteFile(const char *filename, u32 *buff, u32 *size, void *endFunc, u32 *srcPtr);

void sdfMcFuncDelete(u32 context, const char *path);

extern u32 *slSaveWriteFile_buff;

extern u32 *slSaveWriteFile_size;

extern void *slSaveWriteFile_endFunc;

extern u32 *slSaveWriteFile_dataWaitPtr;

saddr_t slSaveProc_openFile(void);

saddr_t slSaveProc_waitDataReady(void);

extern void sdfMcFuncWrite(s32 request, u32 first, u32 second);

saddr_t slSaveProc_writeFile(void);

slMcResult_t sdfMcFuncWrite1(void);

saddr_t slSaveProc_closeFile(void);

slMcResult_t sdfMcFuncClose1(void);

slMcResult_t sdfMcFuncFormat1(void);

extern void dds3SoftResetOn(void);

s32 slCardChk(void);

extern void dds3SoftResetOff(void);

void sdfMcFuncFormat(u32 port);

extern u32 gSlEndingMode;

void slVersionConvert_Step1(void);

saddr_t slLoadProc_openFile(void);

saddr_t slLoadProc_readFile(void);

saddr_t slLoadProc_closeFile(void);

void slVersionConvert_Step3(void);

extern s8 slLoadDoneFlag;

extern s32 dds3ConfigChanged(void);

extern void dds3ConfigSaveBackup(void);

extern void dds3ConfigRestoreBackup(void);

extern s32 gslAnimCnt;

extern s32 slDraw(s32);

s32 pacFileCheckEnd(const fileRequest_t *pacFileHandle);

sdfMemHandle_t *fileGetMemHandle(const fileRequest_t *fileHandle);

u8 *fileGetLoadPtr(const fileRequest_t *fileHandle);

u32 fileGetLoadSize(const fileRequest_t *fileHandle);

s32 fileHandleFree(fileRequest_t *fileHandle);

extern const char slCalcName[];

extern u32 gslExitType;

extern s32 (*slQueryYesFunc)(void);

extern s32 (*slQueryNoFunc)(void);

extern s32 slQueryYesNoCursor;

extern s32 slQueryYesNoKeyFlag;

extern saddr_t slQueryYesNoProc(void);

extern u32 gSlExitMode;

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

extern void *frwGetSmallFontFRQ(s32 X, s32 Y, s32 Pri, u32 Color, const char *pStr, s32 pFRQ);

extern void *frwGetSmallNumFontFRQ(s32 X, s32 Y, s32 Pri, u32 Color, const char *pStr, s32 pFRQ);

extern void frDrawFont(void *pFRQ);

extern void frDrawFontEx(void *pFRQ, s32 Mode);

extern void frDestroyFontRequestAfter(void *pFRQ);

extern void *slKanjiDraw_handle;

extern s32 slDDS3To31Flg;

#include "sdk_libc.h"

extern const char slSaveDirFmt[];

extern s8 D_00569743[];

extern s32 version_convert_ver;

extern u8 slIconSysBuf[];

extern u32 slIconSysSaveBuff;

extern u32 slIconSysSaveSize;

extern void dds3AdminiCallSequence(s32 sequence, s32 *data, s32 datasize, s32 restore);

extern void frSetFontDrawOT(s32 OtType);

extern void sdfMcClearSlotFlags(s32 request, s32 slot);

extern saddr_t slChooseData_getCardInfo_B(void);

extern s32 slTopMenuSel[];

extern f32 slNextPageDraw_fc;

extern f32 sdfSin(f32 aRadius);

extern void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh,
                         s32 rgba, sdfTexHandle_t *th);

extern sdfTexHandle_t *slProcPtr;

extern s32 slErrorWaitMc(void);

extern s32 gSlSelCnt;

extern saddr_t slSaveProc_CheckCard(void);

extern s32 slChooseData(void);

extern saddr_t slFormatStart(void);

extern s32 slFormatCancel(void);

extern u8 frEnableMode(s32 Flag);

extern u8 frDisableMode(u8 Flag);

extern void *frSetFontString(const char *text, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

extern void frSetFontSpace(void *pFRQ, s32 Space);

extern void frSetDrawPosition(void *pFRQ, s32 X, s32 Y);

extern void frSetFontColor(void *pFRQ, u32 Color);

extern void func_001C7E38(void *request, s32 a1, s32 a2);

extern void *slKanjiDrawH_handle;

extern s32 sdfMcGetStatus(u32 request);

extern void sdfMcClearCardStat(u32 request);

extern s32 gSlFileErroeFlg;

extern saddr_t slFileSelCalc(void);

extern void *memset(void *a, s32 b, size_t c);

typedef struct slFileName_t {
    s8 ch[20];
} slFileName_t;

/* The 0x34-byte save preview: the renderer reads playTime as a word and
 * endcnt/newfile as signed halfwords. The last count is for the encoded
 * player name, not map state (nmeBuildNameFont and slCopyNonZero). */
typedef struct slFileRec_t {
    s8 verstr[3];
    s8 ver;
    s8 tmnlMode;
    s8 tmnlId;
    s8 level;
    s8 mode;
    s32 playTime;
    s16 endcnt;
    s16 newfile;
    slFileName_t name;
    s8 nameLength;
    s8 unk25[15];
} slFileRec_t;

SIZE_CHECK(slFileRecSizeCheck, slFileRec_t, 0x34);

#define SL_FILE_HEAD ((GameFileInfo_t *)DDS3_GBWK)

extern slFileRec_t slFileRecs[];

extern sdfTexHandle_t *_saveUI;

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern void DrwSprite2D(s32, s32, s32, s32, s32, s32, s32, s32, u32, sdfTexHandle_t *);

extern void aDrawBoxG(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0,
                      u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2,
                      u32 r3, u32 g3, u32 b3, u32 a3);

extern s32 slFileSelDraw(s32 id);

extern void slDrawInfo(void);

extern void slQueryYesNoDraw(void);

extern void slDrawError(void);

extern s32 datGetPlayerLevel(void);

extern s32 evtBitCheck(s32 no);

extern s32 gSlSaveMode;

extern addr_t slSaveDataBuff;

extern u32 slSaveDataSize;

extern char slCheckingMsg0[];

extern char slCheckingMsg1[];

extern char slSavingMsg[];

extern char slDoNotRemoveMsg0[];

extern char slDoNotRemoveMsg1[];

extern char slLoadingMsg[];

extern char slSaveDoneMsg[];

extern char slFormattingMsg[];

extern char slFormatDoneMsg[];

extern char slSelectSaveMsg[];

extern char slSelectLoadMsg[];

extern char slLoadDoneMsg[];

extern s32 slDrawWindow(void);

extern void slMcAccessDraw(void);

extern s32 slExitToTitle2(void);

extern char slNoCardSaveMsg0[];

extern char slNoCardSaveMsg1[];

extern char slNoCardSaveMsg2[];

extern char slNoCardSaveMsg3[];

extern char slNoCardSaveMsg4[];

extern char slNoCardSaveMsg5[];

extern char slNoCardMsg0[];

extern char slNoCardMsg1[];

extern char slNoCardMsg2[];

extern char slNoCardMsg3[];

extern char slNotCardSaveMsg0[];

extern char slNotCardSaveMsg1[];

extern char slNotCardSaveMsg2[];

extern char slNotCardSaveMsg3[];

extern char slNotCardSaveMsg4[];

extern char slNotCardMsg0[];

extern char slNotCardMsg1[];

extern char slNotCardMsg2[];

extern char slLoadFailedMsg[];

extern char slSaveFailedMsg[];

extern char slNoSpaceMsg0[];

extern char slNoSpaceMsg1[];

extern char slNoSpaceMsg2[];

extern char slNoSpaceMsg3[];

extern char slFormatFailedMsg[];

extern char slNoSaveDataMsg0[];

extern char slNoSaveDataMsg1[];

extern char slNoNocturneDataMsg0[];

extern char slNoNocturneDataMsg1[];

extern char slNoNocturneDataMsg2[];

extern s32 gslInfoLoadFlg;

extern s32 gslFlushCnt;

extern s32 gSlMcAccessCnt;

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);

extern sdfTexHandle_t *sdfTexSet(void *aTex);

extern void evtBitOn(s32 no);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                             s32 (*drawExit)(dds3ProcessID_t *),
                             s32 (*deleteExit)(dds3ProcessID_t *), void *p);

void slShutDownProcess(dds3ProcessID_t *id);

void func_00157058(void);

extern void *dds3GetActiveScene(void);

extern void dds3VisibleSceneBasic(void *basic, s32 visible);

void mcMakeFileName(char *path, s32 num);

void slVersionConvert_Step1(void);

/* Save revisions have different persistent block extents. Keeping this
 * earlier provider in C also exposes the native same-TU call visibility to
 * slLoadProc_openFile (DDS idioms: earlier definitions and branch-likely). */
u32 slVersionConvert_GetSize(void);

/* Save fields used by revision conversion; the party and encyclopedia
 * records share the persistent allocation owned by dds3GlobalWorkInit. */
typedef struct {
    u8 unknown00[0x34];
    sdfMemHandle_t *memHandle;
    u8 unknown38[0xA54 - 0x38];
    datUnitWork_t unitwork[16];
    u8 unknown1914[0x11E90 - 0x1914];
    fclencyceelem_t encyclopedia[184];
} slConvertWork_t;

extern const char slConvert03Msg[];

extern const char slConvert04Msg[];

void slVersionConvert_Step3(void);

u8 slGetPadTrig(s32 map);

u8 slGetPadRep(s32 map);

void slCopyNonZero(u8 *dst, u8 *src, s32 count);

void slKanjiDraw(s32 x, s32 y, u32 rgba, const char *moji);

void slKanjiDrawH(s32 x, s32 y, u32 rgba, const char *moji);

void slKanjiDrawN(s32 x, s32 y, u32 rgba, const char *moji);

void slNextPageDraw(void);

extern f32 slMcAccessDraw_fc;

extern void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 r, u32 g, u32 b, u32 a);

extern void aDrawBoxGF(f32 x, f32 y, f32 w, f32 h,
                       u32 r0, u32 g0, u32 b0, u32 a0,
                       u32 r1, u32 g1, u32 b1, u32 a1,
                       u32 r2, u32 g2, u32 b2, u32 a2,
                       u32 r3, u32 g3, u32 b3, u32 a3);

void slMcAccessDraw(void);

s32 slChkWindowOpenDone(void);

s32 slDrawWindow(void);

void slDrawError(void);

void slSetInfo(u32 id);

void slDrawInfo(void);

saddr_t slHeadLoad(void);

saddr_t slHeadLoadProc_openFile(void);

saddr_t slHeadLoadProc_readFile(void);

s32 slHeadLoadProc_closeFile(void);

saddr_t slExitWait(void *nextFunc);

s32 slEndingTop(void);

s32 slEndingUpd2(void);

s32 slEndingExit(void);

saddr_t slBootTop(void);

saddr_t slBootRetry(void);

s32 slBootError(void);

s32 slBootError2(void);

saddr_t slNoCard(void);

saddr_t slTop(void);

s32 slExit(void);

s32 slExitToTitleSub(void);

s32 slExitToTitle(void);

s32 slExitToTitle2(void);

s32 slCardChk(void);

saddr_t slFileSel(void);

s32 slCancel(void);

s32 slSaveProc1st(void);

s32 slLoadProc1st(void);

s32 slEndingProc1st(void);

s32 slBootProc1st(void);

saddr_t slSaveProc_CheckCard(void);

s32 sl1Ken(void);

s32 slSizeChk(void);

s32 slBootProc_CheckCard(void);

s32 slBootProc_CheckCardR(void);

s32 slErrorWaitMc(void);

s32 slErrorWaitToFileMc(void);

s32 slErrorWait(void);

s32 slExitWaitProc(void);

saddr_t slHitAnyKeyNoChk(u32 nextFunc);

s32 slHitAnyKeyNoChkProc(void);

saddr_t slHitAnyKey(addr_t nextFunc);

s32 slHitAnyKeyProc(void);

saddr_t slChooseData_getCardInfo(void);

saddr_t slChooseData_getCardInfo2(void);

s32 slChooseData_getCardInfo3(void);

saddr_t slChooseData_getCardInfoNext(void);

s32 slChooseData(void);

saddr_t slChooseData_getCardInfo_B(void);

saddr_t slChooseData_getCardInfo2_B(void);

s32 slChooseData_getCardInfo3_B(void);

s32 slChooseData_getCardInfoNext_B(void);

s32 slChooseData_B(void);

saddr_t slSaveData(void);

saddr_t slSaveData_chdir(void);

saddr_t slSaveProc_Write_mkdir(void);

saddr_t slSaveProc_Write_chdir(void);

saddr_t slSaveProc_Write_deleteIconSys(void);

s32 slSaveProc_doNothing(void);

saddr_t slSaveWriteEnd_iconSys(void);

s32 slSaveWriteEnd_baseIcon(void);

s32 slSaveWriteEnd_saveData(void);

s32 slSaveData_writeFile(void);

saddr_t slSaveData_deleteIconSys(void);

saddr_t slSaveWriteFile(const char *filename, u32 *buff, u32 *size, void *endFunc, u32 *srcPtr);

saddr_t slSaveProc_openFile(void);

saddr_t slSaveProc_writeFile(void);

saddr_t slSaveProc_closeFile(void);

saddr_t slSaveProc_waitDataReady(void);

saddr_t slProc_Format(void);

saddr_t slFormatStart(void);

s32 slFormatCancel(void);

saddr_t slLoadData(void);

saddr_t slLoadProc_openFile(void);

saddr_t slLoadProc_readFile(void);

saddr_t slLoadProc_closeFile(void);

saddr_t slFileSelCalc(void);

extern f32 slFileSelPulsePhase, slFileSelPulseStep, slFileSelWork;

extern char slNewFileLabel[], slCorruptedFileLabel[], slNewCycleLabel[];

extern char slNoFileLabel[], slCycleLabel[];

extern char slNum2dFmt[], slNum3dFmt[], slNum02dFmt[], D_0054F098[], D_0054F0A0[];

extern char D_00311450[][0x20], D_00311850[][0x20];

extern void cmpSetSkillListRows(s32 rows);

extern void fldKanjiDraw(s32 x, s32 y, u32 rgba, const u8 *text);

extern const char slSaveDirFmt[20];

extern const char slConvert03Msg[38];

extern const char slConvert04Msg[31];

extern const char slFldProcSequenceName[16];

extern const char slIconPath[13];

extern const char slIconSysName[9];

extern const char slBaseIconName[9];

s32 slCalc(dds3ProcessID_t *id);

s32 slDraw(s32 id);

void fmProcTrigSequence(s32 mode);

/* KillProcess passes this callback's process ID but discards its return. */
void slShutDownProcess(dds3ProcessID_t *id);

/* ExitBootMc, ExitEndSave, and the neighboring exit path call this leaf. */
void func_00157050(void);

extern void sdfTexFreeDelayed(sdfTexHandle_t *handle);

extern void fileSync(fileRequest_t *fileHandle);

extern void sdfMemFreeHandleDirect(sdfMemHandle_t **aHandlePtr);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

/* Shared cleanup entry used by the callback wrapper and three exit paths. */
void func_00157058(void);

u32 fmShutDown2(void);

s32 fmProcessChk(void);

u32 slGetExitType(void);

saddr_t slQueryYesNo(s32 (*yesFunc)(void), s32 (*noFunc)(void), s32 yesNoDefault);

extern char slNotFormattedMsg0[];

extern char slNotFormattedMsg1[];

extern char slNotFormattedMsg2[];

extern char slCancelSaveMsg[];

extern char slOverwriteMsg0[];

extern char slOverwriteMsg1[];

extern char slOverwriteMsg2[];

extern char slLoadingFileMsg1[];

extern char slLoadingFileMsg2[];

extern char slLoadingFileMsg3[];

extern char slLoadingFileMsg4[];

extern char slLoadingFileMsg5[];

extern char slIsOkMsg[];

extern char slNewCycleMsg0[];

extern char slNewCycleMsg1[];

extern char slNewCycleMsg2[];

extern char slReturnTitleMsg[];

extern char slOverwriteWarnMsg0[];

extern char slOverwriteWarnMsg1[];

extern char slOverwriteWarnMsg2[];

extern char slNoCardStartMsg0[];

extern char slNoCardStartMsg1[];

extern char slNoCardStartMsg2[];

extern char slNoCardStartMsg3[];

extern char slNoCardStartMsg4[];

extern char slNoCardStartMsg5[];

extern char slNoSpaceStartMsg0[];

extern char slNoSpaceStartMsg1[];

extern char slNoSpaceStartMsg2[];

extern char slNoSpaceStartMsg3[];

extern char slNoSpaceStartMsg4[];

extern char slNoSpaceStartMsg5[];

extern char slNoSpaceStartMsg6[];

extern char slYesLabel[];

extern char slNoLabel[];

extern char slNewCycleMsg3[];

extern const char slCalcName[13];

void slQueryYesNoDraw(void);

u32 slGetExitMode(void);

void slSetTmnlID(s16 type, u16 id);
/* End of declarations from src/smt3/mc/slMain.c */

/* Declarations from dds-decomp src/dds1/game/code_0028A150.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_SL_MC_RESULT_ERROR 1

#define SMT3_DEFINES_SL_MC_RESULT_PENDING 1

#define SMT3_DEFINES_SL_MC_RESULT_SPECIAL_ERROR 1

#define SMT3_DEFINES_SL_MC_RESULT_SUCCESS 1

#define SMT3_DEFINES_SL_SLOT_FLAG_CAN_READ_SAVE 1

#define SMT3_DEFINES_SL_SLOT_FLAG_DELETE_BEFORE_WRITE 1

#define SMT3_DEFINES_SL_SLOT_FLAG_DIRECTORY_READY 1

#define SMT3_DEFINES_fileRequest_t 1

#define SMT3_DEFINES_slConvertWork_t 1

#define SMT3_DEFINES_slFileName_t 1

#define SMT3_DEFINES_slFileRec_t 1

#define SMT3_DEFINES_slMcResult_t 1

#define SMT3_DEFINES_slProcFunc_t 1

#define SMT3_DEFINES_sl_mc_result 1

#define SMT3_DEFINES_sl_slot_flag 1

#include "sdk_libkernl.h"

#include "dds/bill_object_api.h"

#include "dds/btl_stage_task_cleanup.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/eff_random.h"

#include "dds/file_request_api.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_dev_state.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_trig.h"

#include "dds/sdf_vu_matrix.h"

#include "dds/sdf_vu_matrix_multiply_api.h"

#include "dds/sdf_vu_rotation_axes.h"

#include "dds/sdf_vu_transform_api.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libkernl.h"

#include "dds/sdk_libm.h"

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

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(EffRandState *state);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(EffRandState *state);

dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

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

/* Return the descriptor's payload base as a byte address, not the descriptor. */
u8 *sdfMemGetPtr(const struct sdfMemHandle_t *block);

char *strcpy(char *dst, const char *src);

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

extern s32 mcResetSlotMetadata(void);

extern void p2sdStopBGM(u32 trackHandle);

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

extern s32 fileBeginLabeledSlotWrite(void);

extern s32 fileBuildMainBlobAfterDelete(void);

extern McFileDescriptor fileSaveFileDescriptor;

extern struct sdfMemHandle_t *fileSaveReadBufferResource;

extern s32 fileStoreSlotHeader(void);

extern s32 fileSlotSelectPollClear(void);

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

extern s32 fileSlotSelectPoll(void);

extern s32 fileOnAllWritten(void);

extern s32 mcdFinishFileDetection(void);

extern s32 fileSlotSelectionPollCount;

extern s32 fileRequestDefaultMode(void);

extern s32 fileSelectedSlotIndex;

extern s32 fileBeginDetectionRequest(FileMenuContinuationCallback callback);

extern s32 fileScanSlotStates(void);

extern s32 filePollSlotRequestAndResumeFlow(void);

extern u32 D_003BC804;

extern s32 mcHandleDetectionResult(void);

extern void fileQueueInitTransform(void *queue);

extern void sdfVuMatrixToQuaternion(const f32 matrix[4][4]);

extern void mnuRecordSetVector(FileSlotTable *record, const void *src);

extern void fileSetRecordSecondVector(FileSlotTable *record, const void *src);

extern FileMenuStartMode D_003BC848;

extern s32 fileSlotScanIndex;

extern char D_003B2658[];

extern void mcFormatSaveFilename(char *dst, s32 number);

extern char D_003B2678[];

extern sceMcTblGetDir fileDirectoryEntryScratch[];

extern s32 fileBeginSlotMetadataRefresh(void);

extern s32 fileScanSlotStatesAdvance(void);

extern u32 D_003BC834;

extern s32 filePollSlotDetectionAndBranch(void);

extern s32 filePrepareMainBlobWrite(void);

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

void fileShowFlowDialog(void);

void fileClearAllSlotFlags(void);

s32 fileReadSlotPreviewBegin(void);

s32 fileStoreSlotHeader(void);

s32 fileBeginSlotMetadataRefresh(void);

s32 fileResetSelection(void);

s32 mcdEnterDefaultFileFlow(void);

s32 fileRequestDefaultMode(void);

s32 mcdEnterSelectedFileFlow(void);

s32 fileRestartSlotScan(void);

extern s32 D_003BC82C;

extern s32 fileSlotBrowserUpdate(void);

s32 fileScanSlotStates(void);

s32 fileLoadIconFileAndResetSelection(void);

s32 fileResetSlotSelection(void);

s32 fileLoadIconFileAndBeginSlotReset(void);

s32 fileSlotStatusPoll(void);

s32 fileCountSelectableFiles(void);

s32 fileSlotSelectPoll(void);

extern s32 mcClearSlotMetadata(void);

s32 fileSlotSelectPollClear(void);

s32 filePollSlotDetectionAndBranch(void);

s32 fileSetMenuCallbackAndClearResult(FileMenuContinuationCallback callback);

s32 mcdFinishFileDetection(void);

s32 fileBeginDetectionRequest(FileMenuContinuationCallback callback);

s32 filePollSlotRequestAndResumeFlow(void);

s32 fileBeginReadSlotIcon(void);

s32 fileAdvanceSlotScan(void);

s32 mcResetSlotMetadata(void);

s32 fileBeginSaveSlotIconScan(void);

s32 fileScanSlotStatesAdvance(void);

s32 mcClearSlotMetadata(void);

s32 filePrepareMainBlobWrite(void);

s32 fileBuildMainBlobAfterDelete(void);

s32 fileOnAllWritten(void);

s32 fileWriteIconSysComplete(void);

extern u8 fileSaveIconSysData[];

extern u32 D_003BC890;

extern u32 D_003BC894;

s32 fileBeginLabeledSlotWrite(void);

s32 fileBuildMainBlobAndWrite(void);

s32 mcHandleDetectionResult(void);

extern s8 D_003BC7ED;

extern void fileCacheSlotFlagsFromState(void);

extern void fileRestoreSlotFlagsToState(void);

s32 mcdHandleSaveSetupDone(void);

s32 fileSlotBrowserUpdate(void);

s32 fileDrawSlotListAndPreview(s32 work);

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

FileMenuOperationResult fileGetSaveLoadResult(void);

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

INCLUDE_ASM(const s32, "mc/slMain", func_001D5240);

INCLUDE_ASM(const s32, "mc/slMain", func_001D52F8);

INCLUDE_ASM(const s32, "mc/slMain", func_001D53A0);

INCLUDE_ASM(const s32, "mc/slMain", func_001D5438);

INCLUDE_ASM(const s32, "mc/slMain", func_001D54C0);

INCLUDE_ASM(const s32, "mc/slMain", func_001D54F0);

INCLUDE_ASM(const s32, "mc/slMain", func_001D5500);

INCLUDE_ASM(const s32, "mc/slMain", func_001D5510);

INCLUDE_ASM(const s32, "mc/slMain", func_001D5678);

void slSetInfo(u32 id) {
    s32 previous;

    previous = gslInfoCode;
    gslInfoCode = id;
    if (previous == 0) {
        gSlWindowCnt = 0;
    }
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D5B30);

void fileClearAllSlotFlags(void) {
    sdfMcSetCardStat(fileMemoryCardRequestIndex);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 0);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 1);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 2);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 3);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 4);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 5);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 6);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 7);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 8);
    sdfMcClearSlotFlags(fileMemoryCardRequestIndex, 9);
}

saddr_t slHeadLoad(void) {
    char path[0x50];
    u32 ctx;
    s32 slot;
    s32 len;

    sdfMcSetSlotIndex(mccb, slChooseSaveIndex);
    ctx = mccb;
    slot = sdfMcGetSlotIndex(ctx);
    if ((sdfMcGetSlotFlags(ctx, slot) & SL_SLOT_FLAG_CAN_READ_SAVE) == 0) {
        return slChooseData_getCardInfoNext();
    }
    path[0] = 0x2F;
    mcMakeFileName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = 0x2F;
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, 1);
    return EE_ADDR_S(slHeadLoadProc_openFile);
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D6048);

saddr_t slHeadLoadProc_readFile(void) {
    slMcResult_t status = sdfMcFuncRead1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(slMcFd);
        return EE_ADDR_S(slHeadLoadProc_closeFile);
    }
    sdfMemFreeDirect(slLoadFileFd);
    return slNoCard();
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D6120);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6258);

s32 slEndingTop(void) {
    gslMode = 0;
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 4;
    gslDrawFlagFile = 0;
    return slQueryYesNo(&slTop, &slEndingExit, 0);
}

s32 slEndingUpd2(void) {
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 9;
    return slQueryYesNo(&slSaveData, &slFileSel, 1);
}

INCLUDE_ASM(const s32, "mc/slMain", slEndingExit);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6368);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6390);

s32 slBootError(void) {
    gslDrawFlagBg = 1;
    slSetInfo(0);
    slQueryYesNoMsg = 5;
    return slQueryYesNo(&slExitToTitle, &slBootRetry, 1);
}

s32 slBootError2(void) {
    gslDrawFlagBg = 1;
    slSetInfo(0);
    slQueryYesNoMsg = 6;
    return slQueryYesNo(&slExitToTitle, &slBootRetry, 1);
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D6460);

INCLUDE_ASM(const s32, "mc/slMain", func_001D64A0);

s32 slExit(void) {
    slSetInfo(0);
    gslDrawFlagFile = 0;
    gslDrawFlagBg = 0;
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D6548);

s32 mcdEnterDefaultFileFlow(void) {
    u32 mode[4];

    mode[0] = 0;
    dds3AdminiCallSequence(2, mode, 4, 0);
    return 0;
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D65A0);

INCLUDE_ASM(const s32, "mc/slMain", func_001D65B8);

INCLUDE_ASM(const s32, "mc/slMain", func_001D65F8);

s32 fileRestartSlotScan(void) {
    fileClearAllSlotFlags();
    slSetInfo(1);
    D_003BC854 = 0;
    D_003BC810 = 0;
    return fileResetSelection();
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D6640);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6708);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6760);

INCLUDE_ASM(const s32, "mc/slMain", func_001D67B0);

INCLUDE_ASM(const s32, "mc/slMain", func_001D67D8);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6808);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6820);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6830);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6870);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6898);

INCLUDE_ASM(const s32, "mc/slMain", func_001D68A8);

INCLUDE_ASM(const s32, "mc/slMain", func_001D68E8);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6900);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6918);

s32 slBootProc1st(void) {
    sdfMcRegetInfo(0);
    gSlNowCardCheck = 1;
    gslCheckCardWait = 0;
    slSetInfo(0);
    gslDrawFlagFile = 0;
    return slBootTop();
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D6978);

INCLUDE_ASM(const s32, "mc/slMain", func_001D69A8);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6A10);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6A40);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6A80);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6AB8);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6AC0);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6C08);

INCLUDE_ASM(const s32, "mc/slMain", slSizeChk);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6CA8);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6D70);

INCLUDE_ASM(const s32, "mc/slMain", func_001D6E38);

INCLUDE_ASM(const s32, "mc/slMain", func_001D7098);

INCLUDE_ASM(const s32, "mc/slMain", func_001D7168);

s32 slExitWaitProc(void) {
    s32 remaining = slExitWaitCnt - 1;
    slExitWaitCnt = remaining;
    if (remaining <= 0) {
        if (slHitAnyKeyNextFunc == -1) {
            return -1;
        }
        return ((s32 (*)())slExitWaitNextFunc)();
    }
    return 0;
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D7228);

INCLUDE_ASM(const s32, "mc/slMain", func_001D7240);

INCLUDE_ASM(const s32, "mc/slMain", func_001D72C0);

INCLUDE_ASM(const s32, "mc/slMain", func_001D72F0);

INCLUDE_ASM(const s32, "mc/slMain", func_001D74F8);

saddr_t slChooseData_getCardInfo2(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(mccb, slChooseSaveIndex, SL_SLOT_FLAG_DIRECTORY_READY);
        sdfMcFuncGetDir(mccb, slIconSysName, slDirEntries, 1);
        return EE_ADDR_S(slChooseData_getCardInfo3);
    }
    if (t == SL_MC_RESULT_ERROR) {
        return slNoCard();
    }
    return slHeadLoad();
}

s32 slChooseData_getCardInfo3(void) {
    s32 value;
    slMcResult_t status = sdfMcFuncGetDir1(&value);
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (value == 1) {
            sdfMcSetSlotFlags(mccb, slChooseSaveIndex,
                                SL_SLOT_FLAG_CAN_READ_SAVE | SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
        }
        return slHeadLoad();
    }
    if (status == SL_MC_RESULT_ERROR) {
        return slNoCard();
    }
    return slHeadLoad();
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D7640);

INCLUDE_ASM(const s32, "mc/slMain", func_001D7710);

INCLUDE_ASM(const s32, "mc/slMain", func_001D77A8);

saddr_t slChooseData_getCardInfo2_B(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(mccb, slChooseSaveIndex, SL_SLOT_FLAG_DIRECTORY_READY);
        sdfMcFuncGetDir(mccb, slIconSysName, slDirEntries, 1);
        return EE_ADDR_S(slChooseData_getCardInfo3_B);
    }
    if (t == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError();
    }
    return slChooseData_getCardInfoNext_B();
}

s32 slChooseData_getCardInfo3_B(void) {
    s32 value;
    slMcResult_t status = sdfMcFuncGetDir1(&value);
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (value == 1) {
            sdfMcSetSlotFlags(mccb, slChooseSaveIndex,
                                SL_SLOT_FLAG_CAN_READ_SAVE | SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
        }
        return slChooseData_getCardInfoNext_B();
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError();
    }
    return slChooseData_getCardInfoNext_B();
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D7918);

INCLUDE_ASM(const s32, "mc/slMain", func_001D79B8);

saddr_t slSaveData(void) {
    u8 name[0x50];
    u32 entry = mccb;
    s32 slot = sdfMcGetSlotIndex(entry);
    sdfMcGetSlotFlags(entry, slot);
    slSetInfo(2);
    sdfMcSetCardStat(entry);
    if (sdfMcGetSlotFlags(entry, slot) & SL_SLOT_FLAG_DIRECTORY_READY) {
        return slSaveData_chdir();
    }
    name[0] = '/';
    mcMakeFileName(name + 1, slot);
    sdfMcFuncMkdir(entry, (const char *)name);
    return EE_ADDR_S(slSaveProc_Write_mkdir);
}

saddr_t slSaveData_chdir(void) {
    u8 buf[0x50];
    u32 entry = mccb;
    s32 slot = sdfMcGetSlotIndex(entry);

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slot);
    sdfMcFuncChdir(entry, (const char *)buf);
    return EE_ADDR_S(slSaveProc_Write_chdir);
}

saddr_t slSaveProc_Write_mkdir(void) {
    slMcResult_t status = sdfMcFuncMkdir1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(mccb, slFileSelError, SL_SLOT_FLAG_DIRECTORY_READY);
        return slSaveData_chdir();
    }
    slSetInfo(0);
    gslErrorCode = 4;
    return EE_ADDR_S(slErrorWait);
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D7B80);

INCLUDE_ASM(const s32, "mc/slMain", func_001D7BE0);

INCLUDE_ASM(const s32, "mc/slMain", func_001D7C40);

INCLUDE_ASM(const s32, "mc/slMain", func_001D7C98);

INCLUDE_ASM(const s32, "mc/slMain", func_001D7CC0);

s32 slSaveWriteEnd_saveData(void) {
    return slSaveWriteFile(slBaseIconName, &slBaseIconBuff, &slBaseIconBuffSize,
                          slSaveWriteEnd_baseIcon, &baseIconFileHandle);
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D7DA0);

saddr_t slSaveData_deleteIconSys(void) {
    u32 entry = mccb;
    s32 slot = sdfMcGetSlotIndex(entry);
    u32 flags = sdfMcGetSlotFlags(entry, slot);
    if (!(flags & SL_SLOT_FLAG_DELETE_BEFORE_WRITE)) {
        return slSaveData_writeFile();
    }
    sdfMcFuncDelete(entry, slIconSysName);
    return EE_ADDR_S(slSaveProc_Write_deleteIconSys);
}

saddr_t slSaveWriteFile(const char *filename, u32 *buff, u32 *size, void *endFunc, u32 *srcPtr) {
    slSaveWriteFile_buff = buff;
    slSaveWriteFile_size = size;
    slSaveWriteFile_endFunc = endFunc;
    slSaveWriteFile_dataWaitPtr = srcPtr;
    sdfMcFuncOpen(mccb, filename, 0x203);
    return EE_ADDR_S(slSaveProc_openFile);
}

saddr_t slSaveProc_openFile(void) {
    slMcResult_t status = sdfMcFuncOpen1(&slMcFd);

    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (slSaveWriteFile_dataWaitPtr != NULL && *slSaveWriteFile_dataWaitPtr != 0) {
            return EE_ADDR_S(slSaveProc_waitDataReady);
        }
        sdfMcFuncWrite(slMcFd, *slSaveWriteFile_buff, *slSaveWriteFile_size);
        return EE_ADDR_S(slSaveProc_writeFile);
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return EE_ADDR_S(slErrorWait);
    }
    return 0;
}

saddr_t slSaveProc_writeFile(void) {
    slMcResult_t status = sdfMcFuncWrite1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(slMcFd);
        return EE_ADDR_S(slSaveProc_closeFile);
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return EE_ADDR_S(slErrorWait);
    }
    return 0;
}

saddr_t slSaveProc_closeFile(void) {
    slMcResult_t status = sdfMcFuncClose1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        return ((s32 (*)())slSaveWriteFile_endFunc)();
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return EE_ADDR_S(slErrorWait);
    }
    return 0;
}

saddr_t slSaveProc_waitDataReady(void) {
    if (baseIconFileHandle != 0) {
        return 0;
    }
    sdfMcFuncWrite(slMcFd, *slSaveWriteFile_buff, *slSaveWriteFile_size);
    return EE_ADDR_S(slSaveProc_writeFile);
}

saddr_t slProc_Format(void) {
    slMcResult_t status = sdfMcFuncFormat1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    dds3SoftResetOn();
    if (status < 0) {
        slSetInfo(0);
        gslErrorCode = 6;
        return EE_ADDR_S(slErrorWait);
    }
    slSetInfo(6);
    return slHitAnyKey(EE_ADDR(slCardChk));
}

saddr_t slFormatStart(void) {
    slSetInfo(5);
    dds3SoftResetOff();
    sdfMcFuncFormat(mccb);
    return EE_ADDR_S(slProc_Format);
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D8710);

INCLUDE_ASM(const s32, "mc/slMain", func_001D87A0);

INCLUDE_ASM(const s32, "mc/slMain", slLoadProc_openFile);

saddr_t slLoadProc_readFile(void) {
    slMcResult_t status = sdfMcFuncRead1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(slMcFd);
        return EE_ADDR_S(slLoadProc_closeFile);
    }
    sdfMemFreeDirect(slLoadFileFd);
    slSetInfo(0);
    gslErrorCode = 3;
    return EE_ADDR_S(slErrorWait);
}

INCLUDE_ASM(const s32, "mc/slMain", func_001D89A0);

INCLUDE_ASM(const s32, "mc/slMain", func_001D8A20);

INCLUDE_ASM(const s32, "mc/slMain", func_001D8A98);

INCLUDE_ASM(const s32, "mc/slMain", func_001D8F88);

s32 slCalc(dds3ProcessID_t *id) {
    s32 next;
    u32 job;
    s32 (*cur)(dds3ProcessID_t *);
    gslAnimCnt++;
    slDraw(EE_ADDR_S(id));
    next = proc_func_ans(id);
    if (next == -1) {
        return next;
    }
    job = baseIconFileHandle;
    cur = proc_func_ans;
    if (next != 0) {
        cur = EE_ADDR_AS(slProcFunc_t, next);
    }
    proc_func_ans = cur;
    if (job != 0 && pacFileCheckEnd(EE_ADDR_AS(fileRequest_t *, job)) != 0) {
        baseIconFileHandle = 0;
        slBaseIconMemHandle = fileGetMemHandle((fileRequest_t *)EE_ADDR_PTR(job));
        slBaseIconBuff = EE_ADDR(fileGetLoadPtr((fileRequest_t *)EE_ADDR_PTR(job)));
        slBaseIconBuffSize = fileGetLoadSize((fileRequest_t *)EE_ADDR_PTR(job));
        fileHandleFree((fileRequest_t *)EE_ADDR_PTR(job));
    }
    return 0;
}

INCLUDE_ASM(const s32, "mc/slMain", func_001DA340);

INCLUDE_RODATA(const s32, "mc/slMain", D_00455250);

INCLUDE_ASM(const s32, "mc/slMain", func_001DA4A8);

INCLUDE_ASM(const s32, "mc/slMain", func_001DA7A8);

void func_001DA7C0(void) {
}

INCLUDE_ASM(const s32, "mc/slMain", func_001DA7C8);

INCLUDE_ASM(const s32, "mc/slMain", func_001DA908);

INCLUDE_ASM(const s32, "mc/slMain", func_001DA910);

INCLUDE_ASM(const s32, "mc/slMain", func_001DA938);

INCLUDE_ASM(const s32, "mc/slMain", func_001DA940);

INCLUDE_ASM(const s32, "mc/slMain", func_001DAEC0);

INCLUDE_ASM(const s32, "mc/slMain", func_001DAF28);

addr_t baseIconFileHandle = 0;

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CDE4);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CDE8);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CDEC);

s32 slExitWaitCnt = 0;

u32 gSlNowCardCheck = 0;

s32 gslCheckCardWait = 0;

u32 gslDrawFlagBg = 0;

u32 gslDrawFlagFile = 0;

s32 gslAnimCnt = 0;

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE08);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE0C);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE10);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE14);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE18);

s32 slChooseSaveIndex = 0;

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE20);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE24);

u32 slQueryYesNoMsg = 0;

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE2C);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE30);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE34);

s32 slFileSelError = 0;

s32 gslMode = 0;

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE40);

s32 gslInfoCode = 0;

u32 gslErrorCode = 0;

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE4C);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE50);

s32 gSlWindowCnt = 0;

INCLUDE_RODATA(const s32, "mc/slMain", D_00455270);

INCLUDE_RODATA(const s32, "mc/slMain", jtbl_00455280);

INCLUDE_RODATA(const s32, "mc/slMain", D_00455298);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE58);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE5C);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE60);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE64);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE68);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE6C);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE70);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE74);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE78);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE7C);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE80);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE88);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE90);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE94);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CE98);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CEA0);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CEA8);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CEB0);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CEB8);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CEC0);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CEC8);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CED8);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CEE0);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CEF0);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF00);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF08);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF10);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF18);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF20);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF28);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF30);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF38);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF40);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF48);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF50);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF58);

INCLUDE_SDATA(const s32, "mc/slMain", D_0046CF60);

