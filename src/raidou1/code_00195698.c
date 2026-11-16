#include "common.h"
#include "dds3KernelCore.h"
#include "itf.h"
#include "dds3PadManager.h"
#include "sdfMemory.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "code_r1_002C8188.h"
#include "code_00195698.h"

INCLUDE_ASM(const s32, "code_00195698", MoneyShutdown);

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

INCLUDE_ASM(const s32, "code_00195698", awdMenuStart);

INCLUDE_ASM(const s32, "code_00195698", awdCallGetProcessStat);

INCLUDE_ASM(const s32, "code_00195698", awdMenuGetSelectID);

INCLUDE_ASM(const s32, "code_00195698", mapMenuCheckWait);

INCLUDE_ASM(const s32, "code_00195698", mapMenuStart);

INCLUDE_ASM(const s32, "code_00195698", mapMenuDrawMagWindow);

INCLUDE_ASM(const s32, "code_00195698", itfGeneralMenuLoadSpriteHnd);

void itfHanyoMenu(void) {
    effQueuedResourceNameRecord5 = sdfDevLoadFileEx(itfStrItfHanyoMenuSpr, NULL, NULL);
}

INCLUDE_ASM(const s32, "code_00195698", itfCallMemFreeDelayedDirect2);

INCLUDE_ASM(const s32, "code_00195698", itfInitEventViewerSub);

INCLUDE_ASM(const s32, "code_00195698", itfGeneralMenuFreeSpriteHnd);

INCLUDE_ASM(const s32, "code_00195698", itfGeneralMenuDecideFunc);

INCLUDE_ASM(const s32, "code_00195698", campGetCursorColorSel);

INCLUDE_ASM(const s32, "code_00195698", itfGeneralMenuCreatePID);

INCLUDE_ASM(const s32, "code_00195698", itfCallMenuProcessStart);

INCLUDE_ASM(const s32, "code_00195698", itfCallGetProcessData10);

INCLUDE_ASM(const s32, "code_00195698", itfCallGetProcessData11);

INCLUDE_ASM(const s32, "code_00195698", saveCallGetProcessData);

INCLUDE_ASM(const s32, "code_00195698", saveCallGetProcessData2);

INCLUDE_ASM(const s32, "code_00195698", ListMakeListStation);

INCLUDE_ASM(const s32, "code_00195698", savePointMenuCreateMenuPID);

INCLUDE_ASM(const s32, "code_00195698", savePointMenuChkEnableDecide);

void saveLeaf(tagMAP_MENU_WORK *wk) {
    itfMenuProcessEnd(wk->menuPID);
}

s32 saveCallMenuProcessCheck2(tagMAP_MENU_WORK *wk) {
    s32 ret = 0;

    if (itfMenuProcessCheck(wk->menuPID) != 0) {
        ret = 1;
    } else {
        wk->menuPID = NULL;
    }
    return ret;
}

s32 nmeCallGetProcessData(dds3ProcessID_t *id) {
    tagMAP_MENU_WORK *wk = dds3GetProcessData(id);

    if (dds3PadButtonState[0x2A] < 0) {
        cmpSklTypeSet(wk->menuPID);
        return 0;
    }
    switch (wk->step) {
        case 0:
            if (wk->flag & 0x10) {
                wk->flag &= ~0x10;
                wk->menuPID = savePointMenuCreateMenuPID();
                savePointMenuChkEnableDecide(wk);
                itfMenuStart(wk->menuPID);
                wk->selectID = -1;
                wk->step = 1;
            }
            break;
        case 1:
            if (itfMenuProcessCheck(wk->menuPID) == 0) {
                wk->menuPID = NULL;
                wk->step = 0;
            }
            break;
        case 2:
            if (wk->count == 0) {
                saveLeaf(wk);
            }
            if (saveCallMenuProcessCheck2(wk) == 0) {
                wk->count = 0;
                wk->step = 4;
            } else {
                wk->count++;
            }
            break;
        case 4:
            return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00195698", nmeCalc);

INCLUDE_ASM(const s32, "code_00195698", mapMenuGetSelectUNIQ);

INCLUDE_ASM(const s32, "code_00195698", mapCallGetProcessData);

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

void mapMenuProcessStart(void) {
    sdfMemHandle_t *hMem;
    tagMAP_MENU_WORK *wk;

    hMem = sdfMemAllocDirect(sizeof(tagMAP_MENU_WORK));
    wk = sdfMemGetPtr(hMem);
    memset(wk, 0, sizeof(tagMAP_MENU_WORK));
    wk->hMem = hMem;
    mapMenuProcessCheckData = dds3StartProcess(mapStrMapMenu, 0x3FE, 0, 0, nmeCallGetProcessData,
                                               (dds3ProcessFunc_t)mapMenuShutdown, wk);
    hMem = sdfDevLoadFileEx(mapStrBattlePanelBattleWakuSpr, NULL, NULL);
    wk->hSpr = effCreateResourceSlotSetFromAllocation(hMem, 0);
    sdfMemFreeDelayedDirect(hMem);
    wk->selectID = -1;
}

void awdMenuProcessEnd(void) {
    tagMAP_MENU_WORK *wk;

    if (mapMenuProcessCheckData != NULL && dds3ExistProcess(mapMenuProcessCheckData)) {
        wk = dds3GetProcessData(mapMenuProcessCheckData);
        if (!(wk->flag & 1)) {
            wk->flag |= 1;
            wk->step = 2;
            wk->count = 0;
        }
    }
}

INCLUDE_ASM(const s32, "code_00195698", mapMenuProcessCheck);

INCLUDE_ASM(const s32, "code_00195698", awdMenuMakeList);

INCLUDE_ASM(const s32, "code_00195698", mjnAnimFunc);

INCLUDE_ASM(const s32, "code_00195698", SetCommand);

INCLUDE_ASM(const s32, "code_00195698", mjnUpdateMessageSub);

INCLUDE_ASM(const s32, "code_00195698", mjnCallMenuProcessCheck);

INCLUDE_ASM(const s32, "code_00195698", UpdateMessage);

INCLUDE_ASM(const s32, "code_00195698", awdCallGetProcessData);

INCLUDE_ASM(const s32, "code_00195698", awdCallGetProcessData2);

INCLUDE_ASM(const s32, "code_00195698", awdCallGetProcessData3);

INCLUDE_ASM(const s32, "code_00195698", awdMenuRelease);

INCLUDE_ASM(const s32, "code_00195698", awdMenuShutdown);

INCLUDE_ASM(const s32, "code_00195698", evtInitEventViewer);

INCLUDE_ASM(const s32, "code_00195698", evtCallExistProcess);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/001973A0.c:1, found */
s32 cmpCallGetProcessStat(void) {
    if (cmpCallGetProcessStatData == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(cmpCallGetProcessStatData) == 3 || dds3GetProcessStat(cmpCallGetProcessStatData) == 0) {
        cmpCallGetProcessStatData = NULL;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_00195698", cmpPartyListMake);

INCLUDE_ASM(const s32, "code_00195698", cmpCallGeneralMenuCreatePID);

INCLUDE_ASM(const s32, "code_00195698", cmpLeaf33);

INCLUDE_ASM(const s32, "code_00195698", elvCallMenuProcessCheck);

INCLUDE_ASM(const s32, "code_00195698", elvCallGetProcessData6);

INCLUDE_ASM(const s32, "code_00195698", elvMenuCheckWait);

INCLUDE_ASM(const s32, "code_00195698", elvMenuStart);

INCLUDE_ASM(const s32, "code_00195698", elvMenuGetSelectID);

INCLUDE_ASM(const s32, "code_00195698", elvMenuRelease);

INCLUDE_ASM(const s32, "code_00195698", elvMenuShutdown);

INCLUDE_ASM(const s32, "code_00195698", elvMenuProcessStart);

INCLUDE_ASM(const s32, "code_00195698", elvCallExistProcess2);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/001978C8.c:1, found */
s32 elvCallGetProcessStat(void) {
    if (elvCallGetProcessStatData == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(elvCallGetProcessStatData) == 3 || dds3GetProcessStat(elvCallGetProcessStatData) == 0) {
        elvCallGetProcessStatData = NULL;
        return 0;
    }
    return 1;
}

void mphCallMemAllocDirect(void) {
    sdfMemHandle_t *hMem = sdfMemAllocDirect(sizeof(mphWork_t));

    mphWork = sdfMemLock(hMem);
    mphWork->hMem = hMem;
    memset(mphWork, 0, sizeof(mphWork_t));
    mphWork->fileStat = 0;
    mphWork->alpha = 0x80;
    mphWork->ot = MPH_DEFAULT_OT;
    mphWork->z = 0;
}

INCLUDE_ASM(const s32, "code_00195698", mphStartLoad);

INCLUDE_ASM(const s32, "code_00195698", mphCommandBUSTUPLOADSub);

INCLUDE_ASM(const s32, "code_00195698", mphLoadFileProcess);

INCLUDE_ASM(const s32, "code_00195698", mphCommandBUSTUPLOADWAITSub);

INCLUDE_ASM(const s32, "code_00195698", mphLeaf);

INCLUDE_ASM(const s32, "code_00195698", mphEndDraw);

INCLUDE_ASM(const s32, "code_00195698", mphEnd);

INCLUDE_ASM(const s32, "code_00195698", mphCallHandleFree);

INCLUDE_ASM(const s32, "code_00195698", mphOrder);

INCLUDE_ASM(const s32, "code_00195698", mphStartDraw);

INCLUDE_ASM(const s32, "code_00195698", mphStartDrawBtlRight);

INCLUDE_ASM(const s32, "code_00195698", mphStartDrawBtlLeft);

INCLUDE_ASM(const s32, "code_00195698", mphStartDrawSub);

INCLUDE_ASM(const s32, "code_00195698", mphSetAlpha);

INCLUDE_ASM(const s32, "code_00195698", mphGetAlpha);

INCLUDE_ASM(const s32, "code_00195698", mphSetZ);

INCLUDE_ASM(const s32, "code_00195698", mphGetZ);

INCLUDE_ASM(const s32, "code_00195698", mphSetOt);

INCLUDE_ASM(const s32, "code_00195698", mphGetOt);

INCLUDE_ASM(const s32, "code_00195698", mphCallMemAllocDirect2);

INCLUDE_ASM(const s32, "code_00195698", mphMorphFreeindData);

INCLUDE_ASM(const s32, "code_00195698", mphDestroy);

INCLUDE_ASM(const s32, "code_00195698", mphCallGrAllocPacketBuff);

INCLUDE_ASM(const s32, "code_00195698", mphCallGrAllocPacketBuff2);

INCLUDE_ASM(const s32, "code_00195698", mphCallGrAllocPacketBuff3);

INCLUDE_ASM(const s32, "code_00195698", mphCallGrAllocPacketBuff4);

INCLUDE_ASM(const s32, "code_00195698", mphCallDGetPrimPacketSize);

INCLUDE_ASM(const s32, "code_00195698", mphCallDGetPrimPacketSize2);

INCLUDE_ASM(const s32, "code_00195698", mphCallDGetPrimPacketSize3);

INCLUDE_ASM(const s32, "code_00195698", mphFunc);

INCLUDE_ASM(const s32, "code_00195698", mphLeaf2);

INCLUDE_ASM(const s32, "code_00195698", mphCommand_BUSTUP_LOAD);

INCLUDE_ASM(const s32, "code_00195698", mphCommand_BUSTUP_LOAD_WAIT);

INCLUDE_ASM(const s32, "code_00195698", mphCommand_BUSTUP_ON);

INCLUDE_ASM(const s32, "code_00195698", mphCommand_BUSTUP_OFF);

INCLUDE_ASM(const s32, "code_00195698", mphDebugDrawMenu);

INCLUDE_ASM(const s32, "code_00195698", mphLEFT);

INCLUDE_ASM(const s32, "code_00195698", mphDebugPadDealer);

INCLUDE_ASM(const s32, "code_00195698", mphDebugProcess);

INCLUDE_ASM(const s32, "code_00195698", mphStart);

INCLUDE_ASM(const s32, "code_00195698", mphStart_itf);

INCLUDE_ASM(const s32, "code_00195698", mphShutDown);
