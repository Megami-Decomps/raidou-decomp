#include "common.h"
#include "dds3KernelDraw.h"
#include "fld.h"
#include "mapData.h"
#include "mapLbWork.h"
#include "sdfTex.h"

INCLUDE_ASM(const s32, "code_001431D0", mapGetScreenDir);

INCLUDE_ASM(const s32, "code_001431D0", mapCallSetOt5);

INCLUDE_ASM(const s32, "code_001431D0", mapBgDrawOnOff);

void mapDrawSBtlSceneSub(void) {
}

INCLUDE_ASM(const s32, "code_001431D0", mapSKYBGEDIT);

INCLUDE_ASM(const s32, "code_001431D0", mapBgSetScrollReset);

INCLUDE_ASM(const s32, "code_001431D0", map_fldPoint3To2);

INCLUDE_ASM(const s32, "code_001431D0", map_fldPoint3To2O);

INCLUDE_ASM(const s32, "code_001431D0", map_fldPoint3To2O2);

INCLUDE_ASM(const s32, "code_001431D0", map_fldPoint3To2O3);

void fldBgZoomTest(s32 mode) {
    if (mode == 2) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.0f, 30);
    } else if (mode == 3) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 2.0f, 30);
    } else if (mode == 4) {
        mapZoomClear();
    } else if (mode == 5) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 2.0f, 0);
    } else if (mode == 6) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.7f, 30);
    } else if (mode == 7) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 3.0f, 30);
    } else if (mode == 8) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 80.0f, fldGlobalBase.focusZ, 1.5f, 9);
    } else if (mode == 9) {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.0f, 1);
    } else if (mode == 0) {
        if (tzoom_event != 0) {
            tzoom_event = 1;
            tzoom_event_time = 30;
            tzoom_event_cnt = 30;
            mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.0f, 30);
            FAKEMATCH_ASM_LIVE(mode);
        } else {
            mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 1.0f, 30);
            FAKEMATCH_ASM_LIVE(mode);
        }
    } else {
        mapZoomSetGoal(fldGlobalBase.focusX, fldGlobalBase.focusY - 130.0f, fldGlobalBase.focusZ, 2.0f, 30);
        FAKEMATCH_ASM_LIVE(mode);
    }
}

INCLUDE_ASM(const s32, "code_001431D0", fldBgZoomTest2);

INCLUDE_ASM(const s32, "code_001431D0", mapZoomSetGoal);

void mapZoomClear(void) {
    mapData75 = 1.0f;
    mapData76 = 1.0f;
    mapZoomClearData = 0.0f;
    mapData77 = 0;
    mapData78 = 0;
    mapZoomSetGoalData11 = 0.0f;
    mapZoomSetGoalData12 = 0.0f;
    mapZoomSetGoalData13 = 512.0f;
    mapZoomSetGoalData14 = 448.0f;
    mapZoomSetGoalData15 = 0.0f;
    mapZoomSetGoalData16 = 0.0f;
    mapZoomSetGoalData17 = 0.0f;
    mapZoomSetGoalData18 = 0.0f;
    mapData79 = 512.0f;
    mapData80 = 448.0f;
    mapZoomSetGoalData19 = 0.0f;
    mapZoomSetGoalData20 = 0.0f;
    mapZoomSetGoalData21 = 0.0f;
    mapZoomSetGoalData22 = 0.0f;
    mapZoomSetGoalData23 = 512.0f;
    mapZoomSetGoalData24 = 448.0f;
    mapZoomSetGoalData25 = 0.0f;
    mapZoomSetGoalData26 = 0.0f;
    mapData81 = 0.0f;
    mapData82 = 0.0f;
    mapData83 = 0.0f;
    mapData84 = 0.0f;
    mapData85 = 0.0f;
    mapData86 = 0.0f;
    Zoom = 1.0f;
    mapData73 = 1.0f;
    mapZoomSetGoalData9 = 1.0f;
    mapZoomSetGoalData10 = 1.0f;
    mapData74 = 0.0f;
    mapBgSetScrollData4 = 0;
    tzoom_event = 0;
    mapZoomSetGoalData = mapData59 = mapZoomSetGoalData6 = mapZoomClearData;
    mapZoomSetGoalData2 = mapZoomSetGoalData4 = mapZoomSetGoalData7 = mapZoomClearData;
    mapZoomSetGoalData3 = mapZoomSetGoalData5 = mapZoomSetGoalData8 = mapZoomClearData;
    mapZoomSetGoalData27 = mapZoomClearData;
    mapZoomSetGoalData28 = mapZoomClearData;
    mapZoomSetGoalData29 = mapZoomClearData;
    mapZoomSetGoalData30 = mapZoomClearData;
    mapData87 = mapZoomClearData;
    mapData88 = mapZoomClearData;
    mapBgSetScrollData7 = mapZoomClearData;
    mapBgSetScrollData8 = mapZoomClearData;
    mapZoomSetGoalData31 = mapZoomClearData;
    mapZoomSetGoalData32 = mapZoomClearData;
    mapData89 = mapZoomClearData;
    mapData90 = mapZoomClearData;
    mapBgSetScrollData9 = mapZoomClearData;
    mapBgSetScrollData10 = mapZoomClearData;
    mapBgSetScrollData11 = mapZoomClearData;
    mapBgSetScrollData12 = mapZoomClearData;
    mapZoomSetGoalData33 = mapZoomClearData;
    mapZoomSetGoalData34 = mapZoomClearData;
    mapData91 = mapZoomClearData;
    mapData92 = mapZoomClearData;
    mapBgSetScrollData13 = mapZoomClearData;
    mapBgSetScrollData14 = mapZoomClearData;
    mapBgSetScrollData15 = mapZoomClearData;
    mapBgSetScrollData16 = mapZoomClearData;
    fscroll_mode = 0;
    mapData61 = 0;
    mapBgSetScrollData3 = 0;
    mapData62 = mapData65 = mapData68 = mapZoomClearData;
    mapData63 = mapData66 = mapData69 = mapZoomClearData;
    mapData64 = mapData67 = mapData70 = mapZoomClearData;
}

INCLUDE_ASM(const s32, "code_001431D0", mapBgFScrollOn);

INCLUDE_ASM(const s32, "code_001431D0", sndSePlay_sndAt3);

void setCngCameraDir(void) {
    mapBgSetScrollData20 = 1;
}

INCLUDE_RODATA(const s32, "code_001431D0", map_fldPoint3To2OTable);

INCLUDE_RODATA(const s32, "code_001431D0", map_fldPoint3To2O3Table);

INCLUDE_ASM(const s32, "code_001431D0", mapBgSetScroll);

INCLUDE_ASM(const s32, "code_001431D0", mapLeaf11);

void mapBgFree2(void) {
    fldCallMemFreeDelayedDirect4();
    mapCallMemFreeDirect();
    if (fldGlobalBase.word30 != NULL) {
        sdfTexFreeDelayed(fldGlobalBase.word30);
        fldGlobalBase.word44 = -1;
        fldGlobalBase.word30 = NULL;
    }
    mapBgSetScrollReset();
    dds3FilterClear();
    fldGlobalBase.hitMatterIndex = 0;
    fldGlobalBase.cutIdxPrev = 0;
    fldGlobalBase.word4C = 0;
}

INCLUDE_ASM(const s32, "code_001431D0", mapCallJmsQuestMapLoadFld);

INCLUDE_ASM(const s32, "code_001431D0", fldSpecificSceneOnVisible);

INCLUDE_ASM(const s32, "code_001431D0", mapCutSetTblID);

INCLUDE_ASM(const s32, "code_001431D0", mapCallSpecificSceneOnVisible);

INCLUDE_ASM(const s32, "code_001431D0", mapCutBgLoad);

INCLUDE_ASM(const s32, "code_001431D0", mapCutBgLoad_NB);

INCLUDE_ASM(const s32, "code_001431D0", fldAllTblPacLoad);
