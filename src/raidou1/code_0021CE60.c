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

INCLUDE_ASM(const s32, "code_0021CE60", fileJobFreeSecondaryBuffer);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobCreateChild);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobNotifyPair);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobNotifyComplete);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobInvokeTypeCallback);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobInvokePositionCallback);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobInvokeRotationCallback);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobInvokeScaleCallback);

INCLUDE_ASM(const s32, "code_0021CE60", fileDispatchJobTypeCallback);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobSetPrimaryData);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobCopyCommandIntoPrimaryData);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobSetSecondaryData);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobCopyCommandIntoSecondaryData);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobWriteSerializedPayload);

INCLUDE_ASM(const s32, "code_0021CE60", fileWriteToPfs);

INCLUDE_ASM(const s32, "code_0021CE60", fileDuplicateJob);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobCreateFromCommandState);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobSerializedSize);

INCLUDE_ASM(const s32, "code_0021CE60", func_0021D798);

INCLUDE_ASM(const s32, "code_0021CE60", func_0021D7F0);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueAppend);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueInsertAfter);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueRemove);

INCLUDE_ASM(const s32, "code_0021CE60", func_0021D8E8);

INCLUDE_ASM(const s32, "code_0021CE60", func_0021D920);

INCLUDE_ASM(const s32, "code_0021CE60", fileDestroyJob);

INCLUDE_ASM(const s32, "code_0021CE60", fileCloneQueueEntries);

void CallShopTest(s32 flg, void *data)
{
    fclShopProcessStart(data);
}

INCLUDE_ASM(const s32, "code_0021CE60", func_0021DBD0);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueDestroy);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueClone);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueNotifyAllJobsComplete);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueSetPosition);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueSetRotation);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueSetScale);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueSetColor);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueCopyRotationFromSource);

INCLUDE_ASM(const s32, "code_0021CE60", fileAppendJob);

INCLUDE_ASM(const s32, "code_0021CE60", fileDuplicateAndAppendJob);

INCLUDE_ASM(const s32, "code_0021CE60", fileAppendJobFromCommandPath);

INCLUDE_ASM(const s32, "code_0021CE60", fileJobDuplicateAfter);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueLinkJobToSectorLeader);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueDetachSectorFollower);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueRemoveAndDestroyJob);

INCLUDE_ASM(const s32, "code_0021CE60", func_0021E800);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueSaveImage);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueSaveVersionedImage);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueCreateFromCommandState);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueFindById);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueFindFlaggedById);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueFindBySector);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueGetAt);

INCLUDE_ASM(const s32, "code_0021CE60", fileFindQueuedJobIndex);

INCLUDE_ASM(const s32, "code_0021CE60", fileQueueCountLinkedJobs);

INCLUDE_ASM(const s32, "code_0021CE60", camFollowOffsetVec);

INCLUDE_ASM(const s32, "code_0021CE60", camAimRotation);

INCLUDE_ASM(const s32, "code_0021CE60", fileCreateSurfaceLoaderState);

INCLUDE_ASM(const s32, "code_0021CE60", fileCreateGridLoaderRecord);

INCLUDE_ASM(const s32, "code_0021CE60", func_0021F848);

INCLUDE_ASM(const s32, "code_0021CE60", fileDestroyEffectSurfaceAndChildren);

INCLUDE_ASM(const s32, "code_0021CE60", fileLoadObjectCreateChild);

INCLUDE_ASM(const s32, "code_0021CE60", func_0021FAC8);

INCLUDE_ASM(const s32, "code_0021CE60", fileLoadObjectSetResource);

INCLUDE_ASM(const s32, "code_0021CE60", fileLoadObjectOpenNamedDevice);

INCLUDE_ASM(const s32, "code_0021CE60", fileLoadObjectOpenDevice);

INCLUDE_ASM(const s32, "code_0021CE60", fileLoadObjectOpenAndStartDevice);

INCLUDE_ASM(const s32, "code_0021CE60", fileReplaceEffectSurfaceJobs);

INCLUDE_ASM(const s32, "code_0021CE60", fileReplaceEffectSurfaceQueues);

INCLUDE_ASM(const s32, "code_0021CE60", fileReplaceReferenceHolder);

INCLUDE_ASM(const s32, "code_0021CE60", fileClearLoadObjectReferences);

INCLUDE_ASM(const s32, "code_0021CE60", func_00220170);

INCLUDE_ASM(const s32, "code_0021CE60", func_002201A0);

INCLUDE_ASM(const s32, "code_0021CE60", func_00220B40);

INCLUDE_ASM(const s32, "code_0021CE60", fileSendLoadObjectRecordVector);

INCLUDE_ASM(const s32, "code_0021CE60", fileCopyLoadObjectRecordVector);

INCLUDE_ASM(const s32, "code_0021CE60", fileSetRecordWordFour);

INCLUDE_ASM(const s32, "code_0021CE60", fileSetLoadObjectScale);

INCLUDE_ASM(const s32, "code_0021CE60", fileResetSlotStates);

INCLUDE_ASM(const s32, "code_0021CE60", effSampleColorAlphaTracks);

INCLUDE_ASM(const s32, "code_0021CE60", effSampleScalarCurve);

INCLUDE_ASM(const s32, "code_0021CE60", func_002210A8);

INCLUDE_ASM(const s32, "code_0021CE60", func_00221218);

INCLUDE_ASM(const s32, "code_0021CE60", fileInvalidateSlotGroup);

INCLUDE_ASM(const s32, "code_0021CE60", fileCopyAndInvalidateSlotGroup);

INCLUDE_ASM(const s32, "code_0021CE60", effUpdateGridRecordSlotHistory);

INCLUDE_ASM(const s32, "code_0021CE60", effUpdateWaveEmitterSlots);

INCLUDE_ASM(const s32, "code_0021CE60", effScaleWaveEmitterParameters);

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

