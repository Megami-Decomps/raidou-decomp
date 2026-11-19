#include "common.h"
#include "dds3KernelCore.h"
#include "gohMain.h"
#include "code_003B7840.h"
#include "scrTraceCode.h"

INCLUDE_ASM(const s32, "code_003B7840", jmsCallGetIntPara);

INCLUDE_ASM(const s32, "code_003B7840", jmsCallGetIntPara2);

INCLUDE_ASM(const s32, "code_003B7840", jmsCallGetIntPara3);

INCLUDE_ASM(const s32, "code_003B7840", jmsCallSetIntReturnValue);

INCLUDE_ASM(const s32, "code_003B7840", jmsCallSetIntReturnValue2);

INCLUDE_ASM(const s32, "code_003B7840", jmsCallSetIntReturnValue3);

INCLUDE_ASM(const s32, "code_003B7840", jmsFunc16);

INCLUDE_ASM(const s32, "code_003B7840", jmsCallGetProcessData4);

INCLUDE_ASM(const s32, "code_003B7840", jmsCallGetProcessData5);

INCLUDE_ASM(const s32, "code_003B7840", jmsScrCmd_JMS_EXIT);

INCLUDE_ASM(const s32, "code_003B7840", jmsScrCmd_JMS_SET_MODE);

s32 jmsCallAnimFunc2(void) {
    scrSetIntReturnValue(jmsAnimFunc());
    return 1;
}

INCLUDE_ASM(const s32, "code_003B7840", jmsCallGetStrPara);

INCLUDE_ASM(const s32, "code_003B7840", jmsCallGetProcessData6);

INCLUDE_ASM(const s32, "code_003B7840", jmsCheckScrRun);

INCLUDE_ASM(const s32, "code_003B7840", jmsScrControl);

INCLUDE_ASM(const s32, "code_003B7840", jmsRootWaitCalcProcess);

dds3ProcessFunc_t jmsRootWaitDrawProcess(dds3ProcessID_t *PID)
{
    gohData_t *work = dds3GetProcessData(PID);

    jmsCallSetOt(work);
    DrawModel_M(work, -1);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t jmsRootWaitUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_003B7840", nbCommand_BTL_FORM_ENE_ON);

s32 RootDestroy(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_003B7840", nbRootCalcProcess);

INCLUDE_ASM(const s32, "code_003B7840", nbCallGetProcessData30);

dds3ProcessFunc_t RootUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_003B7840", TalkSelInit);
