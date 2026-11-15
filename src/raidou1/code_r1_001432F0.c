#include "common.h"
#include "mapData.h"
#include "field.h"

void mapFileLoadSub2(void)
{
    s32 i;

    for (i = 0; i < 128; i++)
    {
        mapObjTaskData2[i].basic = NULL;
        mapObjTaskData2[i].use = 0;
        mapObjTaskData2[i].uniq = -1;
        mapObjTaskData2[i].visible = 0;
        mapObjTaskData2[i].scr_name[0] = 0;
        mapObjTaskData2[i].spcial = 0;
        mapObjTaskData2[i].floor = -1;
        mapObjTaskData2[i].npc_idx = -1;
    }
    for (i = 0; i < 128; i++)
    {
        mapScriptGet_PosData2[i].use = 0;
        mapScriptGet_PosData2[i].scr_name[0] = 0;
        mapScriptGet_PosData2[i].msk = 0;
    }
    for (i = 0; i < 128; i++)
    {
        mapData23[i].use = 0;
        mapData23[i].flag = 0;
        mapData23[i].ix = 0;
        mapData23[i].iy = 0;
        mapData23[i].attr = 0;
    }
    for (i = 0; i < 64; i++)
    {
        mapWallObjTaskData[i].use = 0;
        mapWallObjTaskData[i].uniq = -1;
        mapWallObjTaskData[i].type = 0;
        mapWallObjTaskData[i].flag = 0;
    }
    for (i = 0; i < 32; i++)
    {
        mapData32[i].use = 0;
        mapData32[i].uniq = -1;
    }
    for (i = 0; i < 16; i++)
    {
        mapData25[i].use = 0;
        mapData25[i].uniq = -1;
    }
    for (i = 0; i < 256; i++)
    {
        mapData26[i].basic = NULL;
        mapData26[i].use = 0;
        mapData26[i].uniq = -1;
    }
    for (i = 0; i < 80; i++)
    {
        mapData24[i].use = 0;
        mapData24[i].id = 0;
    }
    for (i = 0; i < 64; i++)
    {
        mapData27[i].use = 0;
        mapData27[i].uniq = -1;
    }
    for (i = 0; i < 256; i++)
    {
        mapData28[i].use = 0;
        mapData28[i].uniq = -1;
    }
    for (i = 0; i < 128; i++)
    {
        mapData29[i].use = 0;
        mapData29[i].uniq = -1;
    }
    for (i = 0; i < 32; i++)
    {
        mapData30[i].use = 0;
        mapData30[i].uniq = -1;
    }
    for (i = 0; i < 32; i++)
    {
        mapData31[i].use = 0;
        mapData31[i].uniq = -1;
    }
}
