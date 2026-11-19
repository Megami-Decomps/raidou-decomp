#include "common.h"
#include "dds3KernelCore.h"
#include "itf.h"
#include "dds3PadManager.h"
#include "sdfMemory.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "code_r1_002C8188.h"
#include "code_00195698.h"
#include "sdfDraw.h"
#include "sdf2D.h"
#include "sdfPacketBuff.h"
#include "sdfMenuAnchor.h"

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

void awdMenuStart(void) {
    struct tagITF_MONEY_WORK *wk;

    if (g_mainPID2 != NULL && dds3ExistProcess(g_mainPID2)) {
        wk = dds3GetProcessData(g_mainPID2);
        if (!(wk->flags & 1)) {
            wk->flags |= 1;
            wk->step = 5;
            wk->count = 0;
        }
    }
}

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/00195800.c:1, match */
s32 awdCallGetProcessStat(void) {
    if (g_mainPID2 == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(g_mainPID2) == 3 || dds3GetProcessStat(g_mainPID2) == 0) {
        g_mainPID2 = NULL;
        return 0;
    }
    return 1;
}

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

s32 ListMakeListStation(dds3ProcessID_t *PID) {
    tagMAP_MENU_WORK *work = dds3GetProcessData(mapMenuProcessCheckData);

    itfMenuLinkSprite(PID, work->hSpr);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_TITLE, 0, 0, 0, 0, 23, 24, NULL, ListMakeListStationData3);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_PANEL, 322, 71, 128, 23, 17, 4, NULL, ListMakeListStationData);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_SELECT, 322, 71, 128, 23, 17, 4, NULL, ListMakeListStationData2);
    itfMenuSetSpriteData(PID, ITFMENU_IMG_DAT_SCROLL, 0, 0, 3, 163, 490, 84, NULL, ListMakeListStationData4);
    return itfMenuSetImageColor(PID, ITFMENU_IMG_DAT_SCROLL, 200, 180, 80, 128);
}

INCLUDE_ASM(const s32, "code_00195698", savePointMenuCreateMenuPID);

INCLUDE_ASM(const s32, "code_00195698", savePointMenuChkEnableDecide);

void savePointMenuCallProcEnd(tagMAP_MENU_WORK *wk) {
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
                SDF_MENU_SET_ANCHOR(wk->menuPID, SDF_MENU_ANCHOR_RIGHT);
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
                savePointMenuCallProcEnd(wk);
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

s32 mapCallGetProcessData(void)
{
    tagMAP_MENU_WORK *wk = dds3GetProcessData(mapMenuProcessCheckData);

    return wk->selectID;
}

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

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/00196C48.c:1, match */
s32 mapMenuProcessCheck(void) {
    if (mapMenuProcessCheckData == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(mapMenuProcessCheckData) == 3 || dds3GetProcessStat(mapMenuProcessCheckData) == 0) {
        mapMenuProcessCheckData = NULL;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_00195698", awdMenuMakeList);

INCLUDE_ASM(const s32, "code_00195698", mjnAnimFunc);

INCLUDE_ASM(const s32, "code_00195698", SetCommand);

INCLUDE_ASM(const s32, "code_00195698", mjnUpdateMessageSub);

INCLUDE_ASM(const s32, "code_00195698", mjnCallMenuProcessCheck);

INCLUDE_ASM(const s32, "code_00195698", UpdateMessage);

INCLUDE_ASM(const s32, "code_00195698", awdCallGetProcessData);

INCLUDE_ASM(const s32, "code_00195698", awdCallGetProcessData2);

s32 awdCallGetProcessData3(void)
{
    awdMenu_t *wk = dds3GetProcessData(cmpCallGetProcessStatData);

    return wk->selectID;
}

INCLUDE_ASM(const s32, "code_00195698", awdMenuRelease);

INCLUDE_ASM(const s32, "code_00195698", awdMenuShutdown);

INCLUDE_ASM(const s32, "code_00195698", evtInitEventViewer);

void evtCallExistProcess(void) {
    awdMenu_t *wk;

    if (cmpCallGetProcessStatData != NULL && dds3ExistProcess(cmpCallGetProcessStatData)) {
        wk = dds3GetProcessData(cmpCallGetProcessStatData);
        if (!(wk->flag & 1)) {
            wk->flag |= 1;
            wk->step = 6;
            wk->count = 0;
        }
    }
}

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

s32 elvMenuGetSelectID(void)
{
    elvMenu_t *wk = dds3GetProcessData(elvCallGetProcessStatData);

    return wk->selectID;
}

INCLUDE_ASM(const s32, "code_00195698", elvMenuRelease);

void elvMenuShutdown(dds3ProcessID_t *PID) {
    elvMenu_t *wk = dds3GetProcessData(PID);

    elvMenuRelease(wk);
    sdfMemFreeDirect(wk->hMem);
    elvCallGetProcessStatData = NULL;
}

INCLUDE_ASM(const s32, "code_00195698", elvMenuProcessStart);

void elvCallExistProcess2(void) {
    elvMenu_t *wk;

    if (elvCallGetProcessStatData != NULL && dds3ExistProcess(elvCallGetProcessStatData)) {
        wk = dds3GetProcessData(elvCallGetProcessStatData);
        if (!(wk->flag & 1)) {
            wk->flag |= 1;
            wk->step = 2;
            wk->count = 0;
        }
    }
}

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

void mphCommandBUSTUPLOADSub(s32 id)
{
    mphStartLoad(mphCommandBUSTUPLOADSubData[id]);
}

INCLUDE_ASM(const s32, "code_00195698", mphLoadFileProcess);

INCLUDE_ASM(const s32, "code_00195698", mphCommandBUSTUPLOADWAITSub);

INCLUDE_ASM(const s32, "code_00195698", mphCheckStat);

INCLUDE_ASM(const s32, "code_00195698", mphEndDraw);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/00197BF0.c:1, match */
void mphEnd(void) {
    mphDestroy();
}

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

typedef struct { u8 unk00[0x20]; u64 value, registerCode; } mphGsState_t;

void mphCallGrAllocPacketBuff4(sdfDrawTag_t *pdrawtag, s32 wms, s32 wmt, s32 min_u, s32 max_u, s32 min_v, s32 max_v) {
    s32 packet = sdfGrAllocPacketBuff(0x30);
    mphGsState_t *dst = EE_ADDR_AS(mphGsState_t *, sdf2DMakeStdPacket(packet, 0x30));

    dst->value = ((u64)max_v << 34) | ((u64)min_v << 24) | (max_u << 14) | (min_u << 4) | (wmt << 2) | wms;
    dst->registerCode = 8;
    sdfDrawTagLink(pdrawtag, packet);
}

INCLUDE_ASM(const s32, "code_00195698", mphCallDGetPrimPacketSize);

INCLUDE_ASM(const s32, "code_00195698", mphCallDGetPrimPacketSize2);

INCLUDE_ASM(const s32, "code_00195698", mphCallDGetPrimPacketSize3);

INCLUDE_ASM(const s32, "code_00195698", mphFunc);

INCLUDE_ASM(const s32, "code_00195698", mphSetPoints);

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
