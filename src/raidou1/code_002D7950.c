#include "common.h"
#include "lmap.h"
#include "fileManager.h"
#include "dds3KernelDraw_0010A750.h"
#include "dds3KernelCore.h"
#include "ctn.h"
#include "code_002D7950.h"
#include "dds3WorldBasic.h"
#include "basic.h"
#include "dds3AreaBasic.h"
#include "dds3Basic.h"
#include "sdfMemory.h"
#include "sdk_libc.h"

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn3);

INCLUDE_ASM(const s32, "code_002D7950", nbFunc30);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn4);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn5);

INCLUDE_ASM(const s32, "code_002D7950", nbCheckAgariSub);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn6);

INCLUDE_ASM(const s32, "code_002D7950", AnalyzeTehaiNormal);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn7);

INCLUDE_ASM(const s32, "code_002D7950", nbFunc31);

INCLUDE_ASM(const s32, "code_002D7950", nbFunc32);

INCLUDE_ASM(const s32, "code_002D7950", nbChooseAgariSub);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn8);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn9);

INCLUDE_ASM(const s32, "code_002D7950", CheckRon_Reach);

INCLUDE_ASM(const s32, "code_002D7950", CalcFu);

INCLUDE_ASM(const s32, "code_002D7950", CalcTensu);

INCLUDE_ASM(const s32, "code_002D7950", CalcAgari);

INCLUDE_ASM(const s32, "code_002D7950", ChooseAgari);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn10);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn11);

INCLUDE_ASM(const s32, "code_002D7950", nbCalcFuSub);

INCLUDE_ASM(const s32, "code_002D7950", GetHaiColor);

INCLUDE_ASM(const s32, "code_002D7950", GetMentsuType);

INCLUDE_ASM(const s32, "code_002D7950", CheckPinfu);

INCLUDE_ASM(const s32, "code_002D7950", CheckTanyao);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn12);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn13);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn14);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn15);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn16);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn17);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn18);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn19);

INCLUDE_ASM(const s32, "code_002D7950", CheckChankan);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn20);

INCLUDE_ASM(const s32, "code_002D7950", CheckChanta);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn21);

INCLUDE_ASM(const s32, "code_002D7950", CheckIttsu);

INCLUDE_ASM(const s32, "code_002D7950", CheckSanshokujun);

INCLUDE_ASM(const s32, "code_002D7950", CheckSanshokupon);

INCLUDE_ASM(const s32, "code_002D7950", CheckToitoi);

INCLUDE_ASM(const s32, "code_002D7950", CheckSananko);

INCLUDE_ASM(const s32, "code_002D7950", CheckSankantsu);

INCLUDE_ASM(const s32, "code_002D7950", sdfRefWorkMjn);

INCLUDE_ASM(const s32, "code_002D7950", CheckHonitsu);

INCLUDE_ASM(const s32, "code_002D7950", CheckJunchan);

INCLUDE_ASM(const s32, "code_002D7950", sdfRefWorkMjn2);

INCLUDE_ASM(const s32, "code_002D7950", sdfCalcYakuSub);

INCLUDE_ASM(const s32, "code_002D7950", CheckTsuiso);

INCLUDE_ASM(const s32, "code_002D7950", sdfRefWorkMjn3);

INCLUDE_ASM(const s32, "code_002D7950", sdfRefWorkMjn4);

INCLUDE_ASM(const s32, "code_002D7950", sdfRefWorkMjn5);

INCLUDE_ASM(const s32, "code_002D7950", sdfRefWorkMjn6);

INCLUDE_ASM(const s32, "code_002D7950", sdfRefWorkMjn7);

INCLUDE_ASM(const s32, "code_002D7950", sdfRefWorkMjn8);

INCLUDE_ASM(const s32, "code_002D7950", sdfRefWorkMjn9);

INCLUDE_ASM(const s32, "code_002D7950", sdfRefWorkMjn10);

INCLUDE_ASM(const s32, "code_002D7950", sdfCalcYakuSub2);

INCLUDE_ASM(const s32, "code_002D7950", sdfCalcYakuSub3);

INCLUDE_ASM(const s32, "code_002D7950", sdfCalcYakuSub4);

INCLUDE_ASM(const s32, "code_002D7950", sdfRefWorkMjn11);

INCLUDE_ASM(const s32, "code_002D7950", CalcYaku);

INCLUDE_ASM(const s32, "code_002D7950", CalcYakuChitoi);

INCLUDE_ASM(const s32, "code_002D7950", CalcYakuKokushi);

INCLUDE_ASM(const s32, "code_002D7950", SetBestAnalysis);

INCLUDE_ASM(const s32, "code_002D7950", mnuRebuildScrollLists);

INCLUDE_ASM(const s32, "code_002D7950", CalcHaiValueTanki);

INCLUDE_ASM(const s32, "code_002D7950", CalcHaiValue);

INCLUDE_ASM(const s32, "code_002D7950", CalcHaiValueChitoi);

INCLUDE_ASM(const s32, "code_002D7950", AIChooseSutehai);

INCLUDE_ASM(const s32, "code_002D7950", UpdateTensu);

INCLUDE_ASM(const s32, "code_002D7950", Shukyoku);

INCLUDE_ASM(const s32, "code_002D7950", CheckYakuAri);

INCLUDE_ASM(const s32, "code_002D7950", ApplyAgari);

INCLUDE_ASM(const s32, "code_002D7950", CheckFuriten);

INCLUDE_ASM(const s32, "code_002D7950", ApplyChonbo);

INCLUDE_ASM(const s32, "code_002D7950", sdfCallCounterDrawGlyphAtGridCell);

INCLUDE_ASM(const s32, "code_002D7950", sdfCounterDrawGlyphAtGridCell);

INCLUDE_ASM(const s32, "code_002D7950", sdfCallDrawFontEx2);

INCLUDE_ASM(const s32, "code_002D7950", sdfCallGetSmallFontFRQ2);

INCLUDE_ASM(const s32, "code_002D7950", sdfCallSprintf);

INCLUDE_ASM(const s32, "code_002D7950", sdfCallSprintf2);

INCLUDE_ASM(const s32, "code_002D7950", sdfCallSprintf3);

INCLUDE_ASM(const s32, "code_002D7950", InitLight);

INCLUDE_ASM(const s32, "code_002D7950", InitCamera);

INCLUDE_ASM(const s32, "code_002D7950", SetCamera);

INCLUDE_ASM(const s32, "code_002D7950", DrawOya3D);

INCLUDE_ASM(const s32, "code_002D7950", DrawHai3DExCalc);

INCLUDE_ASM(const s32, "code_002D7950", DrawHai3DCalc);

INCLUDE_ASM(const s32, "code_002D7950", DrawHai3DIndex);

INCLUDE_ASM(const s32, "code_002D7950", pipeRefWorkMjn);

INCLUDE_ASM(const s32, "code_002D7950", pipeDrawSection);

INCLUDE_ASM(const s32, "code_002D7950", DrawTable);

INCLUDE_ASM(const s32, "code_002D7950", pipeCallDrawHai3DExCalc);

INCLUDE_ASM(const s32, "code_002D7950", pipeCallDrawHai3DExCalc2);

INCLUDE_ASM(const s32, "code_002D7950", pipeRefWorkMjn2);

INCLUDE_ASM(const s32, "code_002D7950", pipeCallDrawHai3DExCalc3);

INCLUDE_ASM(const s32, "code_002D7950", pipeCallLink);

INCLUDE_ASM(const s32, "code_002D7950", InitTenbouModel);

INCLUDE_ASM(const s32, "code_002D7950", DrawInfoUnit);

INCLUDE_ASM(const s32, "code_002D7950", InitOyaModel);

INCLUDE_ASM(const s32, "code_002D7950", nbCallLink);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn22);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn23);

INCLUDE_ASM(const s32, "code_002D7950", nbCallMemset2);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn24);

INCLUDE_ASM(const s32, "code_002D7950", nbRefWorkMjn25);

INCLUDE_ASM(const s32, "code_002D7950", nbPanelPressGaugeReset);

INCLUDE_ASM(const s32, "code_002D7950", Draw3D);

INCLUDE_ASM(const s32, "code_002D7950", StartCutin);

INCLUDE_ASM(const s32, "code_002D7950", MenuInit);

INCLUDE_ASM(const s32, "code_002D7950", MenuInit_mjn);

INCLUDE_ASM(const s32, "code_002D7950", nbCallSetOt);

INCLUDE_ASM(const s32, "code_002D7950", MenuLoop_mjn);

INCLUDE_ASM(const s32, "code_002D7950", MenuLoop);

INCLUDE_ASM(const s32, "code_002D7950", nbMahjonMenuProcess);

INCLUDE_ASM(const s32, "code_002D7950", nbCallMenuGetSelectID);

INCLUDE_ASM(const s32, "code_002D7950", ctnCallMenuGetSelectID);

INCLUDE_ASM(const s32, "code_002D7950", BridgeLoop);

INCLUDE_ASM(const s32, "code_002D7950", BridgeDestroy);

INCLUDE_ASM(const s32, "code_002D7950", ctnRefWorkCtn);

INCLUDE_ASM(const s32, "code_002D7950", ctnSimulateSetFlagsSub);

INCLUDE_ASM(const s32, "code_002D7950", ctnMoveFollowerSub);

INCLUDE_ASM(const s32, "code_002D7950", InitWork);

INCLUDE_ASM(const s32, "code_002D7950", ctnMapMapMAP);

INCLUDE_ASM(const s32, "code_002D7950", ctnLoadDataIDFloor);

INCLUDE_ASM(const s32, "code_002D7950", InitFromFile);

INCLUDE_ASM(const s32, "code_002D7950", LoadDataSub);

INCLUDE_ASM(const s32, "code_002D7950", ctnGetGrid);

INCLUDE_ASM(const s32, "code_002D7950", ctnMemFree);

INCLUDE_ASM(const s32, "code_002D7950", ctnFindAheadListSub);

INCLUDE_ASM(const s32, "code_002D7950", ctnSimulateSetFlagsSub2);

INCLUDE_ASM(const s32, "code_002D7950", KillCitizenFriend);

INCLUDE_ASM(const s32, "code_002D7950", KillCitizen);

INCLUDE_ASM(const s32, "code_002D7950", FindAheadList);

INCLUDE_ASM(const s32, "code_002D7950", FindAhead);

INCLUDE_ASM(const s32, "code_002D7950", FindHeadOn);

INCLUDE_ASM(const s32, "code_002D7950", SetHeadOn);

INCLUDE_ASM(const s32, "code_002D7950", SimulateSetFlags);

INCLUDE_ASM(const s32, "code_002D7950", ctnRefWorkCtn2);

INCLUDE_ASM(const s32, "code_002D7950", FreeGridBeside);

INCLUDE_ASM(const s32, "code_002D7950", ctnUpdateTargetVelocitySub);

INCLUDE_ASM(const s32, "code_002D7950", MoveFollower);

INCLUDE_ASM(const s32, "code_002D7950", ctnSetPrefSub);

INCLUDE_ASM(const s32, "code_002D7950", ctnUpdateTargetVelocitySub2);

INCLUDE_ASM(const s32, "code_002D7950", SetPref);

INCLUDE_ASM(const s32, "code_002D7950", ctnFunc);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/002E6B60.c:1, match */
void ctnFindAheadListSub2(struct ctnCitizen_t *pCitizen, struct ctnCitizen_t *pOther) {
    f32 leftWallDist;
    f32 rightWallDist;
    f32 width;
    f32 left;
    f32 right;
    f32 start;
    f32 end;

    ctnFunc4(pCitizen, pOther, &leftWallDist, &rightWallDist);
    width = leftWallDist + rightWallDist;
    left = leftWallDist - 0.5f;
    right = leftWallDist + 0.5f;

    start = 0.0f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->aheadEvadeArray[0] = 0;
    }
    start = width / 6.0f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->aheadEvadeArray[1] = 0;
    }
    start = width / 3.0f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->aheadEvadeArray[2] = 0;
    }
    start = width * 5.0f / 12.0f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->aheadEvadeArray[3] = 0;
    }
    start = width * 0.5f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->aheadEvadeArray[4] = 0;
    }
    start = width / 6.0f + width * 0.5f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->aheadEvadeArray[5] = 0;
    }
    start = width / 3.0f + width * 0.5f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->aheadEvadeArray[6] = 0;
    }
}

INCLUDE_ASM(const s32, "code_002D7950", ctnRefWorkCtn3);

INCLUDE_ASM(const s32, "code_002D7950", ctnRefWorkCtn4);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/002E7108.c:1, match */
void ctnFunc2(struct ctnCitizen_t *pCitizen) {
    f32 leftWallDist;
    f32 rightWallDist;
    f32 width;
    f32 left;
    f32 right;
    f32 start;
    f32 end;

    ctnRefWorkCtn8(pCitizen, &leftWallDist, &rightWallDist);
    width = leftWallDist + rightWallDist;
    left = leftWallDist - 0.5f;
    right = leftWallDist + 0.5f;

    start = 0.0f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->playerEvadeArray[0] = 0;
    }
    start = width / 6.0f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->playerEvadeArray[1] = 0;
    }
    start = width / 3.0f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->playerEvadeArray[2] = 0;
    }
    start = width * 5.0f / 12.0f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->playerEvadeArray[3] = 0;
    }
    start = width * 0.5f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->playerEvadeArray[4] = 0;
    }
    start = width / 6.0f + width * 0.5f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->playerEvadeArray[5] = 0;
    }
    start = width / 3.0f + width * 0.5f;
    end = start + width / 6.0f;
    if ((left <= start && start <= right) || (left <= end && end <= right) || (start <= left && right <= end)) {
        pCitizen->playerEvadeArray[6] = 0;
    }
}

INCLUDE_ASM(const s32, "code_002D7950", UpdateTargetVelocity);

INCLUDE_ASM(const s32, "code_002D7950", ctnRefWorkCtn5);

INCLUDE_ASM(const s32, "code_002D7950", ctnRefWorkCtn6);

INCLUDE_ASM(const s32, "code_002D7950", ctnKillCitizenSub);

INCLUDE_ASM(const s32, "code_002D7950", EnterBlock);

INCLUDE_ASM(const s32, "code_002D7950", UpdateCitizenVelocity);

INCLUDE_ASM(const s32, "code_002D7950", UpdateCitizenPos);

INCLUDE_ASM(const s32, "code_002D7950", UpdateCitizenRotate);

INCLUDE_ASM(const s32, "code_002D7950", GetClassVelocity);

INCLUDE_ASM(const s32, "code_002D7950", AddCitizen);

INCLUDE_ASM(const s32, "code_002D7950", ShootCitizen);

INCLUDE_ASM(const s32, "code_002D7950", ctnRefWorkCtn7);

INCLUDE_ASM(const s32, "code_002D7950", SetWallInfo);

INCLUDE_ASM(const s32, "code_002D7950", ctnLeaf2);

INCLUDE_ASM(const s32, "code_002D7950", ctnRefWorkCtn8);

INCLUDE_ASM(const s32, "code_002D7950", ctnFunc3);

INCLUDE_ASM(const s32, "code_002D7950", ctnFunc4);

INCLUDE_ASM(const s32, "code_002D7950", IsValidNewMobSystem);

INCLUDE_ASM(const s32, "code_002D7950", CheckExit);

INCLUDE_ASM(const s32, "code_002D7950", ctnUpdate);

INCLUDE_ASM(const s32, "code_002D7950", ctnRefWorkCtn9);

INCLUDE_ASM(const s32, "code_002D7950", ctnMoveFollowerSub2);

INCLUDE_ASM(const s32, "code_002D7950", ctnCallFptodp);

INCLUDE_ASM(const s32, "code_002D7950", ctnLeaf3);

INCLUDE_ASM(const s32, "code_002D7950", stfCallUpdate);

INCLUDE_ASM(const s32, "code_002D7950", stfCallMemFree);

INCLUDE_ASM(const s32, "code_002D7950", stfLeaf);

INCLUDE_ASM(const s32, "code_002D7950", stfCtnTestProcess);

s32 stfInit(void)
{
    dds3KillProcessName(stfProcName, 0);
    return 0;
}

void stfLeaf2(s32 x, s32 y, s32 width, s32 height, s32 color)
{
    evtSubmitDefaultDepthGradientRect(x, y, width, height, color, color, color, color);
}

INCLUDE_ASM(const s32, "code_002D7950", stfInterval);

INCLUDE_ASM(const s32, "code_002D7950", stfMapMapNeko);

INCLUDE_ASM(const s32, "code_002D7950", nekoCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_002D7950", nekoCallDevLoadFileEx);

INCLUDE_ASM(const s32, "code_002D7950", nekoCallMemFreeDelayedDirect);

INCLUDE_ASM(const s32, "code_002D7950", nekoUpdate);

INCLUDE_ASM(const s32, "code_002D7950", nekoCallSin);

INCLUDE_ASM(const s32, "code_002D7950", ChooseNextTarget);

INCLUDE_ASM(const s32, "code_002D7950", nekoUpdateSub);

INCLUDE_ASM(const s32, "code_002D7950", UpdateRotation);

INCLUDE_ASM(const s32, "code_002D7950", nekoRefWorkNeko);

INCLUDE_ASM(const s32, "code_002D7950", mapRefWorkNeko);

INCLUDE_ASM(const s32, "code_002D7950", mapRefWorkNeko2);

INCLUDE_ASM(const s32, "code_002D7950", mapRefWorkNeko3);

INCLUDE_ASM(const s32, "code_002D7950", UpdateTargetVelocity_neko);

INCLUDE_ASM(const s32, "code_002D7950", mapUpdateSub);

INCLUDE_ASM(const s32, "code_002D7950", mapMenuCreateMenuPID);

INCLUDE_ASM(const s32, "code_002D7950", ListDecideStation);

INCLUDE_ASM(const s32, "code_002D7950", mapCallGetProcessData2);

INCLUDE_ASM(const s32, "code_002D7950", ListChkUseStation);

INCLUDE_ASM(const s32, "code_002D7950", ListMakeListStation_lmap);

INCLUDE_ASM(const s32, "code_002D7950", ListMakeList);

INCLUDE_ASM(const s32, "code_002D7950", ListCallProcEnd);

INCLUDE_ASM(const s32, "code_002D7950", ListCheckProcEnd);

INCLUDE_ASM(const s32, "code_002D7950", ListProcess);

INCLUDE_ASM(const s32, "code_002D7950", ListDrawProcess);

INCLUDE_ASM(const s32, "code_002D7950", mapCallGetProcessData3);

INCLUDE_ASM(const s32, "code_002D7950", ListLinkSprite);

INCLUDE_ASM(const s32, "code_002D7950", mapCallGetProcessData4);

INCLUDE_ASM(const s32, "code_002D7950", ListStart);

INCLUDE_ASM(const s32, "code_002D7950", mapCallGetProcessData5);

s32 ListSetFlagFocus(void)
{
    return ((lmapLmpListWork_t *)dds3GetProcessData(g_mainPID_LmpList))->unk40;
}

INCLUDE_ASM(const s32, "code_002D7950", ListSetFlagFocus_lmap);

INCLUDE_ASM(const s32, "code_002D7950", ListSetFlagDisp);

INCLUDE_ASM(const s32, "code_002D7950", getListProcessSub);

INCLUDE_ASM(const s32, "code_002D7950", getListProcessSub2);

INCLUDE_ASM(const s32, "code_002D7950", getFunc);

INCLUDE_ASM(const s32, "code_002D7950", getListProcessStartSub);

void ListProcessStart(dds3ProcessID_t *PID)
{
    lmapLmpListWork_t *work = dds3GetProcessData(PID);

    getListProcessStartSub(work);
    if (dds3ExistProcess(work->drawPID)) {
        dds3KillProcess(work->drawPID, 0);
        work->drawPID = NULL;
    }
    sdfMemFreeDirect(work->hMem);
    g_mainPID_LmpList = NULL;
}

INCLUDE_ASM(const s32, "code_002D7950", getLeaf);

void ListProcessStart_lmap(void) {
    sdfMemHandle_t *hMem;
    lmapLmpListWork_t *wk;

    hMem = sdfMemAllocDirect(sizeof(lmapLmpListWork_t));
    wk = sdfMemGetPtr(hMem);
    memset(wk, 0, sizeof(lmapLmpListWork_t));
    wk->hMem = hMem;
    g_mainPID_LmpList = dds3StartProcess(ListProcessStart_lmapStrLmapListMain, 0x406, 0, 0, ListProcess, (dds3ProcessFunc_t)ListProcessStart, wk);
    wk->drawPID = dds3StartProcess(ListProcessStart_lmapStrLmapListDraw, 0x2B17, 0, 0, ListDrawProcess, NULL, wk);
    getLeaf(wk);
}

void getCallExistProcess(void)
{
    lmapLmpListWork_t *work;

    if (g_mainPID_LmpList != NULL && dds3ExistProcess(g_mainPID_LmpList)) {
        work = dds3GetProcessData(g_mainPID_LmpList);
        if (!(work->flags & 1)) {
            work->flags |= 1;
            work->step = 17;
            work->count = 0;
        }
    }
}

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/002EB610.c:1, match */
s32 getCallGetProcessStat(void) {
    if (g_mainPID_LmpList == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(g_mainPID_LmpList) == 3 || dds3GetProcessStat(g_mainPID_LmpList) == 0) {
        g_mainPID_LmpList = NULL;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002D7950", getCallStartGame);

INCLUDE_ASM(const s32, "code_002D7950", SoundSmgLoad);

INCLUDE_ASM(const s32, "code_002D7950", SoundSmgLoadChk);

s32 Start_lmap(s8 aFieldNo) {
    if (DDS3_GBWK2 == NULL) {
        return 0;
    }
    DDS3_GBWK2->flags |= 0x10;
    if (aFieldNo != 0) {
        DDS3_GBWK2->flags |= 0x20;
    } else {
        DDS3_GBWK2->flags &= ~0x20;
    }
    return 1;
}

s32 getLeaf3(s8 flag) {
    if (DDS3_GBWK2 == NULL) {
        return 0;
    }
    if (flag != 0) {
        DDS3_GBWK2->flags |= 0x80;
    } else {
        DDS3_GBWK2->flags &= ~0x80;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_002D7950", getID);

INCLUDE_ASM(const s32, "code_002D7950", ChkWarp);

INCLUDE_ASM(const s32, "code_002D7950", getLmpMainProc);

INCLUDE_ASM(const s32, "code_002D7950", LoadFile);

INCLUDE_ASM(const s32, "code_002D7950", Initialize_lmap);

INCLUDE_ASM(const s32, "code_002D7950", LoadFile_lmap);

INCLUDE_ASM(const s32, "code_002D7950", getCallInitializeLmap);

INCLUDE_ASM(const s32, "code_002D7950", DrawProcess_lmap);

INCLUDE_ASM(const s32, "code_002D7950", Release_lmap);

INCLUDE_ASM(const s32, "code_002D7950", Shutdown_lmap);

INCLUDE_ASM(const s32, "code_002D7950", getCallTexFreeDelayed);

INCLUDE_ASM(const s32, "code_002D7950", getCallSetOt);

void M_InitScene_lmap(void)
{
    dds3Basic_t *scene = dds3AddScene();

    dds3SetControlScene(scene);
    dds3SetActiveScene(scene);
    dds3AddAreaBasic(dds3AllocObjectId(), M_InitScene_lmapData);
}

/* body: matched in raidou @ 88f0d54: src/shared/event/evtStage.c:194, rework, same function 1.00 (words 13/13, calls 1.00, shape 1.00) */
void getCallGetControlScene(void)
{
    dds3Basic_t *node;

    node = (dds3Basic_t *)dds3GetControlScene();
    if (node != 0) {
        dds3DeleteScene(node);
    }
}

INCLUDE_ASM(const s32, "code_002D7950", M_InitCamera);

INCLUDE_ASM(const s32, "code_002D7950", GetCameraBasePos);

INCLUDE_ASM(const s32, "code_002D7950", M_UpdateCamera);

INCLUDE_ASM(const s32, "code_002D7950", M_MoveMap);

INCLUDE_ASM(const s32, "code_002D7950", M_MoveMap_lmap);

INCLUDE_ASM(const s32, "code_002D7950", SetPanel);

INCLUDE_ASM(const s32, "code_002D7950", datResetPanelSub);

INCLUDE_ASM(const s32, "code_002D7950", SetPanel_2);

INCLUDE_ASM(const s32, "code_002D7950", ResetPanel);

INCLUDE_ASM(const s32, "code_002D7950", CtrlActionWindow);

INCLUDE_ASM(const s32, "code_002D7950", SetMessage);

INCLUDE_ASM(const s32, "code_002D7950", datCallLMAPPOINTNO);

INCLUDE_ASM(const s32, "code_002D7950", datCallChrSetNpc);

INCLUDE_ASM(const s32, "code_002D7950", SetNpcDat);

INCLUDE_ASM(const s32, "code_002D7950", M_SetLockPointNpcCB);

INCLUDE_ASM(const s32, "code_002D7950", M_UpdateKaya);

INCLUDE_ASM(const s32, "code_002D7950", datAllClearCommMemory);

INCLUDE_ASM(const s32, "code_002D7950", datCallLMAPLINENO);

INCLUDE_ASM(const s32, "code_002D7950", itfCallSetOt5);

INCLUDE_ASM(const s32, "code_002D7950", itfFloatTo32s);

INCLUDE_ASM(const s32, "code_002D7950", DrawActWin);

INCLUDE_ASM(const s32, "code_002D7950", UpdateAutoMove);

INCLUDE_ASM(const s32, "code_002D7950", NpcDelete);

INCLUDE_ASM(const s32, "code_002D7950", NpcMoveRoute);

INCLUDE_ASM(const s32, "code_002D7950", dmLMAP_POINT_FLAG);

INCLUDE_ASM(const s32, "code_002D7950", dmLMAP_LINE_NO);

INCLUDE_ASM(const s32, "code_002D7950", MapDat_GetRouteDat);

INCLUDE_ASM(const s32, "code_002D7950", dmLMAP_POINT_NO);

INCLUDE_ASM(const s32, "code_002D7950", MapDat_GetPointID);

INCLUDE_ASM(const s32, "code_002D7950", dmCallLMAPPOINTNO);

INCLUDE_ASM(const s32, "code_002D7950", MapDat_GetPointXY);

INCLUDE_ASM(const s32, "code_002D7950", dmLoadFileLmapSub);

INCLUDE_ASM(const s32, "code_002D7950", dmCallBitCheck);

INCLUDE_ASM(const s32, "code_002D7950", Util_CalcLength);

INCLUDE_ASM(const s32, "code_002D7950", dmChrUpdatePadTrainSub);

INCLUDE_ASM(const s32, "code_002D7950", MdlDelete);

INCLUDE_ASM(const s32, "code_002D7950", MdlCreate);

INCLUDE_ASM(const s32, "code_002D7950", MdlSetDraw);

INCLUDE_ASM(const s32, "code_002D7950", MdlSetLink);

INCLUDE_ASM(const s32, "code_002D7950", MdlSetOfsY);

INCLUDE_ASM(const s32, "code_002D7950", MdlSetOfsLength);

INCLUDE_ASM(const s32, "code_002D7950", MdlSetTrain);

INCLUDE_ASM(const s32, "code_002D7950", MdlUpdate);

INCLUDE_ASM(const s32, "code_002D7950", campCallMemset);

/* NON_MATCHING: 6 of 11 words match.  */
#ifdef NON_MATCHING
s32 campDhIconIsReady(campDhIcon_t *pIcon)
{
    s32 result;

    result = 0;
    if (pIcon != NULL && pIcon->handle != 0) {
        result = (pIcon->flags & 7) == 7;
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "code_002D7950", campDhIconIsReady);
#endif

INCLUDE_ASM(const s32, "code_002D7950", campCallDhIconIsReady);

INCLUDE_ASM(const s32, "code_002D7950", campMMoveMapLmapSub);

INCLUDE_ASM(const s32, "code_002D7950", Chr_UpdateScaleModel);

INCLUDE_ASM(const s32, "code_002D7950", ChrSetModel);

INCLUDE_ASM(const s32, "code_002D7950", campCallMemset2);

INCLUDE_ASM(const s32, "code_002D7950", Chr_SetParam);

INCLUDE_ASM(const s32, "code_002D7950", _Chr_SetPlayer);

INCLUDE_ASM(const s32, "code_002D7950", Chr_SetPlayer);

INCLUDE_ASM(const s32, "code_002D7950", Chr_SetPlayerRoute);

INCLUDE_ASM(const s32, "code_002D7950", ChrSetNpc);

INCLUDE_ASM(const s32, "code_002D7950", Chr_UpdateLineData);

INCLUDE_ASM(const s32, "code_002D7950", ChrCalcLineVector);

INCLUDE_ASM(const s32, "code_002D7950", checkCallAtan2);

INCLUDE_ASM(const s32, "code_002D7950", checkCallLMAPPOINTFLAG);

INCLUDE_ASM(const s32, "code_002D7950", checkCallLMAPPOINTFLAG2);

INCLUDE_ASM(const s32, "code_002D7950", Chr_CalcPosition);

INCLUDE_ASM(const s32, "code_002D7950", Chr_UpdatePosFreePad);

INCLUDE_ASM(const s32, "code_002D7950", check_point_line);

INCLUDE_ASM(const s32, "code_002D7950", check_cross_line2);

INCLUDE_ASM(const s32, "code_002D7950", Chr_UpdatePosShipPad);

INCLUDE_ASM(const s32, "code_002D7950", Chr_UpdatePosition);

INCLUDE_ASM(const s32, "code_002D7950", Chr_UpdatePadRandom);

INCLUDE_ASM(const s32, "code_002D7950", ChrRouteCalcPadParam);

INCLUDE_ASM(const s32, "code_002D7950", _Chr_UpdatePadRoute);

INCLUDE_ASM(const s32, "code_002D7950", Chr_UpdatePadRoute);

INCLUDE_ASM(const s32, "code_002D7950", Chr_UpdatePadTrain);

INCLUDE_ASM(const s32, "code_002D7950", _Chr_GetExLineDat);

INCLUDE_ASM(const s32, "code_002D7950", Chr_GetExLineDat);
