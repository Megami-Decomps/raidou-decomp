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

INCLUDE_ASM(const s32, "code_0034F3B8", DrawProcess_jmsMenu);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallGetProcessData4);

INCLUDE_ASM(const s32, "code_0034F3B8", Release_jmsMenu);

INCLUDE_ASM(const s32, "code_0034F3B8", Shutdown);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallGetProcessData5);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallGetProcessData6);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsStart);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsSetFocus);

INCLUDE_ASM(const s32, "code_0034F3B8", ProcessStart_jmsMenu);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsMenuMain);

INCLUDE_ASM(const s32, "code_0034F3B8", ProcessEnd);

INCLUDE_ASM(const s32, "code_0034F3B8", ProcessCheck_jmsMenu);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsDisableSelect);

INCLUDE_ASM(const s32, "code_0034F3B8", SearchBasicFromNameIndex_evtEventViewer);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallBitCheck2);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCheckMovePrev);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsGetCursorPosID);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallGlobalWorkUserInit2);

INCLUDE_ASM(const s32, "code_0034F3B8", jmsCallGetProcessData17);

INCLUDE_ASM(const s32, "code_0034F3B8", cmpCallGetProcessData36);

INCLUDE_ASM(const s32, "code_0034F3B8", cmpCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_0034F3B8", cmpStartProcessPanelEff);

INCLUDE_ASM(const s32, "code_0034F3B8", cmpStorySelectDebug);

INCLUDE_ASM(const s32, "code_0034F3B8", rstStorySelectDebug);

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

INCLUDE_ASM(const s32, "code_0034F3B8", ProcessEnd_rstInit);

INCLUDE_ASM(const s32, "code_0034F3B8", ProcessCheck_rstInit);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultStartWaitProcess);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultLoadWaitProcess);

INCLUDE_ASM(const s32, "code_0034F3B8", rstSetReward);

INCLUDE_ASM(const s32, "code_0034F3B8", rstSetEncountDlcID);

INCLUDE_ASM(const s32, "code_0034F3B8", rstGetGlobalEncountNo);

INCLUDE_ASM(const s32, "code_0034F3B8", rstSetFlag);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCheckFlag);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultStart);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultFadeCheck);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetProcessData36);

INCLUDE_ASM(const s32, "code_0034F3B8", rstGetGiftItem);

INCLUDE_ASM(const s32, "code_0034F3B8", rstGetGiftItemNums);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCheckRankUp);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCalcProcess);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallLoadMapResourceFunc);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetProcessData3);

INCLUDE_ASM(const s32, "code_0034F3B8", rstDrawProcess);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetProcessData37);

INCLUDE_ASM(const s32, "code_0034F3B8", rstLeaf14);

INCLUDE_ASM(const s32, "code_0034F3B8", rstLeaf4);

INCLUDE_ASM(const s32, "code_0034F3B8", rstEndCalcProcess);

INCLUDE_ASM(const s32, "code_0034F3B8", rstEndDrawProcess);

INCLUDE_ASM(const s32, "code_0034F3B8", rstEndUpdateProcess);

INCLUDE_ASM(const s32, "code_0034F3B8", rstBtlEndWaitInit);

INCLUDE_ASM(const s32, "code_0034F3B8", rstLeaf15);

INCLUDE_ASM(const s32, "code_0034F3B8", rstBtlEndWaitCalcProcess);

INCLUDE_ASM(const s32, "code_0034F3B8", rstBtlEndWaitDrawProcess);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetProcessData38);

INCLUDE_ASM(const s32, "code_0034F3B8", rstLeaf16);

INCLUDE_ASM(const s32, "code_0034F3B8", rstLeaf17);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetProcessData39);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetProcessData40);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetProcessData41);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultCyuuseiGaugeSub);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultNakamaLevelSub);

INCLUDE_ASM(const s32, "code_0034F3B8", rstResultLevelUpAnimSub);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCalcAlphaRateSub);

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

INCLUDE_ASM(const s32, "code_0034F3B8", rstGetGiftMsgPID);

INCLUDE_ASM(const s32, "code_0034F3B8", rstSetFlagPlayBgm);

INCLUDE_ASM(const s32, "code_0034F3B8", evtCheckPolygonMovieRun);

INCLUDE_ASM(const s32, "code_0034F3B8", evtCallCheckRankUp);

INCLUDE_ASM(const s32, "code_0034F3B8", rstInit);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetProcessData42);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCmpItemGrowthStart);

INCLUDE_ASM(const s32, "code_0034F3B8", rstProcessEnd);

INCLUDE_ASM(const s32, "code_0034F3B8", rstCallGetProcessStat);

/* body: matched in smt3 @ 9401f79e: src/smt3/sdf/sdfSystem.c:261, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void rstCloseStreamSub(void) {
}
