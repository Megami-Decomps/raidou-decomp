#include "common.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "code_r1_001CC1D0.h"
#include "gohMain.h"
#include "gohDraw.h"
#include "code_0032F2B8.h"
#include "gohInit.h"
#include "fclMisc.h"

INCLUDE_ASM(const s32, "code_00336E90", fldCallMenuProcessCheck);

INCLUDE_ASM(const s32, "code_00336E90", fldFunc28);

INCLUDE_ASM(const s32, "code_00336E90", fldFunc29);

INCLUDE_ASM(const s32, "code_00336E90", fldDreamNums);

INCLUDE_ASM(const s32, "code_00336E90", fldCallMenuCheckWait);

INCLUDE_ASM(const s32, "code_00336E90", fldCallMenuProcessStart);

INCLUDE_ASM(const s32, "code_00336E90", gohCallMenuProcessStart);

INCLUDE_ASM(const s32, "code_00336E90", gohCallDrawFontEx2);

INCLUDE_ASM(const s32, "code_00336E90", gohDebugMenu);

INCLUDE_ASM(const s32, "code_00336E90", gohCallMemFreeDirect);

INCLUDE_ASM(const s32, "code_00336E90", gohShopDebug);

INCLUDE_ASM(const s32, "code_00336E90", gohCallExistProcess);

s32 gohCallGetProcessStat(void) {
    if (gohCallGetProcessStatData == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(gohCallGetProcessStatData) == 3 || dds3GetProcessStat(gohCallGetProcessStatData) == 0) {
        gohCallGetProcessStatData = NULL;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_00336E90", gohGetSpriteHandle);

INCLUDE_ASM(const s32, "code_00336E90", gohCheckModelLoad);

INCLUDE_ASM(const s32, "code_00336E90", ModelLoad_gohInit);

INCLUDE_ASM(const s32, "code_00336E90", ModelFree);

INCLUDE_ASM(const s32, "code_00336E90", gohTableLoad);

INCLUDE_ASM(const s32, "code_00336E90", gohCallMemFreeDirect2);

INCLUDE_ASM(const s32, "code_00336E90", gohStartEtcProcess);

INCLUDE_ASM(const s32, "code_00336E90", gohLoadBgModel);

INCLUDE_ASM(const s32, "code_00336E90", gohCheckLoadBgModel);

INCLUDE_ASM(const s32, "code_00336E90", gohFreeBgModel);

INCLUDE_ASM(const s32, "code_00336E90", gohBgmLoad);

INCLUDE_ASM(const s32, "code_00336E90", gohBgmPlay);

INCLUDE_ASM(const s32, "code_00336E90", gohBgmStop);

INCLUDE_ASM(const s32, "code_00336E90", gohLoad2ndProcess);

INCLUDE_ASM(const s32, "code_00336E90", Destroy_gohInit);

INCLUDE_RODATA(const s32, "code_00336E90", campDrawProcName2);

INCLUDE_RODATA(const s32, "code_00336E90", campUpdateProcName2);

INCLUDE_ASM(const s32, "code_00336E90", gohLoad1stProcess);

INCLUDE_ASM(const s32, "code_00336E90", sdfCreateRuntimeTask);

INCLUDE_ASM(const s32, "code_00336E90", sdfGohmadenDraw);

INCLUDE_ASM(const s32, "code_00336E90", campGohmadenCalc);

INCLUDE_ASM(const s32, "code_00336E90", campKillProcesses);

INCLUDE_ASM(const s32, "code_00336E90", gohProcessCheck);

INCLUDE_ASM(const s32, "code_00336E90", gohFunc);

void gohLeaf22(void) {
    campKillProcesses();
}

void gohLeaf23(void) {
    gohProcessCheck();
}

INCLUDE_ASM(const s32, "code_00336E90", gohScrCmd_RenkenSpirit);

INCLUDE_ASM(const s32, "code_00336E90", fldFacilityMistDraw);

void gohStopSequence(void) {
    campKillProcesses();
}

INCLUDE_ASM(const s32, "code_00336E90", gohCheckSequence);

INCLUDE_ASM(const s32, "code_00336E90", gohCallGetIntPara);

INCLUDE_ASM(const s32, "code_00336E90", gohCheckScrRun);

INCLUDE_ASM(const s32, "code_00336E90", ScrControl);

INCLUDE_ASM(const s32, "code_00336E90", gohFunc2);

INCLUDE_ASM(const s32, "code_00336E90", gohCallGetStockDevilNums);

INCLUDE_ASM(const s32, "code_00336E90", gohCallChkCommonInput);

INCLUDE_ASM(const s32, "code_00336E90", gohFastTravelSetupSub);

INCLUDE_ASM(const s32, "code_00336E90", gohRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_00336E90", gohCallGetActiveScene);

INCLUDE_ASM(const s32, "code_00336E90", gohStopStatusDraw);

INCLUDE_ASM(const s32, "code_00336E90", DlcDevilSelCheck);

INCLUDE_ASM(const s32, "code_00336E90", gohCreateSwordList);

INCLUDE_ASM(const s32, "code_00336E90", gohCreateRecoverList);

INCLUDE_ASM(const s32, "code_00336E90", gohCallBitCheck);

INCLUDE_ASM(const s32, "code_00336E90", gohRootWaitCalcProcess);

dds3ProcessFunc_t gohRootWaitDrawProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    gohModel(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohRootWaitUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00336E90", gohRootInit);

INCLUDE_ASM(const s32, "code_00336E90", gohRootDestroy);

INCLUDE_RODATA(const s32, "code_00336E90", cmpCalcProcName2);

INCLUDE_ASM(const s32, "code_00336E90", gohRootCalcProcess);

dds3ProcessFunc_t gohRootDrawProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    gohModel(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohRootUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    fclUpdateMessage();
    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00336E90", gohCallGetProcessData);

INCLUDE_ASM(const s32, "code_00336E90", gohBakNmlCalcProcess);

INCLUDE_ASM(const s32, "code_00336E90", gohCallGetProcessData2);

dds3ProcessFunc_t gohBakNmlDrawProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    gohModel(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t gohBakNmlUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00336E90", gohBakBakInit);

INCLUDE_ASM(const s32, "code_00336E90", gohBakBakDestroy);

INCLUDE_ASM(const s32, "code_00336E90", gohCallGetProcessData3);

INCLUDE_ASM(const s32, "code_00336E90", gohCallGetProcessData4);

dds3ProcessFunc_t gohCallGetProcessData23(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_00336E90", gohLeaf24);
