#ifndef SLMAIN_H
#define SLMAIN_H

#include "common.h"

void fmShutDown(void);
void fmShutDownFade(void);

typedef s32 (*FileSelectionCallback)(void);

extern const char slCalcName[];

struct rstBG_t;
struct sdfTexHandle_t;
struct sprHandle_t;

/* Set while the file menu owns the textures and resources fmShutDown releases. */
extern s32 slProcShutDownFlg;
extern struct rstBG_t *fmShutDownData;
extern struct sprHandle_t *slNewCycleData16;
extern struct sdfTexHandle_t *fmData2;
extern struct sdfTexHandle_t *fmData3;
extern struct sdfTexHandle_t *fmShutDownData3;
extern struct sdfTexHandle_t *fmShutDownData4;
extern struct sdfTexHandle_t *fmShutDownData5;
extern struct sdfTexHandle_t *slNewCycleData19;
extern s32 slData5;
extern s32 slData2;
extern s32 slNewCycleData2[];
extern s32 slData3;
extern s32 slNewCycleData99;
extern s32 gSlExitMode;
extern s32 gSlSaveMode;
extern s32 gSlFileErroeFlg;
extern s32 gSlEndingMode;
extern s32 fmProcTrigSequence_slMainData2;
extern s32 gSlMcAccessCnt;

#include "kernel.h"

void gohKarteRegStatInit(void);
void fmProcTrigSequence_slMain(s32 mode);
s32 fmProcessChk_slMain(void);
s32 fmShutDown2_slMain(void);
dds3ProcessFunc_t sl1Ken(void);
dds3ProcessFunc_t slBootRetry_slMain(void);
saddr_t slBootRetry(void);
dds3ProcessFunc_t slCallMcClearCardStat(void);
dds3ProcessFunc_t slCallMcFuncClose12(void);
dds3ProcessFunc_t slCallSetInfo2(void);
dds3ProcessFunc_t slCallSetInfo3(void);
s32 slCallPlaySE2(dds3ProcessID_t *id);
extern s32 slCallPlaySE2Data;
void slNextPageDraw(void);
s32 slDrawWindow(void);
void slKanjiDrawH(s32 x, s32 y, u32 rgba, const char *moji);
void slMcAccessDraw(void);
s32 slQueryYesNoProcSub(void);
void fileResetLoadContextSlide(void);
dds3ProcessFunc_t slCancel(void);
dds3ProcessFunc_t slChooseData(void);
dds3ProcessFunc_t slChooseData_B(void);
void slDrawInfo(void);
#ifdef VERSION_RAIDOU1
void slDrawError(void);
#else
void slDrawError(void);
#endif
void slNewCycle(dds3ProcessID_t *id);
void slQueryYesNoDraw(void);
dds3ProcessFunc_t slDraw_slMain(dds3ProcessID_t *id);
dds3ProcessFunc_t slErrorWaitMc(void);
dds3ProcessFunc_t slExitWait(dds3ProcessID_t *procId);
saddr_t slTop(void);
dds3ProcessFunc_t slHitAnyKeyNoChkProc(void);
void gohKarteResResultUpdateProcess(s32 msgNum);
void slKanjiDraw3(s32 x, s32 y, u32 rgba, char **moji, s32 ot);
s32 slExitToTitle(void);
dds3ProcessFunc_t slLoadData_slMain(void);
dds3ProcessFunc_t slDefaultFileFlowProc1st(void);
dds3ProcessFunc_t slSaveProc1st(void);
dds3ProcessFunc_t slSaveWriteEnd_baseIcon_slMain(void);
dds3ProcessFunc_t slLoadProc1st(void);
s32 slLoadProc1stBgm(void);
s32 slLeaf7(void);
s32 slCallSetInfo5(void);
saddr_t slCallMcRegetInfo(s32 (*yesFunc)(void), s32 (*noFunc)(void), s32 yesNoDefault);
s32 slEndingProc1st(void);
void slShutDownProcess(dds3ProcessID_t *id);
extern char fmProcTrigSequence_slMainData3[];
s32 slBootProc_CheckCard(void);
s32 slBootProc_CheckCardR(void);
extern s32 (*slQueryYesFunc)(void);
extern s32 (*slQueryNoFunc)(void);
saddr_t slQueryYesNoProc(void);
extern void *slLoadFileHandle;
extern struct frFontHandle_t *slKanjiDrawH_handle2;
s32 slSizeChk(void);
dds3ProcessFunc_t slSystemDataProc(void);
extern const char slIconPath[];
/* slDrawInfo's message strings; the labels past slDrawInfoData13 differ per game. */
extern char slDrawInfoData[], slDrawInfoData2[], slDrawInfoData3[], slDrawInfoData4[];
extern char slDrawInfoData5[], slDrawInfoData6[], slDrawInfoData7[], slDrawInfoData8[];
extern char slDrawInfoData9[], slDrawInfoData10[], slDrawInfoData11[], slDrawInfoData12[];
extern char slDrawInfoData13[], slDrawInfoData14[];
#ifdef VERSION_RAIDOU2
extern char slDrawInfoData18[];
#else
extern char slDrawInfoData17[];
#endif
saddr_t slHitAnyKeyProc(void);
s32 slFileSel(void);
saddr_t slLoadProc_openFile(void);
saddr_t slChooseData_getCardInfo(void);

#endif
