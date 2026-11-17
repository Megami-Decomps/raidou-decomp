#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "basic.h"
#include "field.h"
#include "fldDbg.h"
#include "lmap.h"
#include "fldProcess.h"
#include "mapData.h"
#include "mapLbWork.h"
#include "sdfFile.h"
#include "code_r1_0021CE60.h"
#include "newdata.h"
#include "fld.h"
#include "sBtlContinue.h"
#include "fldFileResolver.h"
#include "code_r1_00128378.h"
#include "fclDrawBack.h"

void fclLeaf6(fileJobPayload_t *job) {
    fclCallMemFreeDirect(job);
}

INCLUDE_ASM(const s32, "fclDrawBack", fclCallGetProcessData);

INCLUDE_ASM(const s32, "fclDrawBack", fclProcessStart);

INCLUDE_ASM(const s32, "fclDrawBack", fclProcessEnd);

INCLUDE_ASM(const s32, "fclDrawBack", dds3DestroyCallbackNodeAfterLastNotification);

INCLUDE_ASM(const s32, "fclDrawBack", fclCallSetOt);

INCLUDE_ASM(const s32, "fclDrawBack", _BackSprite);

INCLUDE_ASM(const s32, "fclDrawBack", fclBackSprite);

INCLUDE_ASM(const s32, "fclDrawBack", fclCommandHelp);

void fldAllTblLoad(void) {
    sdfFileCmdHandle_t *file;

    file = sdfFileOpen("/map/all/MAPALL.TBL");
    sdfFileRead(file, mapData, 6400);
    sdfFileRead(file, mapData2, 3456);
    sdfFileRead(file, nbCallGetIntParaData, 4096);
    sdfFileRead(file, mapData3, 1024);
    sdfFileRead(file, p2sdData, 1536);
    sdfFileRead(file, mapData4, 5120);
    sdfFileRead(file, mapData5, 3072);
    sdfFileRead(file, fldData, 3072);
    sdfFileRead(file, mapData6, 1536);
    sdfFileRead(file, mapFileLoadData2, 48);
    sdfFileRead(file, mapFileLoadData3, 48);
    sdfFileRead(file, mapFileLoadData4, 48);
    sdfFileRead(file, fldData2, 48);
    sdfFileRead(file, mapCallSetOt2Data, 2048);
    sdfFileRead(file, mapMapObjectRefresh_fldData, 256);
    sdfFileRead(file, mapData7, 1024);
    sdfFileRead(file, mapCallBitCheckData, 4608);
    sdfFileRead(file, fldGenbaTbl, 3072);
    sdfFileRead(file, mapMapObjectRefreshData, 512);
    sdfFileRead(file, nbCallGetIntParaData4, 192);
    sdfFileRead(file, nbData, 16);
    sdfFileRead(file, mapData8, 1024);
    sdfFileRead(file, mapData9, 1024);
    sdfFileRead(file, mapData10, 1024);
    sdfFileRead(file, mapData11, 1024);
    sdfFileRead(file, mapMskEveHitTbl, 320);
    sdfFileClose(file);
}

/* NON_MATCHING: 247 of 267 words match.  */
#ifdef NON_MATCHING
void fldGbInit_System(s32 mode) {
    u32 *p;
    s32 i;

    fldGlobalBase.cutIdx = 0;
    fldGlobalBase.cutIdxPrev = 0;
    fldGlobalBase.cutState = 0;
    fldGlobalBase.playerSeTable = 0;
    fldGlobalBase.hitMatterIndex = 0;
    fldGlobalBase.word4C = 0;
    fldGlobalBase.mapCallMode = 0;
    fldGlobalBase.amSBlock = 0;
    fldGlobalBase.amFloor = 0;
    fldGlobalBase.nameU = 0;
    fldGlobalBase.menuOpenFlag = 0;
    fldGlobalBase.nameD = 0;
    fldGlobalBase.encountState = 0;
    fldGlobalBase.word80 = 0;
    fldGlobalBase.resourceFlag = 0;
    fldGlobalBase.word5C = 0;
    fldGlobalBase.word20 = 0;
    fldGlobalBase.word24 = 0;
    fldGlobalBase.noEncountCnt = 0;
    fldGlobalBase.noInpPlCnt = 0;
    fldGlobalBase.previousFloor = -1;
    fldGlobalBase.resourceArea = -1;
    fldGlobalBase.float28 = 0.0f;
    fldGlobalBase.word2C = 0;
    fldGlobalBase.lastMoveLeng = 0.0f;
    fldGlobalBase.aiboRunStop = 0;
    fldGlobalBase.focusX = 0.0f;
    fldGlobalBase.focusY = 0.0f;
    fldGlobalBase.focusZ = 0.0f;
    fldGlobalBase.floatA8 = 0.0f;
    fldGlobalBase.floatAC = 0.0f;
    fldGlobalBase.floatB0 = 0.0f;
    fldGlobalBase.floatB4 = 0.0f;
    fldGlobalBase.lightMaIdx = 0;
    fldGlobalBase.floatC4 = 180.0f;
    fldGlobalBase.camZoom = 4;
    fldGlobalBase.nowDoorSe = 16;
    fldGlobalBase.defaultEncounterId = 0;
    fldGlobalBase.floatC0 = 0.0f;
    fldGlobalBase.wordC8 = 0;
    fldGlobalBase.wordCC = 0;
    fldGlobalBase.wordD0 = 0;
    fldGlobalBase.wordD4 = 0;
    fldGlobalBase.wordD8 = 0;
    fldGlobalBase.wordDC = 0;
    fldGlobalBase.wordE0 = 0;
    fldGlobalBase.floatE4 = 0.0f;
    fldGlobalBase.camDir = 0;
    fldGlobalBase.word44 = -1;
    fldGlobalBase.wordF8 = 0;
    fldGlobalBase.z = 0.0f;
    fldGlobalBase.prevX = 0.0f;
    fldGlobalBase.btlReturnMap = 0;
    fldGlobalBase.bgZoomSet = 0;
    fldGlobalBase.hitX = 0.0f;
    fldGlobalBase.hitY = 0.0f;
    fldGlobalBase.wordFC = 0;
    fldGlobalBase.x = 0.0f;
    fldGlobalBase.y = 0.0f;
    fldGlobalBase.nowTalkModelID = 0;
    fldGlobalBase.nowTakaraTblIdx = 0;
    fldGlobalBase.nowHealTblIdx = 0;
    fldGlobalBase.word12C = -1;
    fldGlobalBase.mskIndex = 0;
    fldGlobalBase.word148 = 0;
    fldGlobalBase.word14C = 0;
    fldGlobalBase.word150 = 0;
    fldGlobalBase.word154 = -1;
    fldGlobalBase.word170 = 5;
    fldGlobalBase.word16C = 53;
    fldGlobalBase.word17C = 5;
    fldGlobalBase.word15C = 0;
    fldGlobalBase.word160 = 0;
    fldGlobalBase.word164 = 0;
    fldGlobalBase.nakamaMode = 0;
    fldGlobalBase.word178 = 0;
    fldGlobalBase.word174 = 6;
    fldGlobalBase.word180 = 6;
    fldGlobalBase.word158 = 0;
    fldGlobalBase.float184 = 0.0f;
    fldGlobalBase.float188 = 0.0f;
    fldGlobalBase.float18C = 0.0f;
    fldGlobalBase.word190 = 0;
    fldGlobalBase.word194 = 0;
    fldGlobalBase.float198 = 0.0f;
    fldGlobalBase.float19C = 0.0f;
    fldGlobalBase.float1A0 = 0.0f;
    fldGlobalBase.float1A4 = 0.0f;
    fldGlobalBase.word1A8 = 0;
    fldGlobalBase.word1AC = 0;
    fldGlobalBase.float1B0 = 0.0f;
    fldGlobalBase.float1B4 = 0.0f;
    fldGlobalBase.float1B8 = 0.0f;
    fldGlobalBase.float1BC = 0.0f;
    fldGlobalBase.word1C0 = 0;
    fldGlobalBase.word1CC = 0;
    fldGlobalBase.float1C8 = 0.0f;
    fldGlobalBase.word1D8 = 1;
    fldGlobalBase.word1D0 = 0;
    fldGlobalBase.word1E0 = 0;
    fldGlobalBase.word1DC = 0;
    fldGlobalBase.float208 = 5000.0f;
    fldGlobalBase.automapMaskCurrentIdx = -1;
    fldGlobalBase.word1E4 = 0;
    fldGlobalBase.rndEncMode = 0;
    fldGlobalBase.rndEncFieldID = 0;
    fldGlobalBase.rndEncFloorID = 0;
    fldGlobalBase.rndEncY = 0.0f;
    fldGlobalBase.rndEncX = 0.0f;
    fldGlobalBase.word1C4 = 0;
    fldGlobalBase.rndEncZ = 0.0f;
    fldGlobalBase.word204 = 0;
    fldGlobalBase.float20C = 0.0f;
    fldGlobalBase.float210 = 0.0f;
    fldGlobalBase.float214 = 0.0f;
    fldGlobalBase.word218 = -1;
    fldGlobalBase.rndEncNo = 0;
    fldGlobalBase.word21C = -1;
    fldGlobalBase.word240 = 1;
    fldGlobalBase.word78 = 0;
    fldGlobalBase.word248 = 0;
    fldGlobalBase.word24C = 0;
    fldGlobalBase.ifDrawOn = 1;
    fldGlobalBase.word2AC = 0;
    fldGlobalBase.float2B4 = 0.0f;
    fldGlobalBase.float2B8 = 0.0f;
    fldGlobalBase.word2BC = 0;
    fldGlobalBase.word2C0 = 0;
    fldGlobalBase.word2C4 = 0;
    fldGlobalBase.initScript = NULL;
    fldGlobalBase.startScript = NULL;
    fldGlobalBase.word2D0 = 0;
    fldGlobalBase.word2D4 = 0;
    fldGlobalBase.word2D8 = 0;
    fldGlobalBase.word2DC = 0;
    fldGlobalBase.word2E0 = 0;
    fldGlobalBase.word2E4 = 0;
    fldDbg.skyScale = 1.0f;
    fldGlobalBase.word2EC = 0;
    fldGlobalBase.word2E8 = 0;
    fldGlobalBase.word2F4 = 0;
    fldGlobalBase.word2F0 = 0;
    fldGlobalBase.word300 = 0;
    fldGlobalBase.nakamaTaskOff = 0;
    fldGlobalBase.word308 = 0;
    fldGlobalBase.word304 = 0;
    fldGlobalBase.word1D4 = 0;
    fldGlobalBase.word30C = 0;
    fldGlobalBase.word310 = 0;
    fldGlobalBase.word314 = 0;
    fldDbg.menuOn = 0;
    fldDbg.hitOn = 1;
    fldGlobalBase.word318 = 0;
    fldDbg.infoOn = 0;
    fldDbg.spriteTint = 0;
    fldDbg.skyEnable = 1;
    fldDbg.camOn = 0;
    fldDbg.skyCngOn = 1;
    fldDbg.bgCngOn = 1;
    fldDbg.menuCur = 0;
    fldDbg.ctnOn = 1;
    fldDbg.mizuEffOn = 0;
    fldDbg.encountOn = 0;
    fldDbg.encountBtnOn = 0;
    fldDbg.eveHitOn = 1;
    fldDbg.sousaAllMSKOn = 1;
    fldDbg.tmyDrawType = 0;
    fldDbg.zoomTestOn = 0;
    fldDbg.skyOffset = 0.0f;
    fldDbg.moonSpeed = 0;
    if (mode == 0) {
        DDS3_GBWK->unk2C70 = 0;
        DDS3_GBWK->unk2C72 = 0;
        p = DDS3_GBWK->unk2C78;
        for (i = 14; i >= 0; i--) {
            *p = 0;
            p++;
        }
        DDS3_GBWK->ver[0] = 'V';
        DDS3_GBWK->ver[1] = 'E';
        DDS3_GBWK->ver[2] = 'R';
        DDS3_GBWK->ver[3] = 1;
        DDS3_GBWK->ver[4] = 0;
        DDS3_GBWK->ver[5] = 0;
        DDS3_GBWK->ver[6] = 0;
        DDS3_GBWK->ver[7] = 1;
        DDS3_GBWK->word08 = 0;
        DDS3_GBWK->half0C = 0;
        DDS3_GBWK->transition = 0;
        DDS3_GBWK->unk21 = 0;
        for (i = 0; i < 15; i++) {
            DDS3_GBWK->unk22[i] = 0;
        }
        for (i = 0; i < 16; i++) {
            DDS3_GBWK->unk30[i] = 0;
        }
        DDS3_GBWK->mapSummonID = 0;
        mapAM_MaskClear();
        evtMoonTakaraCntReset();
        evtMoonZeniCntReset();
        evtMoonTakumiCntReset();
        evtMoonTeisatuCntReset();
        fldHirukoYukaClear();
    }
    mapIfMenuInit();
    mapAiboInit();
    mapSummonDvInit();
    mapNpcPathFastInit();
    mapLeaf12();
}
#else
INCLUDE_ASM(const s32, "fclDrawBack", fldGbInit_System);
#endif

/* NON_MATCHING: 25 of 37 words match. Fragile: retyping fldAreaWork_t 0x120..0x128 moves its stores. */
#ifdef NON_MATCHING
void fldGbInit_Field(void) {
    fldGlobalBase.word1D8 = 1;
    fldGlobalBase.word138 = 24;
    fldGlobalBase.lastMoveLeng = 0.0f;
    fldGlobalBase.word5C = 0;
    fldGlobalBase.word24 = 0;
    fldGlobalBase.previousFloor = -1;
    fldGlobalBase.hitX = 0.0f;
    fldGlobalBase.nowTalkModelID = 0;
    fldGlobalBase.hitY = 0.0f;
    fldGlobalBase.nowTakaraTblIdx = 0;
    fldGlobalBase.nowHealTblIdx = 0;
    fldGlobalBase.word158 = 0;
    fldDbg.menuOn = 0;
    fldDbg.menuCur = 0;
    fldGlobalBase.floatE4 = 0.0f;
    fldGlobalBase.word300 = 0;
    fldGlobalBase.initScript = NULL;
    fldGlobalBase.word318 = 0;
    fldGlobalBase.startScript = NULL;
    fldGlobalBase.hitMatterIndex = 0;
    fldGlobalBase.word2C4 = 0;
    fldGlobalBase.word2D4 = 0;
    fldGlobalBase.word2E0 = 0;
    fldGlobalBase.word2E4 = 0;
    fldGlobalBase.resourceArea = -1;
    cfgChkFlagChange();
}
#else
INCLUDE_ASM(const s32, "fclDrawBack", fldGbInit_Field);
#endif

s32 cfgChkFlagChange(void) {
    fldDbg.encountOn = 1;
    fldDbg.sousaAllMSKOn = 0;
    fldGlobalBase.word240 = 0;
    return getLeaf3(0);
}
