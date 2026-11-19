#include "common.h"
#include "dds3KernelCore.h"
#include "sbtlWork.h"
#include "sBtlCalc.h"
#include "sBtlDraw.h"
#include "frFont.h"
#include "sBtlMenu.h"
#include "code_r1_001CC1D0.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "sBtlPlayer.h"
#include "code_002F5CF0.h"
#include "sBtlSM.h"
#include "code_r1_002C8188.h"
#include "dds3ObjectBase.h"
#include "code_0027A1E8.h"
#include "sBtlCmndMenuNew.h"
#include "sBtlPanel.h"

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallSprintf7);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallDclose);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlLenDir);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallMemcpy);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlMotionMoveData);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFilename);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSetEffHitPara);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlDebugMDLHitDrawSub);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlDebugMDLHitDrawSub2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSetVisualPara);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlEffectCylinderData);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlMotionDataSave);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSousaProc);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_UnitPad);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSetSword);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlOpti);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSetDebugMenuProc);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFunc21);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlMainProcSub);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallSprintf2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_UnitPanelCursor);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlRefSbtl52);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlRefSbtl70);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_SetEncountPara);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_SetBattleMode);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallBtlGetPlayerData7);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_CheckLoadEncountData);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_Shutdown);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlLeaf96);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlLeaf88);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFunc16);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_DrawNormalList_Col);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_SetStatusflg);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_DrawONOFFList_Motion);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_DrawNormalList);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_DrawONOFFList);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlLeaf117);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallVsprintf);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGrAllocPacketBuff8);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallMemset29);

INCLUDE_ASM(const s32, "code_002F5CF0", evtStageTestRequestModelAsset);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetRunTimeCount);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallVsprintf2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlLeaf91);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlPfsBattleEfftime);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallBtlGetUnitHelperPos2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallSetOt5);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallMemset34);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallMemset32);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallMemset35);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallMemset28);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFunc18);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_DrawC);

INCLUDE_ASM(const s32, "code_002F5CF0", _3DCylinderDraw);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallSetOt4);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallMemAllocDirect7);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_DeleteHitList);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawHitList);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDraw3DSphere);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlGetMotionNumSub);

INCLUDE_ASM(const s32, "code_002F5CF0", sdfSetLocalMapCounterMarkerState);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGrAllocPacketBuff7);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGrAllocPacketBuff6);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlLeaf106);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlLeaf116);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlDebugProcSub);

sBtlMenu_t *sbtlFuncKatana(void) {
    if (ProcessCheck()) {
        return dds3GetProcessData(ProcessCheckData);
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawTargetCursorNakama);

void sbtlDrawTargetCursorEnemy(void) {
    struct SBTLUNIT_WORK *unit;

    sbtlSelectTargetCursor(0);
    sbtlUseItemProcSBtlPlayerSub();
    unit = sbtlSearchUnit_ID(wk_sbtl->tgtList, wk_sbtl->tgt_csr);
    if (unit) {
        sbtlUseItemProcSBtlPlayerSub2(unit, 0x80808080);
    }
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawTargetCharList);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlReturnAL);

INCLUDE_ASM(const s32, "code_002F5CF0", summonMenuResetMenu);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlCallEndSub);

SBTLMENU_ENABLETALK sbtlCheckEnableTalk(struct SBTLUNIT_WORK *unit) {
    BST bst;

    if (!unit) {
        return SBTLMENU_ENABLETALK_ERROR;
    }
    if (unit->field6C4 & 1) {
        return SBTLMENU_ENABLETALK_NG;
    }
    if (unit->field6E9) {
        return SBTLMENU_ENABLETALK_NG_BREAK;
    }
    bst = sbtlGetBadStatus(&unit->unit);
    if (bst != BST_BOYCOTT && bst != BST_MOUDOKU && bst != BST_STONE) {
        return SBTLMENU_ENABLETALK_NG_BST;
    }
    if (!sbtlCheckTalkSelectTarget(unit)) {
        return SBTLMENU_ENABLETALK_NG;
    }
    return SBTLMENU_ENABLETALK_OK;
}

void sbtlSetMenuImage(dds3ProcessID_t *PID) {
    itfMenuLinkSprite(PID, wk_sbtl->panel->hPanelSpr3);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_TITLE, 0, 0, 0, 0, 23, 24, NULL, sbtlSetMenuImageData3);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_PANEL, 322, 160, 128, 23, 17, 3, NULL, sbtlSetMenuImageData);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_SELECT, 322, 160, 128, 23, 17, 3, NULL, sbtlSetMenuImageData2);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_SCROLL, 322, 160, 3, 164, 172, 11, NULL, sbtlSetMenuImageData4);
    itfMenuSetImageColor(PID, ITFMENU_IMG_DAT_SCROLL, 200, 180, 80, 128);
}

dds3ProcessID_t *sbtlInitMenuPID(u8 a_max) {
    dds3ProcessID_t *PID = itfMenuProcessStart();
    s32 i;

    if (!PID) {
        return PID;
    }
    for (i = 0; i < a_max; i++) {
        itfMenuSetRootMenu(PID, i, sbtlData173, NULL);
    }
    itfGeneralMenuCreatePIDSub(PID, 3);
    itfGeneralMenuCreatePIDSub2(PID, 3);
    itfMenuSetFlag(PID, 2, 0);
    itfMenuSetFlag(PID, 0x10, 0);
    sbtlSetMenuImage(PID);
    itfMenuSetOt(PID);
    return PID;
}

/* NON_MATCHING: 33 of 60 words match.  */
#ifdef NON_MATCHING
void sbtlDrawFuncTalk(s32 a_id, s32 x, s32 y, s32 z, u32 fontColor, u8 alphaRate) {
    dds3ProcessID_t **data = dds3GetProcessData(ProcessCheckData);
    s32 ret;

    if (wk_sbtl->tgtList && a_id == itfMenuGetSelectID(data[4])) {
        wk_sbtl->tgt_csr = a_id;
        sbtlDrawTargetCursorEnemy();
        ret = sbtlCheckEnableTalk(sbtlSearchUnit_ID(wk_sbtl->tgtList, wk_sbtl->tgt_csr));
        switch (ret) {
        case SBTLMENU_ENABLETALK_OK:
            sbtlHelp(3, 17, alphaRate);
            break;
        case SBTLMENU_ENABLETALK_NG:
        case SBTLMENU_ENABLETALK_NG_BST:
            sbtlHelpCantUse(5, 15, alphaRate);
            break;
        case SBTLMENU_ENABLETALK_NG_BREAK:
            sbtlHelpCantUse(5, 14, alphaRate);
            break;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawFuncTalk);
#endif

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawAnalyzePlayer);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawFuncPartyDat);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData54);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncStart);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSkillChkUseUnit);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncMeirei);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncSummonExe);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawFuncSummon2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncSummon2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFunc23);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData44);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawFuncReturn);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData48);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncDecideItemTarget);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlItemChkUseUnit);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlItemChkUseUnitEx);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncDecideItemStock);

INCLUDE_ASM(const s32, "code_002F5CF0", DrawFuncSkl);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlItemChkUse);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncItem);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData42);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData52);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData47);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncEscape);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncDecideTalk);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncTalk);

void ChkEnableDecideEscape(sBtlMenu_t *wk) {
    s32 esc = sbtlEscapeCheck();
    s32 idx;
    s32 enable;

    if (esc == 1) {
        idx = 9;
    } else {
        idx = 8;
    }
    enable = esc != 2;
    itfMenuSetFont(wk->menuPID, 7, sbtlCallMenuClearMenuAllData[idx]);
    if (wk_sbtl->status3 & 0x200000) {
        enable = 0;
    }
    itfMenuSetFlagMenu(wk->menuPID, 7, ITFMENU_CB_GF_ENABLE_DECIDE, enable);
}

INCLUDE_ASM(const s32, "code_002F5CF0", TutorialMenuCtrl);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlChkEnableDecideMenu);

void Init_sbtlMenu(sBtlMenu_t *wk) {
    wk->menuPID = sbtlInitMenuPID(8);
    wk->listPID = sbtlInitMenuPID(32);
    wk->charPID = sbtlInitMenuPID(32);
    wk->nakamaPID = sbtlInitMenuPID(2);
    cmpCallMenuProcessCheck(wk->menuPID, 1, 0, 50, 0);
    cmpCallMenuProcessCheck(wk->charPID, 1, 0, 12, 0);
    cmpCallMenuProcessCheck(wk->nakamaPID, 1, 0, 12, 0);
    itfCallGetProcessData7(wk->charPID, 1, -87);
    itfCallGetProcessData7(wk->charPID, 2, -87);
    itfCallGetProcessData7(wk->nakamaPID, 1, -87);
    itfCallGetProcessData7(wk->nakamaPID, 2, -87);
    if (sbtlCheckTutorialBattle()) {
        sbtlFuncEscapeData |= 4;
    }
}

void sbtlCallMenuClearMenuAll(sBtlMenu_t *wk) {
    itfMenuClearMenuAll(wk->menuPID);
    if (wk->type == 1) {
        itfMenuSetRootMenu(wk->menuPID, 15, sbtlCallMenuClearMenuAllData[1], sbtlFuncStart);
    } else {
        itfMenuSetRootMenu(wk->menuPID, 1, sbtlCallMenuClearMenuAllData[2], sbtlFuncMeirei);
    }
    itfMenuSetRootMenu(wk->menuPID, 2, sbtlCallMenuClearMenuAllData[3], sbtlFunc23);
    itfMenuSetRootMenu(wk->menuPID, 3, sbtlCallMenuClearMenuAllData[4], sbtlCallGetProcessData48);
    if (wk->type == 0) {
        itfMenuSetRootMenu(wk->menuPID, 5, sbtlCallMenuClearMenuAllData[6], sbtlFuncItem);
    }
    itfMenuSetRootMenu(wk->menuPID, 6, sbtlCallMenuClearMenuAllData[7], sbtlCallGetProcessData47);
    itfMenuSetRootMenu(wk->menuPID, 7, sbtlCallMenuClearMenuAllData[8], sbtlFuncEscape);
    itfMenuSetRootMenu(wk->menuPID, 14, sbtlCallMenuClearMenuAllData[10], sbtlFuncTalk);
}

void sbtlCallProcEnd(sBtlMenu_t *wk) {
    itfMenuProcessEnd(wk->menuPID);
    itfMenuProcessEnd(wk->listPID);
    itfMenuProcessEnd(wk->charPID);
    itfMenuProcessEnd(wk->nakamaPID);
}

/* NON_MATCHING: 16 of 34 words match.  */
#ifdef NON_MATCHING
u8 sbtlCheckProcEnd(sBtlMenu_t *wk) {
    u8 cnt = itfMenuProcessCheck(wk->menuPID) != 0;

    if (itfMenuProcessCheck(wk->listPID)) {
        cnt++;
    }
    if (itfMenuProcessCheck(wk->charPID)) {
        cnt++;
    }
    if (itfMenuProcessCheck(wk->nakamaPID)) {
        cnt++;
    }
    return cnt;
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCheckProcEnd);
#endif

void sbtlCallEnd(sBtlMenu_t *wk) {
    if (wk_sbtl->tgtList) {
        sbtlDeleteUnitList(wk_sbtl->tgtList);
        wk_sbtl->tgtList = NULL;
    }
    if (wk->type == 0) {
        sbtlBtlCallEndSub();
        sbtlCancelMenuPara();
    } else if (wk->type == 1) {
        wk_sbtl->bBfrBtlEnd = 1;
        sbtlMenuClose();
        if (wk_sbtl->stageNo == 500 || wk_sbtl->stageNo == 567) {
            StopBattle();
        }
    }
    wk->step = 1;
    wk->flag &= 0xCD;
    if (wk_sbtl->flg2 & 2) {
        wk_sbtl->flg2 &= ~2;
        sbtlMenuClose();
    }
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlChkCallEnd);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncDecideItem_sbtlMenu);

s32 sbtlBtlDrawProcessSub(struct sBtlMenu_t *menu) {
}

s32 sbtlDrawProcess(dds3ProcessID_t *PID)
{
    sBtlMenu_t *menu = dds3GetProcessData(PID);

    sbtlBtlDrawProcessSub(menu);
    if (dds3ExistProcess(menu->drawPID)) {
        dds3KillProcess(menu->drawPID, 0);
        menu->drawPID = NULL;
    }
    sdfMemFreeDirect(menu->hMem);
    ProcessCheckData = NULL;
#ifdef FIX_BUGS
    return 0;
#endif
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawProcess_sbtlMenu);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallMemFreeDelayedDirect);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCampSubSpr);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlIsMenuStartScript);

INCLUDE_ASM(const s32, "code_002F5CF0", ProcessStart_sbtlMenu);

void IsStart(s32 type) {
    sBtlMenu_t *wk;

    if (ProcessCheck()) {
        wk = dds3GetProcessData(ProcessCheckData);
        wk->type = type;
        wk->flag |= 0x10;
        sbtlMenuOpen();
    }
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSetSummonMenuSub);

void ProcessStart(void) {
    sdfMemHandle_t *hMem;
    sBtlMenu_t *wk;

    hMem = sdfMemAllocDirect(sizeof(sBtlMenu_t));
    wk = sdfMemLock(hMem);
    memset(wk, 0, sizeof(sBtlMenu_t));
    wk->hMem = hMem;
    ProcessCheckData = dds3StartProcess(sbtlStrSbtlmenuMain, 0x3FE, 0, 0, sbtlFuncDecideItem_sbtlMenu, sbtlDrawProcess, wk);
    wk->drawPID = dds3StartProcess(sbtlStrSbtlmenuDraw, 0x2B15, 0, 0, sbtlDrawProcess_sbtlMenu, NULL, wk);
    wk->step = 0;
    sbtlFuncEscapeData = 0;
}

void sbtlProcessEnd(void) {
    sBtlMenu_t *wk;

    if (ProcessCheckData != NULL && dds3ExistProcess(ProcessCheckData)) {
        wk = dds3GetProcessData(ProcessCheckData);
        if (!(wk->flag & 1)) {
            wk->flag |= 1;
            wk->step = 15;
            wk->count = 0;
        }
    }
}

s32 ProcessCheck(void) {
    if (ProcessCheckData == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(ProcessCheckData) == 3 || dds3GetProcessStat(ProcessCheckData) == 0) {
        ProcessCheckData = NULL;
        return 0;
    }
    return 1;
}

SBTLMENU_NODE sbtlGetNode(void) {
    SBTLMENU_NODE node = SBTLMENU_NODE_NONE;
    sBtlMenu_t *wk = sbtlFuncKatana();
    u32 id;

    if (!wk) {
        return SBTLMENU_NODE_NONE;
    }
    if (!itfMenuProcessCheck(wk->menuPID)) {
        return SBTLMENU_NODE_NONE;
    }
    if (wk->step == 1 && sbtlGetStat() == SBTLMENU_NODE_ORDER) {
        return SBTLMENU_NODE_ORDER;
    }
    if (wk->step == 2) {
        node = SBTLMENU_NODE_ROOT;
    } else if ((u8)(wk->step - 3) < 12) {
        id = itfMenuGetSelectID(wk->menuPID);
        if (id < 16) {
            node = sbtlGetNodeData[id];
        }
    }
    return node;
}

INCLUDE_ASM(const s32, "code_002F5CF0", GetActionUnit);

INCLUDE_ASM(const s32, "code_002F5CF0", GetTargetUnit);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlFuncDecideItemSBtlMenuSub);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlFuncDecideItemSBtlMenuSub2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawTabPanel);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlDrawProcessSBtlMenuSub);

void sbtlInitAddMag(void) {
    gAddMagNum = 0;
    gAddMagNumDrawCnt = 0;
    gAddMagNumNow = 0;
    gAddMagNumUp = 0;
}

/* NON_MATCHING: 15 of 24 words match.  */
#ifdef NON_MATCHING
void SetAddMag(s32 num) {
    s32 diff;

    gAddMagNum += num;
    gAddMagNumDrawCnt = 100;
    diff = gAddMagNum - gAddMagNumNow;
    if (diff / 8 > 0) {
        gAddMagNumUp = diff / 8;
    } else {
        gAddMagNumUp = 1;
    }
    sbtlPlaySE(0x10020);
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", SetAddMag);
#endif

INCLUDE_ASM(const s32, "code_002F5CF0", DrawAddMag);

void sbtlGetIdling_HD(s32 a_x, s32 a_y, u8 a_alphaRate, s32 a_otNo, struct sdatUnitWork_t *unit) {
    if (unit != NULL) {
        itfIconDrawBadStatus(a_x, a_y, 0, a_alphaRate, sbtlGetBadStatus(unit), a_otNo);
    }
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlMoonOff);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBarCtrl);

INCLUDE_ASM(const s32, "code_002F5CF0", DrawStatusBar);

void btlLoadInterfaceSprite(void) {
    struct sprHandle_t *hSpr;

    memset(&sbtlData135, 0, sizeof(sbtlData135));
    hSpr = effLoadIndexedResource(btlLoadInterfaceSpriteStrBattlePanel, btlLoadInterfaceSpriteStrBattleInterfaceSpr, 0);
    sbtlData135.hSpr = hSpr;
    sbtlData135.field04 = -1;
}

s64 sbtlCallBtlCheckTutorialBattle3(void)
{
    return sbtlCheckTutorialBattle();
}

char *sbtlLeaf112(u8 n)
{
    char *name = NULL;

    if (n < 3) {
        name = sbtlData81[n];
    }
    return name;
}

char *sbtlGetPlayerName(void)
{
    if (sbtlCallBtlCheckTutorialBattle3()) {
        return sbtlLeaf112(2);
    }
    return sbtlLeaf112(1);
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlLeaf113);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlEnableSceneGroupAdvancementSub);

/* NON_MATCHING: 1 of 23 words match.  */
#ifdef NON_MATCHING
void sbtlInitSBtlSub2(void) {
    sbtlData121 = 1;
    sbtlData123 = -1;
    sbtlRefSbtl57Data = 0;
    sbtlRefSbtl61Data = -1;
    sbtlRefSbtl59Data = -1;
    sbtlCallBtlSearchUnitID37Data2 = -1;
    sbtlCallBtlSpriteData = -1;
    sbtlCallBtlSpriteData2 = 0;
    sbtlEscapeGaugeData = -1;
    sbtlCallCheckDying2Data = -1;
    sbtlCallCheckDying3Data = -1;
    sbtlCallCheckDying2Data2 = -1;
    sbtlCallCheckDying3Data2 = -1;
    sbtlCallCheckDying2Data3 = -1;
    sbtlCallCheckDying3Data3 = -1;
    sbtlCallCheckDying3Data4 = -1;
    sbtlCallEventBattleStopChack2Data2 = -1;
    Init_sbtlDrawData = -1;
    sbtlData122 = -1;
    Init_sbtlDrawData2 = -1;
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlInitSBtlSub2);
#endif

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlInitDevil);

char *sbtlGetRaceName(s32 a_raceID) {
    return sbtlData94[a_raceID + 34];
}

void sbtlSpriteEx(s32 a_x, s32 a_y, u8 a_alpha, s32 a_sprNo, DDS3_OT a_ot)
{
    itfDrawMiscSpriteAlpha(a_x, a_y, 0, a_alpha, wk_sbtl->panel->hPanelSpr3, a_sprNo, a_ot);
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSprite);

void sbtlNum(s32 a_x, s32 a_y, u32 a_color, u8 a_alpha, s32 a_num, DDS3_OT a_ot) {
    s32 i = 0;
    s32 figure = itfGetFigure(a_num);
    s32 sprNo;
    s32 j;

    for (; i < figure; i++) {
        sprNo = itfGetFigureNum(a_num, figure - i);
        if (sprNo >= 0) {
            sprNo += 70;
            for (j = 0; j < 4; j++) {
                wk_sbtl->panel->hPanelSpr3->workEntries[sprNo].geometry.cornerColors[j] = a_color;
            }
            sbtlSpriteEx(a_x, a_y, a_alpha, sprNo, a_ot);
            a_x += 9;
        }
    }
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlNum3);

/* NON_MATCHING: 2 of 21 words match.  */
#ifdef NON_MATCHING
void sbtlDrawNum4(s32 a_x, s32 a_y, u32 a_rgba, u8 a_alphaRate, s32 a_num, DDS3_OT a_ot, u32 rinkaku_rgba, f32 scale) {
    s32 ofs1;
    s32 ofs2;
    s32 ofs;

    if (a_num >= 10) {
        ofs1 = 36;
    } else {
        ofs1 = 16;
    }
    if (a_num < 100) {
        ofs2 = ofs1;
    } else {
        ofs2 = 48;
    }
    if (a_num < 1000) {
        ofs = ofs2;
    } else {
        ofs = 60;
    }
    sbtlNum3(a_x - ofs, a_y - 12, a_rgba, a_alphaRate, a_num, a_ot, rinkaku_rgba, scale);
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawNum4);
#endif

s32 sbtlCallGetSmallFontFRQ4(s32 x, s32 y, u8 bRight, u32 color, u8 alpha, s32 ot, void *str) {
    frFontHandle_t *frq;
    s32 width;

    if (str == NULL) {
        return -1;
    }
    frq = frwGetSmallFontFRQ2(x << 4, y << 3, 0, (color & ~0xFF) + (u8)((color & 0xFF) * alpha / 255), str, 0);
    frSetFontSpace(frq, 0);
    width = frq->advance;
    if (bRight) {
        frSetDrawPosition(frq, (x - width) << 4, y << 3);
    }
    frDrawFontOt(frq, 1, ot);
    frDestroyFontRequestAfter(frq);
    return width;
}

s32 sbtlCallGetSmallFontFRQ3(s32 x, s32 y, s32 w, u32 color, u8 alpha, s32 ot, void *str) {
    frFontHandle_t *frq;
    s32 width;

    if (str == NULL) {
        return -1;
    }
    frq = frwGetSmallFontFRQ2(x << 4, y << 3, 0, (color & ~0xFF) + (u8)((color & 0xFF) * alpha / 255), str, 0);
    frSetFontSpace(frq, 0);
    width = frq->advance;
    frSetDrawPosition(frq, (x << 4) + (((w - width) >> 1) << 4), y << 3);
    frDrawFontOt(frq, 1, ot);
    frDestroyFontRequestAfter(frq);
    return width;
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlNumFont);

void sbtlDrawRedGg(s32 a_x, s32 a_y, u8 a_alphaRate, s32 a_sprNo, DDS3_OT a_ot)
{
    itfIconDrawSprite(a_x, a_y, 0, a_alphaRate, a_sprNo, a_ot);
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallCheckDying);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallSprintf3);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlPlayerData);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSummonDevilAshimotoReset);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSummonDevilAshimoto);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSummonDevilData_Sub);

void sbtlSummonDevilData(void) {
    struct SBTLUNIT_WORK *nakama[2];
    s32 i;

    if (wk_sbtl) {
        for (i = 0; i < 2; i++) {
            if (wk_sbtl->apMarkerUnit[i]) {
                dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnit[i], 1);
                dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnitR[i], 1);
            }
        }
    }
    nakama[0] = sbtlGetNakama(0);
    nakama[1] = sbtlGetNakama(1);
    if (nakama[0]) {
        sbtlSummonDevilData_Sub(nakama[0], 0);
    }
    if (nakama[1]) {
        sbtlSummonDevilData_Sub(nakama[1], 1);
    }
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlEnemyData);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallBtlGetRaceName);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlAnalyzeBackEx);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlAnalyzeBack);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlAnalyzeLvEx);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlAnalyzeLv);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallCalcSpriteWidth);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlRefSbtl53);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallDrawMiscSpriteAlpha);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSkillData);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallBtlChkRepeat);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlAnalyzeAisyo);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlAnalyzeTalkSkill);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlAnalyzeMeisi);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlAnalyzePlayer);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallBtlAnalyzeBack);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallBtlAnalyzeBack2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlAnalyzeBoss);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallSetFontDrawOT);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlTargetCursor);

void sbtlResetTargetCursor(void) {
    sbtlRefSbtl57Data = 0;
    sbtlTargetCursorData6 = 1;
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSelectTargetCursorSub);

/* NON_MATCHING: 9 of 27 words match.  */
#ifdef NON_MATCHING
s32 sbtlSelectTargetCursor(s8 bDrawName) {
    if (!wk_sbtl->tgtList) {
        return 0;
    }
    if (wk_sbtl->tgtList->count < 1) {
        return 0;
    }
    if (!(wk_sbtl->flg & 0x80000)) {
        return 0;
    }
    return sbtlSelectTargetCursorSub(sbtlSearchUnit_ID(wk_sbtl->tgtList, wk_sbtl->tgt_csr), bDrawName);
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSelectTargetCursor);
#endif

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlHelpEx);

void sbtlHelp(s32 a_type, s32 a_num, u8 a_alphaRate) {
    sbtlHelpEx(a_type, a_num, wk_sbtl->panel->hPanelSpr3, a_alphaRate);
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlHelpCantUseEx);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlHelpCantUse);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlEscapeGauge);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlResetEscapeGauge);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDamagePanelE);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDamagePanelN);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDamagePanelR);

INCLUDE_ASM(const s32, "code_002F5CF0", DrawTime);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData45);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData55);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncDecideItem);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData49);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData57);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawFuncStockDat);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawFuncSummon);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData58);

INCLUDE_ASM(const s32, "code_002F5CF0", FuncDecideBullet);

INCLUDE_ASM(const s32, "code_002F5CF0", FuncDecideKatana);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallStockListRecreate);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData41);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFunc24);

INCLUDE_ASM(const s32, "code_002F5CF0", cmpStockListRecreate);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFunc19);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData51);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData50);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData46);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallStockListRecreate2);

INCLUDE_ASM(const s32, "code_002F5CF0", CallCheck);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData56);

INCLUDE_ASM(const s32, "code_002F5CF0", awdMenuMakeList_itf);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallMenuLinkSprite);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSbtlmenubfrbtlMain);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallExistProcess2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessStat2);

void fldReleaseTitleMiniTexture(void) {
    if (effQueuedResourceNameRecord7 != NULL) {
        sbtlDeleteUnitList(effQueuedResourceNameRecord7);
        effQueuedResourceNameRecord7 = NULL;
    }
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallFileOpen2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallCreateEffPartsList);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallPartsCreateFinish2);
