#ifndef SLMAIN_H
#define SLMAIN_H

#include "common.h"

void fmShutDown(void);
void fmShutDownFade(void);

typedef s32 (*FileSelectionCallback)(void);

#include "kernel.h"

void fldCallEndMessage(void);
void fmProcTrigSequence_slMain(s32 mode);
s32 fmProcessChk_slMain(void);
s32 fmShutDown2_slMain(void);
dds3ProcessFunc_t sl1Ken(void);
dds3ProcessFunc_t slBootRetry_slMain(void);
dds3ProcessFunc_t slBootTop_slMain(void);
dds3ProcessFunc_t slCallMcClearCardStat(void);
dds3ProcessFunc_t slCallMcFuncClose12(void);
dds3ProcessFunc_t slCallSetInfo2(void);
dds3ProcessFunc_t slCallSetInfo3(void);
void slCallSin2(void);
dds3ProcessFunc_t slCancel(void);
dds3ProcessFunc_t slChooseData(void);
dds3ProcessFunc_t slChooseData_B(void);
void slDrawInfo(void);
dds3ProcessFunc_t slDraw_slMain(dds3ProcessID_t *id);
dds3ProcessFunc_t slErrorWaitMc(void);
dds3ProcessFunc_t slExitWait(dds3ProcessID_t *procId);
void slHardSelect2(void);
dds3ProcessFunc_t slHitAnyKeyNoChkProc(void);
void slInfoMsgDraw(s32 msgNum);
void slKanjiDraw3(s32 x, s32 y, u32 rgba, char **moji, s32 ot);
dds3ProcessFunc_t slLeaf4(void);
dds3ProcessFunc_t slLoadData_slMain(void);
dds3ProcessFunc_t slLoadProc1st(void);
dds3ProcessFunc_t slMcSumIco(void);
dds3ProcessFunc_t slSaveWriteEnd_baseIcon_slMain(void);
s32 slSizeChk(void);
dds3ProcessFunc_t slSystemDataProc(void);

#endif
