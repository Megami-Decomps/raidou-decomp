#include "common.h"
#include "code_r1_0010DF68.h"
#include "fld.h"
#include "mapData.h"
#include "field.h"

/* NON_MATCHING: 165 of 272 words match. Retail keeps the subfloor index in the register of the area argument and saves one callee register; mine saves two. */
#ifdef NON_MATCHING
void mapMapSetArea(s32 area)
{
    s32 sub;
    void *colm;
    void *d3;
    void *d4;
    void *d5;
    void *obj;
    void *d7;
    void *evehit;

    datDebugPrintf("mapMapSetArea(%d)\n", area);
    colm = mapMapSetAreaData2[area];
    d3 = mapMapSetAreaData3[area];
    d4 = mapMapSetAreaData4[area];
    d5 = mapMapSetAreaData5[area];
    obj = mapMapSetAreaData6[area];
    d7 = mapMapSetAreaData7[area];
    evehit = mapMapSetAreaData8[area];
    pMapAreaNow = mapMapSetAreaData[area];
    pMapColmNow = colm;
    mapMapSetAreaRefreshData = d3;
    mapMapSetAreaRefreshData2 = d4;
    mapMapSetAreaRefreshData3 = d5;
    pMapObjNow = obj;
    mapMapSetAreaRefreshData4 = d7;
    pMapEveHitNow = evehit;
    if (mapData105 != NULL)
    {
        if (area < mapFileLoadData10->count)
        {
            mapData106 = mapMapSetAreaData12[area];
            mapData108 = mapMapSetAreaData13[area];
        }
        else
        {
            mapData106 = NULL;
            mapData108 = NULL;
        }
    }
    if (mapData99 != NULL)
    {
        if (area < mapData100->count)
        {
            mapMapSetAreaRefreshData5 = mapMapSetAreaData9[area];
            mapMapSetAreaRefreshData6 = mapMapSetAreaData10[area];
            mapData102 = mapMapSetAreaData11[area];
        }
        else
        {
            mapMapSetAreaRefreshData5 = NULL;
            mapMapSetAreaRefreshData6 = NULL;
            mapData102 = NULL;
        }
    }
    if (mapData110 != NULL)
    {
        if (area < mapData111->count)
        {
            mapMapSetAreaData18 = mapMapSetAreaData14[area];
            mapMapSetAreaData20 = mapMapSetAreaData15[area];
            mapMapSetAreaData22 = mapMapSetAreaData16[area];
        }
        else
        {
            mapMapSetAreaData18 = NULL;
            mapMapSetAreaData20 = NULL;
            mapMapSetAreaData22 = NULL;
        }
    }
    sub = -1;
    if (fldGlobalBase.menuOpenFlag == 0)
    {
        if (fldGlobalBase.amSBlock == 2)
        {
            if (area == 0)
            {
                sub = 2;
                goto have_sub;
            }
            if (area == 2)
            {
                goto sub_zero;
            }
        }
        if (fldGlobalBase.amSBlock == 22)
        {
            if (area == 0)
            {
                sub = 2;
                goto have_sub;
            }
            if (area == 2)
            {
                goto sub_zero;
            }
        }
        if (fldGlobalBase.amSBlock != 8)
        {
            goto field13;
        }
        if (area == 0)
        {
            sub = 1;
            goto have_sub;
        }
        if (area == 1)
        {
sub_zero:
            sub = 0;
        }
        else
        {
field13:
            if (fldGlobalBase.amSBlock == 13)
            {
                if (area == 4)
                {
                    sub = 3;
                }
                if (area == 3)
                {
                    sub = 4;
                }
            }
        }
    }
have_sub:
    if (sub != -1)
    {
        gSubFloorID = sub;
        datDebugPrintf("gSubFloorID:%d\n", sub);
        pMapAreaDat = mapMapSetAreaData[sub];
        pMapColmDat = mapMapSetAreaData2[sub];
        fldData25 = mapMapSetAreaData3[sub];
        fldData26 = mapMapSetAreaData4[sub];
        fldData27 = mapMapSetAreaData5[sub];
        pMapObjSub = mapMapSetAreaData6[sub];
        fldData28 = mapMapSetAreaData7[sub];
        fldData29 = mapMapSetAreaData8[sub];
        if (mapData105 != NULL)
        {
            mapData107 = mapMapSetAreaData12[sub];
            mapData109 = mapMapSetAreaData13[sub];
        }
        if (mapData99 != NULL)
        {
            if (sub < mapData100->count)
            {
                mapData101 = mapMapSetAreaData9[sub];
                mapMapSetAreaRefreshData7 = mapMapSetAreaData10[sub];
                mapMapSetAreaData17 = mapMapSetAreaData11[sub];
            }
            else
            {
                mapData101 = NULL;
                mapMapSetAreaRefreshData7 = NULL;
                mapMapSetAreaData17 = NULL;
            }
        }
        if (mapData110 != NULL)
        {
            if (sub < mapData111->count)
            {
                mapMapSetAreaData19 = mapMapSetAreaData14[sub];
                mapMapSetAreaData21 = mapMapSetAreaData15[sub];
                mapMapSetAreaData23 = mapMapSetAreaData16[sub];
            }
            else
            {
                mapMapSetAreaData19 = NULL;
                mapMapSetAreaData21 = NULL;
                mapMapSetAreaData23 = NULL;
            }
        }
    }
    mapData103 = (f32)(-100 * pMapAreaNow->originX);
    mapData104 = (f32)(100 * pMapAreaNow->originZ);
}
#else
INCLUDE_ASM(const s32, "code_r1_00151980", mapMapSetArea);
#endif
