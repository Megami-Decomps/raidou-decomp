#include "common.h"
#include "kernel.h"
#include "code_r1_001CC1D0.h"

INCLUDE_ASM(const s32, "newbattle/nbAiScript", nbCommand_BTL_AI_CHANGE_ON);

#ifdef VERSION_RAIDOU1
s32 TestProcess2(dds3ProcessID_t *PID);
s32 evtRefreshDispatchStart(dds3ProcessID_t *PID);
s32 gohAccidentCalcProcess(dds3ProcessID_t *PID);
s32 gohBakBakDestroy(dds3ProcessID_t *PID);
s32 gohBakBakInit(dds3ProcessID_t *PID);
s32 gohBakNmlCalcProcess(dds3ProcessID_t *PID);
s32 gohBakNmlDrawProcess(dds3ProcessID_t *PID);
s32 gohBakNmlUpdateProcess(dds3ProcessID_t *PID);
s32 gohCalcProcess(dds3ProcessID_t *PID);
s32 gohCallCallDrawBack2(dds3ProcessID_t *PID);
s32 gohCallCallDrawBack3(dds3ProcessID_t *PID);
s32 gohCallEndMessageEx(dds3ProcessID_t *PID);
s32 gohCallGetProcessData(dds3ProcessID_t *PID);
s32 gohCallGetProcessData10(dds3ProcessID_t *PID);
s32 gohCallGetProcessData11(dds3ProcessID_t *PID);
s32 gohCallGetProcessData12(dds3ProcessID_t *PID);
s32 gohCallGetProcessData13(dds3ProcessID_t *PID);
s32 gohCallGetProcessData2(dds3ProcessID_t *PID);
s32 gohCallGetProcessData23(dds3ProcessID_t *PID);
s32 gohCallGetProcessData24(dds3ProcessID_t *PID);
s32 gohCallGetProcessData25(dds3ProcessID_t *PID);
s32 gohCallGetProcessData26(dds3ProcessID_t *PID);
s32 gohCallGetProcessData27(dds3ProcessID_t *PID);
s32 gohCallGetProcessData28(dds3ProcessID_t *PID);
s32 gohCallGetProcessData29(dds3ProcessID_t *PID);
s32 gohCallGetProcessData3(dds3ProcessID_t *PID);
s32 gohCallGetProcessData30(dds3ProcessID_t *PID);
s32 gohCallGetProcessData31(dds3ProcessID_t *PID);
s32 gohCallGetProcessData32(dds3ProcessID_t *PID);
s32 gohCallGetProcessData33(dds3ProcessID_t *PID);
s32 gohCallGetProcessData34(dds3ProcessID_t *PID);
s32 gohCallGetProcessData35(dds3ProcessID_t *PID);
s32 gohCallGetProcessData36(dds3ProcessID_t *PID);
s32 gohCallGetProcessData37(dds3ProcessID_t *PID);
s32 gohCallGetProcessData38(dds3ProcessID_t *PID);
s32 gohCallGetProcessData39(dds3ProcessID_t *PID);
s32 gohCallGetProcessData4(dds3ProcessID_t *PID);
s32 gohCallGetProcessData40(dds3ProcessID_t *PID);
s32 gohCallGetProcessData41(dds3ProcessID_t *PID);
s32 gohCallGetProcessData5(dds3ProcessID_t *PID);
s32 gohCallGetProcessData6(dds3ProcessID_t *PID);
s32 gohCallGetProcessData7(dds3ProcessID_t *PID);
s32 gohCallGetProcessData8(dds3ProcessID_t *PID);
s32 gohCallGetProcessData9(dds3ProcessID_t *PID);
s32 gohCmbSearchiCalcProcess(dds3ProcessID_t *PID);
s32 gohCmbSelCalcProcess(dds3ProcessID_t *PID);
s32 gohCmbSelDestroy(dds3ProcessID_t *PID);
s32 gohCyuuseiDrawProcess(dds3ProcessID_t *PID);
s32 gohDrawProcess(dds3ProcessID_t *PID);
s32 gohLeaf24(dds3ProcessID_t *PID);
s32 gohLeaf26(dds3ProcessID_t *PID);
s32 gohLeaf27(dds3ProcessID_t *PID);
s32 gohLeaf28(dds3ProcessID_t *PID);
s32 gohLeaf29(dds3ProcessID_t *PID);
s32 gohLeaf30(dds3ProcessID_t *PID);
s32 gohLeaf33(dds3ProcessID_t *PID);
s32 gohLeaf34(dds3ProcessID_t *PID);
s32 gohMiscInit(dds3ProcessID_t *PID);
s32 gohRecoverCalcProcess(dds3ProcessID_t *PID);
s32 gohRecoverDrawProcess(dds3ProcessID_t *PID);
s32 gohRecoverUpdateProcess(dds3ProcessID_t *PID);
s32 gohRootCalcProcess(dds3ProcessID_t *PID);
s32 gohRootDestroy(dds3ProcessID_t *PID);
s32 gohRootDrawProcess(dds3ProcessID_t *PID);
s32 gohRootInit(dds3ProcessID_t *PID);
s32 gohRootUpdateProcess(dds3ProcessID_t *PID);
s32 gohRootWaitCalcProcess(dds3ProcessID_t *PID);
s32 gohRootWaitDrawProcess(dds3ProcessID_t *PID);
s32 gohRootWaitUpdateProcess(dds3ProcessID_t *PID);
s32 gohSearchiErrorCalcProcess(dds3ProcessID_t *PID);
s32 gohSinanCalcProcess(dds3ProcessID_t *PID);
s32 gohStatInit(dds3ProcessID_t *PID);
s32 gohSwordTreeUpdateProcess(dds3ProcessID_t *PID);
s32 gohTalkCalcProcess(dds3ProcessID_t *PID);
s32 gohTalkDestroy(dds3ProcessID_t *PID);
s32 gohTalkInit(dds3ProcessID_t *PID);
s32 gohUpdateProcess(dds3ProcessID_t *PID);
s32 nbCommand_BTL_AI_CHANGE_ON(dds3ProcessID_t *PID);

jmsSeqUnit_t gohSeqUnit[16] = {
    {0, NULL, NULL, gohRootWaitCalcProcess, gohRootWaitDrawProcess, gohRootWaitUpdateProcess, NULL},
    {0, gohRootInit, gohRootDestroy, gohRootCalcProcess, gohRootDrawProcess, gohRootUpdateProcess, NULL},
    {0, gohCallGetProcessData, gohBakNmlCalcProcess, gohCallGetProcessData2, gohBakNmlDrawProcess, gohBakNmlUpdateProcess, NULL},
    {0, gohBakBakInit, gohBakBakDestroy, gohCallGetProcessData3, gohCallGetProcessData4, gohCallGetProcessData23, NULL},
    {0, gohLeaf24, nbCommand_BTL_AI_CHANGE_ON, gohCallGetProcessData5, gohCallGetProcessData6, gohCallGetProcessData24, NULL},
    {0, gohCallEndMessageEx, gohCallCallDrawBack2, gohCallGetProcessData7, gohCallGetProcessData8, gohSwordTreeUpdateProcess, NULL},
    {0, gohStatInit, gohCmbSearchiCalcProcess, gohCallGetProcessData9, gohCallGetProcessData25, gohCallGetProcessData26, NULL},
    {0, gohCallGetProcessData10, gohLeaf26, gohCallGetProcessData11, gohCallGetProcessData27, gohCallGetProcessData28, NULL},
    {0, gohCallGetProcessData12, gohLeaf27, gohCallGetProcessData13, gohCyuuseiDrawProcess, gohAccidentCalcProcess, NULL},
    {0, gohCallGetProcessData39, gohCmbSelDestroy, gohCmbSelCalcProcess, gohCallGetProcessData40, gohCallGetProcessData41, NULL},
    {0, TestProcess2, gohCallCallDrawBack3, gohRecoverCalcProcess, gohRecoverDrawProcess, gohRecoverUpdateProcess, NULL},
    {0, gohTalkInit, gohTalkDestroy, gohTalkCalcProcess, gohCallGetProcessData29, gohCallGetProcessData30, NULL},
    {0, gohMiscInit, gohLeaf28, gohCallGetProcessData31, gohCallGetProcessData32, gohCallGetProcessData33, NULL},
    {0, gohCallGetProcessData34, gohLeaf29, gohSinanCalcProcess, gohCallGetProcessData35, gohCallGetProcessData36, NULL},
    {0, gohCallGetProcessData37, gohLeaf30, gohSearchiErrorCalcProcess, evtRefreshDispatchStart, gohCallGetProcessData38, NULL},
    {0, gohLeaf33, gohLeaf34, gohCalcProcess, gohDrawProcess, gohUpdateProcess, NULL},
};
#endif
