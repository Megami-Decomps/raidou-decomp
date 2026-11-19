#include "common.h"
#include "fclDrawBack.h"
#include "dds3KernelCore.h"
#include "sdfMemory.h"
#include "mapData.h"
#include "sdfFile.h"
#include "mapLbWork.h"

void fclLeaf6(fclMsgWork_t *wk) {
    fclCallMemFreeDirect(wk);
}

s32 fclCallGetProcessData(dds3ProcessID_t *PID) {
    fclMsgWork_t *wk = dds3GetProcessData(PID);

    fclLeaf6(wk);
    sdfMemFreeDirect(wk->hMem);
    FAKEMATCH_ASM_READ(wk);
#ifdef FIX_BUGS
    return 0;
#endif
}

INCLUDE_ASM(const s32, "fclDrawBack", fclProcessStart);

INCLUDE_ASM(const s32, "fclDrawBack", fclProcessEnd);

s32 dds3DestroyCallbackNodeAfterLastNotification(dds3ProcessID_t *PID)
{
    if (PID == NULL) {
        return 0;
    }
    if (dds3GetProcessStat(PID) == 3 || dds3GetProcessStat(PID) == 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "fclDrawBack", fclCallSetOt);

INCLUDE_ASM(const s32, "fclDrawBack", _BackSprite);

INCLUDE_ASM(const s32, "fclDrawBack", fclBackSprite);

INCLUDE_ASM(const s32, "fclDrawBack", fclCommandHelp);

INCLUDE_RODATA(const s32, "fclDrawBack", mapProcSequenceSubTable);

void fldAllTblLoad(void) {
    sdfFileCmdHandle_t *file;

    file = sdfFileOpen("/map/all/MAPALL.TBL");
    sdfFileRead(file, mapData155, 11840);
    sdfFileRead(file, mapData2, 8816);
    sdfFileRead(file, fldCallGetIntParaData, 2560);
    sdfFileRead(file, mapData3, 2048);
    sdfFileRead(file, mapData156, 1536);
    sdfFileRead(file, mapData157, 10240);
    sdfFileRead(file, mapData5, 3072);
    sdfFileRead(file, fldData, 3072);
    sdfFileRead(file, mapData6, 1536);
    sdfFileRead(file, mapFileLoadData2, 48);
    sdfFileRead(file, mapFileLoadData3, 48);
    sdfFileRead(file, mapFileLoadData4, 48);
    sdfFileRead(file, fldData2, 48);
    sdfFileRead(file, mapAutoMapDraw3Data, 768);
    sdfFileRead(file, mapData158, 1024);
    sdfFileRead(file, mapAutoMapDraw3Data2, 288);
    sdfFileRead(file, mapKazeLabelData, 128);
    sdfFileRead(file, mapCallBitCheckData, 5632);
    sdfFileRead(file, fldGenbaTbl, 3072);
    sdfFileRead(file, mapMapObjectRefreshData, 1536);
    sdfFileRead(file, fldCallGetIntParaData4, 192);
    sdfFileRead(file, fldData44, 16);
    sdfFileRead(file, tipsData, 1024);
    sdfFileRead(file, tipsData2, 1280);
    sdfFileRead(file, tipsData3, 1024);
    sdfFileRead(file, mapData164, 1280);
    sdfFileRead(file, mapData165, 1024);
    sdfFileRead(file, mapData166, 1280);
    sdfFileRead(file, mapMskEveHitTbl, 320);
    sdfFileRead(file, fldData45, 6400);
    sdfFileRead(file, nbData10, 256);
    sdfFileRead(file, nbData9, 128);
    sdfFileRead(file, nbData11, 768);
    sdfFileRead(file, nbData12, 512);
    sdfFileRead(file, mapWork20, 512);
    sdfFileRead(file, mapData159, 128);
    sdfFileClose(file);
}

INCLUDE_ASM(const s32, "fclDrawBack", fclCallNewGameInit);
