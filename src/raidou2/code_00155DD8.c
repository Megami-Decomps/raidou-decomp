#include "common.h"
#include "mapData.h"
#include "fld.h"
#include "sdfTex.h"
#include "dds3KernelDraw_0010A750.h"
#include "code_r1_0010DF68.h"
#include "sdfMemory.h"
#include "nb.h"

INCLUDE_ASM(const s32, "code_00155DD8", mapKaidanAdd);

void mapCHitAdd(s16 uniq, f32 x, f32 z, f32 r) {
    s32 i;

    for (i = 0; i < 256; i++) {
        if (mapData28[i].use == 0) {
            mapData28[i].use = 1;
            mapData28[i].uniq = uniq;
            mapData28[i].x = x;
            mapData28[i].z = z;
            mapData28[i].r = r;
            return;
        }
    }
}

INCLUDE_ASM(const s32, "code_00155DD8", mapWHitAdd);

INCLUDE_ASM(const s32, "code_00155DD8", mapFileLoadSub5);

INCLUDE_ASM(const s32, "code_00155DD8", mapNpcTblAdd);

INCLUDE_ASM(const s32, "code_00155DD8", mapEveHitTaskFldSub3);

INCLUDE_ASM(const s32, "code_00155DD8", mapNpcTblAdd_fld);

INCLUDE_ASM(const s32, "code_00155DD8", mapNpcTbl_GetModelID);

INCLUDE_ASM(const s32, "code_00155DD8", mapCallObjPointGet);

INCLUDE_ASM(const s32, "code_00155DD8", mapCallBitCheck3);

INCLUDE_ASM(const s32, "code_00155DD8", mapNpcTblRefresh_Wait);

INCLUDE_ASM(const s32, "code_00155DD8", mapBtlReLoadMapSBtlSceneSub);

INCLUDE_ASM(const s32, "code_00155DD8", mapMapObjectRefresh);

INCLUDE_ASM(const s32, "code_00155DD8", mapCallPositioningGuardSystem);

INCLUDE_ASM(const s32, "code_00155DD8", mapKazeLabel);

INCLUDE_ASM(const s32, "code_00155DD8", mapCallGetFloorY2);

INCLUDE_ASM(const s32, "code_00155DD8", mapCallObjNoClip);

INCLUDE_ASM(const s32, "code_00155DD8", mapCallBitCheck4);

void mapBtlReLoadMapSBtlSceneSub2(void) {
    mapBtlReLoadMapSBtlSceneSub2Data[0] = 1;
}

INCLUDE_ASM(const s32, "code_00155DD8", mapFileLoad);

INCLUDE_ASM(const s32, "code_00155DD8", mapBtlReLoadMapSBtlSceneSub3);

/* body: matched in raidou @ 780f3f57: src/raidou1/code_r1_0014F378.c:667, found, same function 0.99 (words 149/147, calls 0.98, shape 0.99) */
void fldFreeMapData(void)
{
    s32 i;

    mapFileLoadSub2();
    scrClearProcessGlobals();
    mapFreeMapDataSub();
    fldTitleMini_Shutdown();
    nbCallFreeEventPack();
    if (fldGlobalBase.texSlot[0].texture != NULL)
    {
        sdfTexFreeDelayed(fldGlobalBase.texSlot[0].texture);
        dds3FileLoadSub(fldGlobalBase.texSlot[0].file);
        fldGlobalBase.texSlot[0].texture = NULL;
        fldGlobalBase.texSlot[0].file = NULL;
    }
    if (fldGlobalBase.texSlot[1].texture != NULL)
    {
        sdfTexFreeDelayed(fldGlobalBase.texSlot[1].texture);
        dds3FileLoadSub(fldGlobalBase.texSlot[1].file);
        fldGlobalBase.texSlot[1].texture = NULL;
        fldGlobalBase.texSlot[1].file = NULL;
    }
    if (fldGlobalBase.texSlot[2].texture != NULL)
    {
        sdfTexFreeDelayed(fldGlobalBase.texSlot[2].texture);
        dds3FileLoadSub(fldGlobalBase.texSlot[2].file);
        fldGlobalBase.texSlot[2].texture = NULL;
        fldGlobalBase.texSlot[2].file = NULL;
    }
    if (fldGlobalBase.texture2B0 != NULL)
    {
        sdfTexFreeDelayed(fldGlobalBase.texture2B0);
        fldGlobalBase.texture2B0 = NULL;
    }
    mapCallEffectDelete();
    workRelease();
    if (fldFreeMapDataData != NULL)
    {
        fldGlobalBase.menuOpenFlag = 0;
        datDebugPrintf("map data free!!\n");
        sdfMemFreeDirect(fldFreeMapDataData);
        fldFreeMapDataData = NULL;
        mapFileLoadData7 = 0;
        pFldMap = NULL;
        pMapAreaNow = NULL;
        pMapAreaDat = NULL;
        pMapColmNow = NULL;
        pMapColmDat = NULL;
        mapMapSetAreaRefreshData = NULL;
        fldData25 = NULL;
        mapMapSetAreaRefreshData2 = NULL;
        fldData26 = NULL;
        mapMapSetAreaRefreshData3 = NULL;
        fldData27 = NULL;
        pMapObjNow = NULL;
        pMapObjSub = NULL;
        mapMapSetAreaRefreshData4 = NULL;
        fldData28 = NULL;
        pMapEveHitNow = NULL;
        fldData29 = NULL;
        for (i = 63; i >= 0; i--)
        {
            mapCallSetOt2Data2[i] = 0;
        }
    }
    if (mapData105 != NULL)
    {
        sdfMemFreeDirect(mapData105);
        mapData105 = NULL;
        mapFileLoadData9 = 0;
        mapFileLoadData10 = 0;
        mapData106 = NULL;
        mapData107 = NULL;
        mapData108 = NULL;
        mapData109 = NULL;
    }
    if (mapData99 != NULL)
    {
        sdfMemFreeDirect(mapData99);
        mapData99 = NULL;
        mapFileLoadData8 = 0;
        mapData100 = 0;
        mapMapSetAreaRefreshData5 = NULL;
        mapData101 = NULL;
        mapMapSetAreaRefreshData6 = NULL;
        mapMapSetAreaRefreshData7 = NULL;
        mapData102 = NULL;
        mapMapSetAreaData17 = NULL;
    }
    if (mapData110 != NULL)
    {
        sdfMemFreeDirect(mapData110);
        mapData110 = NULL;
        mapFileLoadData11 = 0;
        mapData111 = 0;
        mapMapSetAreaData18 = NULL;
        mapMapSetAreaData19 = NULL;
        mapMapSetAreaData20 = NULL;
        mapMapSetAreaData21 = NULL;
        mapData112 = 0;
        fldFreeMapDataData2 = 0;
        pMpbPointNow = 0;
        fldFreeMapDataData3 = 0;
        mapData113 = 0;
        fldFreeMapDataData4 = 0;
        mapMapSetAreaData22 = NULL;
        mapMapSetAreaData23 = NULL;
    }
    if (mefMistTexOnDataCheck() != 0)
    {
        mefMistTexDestroy();
    }
    gSubFloorID = -1;
}
