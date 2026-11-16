#include "common.h"
#include "dds3KernelCore.h"
#include "code_00346258.h"
#include "code_r1_001CC1D0.h"
#include "jmsInit.h"
#include "gohMain.h"
#include "gohDraw.h"
#include "fclMisc.h"

dds3ProcessFunc_t gohCallGetProcessData46(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    gohModel(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohCallGetProcessData47(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00346258", gohSetModelScale);

INCLUDE_ASM(const s32, "code_00346258", gohModelLoad);

INCLUDE_ASM(const s32, "code_00346258", gohModelFree);

INCLUDE_ASM(const s32, "code_00346258", gohModelLightSet);

INCLUDE_ASM(const s32, "code_00346258", gohLeaf33);

INCLUDE_ASM(const s32, "code_00346258", gohLeaf34);

INCLUDE_ASM(const s32, "code_00346258", gohCalcProcess);

dds3ProcessFunc_t gohDrawProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00346258", gohIsCombineFlag);

INCLUDE_ASM(const s32, "code_00346258", Direction2Destroy);

INCLUDE_ASM(const s32, "code_00346258", Direction2CalcProcess);

INCLUDE_ASM(const s32, "code_00346258", Direction2DrawProcess);

dds3ProcessFunc_t gohResultInit(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00346258", gohCallGetProcessData14);

INCLUDE_ASM(const s32, "code_00346258", gohResultDrawProcess);

INCLUDE_ASM(const s32, "code_00346258", gohCyuuseiCalcProcess);

INCLUDE_ASM(const s32, "code_00346258", Direction2DrawProcess_gohDirection);

dds3ProcessFunc_t gohResultUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00346258", gohEndInit);

INCLUDE_ASM(const s32, "code_00346258", gohEndDestroy);

INCLUDE_ASM(const s32, "code_00346258", gohEndCalcProcess);

dds3ProcessFunc_t gohEndDrawProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohEndUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00346258", gohEnd2Init);

INCLUDE_ASM(const s32, "code_00346258", gohEnd2CalcProcess);

INCLUDE_ASM(const s32, "code_00346258", gohEnd2CalcProcess_gohDirection);

dds3ProcessFunc_t gohEquipSwordCalcProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohCallGetProcessData48(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00346258", fldCallGetProcessData10);

INCLUDE_ASM(const s32, "code_00346258", fldPlayerSvsOff);

INCLUDE_ASM(const s32, "code_00346258", gohChkEnableSelect);

INCLUDE_ASM(const s32, "code_00346258", gohCallBitCheck4);

INCLUDE_ASM(const s32, "code_00346258", gohSetCommandSub);

INCLUDE_ASM(const s32, "code_00346258", gohSetCommandSub2);

INCLUDE_ASM(const s32, "code_00346258", gohSetCommandSub3);

INCLUDE_ASM(const s32, "code_00346258", gohSetCommand);

INCLUDE_ASM(const s32, "code_00346258", gohProcessSub);

INCLUDE_ASM(const s32, "code_00346258", gohCallMenuProcessCheck);

INCLUDE_ASM(const s32, "code_00346258", gohProcess);

INCLUDE_ASM(const s32, "code_00346258", gohShutdown);

INCLUDE_ASM(const s32, "code_00346258", DrawProcess_gohMenu);

INCLUDE_ASM(const s32, "code_00346258", gohGetBackStat);

INCLUDE_ASM(const s32, "code_00346258", gohStart);

INCLUDE_ASM(const s32, "code_00346258", gohSetFocus);

INCLUDE_ASM(const s32, "code_00346258", gohCallDrawBack);

INCLUDE_ASM(const s32, "code_00346258", gohSetDrawBack);

INCLUDE_ASM(const s32, "code_00346258", gohCallGetProcessData49);

INCLUDE_ASM(const s32, "code_00346258", ProcessStart_gohMenu);

INCLUDE_ASM(const s32, "code_00346258", ProcessEnd_gohMenu);

s32 ProcessCheck_gohMenu(void) {
    if (g_mainPID_gohMenu == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(g_mainPID_gohMenu) == 3 || dds3GetProcessStat(g_mainPID_gohMenu) == 0) {
        g_mainPID_gohMenu = NULL;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_00346258", gohSetEnableSelect);

INCLUDE_ASM(const s32, "code_00346258", gohCheckMovePrev);

INCLUDE_ASM(const s32, "code_00346258", gohSetDraw);

INCLUDE_ASM(const s32, "code_00346258", jmsGetSpriteHandle);

INCLUDE_ASM(const s32, "code_00346258", CheckModelLoad);

INCLUDE_ASM(const s32, "code_00346258", PosReset);

INCLUDE_ASM(const s32, "code_00346258", jmsModelLoad);

INCLUDE_ASM(const s32, "code_00346258", jmsModelFree);

INCLUDE_ASM(const s32, "code_00346258", jmsCallBitCheck);

INCLUDE_ASM(const s32, "code_00346258", jmsCheckBgModel);

INCLUDE_ASM(const s32, "code_00346258", jmsStartEtcProcess);

INCLUDE_ASM(const s32, "code_00346258", jmsStartEtcProcess_jmsInit);

INCLUDE_ASM(const s32, "code_00346258", jmsAddTalkMenu);

INCLUDE_ASM(const s32, "code_00346258", jmsBgmLoad);

INCLUDE_ASM(const s32, "code_00346258", jmsDestroy);

INCLUDE_ASM(const s32, "code_00346258", jmsLoad2ndProcess);

INCLUDE_ASM(const s32, "code_00346258", jmsEventScrProcess);

INCLUDE_ASM(const s32, "code_00346258", jmsJimusyoJmsMes);

INCLUDE_ASM(const s32, "code_00346258", Init_jmsInit);

INCLUDE_ASM(const s32, "code_00346258", jmsDestroy_jmsInit);

INCLUDE_ASM(const s32, "code_00346258", jmsJimusyoCalc);

INCLUDE_ASM(const s32, "code_00346258", jmsProcessEnd);

INCLUDE_ASM(const s32, "code_00346258", jmsProcessCheck);

INCLUDE_ASM(const s32, "code_00346258", jmsDebugProcessStart);

void jmsDebugProcessEnd(void) {
    jmsProcessEnd();
}

void jmsDebugProcessCheck(void) {
    jmsProcessCheck();
}

INCLUDE_ASM(const s32, "code_00346258", jmsScrCmd_JMS_MOTION);

INCLUDE_ASM(const s32, "code_00346258", jmsScrCmd_JMS_MOTION_jmsInit);

INCLUDE_ASM(const s32, "code_00346258", jmsScrCmd_JMS_GET_INDEX);

INCLUDE_ASM(const s32, "code_00346258", jmsScrCmd_JMS_EXIT);

INCLUDE_ASM(const s32, "code_00346258", jmsScrCmd_JMS_SET_MODE);

INCLUDE_ASM(const s32, "code_00346258", jmsCheckScrRun);

INCLUDE_ASM(const s32, "code_00346258", ScrControl_jmsMain);

INCLUDE_ASM(const s32, "code_00346258", jmsRootWaitCalcProcess);

dds3ProcessFunc_t TalkDrawProcess(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    DrawModel(work, -1);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t TalkUpdateProcess(dds3ProcessID_t *PID) {
    jmsData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00346258", RootInit);

INCLUDE_ASM(const s32, "code_00346258", RootDestroy);

INCLUDE_ASM(const s32, "code_00346258", RootCalcProcess);

INCLUDE_ASM(const s32, "code_00346258", RootDrawProcess);

dds3ProcessFunc_t RootUpdateProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00346258", nbUpdateEventProcessData);
