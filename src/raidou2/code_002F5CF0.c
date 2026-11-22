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
#include "jmsMisc.h"
#include "ps2asm.h"
#include "sbtlSub_0027ED08.h"
#include "event.h"
#include "sdfModel.h"
#include "sBtlAI.h"
#include "code_r1_002250F8.h"
#include "effPacket.h"
#include "dds3KernelDraw.h"
#include "dds3KernelDraw_0010A750.h"
#include "evtMoon.h"
#include "code_r1_0027CBE8.h"
#include "sBtlItem.h"
#include "sbtlActAI.h"
#include "EventBit.h"
#include "sBtlSkill.h"
#include "sdfMath.h"
#include "dds3Basic.h"
#include "mdlManager.h"
#include "code_r1_002CA3F0.h"
#include "fpu.h"
#include "dds3PadManager.h"
#include "itf.h"

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallSprintf7);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallDclose);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlCallSprintf7Table);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlLenDir);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallMemcpy);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlMotionMoveData);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFilename);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlSetEffHitParaTable);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSetEffHitPara);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlDebugMDLHitDrawSub);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlDebugMDLHitDrawSub2);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlBtlDebugMDLHitDrawSub2Table);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSetVisualPara);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlEffectCylinderData);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrCylinderHeight);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStr31Ff31Ff31Ff31Ff31Ff);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStr31Ff31Ff31Ff31Ff31Ff2);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrPfs0ModelBattleEfftxt03X);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSmodelBattleEfftxt03XTxt);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSbtlmodelSizeM);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSbtlmodelSizeS);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSbtlmodelSizeL);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSbtlmodelSizeLl);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlMotionDataSaveTable4);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrMotionFlgNone);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrMotionFlgSpeedOn);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrMotionFlgNcancel);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrMotionFlgSpeedOnMotion);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlMotionDataSaveTable5);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSbtlsdwFlgNone);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSbtlsdwFlgCircleSbtlsdw);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSbtlsdwFlgCircleSbtlsdw2);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSbtlsdwFlgCircleSbtlsdw3);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSbtlsdwFlgCircleSbtlsdw4);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSbtlsdwFlgHelper);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlMotionDataSaveTable6);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlMotionDataSave);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSousaProc);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_UnitPad);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSetSword);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlOpti);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSetDebugMenuProc);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlEffEditorProc);

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

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_DrawCursor);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_DrawCursor_OFFSET);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFunc16);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_DrawNormalList_Col);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_SetStatusflg);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_DrawONOFFList_Motion);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDebug_DrawNormalList);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlDebug_DrawONOFFList_MotionTable);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlDebug_DrawNormalListTable);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlDebug_DrawONOFFListTable);

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

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlAIProc);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlAIDraw);

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

s32 sbtlFuncDecideTalk(void) {
    sBtlMenu_t *wk;
    struct SBTLUNIT_WORK *unit;

    wk = dds3GetProcessData(ProcessCheckData);
    unit = wk_sbtl->tgtList->pUnitL[itfMenuGetSelectID(wk->charPID)].pUnit;
    sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (wk_sbtl->stageNo == 476 && unit->unit.devilId == 505) {
        wk_sbtl->boss_eventID = unit->field1A0;
        sbtlProcessStart(SBTLBOSSEVENT_DEAD, sbtlCheck(unit, 4, 0));
        unit->field6C0 = -2;
        unit->aiwk->bWait2 = 1;
        sbtlCallEnd(wk);
        sbtlSetMenuPara(SBTLPNL_MENU_NONE, 0, SKLID_INIT, 0, -1, 0);
        wk->endtype = 2;
        return 1;
    }
    wk_sbtl->field42C0 = 0;
    wk_sbtl->huumaTargetID = unit->field1A0;
    cmpSbtlkaiwaMain();
    wk->endtype = 2;
    sbtlCallEnd(wk);
    sbtlSetMenuPara(SBTLPNL_MENU_NONE, 0, SKLID_INIT, 0, -1, 0);
    return 1;
}

s32 sbtlFuncTalk(void) {
    sBtlMenu_t *wk;
    struct SBTLUNIT_WORK *unit;
    s32 i;

    wk = dds3GetProcessData(ProcessCheckData);
    if (wk_sbtl->tgtList) {
        sbtlDeleteUnitList(wk_sbtl->tgtList);
        wk_sbtl->tgtList = NULL;
    }
    wk_sbtl->tgtList = sbtlCreateTargetList_Talk(SBTLUNIT_SIDE_ENEMY);
    if (wk_sbtl->tgtList->count <= 0) {
        itfMenuStart(wk->menuPID);
        wk->step = 2;
        return 1;
    }
    itfMenuSetFlag(wk->charPID, 0x200, 0);
    itfMenuClearMenuAll(wk->charPID);
    for (i = 0; i < wk_sbtl->tgtList->count; i++) {
        unit = sbtlSearchUnit_ID(wk_sbtl->tgtList, i);
        if (unit->flg & 0x800) {
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, unit->field492);
        }
        itfMenuSetRootMenu(wk->charPID, i, g_itemNo[unit->unit.devilId], sbtlFuncDecideTalk);
        itfMenuSetDrawFunc(wk->charPID, i, sbtlDrawFuncTalk);
        itfMenuSetFlagMenu(wk->charPID, i, ITFMENU_CB_GF_ENABLE_DECIDE, sbtlCheckEnableTalk(unit) == SBTLMENU_ENABLETALK_OK);
    }
    wk_sbtl->flg |= 0x80000;
    sbtlResetTargetCursor();
    itfMenuStart(wk->charPID);
    wk->step = 4;
    return 1;
}

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

INCLUDE_RODATA(const s32, "code_002F5CF0", sdfSetLocalMapCounterMarkerStateTable);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrILeaveItToYou);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrDonWasteMp);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrHealSupportMe);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrUseMagicAttacks);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrListenToMyOrdersUse);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrNegotiate);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrCancelEscape2);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrBeginBattle);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrCommandMenu2);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrChangeDemon);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrOneShot);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrAutoRepeat);

s32 TutorialMenuCtrl(sBtlMenu_t *wk)
{
    s32 devilTbl[7] = { 0x1D9, 0x1ED, 0x1EE, 0x184, 0x185, 0x186, 0x187 };
    struct SBTLUNIT_WORK *unit;
    SBTL_STAGE_DATA *stage;
    s32 enable = 0;
    s32 i;
    u32 j;

    if (wk_sbtl->stageNo == 0x1DC) {
        for (i = 0; i < wk_sbtl->unitList->number; i++) {
            unit = wk_sbtl->unitList->pUnitL[i].pUnit;
            if (unit != NULL && unit->unit.devilId == 505 && unit->unit.now.hp <= unit->unit.now.maxhp / 100) {
                return 0;
            }
        }
    }
    if (wk_sbtl == NULL) {
        return 0;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit != NULL) {
            for (j = 0; j < 7; j++) {
                if (unit->unit.devilId == devilTbl[j]) {
                    return 0;
                }
            }
            if (unit->flg & 0x100000) {
                enable = 1;
            }
        }
    }
    stage = &sbtlData100[wk_sbtl->stageNo];
    if (!(stage->flags & 6) && !enable) {
        return 0;
    }
    itfMenuSetFlagMenu(wk->menuPID, 14, ITFMENU_CB_GF_ENABLE_DECIDE, 0);
    return 1;
}

/* NON_MATCHING: 134 of 190 words match.  */
#ifdef NON_MATCHING
void sbtlChkEnableDecideMenu(sBtlMenu_t *wk)
{
    struct SBTLUNIT_LISTWORK *list;
    struct SBTLUNIT_WORK *unit;
    s32 enable;
    s32 talk;
    BST bst;

    if (wk_sbtl->battleMode == 3) {
        itfMenuSetFlagMenu(wk->menuPID, 1, ITFMENU_CB_GF_ENABLE_DECIDE, 0);
        itfMenuSetFlagMenu(wk->menuPID, 2, ITFMENU_CB_GF_ENABLE_DECIDE, 0);
        itfMenuSetFlagMenu(wk->menuPID, 3, ITFMENU_CB_GF_ENABLE_DECIDE, 0);
        itfMenuSetFlagMenu(wk->menuPID, 6, ITFMENU_CB_GF_ENABLE_DECIDE, 0);
        itfMenuSetFlagMenu(wk->menuPID, 4, ITFMENU_CB_GF_ENABLE_DECIDE, 0);
        itfMenuSetFlagMenu(wk->menuPID, 14, ITFMENU_CB_GF_ENABLE_DECIDE, 0);
        itfMenuSetFlagMenu(wk->menuPID, 5, ITFMENU_CB_GF_ENABLE_DECIDE, wk_sbtl->itemList->num > 0);
        ChkEnableDecideEscape(wk);
        return;
    }
    enable = 1;
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (unit != NULL && CheckLiveTarget(unit)) {
        bst = sbtlGetBadStatus(&unit->unit);
        enable = !(bst == BST_BOUDO || bst == BST_MAHUU);
    }
    if (enable) {
        enable = sbtlIsStart(0) || sbtlIsStart(1);
    }
    itfMenuSetFlagMenu(wk->menuPID, 1, ITFMENU_CB_GF_ENABLE_DECIDE, enable);
    enable = sbtlCheckSummonUnit() != 1;
    if (!sbtlCheckSummonUnit_Dead_or_ALive(0) && !sbtlCheckSummonUnit_Dead_or_ALive(1)) {
        enable = 0;
    }
    itfMenuSetFlagMenu(wk->menuPID, 2, ITFMENU_CB_GF_ENABLE_DECIDE, enable);
    enable = wk_sbtl->member_num > 0;
    if (enable && sbtlCheckTutorialBattle()) {
        enable = 0;
    }
    itfMenuSetFlagMenu(wk->menuPID, 3, ITFMENU_CB_GF_ENABLE_DECIDE, enable);
    enable = 0;
    if (!(wk_sbtl->status3 & 0x100000)) {
        enable = wk_sbtl->itemList->num > 0;
    }
    itfMenuSetFlagMenu(wk->menuPID, 5, ITFMENU_CB_GF_ENABLE_DECIDE, enable);
    list = sbtlCreateTargetList(2);
    talk = (list != NULL ? list->count : 0) > 0;
    itfMenuSetFlagMenu(wk->menuPID, 6, ITFMENU_CB_GF_ENABLE_DECIDE, talk);
    ChkEnableDecideEscape(wk);
    if (talk) {
        unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
        if (unit != NULL && CheckLiveTarget(unit)) {
            bst = sbtlGetBadStatus(&unit->unit);
            if (bst != BST_BOYCOTT && bst != BST_MOUDOKU && bst != BST_STONE) {
                talk = 0;
            }
        }
        if (!sbtlCheckStatus_Move(unit)) {
            talk = 0;
        }
        if (evtBitCheck(2228)) {
            talk = 0;
        }
    }
    itfMenuSetFlagMenu(wk->menuPID, 14, ITFMENU_CB_GF_ENABLE_DECIDE, TutorialMenuCtrl(wk) ? 0 : talk);
    sbtlDeleteUnitList(list);
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlChkEnableDecideMenu);
#endif

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

s32 sbtlChkCallEnd(sBtlMenu_t *wk) {
    s32 end = 0;

    if (jmsChkCommonInputData[2] < 0) {
        switch (wk->step) {
        case 2:
            if (itfMenuProcessCheck(wk->menuPID) && itfMenuGetCtrlType(wk->menuPID) == ITFMENU_CTRL_TYPE_WAIT) {
                end = 1;
                cmpSklTypeSet(wk->menuPID);
            }
            break;
        case 3:
            if (itfMenuProcessCheck(wk->listPID) && itfMenuGetCtrlType(wk->listPID) == ITFMENU_CTRL_TYPE_WAIT) {
                end = 1;
                cmpSklTypeSet(wk->listPID);
            }
            break;
        case 4:
        case 5:
        case 9:
        case 10:
            if (itfMenuProcessCheck(wk->charPID) && itfMenuGetCtrlType(wk->charPID) == ITFMENU_CTRL_TYPE_WAIT) {
                end = 1;
                cmpSklTypeSet(wk->charPID);
            }
            break;
        case 6:
        case 7:
            if (itfMenuProcessCheck(wk->nakamaPID) && itfMenuGetCtrlType(wk->nakamaPID) == ITFMENU_CTRL_TYPE_WAIT) {
                end = 1;
                cmpSklTypeSet(wk->nakamaPID);
            }
            break;
        case 11:
            if (itfMenuProcessCheck(wk->charPID) && itfMenuGetCtrlType(wk->charPID) == ITFMENU_CTRL_TYPE_WAIT) {
                end = 1;
                itfMenuSetFlag(wk->charPID, 8, 1);
                cmpSklTypeSet(wk->charPID);
            }
            break;
        }
        if (end) {
            wk->flag |= 0x20;
            return 1;
        }
    }
    return 0;
}

/* NON_MATCHING: 104 of 504 words match.  */
#ifdef NON_MATCHING
s32 sbtlProcess(dds3ProcessID_t *id) {
    sBtlMenu_t *wk;
    struct SBTLUNIT_WORK *nakama;
    s32 min;
    s32 max;

    wk = dds3GetProcessData(id);
    if (wk->step != 0 && wk->step != 15 && wk->step != 16 && wk->step != 17 && wk->type == 0) {
        if (sbtlChkCallEnd(wk)) {
            wk->flag |= 0x20;
            return 0;
        }
    }
    switch (wk->step) {
    case 0:
        if (sbtlCheckLoadFlg()) {
            Init_sbtlMenu(wk);
            wk->step = 1;
        }
        return 0;
    case 1:
        if (!(wk->flag & 0x10)) {
            wk->flag &= ~2;
            return 0;
        }
        if (wk_sbtl->smnCtrl.bNoOk && wk->type == 1) {
            return 0;
        }
        wk->flag = (wk->flag & ~0x32) | 2;
        sbtlCallMenuClearMenuAll(wk);
        sbtlChkEnableDecideMenu(wk);
        if (wk->type == 1) {
            itfMenuSetFlag(wk->menuPID, 0x10000, 1);
        } else {
            itfMenuSetFlag(wk->menuPID, 0x10000, 0);
        }
        itfMenuStart(wk->menuPID);
        wk->endtype = 0;
        wk->step = 2;
        return 0;
    case 2:
        if (itfMenuGetWaitType(wk->menuPID) == ITFMENU_WAIT_TYPE_CANCEL) {
            sbtlCallEnd(wk);
            return 0;
        }
        if (dds3PadButtonState[0x22] < 0 && wk->type == 1 && wk_sbtl->member_num > 0) {
            sbtlGetShortCutFlg(wk->menuPID);
        }
        return 0;
    case 3:
        if (itfMenuGetWaitType(wk->listPID) != ITFMENU_WAIT_TYPE_CANCEL) {
            return 0;
        }
        wk_sbtl->flg &= ~0x80000;
        if (wk->flag & 0x20) {
            sbtlCallEnd(wk);
        } else {
            sbtlChkEnableDecideMenu(wk);
            itfMenuStart(wk->menuPID);
            wk->step = 2;
        }
        sbtlUseItemProcSBtlPlayerSub();
        return 0;
    case 4:
        if (itfMenuGetWaitType(wk->charPID) != ITFMENU_WAIT_TYPE_CANCEL) {
            return 0;
        }
        wk_sbtl->flg &= ~0x80000;
        if (wk->flag & 0x20) {
            sbtlCallEnd(wk);
        } else {
            sbtlChkEnableDecideMenu(wk);
            itfMenuStart(wk->menuPID);
            wk->step = 2;
        }
        sbtlUseItemProcSBtlPlayerSub();
        return 0;
    case 5:
        if (itfMenuGetWaitType(wk->charPID) != ITFMENU_WAIT_TYPE_CANCEL) {
            return 0;
        }
        wk_sbtl->flg &= ~0x80000;
        if (wk->flag & 0x20) {
            sbtlCallEnd(wk);
        } else {
            sbtlChkEnableDecideMenu(wk);
            itfMenuStart(wk->listPID);
            wk->step = 3;
        }
        sbtlUseItemProcSBtlPlayerSub();
        return 0;
    case 6:
        if (itfMenuGetWaitType(wk->nakamaPID) != ITFMENU_WAIT_TYPE_CANCEL) {
            return 0;
        }
        wk_sbtl->flg &= ~0x80000;
        if (wk->flag & 0x20) {
            sbtlCallEnd(wk);
        } else {
            sbtlChkEnableDecideMenu(wk);
            itfMenuStart(wk->menuPID);
            wk->step = 2;
        }
        sbtlUseItemProcSBtlPlayerSub();
        return 0;
    case 7:
        if (itfMenuGetWaitType(wk->nakamaPID) != ITFMENU_WAIT_TYPE_CANCEL) {
            return 0;
        }
        wk_sbtl->flg &= ~0x80000;
        if (wk->flag & 0x20) {
            sbtlCallEnd(wk);
        } else {
            sbtlChkEnableDecideMenu(wk);
            itfMenuStart(wk->charPID);
            wk->step = 9;
        }
        sbtlUseItemProcSBtlPlayerSub();
        return 0;
    case 8:
        if (sbtlGetStat() == 2) {
            return 0;
        }
        if (!sbtlChkDecide()) {
            sbtlChkEnableDecideMenu(wk);
            itfMenuStart(wk->menuPID);
            wk->step = 2;
            return 0;
        }
        wk->step = 1;
        wk->flag &= ~0x10;
        return 0;
    case 9:
        if (itfMenuGetWaitType(wk->charPID) == ITFMENU_WAIT_TYPE_CANCEL) {
            wk_sbtl->flg &= ~0x80000;
            if (wk->flag & 0x20) {
                sbtlCallEnd(wk);
            } else {
                sbtlChkEnableDecideMenu(wk);
                itfMenuStart(wk->menuPID);
                wk->step = 2;
            }
            sbtlUseItemProcSBtlPlayerSub();
            return 0;
        }
        if (itfMenuGetCtrlType(wk->charPID) != ITFMENU_CTRL_TYPE_WAIT) {
            return 0;
        }
        sbtlBtlFuncDecideItemSBtlMenuSub(&min, &max);
        if (max <= 0) {
            return 0;
        }
        if (dds3PadButtonState[0x24] < 0) {
            if ((s8)(sbtlFuncDecideItem_sbtlMenuData - 1) < min) {
                sbtlFuncDecideItem_sbtlMenuData = max;
            } else {
                sbtlFuncDecideItem_sbtlMenuData--;
            }
            sbtlCallGetProcessData44();
            sbtlPlaySE(0x1001F);
        } else if (dds3PadButtonState[0x25] < 0) {
            if (max < (s8)(sbtlFuncDecideItem_sbtlMenuData + 1)) {
                sbtlFuncDecideItem_sbtlMenuData = min;
            } else {
                sbtlFuncDecideItem_sbtlMenuData++;
            }
            sbtlCallGetProcessData44();
            sbtlPlaySE(0x1001F);
        }
        return 0;
    case 10:
        if (itfMenuGetWaitType(wk->charPID) == ITFMENU_WAIT_TYPE_CANCEL) {
            wk_sbtl->flg &= ~0x80000;
            if (wk->flag & 0x20) {
                sbtlCallEnd(wk);
            } else {
                sbtlChkEnableDecideMenu(wk);
                itfMenuStart(wk->listPID);
                wk->step = 3;
            }
            sbtlUseItemProcSBtlPlayerSub();
            return 0;
        }
        if (itfMenuGetCtrlType(wk->charPID) != ITFMENU_CTRL_TYPE_WAIT) {
            return 0;
        }
        if (!sbtlFuncDecideItemStockData) {
            return 0;
        }
        sbtlBtlFuncDecideItemSBtlMenuSub2(&min, &max);
        if (max <= 0) {
            return 0;
        }
        if (dds3PadButtonState[0x24] < 0) {
            if ((s8)(sbtlFuncDecideItem_sbtlMenuData - 1) >= min) {
                sbtlFuncDecideItem_sbtlMenuData--;
            } else {
                sbtlFuncDecideItem_sbtlMenuData = max;
            }
        } else if (dds3PadButtonState[0x25] < 0) {
            if (max >= (s8)(sbtlFuncDecideItem_sbtlMenuData + 1)) {
                sbtlFuncDecideItem_sbtlMenuData++;
            } else {
                sbtlFuncDecideItem_sbtlMenuData = min;
            }
        } else {
            return 0;
        }
        sbtlFuncDecideItemStock();
        sbtlPlaySE(0x1001F);
        return 0;
    case 11:
        if (wk->flag & 0x20) {
            wk_sbtl->flg &= ~0x80000;
            sbtlCallEnd(wk);
            return 0;
        }
        if (wk->count) {
            if (wk_sbtl->mag < 0) {
                itfMenuSetFlagMenu(wk->charPID, -1, ITFMENU_CB_GF_ENABLE_DECIDE, 0);
            }
            itfMenuSetFlag(wk->charPID, 8, 1);
            itfMenuSetFlagMenu(wk->charPID, -1, ITFMENU_CB_GF_SELECT, 0);
            wk->count = 0;
            wk->cntAnalyze = 0;
            wk->step = 4;
        } else {
            if (dds3PadButtonState[0x21] < 0) {
                wk->count = 1;
            }
            if (++wk->cntAnalyze >= 11) {
                wk->cntAnalyze = 10;
            }
        }
        return 0;
    case 12:
        if (sbtlGetStat() == 2) {
            return 0;
        }
        if (!sbtlChkDecide()) {
            sbtlChkEnableDecideMenu(wk);
            itfMenuStart(wk->menuPID);
            wk->step = 2;
            return 0;
        }
        wk->flag |= 0x40;
        sbtlCallEnd(wk);
        return 0;
    case 13:
        if (slChoiceMsgDraw(195)) {
            wk->step = 1;
            wk->flag |= 0x10;
        }
        return 0;
    case 14:
        nakama = sbtlGetNakama((s8)wk_sbtl->ctrlSmnIdx);
        if (nakama == NULL) {
            return 0;
        }
        if (sbtlLoadEffectData_Decode(nakama)) {
            wk->step = 1;
            wk->flag |= 0x10;
        }
        return 0;
    case 15:
        if (wk->count == 0) {
            sbtlCallProcEnd(wk);
        }
        if (sbtlCheckProcEnd(wk)) {
            wk->count++;
            return 0;
        }
        wk->count = 0;
        wk->step = 16;
        return 0;
    case 16:
        if (wk->count == 0) {
            wk->flag &= ~2;
            wk->count = 1;
        } else if (wk->count < 2) {
            wk->count++;
        } else {
            wk->count = 0;
            wk->step = 17;
        }
        return 0;
    case 17:
        return -1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlProcess);
#endif

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

/* NON_MATCHING: 200 of 203 words match.  */
#ifdef NON_MATCHING
s32 sbtlDrawProcess_sbtlMenu(dds3ProcessID_t *id)
{
    sBtlMenu_t *wk = dds3GetProcessData(id);
    struct SBTLUNIT_WORK *unit;
    s32 tab;
    s32 max;
    s32 sel;
    s32 num;
    u32 alpha;
    u32 color;
    u8 rate;

    if (!(wk->flag & 2)) {
        return 0;
    }
    switch (wk->step) {
    case 2:
        sel = itfMenuGetSelectID(wk->menuPID);
        if (sel >= 0) {
            num = sbtlDrawProcess_sbtlMenuData[sel];
            if (num >= 0) {
                if (num == 7 && sbtlEscapeCheck() == 1) {
                    num = 8;
                }
                sbtlHelp(3, num, GetSelectRootID(wk->menuPID, 2));
            }
        }
        slCallSin(GetSelectRootID(wk->menuPID, 2));
        if (wk->type == 1 && wk_sbtl->member_num > 0) {
            alpha = GetSelectRootID(wk->menuPID, 2);
            itfDrawMiscSpriteAlpha(368, 335, 0, alpha, sbtlData135.hSpr, 15, 104);
            color = 0xA09DC300 | ((alpha << 7) / 255);
            itfIconDrawSprite(384, 336, 0, alpha, 14, 104);
            itfDrawFont(ITF_DM_STYLE_SFNT_CNV, 407, 336, 0x100000, 0, 0, color, 1.0f, 104, sbtlDrawProcess_sbtlMenuData2[0]);
        }
        break;
    case 9:
        sbtlBtlFuncDecideItemSBtlMenuSub(&tab, &max);
        sbtlBtlDrawProcessSBtlMenuSub(tab, max, sbtlFuncDecideItem_sbtlMenuData, 1, 1.0f);
        break;
    case 5:
        sel = itfMenuGetSelectID(wk->menuPID);
        rate = GetSelectRootID(wk->charPID, 2);
        if (sel == 5) {
            sbtlHelp(3, 15, rate);
        }
        break;
    case 10:
        rate = GetSelectRootID(wk->charPID, 2);
        sbtlBtlFuncDecideItemSBtlMenuSub2(&tab, &max);
        sbtlBtlDrawProcessSBtlMenuSub(tab, max, sbtlFuncDecideItem_sbtlMenuData, 0, rate / 255.0f);
        sbtlHelp(3, 15, rate);
        break;
    case 11:
        sel = itfMenuGetSelectID(wk->charPID);
        unit = wk_sbtl->tgtList->pUnitL[sel].pUnit;
        rate = (u32)(wk->cntAnalyze * 255.0f / 10.0f);
        if (unit->flg & 0x800) {
            unit = sbtlSearchUnit_ID(wk_sbtl->unitList, unit->field492);
        }
        sbtlCallBtlAnalyzeBack2(unit, rate);
        break;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawProcess_sbtlMenu);
#endif

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
    ProcessCheckData = dds3StartProcess(sbtlStrSbtlmenuMain, 0x3FE, 0, 0, sbtlProcess, sbtlDrawProcess, wk);
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

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSbtlmenuMain);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSbtlmenuDraw);

INCLUDE_ASM(const s32, "code_002F5CF0", GetActionUnit);

struct SBTLUNIT_WORK *GetTargetUnit(void) {
    sBtlMenu_t *menu;
    struct SBTLUNIT_WORK *unit;
    struct SBTLUNIT_WORK *nakama;
    struct sdatUnitWork_t *stock;
    SBTLMENU_NODE node;
    s32 id;
    s32 i;

    menu = sbtlFuncKatana();
    if (menu == NULL) {
        return NULL;
    }
    if (!itfMenuProcessCheck(menu->menuPID)) {
        return NULL;
    }
    node = sbtlGetNode();
    if (node < SBTLMENU_NODE_ORDER) {
        return NULL;
    }
    unit = NULL;
    switch (node) {
        case SBTLMENU_NODE_ORDER:
            unit = GetTargetUnit_sbtlCmndMenuNew();
            break;
        case SBTLMENU_NODE_SUMMON:
            if (menu->step == 9) {
                id = itfMenuGetSelectID(menu->charPID);
                if (id < 0) {
                    break;
                }
                stock = sbtlRefDDS3GBWK2(id);
                if (stock != NULL) {
                    for (i = 0; i < 2; i++) {
                        nakama = sbtlGetNakama(i);
                        if (nakama != NULL && nakama->unit.devilId == stock->devilId) {
                            unit = nakama;
                            break;
                        }
                    }
                }
                break;
            }
        case SBTLMENU_NODE_RETURN:
            id = itfMenuGetSelectID(menu->nakamaPID);
            if (id >= 0 && id < 2) {
                unit = sbtlGetNakama(id);
            }
            break;
        case SBTLMENU_NODE_ITEM:
            if (menu->step == 10) {
                id = itfMenuGetSelectID(menu->charPID);
                if (id < 0) {
                    break;
                }
                if (id == 0) {
                    unit = sbtlRefSbtl10(sbtlGetPlayerID());
                } else if (id > 0) {
                    stock = sbtlRefDDS3GBWK2(id - 1);
                    if (stock != NULL) {
                        for (i = 0; i < 2; i++) {
                            nakama = sbtlGetNakama(i);
                            if (nakama != NULL && nakama->unit.devilId == stock->devilId) {
                                unit = nakama;
                                break;
                            }
                        }
                    }
                }
            } else if (menu->step == 5) {
                id = itfMenuGetSelectID(menu->charPID);
                if (id >= 0) {
                    unit = wk_sbtl->unitList->pUnitL[id].pUnit;
                }
            }
            break;
        case SBTLMENU_NODE_KATANA:
        case SBTLMENU_NODE_ANALYZE:
            id = itfMenuGetSelectID(menu->charPID);
            if (id >= 0 && wk_sbtl->tgtList != NULL) {
                unit = wk_sbtl->tgtList->pUnitL[id].pUnit;
            }
            break;
    }
    return unit;
}

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

/* NON_MATCHING: 160 of 181 words match.  */
#ifdef NON_MATCHING
void DrawAddMag(s32 x, s32 y)
{
    s32 ofs;
    s32 bx;
    s32 t;
    s32 now;

    if (wk_sbtl->status3 & 2) {
        if (gAddMagNumDrawCnt == 90) {
            ofs = 72;
            sbtlPlaySE(0x10020);
            dds3DrawAddMagSub(104);
            if (gAddMagNum < 101) {
                ofs = 48;
                if (gAddMagNum < 11) {
                    ofs = 24;
                }
            }
            bx = x + ofs;
            nbResultCountEntries(bx + GetRand(2), y + GetRand(2) + 24, 0);
            nbResultCountEntries(bx + GetRand(2), y + GetRand(2), 4);
            nbResultCountEntries(x + GetRand(2) + 2, y + GetRand(2), 8);
            nbResultCountEntries(x + GetRand(2) + 2, y + GetRand(2) + 24, 12);
        }
        if (gAddMagNumDrawCnt == 80) {
            sbtlPlaySE(0x10020);
        }
        if (gAddMagNumDrawCnt == 70) {
            sbtlPlaySE(0x10020);
        }
    }
    if (gAddMagNumNow < gAddMagNum) {
        now = gAddMagNum;
        if (now >= gAddMagNumNow + gAddMagNumUp) {
            now = gAddMagNumNow + gAddMagNumUp;
        }
        gAddMagNumNow = now;
    }
    if (gAddMagNumDrawCnt <= 0) {
        gAddMagNum = 0;
        gAddMagNumDrawCnt = 0;
        gAddMagNumNow = 0;
        gAddMagNumUp = 0;
        return;
    }
    if (gAddMagNumNow >= gAddMagNum) {
        gAddMagNumDrawCnt--;
    }
    if (gAddMagNumDrawCnt < 10) {
        t = 10 - gAddMagNumDrawCnt;
        sbtlNum3(x - t * 4, y, ((64 - t * 6) << 24) | 0x804080, 0xFF - t * 25, gAddMagNum, 103,
                 ((64 - t * 6) << 24) | 0x804080, 0.79999996f - t / 20.0f);
    } else {
        sbtlNum3(x, y, 0x40804080, 0xFF, gAddMagNum, 103, 0x40804080, 0.79999996f);
    }
    if ((wk_sbtl->status3 & 2) && (gAddMagNumDrawCnt & 1)) {
        ofs = 72;
        if (gAddMagNum < 101) {
            ofs = 48;
            if (gAddMagNum < 11) {
                ofs = 24;
            }
        }
        nbResultCountEntries(x + GetRand(ofs), y + GetRand(24), 0);
    }
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", DrawAddMag);
#endif

void sbtlGetIdling_HD(s32 a_x, s32 a_y, u8 a_alphaRate, s32 a_otNo, struct sdatUnitWork_t *unit) {
    if (unit != NULL) {
        itfIconDrawBadStatus(a_x, a_y, 0, a_alphaRate, sbtlGetBadStatus(unit), a_otNo);
    }
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlMoonOff);

/* NON_MATCHING: 133 of 135 words match.  */
#ifdef NON_MATCHING
s32 sbtlBarCtrl(void)
{
    s32 len = 350;
    s32 alpha = 126;

    if (!wk_sbtl->bBfrBtlEnd) {
        wk_sbtl->field4200 = len;
        wk_sbtl->field41FC = alpha;
        sbtlData135.hSpr->workEntries[13].geometry.bounds[2] = len * 16;
        sbtlData135.hSpr->workEntries[41].geometry.bounds[2] = wk_sbtl->field4200 * 16;
        evtBtlBarCtrlSub(0);
    } else if (!(wk_sbtl->flg2 & 0x200000) && wk_sbtl->mode != 8) {
        if (wk_sbtl->field430) {
            wk_sbtl->field4200 = len;
            wk_sbtl->field41FC = alpha;
            sbtlData135.hSpr->workEntries[13].geometry.bounds[2] = len * 16;
            sbtlData135.hSpr->workEntries[41].geometry.bounds[2] = wk_sbtl->field4200 * 16;
            evtBtlBarCtrlSub(0);
            return 0;
        }
        if (wk_sbtl->field42CA != 0) {
            if (wk_sbtl->field42CA != 3) {
            if (wk_sbtl->field42CA == 2) {
                alpha = 40;
                len = 200;
                wk_sbtl->field4200 = len;
                wk_sbtl->field41FC = alpha;
                sbtlData135.hSpr->workEntries[13].geometry.bounds[2] = len * 16;
                sbtlData135.hSpr->workEntries[41].geometry.bounds[2] = wk_sbtl->field4200 * 16;
            } else {
                wk_sbtl->field4200 = len;
                wk_sbtl->field41FC = alpha;
                sbtlData135.hSpr->workEntries[13].geometry.bounds[2] = len * 16;
                sbtlData135.hSpr->workEntries[41].geometry.bounds[2] = wk_sbtl->field4200 * 16;
            }
            }
        } else {
            alpha = 40;
            len = 200;
        }
    }
    if (wk_sbtl->field4200 < len) {
        wk_sbtl->field4200 += 16;
        if (wk_sbtl->field4200 > len) {
            wk_sbtl->field4200 = len;
        }
    } else if (wk_sbtl->field4200 > len) {
        wk_sbtl->field4200 -= 16;
        if (wk_sbtl->field4200 < len) {
            wk_sbtl->field4200 = len;
        }
    }
    if (wk_sbtl->field41FC < alpha) {
        wk_sbtl->field41FC += 16;
        if (wk_sbtl->field41FC > alpha) {
            wk_sbtl->field41FC = alpha;
        }
    } else if (wk_sbtl->field41FC > alpha) {
        wk_sbtl->field41FC -= 16;
        if (wk_sbtl->field41FC < alpha) {
            wk_sbtl->field41FC = alpha;
        }
    }
    sbtlData135.hSpr->workEntries[13].geometry.bounds[2] = wk_sbtl->field4200 * 16;
    sbtlData135.hSpr->workEntries[41].geometry.bounds[2] = wk_sbtl->field4200 * 16;
    sbtlData135.hSpr->workEntries[44].geometry.bounds[2] = wk_sbtl->field4200 * 16;
    evtBtlBarCtrlSub((wk_sbtl->field41FC - 126) * 16);
    return wk_sbtl->field4200 == 350;
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBarCtrl);
#endif

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrSummoner2);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrKuzunoha);

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
    g_cntTgtCur = 0;
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

void sbtlNum3(s32 a_x, s32 a_y, u32 a_rgba, u8 a_alphaRate, s32 a_num, DDS3_OT a_ot, u32 rinkaku_rgba, f32 scale) {
    u32 colors[4];
    s32 i;
    s32 j;
    s32 figure;
    s32 sprNo;
    s32 width;
    s32 height;

    figure = itfGetFigure(a_num);
    for (i = 0; i < figure; i++) {
        sprNo = itfGetFigureNum(a_num, figure - i);
        if (sprNo >= 0) {
            sprNo += 27;
            for (j = 0; j < 4; j++) {
                colors[j] = sbtlData135.hSpr->workEntries[sprNo].geometry.cornerColors[j];
            }
            width = sbtlData135.hSpr->workEntries[sprNo].geometry.bounds[2];
            height = sbtlData135.hSpr->workEntries[sprNo].geometry.bounds[3];
            sbtlData135.hSpr->workEntries[sprNo].geometry.bounds[2] = width * scale;
            sbtlData135.hSpr->workEntries[sprNo].geometry.bounds[3] = height * scale;
            for (j = 0; j < 4; j++) {
                sbtlData135.hSpr->workEntries[sprNo].geometry.cornerColors[j] = (rinkaku_rgba & 0xFFFFFF00) | (a_rgba & 0xFF);
            }
            itfCallLinkAlpha(a_x - 4, a_y, 0, 0x20, sbtlData135.hSpr, sprNo, a_ot);
            itfCallLinkAlpha(a_x + 4, a_y, 0, 0x20, sbtlData135.hSpr, sprNo, a_ot);
            itfCallLinkAlpha(a_x, a_y - 4, 0, 0x20, sbtlData135.hSpr, sprNo, a_ot);
            itfCallLinkAlpha(a_x, a_y + 4, 0, 0x20, sbtlData135.hSpr, sprNo, a_ot);
            sbtlData135.hSpr->workEntries[sprNo].geometry.bounds[2] += 64.0f;
            sbtlData135.hSpr->workEntries[sprNo].geometry.bounds[3] += 32.0f;
            itfCallLinkAlpha(a_x - 2, a_y - 2, 0, 0x60, sbtlData135.hSpr, sprNo, a_ot);
            itfCallLinkAlpha(a_x - 6, a_y - 2, 0, 0x20, sbtlData135.hSpr, sprNo, a_ot);
            itfCallLinkAlpha(a_x + 2, a_y - 2, 0, 0x20, sbtlData135.hSpr, sprNo, a_ot);
            itfCallLinkAlpha(a_x - 2, a_y - 6, 0, 0x20, sbtlData135.hSpr, sprNo, a_ot);
            itfCallLinkAlpha(a_x - 2, a_y + 2, 0, 0x20, sbtlData135.hSpr, sprNo, a_ot);
            sbtlData135.hSpr->workEntries[sprNo].geometry.bounds[2] -= 64.0f;
            sbtlData135.hSpr->workEntries[sprNo].geometry.bounds[3] -= 32.0f;
            for (j = 0; j < 4; j++) {
                sbtlData135.hSpr->workEntries[sprNo].geometry.cornerColors[j] = a_rgba;
            }
            itfDrawMiscSpriteAlpha(a_x, a_y, 0, a_alphaRate, sbtlData135.hSpr, sprNo, a_ot);
            a_x += scale * 24.0f;
            sbtlData135.hSpr->workEntries[sprNo].geometry.bounds[2] = width;
            sbtlData135.hSpr->workEntries[sprNo].geometry.bounds[3] = height;
            for (j = 0; j < 4; j++) {
                sbtlData135.hSpr->workEntries[sprNo].geometry.cornerColors[j] = colors[j];
            }
        }
    }
}

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

/* NON_MATCHING: 44 of 162 words match.  */
#ifdef NON_MATCHING
void sbtlPlayerData(void) {
    sdfVector4_t pos;
    SBTLPROJECTEDSCREEN_POINT point;
    struct SBTLUNIT_WORK *unit;
    s32 x;
    s32 bx;
    s32 frame;

    if (sbtlGetMode() == E_CAMERA_MODE_End) {
        return;
    }
    if (wk_sbtl->flg2 & 0x20000) {
        return;
    }
    if (wk_sbtl->flg2 & 0x10000) {
        return;
    }
    if (wk_sbtl->flg2 & 0x40000) {
        return;
    }
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (unit == NULL) {
        return;
    }
    DrawStatusBar(&unit->unit, NULL, 10, 1, 0);
    itfDrawMiscSprite(wk_sbtl->field41FC - 25, 10, 0, sbtlData135.hSpr, 20, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    x = wk_sbtl->field41FC + wk_sbtl->field4200;
    bx = x - 100;
    if ((s8)(sbtlPlayerDataData + 1) < 24) {
        sbtlPlayerDataData++;
    } else {
        sbtlPlayerDataData = 0;
    }
    frame = (s8)sbtlPlayerDataData / 4;
    itfDrawMiscSprite(bx, 24, 0, sbtlData135.hSpr, 0, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
    if (wk_sbtl->mag > 0) {
        itfCalcSpriteWidth(sbtlData135.hSpr, frame + 1, (wk_sbtl->magMax < wk_sbtl->mag) ? wk_sbtl->magMax : wk_sbtl->mag,
                           wk_sbtl->magMax);
        itfDrawMiscSprite(x - 97, 24, 0, sbtlData135.hSpr, frame + 1, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
    }
    sbtlNum(bx + (4 - itfGetFigure(wk_sbtl->mag)) * 9 + 63, 14, 0x50FF5064, 0xFF, wk_sbtl->mag, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
    DrawAddMag(x + 15, 12);
    if (wk_sbtl->bltNumDrwCnt != 0) {
        wk_sbtl->bltNumDrwCnt--;
        SDF_VU0_LOADVECTOR(&unit->pos);
        SDF_VU0_STOREVECTOR(&pos);
        pos.y -= 200.0f;
        SDF_VU0_LOADVECTOR(&pos);
        if (btlProjectForwardPositionToScreen(&point)) {
            itfDrawMiscSprite(point.x, point.y, 0, sbtlData135.hSpr, 14, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
            sbtlNum(point.x + 10, point.y + 5, 0xA09DC380, 0xFF, sbtlGetGun_Num(), DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlPlayerData);
#endif

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSummonDevilAshimotoReset);

/* NON_MATCHING: 316 of 360 words match.  */
#ifdef NON_MATCHING
void sbtlSummonDevilAshimoto(struct SBTLUNIT_WORK *wk, s32 idx)
{
    sdfVector4_t pos;
    sdfVector4_t pos2;
    sdfVector4_t pos3;
    sdfVector4_t pos4;
    sdfVector4_t scl;
    dds3Basic_t *basic;
    dds3Basic_t *basic2 = NULL;
    dds3Basic_t *basic3 = NULL;
    dds3Basic_t *basic4 = NULL;
    s32 lowhp = 0;
    f32 size;
    u8 sdw_flg;
    u32 color;
    s32 alpha;
    mdlObj_t *model;

    if (idx == 0) {
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnit[2], 1);
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnitR[2], 1);
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnit[3], 1);
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnitR[3], 1);
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnit[4], 1);
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnitR[4], 1);
    } else {
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnit[5], 1);
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnitR[5], 1);
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnit[6], 1);
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnitR[6], 1);
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnit[7], 1);
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnitR[7], 1);
    }
    if (wk->unit.now.maxhp / 4 < wk->unit.now.hp) {
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnitR[idx], 1);
        dds3ObjectBaseOffFlag(wk_sbtl->apMarkerUnit[idx], 1);
        basic = wk_sbtl->apMarkerUnit[idx];
        if (!IsSceanStop()) {
            wk->nakama_asimoto_anim += 0.05f;
        }
    } else {
        lowhp = 1;
        dds3ObjectBaseOnFlag(wk_sbtl->apMarkerUnit[idx], 1);
        dds3ObjectBaseOffFlag(wk_sbtl->apMarkerUnitR[idx], 1);
        basic = wk_sbtl->apMarkerUnitR[idx];
        if (!IsSceanStop()) {
            wk->nakama_asimoto_anim += 0.2f;
        }
    }
    if (wk->asimoto_mode == 1) {
        if (!lowhp) {
            if (!idx) {
                basic4 = wk_sbtl->apMarkerUnit[4];
                basic2 = wk_sbtl->apMarkerUnit[2];
                basic3 = wk_sbtl->apMarkerUnit[3];
            } else {
                basic4 = wk_sbtl->apMarkerUnit[7];
                basic2 = wk_sbtl->apMarkerUnit[5];
                basic3 = wk_sbtl->apMarkerUnit[6];
            }
        } else {
            if (!idx) {
                basic4 = wk_sbtl->apMarkerUnitR[4];
                basic2 = wk_sbtl->apMarkerUnitR[2];
                basic3 = wk_sbtl->apMarkerUnitR[3];
            } else {
                basic4 = wk_sbtl->apMarkerUnitR[7];
                basic2 = wk_sbtl->apMarkerUnitR[5];
                basic3 = wk_sbtl->apMarkerUnitR[6];
            }
        }
    }
    if ((wk->vstatus & 0x1000) || wk_sbtl->field42CA == 1 || wk_sbtl->field42CA == 2) {
        dds3ObjectBaseOnFlag(basic, 1);
        if (basic2) dds3ObjectBaseOnFlag(basic2, 1);
        if (basic3) dds3ObjectBaseOnFlag(basic3, 1);
        if (basic4) dds3ObjectBaseOnFlag(basic4, 1);
        return;
    }
    dds3ObjectBaseOffFlag(basic, 1);
    if (basic2) dds3ObjectBaseOffFlag(basic2, 1);
    if (basic3) dds3ObjectBaseOffFlag(basic3, 1);
    if (basic4) dds3ObjectBaseOffFlag(basic4, 1);
    SDF_VU0_LOADVECTOR(&wk->pos);
    SDF_VU0_STOREVECTOR(&pos);
    SDF_VU0_STOREVECTOR(&pos2);
    SDF_VU0_STOREVECTOR(&pos3);
    SDF_VU0_STOREVECTOR(&pos4);
    if (wk->asimoto_mode == 1) {
        pos2.x = wk->asimoto_pos_x2;
        pos2.z = wk->asimoto_pos_z2;
        pos.x = wk->asimoto_pos_x;
        pos.z = wk->asimoto_pos_z;
        dds3SetPosPoseSet(basic2, &pos2);
        pos3.x = wk->asimoto_pos_x3;
        pos3.z = wk->asimoto_pos_z3;
        dds3SetPosPoseSet(basic3, &pos3);
        pos4.x = wk->asimoto_pos_x4;
        pos4.z = wk->asimoto_pos_z4;
        dds3SetPosPoseSet(basic4, &pos4);
    }
    dds3SetPosPoseSet(basic, &pos);
    size = 100.0f;
    sdw_flg = sbtlGetMotionVisualData(wk->major, wk->minor)->sdw_flg;
    if (sdw_flg & 4) {
        size = 34.999996185302734375f;
    } else if (sdw_flg & 8) {
        size = 50.99999237060546875f;
    } else if (sdw_flg & 0x10) {
        size = 58.79998779296875f;
    } else if (sdw_flg & 0x20) {
        size = 83.99997711181640625f;
    }
    size += sdfSin(wk->nakama_asimoto_anim) * 4.0f;
    if (size < 1.0f) {
        size = 1.0f;
    }
    scl.w = 1.0f;
    scl.y = 1.0f;
    scl.z = size * 0.03f;
    scl.x = scl.z;
    dds3SetSclPoseSet(basic, &scl);
    if (basic2) dds3SetSclPoseSet(basic2, &scl);
    if (basic3) dds3SetSclPoseSet(basic3, &scl);
    if (basic4) dds3SetSclPoseSet(basic4, &scl);
    color = 0x808080;
    alpha = (s32)(sdfSin(wk->nakama_asimoto_anim * 1.37f) * 31.0f) + 88;
    if ((wk->unit.badStatus & 0x40000) || wk->byctCnt) {
        color = 0x101010;
    }
    model = dds3ObjectBaseGetModelHandle(basic);
    mdlSetColor(model, color | (alpha << 24));
    if (basic2) {
        alpha -= 24;
        model = dds3ObjectBaseGetModelHandle(basic2);
        if (alpha < 16) alpha = 16;
        mdlSetColor(model, color | (alpha << 24));
    }
    if (basic3) {
        alpha -= 24;
        model = dds3ObjectBaseGetModelHandle(basic3);
        if (alpha < 16) alpha = 16;
        mdlSetColor(model, color | (alpha << 24));
    }
    if (basic4) {
        alpha -= 24;
        model = dds3ObjectBaseGetModelHandle(basic4);
        if (alpha < 16) alpha = 16;
        mdlSetColor(model, color | (alpha << 24));
    }
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSummonDevilAshimoto);
#endif

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

/* NON_MATCHING: 9 of 257 words match.  */
#ifdef NON_MATCHING
void sbtlEnemyData(void)
{
    SBTLPROJECTEDSCREEN_POINT point;
    sdfVector4_t pos;
    struct SBTLUNIT_WORK *unit;
    struct SBTLSKILLP_TBL *skill;
    SKLID id;
    u32 ot;
    s32 raceWidth;
    s32 width;
    s32 i;

    if (sbtlGetCutFlg() & 1) {
        return;
    }
    for (i = 0; i < wk_sbtl->unitList->number; i++) {
        unit = wk_sbtl->unitList->pUnitL[i].pUnit;
        if (unit == NULL || !(unit->field414 & 2)) {
            continue;
        }
        if (!(unit->vstatus & 0x1000)) {
            skill = sbtlGetSkilltbl(unit->skillid);
            if (!(unit->flg & 0x100000) && !(skill->flg & 0x40000000) && !(skill->flg2 & 0x40000) &&
                !(wk_sbtl->flg2 & 0x1000)) {
                id = SKLID_NONE;
                switch (unit->status) {
                case 3:
                case 6:
                case 31:
                    id = unit->skillid;
                    if (id == SKLID_ATK) {
                        id = SKLID_NONE;
                    }
                    break;
                }
                if (id != SKLID_NONE) {
                    sdfModelEvalTm(unit->hEveUnit->owner->inner);
                    if (!sdfModelGetMarkPos_sdfModel(unit->hEveUnit->owner->inner, 1)) {
                        sdfModelGetMarkPos_sdfModel(unit->hEveUnit->owner->inner, 0);
                    }
                    if (btlProjectForwardPositionToScreen(&point)) {
                        ot = frBtlEnemyDataSub();
                        frSetFontDrawOT(0x68);
                        unit->cap_skillname_x = point.x - 32;
                        unit->cap_skillname_y = point.y + 16;
                        sbtlDrawSkillName(unit->cap_skillname_x, unit->cap_skillname_y, 0, 0xB080A080, id);
                        frSetFontDrawOT(ot);
                    }
                }
            }
        }
        if (wk_sbtl->bNoEnemyName || (wk_sbtl->flg2 & 0x4400000) || unit->unit.devilId == 462) {
            continue;
        }
        if (wk_sbtl->fieldE8C || wk_sbtl->bBfrBtlEnd) {
            continue;
        }
        if ((unit->vstatus & 0x2000) || unit->mode != 8 || (unit->vstatus & 0x1000)) {
            continue;
        }
        if (wk_sbtl->field42CA) {
            continue;
        }
        SDF_VU0_LOADVECTOR(&unit->pos);
        SDF_VU0_STOREVECTOR(&pos);
        pos.y -= sbtlGetMotionVisualData(unit->major, unit->minor)->field28 * 0.5f;
        SDF_VU0_LOADVECTOR(&pos);
        if (btlProjectForwardPositionToScreen(&point)) {
            raceWidth = frPreCalcStrWidth(sbtlGetRaceName(unit->unit.raceId), 1, 1);
            width = raceWidth + frPreCalcStrWidth(g_itemNo[unit->unit.devilId], 1, 1);
            wk_sbtl->panel->hPanelSpr3->descriptions[11].rect[3] = width + 6;
            width += 30;
            point.x -= width >> 1;
            itfDrawMiscSpriteAlpha(point.x, point.y - 7, 0, 0x80, wk_sbtl->panel->hPanelSpr3, 11, 0x67);
            itfDrawMiscSpriteAlpha(point.x + width, point.y - 6, 0, 0x80, wk_sbtl->panel->hPanelSpr3, 15, 0x67);
            sbtlCallGetSmallFontFRQ4(point.x + 17, point.y + 1, 0, 0xA09DC380, 0xFF, 0x67,
                                     sbtlGetRaceName(unit->unit.raceId));
            sbtlCallGetSmallFontFRQ4(point.x + raceWidth + 32, point.y + 1, 0, 0xA09DC380, 0xFF, 0x67,
                                     g_itemNo[unit->unit.devilId]);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlEnemyData);
#endif

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallBtlGetRaceName);

INCLUDE_RODATA(const s32, "code_002F5CF0", btlLoadInterfaceSpriteStrBattlePanel);

INCLUDE_RODATA(const s32, "code_002F5CF0", btlLoadInterfaceSpriteStrBattleInterfaceSpr);

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

void sbtlCallSetFontDrawOT(struct SBTLUNIT_WORK *wk, sdfVector4_t *pos)
{
    SBTLPROJECTEDSCREEN_POINT point;
    u32 ot;
    s32 x;
    s32 y;
    s32 left;

    if (wk_sbtl->panel == NULL || wk_sbtl->panel->hPanelSpr4 == NULL) {
        return;
    }
    VU0_LOAD_VF(vf10, pos);
    if (!btlProjectForwardPositionToScreen(&point)) {
        return;
    }
    ot = frBtlEnemyDataSub();
    frSetFontDrawOT(0x68);
    if (!(wk_sbtl->flg2 & 0x1000) && wk->field6A8 != 0) {
        sbtlDrawSkillName(point.x - 32, point.y + 16, 0, 0x80A0A0A0, wk->field6A8);
    }
    frSetFontDrawOT(ot);
    if (wk->field708 > 0) {
        wk->field708--;
        x = point.x;
        y = point.y;
        left = x - 32;
        eglSetOt(0x68);
        eglLinkAlpha(0);
        eglLinkTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        evtSubmitDefaultDepthGradientRect(left, y, 48, 6, 0x80000080, 0x80000080, 0x80000080, 0x80000080);
        evtSubmitDefaultDepthGradientRect(left, y, wk->unit.now.hp * 48 / wk->unit.now.maxhp, 6, 0x8000C0C0,
                                          0x8000C0C0, 0x8000C0C0, 0x8000C0C0);
        eglLinkSprite(x - 64, y - 16, 32, 32, 1, 1, 32, 32, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      sdfGetOrCreateTextureForReference(&evtData2, wk->field624));
    }
}

/* NON_MATCHING: 361 of 405 words match.  */
#ifdef NON_MATCHING
void sbtlTargetCursor(const sdfVector4_t *pos, char *str) {
    struct sprHandle_t *spr;
    itfBdWork_t *bd;
    sdfVector4_t v;
    sdfVector4_t w;
    SBTLPROJECTEDSCREEN_POINT pt[2];
    f32 len;
    f32 dx, dy, dz;
    f32 width, height;
    f32 growW, growH;
    u8 alpha;

    if (wk_sbtl->panel == NULL) {
        return;
    }
    spr = wk_sbtl->panel->hPanelSpr4;
    if (spr == NULL) {
        return;
    }
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&v);
    if (gTargetPosHokanFastFlag) {
        gTargetPosHokanFastFlag = 0;
        gTargetPosHokanBuff0.x = v.x;
        gTargetPosHokanBuff0.y = v.y;
        gTargetPosHokanBuff0.z = v.z;
        gTargetPosHokanBuff1.x = v.x;
        gTargetPosHokanBuff1.y = v.y;
        gTargetPosHokanBuff1.z = v.z;
        gTargetPosHokanBuff2.x = v.x;
        gTargetPosHokanBuff2.y = v.y;
        gTargetPosHokanBuff2.z = v.z;
        gTargetPosHokanBuff3.x = v.x;
        gTargetPosHokanBuff3.y = v.y;
        gTargetPosHokanBuff3.z = v.z;
        gTargetPosHokanBuff4.x = v.x;
        gTargetPosHokanBuff4.y = v.y;
        gTargetPosHokanBuff4.z = v.z;
    } else {
        w.x = gTargetPosHokanBuff0.x += (v.x - gTargetPosHokanBuff0.x) / 10.0f;
        w.y = gTargetPosHokanBuff0.y += (v.y - gTargetPosHokanBuff0.y) / 10.0f;
        w.z = gTargetPosHokanBuff0.z += (v.z - gTargetPosHokanBuff0.z) / 10.0f;
        len = fsqrtf((w.x - v.x) * (w.x - v.x) + (w.y - v.y) * (w.y - v.y) + (w.z - v.z) * (w.z - v.z));
        dx = (w.x - v.x) / len * (len / 5.0f);
        dy = (w.y - v.y) / len * (len / 5.0f);
        dz = (w.z - v.z) / len * (len / 5.0f);
        gTargetPosHokanBuff1.x = v.x + dx;
        gTargetPosHokanBuff1.y = v.y + dy;
        gTargetPosHokanBuff1.z = v.z + dz;
        gTargetPosHokanBuff2.x = v.x + (dx + dx);
        gTargetPosHokanBuff2.y = v.y + (dy + dy);
        gTargetPosHokanBuff2.z = v.z + (dz + dz);
        gTargetPosHokanBuff3.x = v.x + dx * 3.0f;
        gTargetPosHokanBuff3.y = v.y + dy * 3.0f;
        gTargetPosHokanBuff3.z = v.z + dz * 3.0f;
        gTargetPosHokanBuff4.x = v.x + dx * 4.0f;
        gTargetPosHokanBuff4.y = v.y + dy * 4.0f;
        gTargetPosHokanBuff4.z = v.z + dz * 4.0f;
    }
    if (g_cntTgtCur < 20) {
        alpha = (g_cntTgtCur << 7) / 20;
    } else {
        alpha = ((60 - g_cntTgtCur) << 7) / 40;
    }
    spr->descriptions[1].presetMode = 1;
    SDF_VU0_LOADVECTOR(&gTargetPosHokanBuff1);
    if (btlProjectForwardPositionToScreen(&pt[0]) && (u32)(pt[0].x + 15) < 543 && pt[0].y > 0 && pt[0].y < 448) {
        effDrawSpriteWithCornerFade(pt[0].x * 16, pt[0].y * 8, 0, 0x80, 0, spr, 1, 103);
    }
    SDF_VU0_LOADVECTOR(&gTargetPosHokanBuff2);
    if (btlProjectForwardPositionToScreen(&pt[0]) && (u32)(pt[0].x + 15) < 543 && pt[0].y > 0 && pt[0].y < 448) {
        effDrawSpriteWithCornerFade(pt[0].x * 16, pt[0].y * 8, 0, 0x60, 0, spr, 1, 103);
    }
    SDF_VU0_LOADVECTOR(&gTargetPosHokanBuff3);
    if (btlProjectForwardPositionToScreen(&pt[0]) && (u32)(pt[0].x + 15) < 543 && pt[0].y > 0 && pt[0].y < 448) {
        effDrawSpriteWithCornerFade(pt[0].x * 16, pt[0].y * 8, 0, 0x40, 0, spr, 1, 103);
    }
    SDF_VU0_LOADVECTOR(&gTargetPosHokanBuff4);
    if (btlProjectForwardPositionToScreen(&pt[0]) && (u32)(pt[0].x + 15) < 543 && pt[0].y > 0 && pt[0].y < 448) {
        effDrawSpriteWithCornerFade(pt[0].x * 16, pt[0].y * 8, 0, 0x30, 0, spr, 1, 103);
    }
    spr->descriptions[1].presetMode = 0;
    SDF_VU0_LOADVECTOR(pos);
    if (btlProjectForwardPositionToScreen(&pt[0]) && (u32)(pt[0].x + 15) < 543 && pt[0].y > 0 && pt[0].y < 448) {
        bd = spr->workEntries;
        width = bd->sourceWidth;
        height = bd->sourceHeight;
        growW = width * 0.79999996f * g_cntTgtCur / 59.0f;
        growH = height * 0.79999996f * g_cntTgtCur / 59.0f;
        spr->workEntries->geometry.bounds[2] = itfCalcSpriteWidthSub(width + growW, 4);
        spr->workEntries->geometry.bounds[3] = itfCalcSpriteWidthSub(height + growH, 3);
        pt[1].x = itfCalcSpriteWidthSub(pt[0].x - growW * 0.5f, 4);
        pt[1].y = itfCalcSpriteWidthSub(pt[0].y - growH * 0.5f, 3);
        effDrawSpriteWithCornerFade(pt[1].x, pt[1].y, 0, alpha, 0, spr, 0, 103);
        effDrawSpriteWithCornerFade(pt[0].x * 16, pt[0].y * 8, 0, 0xFF, 0, spr, 1, 103);
        if (str != NULL) {
            itfDrawFont(ITF_DM_STYLE_SFNT_CNV, pt[0].x, pt[0].y, 0x100000, 0, 0, 0xA0A0A0A0, 1.0f, 103, str);
        }
    }
    if (++g_cntTgtCur >= 60) {
        g_cntTgtCur = 0;
    }
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlTargetCursor);
#endif

void sbtlResetTargetCursor(void) {
    g_cntTgtCur = 0;
    gTargetPosHokanFastFlag = 1;
}

s32 sbtlSelectTargetCursorSub(struct SBTLUNIT_WORK *wk, s8 bDrawName) {
    sdfVector4_t pos;

    if (wk != NULL && CheckLiveTarget(wk)) {
        if (wk->flg & 0x800) {
            SDF_VU0_LOADVECTOR(&wk->pos);
            SDF_VU0_STOREVECTOR_MEMOPS(pos);
            if (wk->unit.devilId == 685) {
                pos.y -= 600.0f;
                pos.x += 32.0f;
            }
        } else {
            sdfModelEvalTm(wk->hEveUnit->owner->inner);
            sdfModelGetMarkPos_sdfModel(wk->hEveUnit->owner->inner, 0);
            SDF_VU0_STOREVECTOR_MEMOPS(pos);
        }
        if (bDrawName) {
            if (wk->unit.devilId) {
                sbtlTargetCursor(&pos, g_itemNo[wk->unit.devilId]);
            } else {
                sbtlTargetCursor(&pos, sbtlGetPlayerName());
            }
        } else {
            sbtlTargetCursor(&pos, NULL);
        }
    }
    return 1;
}

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

/* NON_MATCHING: 87 of 93 words match.  */
#ifdef NON_MATCHING
void sbtlHelpEx(s32 a_type, s32 a_num, struct sprHandle_t *a_hSpr, u8 a_alphaRate) {
    frTextBank_t *bank = NULL;
    frFontHandle_t *frq;

    switch (a_type) {
    case 1:
        bank = cmpCallGetProcessData12Data;
        break;
    case 2:
        bank = sbtlData165;
        break;
    case 3:
        bank = &sbtlData131;
        break;
    case 4:
        bank = &sbtlData132;
        break;
    case 5:
        bank = &sbtlData133;
        break;
    case 6:
        bank = &sbtlData134;
        break;
    }
    if (bank) {
        itfDrawMiscSpriteAlpha(190, 352, 0, a_alphaRate, a_hSpr, 36, DDS3_OT_3DHIGH_INIT | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(203, 367, 0, a_alphaRate, a_hSpr, 38, DDS3_OT_3DHIGH_INIT | DDS3_OT_BACKEX_NORMAL);
        frq = itfDrawBankTextWithLayoutFlags(3360, 3056, 0, a_num, bank, 1);
        frSetFontColor(frq, ((a_alphaRate << 7) / 255U) | 0xA09DC300);
        frDrawFontOt(frq, 1, 0x69);
        frDestroyFontRequestAfter(frq);
    }
}
#else
INCLUDE_ASM(const s32, "code_002F5CF0", sbtlHelpEx);
#endif

void sbtlHelp(s32 a_type, s32 a_num, u8 a_alphaRate) {
    sbtlHelpEx(a_type, a_num, wk_sbtl->panel->hPanelSpr3, a_alphaRate);
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlHelpCantUseEx);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlHelpCantUse);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlEscapeGauge);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlResetEscapeGauge);

void sbtlDamagePanelE(const sdfVector4_t *pos, s32 dmg, u32 rgba, u32 rgba2) {
    sdfVector4_t v;
    SBTLPROJECTEDSCREEN_POINT point;
    char str[32];

    if (sbtlGetCutFlg() & 1) {
        return;
    }
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR_MEMOPS(v);
    v.y -= 40.0f;
    SDF_VU0_LOADVECTOR_MEMOPS(v);
    if (btlProjectForwardPositionToScreen(&point)) {
        sprintf(str, sbtlData175, dmg);
        if ((u32)(point.x - 472) < 41) {
            point.x = 472;
        }
        sbtlDrawNum4(point.x, point.y, rgba, 255, dmg, 103, 0xF0101080, 1.0f);
    }
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDamagePanelN);

void sbtlDamagePanelR(const sdfVector4_t *pos, s32 dmg, u32 rgba, u32 rgba2) {
    struct SBTLUNIT_WORK *unit;
    sdfVector4_t v;
    SBTLPROJECTEDSCREEN_POINT point;
    char str[32];

    if (sbtlGetCutFlg() & 1) {
        return;
    }
    unit = sbtlSearchUnit_ID(wk_sbtl->unitList, wk_sbtl->targetUnitID);
    if (unit != NULL) {
        sdfModelEvalTm(unit->hEveUnit->owner->inner);
        if (!sdfModelGetMarkPos_sdfModel(unit->hEveUnit->owner->inner, 1)) {
            sdfModelGetMarkPos_sdfModel(unit->hEveUnit->owner->inner, 0);
        }
    } else {
        SDF_VU0_LOADVECTOR(pos);
    }
    SDF_VU0_STOREVECTOR_MEMOPS(v);
    v.y = -120.0f;
    SDF_VU0_LOADVECTOR_MEMOPS(v);
    if (btlProjectForwardPositionToScreen(&point)) {
        sprintf(str, sbtlData175, dmg);
        if ((u32)(point.x - 472) < 41) {
            point.x = 472;
        }
        sbtlDrawNum4(point.x, point.y, rgba, 255, dmg, 103, 0x1010F080, 1.0f);
    }
}

void DrawTime(void) {
    struct sprHandle_t *hSpr = sbtlData135.hSpr;
    char str[32];
    s32 time;
    s32 min;
    s32 sec;
    s32 i;
    s32 x;

    if (wk_sbtl->status4 & 0x10) {
        return;
    }
    DrawTimeData++;
    if (wk_sbtl->timeLimit) {
        time = wk_sbtl->timeLimit - gb_btltime;
        if (time < 0) {
            time = 0;
        }
    } else {
        time = gb_btltime;
    }
    if (!wk_sbtl->field42CE) {
        return;
    }
    min = time / 3600;
    sec = time / 60 % 60;
    sprintf(str, DrawTimeData2, min);
    for (i = 0, x = 8; i < 2; i++, x += 32) {
        itfDrawMiscSpriteAlpha(x, 384, 0, 255, hSpr, (u8)str[i] - 21, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    itfDrawMiscSpriteAlpha(72, 384, 0, 255, hSpr, 37, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    sprintf(str, DrawTimeData2, sec);
    for (i = 0, x = 104; i < 2; i++, x += 32) {
        itfDrawMiscSpriteAlpha(x, 384, 0, 255, hSpr, (u8)str[i] - 21, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    itfDrawMiscSpriteAlpha(168, 384, 0, 255, hSpr, 38, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData45);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetProcessData55);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncDecideItem);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlEscapeGaugeTable);

INCLUDE_RODATA(const s32, "code_002F5CF0", awdStrNegotiate);

INCLUDE_RODATA(const s32, "code_002F5CF0", awdStrBeginBattle);

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

INCLUDE_RODATA(const s32, "code_002F5CF0", cmpStr);

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

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrBattleBgeplBgHiEpl);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrBattleBgeplBgIceEpl);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrBattleBgeplBgThEpl);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrBattleBgeplBgWiEpl);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlStrBattleBgeplBgMudEpl);

INCLUDE_RODATA(const s32, "code_002F5CF0", sbtlCallFileOpen2Table);

