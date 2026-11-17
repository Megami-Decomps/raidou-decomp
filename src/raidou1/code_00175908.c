#include "common.h"
#include "fld.h"
#include "fldProcess.h"
#include "mapData.h"
#include "ctn.h"
#include "neko.h"
#include "lmap.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3ObjectBase.h"
#include "code_r1_00128378.h"
#include "evtUnitManager.h"

INCLUDE_ASM(const s32, "code_00175908", mapDrinkClearMsg);

INCLUDE_ASM(const s32, "code_00175908", mapMoudokuClearMsg);

INCLUDE_ASM(const s32, "code_00175908", mapRiberaMaClearMsg);

INCLUDE_ASM(const s32, "code_00175908", mapRiberaMaClear);

INCLUDE_ASM(const s32, "code_00175908", mapRiberaMaStart);

INCLUDE_ASM(const s32, "code_00175908", mapRefDDS3GBWK10);

INCLUDE_ASM(const s32, "code_00175908", mapEsutoMaClearMsg);

INCLUDE_ASM(const s32, "code_00175908", mapEsutoMaClear);

INCLUDE_ASM(const s32, "code_00175908", mapEsutoMaStart);

INCLUDE_ASM(const s32, "code_00175908", mapRefDDS3GBWK11);

INCLUDE_ASM(const s32, "code_00175908", mapRiberaMa2ClearMsg);

INCLUDE_ASM(const s32, "code_00175908", mapRiberaMa2Clear);

INCLUDE_ASM(const s32, "code_00175908", mapRiberaMa2Start);

INCLUDE_ASM(const s32, "code_00175908", mapRefDDS3GBWK12);

INCLUDE_ASM(const s32, "code_00175908", mapEsutoMa2ClearMsg);

INCLUDE_ASM(const s32, "code_00175908", mapEsutoMa2Clear);

INCLUDE_ASM(const s32, "code_00175908", mapEsutoMa2Start);

INCLUDE_ASM(const s32, "code_00175908", mapRefDDS3GBWK13);

INCLUDE_ASM(const s32, "code_00175908", mapRefDDS3GBWK14);

INCLUDE_RODATA(const s32, "code_00175908", mapStrPlNlRnd10);

INCLUDE_ASM(const s32, "code_00175908", mapPlNlRnd);

INCLUDE_ASM(const s32, "code_00175908", mapCallBitCheck4);

INCLUDE_RODATA(const s32, "code_00175908", mapStrIdx);

INCLUDE_ASM(const s32, "code_00175908", mapPlNlRnd2);

INCLUDE_ASM(const s32, "code_00175908", mapCallBitCheck5);

INCLUDE_ASM(const s32, "code_00175908", mapGetSkilTakumiMapCheck);

INCLUDE_ASM(const s32, "code_00175908", mapCtnMdlLoadPreSetSub);

INCLUDE_ASM(const s32, "code_00175908", mapBCheckPanelModeSub);

INCLUDE_ASM(const s32, "code_00175908", mapBCheckPanelModeSub2);

INCLUDE_ASM(const s32, "code_00175908", mapCtnColorFadeTask);

INCLUDE_ASM(const s32, "code_00175908", mapCtnModelMake);

INCLUDE_ASM(const s32, "code_00175908", sbtlCheckModelDataLoadEnd);

INCLUDE_ASM(const s32, "code_00175908", mapCtnMdlLoad_PreSet);

INCLUDE_ASM(const s32, "code_00175908", mapCallBtlCheckModelDataLoadEnd);

INCLUDE_ASM(const s32, "code_00175908", mapCtnMdlLoad);

INCLUDE_ASM(const s32, "code_00175908", mapCtnMdlTypeRand);

INCLUDE_ASM(const s32, "code_00175908", mapCtnAndMdlLinkOn);

INCLUDE_ASM(const s32, "code_00175908", mapCtnGetSub);

INCLUDE_ASM(const s32, "code_00175908", mapCtnAndMdlLinkOff);

INCLUDE_ASM(const s32, "code_00175908", mapCtnAndMdlLinkOff2);

INCLUDE_ASM(const s32, "code_00175908", mapCtnAndMdlLinkGet);

INCLUDE_ASM(const s32, "code_00175908", mapCtnSyncFast);

INCLUDE_ASM(const s32, "code_00175908", mapCtnLoad);

INCLUDE_ASM(const s32, "code_00175908", mapCallLoadDataIDFloor);

INCLUDE_ASM(const s32, "code_00175908", mapCallCtnMdlLoad);

INCLUDE_ASM(const s32, "code_00175908", mapCtnSync);

void mapCtnFree(void) {
    if (mapCtnLoadFlag) {
        mapData149 = 0;
        ctnMemFree();
        if (mapCtnFreeData != 0.0f) {
            nekoCallMemFreeDelayedDirect();
            mapCtnFreeData = 0.0f;
        }
    }
    mapCtnLoadFlag = 0;
}

INCLUDE_ASM(const s32, "code_00175908", mapCallMemset3);

INCLUDE_ASM(const s32, "code_00175908", mapNakamaSummonTaskSub);

INCLUDE_ASM(const s32, "code_00175908", mapCtnGet);

INCLUDE_ASM(const s32, "code_00175908", mapCtnViewOff);

INCLUDE_ASM(const s32, "code_00175908", mapCtnGetObjNum);

INCLUDE_ASM(const s32, "code_00175908", mapCallSetIntReturnValue);

INCLUDE_ASM(const s32, "code_00175908", mapCallSetIntReturnValue2);

s32 fldCallReleaseUnitPad(void) {
    dds3Basic_t *object;

    fldReleaseUnitPad();
    object = dds3GetActiveUnitSceneBasic(dds3GetActiveScene());
    if (object == NULL) {
        return 1;
    }
    Start_lmap(0);
    return 1;
}

s32 fldCommand_PAD_ENABLE(void) {
    dds3Basic_t *object;

    object = dds3GetActiveUnitSceneBasic(dds3GetActiveScene());
    if (object == NULL) {
        return 1;
    }
    dds3ObjectBaseSetPrimaryControl(object, NULL);
    fldPlayerMotReset();
    fldRegistUnitPad();
    Start_lmap(1);
    mapEffEplAllEnd();
    evtSetUnitStat(evtGetUnitHandle2(playerBasic), 0);
    return 1;
}

INCLUDE_ASM(const s32, "code_00175908", itfSetMesDispPriority);

INCLUDE_ASM(const s32, "code_00175908", itfCallGetControlScene);

INCLUDE_RODATA(const s32, "code_00175908", mapCtnGetTable);

