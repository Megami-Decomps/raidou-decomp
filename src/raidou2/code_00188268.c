#include "common.h"
#include "mapData.h"
#include "ctn.h"
#include "neko.h"
#include "fld.h"
#include "fldProcess.h"
#include "lmap.h"
#include "dds3WorldBasic.h"
#include "dds3SceneBasic.h"
#include "dds3ObjectBase.h"
#include "code_r1_00128378.h"
#include "evtUnitManager.h"

INCLUDE_ASM(const s32, "code_00188268", mapDrinkClearMsg);

INCLUDE_ASM(const s32, "code_00188268", mapMoudokuClearMsg);

INCLUDE_ASM(const s32, "code_00188268", mapRiberaMaClearMsg);

INCLUDE_ASM(const s32, "code_00188268", mapRiberaMaClear);

INCLUDE_ASM(const s32, "code_00188268", mapRiberaMaStart);

INCLUDE_ASM(const s32, "code_00188268", mapRefDDS3GBWK10);

INCLUDE_ASM(const s32, "code_00188268", mapEsutoMaClearMsg);

INCLUDE_ASM(const s32, "code_00188268", mapEsutoMaClear);

INCLUDE_ASM(const s32, "code_00188268", mapEsutoMaStart);

INCLUDE_ASM(const s32, "code_00188268", mapRefDDS3GBWK11);

INCLUDE_ASM(const s32, "code_00188268", mapRiberaMa2ClearMsg);

INCLUDE_ASM(const s32, "code_00188268", mapRiberaMa2Clear);

INCLUDE_ASM(const s32, "code_00188268", mapRiberaMa2Start);

INCLUDE_ASM(const s32, "code_00188268", mapRefDDS3GBWK12);

INCLUDE_ASM(const s32, "code_00188268", mapEsutoMa2ClearMsg);

INCLUDE_ASM(const s32, "code_00188268", mapEsutoMa2Clear);

INCLUDE_ASM(const s32, "code_00188268", mapEsutoMa2Start);

INCLUDE_ASM(const s32, "code_00188268", mapRefDDS3GBWK8);

INCLUDE_ASM(const s32, "code_00188268", mapRefDDS3GBWK9);

INCLUDE_ASM(const s32, "code_00188268", mapGetSkilTakaraSagasiCheck);

INCLUDE_ASM(const s32, "code_00188268", mapCallBitCheck9);

INCLUDE_ASM(const s32, "code_00188268", mapGetSkilZeniSagasiCheck);

INCLUDE_ASM(const s32, "code_00188268", mapPlNlRnd);

INCLUDE_ASM(const s32, "code_00188268", mapCallBitCheck10);

INCLUDE_ASM(const s32, "code_00188268", mapCallBitCheck11);

INCLUDE_ASM(const s32, "code_00188268", mapCtnMdlLoadPreSetSub);

INCLUDE_ASM(const s32, "code_00188268", mapBCheckPanelModeSub);

INCLUDE_ASM(const s32, "code_00188268", mapBCheckPanelModeSub2);

INCLUDE_ASM(const s32, "code_00188268", mapCtnColorFadeTask);

INCLUDE_ASM(const s32, "code_00188268", mapCtnModelMake);

INCLUDE_RODATA(const s32, "code_00188268", mapStrKouhoLackYen2);

INCLUDE_RODATA(const s32, "code_00188268", mapStrIdx);

INCLUDE_ASM(const s32, "code_00188268", sbtlCheckModelDataLoadEnd);

INCLUDE_ASM(const s32, "code_00188268", mapCtnMdlLoad_PreSet);

INCLUDE_ASM(const s32, "code_00188268", mapCallBtlCheckModelDataLoadEnd);

INCLUDE_ASM(const s32, "code_00188268", mapCtnMdlLoad);

INCLUDE_ASM(const s32, "code_00188268", mapCtnMdlTypeRand);

INCLUDE_ASM(const s32, "code_00188268", mapCtnAndMdlLinkOn);

INCLUDE_ASM(const s32, "code_00188268", mapCtnGetSub);

INCLUDE_ASM(const s32, "code_00188268", mapCtnAndMdlLinkOff);

INCLUDE_ASM(const s32, "code_00188268", mapCtnAndMdlLinkOff2);

INCLUDE_ASM(const s32, "code_00188268", mapCtnAndMdlLinkGet);

INCLUDE_ASM(const s32, "code_00188268", mapCtnSyncFast);

INCLUDE_ASM(const s32, "code_00188268", mapCtnLoad);

INCLUDE_ASM(const s32, "code_00188268", mapCallLoadDataIDFloor);

INCLUDE_ASM(const s32, "code_00188268", mapCallCtnMdlLoad);

INCLUDE_ASM(const s32, "code_00188268", mapCtnSync);

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

INCLUDE_ASM(const s32, "code_00188268", mapCtnCamInChk);

INCLUDE_ASM(const s32, "code_00188268", mapNakamaSummonTaskSub);

INCLUDE_ASM(const s32, "code_00188268", mapCtnGet);

INCLUDE_ASM(const s32, "code_00188268", mapCtnViewOff);

INCLUDE_ASM(const s32, "code_00188268", mapCtnGetObjNum);

INCLUDE_RODATA(const s32, "code_00188268", mapCtnGetTable);

INCLUDE_ASM(const s32, "code_00188268", mapCallBitOff2);

INCLUDE_ASM(const s32, "code_00188268", mapCallSetIntReturnValue);

INCLUDE_ASM(const s32, "code_00188268", mapCallSetIntReturnValue2);

s32 fldCallReleaseUnitPad(void) {
    dds3Basic_t *object;

    fldReleaseUnitPad();
    mapCallBitOff2();
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

INCLUDE_ASM(const s32, "code_00188268", itfSetMesDispPriority);

INCLUDE_ASM(const s32, "code_00188268", itfCallGetControlScene);
