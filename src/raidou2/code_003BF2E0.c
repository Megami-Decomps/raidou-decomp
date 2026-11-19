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
#include "itf.h"
#include "rstResult.h"
#include "sdfMemory.h"
#include "EventBit.h"
#include "jmsMenu.h"
#include "code_r1_0010DF68.h"
#include "fld.h"
#include "fldProcess.h"
#include "sndAt3.h"
#include "sbtlWork.h"
#include "dds3PadManager.h"
#include "rstGrowth.h"
#include "code_003B7840.h"
#include "dat.h"

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData27);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData28);

void cmpCallGetProcessData29(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_fontColor, u8 a_alphaRate) {
    jmsInputPanelWork_t *work = dds3GetProcessData(inputpanel_id);
    u8 alpha = GetSelectRootID(work->menuPID, 0);

    itfDrawMiscSpriteAlpha(29, 38, 0, alpha, work->hSpr, jmsAnimFunc() == 0 ? 103 : 107,
                           DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
    cmpCallGetProcessData28(a_id, a_x, a_y, a_z, a_fontColor, a_alphaRate);
}

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBlendColor);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetSpriteHandle);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpLeaf17);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData30);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData31);

INCLUDE_ASM(const s32, "code_003BF2E0", ListDrawStation);

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData32);

/* NON_MATCHING: 9 of 18 words match.  */
#ifdef NON_MATCHING
s32 CheckFlagTGS(s32 MenuID) {
    if (!evtBitCheck(864)) {
        return 0;
    }
    if (MenuID == 3 || MenuID == 5) {
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", CheckFlagTGS);
#endif

/* NON_MATCHING: 24 of 38 words match.  */
#ifdef NON_MATCHING
void SetFlagMenu(dds3ProcessID_t *PID, s32 MenuID, ITFMENU_CB_GF Flag, s8 Bool) {
    u16 flag = SetRootMenuData[MenuID];

    if (!CheckFlagTGS(MenuID) && (flag == 0 || evtBitCheck(flag))) {
        itfMenuSetFlagMenu(PID, MenuID, Flag, Bool);
    }
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", SetFlagMenu);
#endif

/* NON_MATCHING: 22 of 37 words match.  */
#ifdef NON_MATCHING
void SetRootMenu(dds3ProcessID_t *PID, s32 MenuID, itfMenuFunc_t func) {
    u16 flag = SetRootMenuData[MenuID];

    if (!CheckFlagTGS(MenuID) && (flag == 0 || evtBitCheck(flag))) {
        itfMenuSetRootMenu(PID, MenuID, cmpData34[MenuID], func);
    }
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", SetRootMenu);
#endif

void SetMenu(dds3ProcessID_t *PID, s32 MenuID, s32 ParentMenuID, itfMenuFunc_t func) {
    u16 flag = SetRootMenuData[MenuID];

    if (flag == 0 || evtBitCheck(flag)) {
        itfMenuSetMenu(PID, MenuID, ParentMenuID, cmpData34[MenuID], func);
    }
}

void cmpCallBitCheck3(dds3ProcessID_t *PID, s32 MenuID, s32 ParentMenuID) {
    s32 enable = evtBitCheck(MenuID + 0x1EDD) != 0;

    itfMenuSetMenu(PID, MenuID, ParentMenuID, cmpData50, cmpCallGetProcessData27);
    itfMenuSetDrawFunc(PID, MenuID, cmpCallGetProcessData32);
    itfMenuSetFlagMenu(PID, MenuID, ITFMENU_CB_GF_ENABLE_DECIDE, enable);
    SetFlagMenu(PID, MenuID, ITFMENU_CB_GF_DRAW_TITLE_DECIDE, 0);
}

s32 IsBuyItem(void) {
    s32 i;

    for (i = 0x1F01; i < 0x1F64; i++) {
        if (evtBitCheck(i)) {
            return 1;
        }
    }
    for (i = 0xB01; i < 0xB64; i++) {
        if (evtBitCheck(i)) {
            return 1;
        }
    }
    return 0;
}

s32 cmpCallBitCheck4(s32 a_no) {
    s32 i;

    for (i = 0; i < 10; i++) {
        if (evtBitCheck(a_no * 10 + i + 0x1F01) || evtBitCheck(a_no * 10 + i + 0xB01)) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallMenuGetMenuNum);

void CallProcEnd(jmsInputPanelWork_t *wk) {
    itfMenuProcessEnd(wk->menuPID);
}

u8 CheckProcEnd(jmsInputPanelWork_t *wk) {
    u8 ret = 0;

    if (!itfMenuProcessCheck(wk->menuPID)) {
        wk->menuPID = NULL;
    } else {
        ret = 1;
    }
    return ret;
}

INCLUDE_ASM(const s32, "code_003BF2E0", Process_jmsMenu);

void Release_jmsMenu(jmsInputPanelWork_t *pWork) {
    if (pWork->flags & 2) {
        if (pWork->hSpr != NULL) {
            effDestroyResourceSlotSet(pWork->hSpr);
            pWork->hSpr = NULL;
        }
        pWork->flags &= ~2;
    }
}

void jmsMenuShutdown(dds3ProcessID_t *PID) {
    jmsInputPanelWork_t *work = dds3GetProcessData(PID);

    datDebugPrintf(cmpStrJmsMenuShutdownStart);
    Release_jmsMenu(work);
    if (dds3ExistProcess(work->drawPID)) {
        dds3KillProcess(work->drawPID, 0);
    }
    work->drawPID = NULL;
    sdfMemFreeDirect(work->hMem);
    inputpanel_id = NULL;
    datDebugPrintf(cmpStrJmsMenuShutdownEnd);
}

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData33);

s32 cmpGetMarkID(void)
{
    struct tagCMP_WORK *wk = dds3GetProcessData(inputpanel_id);

    return wk->markID;
}

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData34);

void jmsStart(void) {
    jmsInputPanelWork_t *work = dds3GetProcessData(inputpanel_id);

    itfMenuStart(work->menuPID);
    work->unk1C = -1;
    if (!(work->flags & 0x20)) {
        work->flags |= 8;
    }
}

void jmsSetFocus(s8 a_bFocus) {
    jmsInputPanelWork_t *work = dds3GetProcessData(inputpanel_id);

    itfMenuSetFlag(work->menuPID, 8, a_bFocus);
}

INCLUDE_ASM(const s32, "code_003BF2E0", ProcessStart_jmsMenu);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallGetProcessData8);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallGetProcessData9);

INCLUDE_ASM(const s32, "code_003BF2E0", jmsMenuMain);

/* NON_MATCHING: 10 of 26 words match.  */
#ifdef NON_MATCHING
void jmsMenuProcessEnd(void) {
    jmsInputPanelWork_t *work;

    if (inputpanel_id != NULL && dds3ExistProcess(inputpanel_id)) {
        work = dds3GetProcessData(inputpanel_id);
        if (!(work->flags & 1)) {
            work->flags |= 1;
            work->step = 1;
            work->count = 0;
            datDebugPrintf(jmsStrJmsMenuProcessEndAccept);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", jmsMenuProcessEnd);
#endif

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

void jmsCallGetProcessData11(s8 a_bSet) {
    jmsInputPanelWork_t *work;

    if (inputpanel_id != NULL) {
        work = dds3GetProcessData(inputpanel_id);
        SetFlagMenu(work->menuPID, 1, ITFMENU_CB_GF_DRAW_TITLE_DECIDE, a_bSet);
    }
}

INCLUDE_ASM(const s32, "code_003BF2E0", jmsCallBitCheck2);

s32 jmsCheckMovePrev(void) {
    jmsInputPanelWork_t *work;

    if (inputpanel_id == NULL) {
        return 0;
    }
    work = dds3GetProcessData(inputpanel_id);
    if (itfMenuGetCtrlType(work->menuPID) == ITFMENU_CTRL_TYPE_PREV) {
        return 1;
    }
    return itfMenuGetCtrlType(work->menuPID) == ITFMENU_CTRL_TYPE_CANCEL;
}

INCLUDE_ASM(const s32, "code_003BF2E0", jmsGetCursorPosID);

jmsWork_t *jmsLeaf9(void) {
    return jmsLeaf11Data;
}

INCLUDE_ASM(const s32, "code_003BF2E0", jmsLeaf10);

s32 jmsFirstInitSub(void) {
    jmsWork_t *wk;

    wk = sdfCallMemAllocDirect(sizeof(jmsWork_t));
    jmsRefDDS3GBWK(wk);
    jmsLeaf11Data = wk;
    return 1;
}

INCLUDE_ASM(const s32, "code_003BF2E0", jmsLeaf11);

/* NON_MATCHING: 12 of 28 words match.  */
#ifdef NON_MATCHING
s32 jmsJimusyoJmsQuest(void) {
    jmsWork_t *wk = jmsLeaf9();
    jmsVtblEntry_t *entry = &wk->vtbl[2];

    entry->func((u8 *)wk + entry->delta, jmsStrJimusyoJmsQuestTbl);
    wk = jmsLeaf9();
    entry = &wk->vtbl[4];
    return entry->func((u8 *)wk + entry->delta, jmsStrJimusyoJmsQuestStrBmd) != 0;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", jmsJimusyoJmsQuest);
#endif

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

/* NON_MATCHING: 1 of 4 words match.  */
#ifdef NON_MATCHING
void jmsLeaf12(jmsWork_t *wk) {
    wk->vtbl = jmsData13;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", jmsLeaf12);
#endif

INCLUDE_ASM(const s32, "code_003BF2E0", jmsLeaf13);

/* NON_MATCHING: 29 of 31 words match.  */
#ifdef NON_MATCHING
s32 jmsRefDDS3GBWK(jmsWork_t *wk) {
    jmsLeaf12(wk);
    wk->vtbl = jmsData12;
    wk->hFile = NULL;
    wk->fileData = 0;
    wk->hFile2 = NULL;
    wk->fileData2 = 0;
    wk->gbwk = &DDS3_GBWK->unkD230;
    wk->unk18 = 0;
    jmsCallMemset(wk->gbwk);
    wk->unkAC = 79;
    wk->unkB0 = 89;
    wk->unkB4 = 98;
    wk->unkB8 = 99;
    return 99;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", jmsRefDDS3GBWK);
#endif

INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc37);

s32 jmsCallDevLoadFileEx2(jmsWork_t *wk, const char *name) {
    wk->hFile = sdfDevLoadFileEx(name, &wk->fileData, NULL);
    cmpCallBitCheck10(wk);
    return 1;
}

s32 jmsCallMemFreeDirect(jmsWork_t *wk) {
    s32 ret = 0;

    if (wk->hFile != NULL) {
        sdfMemFreeDirect(wk->hFile);
        ret = 1;
        wk->hFile = NULL;
    }
    wk->fileData = 0;
    return ret;
}

s32 sysCallDevLoadFileEx(jmsWork_t *wk, const char *name) {
    wk->hFile2 = sdfDevLoadFileEx(name, &wk->fileData2, NULL);
    return 1;
}

s32 sysCallMemFreeDirect(jmsWork_t *wk) {
    s32 ret = 0;

    if (wk->hFile2 != NULL) {
        sdfMemFreeDirect(wk->hFile2);
        ret = 1;
        wk->hFile2 = NULL;
    }
    wk->fileData2 = 0;
    return ret;
}

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

s32 rstMapReturnChkXFade(rstInitWork_t *pWork) {
    if (pWork->flags & 0x20000) {
        return 0;
    }
    return rstData5;
}

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

/* NON_MATCHING: 17 of 27 words match.  */
#ifdef NON_MATCHING
dds3ProcessFunc_t rstResultStartWaitProcess(dds3ProcessID_t *PID) {
    rstInitWork_t *work = dds3GetProcessData(PID);
    dds3ProcessFunc_t next = NULL;

    if (work->flags & 0x10000) {
        work->flags |= 0x20;
        if (work->flags & 0x40) {
            return (dds3ProcessFunc_t)-1;
        }
        work->flags |= 0x60;
    } else if (work->flags & 2) {
        next = (dds3ProcessFunc_t)rstResultLoadWaitProcess;
    }
    return next;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstResultStartWaitProcess);
#endif

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultLoadWaitProcess);

INCLUDE_ASM(const s32, "code_003BF2E0", rstSetReward);

void rstSetEncountDlcID(s32 EncountNo) {
    rstCallBitCheckWork = EncountNo;
}

s32 rstGetGlobalEncountNo(void) {
    return rstCallBitCheckWork;
}

INCLUDE_ASM(const s32, "code_003BF2E0", rstSetFlag);

s32 rstCheckFlag(s32 Flag) {
    rstInitWork_t *wk;

    if (ProcessStart_rstInitData == NULL) {
        if (rstData4 & Flag) {
            return 1;
        }
    } else {
        wk = dds3GetProcessData(ProcessStart_rstInitData);
        if (wk->flags & Flag) {
            return 1;
        }
    }
    return 0;
}

void rstResultStart(void) {
    rstInitWork_t *wk;

    if (ProcessStart_rstInitData == NULL) {
        rstData4 |= 2;
    } else {
        wk = dds3GetProcessData(ProcessStart_rstInitData);
        wk->flags |= 2;
    }
}

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultFadeCheck);

SBTL_PLAYER_DATA *rstGetPlayerUnit(void) {
    rstInitWork_t *wk;

    if (ProcessStart_rstInitData == NULL) {
        return NULL;
    }
    wk = dds3GetProcessData(ProcessStart_rstInitData);
    return &wk->player;
}

s32 rstGetGiftItem(s32 StockIndex) {
    rstInitWork_t *wk;

    if (ProcessStart_rstInitData == NULL) {
        return 0;
    }
    wk = dds3GetProcessData(ProcessStart_rstInitData);
    return wk->gift[StockIndex].item;
}

s32 rstGetGiftItemNums(s32 StockIndex) {
    rstInitWork_t *wk;

    if (ProcessStart_rstInitData == NULL) {
        return 0;
    }
    wk = dds3GetProcessData(ProcessStart_rstInitData);
    return wk->gift[StockIndex].nums;
}

s32 rstCheckRankUp(void) {
    rstInitWork_t *wk;

    if (ProcessStart_rstInitData == NULL) {
        return -1;
    }
    wk = dds3GetProcessData(ProcessStart_rstInitData);
    return wk->rankUp;
}

/* NON_MATCHING: 23 of 53 words match.  */
#ifdef NON_MATCHING
s32 rstCalcProcess(rstInitWork_t *pWork) {
    sdatUnitWork_t *old;
    s32 i;

    if (DDS3_GBWK->playerData.unit.level < pWork->player2.unit.level) {
        return 1;
    }
    if (rstCheckRankUp() != -1) {
        return 1;
    }
    old = pWork->player2.stock;
    for (i = 0; i < datGetStockDevilNums(); i++) {
        if (DDS3_GBWK->playerData.stock[i].level < old->level) {
            return 1;
        }
        if (DDS3_GBWK->playerData.stock[i].chuusei < old->chuusei && old->chuusei == 100) {
            return 1;
        }
        old++;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstCalcProcess);
#endif

/* NON_MATCHING: 31 of 38 words match.  */
#ifdef NON_MATCHING
s32 rstCallGetGlobalEncountNo(rstInitWork_t *work) {
    s16 type;

    if (!(work->flags & 0x20000) && (work->flags & 1)) {
        type = sbtlData100[rstGetGlobalEncountNo()].unk10;
        if (type == 0 || type == 2) {
            return rstLoadMapResource(rstCalcProcess(work));
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstCallGetGlobalEncountNo);
#endif

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

dds3ProcessFunc_t rstEndCalcProcess(dds3ProcessID_t *PID) {
    rstInitWork_t *work = dds3GetProcessData(PID);
    dds3ProcessFunc_t next = menuRunPanel(&work->panel, &work->seqUnit, 0, PID);

    if (next != NULL) {
        return next;
    }
    if (rstCallGetGlobalEncountNo(work)
        && (dds3PadButtonState[0x23] < 0 || dds3PadButtonState[0x21] < 0)) {
        jmsSeqJumpAndFree(&work->seqUnit, &rstEndCalcProcessData);
    }
    return NULL;
}

dds3ProcessFunc_t rstEndDrawProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    rstCallGetStockDevilNums(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t rstEndUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

s32 rstBtlEndWaitInit(dds3ProcessID_t *PID) {
    rstInitWork_t *work = dds3GetProcessData(PID);

    if (rstMapReturnChkXFade(work)) {
        mapBtlEndWaitInitSub();
        if (sbtlReLoadFldScene()) {
            sbtlRefMapRndEncLoadMode();
        }
    } else {
        dds3FadeOut(0, 0, 0, 20);
        if (rstCalcProcess(work)) {
            sndBgmLoadId_XF(30);
        }
    }
    return 1;
}

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

u16 rstResultCyuuseiGaugeSub(s32 a_table, s32 a_step) {
    return rstData3[a_table].steps[a_step].value;
}

/* NON_MATCHING: 20 of 26 words match.  */
#ifdef NON_MATCHING
void rstResultNakamaLevelSub(s32 a_table, s32 a_step, s32 *a_x, s32 *a_y) {
    rstGaugeStep_t *steps = rstData3[a_table].steps;
    rstGaugeStep_t *step = &steps[a_step];
    s32 x;
    s32 y;

    if (step->parent < 0) {
        y = step->y;
        x = step->x;
    } else {
        y = step->y + steps[step->parent].y;
        x = step->x + steps[step->parent].x;
    }
    *a_x = x * 16;
    *a_y = y * 8;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstResultNakamaLevelSub);
#endif

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

dds3ProcessID_t *rstCmpItemGrowthStart(void) {
    sdfMemHandle_t *hMem;
    tagRST_GROWTH_WORK *wk;

    if (!cmpLoadLocalizeText()) {
        g_mainPID3 = NULL;
        return NULL;
    }
    hMem = sdfMemAllocDirect(sizeof(tagRST_GROWTH_WORK));
    wk = sdfMemGetPtr(hMem);
    memset(wk, 0, sizeof(tagRST_GROWTH_WORK));
    wk->hMem = hMem;
    g_mainPID3 = dds3StartProcess(rstStrResultGrowth, 0x403, 0, 0, MenuCursorCtrl, rstCallGetProcessData42, wk);
    rstInit(wk);
    return g_mainPID3;
}

void rstProcessEnd(void) {
    rstMainWork_t *work;

    if (g_mainPID3 != NULL && dds3ExistProcess(g_mainPID3)) {
        work = dds3GetProcessData(g_mainPID3);
        if (!(work->flags & 1)) {
            work->flags |= 1;
            work->step = 3;
            work->count = 0;
        }
    }
}

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

