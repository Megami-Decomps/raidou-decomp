#include "common.h"
#include "dds3KernelCore.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "datCalc.h"
#include "code_r1_002C8188.h"
#include "code_00195698.h"

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuChkSetMenu);

INCLUDE_ASM(const s32, "code_001A5D20", itfCallDevLoadFileEx);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuGetMenuNo);

s32 itfMenuLinkSprite(dds3ProcessID_t *PID, struct sprHandle_t *a_hSpr)
{
    s32 ret;
    s32 i;

    if (a_hSpr == NULL) {
        return 0;
    }
    ret = 1;
    for (i = 0; i < ITFMENU_IMG_DAT_MAX; i++) {
        if (itfMenuGetMenuNo(PID, i, a_hSpr) == 0) {
            ret = 0;
        }
    }
    return ret;
}

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuSetSpriteData);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuSetImageColorSub);

s32 itfMenuSetImageColor(dds3ProcessID_t *PID, ITFMENU_IMG_DAT a_type, u8 a_R, u8 a_G, u8 a_B, u8 a_A)
{
    return itfMenuSetImageColorSub(dds3GetProcessData(PID), a_type, a_R, a_G, a_B, a_A);
}

INCLUDE_ASM(const s32, "code_001A5D20", GetSelectRootID);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuImgGetRatio);

INCLUDE_ASM(const s32, "code_001A5D20", evtPictureSetPri);

INCLUDE_ASM(const s32, "code_001A5D20", itfMoverSetMenu);

INCLUDE_ASM(const s32, "code_001A5D20", itfCallGetProcessData7);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuSetStringWidth);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuGetPanelParam);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuSetFunc);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuSetDrawFunc);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuSetFont);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuSetDrawParam);

INCLUDE_ASM(const s32, "code_001A5D20", cmpGetWork);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuSetMoveType);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuGetCtrlType);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuUpdateLink);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuSetSelectID);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuSetCancelSE);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuSetAnimDelMenu);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuSetAnimBakMenu);

INCLUDE_ASM(const s32, "code_001A5D20", itfCallGetProcessData8);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuGetFlashCount);

INCLUDE_ASM(const s32, "code_001A5D20", itfLeaf);

INCLUDE_ASM(const s32, "code_001A5D20", itfLeaf2);

INCLUDE_ASM(const s32, "code_001A5D20", MenuRemakeLinkSub);

INCLUDE_ASM(const s32, "code_001A5D20", MenuDelMenuSub);

INCLUDE_ASM(const s32, "code_001A5D20", MenuInit_itf);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuCtrlSelectSub);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuCtrlSelectSub2);

INCLUDE_ASM(const s32, "code_001A5D20", itfCallMenuUpdateSelectMenu);

INCLUDE_ASM(const s32, "code_001A5D20", MenuPadCtrlSelect);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuMoveHierarchySub);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuMoveHierarchySub2);

INCLUDE_ASM(const s32, "code_001A5D20", MenuMoveHierarchy);

INCLUDE_ASM(const s32, "code_001A5D20", MenuPlayDecideSE);

INCLUDE_ASM(const s32, "code_001A5D20", MenuPlayCancelSE);

INCLUDE_ASM(const s32, "code_001A5D20", MenuProcess);

INCLUDE_ASM(const s32, "code_001A5D20", cmpCallDrawFont);

INCLUDE_ASM(const s32, "code_001A5D20", cmpCallSetOt);

INCLUDE_ASM(const s32, "code_001A5D20", MenuDrawScrollBar);

INCLUDE_ASM(const s32, "code_001A5D20", MenuDrawPanelEGL);

INCLUDE_ASM(const s32, "code_001A5D20", MenuDrawPanel);

INCLUDE_ASM(const s32, "code_001A5D20", MenuDrawTitle);

INCLUDE_ASM(const s32, "code_001A5D20", MenuCalcAlpha);

INCLUDE_ASM(const s32, "code_001A5D20", MenuDrawProcess);

INCLUDE_ASM(const s32, "code_001A5D20", cmpSklTypeSet);

INCLUDE_ASM(const s32, "code_001A5D20", cmpCallMenuProcessCheck);

INCLUDE_ASM(const s32, "code_001A5D20", cmpCallGetProcessData);

INCLUDE_ASM(const s32, "code_001A5D20", MenuRelease);

INCLUDE_ASM(const s32, "code_001A5D20", MenuShutdown);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuProcessStart);

INCLUDE_ASM(const s32, "code_001A5D20", itfMenuProcessEnd);

s32 itfMenuProcessCheck(dds3ProcessID_t *PID)
{
    if (PID == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(PID) == 3 || dds3GetProcessStat(PID) == 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_001A5D20", sbtlGetShortCutFlg);

INCLUDE_ASM(const s32, "code_001A5D20", sbtlCallGetProcessData);

INCLUDE_ASM(const s32, "code_001A5D20", itfCountSetData);

INCLUDE_ASM(const s32, "code_001A5D20", itfCountSetMax);

INCLUDE_ASM(const s32, "code_001A5D20", itfCountSetMin);

INCLUDE_ASM(const s32, "code_001A5D20", CountShutdown);

INCLUDE_ASM(const s32, "code_001A5D20", itfCountProcessStart);

INCLUDE_ASM(const s32, "code_001A5D20", itfCountProcessStartMoney);

INCLUDE_ASM(const s32, "code_001A5D20", itfCountProcessEnd);

INCLUDE_ASM(const s32, "code_001A5D20", itfCountProcessCheck);

INCLUDE_ASM(const s32, "code_001A5D20", DvlMsgCreateCB);

INCLUDE_ASM(const s32, "code_001A5D20", DvlMsgDeleteCB);

INCLUDE_ASM(const s32, "code_001A5D20", DvlMsgGetCB);

INCLUDE_ASM(const s32, "code_001A5D20", DvlMsgFreeMsg);

INCLUDE_ASM(const s32, "code_001A5D20", DvlMsgFreeTex);

INCLUDE_ASM(const s32, "code_001A5D20", DvlMsgLoadFile);

INCLUDE_ASM(const s32, "code_001A5D20", itfDvlMsgCreateData);

INCLUDE_ASM(const s32, "code_001A5D20", _DvlMsgDeleteData);

INCLUDE_ASM(const s32, "code_001A5D20", itfDvlMsgDeleteData);

INCLUDE_ASM(const s32, "code_001A5D20", DvlMsgLoadSprite);

INCLUDE_ASM(const s32, "code_001A5D20", itfDvlMsgStart);

INCLUDE_ASM(const s32, "code_001A5D20", itfDvlMsgEnd);

INCLUDE_ASM(const s32, "code_001A5D20", DvlMsgSetMover);

INCLUDE_ASM(const s32, "code_001A5D20", itfCallDvlMsgGetCB);

INCLUDE_ASM(const s32, "code_001A5D20", DvlMsgRelease);

INCLUDE_ASM(const s32, "code_001A5D20", DvlMsgProcess);

INCLUDE_ASM(const s32, "code_001A5D20", itfCallMessageNumSearch);

INCLUDE_ASM(const s32, "code_001A5D20", itfCallSetOt4);

INCLUDE_ASM(const s32, "code_001A5D20", itfCallGetProcessData9);

INCLUDE_ASM(const s32, "code_001A5D20", itfLeaf25);

INCLUDE_ASM(const s32, "code_001A5D20", DvlMsgShutdown);

INCLUDE_ASM(const s32, "code_001A5D20", itfDvlMsgProcessStart);

INCLUDE_ASM(const s32, "code_001A5D20", itfDvlMsgProcessEnd_itf);

INCLUDE_ASM(const s32, "code_001A5D20", itfCallExistProcess);

INCLUDE_ASM(const s32, "code_001A5D20", itfMoneyProcessSub);

INCLUDE_ASM(const s32, "code_001A5D20", MoneyCheckProcEnd);

s32 MoneyProcess(dds3ProcessID_t *PID)
{
    struct tagITF_MONEY_WORK *wk = dds3GetProcessData(PID);

    switch (wk->step) {
    case 0:
        if (wk->flags & 2) {
            wk->step = 1;
            wk->flags = (wk->flags & ~2) | 0x80;
            wk->count = 0;
        }
        break;
    case 1:
        if (wk->count < 10) {
            wk->count++;
        } else {
            wk->count = 0;
            wk->step = 2;
        }
        break;
    case 2:
        if (wk->flags & 8) {
            wk->flags &= ~8;
            wk->countPID = NULL;
            if (wk->calc > 0) {
                if (wk->type == 2) {
                    datAddMaka(wk->calc);
                } else if (wk->type == 1) {
                    datAddMaka(-wk->calc);
                }
                wk->countPID = itfCountProcessStartMoney(wk->calc);
                itfCountSetData(wk->countPID, &wk->money, wk->type);
                itfCountSetMin(wk->countPID, &wk->money, 0);
            }
            wk->step = 3;
        } else if (wk->flags & 4) {
            wk->step = 4;
            wk->flags &= ~4;
            wk->count = 0;
            wk->countPID = NULL;
        }
        break;
    case 3:
        if (itfCountProcessCheck(wk->countPID) != 0) {
            break;
        }
        wk->countPID = NULL;
        if (wk->flags & 4) {
            wk->step = 4;
            wk->flags &= ~4;
            wk->count = 0;
        }
        break;
    case 4:
        if (wk->count < 10) {
            wk->count++;
        } else {
            wk->step = 0;
            wk->count = 0;
            wk->flags &= ~0x80;
        }
        break;
    case 5:
        if (wk->count == 0) {
            itfMoneyProcessSub(wk);
        }
        if (MoneyCheckProcEnd(wk) == 0) {
            wk->count = 0;
            wk->step = 6;
        } else {
            wk->count++;
        }
        break;
    case 6:
        if (wk->count == 0) {
            wk->flags &= ~0x80;
        } else if (wk->count >= 2) {
            wk->count = 0;
            wk->step = 7;
            break;
        }
        wk->count++;
        break;
    case 7:
        return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_001A5D20", MoneyDrawPanel);

INCLUDE_ASM(const s32, "code_001A5D20", MoneyDrawProcess);

INCLUDE_ASM(const s32, "code_001A5D20", MoneyShutdown);

void itfMoneyProcessStart(void) {
    sdfMemHandle_t *hMem;
    struct tagITF_MONEY_WORK *wk;

    hMem = sdfMemAllocDirect(sizeof(struct tagITF_MONEY_WORK));
    wk = sdfMemGetPtr(hMem);
    memset(wk, 0, sizeof(struct tagITF_MONEY_WORK));
    wk->hMem = hMem;
    g_mainPID2 = dds3StartProcess(itfStrItfmoneyMain, 0x3FF, 0, 0, MoneyProcess, (dds3ProcessFunc_t)MoneyShutdown, wk);
    wk->drawPID = dds3StartProcess(itfStrItfmoneyDraw, 0x2B07, 0, 0, MoneyDrawProcess, NULL, wk);
}

INCLUDE_ASM(const s32, "code_001A5D20", awdMenuStart);

INCLUDE_ASM(const s32, "code_001A5D20", awdCallGetProcessStat);

INCLUDE_ASM(const s32, "code_001A5D20", awdMenuGetSelectID);

INCLUDE_ASM(const s32, "code_001A5D20", mapMenuCheckWait);

INCLUDE_ASM(const s32, "code_001A5D20", mapMenuStart);

INCLUDE_ASM(const s32, "code_001A5D20", mapMenuDrawMagWindow);

INCLUDE_ASM(const s32, "code_001A5D20", itfGeneralMenuLoadSpriteHnd);

void itfHanyoMenu(void) {
    effQueuedResourceNameRecord5 = sdfDevLoadFileEx(itfStrItfHanyoMenuSpr, NULL, NULL);
}

INCLUDE_ASM(const s32, "code_001A5D20", itfCallMemFreeDelayedDirect2);

INCLUDE_ASM(const s32, "code_001A5D20", itfInitEventViewerSub);

INCLUDE_ASM(const s32, "code_001A5D20", itfGeneralMenuFreeSpriteHnd);

INCLUDE_ASM(const s32, "code_001A5D20", itfGeneralMenuDecideFunc);

INCLUDE_ASM(const s32, "code_001A5D20", campGetCursorColorSel);

INCLUDE_ASM(const s32, "code_001A5D20", itfGeneralMenuCreatePID);

INCLUDE_ASM(const s32, "code_001A5D20", mapMenuDecideFunc);

INCLUDE_ASM(const s32, "code_001A5D20", GetMenuHelpID);

INCLUDE_ASM(const s32, "code_001A5D20", mapMenuDrawFuncMenu);

INCLUDE_ASM(const s32, "code_001A5D20", mapMenuDrawFunc);

INCLUDE_ASM(const s32, "code_001A5D20", ListMakeListStation);

INCLUDE_ASM(const s32, "code_001A5D20", mapMenuResetMenu);

INCLUDE_ASM(const s32, "code_001A5D20", mapCallGetStockDevilNums);

INCLUDE_ASM(const s32, "code_001A5D20", mapMenuChkEnableDecide);

INCLUDE_ASM(const s32, "code_001A5D20", saveLeaf);

s32 saveCallMenuProcessCheck2(tagMAP_MENU_WORK *wk) {
    s32 ret = 0;

    if (itfMenuProcessCheck(wk->menuPID) != 0) {
        ret = 1;
    } else {
        wk->menuPID = NULL;
    }
    return ret;
}

INCLUDE_ASM(const s32, "code_001A5D20", lmapMenuProcess);

INCLUDE_ASM(const s32, "code_001A5D20", nmeCalc);

INCLUDE_ASM(const s32, "code_001A5D20", mapMenuGetSelectUNIQ);

INCLUDE_ASM(const s32, "code_001A5D20", mapMenuGetSelectID);

INCLUDE_ASM(const s32, "code_001A5D20", mapCallGetProcessData);

void mapMenuRelease(tagMAP_MENU_WORK *wk) {
    if (wk->hSpr != NULL) {
        effDestroyResourceSlotSet(wk->hSpr);
        wk->hSpr = NULL;
    }
}

void mapMenuShutdown(dds3ProcessID_t *PID) {
    tagMAP_MENU_WORK *wk = dds3GetProcessData(PID);

    mapMenuRelease(wk);
    sdfMemFreeDirect(wk->hMem);
    mapMenuProcessCheckData = NULL;
}

INCLUDE_ASM(const s32, "code_001A5D20", mapMenuProcessStart);

INCLUDE_ASM(const s32, "code_001A5D20", awdMenuProcessEnd);

INCLUDE_ASM(const s32, "code_001A5D20", mapMenuProcessCheck);

INCLUDE_ASM(const s32, "code_001A5D20", awdMenuMakeList);

INCLUDE_ASM(const s32, "code_001A5D20", awdCallBitCheck);

INCLUDE_ASM(const s32, "code_001A5D20", awdCallGeneralMenuCreatePID);

INCLUDE_ASM(const s32, "code_001A5D20", mjnUpdateMessageSub);

INCLUDE_ASM(const s32, "code_001A5D20", mjnCallMenuProcessCheck);

INCLUDE_ASM(const s32, "code_001A5D20", UpdateMessage);

INCLUDE_ASM(const s32, "code_001A5D20", awdCallGetProcessData);

INCLUDE_ASM(const s32, "code_001A5D20", awdCallGetProcessData2);

INCLUDE_ASM(const s32, "code_001A5D20", awdCallGetProcessData3);

INCLUDE_ASM(const s32, "code_001A5D20", awdMenuRelease);

INCLUDE_ASM(const s32, "code_001A5D20", awdMenuShutdown);

INCLUDE_ASM(const s32, "code_001A5D20", evtInitEventViewer);

INCLUDE_ASM(const s32, "code_001A5D20", evtCallExistProcess);

INCLUDE_ASM(const s32, "code_001A5D20", cmpCallGetProcessStat);

INCLUDE_ASM(const s32, "code_001A5D20", cmpPartyListMake);

INCLUDE_ASM(const s32, "code_001A5D20", cmpLeaf);

INCLUDE_ASM(const s32, "code_001A5D20", cmpCallGeneralMenuCreatePID);

INCLUDE_ASM(const s32, "code_001A5D20", cmpLeaf33);

INCLUDE_ASM(const s32, "code_001A5D20", elvCallMenuProcessCheck);

INCLUDE_ASM(const s32, "code_001A5D20", elvCallGetProcessData6);

INCLUDE_ASM(const s32, "code_001A5D20", elvMenuCheckWait);

INCLUDE_ASM(const s32, "code_001A5D20", elvMenuStart);

INCLUDE_ASM(const s32, "code_001A5D20", elvCallGetProcessData);

INCLUDE_ASM(const s32, "code_001A5D20", elvCallGeneralMenuFreeSpriteHnd);

INCLUDE_ASM(const s32, "code_001A5D20", elvCallGetProcessData2);

INCLUDE_ASM(const s32, "code_001A5D20", elvKaisouMenu);

INCLUDE_ASM(const s32, "code_001A5D20", elvCallExistProcess2);

INCLUDE_ASM(const s32, "code_001A5D20", lmapCallGetProcessStat);

INCLUDE_ASM(const s32, "code_001A5D20", lmapCallGetProcessData);

INCLUDE_ASM(const s32, "code_001A5D20", lmapCallGetProcessData2);

INCLUDE_ASM(const s32, "code_001A5D20", lmapCallGetProcessData3);

INCLUDE_ASM(const s32, "code_001A5D20", lmapMenuCreateMenuPID);

INCLUDE_ASM(const s32, "code_001A5D20", lmapCallMenuSetFlagMenu);

INCLUDE_ASM(const s32, "code_001A5D20", lmapLeaf);

INCLUDE_ASM(const s32, "code_001A5D20", lmapCallMenuProcessCheck);

INCLUDE_ASM(const s32, "code_001A5D20", elvCallGetProcessData3);

INCLUDE_ASM(const s32, "code_001A5D20", elvCallGetProcessData4);

INCLUDE_ASM(const s32, "code_001A5D20", elvCallGetProcessData5);

INCLUDE_ASM(const s32, "code_001A5D20", elvMenuGetSelectID);

INCLUDE_ASM(const s32, "code_001A5D20", elvMenuRelease);

INCLUDE_ASM(const s32, "code_001A5D20", elvMenuShutdown);

INCLUDE_ASM(const s32, "code_001A5D20", elvMenuProcessStart);

INCLUDE_ASM(const s32, "code_001A5D20", elvCallExistProcess);

INCLUDE_ASM(const s32, "code_001A5D20", elvCallGetProcessStat);

INCLUDE_ASM(const s32, "code_001A5D20", mphCallGetProcessData);

INCLUDE_ASM(const s32, "code_001A5D20", mphCommand_BUSTUP_LOAD);

INCLUDE_ASM(const s32, "code_001A5D20", mphCallGetProcessData2);

INCLUDE_ASM(const s32, "code_001A5D20", mphCallMenuProcessStart);

INCLUDE_ASM(const s32, "code_001A5D20", mphCallMenuClearMenuAll);

INCLUDE_ASM(const s32, "code_001A5D20", mphCallMenuSetFlagMenu);

INCLUDE_ASM(const s32, "code_001A5D20", mphLeaf);

INCLUDE_ASM(const s32, "code_001A5D20", mphCallMenuProcessCheck);

INCLUDE_ASM(const s32, "code_001A5D20", mphCallGetProcessData3);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallGetProcessData);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallGetProcessData2);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallGetProcessData3);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallGetProcessData4);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallGeneralMenuFreeSpriteHnd);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallGetProcessData5);

INCLUDE_ASM(const s32, "code_001A5D20", savePointMenuProcessStart);

INCLUDE_ASM(const s32, "code_001A5D20", savePointMenuStart);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallGetProcessStat);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallGetProcessData6);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallGetProcessData7);

INCLUDE_ASM(const s32, "code_001A5D20", savePointMenuDrawFuncMenu);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallMenuProcessStart);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallMenuSetFlagMenu);

INCLUDE_ASM(const s32, "code_001A5D20", savePointMenuProcessSub);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallMenuProcessCheck);

INCLUDE_ASM(const s32, "code_001A5D20", savePointMenuProcess);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallGetProcessData8);

INCLUDE_ASM(const s32, "code_001A5D20", saveCallGetProcessData9);

INCLUDE_ASM(const s32, "code_001A5D20", mphCallGetProcessData4);

INCLUDE_ASM(const s32, "code_001A5D20", mphStart);

INCLUDE_ASM(const s32, "code_001A5D20", mphShutDown);

INCLUDE_ASM(const s32, "code_001A5D20", mphYataMenu);

INCLUDE_ASM(const s32, "code_001A5D20", mphCallExistProcess);

INCLUDE_ASM(const s32, "code_001A5D20", mphCallGetProcessStat);

INCLUDE_RODATA(const s32, "code_001A5D20", MoneyDrawPanelTable);

INCLUDE_RODATA(const s32, "code_001A5D20", MoneyDrawPanelTable2);

INCLUDE_RODATA(const s32, "code_001A5D20", itfStrItfmoneyMain);

INCLUDE_RODATA(const s32, "code_001A5D20", itfStrItfmoneyDraw);

INCLUDE_RODATA(const s32, "code_001A5D20", itfStrItfHanyoMenuSpr);

