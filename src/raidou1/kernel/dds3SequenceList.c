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
/* End of declarations from src/smt3/kernel/dds3SequenceList.c */

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", CallDebug);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", ExitDebug);

s32 CheckDebug(void)
{
    return 0;
}

void CallBootMc(void)
{
    fmProcTrigSequence(3);
}

s32 ExitBootMc(void)
{
    s32 done;
    s32 ret;

    func_00157050();
    func_00157058();
    done = fmShutDown2();
    ret = -1;
    if (done != 0) {
        ret = 0;
    }
    return ret;
}

s32 CheckBootMc(void)
{
    s32 operationResult;

    operationResult = fmProcessChk();
    return operationResult == 0;
}

void CallTitle(u32 flg, u32 *data)
{
    if (data == 0) {
        titleStart(0);
        return;
    }
    titleStart(*data);
}

s32 ExitTitle(void)
{
    titleExit();
    return 0;
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010D880);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010D8A0);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010D8B8);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010D8F8);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010D918);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010D930);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010D970);

void func_0010D990(void) {
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010D998);

s32 CheckStaff(void)
{
    return 0;
}

void CallEndSave(void)
{
    fmProcTrigSequence(2);
}

s32 ExitEndSave(void)
{
    s32 done;
    s32 ret;

    func_00157050();
    func_00157058();
    done = fmShutDown2();
    ret = -1;
    if (done != 0) {
        ret = 0;
    }
    return ret;
}

s32 CheckEndSave(void)
{
    s32 operationResult;

    operationResult = fmProcessChk();
    return operationResult == 0;
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DA18);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DA40);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DA78);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DA98);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DAF8);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DB40);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DB48);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DB60);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DB68);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DB70);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DB88);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DB90);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DB98);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DC20);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DC50);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DC58);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DC70);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DC90);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DCB0);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DCE0);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DD00);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DD20);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DD38);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DD58);

void CallBattleFieldTest(void) {
}

s32 ExitBattleFieldTest(void)
{
    return 0;
}

s32 CheckBattleFieldTest(void)
{
    return 0;
}

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DD90);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DDB0);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DDD0);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DDF0);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DEC8);

INCLUDE_ASM(const s32, "kernel/dds3SequenceList", func_0010DEF0);

INCLUDE_RODATA(const s32, "kernel/dds3SequenceList", D_00444AE8);

INCLUDE_SDATA(const s32, "kernel/dds3SequenceList", D_0046B2F8);

