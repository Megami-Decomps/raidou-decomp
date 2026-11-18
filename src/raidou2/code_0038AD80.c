#include "common.h"
#include "dds3KernelCore.h"
#include "sdfMemory.h"
#include "fldProcess.h"
#include "cmpFile.h"
#include "code_00323280.h"
#include "code_0038AD80.h"
#include "cmpMain.h"
#include "code_00300578.h"
#include "code_r1_00323138.h"
#include "cmpMap.h"
#include "sdk_libc.h"
#include "code_0016A7D8.h"

INCLUDE_ASM(const s32, "code_0038AD80", DrawTop);

s32 DrawProcess(dds3ProcessID_t *PID) {
    tagCMP_MAP_WORK *wk = dds3GetProcessData(PID);

    if (!(wk->flag & 8)) {
        return 0;
    }
    cmpDrawBack(wk);
    cmpDrawBackPattern(wk);
    mapAutoMapDraw3();
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
    g_mainPID_cmpMap = dds3StartProcess(ProcessStart_cmpMapStrCampAutomapMain, 1007, 0, 0, Process_cmpMap,
                                        (dds3ProcessFunc_t)Shutdown_cmpMap, wk);
    wk->drawPID = dds3StartProcess(ProcessStart_cmpMapStrCampAutomapDraw, 11012, 0, 0, DrawProcess, NULL, wk);
    if (cmpGetSpriteHnd(CMP_SPR_MAP) == NULL) {
        if (cmpLoadSpriteHnd(CMP_SPR_MAP) == NULL) {
            cmpLoadSpriteMem(CMP_SPR_MAP);
            wk->flag |= 2;
        }
        cmpSetupCmpItemSub(CMP_SPR_MAP);
        wk->flag |= 4;
    }
    fldMapAllAmap();
}

INCLUDE_ASM(const s32, "code_0038AD80", ProcessEnd_cmpMap);

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

struct tagCMP_WORK *cmpRefMainPIDCmpMain(void) {
    return dds3GetProcessData(g_mainPID_cmpMain);
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpFunc7);

dds3ProcessID_t *cmpPartyDrawFuncSub(void)
{
    return cmpRefMainPIDCmpMain()->partyPID;
}

dds3ProcessID_t *cmpGetCharPID(void)
{
    return cmpRefMainPIDCmpMain()->charPID;
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpGetsubMenuPID);

struct tagCMP_WORK *cmpGetStockPID(void) {
    return dds3GetProcessData(g_mainPID_cmpMain);
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpUpdateSkillHDCmpKudaSub);

INCLUDE_ASM(const s32, "code_0038AD80", cmpUpdateSkillHDCmpKudaSub2);

INCLUDE_ASM(const s32, "code_0038AD80", cmpUpdateSkillHDCmpKudaSub3);

INCLUDE_ASM(const s32, "code_0038AD80", cmpUpdateSkillHDCmpKudaSub4);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawPartsOmoideSkillSub3);

INCLUDE_ASM(const s32, "code_0038AD80", cmpChkTabChg);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallMenuSetFlagMenu);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallMenuGetSelectID3);

INCLUDE_ASM(const s32, "code_0038AD80", ChkEnableSelectMenuBook);

INCLUDE_ASM(const s32, "code_0038AD80", cmpChkEnableSelectMenuItem);

INCLUDE_ASM(const s32, "code_0038AD80", cmpProcess);

INCLUDE_ASM(const s32, "code_0038AD80", cmpChkEnableSelectMenu);

INCLUDE_ASM(const s32, "code_0038AD80", cmpGetstrCmpMenuHD);

INCLUDE_ASM(const s32, "code_0038AD80", cmpSetMenu);

INCLUDE_ASM(const s32, "code_0038AD80", Init_cmpMain);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallGetProcessData25);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawKeyHelp2);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawHelpText);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawKeyReturnHelp);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawKeyHelp);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawKeyHelp2_cmpMain);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawKeyHelp3);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallGetSpriteHnd);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawMenuFunc);

INCLUDE_ASM(const s32, "code_0038AD80", cmpGetEvtItemSprNo);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallGetSpriteHnd2);

INCLUDE_ASM(const s32, "code_0038AD80", cmpLeaf49);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallGetSpriteHnd9);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallGetSpriteHnd10);

INCLUDE_ASM(const s32, "code_0038AD80", cmpFunc8);

INCLUDE_ASM(const s32, "code_0038AD80", cmpStatusDrawSub);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallGetSpriteHnd11);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawRaidouSkill);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallSetOt3);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallSetOt4);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallSetOt5);

INCLUDE_ASM(const s32, "code_0038AD80", CmpStatusDraw);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawProcess);

INCLUDE_ASM(const s32, "code_0038AD80", cmpDrawProcess_cmpMain);

CMP_MODE cmpGetEndMode(void)
{
    return cmpCallFuncData;
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpSetMarkID);

s32 cmpCallGetProcessData26(void)
{
    struct tagCMP_WORK *wk = dds3GetProcessData(g_mainPID_cmpMain);

    return wk->count;
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallProcEnd);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCheckProcEnd);

void cmpRelease(struct tagCMP_WORK *wk) {
    End_cmpItem();
    cmpFreeSpriteToGS();
}

INCLUDE_ASM(const s32, "code_0038AD80", cmpProcessStartEx);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCampMain);

INCLUDE_ASM(const s32, "code_0038AD80", cmpProcessStart);

INCLUDE_ASM(const s32, "code_0038AD80", cmpProcessEnd);

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

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallAddDevil);

INCLUDE_ASM(const s32, "code_0038AD80", cmpCallProcessEnd);

INCLUDE_ASM(const s32, "code_0038AD80", fclCheck);

INCLUDE_ASM(const s32, "code_0038AD80", Process_nwdNews);

INCLUDE_ASM(const s32, "code_0038AD80", DrawProcess_nwdNews);

INCLUDE_ASM(const s32, "code_0038AD80", nwdLoadSpriteSub);

INCLUDE_ASM(const s32, "code_0038AD80", nwdLoadSprite);

INCLUDE_ASM(const s32, "code_0038AD80", nwdCallNext);

INCLUDE_ASM(const s32, "code_0038AD80", Release_nwdNews);

INCLUDE_ASM(const s32, "code_0038AD80", Shutdown_nwdNews);

INCLUDE_ASM(const s32, "code_0038AD80", nwdProcessStart);

INCLUDE_ASM(const s32, "code_0038AD80", nwdProcessEnd);

INCLUDE_ASM(const s32, "code_0038AD80", nwdProcessCheck);

INCLUDE_ASM(const s32, "code_0038AD80", shpCreateScene);

INCLUDE_ASM(const s32, "code_0038AD80", shpDeleteScene);

INCLUDE_ASM(const s32, "code_0038AD80", shpMakeLightPacket);

INCLUDE_ASM(const s32, "code_0038AD80", SetLight);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetCamera);

INCLUDE_ASM(const s32, "code_0038AD80", shpShopCamera);

INCLUDE_ASM(const s32, "code_0038AD80", Reset3DGlobal);

INCLUDE_ASM(const s32, "code_0038AD80", stfResetUI);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetMove3D);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetMove3DDef);

INCLUDE_ASM(const s32, "code_0038AD80", ChkMotionEnd);

INCLUDE_ASM(const s32, "code_0038AD80", shpUpdateCamera);

INCLUDE_ASM(const s32, "code_0038AD80", shpDeleteModel);

INCLUDE_ASM(const s32, "code_0038AD80", CreateModel);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetModel);

INCLUDE_ASM(const s32, "code_0038AD80", addNpcHukidashi);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetMotion);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetMotionSpeed);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetSingleMotion);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetSmoothLoopMotion);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetSmoothSingleMotion);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetSwitchMotion);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetMotionPause);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetPosModel);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetRotModel);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetRotYModel);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetSclModel);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetSclModelEx);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetDrwModel);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetDrwModelAll);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetColor);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetParamInit);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetTrans);

INCLUDE_ASM(const s32, "code_0038AD80", shpSetDebugPrint);

INCLUDE_ASM(const s32, "code_0038AD80", shpDrawFieldLine);

INCLUDE_ASM(const s32, "code_0038AD80", shpGetBasic);

INCLUDE_ASM(const s32, "code_0038AD80", shpGetExist);

INCLUDE_ASM(const s32, "code_0038AD80", shpCallMemAllocDirect);

INCLUDE_ASM(const s32, "code_0038AD80", shpCallDeleteScene);

INCLUDE_ASM(const s32, "code_0038AD80", campCallDevLoadFileEx);

INCLUDE_ASM(const s32, "code_0038AD80", campCallSetOt);

INCLUDE_ASM(const s32, "code_0038AD80", campRefShpDraw);

INCLUDE_ASM(const s32, "code_0038AD80", scrLeaf4);

INCLUDE_ASM(const s32, "code_0038AD80", scrRefShpDraw);

INCLUDE_ASM(const s32, "code_0038AD80", scrRefShpDraw4);

INCLUDE_ASM(const s32, "code_0038AD80", scrRefShpDraw5);

INCLUDE_ASM(const s32, "code_0038AD80", scrRefShpDraw2);

INCLUDE_ASM(const s32, "code_0038AD80", scrRefShpDraw3);

INCLUDE_ASM(const s32, "code_0038AD80", scrClearProfileFlagsTable);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallGetSmallFontFRQ2);

INCLUDE_ASM(const s32, "code_0038AD80", scrPartsListDatSub);

INCLUDE_ASM(const s32, "code_0038AD80", scrRefDDS3GBWK4);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallGetFigure);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallSprintf2);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallClearProfileFlagsTable);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallSprintf);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallClearProfileFlagsTable2);

INCLUDE_ASM(const s32, "code_0038AD80", scrCallClearProfileFlagsTable3);

INCLUDE_ASM(const s32, "code_0038AD80", shpItemListTestSub);

INCLUDE_ASM(const s32, "code_0038AD80", PartsMoney);

INCLUDE_ASM(const s32, "code_0038AD80", shpPartsListDatSub);

INCLUDE_ASM(const s32, "code_0038AD80", shpCallClearProfileFlagsTable);

INCLUDE_ASM(const s32, "code_0038AD80", Select2);

INCLUDE_ASM(const s32, "code_0038AD80", PartsListDat);

INCLUDE_ASM(const s32, "code_0038AD80", shpCallSetOt);

INCLUDE_ASM(const s32, "code_0038AD80", ScrollBar);

INCLUDE_ASM(const s32, "code_0038AD80", PartsList);

INCLUDE_ASM(const s32, "code_0038AD80", shpItemListTest);

INCLUDE_ASM(const s32, "code_0038AD80", knyDrawKeyHelp);

INCLUDE_ASM(const s32, "code_0038AD80", nwdDrawKeyHelp);

INCLUDE_ASM(const s32, "code_0038AD80", nwdCallChkMessageDraw);

INCLUDE_ASM(const s32, "code_0038AD80", shpRootCursor);

INCLUDE_ASM(const s32, "code_0038AD80", shpRootCursorInit);

INCLUDE_ASM(const s32, "code_0038AD80", shpResetRootLine);

INCLUDE_ASM(const s32, "code_0038AD80", shpCheckRootLineMax);

INCLUDE_ASM(const s32, "code_0038AD80", shpCursorLine);

INCLUDE_ASM(const s32, "code_0038AD80", shpRootLine);

INCLUDE_ASM(const s32, "code_0038AD80", shpRootCursorSelect);

INCLUDE_ASM(const s32, "code_0038AD80", shpBackSpr);

INCLUDE_ASM(const s32, "code_0038AD80", shpCommandHelp);

INCLUDE_ASM(const s32, "code_0038AD80", shpCallGetTitleAlpha);

INCLUDE_ASM(const s32, "code_0038AD80", AbLoadCallback);

INCLUDE_ASM(const s32, "code_0038AD80", fldFacilityMistInit);

INCLUDE_ASM(const s32, "code_0038AD80", fldRefShpDraw);

INCLUDE_ASM(const s32, "code_0038AD80", fldShopMilkhallTmx);

INCLUDE_ASM(const s32, "code_0038AD80", nbRefShpDraw);
