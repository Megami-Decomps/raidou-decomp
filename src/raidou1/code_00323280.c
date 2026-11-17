#include "common.h"
#include "itf.h"
#include "dds3KernelCore.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "fld.h"
#include "fldProcess.h"
#include "cmpFile.h"
#include "cmpMap.h"
#include "code_r1_00323138.h"
#include "code_0016A7D8.h"
#include "code_00323280.h"
#include "code_0032F2B8.h"
#include "cmpMain.h"
#include "sdfTex.h"
#include "frFont.h"
#include "nwdNews.h"
#include "dds3WorldBasic.h"
#include "dds3AreaBasic.h"
#include "dds3Basic.h"

void DrawTop(struct tagCMP_MAP_WORK *wk) {
    struct sprHandle_t *spr;
    s32 flg;

    spr = cmpGetSpriteHnd(CMP_SPR_MAP);
    flg = fldDrawTopSub();
    if (spr == NULL) {
        return;
    }
    itfDrawMiscSpriteList(0, 0, 0, 0xFF, spr, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL, DrawTopData);
    if (flg & 4) {
        itfDrawMiscSpriteAlpha(242, 62, 0, 0xFF, spr, 21, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 8) {
        itfDrawMiscSpriteAlpha(242, 375, 0, 0xFF, spr, 22, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 0x10) {
        itfDrawMiscSpriteAlpha(26, 218, 0, 0xFF, spr, 20, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 0x20) {
        itfDrawMiscSpriteAlpha(459, 219, 0, 0xFF, spr, 19, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 1) {
        itfIconDrawSprite(11, 17, 0, 0xFF, 53, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(43, 15, 0, 0xFF, spr, 2, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 2) {
        itfIconDrawSprite(435, 17, 0, 0xFF, 54, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
        itfDrawMiscSpriteAlpha(467, 15, 0, 0xFF, spr, 3, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    itfIconDrawSprite(438, 419, 0, 0xFF, 13, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    itfDrawMiscSpriteAlpha(460, 420, 0, 0xFF, spr, 4, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
}

s32 DrawProcess(dds3ProcessID_t *PID) {
    tagCMP_MAP_WORK *wk = dds3GetProcessData(PID);

    if (!(wk->flag & 8)) {
        return 0;
    }
    cmpDrawBack(wk);
    cmpDrawBackPattern(wk);
    mapCallSetOt2();
    DrawTop(wk);
    return 0;
}

void Release_cmpMap(tagCMP_MAP_WORK *wk) {
    fldCallTexFreeDelayed();
    if (wk->flag & 2) {
        wk->flag &= ~2;
        cmpFreeSpriteMem(CMP_SPR_MAP);
    }
    if (wk->flag & 4) {
        wk->flag &= ~4;
        cmpFreeSpriteHnd(CMP_SPR_MAP);
    }
}

void Shutdown_cmpMap(dds3ProcessID_t *id) {
    tagCMP_MAP_WORK *wk = dds3GetProcessData(id);

    Release_cmpMap(wk);
    if (dds3ExistProcess(wk->drawPID)) {
        dds3KillProcess(wk->drawPID, 0);
    }
    sdfMemFreeDirect(wk->handle);
    g_mainPID_cmpMap = NULL;
}

void ProcessStart_cmpMap(void) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(sizeof(tagCMP_MAP_WORK));
    tagCMP_MAP_WORK *wk = sdfMemGetPtr(handle);

    memset(wk, 0, sizeof(tagCMP_MAP_WORK));
    wk->handle = handle;
    g_mainPID_cmpMap = dds3StartProcess("camp_automap_main", 1007, 0, 0, Process_cmpMap,
                                        (dds3ProcessFunc_t)Shutdown_cmpMap, wk);
    wk->drawPID = dds3StartProcess("camp_automap_draw", 11012, 0, 0, DrawProcess, NULL, wk);
    if (cmpGetSpriteHnd(CMP_SPR_MAP) == NULL) {
        if (cmpLoadSpriteHnd(CMP_SPR_MAP) == NULL) {
            cmpLoadSpriteMem(CMP_SPR_MAP);
            wk->flag |= 2;
        }
        pipeCreateLink(CMP_SPR_MAP);
        wk->flag |= 4;
    }
    fldMapAllAmap();
}

INCLUDE_ASM(const s32, "code_00323280", ProcessEnd_cmpMap);

s32 ProcessCheck_cmpMap(void) {
    if (g_mainPID_cmpMap == NULL) {
        return 1;
    }
    if (dds3GetProcessStat(g_mainPID_cmpMap) == 3) {
        g_mainPID_cmpMap = NULL;
        return 1;
    }
    if (dds3GetProcessStat(g_mainPID_cmpMap) == 0) {
        g_mainPID_cmpMap = NULL;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00323280", cmpRefMainPIDCmpMain);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc7);

INCLUDE_ASM(const s32, "code_00323280", cmpGetCharPID);

INCLUDE_ASM(const s32, "code_00323280", cmpGetsubMenuPID);

INCLUDE_ASM(const s32, "code_00323280", cmpCallGetCharPID11);

INCLUDE_ASM(const s32, "code_00323280", cmpCallMenuGetSelectID2);

INCLUDE_ASM(const s32, "code_00323280", cmpCallMenuSetFlagMenu2);

INCLUDE_ASM(const s32, "code_00323280", cmpProcess);

INCLUDE_ASM(const s32, "code_00323280", cmpMenuMaskSet);

INCLUDE_ASM(const s32, "code_00323280", cmpCallMenuSetMenuTitle);

INCLUDE_ASM(const s32, "code_00323280", TitleChange);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrValuables);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrThrowAway);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrVibration);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrSubscreen);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrMoneyOnHand);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrReleaseDemon);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrReorganizeDemons);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrChooseDemonTo);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrCheckValuables);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrThrowAwayItems);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrChangeTheEquippedBullets);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrUseItems);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrLoadSavedData);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrVibrationSettings);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrCheckTheMapOfTheCurrent);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrViewStatusForRaidouAnd);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrChangeDemonSettings);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrConsumeItemsReloadCheck);

INCLUDE_RODATA(const s32, "code_00323280", cmpStrUseDemonAbility);

INCLUDE_RODATA(const s32, "code_00323280", cmpCallMenuSetFlagMenu2Table);

INCLUDE_ASM(const s32, "code_00323280", cmpCallFunc);

INCLUDE_ASM(const s32, "code_00323280", cmpDrawKeyHelp2);

INCLUDE_ASM(const s32, "code_00323280", cmpCallDrawKeyHelp2);

INCLUDE_ASM(const s32, "code_00323280", cmpDrawKeyHelp);

INCLUDE_ASM(const s32, "code_00323280", cmpDrawMenuFunc);

INCLUDE_ASM(const s32, "code_00323280", cmpGetEvtItemSprNo);

INCLUDE_ASM(const s32, "code_00323280", cmpCallGetSpriteHnd7);

INCLUDE_ASM(const s32, "code_00323280", cmpCallGetSpriteHnd8);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf49);

INCLUDE_ASM(const s32, "code_00323280", cmpCallGetSpriteHnd9);

INCLUDE_ASM(const s32, "code_00323280", cmpCallGetSpriteHnd10);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc8);

INCLUDE_ASM(const s32, "code_00323280", cmpStatusDrawSub);

INCLUDE_ASM(const s32, "code_00323280", cmpCallGetSpriteHnd11);

INCLUDE_ASM(const s32, "code_00323280", cmpDrawRaidouSkill);

INCLUDE_ASM(const s32, "code_00323280", cmpCallSetOt3);

INCLUDE_ASM(const s32, "code_00323280", cmpCallSetOt4);

INCLUDE_ASM(const s32, "code_00323280", cmpCallSetOt5);

INCLUDE_ASM(const s32, "code_00323280", CmpStatusDraw);

INCLUDE_ASM(const s32, "code_00323280", cmpDrawProcess);

INCLUDE_ASM(const s32, "code_00323280", cmpCallGetProcessData6);

CMP_MODE cmpGetEndMode(void)
{
    return cmpCallFuncData;
}

void cmpSetMarkID(s32 a_markID)
{
    ((struct tagCMP_WORK *)dds3GetProcessData(g_mainPID_cmpMain))->markID = a_markID;
}

s32 cmpGetMarkID(void)
{
    return ((struct tagCMP_WORK *)dds3GetProcessData(g_mainPID_cmpMain))->markID;
}

INCLUDE_ASM(const s32, "code_00323280", cmpCallProcEnd);

INCLUDE_ASM(const s32, "code_00323280", cmpCheckProcEnd);

INCLUDE_ASM(const s32, "code_00323280", cmpRelease);

/* NON_MATCHING: 16 of 21 words match.  */
#ifdef NON_MATCHING
/* body: matched in raidou @ 61f09dc: src/raidou1/code_00323280.c:73, near, same function 0.92 (words 21/21, calls 0.80, shape 0.95) */
void cmpProcessStartEx(dds3ProcessID_t *id) {
    struct tagCMP_WORK *wk = dds3GetProcessData(id);

    cmpRelease(wk);
    if (dds3ExistProcess(wk->drawPID)) {
        dds3KillProcess(wk->drawPID, 0);
        wk->drawPID = NULL;
    }
    sdfMemFreeDirect(wk->hMem);
}
#else
INCLUDE_ASM(const s32, "code_00323280", cmpProcessStartEx);
#endif

INCLUDE_RODATA(const s32, "code_00323280", cmpCallDrawKeyHelp2Table);

INCLUDE_ASM(const s32, "code_00323280", cmpProcessStartEx_cmpMain);

INCLUDE_ASM(const s32, "code_00323280", cmpProcessStart);

void cmpProcessEnd(void) {
    struct tagCMP_WORK *wk;

    if (g_mainPID_cmpMain != NULL && dds3ExistProcess(g_mainPID_cmpMain)) {
        wk = dds3GetProcessData(g_mainPID_cmpMain);
        if (!(wk->flag & 1)) {
            wk->flag |= 1;
            wk->step = 8;
            wk->count = 0;
        }
    }
}

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/003261E8.c:1, found */
s32 cmpProcessCheck(void) {
    if (g_mainPID_cmpMain == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(g_mainPID_cmpMain) == 3 || dds3GetProcessStat(g_mainPID_cmpMain) == 0) {
        g_mainPID_cmpMain = NULL;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_00323280", cmpProcessStart_cmpMain);

INCLUDE_ASM(const s32, "code_00323280", cmpCallProcessEnd);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/003262A8.c:1, match */
s32 cmpCallGetProcessStat2(void) {
    if (effBTLFieldColorProcess == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(effBTLFieldColorProcess) == 3 || dds3GetProcessStat(effBTLFieldColorProcess) == 0) {
        effBTLFieldColorProcess = NULL;
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "code_00323280", cmpClearDispatchVisualFlagSub);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf3);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf4);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf5);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc2);

INCLUDE_ASM(const s32, "code_00323280", cmpCallGetRandIntA2);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf6);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf7);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc9);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf8);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc10);

INCLUDE_ASM(const s32, "code_00323280", cmpCallSetOt);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf9);

INCLUDE_ASM(const s32, "code_00323280", cmpCallSetOt6);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf10);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf11);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf12);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc11);

INCLUDE_ASM(const s32, "code_00323280", cmpCallSetOt7);

INCLUDE_ASM(const s32, "code_00323280", cmpCallSetOt8);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf13);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc12);

INCLUDE_ASM(const s32, "code_00323280", cmpHanafudaHanafudaTmx);

INCLUDE_ASM(const s32, "code_00323280", cmpCallTexFreeDelayed2);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf14);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf15);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf16);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc13);

INCLUDE_ASM(const s32, "code_00323280", cmpErrorCheckYaku);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf17);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc14);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc15);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf18);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc16);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc17);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc18);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc19);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc20);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf19);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc21);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc22);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf20);

INCLUDE_ASM(const s32, "code_00323280", cmpCallPrintf);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc23);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf21);

INCLUDE_ASM(const s32, "code_00323280", cmpCallGetSmallFontFRQ2);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc24);

INCLUDE_ASM(const s32, "code_00323280", cmpCallSetOt9);

void cmpDrawItemListHeader(s32 index, s32 x, s32 y)
{
    frFontHandle_t *frq = frwGetSmallFontFRQ2(x * 16, y * 8, 0xFFFFFF, 0x80808080, cmpDrawItemListHeaderData[index], NULL);

    frDrawFontOt(frq, 1, 103);
    frDestroyFontRequestAfter(frq);
}

INCLUDE_ASM(const s32, "code_00323280", cmpCallSprintf4);

INCLUDE_ASM(const s32, "code_00323280", cmpFunc25);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf22);

INCLUDE_ASM(const s32, "code_00323280", cmpCallSetOt10);

INCLUDE_ASM(const s32, "code_00323280", cmpCallSetOt11);

INCLUDE_ASM(const s32, "code_00323280", cmpCallDrawItemListHeader);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf23);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf24);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf25);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf26);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf27);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf28);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf29);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf30);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf31);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf32);

INCLUDE_ASM(const s32, "code_00323280", cmpLeaf50);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr17);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr2);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr3);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr4);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr5);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr0);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr6);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr7);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr8);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr9);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr10);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr11);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr12);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr13);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr14);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr15);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr16);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr18);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr19);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr20);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr21);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr22);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr23);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr24);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr25);

INCLUDE_RODATA(const s32, "code_00323280", cmpStr26);

INCLUDE_ASM(const s32, "code_00323280", cmpCallGetRandIntA3);

INCLUDE_ASM(const s32, "code_00323280", evtFunc21);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/00328DD0.c:1, match */
void evtLeaf35(void) {
    cmpFunc10();
}

INCLUDE_ASM(const s32, "code_00323280", evtFunc22);

INCLUDE_ASM(const s32, "code_00323280", evtFunc23);

INCLUDE_ASM(const s32, "code_00323280", evtFunc24);

INCLUDE_ASM(const s32, "code_00323280", evtTallyAddNo);

INCLUDE_ASM(const s32, "code_00323280", evtFunc25);

INCLUDE_ASM(const s32, "code_00323280", evtHand);

INCLUDE_ASM(const s32, "code_00323280", evtLeaf36);

INCLUDE_ASM(const s32, "code_00323280", evtCallPrintf);

INCLUDE_ASM(const s32, "code_00323280", evtFunc26);

INCLUDE_RODATA(const s32, "code_00323280", evtStr4);

INCLUDE_ASM(const s32, "code_00323280", evtTargetNoIndex);

INCLUDE_ASM(const s32, "code_00323280", evtCallPrintf2);

INCLUDE_RODATA(const s32, "code_00323280", evtStrPlyEnm);

INCLUDE_ASM(const s32, "code_00323280", evtTurnPhase);

INCLUDE_ASM(const s32, "code_00323280", evtFunc27);

INCLUDE_ASM(const s32, "code_00323280", evtFunc28);

INCLUDE_ASM(const s32, "code_00323280", evtFunc29);

INCLUDE_ASM(const s32, "code_00323280", evtFunc30);

INCLUDE_ASM(const s32, "code_00323280", evtClearDispatchVisualFlag);

INCLUDE_ASM(const s32, "code_00323280", evtLeaf37);

INCLUDE_ASM(const s32, "code_00323280", evtEventViewerProcess2);

INCLUDE_ASM(const s32, "code_00323280", evtCallMemFreeDirect);

INCLUDE_ASM(const s32, "code_00323280", evtLeaf38);

INCLUDE_ASM(const s32, "code_00323280", trmLeaf);

INCLUDE_ASM(const s32, "code_00323280", trmSelect);

INCLUDE_ASM(const s32, "code_00323280", trmCheckSaveAct);

INCLUDE_ASM(const s32, "code_00323280", trmCallGetProcessData);

INCLUDE_ASM(const s32, "code_00323280", trmCallGetProcessData2);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0032ABB8.c:1, match */
void trmLeaf2(void) {
    evtCallMemFreeDirect();
}

INCLUDE_ASM(const s32, "code_00323280", fclCallGetProcessData2);

void fclJpcMain(void)
{
    sdfMemHandle_t *hMem;
    fclJpcWork_t *wk;

    hMem = sdfMemAllocDirect(sizeof(fclJpcWork_t));
    wk = sdfMemGetPtr(hMem);
    memset(wk, 0, sizeof(fclJpcWork_t));
    wk->hMem = hMem;
    fclCheckData = dds3StartProcess("jpc_main", 0x3FF, 0, 0, trmCallGetProcessData, fclCallGetProcessData2, wk);
    wk->drawPID = dds3StartProcess("jpc_draw", 0x2B07, 0, 0, trmCallGetProcessData2, NULL, wk);
    dds3BackColor = 0x80404040;
    evtClearDispatchVisualFlag();
}

void fclCallExistProcess(void) {
    fclJpcWork_t *wk;

    if (fclCheckData != NULL && dds3ExistProcess(fclCheckData)) {
        wk = dds3GetProcessData(fclCheckData);
        if (!(wk->flag & 1)) {
            wk->flag |= 1;
            wk->step = 1;
        }
    }
}

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0032AD40.c:1, match */
s32 fclCheck(void) {
    if (fclCheckData == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(fclCheckData) == 3 || dds3GetProcessStat(fclCheckData) == 0) {
        fclCheckData = NULL;
        return 0;
    }
    return 1;
}

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0032AD90.c:1, match */
void fclLeaf(void) {
    fclJpcMain();
}

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0032ADA8.c:1, match */
void fclLeaf2(void) {
    fclCallExistProcess();
}

INCLUDE_ASM(const s32, "code_00323280", fclCallCheck);

INCLUDE_RODATA(const s32, "code_00323280", nwdStrShopNewsNews01201Spr);

INCLUDE_RODATA(const s32, "code_00323280", nwdStrShopNewsNews01101Spr);

INCLUDE_RODATA(const s32, "code_00323280", nwdStrShopNewsNews01001Spr);

INCLUDE_RODATA(const s32, "code_00323280", nwdStrShopNewsNews00901Spr);

INCLUDE_RODATA(const s32, "code_00323280", nwdStrShopNewsNews00801Spr);

INCLUDE_RODATA(const s32, "code_00323280", nwdStrShopNewsNews00701Spr);

INCLUDE_RODATA(const s32, "code_00323280", nwdLoadSpriteDataEntry);

INCLUDE_RODATA(const s32, "code_00323280", sbtlStr0601Spr);

INCLUDE_RODATA(const s32, "code_00323280", nwdStrShopNewsNews00501Spr);

INCLUDE_RODATA(const s32, "code_00323280", nwdStrShopNewsNews00401Spr);

INCLUDE_RODATA(const s32, "code_00323280", nwdStrShopNewsNews00301Spr);

INCLUDE_RODATA(const s32, "code_00323280", nwdStrShopNewsNews00201Spr);

INCLUDE_RODATA(const s32, "code_00323280", nwdStrShopNewsNews00101Spr);

INCLUDE_ASM(const s32, "code_00323280", Process_nwdNews);

INCLUDE_ASM(const s32, "code_00323280", DrawProcess_nwdNews);

INCLUDE_ASM(const s32, "code_00323280", GetScene);

INCLUDE_ASM(const s32, "code_00323280", nwdLoadSprite);

INCLUDE_ASM(const s32, "code_00323280", nwdCallNext);

INCLUDE_ASM(const s32, "code_00323280", Release_nwdNews);

void Shutdown_nwdNews(dds3ProcessID_t *id) {
    struct tagNWDNEWS_WORK *wk = dds3GetProcessData(id);

    Release_nwdNews(wk);
    if (dds3ExistProcess(wk->drawPID)) {
        dds3KillProcess(wk->drawPID, 0);
        wk->drawPID = NULL;
    }
    sdfMemFreeDirect(wk->hMem);
    g_mainPID_nwdNews = NULL;
}

void nwdProcessStart(void)
{
    sdfMemHandle_t *hMem;
    struct tagNWDNEWS_WORK *wk;

    hMem = sdfMemAllocDirect(sizeof(struct tagNWDNEWS_WORK));
    wk = sdfMemGetPtr(hMem);
    memset(wk, 0, sizeof(struct tagNWDNEWS_WORK));
    wk->hMem = hMem;
    g_mainPID_nwdNews = dds3StartProcess("nwdnews_main", 0x3FF, 0, 0, Process_nwdNews, (dds3ProcessFunc_t)Shutdown_nwdNews, wk);
    wk->drawPID = dds3StartProcess("nwdnews_draw", 0x2B07, 0, 0, DrawProcess_nwdNews, NULL, wk);
}

void nwdProcessEnd(void) {
    struct tagNWDNEWS_WORK *wk;

    if (g_mainPID_nwdNews != NULL && dds3ExistProcess(g_mainPID_nwdNews)) {
        wk = dds3GetProcessData(g_mainPID_nwdNews);
        if (!(wk->flag & 1)) {
            wk->flag |= 1;
            wk->step = 5;
            wk->count = 0;
        }
    }
}

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0032B520.c:1, match */
s32 nwdProcessCheck(void) {
    if (g_mainPID_nwdNews == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(g_mainPID_nwdNews) == 3 || dds3GetProcessStat(g_mainPID_nwdNews) == 0) {
        g_mainPID_nwdNews = NULL;
        return 0;
    }
    return 1;
}

/* body: matched in raidou @ 61f09dc: src/shared/code_r1_002F93D8.c:15, match, same function 1.00 (words 19/19, calls 1.00, shape 1.00) */
void shpCreateScene(void)
{
    dds3Basic_t *scene = dds3AddScene();

    dds3SetControlScene(scene);
    dds3SetActiveScene(scene);
    dds3AddAreaBasic(dds3AllocObjectId(), "shop_area");
}

/* body: matched in raidou @ 61f09dc: src/shared/event/evtStage.c:194, match, same function 1.00 (words 13/13, calls 1.00, shape 1.00) */
void shpDeleteScene(void)
{
    dds3Basic_t *node;

    node = (dds3Basic_t *)dds3GetControlScene();
    if (node != 0) {
        dds3DeleteScene(node);
    }
}

INCLUDE_ASM(const s32, "code_00323280", shpMakeLightPacket);

INCLUDE_ASM(const s32, "code_00323280", SetLight);

INCLUDE_ASM(const s32, "code_00323280", shpSetCamera);

INCLUDE_RODATA(const s32, "code_00323280", shpMakeLightPacketTable);

INCLUDE_RODATA(const s32, "code_00323280", shpMakeLightPacketTable2);

INCLUDE_RODATA(const s32, "code_00323280", shpMakeLightPacketTable3);

INCLUDE_RODATA(const s32, "code_00323280", SetLightTable);

INCLUDE_RODATA(const s32, "code_00323280", SetLightTable2);

INCLUDE_RODATA(const s32, "code_00323280", SetLightTable3);

INCLUDE_RODATA(const s32, "code_00323280", SetLightTable4);

INCLUDE_RODATA(const s32, "code_00323280", SetLightTable5);

INCLUDE_RODATA(const s32, "code_00323280", SetLightTable6);

INCLUDE_ASM(const s32, "code_00323280", shpSetCamera_shpModel);

INCLUDE_ASM(const s32, "code_00323280", shpGetCameraPos);

INCLUDE_ASM(const s32, "code_00323280", shpAddCameraPos);

INCLUDE_ASM(const s32, "code_00323280", Reset3DGlobal);

INCLUDE_ASM(const s32, "code_00323280", stfResetUI);

INCLUDE_ASM(const s32, "code_00323280", shpSetMove3D);

INCLUDE_ASM(const s32, "code_00323280", shpSetMove3DDef);

INCLUDE_ASM(const s32, "code_00323280", ChkMotionEnd);

INCLUDE_ASM(const s32, "code_00323280", shpUpdateCamera);

INCLUDE_ASM(const s32, "code_00323280", shpDeleteModel);

INCLUDE_ASM(const s32, "code_00323280", CreateModel);

INCLUDE_ASM(const s32, "code_00323280", shpSetModel);

INCLUDE_ASM(const s32, "code_00323280", shpSetMotion);

INCLUDE_ASM(const s32, "code_00323280", shpSetMotionSpeed);

INCLUDE_ASM(const s32, "code_00323280", shpSetSingleMotion);

INCLUDE_ASM(const s32, "code_00323280", shpSetSmoothLoopMotion);

INCLUDE_ASM(const s32, "code_00323280", shpSetSmoothSingleMotion);

INCLUDE_ASM(const s32, "code_00323280", shpSetSwitchMotion);

INCLUDE_ASM(const s32, "code_00323280", shpSetMotionPause);

INCLUDE_ASM(const s32, "code_00323280", shpSetPosModel);

INCLUDE_ASM(const s32, "code_00323280", shpSetRotModel);

INCLUDE_ASM(const s32, "code_00323280", shpSetRotYModel);

INCLUDE_ASM(const s32, "code_00323280", shpSetSclModel);

INCLUDE_ASM(const s32, "code_00323280", shpSetSclModelEx);

INCLUDE_ASM(const s32, "code_00323280", shpSetDrwModel);

INCLUDE_ASM(const s32, "code_00323280", shpSetDrwModelAll);

INCLUDE_ASM(const s32, "code_00323280", shpSetColor);

INCLUDE_ASM(const s32, "code_00323280", shpSetParamInit);

INCLUDE_ASM(const s32, "code_00323280", shpSetTrans);

INCLUDE_ASM(const s32, "code_00323280", shpSetDebugPrint);

INCLUDE_ASM(const s32, "code_00323280", shpDrawFieldLine);

INCLUDE_ASM(const s32, "code_00323280", shpGetBasic);

INCLUDE_ASM(const s32, "code_00323280", shpGetExist);

INCLUDE_ASM(const s32, "code_00323280", Init_shpModel);

INCLUDE_ASM(const s32, "code_00323280", camp_update_status_Shutdown);

sdfTexHandle_t *campCallDevLoadFileEx(const char *filename)
{
    u32 resource = 0;
    sdfMemHandle_t *fileAllocation = sdfDevLoadFileEx(filename, &resource, 0);
    sdfTexHandle_t *texture = sdfTexSet(EE_ADDR_AS(TMX_HEADER_t *, resource));

    sdfMemFreeDelayedDirect(fileAllocation);
    return texture;
}

INCLUDE_ASM(const s32, "code_00323280", campCallSetOt);

void campRefShpDraw(s32 a_x, s32 a_y, s32 a_sprNo)
{
    itfDrawMiscSprite(a_x, a_y, 256, wk_shpDraw->hSpr, a_sprNo, 0x67);
}

INCLUDE_ASM(const s32, "code_00323280", scrLeaf4);

INCLUDE_ASM(const s32, "code_00323280", scrRefShpDraw4);

INCLUDE_ASM(const s32, "code_00323280", scrRefShpDraw5);

void scrClearProfileFlagsTable(s32 a_x, s32 a_y, u32 a_alphaRate, itfSprList_t *a_list)
{
    itfDrawMiscSpriteList(a_x, a_y, 256, a_alphaRate, wk_shpDraw->hSpr, 0x67, a_list);
}

INCLUDE_ASM(const s32, "code_00323280", scrCallGetSmallFontFRQ2);

INCLUDE_ASM(const s32, "code_00323280", scrPartsListDatSub);

INCLUDE_ASM(const s32, "code_00323280", scrRefDDS3GBWK4);

INCLUDE_ASM(const s32, "code_00323280", scrCallGetFigure);

INCLUDE_ASM(const s32, "code_00323280", scrCallSprintf2);

INCLUDE_ASM(const s32, "code_00323280", ScrollBarCursor);

INCLUDE_ASM(const s32, "code_00323280", scrCallClearProfileFlagsTable2);

INCLUDE_ASM(const s32, "code_00323280", scrCallClearProfileFlagsTable3);

INCLUDE_ASM(const s32, "code_00323280", PartsMoney);

INCLUDE_ASM(const s32, "code_00323280", PartsMoney2);

INCLUDE_ASM(const s32, "code_00323280", shpCallClearProfileFlagsTable);

INCLUDE_ASM(const s32, "code_00323280", Select2);

INCLUDE_ASM(const s32, "code_00323280", PartsListDat);

INCLUDE_ASM(const s32, "code_00323280", shpCallSetOt);

INCLUDE_ASM(const s32, "code_00323280", ScrollBar);

INCLUDE_ASM(const s32, "code_00323280", PartsList);

INCLUDE_ASM(const s32, "code_00323280", shpItemListTest);

INCLUDE_ASM(const s32, "code_00323280", knyDrawKeyHelp);

INCLUDE_ASM(const s32, "code_00323280", nwdDrawKeyHelp);

INCLUDE_ASM(const s32, "code_00323280", shpRootCursor);

INCLUDE_ASM(const s32, "code_00323280", shpRootCursorInit);

INCLUDE_ASM(const s32, "code_00323280", shpResetRootLine);

INCLUDE_ASM(const s32, "code_00323280", shpCheckRootLineMax);

INCLUDE_ASM(const s32, "code_00323280", shpCursorLine);

INCLUDE_ASM(const s32, "code_00323280", shpRootLine);

INCLUDE_ASM(const s32, "code_00323280", shpRootCursorSelect);

INCLUDE_ASM(const s32, "code_00323280", shpBackSpr);

INCLUDE_ASM(const s32, "code_00323280", shpCommandHelp);

INCLUDE_ASM(const s32, "code_00323280", shpRootMenuTitle);

INCLUDE_RODATA(const s32, "code_00323280", shpSetRotYModelTable);

INCLUDE_ASM(const s32, "code_00323280", shpShopKonnouyaTmx);

INCLUDE_ASM(const s32, "code_00323280", nbRefShpDraw);

INCLUDE_RODATA(const s32, "code_00323280", knyStrShopScriptNwdEvtBf);

INCLUDE_RODATA(const s32, "code_00323280", shpStrShopScriptKnyEvtBf);

