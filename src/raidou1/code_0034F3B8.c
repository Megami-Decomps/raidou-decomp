#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "event.h"
#include "scr.h"
#include "dds3SceneBasic.h"
#include "dds3KernelCore.h"
#include "code_r1_002C8188.h"

#include "rstInit.h"
#include "cmpMain.h"
#include "code_0034F3B8.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "code_r1_001CC1D0.h"
#include "sdfChip.h"
#include "sdfDifficulty.h"

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallGetProcessData2);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallGetProcessData3);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsStartSub);

INCLUDE_ASM(const s32, "code_0034F3B8", CheckFlagTGS);

INCLUDE_ASM(const s32, "code_0034F3B8", SetRootMenu);

INCLUDE_ASM(const s32, "code_0034F3B8", SetMenu);

INCLUDE_ASM(const s32, "code_0034F3B8", SetFlagMenu);

INCLUDE_ASM(const s32, "code_0034F3B8", CheckCyousaFlag);

INCLUDE_ASM(const s32, "code_0034F3B8", SetCommand_jmsMenu);

INCLUDE_ASM(const s32, "code_0034F3B8", CallProcEnd);

INCLUDE_ASM(const s32, "code_0034F3B8", CheckProcEnd);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallGetProcessData4);

INCLUDE_ASM(const s32, "code_0034F3B8", Release_jmsMenu);

void Shutdown(dds3ProcessID_t *PID)
{
    jmsInputPanelWork_t *work = dds3GetProcessData(PID);

    Release_jmsMenu(work);
    if (dds3ExistProcess(work->drawPID)) {
        dds3KillProcess(work->drawPID, 0);
    }
    work->drawPID = NULL;
    sdfMemFreeDirect(work->hMem);
    inputpanel_id = NULL;
}

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallGetProcessData5);

s32 jmsCallGetProcessData6(void)
{
    return ((jmsInputPanelWork_t *)dds3GetProcessData(inputpanel_id))->unk14;
}

INCLUDE_ASM(const s32, "code_0034F3B8", jmsStart);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsSetFocus);

INCLUDE_ASM(const s32, "code_0034F3B8", ProcessStart_jmsMenu);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsMenuMain);

void ProcessEnd(void)
{
    jmsInputPanelWork_t *work;

    if (inputpanel_id != NULL && dds3ExistProcess(inputpanel_id)) {
        work = dds3GetProcessData(inputpanel_id);
        if (!(work->flags & 1)) {
            work->flags |= 1;
            work->step = 1;
            work->count = 0;
        }
    }
}

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/003503D8.c:1, match */
s32 ProcessCheck_jmsMenu(void) {
    if (inputpanel_id == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(inputpanel_id) == 3 || dds3GetProcessStat(inputpanel_id) == 0) {
        inputpanel_id = NULL;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0034F3B8", jmsDisableSelect);

INCLUDE_ASM(const s32, "code_0034F3B8", SearchBasicFromNameIndex_evtEventViewer);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallBitCheck2);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCheckMovePrev);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsGetCursorPosID);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallGlobalWorkUserInit2);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallGetProcessData17);

INCLUDE_ASM(const s32, "code_0034F3B8", cmpCallGetProcessData36);

cmpMemWork_t *cmpCallMemAllocDirect(void)
{
    struct sdfMemHandle_t *handle = sdfMemAllocDirect(sizeof(cmpMemWork_t));
    cmpMemWork_t *work = sdfMemLock(handle);

    memset(work, 0, sizeof(cmpMemWork_t));
    work->handle = handle;
    return work;
}

INCLUDE_ASM(const s32, "code_0034F3B8", cmpStartProcessPanelEff);

s32 cmpStorySelectDebug(void)
{
    return dds3KillProcessName(cmpStrStorySelectDebugProc, 0);
}

/* body: matched in raidou @ 88f0d54: src/shared/code_r1_0019D3A8.c:827, match, same function 1.00 (words 9/9, calls 1.00, shape 1.00) */
s32 rstStorySelectDebug(void) {
    return dds3SearchProcessName(cmpStrStorySelectDebugProc) != 0;
}

INCLUDE_ASM(const s32, "code_0034F3B8", rstLoadMapResource);

INCLUDE_ASM(const s32, "code_0034F3B8", rstMapReturnChkXFade);

INCLUDE_ASM(const s32, "code_0034F3B8", rstLeaf13);

rstBG_t *InitBgCore(sprHandle_t *pSpr) {
    rstBG_t *bg = sdfChipAllocDirect(sizeof(rstBG_t));
    memset(bg, 0, sizeof(rstBG_t));
    bg->pSpr = pSpr;
    return bg;
}

rstBG_t *InitBG(void) {
    return InitBgCore(effLoadIndexedResource(rstStrResult, rstStrRsHaikeiSpr, 0));
}

void rstDestroyBG(rstBG_t *bg) {
    if (bg != NULL) {
        if (bg->pSpr != NULL) {
            effDestroyResourceSlotSet(bg->pSpr);
        }
        sdfChipFreeDirect(bg);
    }
}

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallBitCheck);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetRandA);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallBitCheck2);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetPlayerLevelUpUnit);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultDraw);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCheckTaiitsu);

INCLUDE_ASM(const s32, "code_0034F3B8", ProcessStart_rstInit);

/* body: matched in smt3 @ 9401f79eb: src/smt3/code_0022C578.c:963, rework, same function 1.00 (words 16/16, calls 1.00, shape 1.00) */
void ProcessEnd_rstInit(void)
{
    dds3KillProcessName(cmpCalcProcName, 0);
    dds3KillProcessName(campDrawProcName, 0);
    dds3KillProcessName(campUpdateProcName, 0);
}

/* body: matched in raidou @ 88f0d54: src/shared/code_r1_0019D3A8.c:827, match, same function 1.00 (words 9/9, calls 1.00, shape 1.00) */
s32 ProcessCheck_rstInit(void) {
    return dds3SearchProcessName(cmpCalcProcName) != 0;
}

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultStartWaitProcess);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultLoadWaitProcess);

void rstSetReward(const rstWork_t *pReward) {
    if (pReward != NULL) {
        rstWork = *pReward;
        SDF_DIFFICULTY_REWARD(&rstWork);
        rstData4 |= 1;
    }
}

INCLUDE_ASM(const s32, "code_0034F3B8", rstSetEncountDlcID);

INCLUDE_ASM(const s32, "code_0034F3B8", rstGetGlobalEncountNo);

INCLUDE_ASM(const s32, "code_0034F3B8", rstSetFlag);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCheckFlag);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultStart);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultFadeCheck);

INCLUDE_ASM(const s32, "code_0034F3B8", rstGetPlayerUnit);

INCLUDE_ASM(const s32, "code_0034F3B8", rstGetGiftItem);

INCLUDE_ASM(const s32, "code_0034F3B8", rstGetGiftItemNums);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCheckRankUp);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCalcProcess);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallLoadMapResourceFunc);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetProcessData3);

dds3ProcessFunc_t rstDrawProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    rstResultResultDraw(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t rstCallGetProcessData37(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

/* body: matched in raidou @ 88f0d54: src/raidou1/code_002F5BD0.c:413, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 rstEndInit(dds3ProcessID_t *PID) {
    return 1;
}

INCLUDE_ASM(const s32, "code_0034F3B8", rstEndDestroy);

INCLUDE_ASM(const s32, "code_0034F3B8", rstEndCalcProcess);

dds3ProcessFunc_t rstEndDrawProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    rstResultResultDraw(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t rstEndUpdateProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_0034F3B8", rstBtlEndWaitInit);

/* body: matched in raidou @ 88f0d54: src/raidou1/code_002F5BD0.c:413, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 rstBtlEndWaitDestroy(dds3ProcessID_t *PID) {
    return 1;
}

INCLUDE_ASM(const s32, "code_0034F3B8", rstBtlEndWaitCalcProcess);

dds3ProcessFunc_t rstBtlEndWaitDrawProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    rstResultResultDraw(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t rstCallGetProcessData38(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

/* body: matched in raidou @ 88f0d54: src/raidou1/code_002F5BD0.c:413, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 rstLeaf16(dds3ProcessID_t *PID) {
    return 1;
}

/* body: matched in raidou @ 88f0d54: src/raidou1/code_002F5BD0.c:413, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 rstLeaf17(dds3ProcessID_t *PID) {
    return 1;
}

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetProcessData39);

dds3ProcessFunc_t rstCallGetProcessData40(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t rstCallGetProcessData41(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultCyuuseiGaugeSub);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultNakamaLevelSub);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultLevelUpAnimSub);

/* body: matched in raidou @ 88f0d54: src/raidou1/TitleMain.c:20, match, same function 1.00 (words 9/9, calls 1.00, shape 1.00) */
s32 rstCalcAlphaRateSub(s32 value, s32 low, s32 high) {
    if (value >= low && value < high) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0034F3B8", rstCalcAlphaRate);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCalcScale);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCalcPosition);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallCalcAlphaRate);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallBlendColor);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallBlendColor2);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultAddLevelUpAnimQue);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultLevelUpAnim);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultCyuuseiGauge);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultAddCyuuseiMaxAnimQue);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultCyuuseiMaxAnim);

INCLUDE_ASM(const s32, "code_0034F3B8", rstSummoner);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultNakamaLevel);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallBlendColor3);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultResultDraw);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallPlaySE3);

INCLUDE_ASM(const s32, "code_0034F3B8", rstMenuCursorCtrlSub);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallStartLevelUp);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallStartLevelUp2);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallProcEnd);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallProcessCheckCmpStatus);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallKillProcess);

INCLUDE_ASM(const s32, "code_0034F3B8", MenuCursorCtrl);

dds3ProcessID_t *rstGetGiftMsgPID(void)
{
    return ((rstMainWork_t *)dds3GetProcessData(g_mainPID3))->giftMsgPID;
}

void rstSetFlagPlayBgm(void)
{
    rstMainWork_t *work = dds3GetProcessData(g_mainPID3);

    work->flags |= 2;
}

s32 evtCheckPolygonMovieRun(void)
{
    rstMainWork_t *work = dds3GetProcessData(g_mainPID3);

    if (work->flags & 2) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0034F3B8", evtCallCheckRankUp);

INCLUDE_ASM(const s32, "code_0034F3B8", rstInit);

void rstCallGetProcessData42(dds3ProcessID_t *PID) {
    rstProcWork_t *wk = dds3GetProcessData(PID);

    rstCallKillProcess(wk);
    sdfMemFreeDirect(wk->hMem);
    FAKEMATCH_ASM_READ(wk);
}

INCLUDE_ASM(const s32, "code_0034F3B8", rstCmpItemGrowthStart);

INCLUDE_ASM(const s32, "code_0034F3B8", rstProcessEnd);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/00355E70.c:1, match */
s32 rstCallGetProcessStat(void) {
    if (g_mainPID3 == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(g_mainPID3) == 3 || dds3GetProcessStat(g_mainPID3) == 0) {
        g_mainPID3 = NULL;
        return 0;
    }
    return 1;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/sdf/sdfSystem.c:261, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void rstCloseStreamSub(void) {
}
