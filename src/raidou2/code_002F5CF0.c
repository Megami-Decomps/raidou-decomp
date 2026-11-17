#include "common.h"
#include "dds3KernelCore.h"
#include "sbtlWork.h"
#include "sBtlCalc.h"
#include "sBtlDraw.h"
#include "frFont.h"
#include "sBtlMenu.h"
#include "code_r1_001CC1D0.h"

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

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncKatana);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawTargetCursorNakama);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawTargetCursorEnemy);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawTargetCharList);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlReturnAL);

INCLUDE_ASM(const s32, "code_002F5CF0", summonMenuResetMenu);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlCallEndSub);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCheckEnableTalk);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSetMenuImage);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlInitMenuPID);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawFuncTalk);

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

INCLUDE_ASM(const s32, "code_002F5CF0", ChkEnableDecideEscape);

INCLUDE_ASM(const s32, "code_002F5CF0", TutorialMenuCtrl);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlChkEnableDecideMenu);

INCLUDE_ASM(const s32, "code_002F5CF0", Init_sbtlMenu);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallMenuClearMenuAll);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallProcEnd);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCheckProcEnd);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallEnd);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlChkCallEnd);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlFuncDecideItem_sbtlMenu);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlDrawProcessSub);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawProcess);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawProcess_sbtlMenu);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallMemFreeDelayedDirect);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCampSubSpr);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlIsMenuStartScript);

INCLUDE_ASM(const s32, "code_002F5CF0", ProcessStart_sbtlMenu);

INCLUDE_ASM(const s32, "code_002F5CF0", IsStart);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSetSummonMenuSub);

INCLUDE_ASM(const s32, "code_002F5CF0", ProcessStart);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlProcessEnd);

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

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlGetNode);

INCLUDE_ASM(const s32, "code_002F5CF0", GetActionUnit);

INCLUDE_ASM(const s32, "code_002F5CF0", GetTargetUnit);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlFuncDecideItemSBtlMenuSub);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlFuncDecideItemSBtlMenuSub2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawTabPanel);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBtlDrawProcessSBtlMenuSub);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlInitAddMag);

INCLUDE_ASM(const s32, "code_002F5CF0", SetAddMag);

INCLUDE_ASM(const s32, "code_002F5CF0", DrawAddMag);

void sbtlGetIdling_HD(s32 a_x, s32 a_y, u8 a_alphaRate, s32 a_otNo, struct sdatUnitWork_t *unit) {
    if (unit != NULL) {
        itfIconDrawBadStatus(a_x, a_y, 0, a_alphaRate, sbtlGetBadStatus(unit), a_otNo);
    }
}

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlMoonOff);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlBarCtrl);

INCLUDE_ASM(const s32, "code_002F5CF0", DrawStatusBar);

INCLUDE_ASM(const s32, "code_002F5CF0", btlLoadInterfaceSprite);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallBtlCheckTutorialBattle3);

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

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlInitSBtlSub2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlInitDevil);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlGetRaceName);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSpriteEx);

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

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetFigure);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlAIUpdateSub);

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

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallGetSmallFontFRQ3);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlNumFont);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlDrawRedGg);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallCheckDying);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallSprintf3);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlPlayerData);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSummonDevilAshimotoReset);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSummonDevilAshimoto);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSummonDevilData_Sub);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSummonDevilData);

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

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlResetTargetCursor);

INCLUDE_ASM(const s32, "code_002F5CF0", NewLogSave);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlSelectTargetCursor);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlHelpEx);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlHelp);

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

INCLUDE_ASM(const s32, "code_002F5CF0", fldReleaseTitleMiniTexture);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallFileOpen2);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallCreateEffPartsList);

INCLUDE_ASM(const s32, "code_002F5CF0", sbtlCallPartsCreateFinish2);
