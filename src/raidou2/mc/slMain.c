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

INCLUDE_ASM(const s32, "mc/slMain", func_001EB6A0);

INCLUDE_ASM(const s32, "mc/slMain", func_001EB758);

INCLUDE_ASM(const s32, "mc/slMain", func_001EB800);

INCLUDE_ASM(const s32, "mc/slMain", func_001EB8B8);

INCLUDE_ASM(const s32, "mc/slMain", func_001EB950);

INCLUDE_ASM(const s32, "mc/slMain", func_001EB9D8);

INCLUDE_ASM(const s32, "mc/slMain", func_001EBA08);

INCLUDE_ASM(const s32, "mc/slMain", func_001EBA18);

INCLUDE_ASM(const s32, "mc/slMain", func_001EBA28);

INCLUDE_ASM(const s32, "mc/slMain", func_001EBB90);

void slSetInfo(u32 id) {
    s32 previous;

    previous = gslInfoCode;
    gslInfoCode = id;
    if (previous == 0) {
        gSlWindowCnt = 0;
    }
}

INCLUDE_ASM(const s32, "mc/slMain", func_001EC3D0);

INCLUDE_ASM(const s32, "mc/slMain", func_001EC770);

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

INCLUDE_ASM(const s32, "mc/slMain", func_001EC8E8);

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

INCLUDE_ASM(const s32, "mc/slMain", func_001EC9C0);

INCLUDE_ASM(const s32, "mc/slMain", func_001ECAF8);

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

INCLUDE_ASM(const s32, "mc/slMain", func_001ECC08);

INCLUDE_ASM(const s32, "mc/slMain", func_001ECC30);

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

INCLUDE_ASM(const s32, "mc/slMain", func_001ECD00);

INCLUDE_ASM(const s32, "mc/slMain", func_001ECD40);

s32 slExit(void) {
    slSetInfo(0);
    gslDrawFlagFile = 0;
    gslDrawFlagBg = 0;
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "mc/slMain", func_001ECDE8);

INCLUDE_ASM(const s32, "mc/slMain", func_001ECE10);

INCLUDE_ASM(const s32, "mc/slMain", func_001ECE58);

INCLUDE_ASM(const s32, "mc/slMain", func_001ECE70);

INCLUDE_ASM(const s32, "mc/slMain", func_001ECEC0);

INCLUDE_ASM(const s32, "mc/slMain", func_001ECED8);

INCLUDE_ASM(const s32, "mc/slMain", func_001ECF08);

INCLUDE_ASM(const s32, "mc/slMain", func_001ECFD0);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED028);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED078);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED0A0);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED0D0);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED0E8);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED0F8);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED138);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED160);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED170);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED1B0);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED1C8);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED1E0);

s32 slBootProc1st(void) {
    sdfMcRegetInfo(0);
    gSlNowCardCheck = 1;
    gslCheckCardWait = 0;
    slSetInfo(0);
    gslDrawFlagFile = 0;
    return slBootTop();
}

INCLUDE_ASM(const s32, "mc/slMain", func_001ED240);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED270);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED2D8);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED308);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED348);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED380);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED388);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED4D0);

INCLUDE_ASM(const s32, "mc/slMain", slSizeChk);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED570);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED638);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED700);

INCLUDE_ASM(const s32, "mc/slMain", func_001ED958);

INCLUDE_ASM(const s32, "mc/slMain", func_001EDA28);

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

INCLUDE_ASM(const s32, "mc/slMain", func_001EDAE8);

INCLUDE_ASM(const s32, "mc/slMain", func_001EDB00);

INCLUDE_ASM(const s32, "mc/slMain", func_001EDB80);

INCLUDE_ASM(const s32, "mc/slMain", func_001EDBB0);

INCLUDE_ASM(const s32, "mc/slMain", func_001EDDB8);

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

INCLUDE_ASM(const s32, "mc/slMain", func_001EDF00);

INCLUDE_ASM(const s32, "mc/slMain", func_001EDFD0);

INCLUDE_ASM(const s32, "mc/slMain", func_001EE068);

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

INCLUDE_ASM(const s32, "mc/slMain", func_001EE1D8);

INCLUDE_ASM(const s32, "mc/slMain", func_001EE278);

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

INCLUDE_ASM(const s32, "mc/slMain", func_001EE440);

INCLUDE_ASM(const s32, "mc/slMain", func_001EE4A0);

INCLUDE_ASM(const s32, "mc/slMain", func_001EE500);

INCLUDE_ASM(const s32, "mc/slMain", func_001EE558);

INCLUDE_ASM(const s32, "mc/slMain", func_001EE580);

s32 slSaveWriteEnd_saveData(void) {
    return slSaveWriteFile(slBaseIconName, &slBaseIconBuff, &slBaseIconBuffSize,
                          slSaveWriteEnd_baseIcon, &baseIconFileHandle);
}

INCLUDE_ASM(const s32, "mc/slMain", func_001EE660);

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

INCLUDE_ASM(const s32, "mc/slMain", func_001EEF88);

INCLUDE_ASM(const s32, "mc/slMain", func_001EF018);

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

INCLUDE_ASM(const s32, "mc/slMain", func_001EF218);

INCLUDE_ASM(const s32, "mc/slMain", func_001EF298);

INCLUDE_ASM(const s32, "mc/slMain", func_001EF318);

INCLUDE_ASM(const s32, "mc/slMain", func_001EF800);

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

INCLUDE_ASM(const s32, "mc/slMain", func_001F1550);

INCLUDE_ASM(const s32, "mc/slMain", func_001F16B8);

INCLUDE_ASM(const s32, "mc/slMain", func_001F1A60);

void func_001F1A78(void) {
}

INCLUDE_ASM(const s32, "mc/slMain", func_001F1A80);

INCLUDE_ASM(const s32, "mc/slMain", func_001F1BD8);

INCLUDE_ASM(const s32, "mc/slMain", func_001F1BE0);

INCLUDE_ASM(const s32, "mc/slMain", func_001F1C08);

INCLUDE_ASM(const s32, "mc/slMain", func_001F1C10);

INCLUDE_ASM(const s32, "mc/slMain", func_001F2190);

INCLUDE_ASM(const s32, "mc/slMain", func_001F21F8);

addr_t baseIconFileHandle = 0;

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1580);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1584);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1588);

s32 slExitWaitCnt = 0;

u32 gSlNowCardCheck = 0;

s32 gslCheckCardWait = 0;

u32 gslDrawFlagBg = 0;

u32 gslDrawFlagFile = 0;

s32 gslAnimCnt = 0;

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15A4);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15A8);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15AC);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15B0);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15B4);

s32 slChooseSaveIndex = 0;

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15BC);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15C0);

u32 slQueryYesNoMsg = 0;

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15C8);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15CC);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15D0);

s32 slFileSelError = 0;

s32 gslMode = 0;

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15DC);

s32 gslInfoCode = 0;

u32 gslErrorCode = 0;

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15E8);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15EC);

s32 gSlWindowCnt = 0;

INCLUDE_RODATA(const s32, "mc/slMain", D_004BC040);

INCLUDE_RODATA(const s32, "mc/slMain", jtbl_004BC050);

INCLUDE_RODATA(const s32, "mc/slMain", D_004BC070);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15F4);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15F8);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E15FC);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1600);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1604);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1608);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E160C);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1610);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1614);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1618);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E161C);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1620);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1624);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1628);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1630);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1638);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1640);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1648);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1650);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1658);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1660);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1668);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1670);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1678);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1680);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1688);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E1698);

INCLUDE_SDATA(const s32, "mc/slMain", D_004E16A0);

