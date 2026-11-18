#include "common.h"
#include "code_r1_002C8188.h"
#include "rstInit.h"
#include "sdfChip.h"
#include "TitleMain.h"
#include "jmsMisc.h"
#include "sdk_libc.h"
#include "dds3KernelCore.h"
#include "code_0034F3B8.h"
#include "gohMain.h"
#include "code_003BF2E0.h"
#include "cmpMain.h"

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData27);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData28);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData29);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBlendColor);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetSpriteHandle);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf17);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData30);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData31);

INCLUDE_ASM(const s32, "code_003BF2E0", ListDrawStation);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData32);

INCLUDE_ASM(const s32, "code_003BF2E0", CheckFlagTGS);

INCLUDE_ASM(const s32, "code_003BF2E0", SetRootMenu);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallCheckFlagTGS);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBitCheck2);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBitCheck3);

INCLUDE_ASM(const s32, "code_003BF2E0", IsBuyItem);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBitCheck4);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallMenuGetMenuNum);

INCLUDE_ASM(const s32, "code_003BF2E0", CallProcEnd);

INCLUDE_ASM(const s32, "code_003BF2E0", DrawProcess_jmsMenu);

INCLUDE_ASM(const s32, "code_003BF2E0", Process_jmsMenu);

INCLUDE_ASM(const s32, "code_003BF2E0", RunBadEffKaifukuKouka);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpMenuShutdown2);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData33);

s32 cmpGetMarkID(void)
{
    struct tagCMP_WORK *wk = dds3GetProcessData(inputpanel_id);

    return wk->markID;
}

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData34);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallGetProcessData7);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsSetFocus);

INCLUDE_ASM(const s32, "code_003BF2E0", ProcessStart_jmsMenu);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallGetProcessData8);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallGetProcessData9);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsMenuMain);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsMenuProcessEnd);

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

INCLUDE_ASM(const s32, "code_003BF2E0", jmsDisableSelect);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallGetProcessData10);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsAddTalkTarget);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallGetProcessData11);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallBitCheck2);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCheckMovePrev);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsGetCursorPosID);

s32 jmsLeaf9(void) {
    return jmsLeaf11Data;
}

INCLUDE_ASM(const s32, "code_003BF2E0", jmsLeaf10);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFirstInitSub);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsLeaf11);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsJimusyoJmsQuest);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc17);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc18);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsListDrawStationSub);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc19);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc20);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsListDrawStationSub2);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsListDrawStationSub3);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc21);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc22);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc23);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc24);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsListDrawStationSub4);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallBitOn);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc25);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc26);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc27);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc28);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc29);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc30);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc31);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc32);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsSortSettingSub);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc33);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc34);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsListDrawStationSub5);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc35);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc36);

void jmsCallMemset(jmsMemsetWork_t *work) {
    memset(work, 0, sizeof(jmsMemsetWork_t));
    work->unk00 = 0;
    work->unk04 = -1;
    work->unk08 = -1;
    work->unk0C = 0;
}

INCLUDE_ASM(const s32, "code_003BF2E0", jmsLeaf12);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsLeaf13);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc37);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallDevLoadFileEx2);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallMemFreeDirect);

INCLUDE_ASM(const s32, "code_003BF2E0", sysCallDevLoadFileEx);

INCLUDE_ASM(const s32, "code_003BF2E0", sysCallMemFreeDirect);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf18);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf19);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBitCheck5);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf20);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBitCheck6);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBitCheck7);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf21);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf22);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallMessageNumSearch);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallMessageNumSearch2);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallMessageNumSearch3);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallSprintf);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallSprintf2);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallSetVariableRequest);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallSetVariableRequest2);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf23);

INCLUDE_ASM(const s32, "code_003BF2E0", RewardMsgDraw);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpRefDDS3GBWK7);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBtlGetPlayerData);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpFunc2);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBitCheck8);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBitCheck9);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf24);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBitCheck10);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf25);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf26);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBitCheck11);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf27);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf28);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBitCheck12);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf29);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallProcCallSisetuReturn);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf30);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf31);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf32);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallGlobalWorkUserInit2);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallGetProcessData17);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData36);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpStartProcessPanelEff);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpStorySelectDebug);

INCLUDE_ASM(const s32, "code_003BF2E0", rstStorySelectDebug);

INCLUDE_ASM(const s32, "code_003BF2E0", rstLoadMapResource);

INCLUDE_ASM(const s32, "code_003BF2E0", rstMapReturnChkXFade);

INCLUDE_ASM(const s32, "code_003BF2E0", rstLeaf13);

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

INCLUDE_ASM(const s32, "code_003BF2E0", rstEndNext);

INCLUDE_ASM(const s32, "code_003BF2E0", rstDevilGiftCalc);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCalcRankUp);

INCLUDE_ASM(const s32, "code_003BF2E0", rstPreCalcUnit);

INCLUDE_ASM(const s32, "code_003BF2E0", Init_rstInit);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultDraw);

INCLUDE_ASM(const s32, "code_003BF2E0", rstProcessStartRstInitSub);

INCLUDE_ASM(const s32, "code_003BF2E0", ProcessStart_rstInit);

INCLUDE_ASM(const s32, "code_003BF2E0", ProcessEnd_rstInit);

s32 ProcessCheck_rstInit(void) {
    return dds3SearchProcessName(cmpCalcProcName) != 0;
}

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultStartWaitProcess);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultLoadWaitProcess);

INCLUDE_ASM(const s32, "code_003BF2E0", rstSetReward);

void rstSetEncountDlcID(s32 EncountNo) {
    rstCallBitCheckWork = EncountNo;
}

s32 rstGetGlobalEncountNo(void) {
    return rstCallBitCheckWork;
}

INCLUDE_ASM(const s32, "code_003BF2E0", rstSetFlag);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCheckFlag);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultStart);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultFadeCheck);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallGetProcessData36);

INCLUDE_ASM(const s32, "code_003BF2E0", rstGetGiftItem);

INCLUDE_ASM(const s32, "code_003BF2E0", rstGetGiftItemNums);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCheckRankUp);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCalcProcess);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallGetGlobalEncountNo);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCalcProcess_rstResult);

dds3ProcessFunc_t rstDrawProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    rstCallGetStockDevilNums(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t rstCallGetProcessData37(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

s32 rstLeaf14(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_003BF2E0", ReleaseFontList);

INCLUDE_ASM(const s32, "code_003BF2E0", rstEndCalcProcess);

dds3ProcessFunc_t rstEndDrawProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    rstCallGetStockDevilNums(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t rstEndUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_003BF2E0", rstBtlEndWaitInit);

s32 rstLeaf15(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_003BF2E0", rstBtlEndWaitCalcProcess);

dds3ProcessFunc_t rstBtlEndWaitDrawProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    rstCallGetStockDevilNums(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t rstCallGetProcessData38(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

s32 rstLeaf16(void)
{
    return 1;
}

s32 rstLeaf17(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallGetProcessData39);

dds3ProcessFunc_t rstCallGetProcessData40(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t rstCallGetProcessData41(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultCyuuseiGaugeSub);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultNakamaLevelSub);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultLevelUpAnimSub);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultGetExpCyuuseiSub);

s32 rstCalcAlphaRateSub(s32 value, s32 low, s32 high) {
    if (value >= low && value < high) {
        return 1;
    }
    return 0;
}

s32 rstCalcAlphaRate(s32 frame, const titleKey_t *key, s32 count) {
    s32 i;

    for (i = 0; i < count - 1; i++) {
        if (rstCalcAlphaRateSub(frame, key[i].frame, key[i + 1].frame)) {
            break;
        }
    }
    frame -= key[i].frame;
    return (key[i].value + (key[i + 1].value - key[i].value) * frame / (key[i + 1].frame - key[i].frame)) * 2;
}

INCLUDE_ASM(const s32, "code_003BF2E0", rstCalcScale);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCalcPosition);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallCalcAlphaRate);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultGetExpCyuusei);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultExpGauge);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultAddLevelUpAnimQue);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultLevelUpAnim);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallBlendColor2);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultAddCyuuseiMaxAnimQue);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultCyuuseiMaxAnim);

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultPlayerLevel);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallBlendColor3);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallBlendColor4);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallGetStockDevilNums);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallGetRand);

INCLUDE_ASM(const s32, "code_003BF2E0", rstLeaf9);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallPlaySE);

INCLUDE_ASM(const s32, "code_003BF2E0", rstNmlskl);

INCLUDE_ASM(const s32, "code_003BF2E0", rstMemSkl);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallPlaySE2);

INCLUDE_ASM(const s32, "code_003BF2E0", rstMenuCursorCtrlSub);

INCLUDE_ASM(const s32, "code_003BF2E0", MapMoveCalc);

INCLUDE_ASM(const s32, "code_003BF2E0", rstOmoideGaIppai);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallProcEnd);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallProcessCheckCmpStatus);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallKillProcess);

INCLUDE_ASM(const s32, "code_003BF2E0", MenuCursorCtrl);

INCLUDE_ASM(const s32, "code_003BF2E0", rstGetGiftMsgPID);

INCLUDE_ASM(const s32, "code_003BF2E0", rstSetFlagPlayBgm);

INCLUDE_ASM(const s32, "code_003BF2E0", evtCheckPolygonMovieRun);

INCLUDE_ASM(const s32, "code_003BF2E0", evtBgnNum);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLoadLocalizeText);

INCLUDE_ASM(const s32, "code_003BF2E0", rstInit);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallGetProcessData42);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCmpItemGrowthStart);

INCLUDE_ASM(const s32, "code_003BF2E0", rstProcessEnd);

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

void rstCloseStreamSub(void) {
}

INCLUDE_RODATA(const s32, "code_003BF2E0", rstStrResult);

INCLUDE_RODATA(const s32, "code_003BF2E0", rstStrRsHaikeiSpr);

