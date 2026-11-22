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
#include "jmsInit.h"
#include "code_r1_00128378.h"
#include "dds3KernelDraw.h"
#include "dds3AdminiProcess.h"
#include "mapData.h"
#include "code_r1_002CA3F0.h"
#include "fclDrawBack.h"
#include "dds3Randomize.h"
#include "frFont.h"
#include "code_r1_002CD5A8.h"
#include "sBtlSM.h"
#include "sdfMath.h"
#include "code_003648C0.h"
#include "sbtlDMG.h"
#include "datCalc.h"

INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData27);

void cmpCallGetProcessData28(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_fontColor, u8 a_alphaRate) {
    jmsInputPanelWork_t *work = dds3GetProcessData(inputpanel_id);
    struct sprHandle_t *hSpr = jmsGetSpriteHandle();

    itfDrawMiscSpriteList(a_x, a_y, a_z, a_alphaRate, hSpr, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL,
                          &SetCommand_jmsMenuData5);
    if (a_id == itfMenuGetSelectID(work->menuPID)) {
        itfDrawMiscSpriteList(a_x, a_y, a_z, a_alphaRate, hSpr, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL,
                              &SetCommand_jmsMenuData6);
        if (a_id == 4 && itfMenuGetCtrlType(work->menuPID) == 2) {
            if (work->unk2C == 0) {
                work->unk2C = 1;
            } else if (work->unk2C == 6) {
                work->unk2C = 7;
            }
        }
    }
}

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

/* NON_MATCHING: 222 of 229 words match.  */
#ifdef NON_MATCHING
void cmpCallMenuGetMenuNum(struct jmsInputPanelWork_t *work) {
    dds3ProcessID_t *pid;
    s32 i;
    s32 j;

    pid = work->menuPID;
    if (itfMenuGetMenuNum(pid) > 0) {
        itfMenuClearMenuAll(pid);
    }
    itfMenuSetMenuTitle(pid, cmpData50);
    itfMenuSetDrawParam(pid, 10, 0);
    SetRootMenu(pid, 1, cmpCallGetProcessData27);
    SetRootMenu(pid, 2, cmpCallGetProcessData27);
    SetRootMenu(pid, 3, cmpCallGetProcessData27);
    SetRootMenu(pid, 4, cmpCallGetProcessData27);
    SetRootMenu(pid, 6, cmpCallGetProcessData27);
    if (!jmsAnimFunc()) {
        SetRootMenu(pid, 7, cmpCallGetProcessData27);
    }
    SetRootMenu(pid, 8, cmpCallGetProcessData27);
    SetFlagMenu(pid, 3, ITFMENU_CB_GF_DRAW_TITLE_DECIDE, 0);
    SetFlagMenu(pid, 6, ITFMENU_CB_GF_DRAW_TITLE_DECIDE, 0);
    SetFlagMenu(pid, 7, ITFMENU_CB_GF_DRAW_TITLE_DECIDE, 0);
    SetFlagMenu(pid, 8, ITFMENU_CB_GF_DRAW_TITLE_DECIDE, 0);
    if (!IsBuyItem()) {
        jmsDisableSelect(4);
    }
    if (!cmpMakeCateChk()) {
        jmsDisableSelect(10);
    }
    SetRootMenuData[14] = cmpCallMenuGetMenuNumData[0];
    SetRootMenuData[15] = cmpCallMenuGetMenuNumData[1];
    SetRootMenuData[16] = cmpCallMenuGetMenuNumData[2];
    SetRootMenuData[17] = cmpCallMenuGetMenuNumData[3];
    SetRootMenuData[18] = cmpCallMenuGetMenuNumData2[0];
    SetRootMenuData[19] = cmpCallMenuGetMenuNumData2[1];
    SetRootMenuData[20] = cmpCallMenuGetMenuNumData2[2];
    SetRootMenuData[21] = cmpCallMenuGetMenuNumData2[3];
    SetRootMenuData[22] = cmpCallMenuGetMenuNumData3[0];
    SetRootMenuData[23] = cmpCallMenuGetMenuNumData3[1];
    SetRootMenuData[24] = cmpCallMenuGetMenuNumData3[2];
    SetRootMenuData[25] = cmpCallMenuGetMenuNumData3[3];
    SetFlagMenu(pid, 8, ITFMENU_CB_GF_DECIDE_PREV, 1);
    itfMenuSetDrawFunc(pid, 1, cmpCallGetProcessData29);
    for (i = 2; i < 26; i++) {
        itfMenuSetDrawFunc(pid, i, cmpCallGetProcessData28);
    }
    itfMenuSetSpriteData(pid, ITFMENU_IMG_DAT_TITLE, 93, 27, 0, 0, -75, 16, NULL, cmpCallMenuGetMenuNumData5);
    itfMenuSetSpriteData(pid, ITFMENU_IMG_DAT_PANEL, 45, 88, 228, 26, 10, 4, NULL, cmpCallMenuGetMenuNumData4);
    itfMenuSetSpriteData(pid, ITFMENU_IMG_DAT_SELECT, 45, 88, 228, 26, 10, 4, NULL, cmpCallMenuGetMenuNumData4);
    itfGeneralMenuCreatePIDSub(pid, 1);
    itfGeneralMenuCreatePIDSub2(pid, 1);
    for (i = 0; i < 10; i++) {
        if (cmpCallBitCheck4(i)) {
            SetMenu(pid, i + 26, 4, cmpCallGetProcessData27);
            itfMenuSetDrawFunc(pid, i + 26, cmpCallGetProcessData31);
            for (j = 0; j < 10; j++) {
                cmpCallBitCheck3(pid, i * 10 + 36 + j, i + 26);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallMenuGetMenuNum);
#endif

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

s32 Process_jmsMenu(dds3ProcessID_t *PID) {
    jmsInputPanelWork_t *work = dds3GetProcessData(PID);
    jmsData_t *data;

    work->unk28 += 0.1f;
    switch (work->unk2C) {
    case 0:
        break;
    case 1:
        sndBgmLoadId_XF(93);
        data = jmsProcessJmsMenuSub();
        data->flag &= ~0x100;
        data->flag &= ~0x200;
        work->unk2C = 2;
        break;
    case 2:
        if (sndBgmStatus_XF() == 2) {
            sndBgmPlay();
            work->unk2C = 4;
        }
        break;
    case 3:
        if (sndBgmStatus_XF() == 2) {
            work->unk2C = 5;
        }
        break;
    case 5:
        jmsBgmLoad(jmsProcessJmsMenuSub());
        work->unk2C = 6;
        break;
    case 6:
        if (jmsBgmPlay(jmsProcessJmsMenuSub())) {
            work->unk2C = 0;
        }
        break;
    case 7:
        if (sndBgmStatus_XF() == 2) {
            work->unk2C = 1;
        }
        break;
    }
    switch (work->step) {
    case 0:
        work->unk1C = -1;
        if (!itfMenuProcessCheck(work->menuPID)) {
            work->step = 1;
            work->count = 0;
            break;
        }
        if (itfMenuGetCtrlType(work->menuPID) == ITFMENU_CTRL_TYPE_PREV) {
            work->unk1C = -1;
        }
        if (itfMenuGetCtrlType(work->menuPID) == ITFMENU_CTRL_TYPE_CANCEL) {
            work->unk1C = -1;
            work->flags |= 8;
        }
        break;
    case 1:
        if (work->count == 0) {
            CallProcEnd(work);
            work->flags &= ~4;
        }
        if (!CheckProcEnd(work)) {
            work->count = 0;
            work->step = 2;
        } else {
            work->count++;
        }
        break;
    case 2:
        return -1;
    }
    return 0;
}

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

INCLUDE_RODATA(const s32, "code_003BF2E0", cmpStrJmsMenuShutdownStart);

INCLUDE_RODATA(const s32, "code_003BF2E0", cmpStrJmsMenuShutdownEnd);

/* NON_MATCHING: 111 of 120 words match.  */
#ifdef NON_MATCHING
s32 cmpCallGetProcessData33(dds3ProcessID_t *PID)
{
    jmsInputPanelWork_t *work = dds3GetProcessData(PID);
    u8 backAlpha;
    u8 backAlpha2;
    u8 alpha;
    s32 help;
    s32 id;

    if (!(work->flags & 4)) {
        return 0;
    }
    backAlpha = GetSelectRootID(work->menuPID, 0);
    backAlpha2 = work->unk38;
    if (backAlpha < 0xFF && backAlpha2 == 0xFF) {
        backAlpha = 0xFF;
    }
    fclBackSprite(0, backAlpha, backAlpha2, work->hSpr);
    if (!dds3ExistProcess(work->menuPID)) {
        return 0;
    }
    if (!itfMenuGetFlag(work->menuPID, 8)) {
        return 0;
    }
    if (itfMenuGetWaitType(work->menuPID)) {
        return 0;
    }
    alpha = GetSelectRootID(work->menuPID, 1);
    if (!alpha) {
        return 0;
    }
    id = itfMenuGetSelectID(work->menuPID);
    switch (id) {
    case 1:
        help = 25;
        break;
    case 2:
        help = 26;
        break;
    case 3:
        help = 27;
        break;
    case 5:
        help = 28;
        break;
    case 4:
        help = 29;
        break;
    case 6:
        help = 30;
        break;
    case 7:
        help = 33;
        break;
    case 8:
        help = 31;
        break;
    default:
        help = -1;
        if (id >= 136) {
            help = 32;
        }
        break;
    }
    if (help >= 0) {
        itfDrawMiscSpriteAlpha(0, 379, 0, alpha, work->hSpr, 99, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(-10, 381, 0, alpha, work->hSpr, 101, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(332, 381, 0, alpha, work->hSpr, 102, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(16, 385, 0, alpha, work->hSpr, 100, DDS3_OT_2D_HIGH | DDS3_OT_BACKEX_NORMAL);
        fclCommandHelp(help, alpha);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallGetProcessData33);
#endif

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

void jmsMenuMain(void) {
    sdfMemHandle_t *hMem;
    jmsInputPanelWork_t *work;

    if (ProcessCheck_jmsMenu()) {
        return;
    }
    hMem = sdfMemAllocDirect(sizeof(jmsInputPanelWork_t));
    work = sdfMemGetPtr(hMem);
    memset(work, 0, sizeof(jmsInputPanelWork_t));
    work->hMem = hMem;
    inputpanel_id = dds3StartProcess(jmsStrJmsMenuMain, 0x3FE, 0, 0, Process_jmsMenu,
                                     (dds3ProcessFunc_t)jmsMenuShutdown, work);
    work->drawPID = dds3StartProcess(jmsStrJmsMenuDraw, 0x2B15, 0, 0, cmpCallGetProcessData33, NULL, work);
    work->menuPID = itfMenuProcessStart();
    work->hSpr = jmsGetSpriteHandle();
    itfMenuLinkSprite(work->menuPID, work->hSpr);
    cmpCallMenuGetMenuNum(work);
    itfMenuSetFlag(work->menuPID, 0x10, 0);
    work->flags |= 4;
    work->unk1C = 0;
    work->unk2C = 0;
}

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrJmsMenuMain);

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrJmsMenuDraw);

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
            datDebugPrintf(" jmsMenuProcessEnd() accept\n");
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

    entry->func((u8 *)wk + entry->delta, "/jimusyo/jmsQuest.tbl");
    wk = jmsLeaf9();
    entry = &wk->vtbl[4];
    return entry->func((u8 *)wk + entry->delta, "/jimusyo/jmsQuestStr.bmd") != 0;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", jmsJimusyoJmsQuest);
#endif

/* NON_MATCHING: 11 of 23 words match.  */
#ifdef NON_MATCHING
s32 jmsFunc17(void) {
    jmsWork_t *wk = jmsLeaf9();
    jmsVtblEntry_t *entry = &wk->vtbl[5];

    entry->func((u8 *)wk + entry->delta);
    wk = jmsLeaf9();
    entry = &wk->vtbl[3];
    return entry->func((u8 *)wk + entry->delta);
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", jmsFunc17);
#endif

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

/* NON_MATCHING: 59 of 82 words match.  */
#ifdef NON_MATCHING
void cmpCallBitCheck10(jmsWork_t *wk) {
    s32 table[6] = { 49, 58, 68, 78, 80, -1 };
    s32 *p;
    s32 count;
    s32 i;

    count = 0;
    for (i = 0x1F81; i < 0x1FE5; i++) {
        if (evtBitCheck(i)) {
            count++;
        }
    }
    wk->gbwk->unk0C = count;
    count = 0;
    for (i = 0; i < 100; i++) {
        if (evtBitCheck(i + 0x1F81) || evtBitCheck(i + 0xB01)) {
            count++;
        }
    }
    if (count >= 99) {
        evtBitOn(0x1DFF);
        if (wk->gbwk->unk0C > 0 && !evtBitCheck(0x1FE4)) {
            evtBitOn(0x1D71);
        }
    }
    for (p = table; *p != -1; p++) {
        if (!evtBitCheck(*p + 0xB00)) {
            return;
        }
    }
    evtBitOn(0x1DFE);
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", cmpCallBitCheck10);
#endif

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

s32 rstLoadMapResource(s32 RetType) {
    s32 result;

    rstLoadMapResourceData2++;
    if (rstLoadMapResourceData == 0) {
        fldPlayerALoadStart();
        rstLoadMapResourceData = 1;
        return 0;
    }
    if (rstLoadMapResourceData == 1) {
        result = fldPlayerALoadEndCheck();
        if (!result) {
            return result;
        }
        rstLoadMapResourceData = 2;
    }
    if (rstLoadMapResourceData == 2) {
        fldNakamaCashLoad_RndEncRetStart();
        rstLoadMapResourceData = 3;
    }
    if (rstLoadMapResourceData == 3) {
        result = fldNakamaCashLoad_EndCheck();
        if (!result) {
            return result;
        }
        rstLoadMapResourceData = 4;
    }
    if (rstLoadMapResourceData == 4) {
        fldRefBasic();
        rstLoadMapResourceData = 5;
    }
    if (rstLoadMapResourceData == 5) {
        result = fldAiboCashLoad_EndCheck();
        if (!result) {
            return result;
        }
        rstLoadMapResourceData = 6;
    }
    if (rstLoadMapResourceData == 6) {
        fldLoadMapResourceSub();
        rstLoadMapResourceData = 7;
    }
    if (rstLoadMapResourceData == 7) {
        result = fldLoadMapResourceSub2();
        if (!result) {
            return result;
        }
        rstLoadMapResourceData = 8;
    }
    switch (RetType) {
    case 0:
        rstData5 = 1;
        break;
    case 1:
        rstData6 = 1;
        break;
    }
    return 1;
}

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

INCLUDE_RODATA(const s32, "code_003BF2E0", dpdivStr10CmQuestRes);

INCLUDE_RODATA(const s32, "code_003BF2E0", dpdivStr14CmQuestResImpl);

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrMap01Ep10Scene01);

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrMap01Ep09Scene0101);

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrChapter7);

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrChapter6);

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrChapter5);

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrChapter4);

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrChapter3);

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrChapter2);

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrChapter1);

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrPrologue);

INCLUDE_RODATA(const s32, "code_003BF2E0", cmpStrStorySelectDebugProc);

INCLUDE_RODATA(const s32, "code_003BF2E0", rstStrResult);

INCLUDE_RODATA(const s32, "code_003BF2E0", rstStrRsHaikeiSpr);

void rstEndNext(rstInitWork_t *pData)
{
    s32 seq;

    if (!(pData->flags & 1)) {
        return;
    }
    if (evtBitCheck(0x360) && sbtlData100[rstCallBitCheckWork].endtype < 3 && sbtlData100[rstCallBitCheckWork].endtype > 0) {
        sndBgmFadeOut();
        seq = 501;
        dds3AdminiCallSequence(9, &seq, 4, 0);
        scrAllReleaseScript();
        return;
    }
    if (pData->flags & 0x100000) {
        sbtlCheckEndEscape();
        mapSetSceneParam_BattleReturn(13, 0, 10, "akarana_exreturn");
        return;
    }
    switch (sbtlData100[rstCallBitCheckWork].endtype) {
    case 0:
        mapReturnFromRndEncBattle(sbtlData100[rstCallBitCheckWork].end_scrip);
        break;
    case 1:
        sbtlCheckEndEscape();
        seq = sbtlData100[rstCallBitCheckWork].end_eveno;
        dds3AdminiCallSequence(9, &seq, 4, 0);
        scrAllReleaseScript();
        break;
    case 2:
        sbtlCheckEndEscape();
        mapSetSceneParam_BattleReturn(sbtlData100[rstCallBitCheckWork].end_map, sbtlData100[rstCallBitCheckWork].end_floor,
                                      sbtlData100[rstCallBitCheckWork].end_playerpos, sbtlData100[rstCallBitCheckWork].end_scrip);
        break;
    }
}

void rstDevilGiftCalc(rstInitWork_t *pData)
{
    sdatUnitWork_t *old = pData->player2.stock;
    s32 i;
    s32 j;
    s32 end;
    s32 rate;

    for (i = 0; i < datGetStockDevilNums(); i++) {
        if (DDS3_GBWK->playerData.stock[i].devilId == 0 || DDS3_GBWK->playerData.stock[i].level >= old[i].level) {
            continue;
        }
        j = DDS3_GBWK->playerData.stock[i].devilId * 3;
        for (end = j + 3; j < end; j++) {
            if (DDS3_GBWK->playerData.stock[i].level < sdatLvUpDevilGiftTbl[j].Level) {
                continue;
            }
            rate = sdatLvUpDevilGiftTbl[j].Rate;
            if ((s32)(dds3GetRandA(NULL) % 100) >= rate) {
                continue;
            }
            if (sdatLvUpDevilGiftTbl[j].NumMax == 0) {
                continue;
            }
            pData->gift[i].item = sdatLvUpDevilGiftTbl[j].ItemID;
            pData->gift[i].nums = dds3GetRandA(NULL) % sdatLvUpDevilGiftTbl[j].NumMax + 1;
            if (sbtlChkHaveMemSkl(&DDS3_GBWK->playerData.stock[i], AUTO_GIFT2KAI)) {
                pData->gift[i].nums *= 2;
            } else if (sbtlChkHaveMemSkl(&DDS3_GBWK->playerData.stock[i], AUTO_GIFT3KAI)) {
                pData->gift[i].nums *= 3;
            }
            break;
        }
    }
}

s32 rstCalcRankUp(rstInitWork_t *pData)
{
    u32 i;
    u32 num;
    s8 devil;

    evtBitOff(0x843);
    num = sdatLvUpPlayerRankUpSize / sizeof(sdatLvUpPlayerRankUp_t);
    for (i = 0; i < num; i++) {
        if (sdatLvUpPlayerRankUp[i].ChkFlag != 0 && !evtBitCheck(sdatLvUpPlayerRankUp[i].ChkFlag)) {
            break;
        }
        if (!evtBitCheck(sdatLvUpPlayerRankUp[i].GetFlag)
            && datGetTotalCyuusei(&pData->player.unit) >= sdatLvUpPlayerRankUp[i].Cyuusei) {
            devil = rstCalcRankUpTable[i];
            if (devil != 0) {
                evtBitOn(0x843);
            }
            datAddStock(devil);
            pData->player.partyNum = DDS3_GBWK->playerData.partyNum;
            return i + 2;
        }
    }
    return -1;
}

/* NON_MATCHING: 84 of 86 words match.  */
#ifdef NON_MATCHING
void rstPreCalcUnit(rstInitWork_t *pData)
{
    s32 i;
    s32 j;
    s32 cyuusei;
    sdatUnitWork_t *unit;

    pData->player2.unit.exp += pData->reward.exp;
    datGetPlayerLevelUpUnit(&pData->player2, &pData->player2);
    unit = pData->player2.stock;
    j = 0;
    for (i = 0; i < datGetStockDevilNums(); i++) {
        unit->exp += pData->reward.stockExp[j];
        datGetDevilLevelUpUnit(unit, unit);
        unit->chuusei += pData->reward.stockCyuusei[j++];
        if (unit->chuusei > 100) {
            unit->chuusei = 100;
        }
        unit++;
    }
    cyuusei = pData->reward.exCyuusei;
    if (cyuusei != 0) {
        cyuusei -= datGetTotalCyuusei(&pData->player.unit);
    }
    for (i = 0; i < datGetStockDevilNums(); i++) {
        cyuusei += pData->player2.stock[i].chuusei - DDS3_GBWK->playerData.stock[i].chuusei;
    }
    datAddTotalCyuusei(&pData->player.unit, cyuusei);
    rstDevilGiftCalc(pData);
    pData->rankUp = rstCalcRankUp(pData);
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstPreCalcUnit);
#endif

/* NON_MATCHING: 327 of 385 words match.  */
#ifdef NON_MATCHING
rstInitWork_t *Init_rstInit(rstWork_t *src) {
    sdfMemHandle_t *hMem = sdfMemAllocDirect(sizeof(rstInitWork_t));
    rstInitWork_t *pData = sdfMemLock(hMem);
    s32 i;
    s32 rank;

    memset(pData, 0, sizeof(rstInitWork_t));
    pData->hMem = hMem;
    if (src != NULL) {
        pData->reward = *src;
        pData->reward.exCyuusei = 0;
        pData->flags |= rstData4;
    } else {
        rank = datGetSummonerRank() - 1;
        pData->reward.exCyuusei = sdatLvUpPlayerRankUp[rank].Cyuusei;
        pData->reward.money = 1000;
        pData->reward.item[0][0] = 1;
        pData->reward.item[0][1] = 1;
        pData->reward.unk64 = 0;
        pData->reward.exp = 0;
        pData->reward.unk110 = 10;
        pData->unk2BC = 0;
        for (i = 0; i < datGetStockDevilNums(); i++) {
            pData->reward.stockExp[i] = 200;
            pData->reward.stockCyuusei[i] = 50;
        }
        pData->flags |= 2;
    }
    pData->unk2BC = 0;
    for (i = 3; i >= 0; i--) {
        pData->playSE[i].unk00 = 0;
    }
    pData->reward.exp += pData->reward.unk64;
    for (i = 0; i < 20; i++) {
        if (pData->reward.stockExp[i] > 0) {
            pData->reward.stockExp[i] += pData->reward.unk64;
        }
    }
    pData->reward.unk64 = 0;
    pData->reward2 = pData->reward;
    pData->player = DDS3_GBWK->playerData;
    pData->player1 = DDS3_GBWK->playerData;
    pData->player1.unit.exp <<= 8;
    for (i = 0; i < datGetStockDevilNums(); i++) {
        pData->player1.stock[i].exp <<= 8;
    }
    pData->player2 = DDS3_GBWK->playerData;
    rstPreCalcUnit(pData);
    mnuClearPanelTransitionState(&pData->panel);
    rstLoadMapResourceData = 0;
    rstLoadMapResourceData2 = 0;
    rstData5 = 0;
    rstData6 = 0;
    return pData;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", Init_rstInit);
#endif

/* NON_MATCHING: 18 of 204 words match.  */
#ifdef NON_MATCHING
void Destroy_rstInit(dds3ProcessID_t *PID) {
    rstInitWork_t *pData = dds3GetProcessData(PID);
    s32 luck;
    s32 i;
    s32 nums;

    pData->player.unit.backup.str += pData->player.unit.now.str - DDS3_GBWK->playerData.unit.now.str;
    pData->player.unit.backup.tec += pData->player.unit.now.tec - DDS3_GBWK->playerData.unit.now.tec;
    pData->player.unit.backup.vit += pData->player.unit.now.vit - DDS3_GBWK->playerData.unit.now.vit;
    if (DDS3_GBWK->playerData.unit.backup.luck < pData->player.unit.backup.luck) {
        luck = pData->player.unit.backup.luck + sbtlGetCorrectionParamLuck() + pData->player.unit.pbtl.luck + pData->player.unit.pitem.luck[1]
             + pData->player.unit.pitem.luck[0] + pData->player.unit.huuma.luck;
        if (luck <= 0) {
            luck = 1;
        }
        if (pData->player.unit.now.luck >= luck) {
            pData->player.unit.backup.luck += pData->player.unit.now.luck - luck;
        }
    }
    datCalcNowParam(&pData->player.unit, pData->player.sword);
    pData->player.mag = DDS3_GBWK->playerData.mag;
    pData->player.unk294C = DDS3_GBWK->playerData.unk294C;
    pData->player.val24 = DDS3_GBWK->playerData.val24;
    DDS3_GBWK->playerData = pData->player;
    datAddMaka(pData->reward.money);
    for (i = 0; i < 8; i++) {
        nums = pData->reward.item[i][1] + pData->reward.item[i][2];
        if (pData->reward.item[i][0] != 0 && nums > 0) {
            datAddItem(pData->reward.item[i][0], nums);
        }
    }
    if (evtBitCheck(2603)) {
        evtBitOn(2335);
    }
    for (i = 0; i < 20; i++) {
        if (pData->gift[i].item != 0 && pData->gift[i].nums > 0) {
            datAddItem(pData->gift[i].item, pData->gift[i].nums);
        }
    }
    sbtlCheckTutorialEnd();
    dds3KillProcessName(campDrawProcName, 0);
    dds3KillProcessName(campUpdateProcName, 0);
    rstProcessEnd();
    if (pData->hResultSpr != NULL) {
        effDestroyResourceSlotSet(pData->hResultSpr);
    }
    rstDestroyBG(pData->bg);
    if (!(pData->flags & 0x20000)) {
        rstEndNext(pData);
    }
    sdfMemFreeDirect(pData->hMem);
    ProcessStart_rstInitData = NULL;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", Destroy_rstInit);
#endif

INCLUDE_ASM(const s32, "code_003BF2E0", rstProcessStartRstInitSub);

dds3ProcessID_t *ProcessStart_rstInit(void) {
    dds3ProcessID_t *pid = NULL;
    rstWork_t *src = NULL;
    s32 seq;

    if (ProcessStart_rstInitData != NULL) {
        return ProcessStart_rstInitData;
    }
    if (!(rstData4 & 0x40000)) {
        if (rstData4 & 1) {
            src = &rstWork;
        }
        pid = dds3StartProcess(cmpCalcProcName, 0x403, 0, 1, (dds3ProcessFunc_t)rstResultStartWaitProcess,
                               (dds3ProcessFunc_t)Destroy_rstInit, Init_rstInit(src));
    } else if (evtBitCheck(864)) {
        sndBgmFadeOut();
        seq = 501;
        dds3AdminiCallSequence(9, &seq, 4, 0);
        scrAllReleaseScript();
    } else {
        fldBgTexClear();
        if (!(rstData4 & 0x20000)) {
            sbtlCheckEndEscape();
            if (!rstProcessStartRstInitSub()) {
                if (!(rstData4 & 0x80000)) {
                    seq = 696;
                    dds3AdminiCallSequence(9, &seq, 4, 0);
                } else {
                    mapRetuenFromNakamaDead();
                }
            } else {
                seq = 696;
                dds3AdminiCallSequence(9, &seq, 4, 0);
            }
        }
    }
    ProcessStart_rstInitData = pid;
    rstData4 = 0;
    return pid;
}

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

dds3ProcessFunc_t rstResultLoadWaitProcess(dds3ProcessID_t *PID) {
    rstInitWork_t *work = dds3GetProcessData(PID);
    dds3FadeColor_t *color;

    if (!(work->flags & 4)) {
        effRequestResourceByMode(rstStrResult, rstStrResultSpr, 0, &work->hResultSpr);
        work->flags |= 4;
        return NULL;
    }
    if (work->hResultSpr == NULL) {
        return NULL;
    }
    if (!(work->flags & 8)) {
        effRequestResourceByMode(rstStrResult, rstStrRsHaikeiSpr, 0, &work->hBgSpr);
        work->flags |= 8;
        return NULL;
    }
    if (work->hBgSpr == NULL) {
        return NULL;
    }
    work->bg = InitBgCore(work->hBgSpr);
    work->flags |= 0x10;
    dds3FadeColorGet(&color);
    if (color->a == 0x80) {
        dds3FadeIn2(30);
        work->flags |= 0x100;
    }
    dds3StartProcess(campDrawProcName, 0x2B12, 0, 1, (dds3ProcessFunc_t)rstDrawProcess, NULL, work);
    dds3StartProcess(campUpdateProcName, 0x520F, 0, 1, (dds3ProcessFunc_t)rstCallGetProcessData37, NULL, work);
    return (dds3ProcessFunc_t)rstCalcProcess_rstResult;
}

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
        type = sbtlData100[rstGetGlobalEncountNo()].endtype;
        if (type == 0 || type == 2) {
            return rstLoadMapResource(rstCalcProcess(work));
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstCallGetGlobalEncountNo);
#endif

INCLUDE_RODATA(const s32, "code_003BF2E0", rstCalcRankUpTable);

INCLUDE_RODATA(const s32, "code_003BF2E0", campDrawProcName);

INCLUDE_RODATA(const s32, "code_003BF2E0", campUpdateProcName);

INCLUDE_RODATA(const s32, "code_003BF2E0", cmpCalcProcName);

INCLUDE_RODATA(const s32, "code_003BF2E0", rstStrResultSpr);

INCLUDE_RODATA(const s32, "code_003BF2E0", rstStrSeqResultLvupWait);

INCLUDE_RODATA(const s32, "code_003BF2E0", rstStrSeqResultBtlendWait);

INCLUDE_RODATA(const s32, "code_003BF2E0", rstStrSeqResultEnd);

INCLUDE_RODATA(const s32, "code_003BF2E0", jmsStrSeqResult);

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

s32 rstEndInit(void)
{
    return 1;
}

s32 rstEndDestroy(dds3ProcessID_t *PID)
{
    rstInitWork_t *work = dds3GetProcessData(PID);
    s32 i;

    for (i = 0; i < 3; i++) {
        if (work->countPID[i] != NULL) {
            itfCountProcessEnd(work->countPID[i]);
            work->countPID[i] = NULL;
        }
    }
    return 1;
}

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

s32 rstBtlEndWaitDestroy(void)
{
    return 1;
}

dds3ProcessFunc_t rstBtlEndWaitCalcProcess(dds3ProcessID_t *PID) {
    rstInitWork_t *work = dds3GetProcessData(PID);
    dds3ProcessFunc_t next = menuRunPanel(&work->panel, &work->seqUnit, 0, PID);

    if (next != NULL) {
        return next;
    }
    if (!rstMapReturnChkXFade(work)) {
        if (rstCalcProcess(work) && sndBgmStatus_XF() != 2) {
            return NULL;
        }
        if (dds3FadeCheck()) {
            return NULL;
        }
    } else if (sbtlReLoadFldScene() && !mapRndEncReturn_BgmLoadEndCheck()) {
        return NULL;
    }
    if (!Proc_sbtl()) {
        jmsSeqJumpAndFree(&work->seqUnit, &rstBtlEndWaitCalcProcessData);
    }
    return NULL;
}

dds3ProcessFunc_t rstBtlEndWaitDrawProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    rstCallGetStockDevilNums(work);
    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t rstBtlEndWaitUpdateProcess(dds3ProcessID_t *PID) {
    gohData_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 2, PID);
}

s32 rstLvUpWaitInit(void)
{
    return 1;
}

s32 rstLvUpWaitDestroy(void)
{
    return 1;
}

/* NON_MATCHING: 37 of 73 words match.  */
#ifdef NON_MATCHING
dds3ProcessFunc_t rstLvUpWaitCalcProcess(dds3ProcessID_t *PID) {
    rstInitWork_t *work = dds3GetProcessData(PID);
    dds3ProcessFunc_t next = menuRunPanel(&work->panel, &work->seqUnit, 0, PID);

    if (next != NULL) {
        return next;
    }
    if (!(work->flags & 0x80)) {
        work->flags |= 0x80;
        rstCmpItemGrowthStart();
        dds3FadeIn(0, 0, 0, 0);
    } else {
        if (rstCallGetProcessStat()) {
            return NULL;
        }
        dds3FadeIn(0, 0, 0, 0);
        if (sbtlData100[rstGetGlobalEncountNo()].endtype != 1) {
            return (dds3ProcessFunc_t)-1;
        }
        if (work->fadeCount == 0) {
            sndBgmFadeOut();
        }
        work->fadeCount++;
        if (work->fadeCount >= 60) {
            return (dds3ProcessFunc_t)-1;
        }
    }
    return NULL;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstLvUpWaitCalcProcess);
#endif

dds3ProcessFunc_t rstLvUpWaitDrawProcess(dds3ProcessID_t *PID) {
    jmsSeqWork_t *work = dds3GetProcessData(PID);

    return menuRunPanel(&work->panel, &work->seqUnit, 1, PID);
}

dds3ProcessFunc_t rstLvUpWaitUpdateProcess(dds3ProcessID_t *PID) {
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

/* NON_MATCHING: 81 of 83 words match.  */
#ifdef NON_MATCHING
void rstResultLevelUpAnimSub(s32 a_x, s32 a_y, f32 *a_scale, u32 a_blend, s32 a_table, s32 a_step, sprHandle_t *a_hSpr,
                             s32 a_layer)
{
    s32 x;
    s32 y;
    s32 sprNo = rstData3[a_table].steps[a_step].value;

    rstResultNakamaLevelSub(a_table, a_step, &x, &y);
    x += a_x;
    y += a_y;
    if (a_scale[0] != 1.0f) {
        a_hSpr->workEntries[sprNo].geometry.bounds[2] = (s32)(a_hSpr->workEntries[sprNo].sourceWidth * a_scale[0]) * 16;
    }
    if (a_scale[1] != 1.0f) {
        a_hSpr->workEntries[sprNo].geometry.bounds[3] = (s32)(a_hSpr->workEntries[sprNo].sourceHeight * a_scale[1]) * 8;
    }
    effDrawSpriteWithCornerFade(x, y, 0, a_blend, 0, a_hSpr, sprNo, a_layer);
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstResultLevelUpAnimSub);
#endif

/* NON_MATCHING: 81 of 87 words match.  */
#ifdef NON_MATCHING
void rstResultGetExpCyuuseiSub(s32 a_x, s32 a_y, f32 *a_scale, s32 a_unused, s32 a_table, s32 a_step, sprHandle_t *a_hSpr,
                               u32 a_color, s32 a_layer)
{
    u32 colors[4];
    s32 x;
    s32 y;
    s32 sprNo = rstData3[a_table].steps[a_step].value;

    rstResultNakamaLevelSub(a_table, a_step, &x, &y);
    x += a_x;
    y += a_y;
    if (a_scale[0] != 1.0f) {
        a_hSpr->workEntries[sprNo].geometry.bounds[2] = (s32)(a_hSpr->workEntries[sprNo].sourceWidth * a_scale[0]) * 16;
    }
    if (a_scale[1] != 1.0f) {
        a_hSpr->workEntries[sprNo].geometry.bounds[3] = (s32)(a_hSpr->workEntries[sprNo].sourceHeight * a_scale[1]) * 8;
    }
    colors[0] = a_color;
    colors[1] = a_color;
    colors[2] = a_color;
    colors[3] = a_color;
    effDrawSpriteWithCornerColors(x, y, 0, colors, 0, a_hSpr, sprNo, a_layer);
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstResultGetExpCyuuseiSub);
#endif

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

INCLUDE_SDATA(const s32, "code_003BF2E0", inputpanel_id);

INCLUDE_SDATA(const s32, "code_003BF2E0", cmpData34Entry);

INCLUDE_SDATA(const s32, "code_003BF2E0", SetCommand_jmsMenuData4Entry);

INCLUDE_SDATA(const s32, "code_003BF2E0", SetCommand_jmsMenuData4Entry2);

INCLUDE_SDATA(const s32, "code_003BF2E0", _gp2);

INCLUDE_SDATA(const s32, "code_003BF2E0", cmpData50);

INCLUDE_SDATA(const s32, "code_003BF2E0", cmpData34Entry2);

INCLUDE_SDATA(const s32, "code_003BF2E0", cmpData34Entry3);

INCLUDE_SDATA(const s32, "code_003BF2E0", cmpData34Entry4);

INCLUDE_SDATA(const s32, "code_003BF2E0", cmpData35Entry);

INCLUDE_SDATA(const s32, "code_003BF2E0", cmpData51);

INCLUDE_SDATA(const s32, "code_003BF2E0", jmsLeaf11Data);

INCLUDE_SDATA(const s32, "code_003BF2E0", cmpData52);

INCLUDE_SDATA(const s32, "code_003BF2E0", cmpData53);

INCLUDE_SDATA(const s32, "code_003BF2E0", ProcessStart_rstInitData);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstData4);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstLoadMapResourceData);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstLoadMapResourceData2);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstData5);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstData6);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstData14);

void rstCallCalcAlphaRate(rstBG_t *bg, s32 alpha, s32 ot) {
    f32 scale[2] = { 1.0f, 1.0f };
    titleKey_t key[3] = { { 0, 50 }, { 120, 70 }, { 240, 50 } };
    s32 rate = rstCalcAlphaRate(bg->frame, key, 3);

    if (++bg->frame > key[2].frame) {
        bg->frame = 0;
    }
    rate = rate * alpha / 256;
    rstResultLevelUpAnimSub(0, 0, scale, alpha, 0, 0, bg->pSpr, ot);
    rstResultLevelUpAnimSub(0, 0, scale, alpha, 0, 1, bg->pSpr, ot);
    rstResultLevelUpAnimSub(0, 0, scale, rate, 0, 2, bg->pSpr, ot);
    rstResultLevelUpAnimSub(0, 0, scale, rate, 0, 3, bg->pSpr, ot);
    rstResultLevelUpAnimSub(0, 0, scale, alpha, 0, 4, bg->pSpr, ot);
    rstResultLevelUpAnimSub(0, 0, scale, alpha, 0, 5, bg->pSpr, ot);
    rstResultLevelUpAnimSub(0, 0, scale, alpha, 0, 6, bg->pSpr, ot);
    rstResultLevelUpAnimSub(0, 0, scale, alpha, 0, 7, bg->pSpr, ot);
    rstResultLevelUpAnimSub(0, 0, scale, alpha, 0, 8, bg->pSpr, ot);
    rstResultLevelUpAnimSub(0, 0, scale, alpha, 0, 9, bg->pSpr, ot);
    rstResultLevelUpAnimSub(0, 0, scale, alpha, 0, 10, bg->pSpr, ot);
    rstResultLevelUpAnimSub(0, 0, scale, alpha, 0, 11, bg->pSpr, ot);
    rstResultLevelUpAnimSub(0, 0, scale, alpha, 0, 12, bg->pSpr, ot);
}

/* NON_MATCHING: 349 of 350 words match.  */
#ifdef NON_MATCHING
void rstResultGetExpCyuusei(rstInitWork_t *pData, s32 X, s32 Y, s32 AlphaRate, s32 Exp, s32 Cyuusei, sprHandle_t *pSpr)
{
    char buf[32];
    f32 scale[2] = { 1.0f, 1.0f };
    s32 x;
    s32 y;
    s32 color;
    frFontHandle_t *pFRQ;

    rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 0, pSpr, 105);
    rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 1, pSpr, 105);
    rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 2, pSpr, 105);
    color = fclBlendColor(0xA09DC380, 0, AlphaRate);
    if (pData->reward.unk120 & 1) {
        if (pData->unk8058 >= 7) {
            rstData14 += 0.1f;
            color = sdfSin(rstData14) * 30.0f + 30.0f;
            color = (color << 24) | (color << 16) | (color << 8) | color;
            color += 0xA0966480;
            rstResultGetExpCyuuseiSub(X, Y, scale, AlphaRate, 1, 3, pSpr, color, 105);
        } else {
            rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 3, pSpr, 105);
        }
    } else {
        rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 3, pSpr, 105);
    }
    rstResultNakamaLevelSub(1, 4, &x, &y);
    x += X;
    y += Y;
    sprintf(buf, "%6d", Exp);
    pFRQ = frwGetSmallNumFontFRQ(x, y, 0, color, (u8 *)buf, NULL);
    frSetFontSpace(pFRQ, 0);
    frDrawFontOt(pFRQ, 1, 105);
    frDestroyFontRequestAfter(pFRQ);
    color = fclBlendColor(0xA09DC380, 0, AlphaRate);
    if (pData->reward.unk120 & 2) {
        if (pData->unk8058 >= 5) {
            rstData14 += 0.1f;
            color = sdfSin(rstData14) * 30.0f + 30.0f;
            color = (color << 24) | (color << 16) | (color << 8) | color;
            color += 0xA0966480;
            rstResultGetExpCyuuseiSub(X, Y, scale, AlphaRate, 1, 5, pSpr, color, 105);
        } else {
            rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 5, pSpr, 105);
        }
    } else {
        rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 5, pSpr, 105);
    }
    rstResultNakamaLevelSub(1, 6, &x, &y);
    x += X;
    y += Y;
    sprintf(buf, "%6d", Cyuusei);
    pFRQ = frwGetSmallNumFontFRQ(x, y, 0, color, (u8 *)buf, NULL);
    frSetFontSpace(pFRQ, 0);
    frDrawFontOt(pFRQ, 1, 105);
    frDestroyFontRequestAfter(pFRQ);
    if ((pData->reward.unk120 & 1) && pData->unk8058 > 0) {
        rstResultNakamaLevelSub(1, 4, &x, &y);
        x += X;
        y += Y;
        rstCallPlaySE(pData, 2, x - 800.0f, y - 240.0f, pSpr);
        x += 0x460;
    }
    if ((pData->reward.unk120 & 2) && pData->unk8058 > 0) {
        rstResultNakamaLevelSub(1, 6, &x, &y);
        x += X;
        y += Y;
        rstCallPlaySE(pData, 2, x - 800.0f, y - 240.0f, pSpr);
        x += 0x460;
    }
    if (rstData14 > 10000.0f) {
        rstData14 = 0.0f;
    }
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstResultGetExpCyuusei);
#endif

/* NON_MATCHING: 253 of 256 words match.  */
#ifdef NON_MATCHING
void rstResultExpGauge(rstInitWork_t *pData, s32 X, s32 Y, s32 AlphaRate, s32 CurrentExp, s32 CurrentLevelExp,
                       s32 NextLevelExp, s32 *pPartsList, sprHandle_t *pSpr)
{
    char buf[32];
    u32 color = fclBlendColor(0xA09DC380, 0, AlphaRate);
    f32 scale[2] = { 1.0f, 1.0f };
    s32 x;
    s32 y;
    s32 gaugeColor;
    s32 width;
    s32 w;
    s32 idx;
    frFontHandle_t *pFRQ;

    rstResultNakamaLevelSub(1, pPartsList[2], &x, &y);
    x += X;
    y += Y;
    gaugeColor = fclBlendColor(0xA09DC380, 0, AlphaRate);
    if (pData->reward.unk120 & 1) {
        if (pData->unk8058 >= 7) {
            gaugeColor = sdfSin(rstData14) * 30.0f + 30.0f;
            gaugeColor = (gaugeColor << 24) | (gaugeColor << 16) | (gaugeColor << 8) | gaugeColor;
            gaugeColor += 0xA0966480;
            rstResultGetExpCyuuseiSub(X, Y, scale, AlphaRate, 1, pPartsList[5], pSpr, gaugeColor, 105);
            rstResultGetExpCyuuseiSub(X, Y, scale, AlphaRate, 1, pPartsList[0], pSpr, gaugeColor, 105);
            rstResultGetExpCyuuseiSub(X, Y, scale, AlphaRate, 1, pPartsList[1], pSpr, gaugeColor, 105);
        } else {
            rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, pPartsList[0], pSpr, 105);
            rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, pPartsList[1], pSpr, 105);
            rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, pPartsList[5], pSpr, 105);
        }
    } else {
        rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, pPartsList[0], pSpr, 105);
        rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, pPartsList[1], pSpr, 105);
        rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, pPartsList[5], pSpr, 105);
    }
    if (CurrentExp >= CurrentLevelExp) {
        idx = rstResultCyuuseiGaugeSub(1, pPartsList[3]);
        width = pSpr->workEntries[idx].sourceWidth;
        if (CurrentLevelExp < NextLevelExp) {
            w = width * (CurrentExp - CurrentLevelExp) / (NextLevelExp - CurrentLevelExp);
            pSpr->workEntries[idx].geometry.bounds[2] = w * 16;
            pSpr->workEntries[idx].parameters[2] = w - width;
        } else {
            pSpr->workEntries[idx].parameters[2] = 0;
            pSpr->workEntries[idx].geometry.bounds[2] = width * 16;
        }
        rstResultGetExpCyuuseiSub(X, Y, scale, AlphaRate, 1, pPartsList[3], pSpr, gaugeColor, 105);
    }
    if (NextLevelExp != CurrentLevelExp) {
        sprintf(buf, "%6d", (NextLevelExp - CurrentExp) / 256);
    } else {
        sprintf(buf, "%6d", 0);
    }
    pFRQ = frwGetSmallNumFontFRQ(x, y, 0, color, (u8 *)buf, NULL);
    frSetFontSpace(pFRQ, 0);
    frDrawFontOt(pFRQ, 1, 105);
    frDestroyFontRequestAfter(pFRQ);
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstResultExpGauge);
#endif

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultAddLevelUpAnimQue);

/* NON_MATCHING: 104 of 116 words match.  */
#ifdef NON_MATCHING
void rstResultLevelUpAnim(s32 X, s32 Y, s32 AlphaRate, s32 Type, s32 *pQue, sprHandle_t *pSpr)
{
    sdfIVector2_t pos;
    f32 scale[2] = { 1.0f, 1.0f };
    titleKey_t key[3] = { { 0, 128 }, { 20, 70 }, { 45, 0 } };
    rstPositionTable_t table[3] = { { 0, { 0, 0 } }, { 20, { 0, -10 } }, { 45, { 0, -20 } } };
    s32 i;
    s32 frame;
    s32 alpha;
    s32 step;

    step = Type == 0 ? 17 : 23;
    for (i = 7; i >= 0; i--, pQue++) {
        if (*pQue) {
            frame = 45 - *pQue;
            alpha = rstCalcAlphaRate(frame, key, 3) * AlphaRate / 256;
            rstCalcPosition(frame, table, 3, &pos);
            rstResultLevelUpAnimSub(X + pos.x * 16, Y + pos.y * 8, scale, alpha, 1, step, pSpr, 105);
            (*pQue)--;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstResultLevelUpAnim);
#endif

/* NON_MATCHING: 161 of 170 words match.  */
#ifdef NON_MATCHING
void rstCallBlendColor2(rstInitWork_t *pData, s32 X, s32 Y, s32 AlphaRate, s32 Rate, struct sprHandle_t *pSpr) {
    f32 scale[2] = { 1.0f, 1.0f };
    itfBdWork_t *work = pSpr->workEntries + rstResultCyuuseiGaugeSub(1, 27);
    s32 width = work->sourceWidth;
    s32 w = width * Rate / 100;
    u32 color;

    work->parameters[2] = w - width;
    work->geometry.bounds[2] = w * 16;
    fclBlendColor(0xA09DC380, 0, AlphaRate);
    if (pData->reward.unk120 & 2) {
        if (pData->unk8058 >= 7) {
            color = (s32)(sdfSin(rstData14) * 30.0f + 30.0f);
            color = (((color << 24) | color) | ((color << 16) | (color << 8))) + 0xA0966480;
            rstResultGetExpCyuuseiSub(X, Y, scale, AlphaRate, 1, 25, pSpr, color, 105);
            rstResultGetExpCyuuseiSub(X, Y, scale, AlphaRate, 1, 26, pSpr, color, 105);
            rstResultGetExpCyuuseiSub(X, Y, scale, AlphaRate, 1, 27, pSpr, color, 105);
        } else {
            rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 25, pSpr, 105);
            rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 26, pSpr, 105);
            rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 27, pSpr, 105);
        }
    } else {
        rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 25, pSpr, 105);
        rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 26, pSpr, 105);
        rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 27, pSpr, 105);
    }
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstCallBlendColor2);
#endif

INCLUDE_ASM(const s32, "code_003BF2E0", rstResultAddCyuuseiMaxAnimQue);

/* NON_MATCHING: 142 of 172 words match.  */
#ifdef NON_MATCHING
s32 rstResultCyuuseiMaxAnim(s32 X, s32 Y, s32 AlphaRate, s32 *pQue, sprHandle_t *pSpr)
{
    sdfVector2_t scale;
    titleKey_t key[4] = { { 0, 0 }, { 7, 0 }, { 10, 110 }, { 50, 110 } };
    titleKey_t key2[3] = { { 0, 0 }, { 10, 128 }, { 50, 0 } };
    rstScaleTable_t table[3] = { { 0, { 0.7f, 0.5f } }, { 10, { 1.6f, 1.3f } }, { 50, { 1.0f, 1.0f } } };
    s32 idx;
    s32 alpha;
    s32 i;
    s32 frame;
    s32 w;
    s32 h;
    s32 ret;

    ret = key[3].value * 2;
    for (i = 7; i >= 0; i--, pQue++) {
        if (*pQue) {
            frame = 50 - *pQue;
            scale.x = 1.0f;
            scale.y = 1.0f;
            alpha = rstCalcAlphaRate(frame, key, 4) * AlphaRate / 256;
            rstResultLevelUpAnimSub(X, Y, &scale.x, alpha, 1, 28, pSpr, 105);
            rstCalcScale(frame, table, 3, &scale);
            idx = rstResultCyuuseiGaugeSub(1, 29);
            w = pSpr->workEntries[idx].sourceWidth;
            h = pSpr->workEntries[idx].sourceHeight;
            w -= (s32)(w * scale.x);
            h -= (s32)(h * scale.y);
            alpha = rstCalcAlphaRate(frame, key2, 3) * AlphaRate / 256;
            h = Y + h * 4;
            w = X + w * 8;
            rstResultLevelUpAnimSub(w, h, &scale.x, alpha, 1, 29, pSpr, 105);
            ret = 0;
            (*pQue)--;
        }
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstResultCyuuseiMaxAnim);
#endif

INCLUDE_RODATA(const s32, "code_003BF2E0", rstResultLevelUpAnimTable);

INCLUDE_RODATA(const s32, "code_003BF2E0", rstResultLevelUpAnimTable2);

INCLUDE_RODATA(const s32, "code_003BF2E0", rstResultCyuuseiMaxAnimTable);

INCLUDE_RODATA(const s32, "code_003BF2E0", rstResultCyuuseiMaxAnimTable2);

INCLUDE_RODATA(const s32, "code_003BF2E0", rstResultCyuuseiMaxAnimTable3);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstResultGetExpCyuuseiData);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstData15);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstResultExpGaugeData);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstResultLevelUpAnimData);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstCallBlendColor2Data3);

void rstResultPlayerLevel(s32 X, s32 Y, s32 AlphaRate, rstInitWork_t *pData, struct sprHandle_t *pSpr) {
    char text[32];
    u32 color = fclBlendColor(0xA09DC380, 0, AlphaRate);
    s32 parts[6] = { 13, 14, 15, 16, 17, 18 };
    f32 scale[2] = { 1.0f, 1.0f };
    s32 x;
    s32 y;
    frFontHandle_t *frq;
    s32 exp;
    s32 cur;

    rstResultLevelUpAnimSub(X, Y, scale, AlphaRate, 1, 7, pSpr, 105);
    rstResultNakamaLevelSub(1, 8, &x, &y);
    x += X;
    y += Y;
    sprintf(text, rstCallBlendColor4Data, pData->player.unit.level);
    frq = frwGetSmallNumFontFRQ(x, y, 0, color, (u8 *)text, NULL);
    frSetFontSpace(frq, 0);
    frDrawFontOt(frq, 1, 105);
    frDestroyFontRequestAfter(frq);
    rstResultNakamaLevelSub(1, 9, &x, &y);
    x += X;
    y += Y;
    frq = frwGetSmallFontFRQ2(x, y, 0, color, g_itemNo, NULL);
    frSetFontSpace(frq, -1);
    frDrawFontOt(frq, 1, 105);
    frDestroyFontRequestAfter(frq);
    exp = pData->player1.unit.exp;
    cur = datGetPlayerExp(pData->player.unit.level) << 8;
    rstResultExpGauge(pData, X, Y, AlphaRate, exp, cur, datGetPlayerExp(pData->player.unit.level + 1) << 8, parts,
                      pSpr);
    rstResultLevelUpAnim(X, Y, AlphaRate, 0, pData->aLevelUpAnimQue[0], pSpr);
    if ((pData->reward.unk120 & 1) && pData->unk8058 >= 6) {
        rstCallPlaySE(pData, 3, X + 0x17C0, Y + 0x410, pData->hResultSpr);
    }
}

/* NON_MATCHING: 233 of 236 words match.  */
#ifdef NON_MATCHING
void rstCallBlendColor3(s32 X, s32 Y, s32 AlphaRate, rstInitWork_t *pData, struct sprHandle_t *pSpr, s32 StockIndex,
                        s32 Type) {
    char str[32];
    rstExpGaugeParts_t parts;
    rstAnimScale_t scale;
    s32 x;
    s32 y;
    u32 color;
    sdatUnitWork_t *unit;
    frFontHandle_t *pFRQ;
    s32 exp;
    s32 levelExp;
    s32 rate;

    color = fclBlendColor(0xA09DC380, 0, AlphaRate);
    parts = rstResultNakamaLevelTable;
    scale = rstCallBlendColor3Data4[0];
    if (StockIndex >= 0 && datRefDDS3GBWK11(StockIndex)) {
        unit = &pData->player.stock[StockIndex];
        rstResultLevelUpAnimSub(X, Y, scale.scale, AlphaRate, 1, 10, pSpr, 0x69);
        rstResultNakamaLevelSub(1, 11, &x, &y);
        x += X;
        y += Y;
        sprintf(str, rstCallBlendColor4Data, unit->level);
        pFRQ = frwGetSmallNumFontFRQ(x, y, 0, color, (u8 *)str, NULL);
        frSetFontSpace(pFRQ, 0);
        frDrawFontOt(pFRQ, 1, 0x69);
        frDestroyFontRequestAfter(pFRQ);
        rstResultNakamaLevelSub(1, 12, &x, &y);
        x += X;
        y += Y;
        pFRQ = frwGetSmallFontFRQ2(x, y, 0, color, g_itemNo[unit->devilId], NULL);
        frSetFontSpace(pFRQ, -1);
        frDrawFontOt(pFRQ, 1, 0x69);
        frDestroyFontRequestAfter(pFRQ);
        exp = pData->player1.stock[StockIndex].exp;
        levelExp = datGetDevilExp(unit->devilId, pData->player.stock[StockIndex].level) << 8;
        rstResultExpGauge(pData, X, Y, AlphaRate, exp, levelExp,
                          datGetDevilExp(unit->devilId, pData->player.stock[StockIndex].level + 1) << 8, parts.parts,
                          pSpr);
        rstCallBlendColor2(pData, X, Y, AlphaRate, unit->chuusei, pSpr);
        if (pData->unk8058 >= 6) {
            if (pData->reward.unk120 & 1) {
                rstCallPlaySE(pData, Type, X + 0x17C0, Y + 0x5A0, pData->hResultSpr);
            } else if (pData->reward.unk120 & 2) {
                rstCallPlaySE(pData, Type, X + 0x17C0, Y + 0x640, pData->hResultSpr);
            }
        }
        rstResultLevelUpAnim(X, Y, AlphaRate, 1, pData->aLevelUpAnimQue[Type + 1], pSpr);
        rate = rstResultCyuuseiMaxAnim(X, Y, AlphaRate, pData->aLevelUpAnimQue[Type + 3], pSpr);
        if (rate > 0 && unit->chuusei == 100) {
            rstResultLevelUpAnimSub(X, Y, scale.scale, rate * AlphaRate / 256, 1, 28, pSpr, 0x69);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstCallBlendColor3);
#endif

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallBlendColor4);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallGetStockDevilNums);

INCLUDE_ASM(const s32, "code_003BF2E0", rstCallGetRand);

INCLUDE_ASM(const s32, "code_003BF2E0", rstLeaf9);

INCLUDE_RODATA(const s32, "code_003BF2E0", rstResultNakamaLevelTable);

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

/* NON_MATCHING: 171 of 216 words match.  */
#ifdef NON_MATCHING
s32 rstSetLvUpSkl(s32 index, s32 *count) {
    SBTLMEMORYSKILL_WORK *mem;
    s16 bgn;
    s16 num;
    s32 i;
    s32 j;
    s32 n;
    s32 skill;
    s32 diff;

    memset(&g_aLvUpSkl[index], 0, sizeof(_LvUpSkl));
    if (!(g_pUnitSrc->flag & 1) || !(g_pUnitDst->flag & 1)) {
        return 1;
    }
    diff = g_pUnitDst->level - g_pUnitSrc->level;
    g_dvlLevelUpNum[index] = diff;
    if (diff > 0) {
        ++*count;
        num = g_pUnitDst->level - gohData7[g_pUnitSrc->devilId].level;
        bgn = g_pUnitSrc->level - gohData7[g_pUnitSrc->devilId].level;
        if (num >= 4) {
            num = 3;
        }
        num -= bgn;
        printf("bgn %d,num %d\n", bgn, num);
        for (i = 0; i < num; i++) {
            if (g_pUnitSrc->aRsvSkl[bgn + i] != 0) {
                g_aLvUpSkl[index].nmlSkl[i] = g_pUnitSrc->aRsvSkl[bgn + i];
                g_aLvUpSkl[index].nmlSklPos[i] = bgn + i;
                g_pUnitDst->haveskill[bgn + i + 1] = g_aLvUpSkl[index].nmlSkl[i];
                printf("learn nmlSkl %x %d\n", g_aLvUpSkl[index].nmlSkl[i], bgn + i + 1);
            }
        }
    }
    if (g_pUnitDst->chuusei >= 100 && g_pUnitSrc->chuusei < 100) {
        n = 0;
        for (i = 0; i < 2; i++) {
            mem = &gohData7[g_pUnitSrc->devilId].memskill[i];
            skill = mem->skill;
            if (skill == 0) {
                continue;
            }
            if (g_pUnitDst->chuusei < mem->level) {
                continue;
            }
            if (g_pUnitSrc->bOmoideLearned[i]) {
                continue;
            }
            g_pUnitDst->bOmoideLearned[i] = 1;
            for (j = 0; j < 6; j++) {
                if (g_pUnitSrc->aOmoideSkill[j] == skill) {
                    goto next;
                }
            }
            g_aLvUpSkl[index].memSkl[n] = skill;
            g_aLvUpSkl[index].bGetMemSkl[i] = 1;
            s_bRegistMemSkill[index][n] = datOmoideSkillGetMaster(g_pUnitDst, skill);
            datOmoideSkillSetMaster(g_pUnitDst, g_aLvUpSkl[index].memSkl[n], 1);
            ++*count;
            if (g_dvlLevelUpNum[index] == 0) {
                g_dvlLevelUpNum[index] = -1;
            }
            n++;
        next:;
        }
    }
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", rstSetLvUpSkl);
#endif

/* NON_MATCHING: 105 of 113 words match.  */
#ifdef NON_MATCHING
s32 cmpLoadLocalizeText(void)
{
    s32 count;
    s32 i;

    rstData8 = &DDS3_GBWK->playerData;
    rstData9 = rstGetPlayerUnit();
    evtCallCheckRankUpData2 = 0;
    evtCallCheckRankUpData3 = 0;
    for (i = 0; i < 20; i++) {
        g_dvlLevelUpNum[i] = 0;
        rstWork2[i] = 0;
        rstOmoideGaIppaiWork4[i] = SKLID_INIT;
        rstOmoideGaIppaiWork5[i] = SKLID_INIT;
    }
    count = 0;
    count = rstData9->unit.level - rstData8->unit.level;
    evtCallCheckRankUpData2 = count;
    if (rstCheckRankUp() >= 0) {
        evtCallCheckRankUpData3 = 1;
        count++;
    }
    for (i = 0; i < 20; i++) {
        s_bRegistMemSkill[i][0] = 0;
        s_bRegistMemSkill[i][1] = 0;
        g_pUnitSrc = &rstData8->stock[i];
        g_pUnitDst = &rstData9->stock[i];
        rstSetLvUpSkl(i, &count);
        if (!(g_pUnitSrc->flag & 1)) {
            continue;
        }
        if (!(g_pUnitDst->flag & 1)) {
            continue;
        }
        if (g_pUnitDst->chuusei >= 100 && g_pUnitSrc->chuusei < 100) {
            rstWork2[i] = 1;
            count++;
        }
        rstGetGiftItem(i);
        rstGetGiftItemNums(i);
    }
    return count > 0;
}
#else
INCLUDE_ASM(const s32, "code_003BF2E0", cmpLoadLocalizeText);
#endif

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

INCLUDE_RODATA(const s32, "code_003BF2E0", rstStrResultGrowth);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstCallBlendColor4Data);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstCallBlendColor3Data4);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstCallBlendColor4Data2);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstCallBlendColor4Data3);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstCallBlendColor4Data4);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstCallPlaySEData);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstCallPlaySEData2);

INCLUDE_SDATA(const s32, "code_003BF2E0", g_mainPID3);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstData8);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstData9);

INCLUDE_SDATA(const s32, "code_003BF2E0", g_pUnitSrc);

INCLUDE_SDATA(const s32, "code_003BF2E0", g_pUnitDst);

INCLUDE_SDATA(const s32, "code_003BF2E0", evtCallCheckRankUpData2);

INCLUDE_SDATA(const s32, "code_003BF2E0", evtCallCheckRankUpData3);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstData11);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstData12);

INCLUDE_SDATA(const s32, "code_003BF2E0", rstCallStartLevelUp2Data);

