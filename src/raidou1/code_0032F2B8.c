#include "common.h"
#include "dds3KernelCore.h"
#include "code_0032F2B8.h"
#include "shpMain.h"
#include "sbtlTubeSupport.h"

INCLUDE_ASM(const s32, "code_0032F2B8", shpRefShpMessage);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallRagTestSub);

INCLUDE_ASM(const s32, "code_0032F2B8", shpStartScript);

INCLUDE_ASM(const s32, "code_0032F2B8", shpNewsStartScript);

INCLUDE_ASM(const s32, "code_0032F2B8", shpOrderStartScript);

INCLUDE_ASM(const s32, "code_0032F2B8", CallRagTest);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallGetProcessStat);

INCLUDE_ASM(const s32, "code_0032F2B8", shpMsgInit);

INCLUDE_ASM(const s32, "code_0032F2B8", shpMsgEnd);

INCLUDE_ASM(const s32, "code_0032F2B8", shpRefShpAction);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallPlaySE);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallPlaySE2);

INCLUDE_ASM(const s32, "code_0032F2B8", shpSelectListPage);

INCLUDE_ASM(const s32, "code_0032F2B8", shpRefShpAction2);

INCLUDE_ASM(const s32, "code_0032F2B8", FadeIn);

INCLUDE_ASM(const s32, "code_0032F2B8", SetStatFadeOut);

INCLUDE_ASM(const s32, "code_0032F2B8", FadeOut);

INCLUDE_ASM(const s32, "code_0032F2B8", shpListUpdateBuySelNum);

INCLUDE_ASM(const s32, "code_0032F2B8", shpListCtrlBuySelItem);

INCLUDE_ASM(const s32, "code_0032F2B8", shpListCtrlSelNum);

INCLUDE_ASM(const s32, "code_0032F2B8", UpdateInit);

INCLUDE_ASM(const s32, "code_0032F2B8", UpdateBuyReturnList);

INCLUDE_ASM(const s32, "code_0032F2B8", shpUpdateBuy);

INCLUDE_ASM(const s32, "code_0032F2B8", shpListUpdateSellSelectNum);

INCLUDE_ASM(const s32, "code_0032F2B8", shpListCtrlSellSelectItem);

INCLUDE_ASM(const s32, "code_0032F2B8", UpdateSellReturnList);

INCLUDE_ASM(const s32, "code_0032F2B8", shpUpdateSell);

INCLUDE_ASM(const s32, "code_0032F2B8", shpListCtrlTradeSelectDevil);

INCLUDE_ASM(const s32, "code_0032F2B8", shpUpdateTrade);

INCLUDE_ASM(const s32, "code_0032F2B8", shpListCtrlOrderSelectItem);

INCLUDE_ASM(const s32, "code_0032F2B8", shpUpdateOrder);

INCLUDE_ASM(const s32, "code_0032F2B8", EnterSetMsg);

INCLUDE_ASM(const s32, "code_0032F2B8", shpUpdateEnter);

INCLUDE_ASM(const s32, "code_0032F2B8", shpUpdateLeaveMilkHall);

INCLUDE_ASM(const s32, "code_0032F2B8", shpUpdateLeaveMilkHall_shpAction);

INCLUDE_ASM(const s32, "code_0032F2B8", rstGetEncycPanel);

INCLUDE_ASM(const s32, "code_0032F2B8", TalkSetMotionStart);

INCLUDE_ASM(const s32, "code_0032F2B8", shpUpdateTalk);

INCLUDE_ASM(const s32, "code_0032F2B8", shpUpdateTalk_shpAction);

INCLUDE_ASM(const s32, "code_0032F2B8", shpUpdateNews);

INCLUDE_ASM(const s32, "code_0032F2B8", shpUpdateGoma);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallButtonLineHelpDispSet);

INCLUDE_ASM(const s32, "code_0032F2B8", shpPoint3To2);

INCLUDE_ASM(const s32, "code_0032F2B8", knyGetGoodPoint);

INCLUDE_ASM(const s32, "code_0032F2B8", knySetGoodPoint);

INCLUDE_ASM(const s32, "code_0032F2B8", knyGetGoodRank);

INCLUDE_ASM(const s32, "code_0032F2B8", knyAddGoodPoint);

INCLUDE_ASM(const s32, "code_0032F2B8", knyCtrlGoodRank);

INCLUDE_ASM(const s32, "code_0032F2B8", nwdGetGoodPoint);

INCLUDE_ASM(const s32, "code_0032F2B8", evtBitKityouItemAdd);

INCLUDE_ASM(const s32, "code_0032F2B8", nwdGetGoodRank);

INCLUDE_ASM(const s32, "code_0032F2B8", nwdAddGoodPoint);

INCLUDE_ASM(const s32, "code_0032F2B8", nwdCtrlGoodRank);

INCLUDE_ASM(const s32, "code_0032F2B8", shpAddGoodPointBuy);

INCLUDE_ASM(const s32, "code_0032F2B8", shpAddGoodPointSell);

INCLUDE_ASM(const s32, "code_0032F2B8", shpAddGoodPointOrder);

INCLUDE_ASM(const s32, "code_0032F2B8", shpAddGoodPointTrade);

INCLUDE_ASM(const s32, "code_0032F2B8", knyGetPriceBuy);

INCLUDE_ASM(const s32, "code_0032F2B8", knyGetPriceSell);

INCLUDE_ASM(const s32, "code_0032F2B8", nwdGetPriceOrder);

INCLUDE_ASM(const s32, "code_0032F2B8", nwdGetMagTrade);

INCLUDE_ASM(const s32, "code_0032F2B8", shpMiscQuickSort);

INCLUDE_ASM(const s32, "code_0032F2B8", shpGetChapter);

INCLUDE_ASM(const s32, "code_0032F2B8", shpChkArrivalChapter);

INCLUDE_ASM(const s32, "code_0032F2B8", knyChkArrivalChapter);

INCLUDE_ASM(const s32, "code_0032F2B8", nwdChkArrivalChapter);

INCLUDE_ASM(const s32, "code_0032F2B8", knyChkArrivalRank);

INCLUDE_ASM(const s32, "code_0032F2B8", nwdChkArrivalRank);

INCLUDE_ASM(const s32, "code_0032F2B8", shpSetItemList);

INCLUDE_ASM(const s32, "code_0032F2B8", _shpChkNewArrival);

INCLUDE_ASM(const s32, "code_0032F2B8", shpChkNewArrival);

INCLUDE_ASM(const s32, "code_0032F2B8", shpLeaf2);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallDGetPrimPacketSize);

INCLUDE_ASM(const s32, "code_0032F2B8", FloatToSprX);

INCLUDE_ASM(const s32, "code_0032F2B8", FloatToSprY);

INCLUDE_ASM(const s32, "code_0032F2B8", LineWide);

INCLUDE_ASM(const s32, "code_0032F2B8", GetPosBezierCurve);

INCLUDE_ASM(const s32, "code_0032F2B8", shpDrawBezierCurveWide);

INCLUDE_ASM(const s32, "code_0032F2B8", shpDrawStraightLineWide);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallGetRandIntA);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallMemset);

INCLUDE_ASM(const s32, "code_0032F2B8", shpErrorShpDraw);

s32 shpLeaf3(void) {
    return shpGetSelectItemID();
}

s32 shpLeaf4(void) {
    return shpGetModelHandle();
}

s32 shpLeaf5(void) {
    return shpSetModelMotion();
}

s32 shpLeaf6(void) {
    return shpCallRelease();
}

s32 nwdLeaf2(void) {
    return shpWaitRelease();
}

s32 nwdLeaf3(void) {
    return nwdGetSelectModelID();
}

s32 nwdLeaf4(void) {
    return knyGetIntimateRank();
}

void nwdLeaf5(void) {
    shpCallProcEnd();
}

INCLUDE_ASM(const s32, "code_0032F2B8", nwdRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_0032F2B8", nwdCallBitCheck);

INCLUDE_ASM(const s32, "code_0032F2B8", nwdLoadModel);

INCLUDE_ASM(const s32, "code_0032F2B8", shpMenuChkSelect);

INCLUDE_ASM(const s32, "code_0032F2B8", shpSetMenuMilkHall);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallSetParam);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallSetParam2);

INCLUDE_ASM(const s32, "code_0032F2B8", shpSetMoveBack);

INCLUDE_ASM(const s32, "code_0032F2B8", shpSetMoveBackDef);

INCLUDE_ASM(const s32, "code_0032F2B8", shpUpdateBack);

INCLUDE_ASM(const s32, "code_0032F2B8", shpInitFunc);

INCLUDE_ASM(const s32, "code_0032F2B8", shpInitBack);

INCLUDE_ASM(const s32, "code_0032F2B8", shpProcess);

INCLUDE_ASM(const s32, "code_0032F2B8", shpErrorShpMain);

INCLUDE_ASM(const s32, "code_0032F2B8", shpSetColorModel);

INCLUDE_ASM(const s32, "code_0032F2B8", shpDrawBack);

INCLUDE_ASM(const s32, "code_0032F2B8", shpDrawGoodPoint);

INCLUDE_ASM(const s32, "code_0032F2B8", shpDrawProcess);

INCLUDE_ASM(const s32, "code_0032F2B8", datGetItemNumEquipMinus);

INCLUDE_ASM(const s32, "code_0032F2B8", shpSetZoom);

INCLUDE_ASM(const s32, "code_0032F2B8", shpLeaf7);

INCLUDE_ASM(const s32, "code_0032F2B8", shpSetStatDraw);

INCLUDE_ASM(const s32, "code_0032F2B8", shpGetSelectItemID);

INCLUDE_ASM(const s32, "code_0032F2B8", shpGetModelHandle);

INCLUDE_ASM(const s32, "code_0032F2B8", shpSetModelMotion);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallRelease);

INCLUDE_ASM(const s32, "code_0032F2B8", shpWaitRelease);

INCLUDE_ASM(const s32, "code_0032F2B8", nwdGetSelectModelID);

INCLUDE_ASM(const s32, "code_0032F2B8", knyGetIntimateRank);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallProcEnd);

INCLUDE_ASM(const s32, "code_0032F2B8", shpLoadModel);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallProcEnd_shpMain);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCheckProcEnd);

INCLUDE_ASM(const s32, "code_0032F2B8", shpCallUpdateStatusShutdown);

INCLUDE_ASM(const s32, "code_0032F2B8", ProcessStart_shpMain);

INCLUDE_ASM(const s32, "code_0032F2B8", ProcessStart);

INCLUDE_ASM(const s32, "code_0032F2B8", ProcessEnd_shpMain);

s32 ProcessCheck_shpMain(void) {
    if (ProcessCheck_shpMainData == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(ProcessCheck_shpMainData) == 3 || dds3GetProcessStat(ProcessCheck_shpMainData) == 0) {
        ProcessCheck_shpMainData = NULL;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_0032F2B8", shpRootCallProcEnd);

INCLUDE_ASM(const s32, "code_0032F2B8", shpRootCheckProcEnd);

INCLUDE_ASM(const s32, "code_0032F2B8", shpRootProcess);

INCLUDE_ASM(const s32, "code_0032F2B8", shpRootShutdown);

INCLUDE_ASM(const s32, "code_0032F2B8", shpProcessStart);

INCLUDE_ASM(const s32, "code_0032F2B8", shpLeaf8);

INCLUDE_ASM(const s32, "code_0032F2B8", fldLeaf35);

INCLUDE_ASM(const s32, "code_0032F2B8", fldShowSceneModels);

s32 fldCallGetProcessStat(void) {
    if (fldCallGetProcessStatData == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(fldCallGetProcessStatData) == 3 || dds3GetProcessStat(fldCallGetProcessStatData) == 0) {
        fldCallGetProcessStatData = NULL;
        return 0;
    }
    return 1;
}
