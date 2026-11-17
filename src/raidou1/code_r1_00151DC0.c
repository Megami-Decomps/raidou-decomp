#include "common.h"
#include "dds3SceneBasic.h"
#include "fld.h"
#include "mapData.h"
#include "field.h"

/* NON_MATCHING: 86 of 896 words match. Draft 884 words vs retail 1437. All calls match in order except the object loop: retail unrolls id&0xF into a jump table with one mapCHitAdd site per mask case (16 sites) and an id compare tree with 7 mapWHitAdd sites; mapCallBitCheck2 appears twice (floor and sub-floor arms). Arguments and constants checked against the asm for all 4 CHit bit offsets and all 16 wall ids; the draft uses a loop and a table. */
#ifdef NON_MATCHING
void mapMapSetAreaRefresh(void)
{
    f32 pos[2];
    f32 pos2[2];
    f32 y;
    f32 yOfs;
    s32 i;
    s32 j;
    s32 k;
    s32 b;
    s32 id;
    u64 mask64;
    u32 mask32;
    u8 *mapBase;
    u64 *grid64;
    u32 *grid32;
    fldMapEveHitHead_t *eve;
    fldMapPointRec_t *pt;
    fldMapHit20Rec_t *h20;
    fldMapHit64Rec_t *h64;
    fldMapKaidanRec_t *kai;
    fldMapFlagRec_t *flag2;
    fldMapBitRec_t *flag1;
    fldMapAreaHead_t *area;
    fldMapEveHitHead_t *ev;
    static const f32 wallOfs[2][8][5] = {
        { { 0.0f, 100.0f, 200.0f, -1.0f, -0.5f }, { 0.0f, 100.0f, 200.0f, 1.0f, 0.5f },
          { 100.0f, 0.0f, 200.0f, 1.0f, -0.5f }, { 100.0f, 0.0f, 200.0f, -1.0f, 0.5f },
          { 0.0f, 200.0f, 100.0f, -0.5f, -1.0f }, { 0.0f, 200.0f, 100.0f, 0.5f, 1.0f },
          { 200.0f, 0.0f, 100.0f, 0.5f, -1.0f }, { 200.0f, 0.0f, 100.0f, -0.5f, 1.0f } },
        { { 0.0f, 100.0f, 300.0f, -3.0f, -1.0f }, { 0.0f, 100.0f, 300.0f, 3.0f, 1.0f },
          { 100.0f, 0.0f, 300.0f, 3.0f, -1.0f }, { 100.0f, 0.0f, 300.0f, -3.0f, 1.0f },
          { 0.0f, 300.0f, 100.0f, -1.0f, -3.0f }, { 0.0f, 300.0f, 100.0f, 1.0f, 3.0f },
          { 300.0f, 0.0f, 100.0f, 1.0f, -3.0f }, { 300.0f, 0.0f, 100.0f, -1.0f, 3.0f } },
    };

    for (i = 255; i >= 0; i--)
    {
        mapData26[255 - i].use = 0;
        mapData26[255 - i].uniq = -1;
        if (mapData26[255 - i].basic != NULL)
        {
            dds3DeleteSceneBasicData(mapData26[255 - i].basic);
            mapData26[255 - i].basic = NULL;
        }
    }
    for (i = 0; i < 128; i++)
    {
        mapScriptGet_PosData2[i].use = 0;
        mapScriptGet_PosData2[i].scr_name[0] = 0;
    }
    for (i = 0; i < 80; i++)
    {
        mapData24[i].use = 0;
    }
    for (i = 0; i < 64; i++)
    {
        mapData27[i].use = 0;
    }
    for (i = 0; i < 256; i++)
    {
        mapData28[i].use = 0;
    }
    for (i = 0; i < 128; i++)
    {
        mapData29[i].use = 0;
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
    for (i = 0; i < 128; i++)
    {
        mapData23[i].use = 0;
        mapData23[i].flag = 0;
        mapData23[i].ix = 0;
        mapData23[i].iy = 0;
        mapData23[i].attr = 0;
    }
    fldCallMemFreeDirect();

    ev = pMapEveHitNow;
    if (ev != NULL)
    {
        mapCallMemAllocDirect(ev->w + 1, ev->h + 1);
        mapBase = pFldMap;
        if (ev->wide != 0)
        {
            grid64 = (u64 *)(mapBase + ev->maskOfs);
            for (k = 0; k < 64; k++)
            {
                if (ev->type[k] != 0)
                {
                    for (i = 0; i <= ev->w; i++)
                    {
                        for (j = 0; j <= ev->h; j++)
                        {
                            mapSetAreaRefreshSub(i, j, ev->type[k], 0);
                        }
                    }
                }
            }
            for (i = 0; i < ev->w; i++)
            {
                for (j = 0; j < ev->h; j++)
                {
                    mask64 = grid64[ev->w * j + i];
                    for (k = 0; k < 64; k++)
                    {
                        if (ev->type[k] != 0)
                        {
                            if (((mask64 >> k) & 1) != 0)
                            {
                                mapSetAreaRefreshSub(i, j, ev->type[k], 1);
                            }
                            else
                            {
                                mapSetAreaRefreshSub(i, j, ev->type[k], 0);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            grid32 = (u32 *)(mapBase + ev->maskOfs);
            for (k = 0; k < 32; k++)
            {
                for (i = 0; i <= ev->w; i++)
                {
                    for (j = 0; j <= ev->h; j++)
                    {
                        mapSetAreaRefreshSub(i, j, ev->type[k], 0);
                    }
                }
            }
            for (i = 0; i < ev->w; i++)
            {
                for (j = 0; j < ev->h; j++)
                {
                    mask32 = grid32[ev->w * j + i];
                    for (k = 0; k < 32; k++)
                    {
                        if ((mask32 & (1 << k)) != 0)
                        {
                            mapSetAreaRefreshSub(i, j, ev->type[k], 1);
                        }
                        else
                        {
                            mapSetAreaRefreshSub(i, j, ev->type[k], 0);
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i < pMapAreaNow->pointCount; i++)
    {
        pt = &mapMapSetAreaRefreshData2[i];
        yOfs = 0.0f;
        PositioningGuardSystem(pt->ix, pt->iy, &pos[0], &pos[1]);
        switch (pt->ofs)
        {
        case 0:
            pos[0] += 50.0f;
            pos[1] -= 50.0f;
            break;
        case 2:
            pos[0] += 100.0f;
            break;
        case 3:
            pos[1] -= 100.0f;
            break;
        case 4:
            pos[0] += 100.0f;
            pos[1] -= 100.0f;
            break;
        case 5:
            yOfs = -200.0f;
            break;
        case 6:
            yOfs = -400.0f;
            break;
        case 7:
            yOfs = -600.0f;
            break;
        case 8:
            yOfs = -800.0f;
            break;
        case 9:
            yOfs = -1000.0f;
            break;
        }
        y = yOfs + mapGetY(pos[0], pos[1]);
        mapObjPointAdd2(pt->id, evtWorkScriptContextReturnValue2, pos[0], y, pos[1], mapData20[pt->dir]);
    }
    for (i = 0; i < pMapAreaNow->hit20Count; i++)
    {
        h20 = &mapMapSetAreaRefreshData3[i];
        mapCallPositioningGuardSystem3(h20->w00, h20->w02, h20->w04, h20->w06, h20->w08, h20->b0C, h20->w0A, h20->w0E,
                                       h20->w10, h20->w12, h20->b0D);
    }
    for (i = 0; i < pMapAreaNow->hit64Count; i++)
    {
        h64 = &mapMapSetAreaRefreshData[i];
        mapCallPositioningGuardSystem2(h64->w00, h64->w02, h64->w04, h64->w06, h64->w08, h64->w0A, h64->w0E, h64->b12,
                                       h64->w10, h64->b13, h64->w0C, h64->word30, h64->w14);
    }
    if (mapMapSetAreaRefreshData5 != NULL)
    {
        for (i = 0; i < mapMapSetAreaRefreshData5->hit64Count; i++)
        {
            h64 = &mapData102[i];
            mapCallPositioningGuardSystem2(h64->w00, h64->w02, h64->w04, h64->w06, h64->w08, h64->w0A, h64->w0E,
                                           h64->b12, h64->w10, h64->b13, h64->w0C, h64->word30, h64->w14);
        }
        for (i = 0; i < mapMapSetAreaRefreshData5->pointCount; i++)
        {
            pt = &mapMapSetAreaRefreshData6[i];
            PositioningGuardSystem(pt->ix, pt->iy, &pos[0], &pos[1]);
            switch (pt->ofs)
            {
            case 0:
                pos[0] += 50.0f;
                pos[1] -= 50.0f;
                break;
            case 2:
                pos[0] += 100.0f;
                break;
            case 3:
                pos[1] -= 100.0f;
                break;
            case 4:
                pos[0] += 100.0f;
                pos[1] -= 100.0f;
                break;
            }
            y = mapGetY(pos[0], pos[1]);
            mapObjPointAdd2(pt->id, evtWorkScriptContextReturnValue2, pos[0], y, pos[1], mapData20[pt->dir]);
        }
    }
    if (mapData101 != NULL)
    {
        for (i = 0; i < mapData101->pointCount; i++)
        {
            pt = &mapMapSetAreaRefreshData7[i];
            PositioningGuardSystem(pt->ix, pt->iy, &pos[0], &pos[1]);
            switch (pt->ofs)
            {
            case 0:
                pos[0] += 50.0f;
                pos[1] -= 50.0f;
                break;
            case 2:
                pos[0] += 100.0f;
                break;
            case 3:
                pos[1] -= 100.0f;
                break;
            case 4:
                pos[0] += 100.0f;
                pos[1] -= 100.0f;
                break;
            }
            y = mapGetFloorY(gSubFloorID, pos[0], pos[1]);
            mapObjPointAdd2(pt->id, gSubFloorID, pos[0], y, pos[1], mapData20[pt->dir]);
        }
    }
    for (i = 0; i < pMapAreaNow->kaidanCount; i++)
    {
        kai = &mapMapSetAreaRefreshData4[i];
        PositioningGuardSystem(kai->ix0, kai->iy0, &pos[0], &pos[1]);
        PositioningGuardSystem(kai->ix1, kai->iy1, &pos2[0], &pos2[1]);
        mapKaidanAdd(-1, kai->type, kai->xory, pos[0], pos[1], pos2[0], pos2[1], (f32)kai->h0 * -5.0f,
                     (f32)kai->h1 * -5.0f, (f32)kai->haba, kai->h0);
    }
    area = pMapAreaNow;
    if (area != NULL)
    {
        for (i = 0; i < area->objCount; i++)
        {
            PositioningGuardSystem(pMapObjNow[i].ix, pMapObjNow[i].iy, &pos[0], &pos[1]);
            id = pMapObjNow[i].id;
            if ((u16)(id - 0x4000) < 0x100)
            {
                for (b = 0; b < 4; b++)
                {
                    if ((id >> b) & 1)
                    {
                        mapCHitAdd(-1, pos[0] + ((b & 1) ? 75.0f : 25.0f), pos[1] - ((b >> 1) ? 75.0f : 25.0f), 25.0f);
                    }
                }
            }
            else if ((u16)(id - 0x4100) < 8 || (u16)(id - 0x4200) < 8)
            {
                k = (id >> 9) & 1;
                j = id & 7;
                mapWHitAdd(-1, id, pos[0] + wallOfs[k][j][0], pos[1], pos[0] + wallOfs[k][j][1],
                           pos[1] - wallOfs[k][j][2], wallOfs[k][j][3], wallOfs[k][j][4]);
            }
        }
    }
    mapNpcTblRefresh_WaitData = -1;
    npc_delcash_cnt = 0;
    npc_addcash_cnt = 0;
    if (fldGlobalBase.menuOpenFlag != 0)
    {
        return;
    }
    if (mapMapSetAreaRefreshData5 != NULL || area != NULL)
    {
        mapNpcTblRefresh();
        while (mapNpcTblRefresh_Wait())
        {
        }
    }
    mapMapObjectRefresh();
    if (fldGlobalBase.menuOpenFlag != 0)
    {
        return;
    }
    for (i = 0; i < 128; i++)
    {
        flag1 = &mapCallSetOt2Data[i];
        if (flag1->field == evtWorkScriptContextReturnValue &&
            (flag1->floor == evtWorkScriptContextReturnValue2 || flag1->floor == gSubFloorID) && flag1->kind < 8)
        {
            mapCallBitCheck2(flag1->floor, -1, flag1->kind, flag1->b09, flag1->w04, flag1->w06, flag1->b08, flag1->s0A,
                             flag1->s0C);
        }
    }
    if (fldGlobalBase.menuOpenFlag == 0)
    {
        for (i = 0; i < 64; i++)
        {
            flag2 = &mapData7[i];
            if (flag2->field == evtWorkScriptContextReturnValue && flag2->floor == evtWorkScriptContextReturnValue2)
            {
                mapSetAreaRefreshSub2(flag2->floor, -1, flag2->b07, flag2->w02, flag2->w04, flag2->b06, flag2->s08,
                                      flag2->s0A, flag2->s0C, flag2->s0E);
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00151DC0", mapMapSetAreaRefresh);
#endif
