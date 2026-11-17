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
        itfCallSetOt2(242, 62, 0, 0xFF, spr, 21, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 8) {
        itfCallSetOt2(242, 375, 0, 0xFF, spr, 22, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 0x10) {
        itfCallSetOt2(26, 218, 0, 0xFF, spr, 20, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 0x20) {
        itfCallSetOt2(459, 219, 0, 0xFF, spr, 19, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 1) {
        itfIconDrawSprite(11, 17, 0, 0xFF, 53, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
        itfCallSetOt2(43, 15, 0, 0xFF, spr, 2, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    if (flg & 2) {
        itfIconDrawSprite(435, 17, 0, 0xFF, 54, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
        itfCallSetOt2(467, 15, 0, 0xFF, spr, 3, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    }
    itfIconDrawSprite(438, 419, 0, 0xFF, 13, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
    itfCallSetOt2(460, 420, 0, 0xFF, spr, 4, DDS3_OT_2D_NORMAL | DDS3_OT_BACKEX_NORMAL);
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

INCLUDE_ASM(const s32, "code_00323280", cmpGetEndMode);

INCLUDE_ASM(const s32, "code_00323280", cmpSetMarkID);

INCLUDE_ASM(const s32, "code_00323280", cmpGetMarkID);

INCLUDE_ASM(const s32, "code_00323280", cmpCallProcEnd);

INCLUDE_ASM(const s32, "code_00323280", cmpCheckProcEnd);

INCLUDE_ASM(const s32, "code_00323280", cmpRelease);

INCLUDE_ASM(const s32, "code_00323280", cmpProcessStartEx);

INCLUDE_ASM(const s32, "code_00323280", cmpProcessStartEx_cmpMain);

INCLUDE_ASM(const s32, "code_00323280", cmpProcessStart);

INCLUDE_ASM(const s32, "code_00323280", cmpProcessEnd);

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

INCLUDE_ASM(const s32, "code_00323280", cmpCallGetProcessStat2);

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

INCLUDE_ASM(const s32, "code_00323280", cmpDrawItemListHeader);

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

INCLUDE_ASM(const s32, "code_00323280", cmpCallGetRandIntA3);

INCLUDE_ASM(const s32, "code_00323280", evtFunc21);

INCLUDE_ASM(const s32, "code_00323280", evtLeaf35);

INCLUDE_ASM(const s32, "code_00323280", evtFunc22);

INCLUDE_ASM(const s32, "code_00323280", evtFunc23);

INCLUDE_ASM(const s32, "code_00323280", evtFunc24);

INCLUDE_ASM(const s32, "code_00323280", evtTallyAddNo);

INCLUDE_ASM(const s32, "code_00323280", evtFunc25);

INCLUDE_ASM(const s32, "code_00323280", evtHand);

INCLUDE_ASM(const s32, "code_00323280", evtLeaf36);

INCLUDE_ASM(const s32, "code_00323280", evtCallPrintf);

INCLUDE_ASM(const s32, "code_00323280", evtFunc26);

INCLUDE_ASM(const s32, "code_00323280", evtTargetNoIndex);

INCLUDE_ASM(const s32, "code_00323280", evtCallPrintf2);

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

INCLUDE_ASM(const s32, "code_00323280", trmLeaf2);

INCLUDE_ASM(const s32, "code_00323280", fclCallGetProcessData2);

INCLUDE_ASM(const s32, "code_00323280", fclJpcMain);

INCLUDE_ASM(const s32, "code_00323280", fclCallExistProcess);

INCLUDE_ASM(const s32, "code_00323280", fclCheck);

INCLUDE_ASM(const s32, "code_00323280", fclLeaf);

INCLUDE_ASM(const s32, "code_00323280", fclLeaf2);

INCLUDE_ASM(const s32, "code_00323280", fclCallCheck);

INCLUDE_ASM(const s32, "code_00323280", Process_nwdNews);

INCLUDE_ASM(const s32, "code_00323280", DrawProcess_nwdNews);

INCLUDE_ASM(const s32, "code_00323280", GetScene);

INCLUDE_ASM(const s32, "code_00323280", nwdLoadSprite);

INCLUDE_ASM(const s32, "code_00323280", nwdCallNext);

INCLUDE_ASM(const s32, "code_00323280", Release_nwdNews);

INCLUDE_ASM(const s32, "code_00323280", Shutdown_nwdNews);

INCLUDE_ASM(const s32, "code_00323280", nwdProcessStart);

INCLUDE_ASM(const s32, "code_00323280", nwdProcessEnd);

INCLUDE_ASM(const s32, "code_00323280", nwdProcessCheck);

INCLUDE_ASM(const s32, "code_00323280", shpCreateScene);

INCLUDE_ASM(const s32, "code_00323280", shpDeleteScene);

INCLUDE_ASM(const s32, "code_00323280", shpMakeLightPacket);

INCLUDE_ASM(const s32, "code_00323280", SetLight);

INCLUDE_ASM(const s32, "code_00323280", shpSetCamera);

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

INCLUDE_ASM(const s32, "code_00323280", campCallDevLoadFileEx);

INCLUDE_ASM(const s32, "code_00323280", campCallSetOt);

INCLUDE_ASM(const s32, "code_00323280", campRefShpDraw);

INCLUDE_ASM(const s32, "code_00323280", scrLeaf4);

INCLUDE_ASM(const s32, "code_00323280", scrRefShpDraw4);

INCLUDE_ASM(const s32, "code_00323280", scrRefShpDraw5);

INCLUDE_ASM(const s32, "code_00323280", scrClearProfileFlagsTable);

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

INCLUDE_ASM(const s32, "code_00323280", shpShopKonnouyaTmx);

INCLUDE_ASM(const s32, "code_00323280", nbRefShpDraw);
