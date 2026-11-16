#include "common.h"
#include "sdfMemory.h"
#include "dds3WorldBasic.h"
#include "mdlManager.h"
#include "dds3ObjectBase.h"
#include "code_r1_0010DF68.h"
#include "dds3KernelDraw_0010A750.h"
#include "EventBit.h"
#include "fileManager.h"
#include "fld.h"
#include "sdfTex.h"
#include "sdk_libc.h"
#include "mapData.h"
#include "dds3SceneBasic.h"
#include "dds3UnitObjectBasic.h"
#include "sdfMotion.h"
#include "field.h"
#include "sbattle.h"
#include "sbtlDebugStr.h"

/* NON_MATCHING: 48 of 2283 words match. Draft 1972 words vs retail 2283. Calls match in order; retail unrolls the 8 UI marker groups (2 mapFileLoadSub4, hukidashi loop) and duplicates the mapEXFloor switch tail; the draft uses a uiTable[8] loop. Table values checked against IDA. */
#ifdef NON_MATCHING
s32 mapFileLoad(s32 mapid)
{
    static const struct
    {
        s32 kind;
        s16 uniq;
        s32 minor;
    } uiTable[8] = {
        { 2, -28672, 66 }, { 3, -28656, 67 }, { 4, -28640, 68 }, { 5, -28624, 69 },
        { 6, -28608, 70 }, { 7, -28592, 71 }, { 0, -28576, 64 }, { 1, -28560, 65 },
    };
    char filename[64];
    f32 lightX;
    f32 lightZ;
    f32 dif[3];
    f32 amb[3];
    f32 px;
    f32 pz;
    f32 py;
    f32 pr;
    void *top;
    u32 loadedTex;
    s32 special;
    s32 k;
    s32 i;
    s32 n;
    s16 uniq;
    fileRequest_t *pac;
    sdfPacDecodeResrc_t *node;
    sdfMemHandle_t *file;
    fldMapFileHead_t *map;
    fldMapAreaHead_t *area;
    fldMapSub2File_t *sub2;
    fldMapSub3File_t *sub3;
    fldMapSub4File_t *sub4;
    fldMapLightRec_t *light;
    mapOt2Block_t *extra;
    sdfTexHandle_t *tex;
    dds3Basic_t *basic;

    top = NULL;
    datDebugPrintf("mapFileLoad() %d\n", fldGlobalBase.menuOpenFlag);
    special = 0;
    switch (mapid)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 10:
    case 15:
        if (!evtBitCheck(145) || (special = 1, evtBitCheck(244)))
        {
            special = evtBitCheck(206) != 0;
        }
        break;
    default:
        break;
    }
    fldGlobalBase.word318 = special;
    if (special)
    {
        mapFileLoadData = 0x506874;
    }
    else
    {
        mapFileLoadData = 0x706864;
    }
    k = 0;
    if ((mapid == 2 || mapid == 4 || mapid == 5) && evtBitCheck(194))
    {
        k = !evtBitCheck(205);
    }
    fldSe_Init();
    scrClearProcessGlobals();
    mapFileLoadSub2();
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
    fldFreeMapData();
    if (fldGlobalBase.texture2B0 != NULL)
    {
        sdfTexFreeDelayed(fldGlobalBase.texture2B0);
        fldGlobalBase.texture2B0 = NULL;
    }
    if (k)
    {
        sprintf(filename, "/map/map%02d/PAC00K.LB", mapid);
    }
    else if (special)
    {
        sprintf(filename, "/map/map%02d/PAC00Y.LB", mapid);
    }
    else
    {
        sprintf(filename, "/map/map%02d/PAC00.LB", mapid);
    }
    pac = pacLoad(filename);
    pacSync(pac);
    for (node = pac->pac.resrcHead; node != NULL; node = node->next)
    {
        switch (((sdfPacStdBlockHeader_t *)node->header)->kind)
        {
        case 1:
            fldFreeMapDataData = node->resrc;
            mapFileLoadData7 = sdfMemGetPtr(fldFreeMapDataData);
            break;
        case 2:
            if (mapFileLoadData2[mapid] != 1)
            {
                sdfMemFreeDirect(node->resrc);
                break;
            }
            mapData105 = node->resrc;
            mapFileLoadData9 = sdfMemGetPtr(mapData105);
            break;
        case 3:
            if (mapFileLoadData3[mapid] != 1)
            {
                sdfMemFreeDirect(node->resrc);
                break;
            }
            mapData99 = node->resrc;
            mapFileLoadData8 = sdfMemGetPtr(mapData99);
            break;
        case 4:
            if (mapFileLoadData4[mapid] != 1)
            {
                sdfMemFreeDirect(node->resrc);
                break;
            }
            mapData110 = node->resrc;
            mapFileLoadData11 = sdfMemGetPtr(mapData110);
            break;
        case 5:
            top = sdfMemGetPtr(node->resrc);
            memcpy(mapData33, top, sizeof(mapData33));
            sdfMemFreeDirect(node->resrc);
            break;
        case 6:
            if (fldGlobalBase.menuOpenFlag != 0 || mapid >= 35)
            {
                sdfMemFreeDirect(node->resrc);
                break;
            }
            dds3SceneBasicLoadScriptFile_FromMem(dds3GetControlScene(), node->resrc);
            break;
        case 7:
            if (mapData6[mapid][evtWorkScriptContextReturnValue2] == 1)
            {
                top = sdfMemGetPtr(node->resrc);
                fldGlobalBase.texture2B0 = sdfTexSet(top);
                sdfMemFreeDelayedDirect(node->resrc);
            }
            else
            {
                sdfMemFreeDirect(node->resrc);
            }
            break;
        case 8:
            top = sdfMemGetPtr(node->resrc);
            fldGlobalBase.texSlot[0].texture = sdfTexSet(top);
            sdfMemFreeDelayedDirect(node->resrc);
            fldGlobalBase.texSlot[0].file = dds3CallMatNew2(fldGlobalBase.texSlot[0].texture);
            fldGlobalBase.texSlot[0].file->constant = 1.0f;
            break;
        case 9:
            top = sdfMemGetPtr(node->resrc);
            fldGlobalBase.texSlot[2].texture = sdfTexSet(top);
            sdfMemFreeDelayedDirect(node->resrc);
            fldGlobalBase.texSlot[2].file = dds3CallMatNew2(fldGlobalBase.texSlot[2].texture);
            fldGlobalBase.texSlot[2].file->constant = 1.0f;
            break;
        default:
            sdfMemFreeDirect(node->resrc);
            break;
        }
    }
    pacFileHandleFree_(pac);

    pFldMap = mapFileLoadData7;
    map = mapFileLoadData7;
    for (i = 0; i < map->count && i < 64; i++)
    {
        area = &map->area[i];
        mapMapSetAreaData[i] = area;
        mapMapSetAreaData2[i] = (MapColm_t *)((u8 *)map + area->colmOfs);
        mapMapSetAreaData3[i] = (u8 *)map + area->hit64Ofs;
        mapMapSetAreaData4[i] = (u8 *)map + area->pointOfs;
        mapMapSetAreaData5[i] = (u8 *)map + area->hit20Ofs;
        mapMapSetAreaData6[i] = (u8 *)map + area->objOfs;
        mapMapSetAreaData7[i] = (u8 *)map + area->kaidanOfs;
        mapMapSetAreaData8[i] = (u8 *)map + area->eveHitOfs;
        if (area->extraOfs != 0)
        {
            mapCallSetOt2Data2[i] = (mapOt2Block_t *)((u8 *)map + area->extraOfs);
        }
        extra = mapCallSetOt2Data2[i];
        if (extra != NULL)
        {
            mapData36[i] = (u8 *)map + extra->ofs04;
            mapData35[i] = (mapOt2Cell_t *)((u8 *)map + extra->ofs00);
        }
    }
    if (mapFileLoadData2[mapid] == 1)
    {
        mapFileLoadData10 = mapFileLoadData9;
        sub2 = mapFileLoadData9;
        for (i = 0; i < sub2->count && i < 64; i++)
        {
            mapMapSetAreaData12[i] = &sub2->entry[i];
            mapMapSetAreaData13[i] = (u8 *)sub2 + sub2->entry[i].ofs0;
        }
    }
    if (mapFileLoadData3[mapid] == 1)
    {
        mapData100 = mapFileLoadData8;
        sub3 = mapFileLoadData8;
        for (i = 0; i < sub3->count && i < 64; i++)
        {
            mapMapSetAreaData9[i] = &sub3->entry[i];
            mapMapSetAreaData11[i] = (u8 *)sub3 + sub3->entry[i].hit64Ofs;
            mapMapSetAreaData10[i] = (u8 *)sub3 + sub3->entry[i].pointOfs;
        }
    }
    if (mapFileLoadData4[mapid] == 1)
    {
        mapData111 = mapFileLoadData11;
        sub4 = mapFileLoadData11;
        for (i = 0; i < sub4->count && i < 64; i++)
        {
            mapMapSetAreaData14[i] = &sub4->entry[i];
            mapMapSetAreaData15[i] = (u8 *)sub4 + sub4->entry[i].addr_btlhit;
            mapFileLoadData6[i] = (u8 *)sub4 + sub4->entry[i].addr_enchead;
            mapMapSetAreaData16[i] = (u8 *)sub4 + sub4->entry[i].addr_rndenc;
        }
    }
    mapMapSetArea(evtWorkScriptContextReturnValue2);
    if (mapData106 != NULL)
    {
        light = (fldMapLightRec_t *)((u8 *)mapFileLoadData10 + mapData106->lightOfs);
        for (i = 0; i < mapData106->count; i++)
        {
            mapFileLoadData5[i] = &light[i];
            mapFileLoadSub6(&lightX, &lightZ, light[i].x, light[i].z);
            dif[0] = light[i].difR;
            dif[1] = light[i].difG;
            dif[2] = light[i].difB;
            amb[0] = light[i].ambR;
            amb[1] = light[i].ambG;
            amb[2] = light[i].ambB;
            for (n = 0; n < 3; n++)
            {
                if (dif[n] > 1.9f)
                {
                    dif[n] = 1.9f;
                }
                if (amb[n] > 1.9f)
                {
                    amb[n] = 1.9f;
                }
            }
            mapObjLightAdd(-1, lightX, -10.0f, lightZ, light[i].inR * 100.0f, light[i].outR * 100.0f, dif[0], dif[1],
                           dif[2], amb[0], amb[1], amb[2]);
        }
    }
    if (evtWorkScriptContextReturnValue == 1 || evtWorkScriptContextReturnValue == 21 ||
        evtWorkScriptContextReturnValue == 15 ||
        (evtWorkScriptContextReturnValue == 3 && evtWorkScriptContextReturnValue2 == 1) ||
        (evtWorkScriptContextReturnValue == 23 && evtWorkScriptContextReturnValue2 == 1) ||
        (evtWorkScriptContextReturnValue == 4 &&
         (evtWorkScriptContextReturnValue2 == 0 || evtWorkScriptContextReturnValue2 == 4 ||
          evtWorkScriptContextReturnValue2 == 5)) ||
        (evtWorkScriptContextReturnValue == 24 && evtWorkScriptContextReturnValue2 == 0))
    {
        if (fldGlobalBase.texSlot[0].texture == NULL)
        {
            sprintf(filename, "/map/all/water01.tmx");
            file = sdfDevLoadFileEx(filename, &loadedTex, NULL);
            fldGlobalBase.texSlot[0].texture = sdfTexSet((TMX_HEADER_t *)loadedTex);
            sdfMemFreeDelayedDirect(file);
            fldGlobalBase.texSlot[0].file = dds3CallMatNew2(fldGlobalBase.texSlot[0].texture);
            fldGlobalBase.texSlot[0].file->constant = 1.0f;
        }
        if (fldGlobalBase.texSlot[2].texture == NULL)
        {
            sprintf(filename, "/map/all/water03.tmx");
            file = sdfDevLoadFileEx(filename, &loadedTex, NULL);
            fldGlobalBase.texSlot[2].texture = sdfTexSet((TMX_HEADER_t *)loadedTex);
            sdfMemFreeDelayedDirect(file);
            fldGlobalBase.texSlot[2].file = dds3CallMatNew2(fldGlobalBase.texSlot[2].texture);
            fldGlobalBase.texSlot[2].file->constant = 1.0f;
        }
    }
    if (fldGlobalBase.menuOpenFlag == 0 &&
        (evtWorkScriptContextReturnValue == 6 ||
         (evtWorkScriptContextReturnValue == 8 && evtWorkScriptContextReturnValue2 == 2) ||
         (evtWorkScriptContextReturnValue == 9 && evtWorkScriptContextReturnValue2 == 1) ||
         evtWorkScriptContextReturnValue == 13 || evtWorkScriptContextReturnValue == 21 ||
         evtWorkScriptContextReturnValue == 22 || evtWorkScriptContextReturnValue == 23 ||
         evtWorkScriptContextReturnValue == 24 || evtWorkScriptContextReturnValue == 25 ||
         evtWorkScriptContextReturnValue == 27 || evtWorkScriptContextReturnValue == 31))
    {
        if (mefMistTexOnDataCheck())
        {
            mefMistTexDestroy();
        }
        mapLoadMistTexture();
        if (evtWorkScriptContextReturnValue >= 20)
        {
            pipeFileLoadSub(0);
        }
        else
        {
            pipeFileLoadSub(1);
        }
        switch (evtWorkScriptContextReturnValue)
        {
        case 6:
            pipeFileLoadSub2(10);
            break;
        case 8:
        case 13:
            pipeFileLoadSub2(8);
            break;
        case 9:
        case 22:
        case 27:
            pipeFileLoadSub2(24);
            break;
        case 21:
        case 24:
            pipeFileLoadSub2(64);
            break;
        case 23:
        case 25:
            pipeFileLoadSub2(32);
            break;
        case 31:
            pipeFileLoadSub2(48);
            break;
        default:
            break;
        }
    }
    mapCallTexAllocFromHead();
    mapMapSetAreaRefresh();
    if (evtWorkScriptContextReturnValue == 31)
    {
        if (evtWorkScriptContextReturnValue2 == 0)
        {
            mapFileLoadSub5(0, 0.0f, -200.0f, -1000.0f, 700.0f, -260.0f, -700.0f, 500.0f, -200.0f, -1700.0f, 1000.0f, -260.0f, -1400.0f);
            mapFileLoadSub5(0, 700.0f, -260.0f, -700.0f, 900.0f, -305.0f, -500.0f, 1000.0f, -260.0f, -1400.0f, 1500.0f, -305.0f, -750.0f);
            mapFileLoadSub5(0, 900.0f, -305.0f, -500.0f, 1000.0f, -350.0f, 0.0f, 1500.0f, -305.0f, -750.0f, 1700.0f, -350.0f, -200.0f);
            mapFileLoadSub5(0, -1300.0f, -455.0f, 900.0f, -800.0f, -455.0f, 600.0f, -1600.0f, -500.0f, 300.0f, -1100.0f, -500.0f, 100.0f);
            mapFileLoadSub5(0, -1100.0f, -410.0f, 1300.0f, -600.0f, -410.0f, 900.0f, -1300.0f, -455.0f, 900.0f, -800.0f, -455.0f, 600.0f);
            mapFileLoadSub5(0, -1100.0f, -410.0f, 1300.0f, -200.0f, -350.0f, 1800.0f, -600.0f, -410.0f, 900.0f, -100.0f, -350.0f, 1000.0f);
        }
        if (evtWorkScriptContextReturnValue2 == 1)
        {
            mapFileLoadSub5(0, 600.0f, -85.0f, -400.0f, 1100.0f, -85.0f, -700.0f, 200.0f, 0.0f, -1000.0f, 800.0f, 0.0f, -1200.0f);
            mapFileLoadSub5(0, 800.0f, -160.0f, 0.0f, 1400.0f, -160.0f, -200.0f, 600.0f, -85.0f, -400.0f, 1100.0f, -85.0f, -700.0f);
            mapFileLoadSub5(0, 600.0f, -245.0f, 400.0f, 1200.0f, -230.0f, 700.0f, 800.0f, -160.0f, 0.0f, 1400.0f, -160.0f, -200.0f);
            mapFileLoadSub5(0, -1500.0f, -350.0f, 0.0f, -700.0f, -365.0f, 0.0f, -800.0f, -500.0f, -1300.0f, -300.0f, -500.0f, -800.0f);
        }
        if (evtWorkScriptContextReturnValue2 == 2)
        {
            mapFileLoadSub5(0, 400.0f, -105.0f, 800.0f, 1000.0f, -105.0f, 1200.0f, 700.0f, 0.0f, 300.0f, 1300.0f, 0.0f, 300.0f);
            mapFileLoadSub5(0, 0.0f, -175.0f, 900.0f, -100.0f, -175.0f, 1800.0f, 400.0f, -105.0f, 800.0f, 1000.0f, -105.0f, 1200.0f);
            mapFileLoadSub5(0, -1200.0f, -350.0f, 200.0f, -2000.0f, -350.0f, 600.0f, 0.0f, -175.0f, 900.0f, -100.0f, -175.0f, 1800.0f);
            mapFileLoadSub5(0, 700.0f, -500.0f, -800.0f, 1600.0f, -500.0f, -900.0f, 100.0f, -350.0f, -1200.0f, 200.0f, -350.0f, -2000.0f);
        }
    }
    if (evtWorkScriptContextReturnValue == 5 || evtWorkScriptContextReturnValue == 25)
    {
        mapFileLoadSub5(0, 200.0f, -105.0f, 300.0f, 400.0f, -105.0f, 300.0f, 200.0f, -105.0f, 200.0f, 400.0f, -105.0f, 200.0f);
        mapFileLoadSub5(0, 700.0f, -210.0f, 600.0f, 1000.0f, -210.0f, 0.0f, 0.0f, 0.0f, 400.0f, 0.0f, 0.0f, -700.0f);
        mapFileLoadSub5(0, 900.0f, -385.0f, 1200.0f, 1700.0f, -385.0f, 1200.0f, 700.0f, -210.0f, 600.0f, 1500.0f, -210.0f, 0.0f);
        mapFileLoadSub5(0, 900.0f, -565.0f, 1600.0f, 1700.0f, -565.0f, 1600.0f, 900.0f, -385.0f, 1200.0f, 1700.0f, -385.0f, 1200.0f);
    }
    if (fldGlobalBase.menuOpenFlag == 0)
    {
        mapMapObjectRefresh_fld();
        if (evtWorkScriptContextReturnValue == 31)
        {
            for (i = 0; i < 26; i++)
            {
                uniq = -19712 + i;
                mapAddHukidashi(uniq, 2, 13, 0, -1, 1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
                mapObjVisible(uniq, 0);
                mapObjNoClip(uniq);
                dds3UnitObjectBasicSetOt(mapObjGetBasic(uniq), 8);
            }
            for (i = 0; i < 26; i++)
            {
                if (DDS3_GBWK->pointTable[i].word != 0)
                {
                    uniq = -19712 + i;
                    if (mapObjPointGet2(DDS3_GBWK->pointTable[i].half, &px, &py, &pz, &pr))
                    {
                        mapObjPos(uniq, px, py, pz);
                        mapObjVisible(uniq, 1);
                        basic = mapObjGetBasic(uniq);
                        mapUnitSetLight(basic, mapCallSetPlayerTargetData3(px, pz), 0);
                    }
                }
            }
        }
        if (!evtBitCheck(184) || evtBitCheck(206))
        {
            if (evtWorkScriptContextReturnValue == 2 &&
                (evtWorkScriptContextReturnValue2 == 0 || evtWorkScriptContextReturnValue2 == 2))
            {
                printf("GINZA DENSYA CAR\n");
                mapObjFloorAdd_Cash(0, -16384, 3, 102, 0, -1, 1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
                mapObjNoClip(-16384);
                mapFileLoadSub3(-16384, 13);
                mapFileLoadSub8(48);
                if (fldGlobalBase.rndEncMode == 0)
                {
                    sdfMotionSeek(mdlGetMotionCtrlr(dds3ObjectBaseGetModelHandle(mapObjGetBasic(-16384)), 0), 810.0f);
                }
                mapObjFloorAdd_Cash(0, -16383, 3, 102, 1, -1, 1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
                mapObjNoClip(-16383);
                mapFileLoadSub3(-16383, 14);
                mapFileLoadSub8(48);
                if (fldGlobalBase.rndEncMode == 0)
                {
                    sdfMotionSeek(mdlGetMotionCtrlr(dds3ObjectBaseGetModelHandle(mapObjGetBasic(-16383)), 0), 690.0f);
                }
                mapObjFloorAdd_Cash(0, -16128, 3, 98, 1, -1, 1, 3000.0f, 0.0f, 500.0f, 90.0f, 1.0f);
                mapFileLoadSub3(-16128, 10);
                mapFileLoadSub8(49);
                mapNpcPathCalc_RotGoal(1, mapObjGetBasic(-16128));
                mapFileLoadSub10(1);
                mapFileLoadSub9(1, 12.0f);
                mapObjTaskSub(1, 0);
                mapObjFloorAdd_Cash(0, -16127, 3, 99, 1, -1, 1, 3000.0f, 0.0f, -500.0f, 270.0f, 1.0f);
                mapFileLoadSub3(-16127, 11);
                mapFileLoadSub8(49);
                mapNpcPathCalc_RotGoal(2, mapObjGetBasic(-16127));
                mapFileLoadSub10(2);
                mapFileLoadSub9(2, 12.0f);
                mapObjTaskSub(2, 0);
                mapObjFloorAdd_Cash(0, -16126, 3, 100, 1, -1, 1, 5000.0f, 0.0f, -500.0f, 270.0f, 1.0f);
                mapFileLoadSub3(-16126, 12);
                mapFileLoadSub8(49);
                mapNpcPathCalc_RotGoal(3, mapObjGetBasic(-16126));
                mapFileLoadSub10(3);
                mapFileLoadSub9(3, 12.0f);
                mapObjTaskSub(3, 0);
            }
            if ((!evtBitCheck(177) || evtBitCheck(179)) && evtWorkScriptContextReturnValue == 1 &&
                evtWorkScriptContextReturnValue2 == 0 && (!evtBitCheck(51) || evtBitCheck(52)))
            {
                mapAddHukidashi(-15870, 3, 79, 1, -1, 1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
                mapFileLoadSub3(-15870, 4);
                mapFileLoadSub8(50);
                scrClearProcessGlobals();
                mapNpcPathCalc_RotGoal(0, mapObjGetBasic(-15870));
                mapFileLoadSub10(0);
                dds3ObjectBaseOnFlag(mapObjGetBasic(-15870), 1024);
                dds3ObjectBaseOnFlag(mapObjGetBasic(-15870), 0x80000);
            }
            if (evtWorkScriptContextReturnValue == 4 && evtWorkScriptContextReturnValue2 == 0)
            {
                mapAddHukidashi(-15616, 3, 105, 1, -1, 1, 4200.0f, 0.0f, 6800.0f, 90.0f, 1.0f);
                mapFileLoadSub3(-15616, 6);
                mapObjectRefreshSub(-15615, 3, 105, 1, -1, 4500.0f, 0.0f, 7800.0f, -90.0f, 1.0f);
                mapFileLoadSub3(-15615, 7);
                mapObjectRefreshSub(-15614, 3, 105, 1, -1, 4800.0f, 0.0f, 8800.0f, 90.0f, 1.0f);
                mapFileLoadSub3(-15614, 8);
                mapObjectRefreshSub(-15613, 3, 105, 1, -1, 5100.0f, 0.0f, 9800.0f, -90.0f, 1.0f);
                mapFileLoadSub3(-15613, 9);
            }
        }
        if (evtWorkScriptContextReturnValue == 8 && evtWorkScriptContextReturnValue2 == 2)
        {
            if (fldGlobalBase.amFloor == 2)
            {
                mapAddHukidashi(-12288, 1, 12, 0, 1408, 1, 3200.0f, 50.0f, -2800.0f, 0.0f, 0.55f);
            }
            else
            {
                mapAddHukidashi(-12288, 1, 12, 0, 1408, 1, 3500.0f, 50.0f, -1950.0f, 180.0f, 0.55f);
            }
            mapFileLoadSub3(-12288, 5);
            mapObjNoClip(-12288);
        }
        if (fldGlobalBase.menuOpenFlag == 0 && evtWorkScriptContextReturnValue == 13)
        {
            switch (evtWorkScriptContextReturnValue2)
            {
            case 3:
            case 4:
                mapEXFloor(5);
                mapEXFloor(6);
                break;
            case 5:
                mapEXFloor(3);
                mapEXFloor(6);
                break;
            case 7:
                mapEXFloor(8);
                mapEXFloor(9);
                mapEXFloor(10);
                break;
            case 9:
                mapEXFloor(10);
                mapEXFloor(11);
                break;
            case 10:
                mapEXFloor(9);
                mapEXFloor(11);
                break;
            }
        }
    }
    for (k = 0; k < 8; k++)
    {
        n = mapFileLoadSub4(evtWorkScriptContextReturnValue2, uiTable[k].kind);
        if (gSubFloorID != -1)
        {
            n += mapFileLoadSub4(gSubFloorID, uiTable[k].kind);
        }
        if (n >= 5)
        {
            n = 4;
        }
        for (i = 0; i < n; i++)
        {
            mapAddHukidashi(uiTable[k].uniq + i, 2, uiTable[k].minor, 0, -1, 1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
            mapObjVisible(uiTable[k].uniq + i, 0);
        }
    }
    if (evtWorkScriptContextReturnValue == 1 || evtWorkScriptContextReturnValue == 21)
    {
        mapAddHukidashi(-8192, 2, 32, -1, -1, 1, 0.0f, 4.0f, 0.0f, 0.0f, 1.0f);
        mapObjMask(-8192);
    }
    if ((evtWorkScriptContextReturnValue == 2 &&
         (evtWorkScriptContextReturnValue2 == 0 || evtWorkScriptContextReturnValue2 == 2)) ||
        (evtWorkScriptContextReturnValue == 22 &&
         (evtWorkScriptContextReturnValue2 == 0 || evtWorkScriptContextReturnValue2 == 2)))
    {
        mapAddHukidashi(-8192, 2, 33, -1, -1, 1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        mapObjMask(-8192);
    }
    if (evtWorkScriptContextReturnValue == 8 && (evtWorkScriptContextReturnValue2 == 0 || evtWorkScriptContextReturnValue2 == 1))
    {
        mapAddHukidashi(-8192, 2, 34, -1, -1, 1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        mapObjMask(-8192);
    }
    if (evtWorkScriptContextReturnValue == 13)
    {
        if (evtWorkScriptContextReturnValue2 == 17 || evtWorkScriptContextReturnValue2 == 24 ||
            evtWorkScriptContextReturnValue2 == 25)
        {
            mapObjectRefreshSub(-8192, 2, 35, -1, -1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
            mapObjMask(-8192);
            mapObjectRefreshSub(-8191, 2, 36, -1, -1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
            mapObjMask(-8191);
        }
        if (evtWorkScriptContextReturnValue2 == 3 || evtWorkScriptContextReturnValue2 == 4)
        {
            mapObjectRefreshSub(-8152, 2, 40, -1, -1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
            mapObjMask(-8152);
        }
        if (evtWorkScriptContextReturnValue2 == 9)
        {
            mapObjectRefreshSub(-8151, 2, 41, -1, -1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
            mapObjMask(-8151);
        }
    }
    if (evtWorkScriptContextReturnValue < 44)
    {
        fldGlobalBase.word32C = 0;
        switch (evtWorkScriptContextReturnValue)
        {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 27:
        case 28:
        case 31:
        case 37:
        case 38:
        case 39:
        case 41:
            mapData18 = 0;
            break;
        default:
            mapData18 = 5;
            break;
        }
    }
    mdlCallMemGetSize2();
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_r1_0014F378", mapFileLoad);
#endif

INCLUDE_ASM(const s32, "code_r1_0014F378", mapBtlReLoadMapSBtlSceneSub3);

/* NON_MATCHING: 142 of 147 words match. The tail loads -1 into one register for the gSubFloorID store and the return value; mine uses two. */
#ifdef NON_MATCHING
s32 fldFreeMapData(void)
{
    s32 i;

    mapFileLoadSub2();
    scrClearProcessGlobals();
    mapFreeMapDataSub();
    fldTitleMini_Shutdown();
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
    return gSubFloorID = -1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0014F378", fldFreeMapData);
#endif
/* Battle debug menu tables (.data 0x41D5B0-0x41E858); the code that reads them is still asm. */
dds3ProcessFunc_t sbdebug_procmenu[20] = {
    MainProc,
    ModelSelectProc,
    sbtlCallBtlDebugSetStatusflg,
    MotionChngProc,
    sbtlUnitChngProc,
    sbtlSousaProc,
    sbtlVisualProc,
    sbtlLeaf105,
    sbtlCallGetUnit,
    sbtlVisualProc,
    sbtlOpti,
    sbtlFunc21,
    sbtlLeaf104,
    sbtlLeaf122,
    sbtlLeaf98,
    sbtlCallBtlSearchUnitID31,
    sbtlBSTProc,
    sbtlParaProc,
    sbtlLeaf106,
    NULL
};

dds3ProcessFunc_t sbdebug_drawmenu[20] = {
    sbtlFunc26,
    ModelSelectDraw,
    sbtlFunc15,
    sbtlFRAME,
    sbtlUnitChngDraw,
    sbtlLeaf100,
    sbtlVisualDraw,
    sbtlLeaf102,
    sbtlSkillChangeProc,
    sbtlVisualDraw,
    sbtlRefSbtl67,
    sbtlLeaf119,
    sbtlBattleEffhit,
    sbtlBattleMdlhit,
    sbtlCallBtlDebugDrawNormalListCol,
    sbtlRefSbtl56,
    sbtlBSTDraw,
    sbtlParaDraw,
    sbtlLeaf116,
    NULL
};

sbtlDebugModeFunc_t sbtlDebug_NEFMODE_Func[18] = {
    sbtlNEFMODE_Func_Open,
    sbtlNEFMODE_Func_Save,
    sbtlNEFMODE_Func_Load,
    sbtlNEFMODE_Func_PageList,
    sbtlLeaf108,
    sbtlSizeofData,
    NULL,
    sbtlCopyData,
    NULL,
    sbtlNEFMODE_Func_Edit,
    sbtlBattleHitpath,
    sbtlPacPBFile,
    sbtlBattleBed,
    sbtlCallFileCheckEnd,
    sbtlFunc28,
    sbtlFunc17,
    sbtlCallRestartBattle,
    NULL
};

sbtlDebugModeFunc_t sbtlDebug_NMDLMODE_Func[18] = {
    sbtlMDLMODE_Func_Open,
    sbtlNEFMODE_Func_Save,
    sbtlNEFMODE_Func_Load,
    sbtlMDLMODE_Func_PageList,
    sbtlLeaf108,
    sbtlSizeofData,
    NULL,
    sbtlCopyData,
    NULL,
    sbtlMDLMODE_Func_Edit,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    sbtlCallRestartBattle,
    NULL
};

sbtlDebugModeFunc_t sbtlDebug_NMDLMVMODE_Func[18] = {
    sbtlDebug_MOVEMODE_Func_Open,
    sbtlNEFMODE_Func_Save,
    NULL,
    sbtlMOVEMODE_Func_PageList,
    NULL,
    sbtlLeaf95,
    NULL,
    sbtlCopyData,
    NULL,
    sbtlMOVEMODE_Func_Edit,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    sbtlCallRestartBattle,
    NULL
};

sbtlDebugHitEditorDrawFunc_t sbtlDebug_EffHitEditorDraw_Functbl[4] = {
    sbtlEffHitEditorDraw_Functbl_Cylinder,
    sbtlEffHitEditorDraw_Functbl_Sphere,
    sbtlEffHitEditorDraw_Functbl_Ring,
    NULL
};

sbtlDebugHitEditorDrawFunc_t sbtlDebug_MDLHitEditorDraw_Functbl[4] = {
    sbtlMDLHitEditorDraw_Functbl_Sphere,
    sbtlMDLHitEditorDraw_Functbl_Cylinder,
    sbtlMDLHitEditorDraw_Functbl_Ring,
    NULL
};

char* sbplayer_cmnd[6] = {
    sbtlStrPlayer1,
    sbtlStrPlayer2,
    sbtlStrPlayer3,
    sbtlStrPlayerAll,
    sbtlStrMahoujin,
    NULL
};

char* sblist_optimenu[26] = {
    sbtlStrEffectEditor,
    sbtlStrAllDead,
    sbtlRefSbtl67DataEntry7,
    sbtlStrEffhitEditor,
    sbtlStrModelhitEditor,
    sbtlStrModelmoveEditor,
    sbtlStrCameraMove,
    sbtlStrCameraLock,
    sbtlStrInterface,
    sbtlRefSbtl67DataEntry6,
    sbtlRefSbtl67DataEntry5,
    sbtlRefSbtl67DataEntry4,
    sbtlRefSbtl67DataEntry3,
    sbtlStrEnemyStop,
    sbtlRefSbtl67DataEntry2,
    sbtlStrNoResult,
    sbtlStrCombination,
    sbtlStrFaceIcon,
    sbtlStrCmndPanel,
    sbtlStrDamageS,
    sbtlStrBststDraw,
    sbtlStrSeDistance,
    sbtlStrSeDouji,
    sbtlStrSeDebug,
    sbtlRefSbtl67DataEntry,
    NULL
};

SBDEBUG_FLGWORK sbflg_optimenu[25] = {
    { 1, SBDEBUG_MODE_EFFEDITOR, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_NONE, 0 },
    { 3, SBDEBUG_MODE_OPTION, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_MODELSEL, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_EFFHITE, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_MODELHITE, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_MODELMOVE, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_NONE, 0 },
    { 0, SBDEBUG_MODE_OPTION, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_ALL, 2 },
    { 0, SBDEBUG_MODE_OPTION, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_ALL, 0x80 },
    { 0, SBDEBUG_MODE_OPTION, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_ALL, 0x200 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 4 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x4000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x8000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x10000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x40000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x100000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x4000000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x1000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x2000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_YALL, 2 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x800000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x2000000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x10000000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x20000000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x40000000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_YALL, 1 }
};

char* sblist_setmenu_00[2] __attribute__((section(".data"))) = {
    sbtlStrDebugMenuKari,
    NULL
};

char* sblist_setmenu_01[12] = {
    sbtlStrEncountNo,
    sbtlStrBattleMode,
    sbtlStrCameraMove,
    sbtlStrCameraLock,
    sbtlStrInterfaceOff,
    sbtlRefSbtl67DataEntry6,
    sbtlRefSbtl67DataEntry5,
    sbtlRefSbtl67DataEntry4,
    sbtlRefSbtl67DataEntry3,
    sbtlStrEnemyStop,
    sbtlRefSbtl67DataEntry2,
    NULL
};

SBDEBUG_FLGWORK sbflg_setmenu[11] = {
    { 2, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ECNT, 8 },
    { 2, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_BMODE, 8 },
    { 0, SBDEBUG_MODE_OPTION, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_ALL, 2 },
    { 0, SBDEBUG_MODE_OPTION, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_ALL, 0x80 },
    { 0, SBDEBUG_MODE_OPTION, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_ALL, 0x200 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 4 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x4000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x8000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x10000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x40000 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x100000 }
};

char* sblist_paramenu[12] = {
    sbtlParaDrawDataEntry10,
    sbtlParaDrawDataEntry9,
    sbtlParaDrawDataEntry8,
    sbtlParaDrawDataEntry7,
    sbtlParaDrawDataEntry6,
    sbtlParaDrawDataEntry5,
    sbtlParaDrawDataEntry4,
    sbtlParaDrawDataEntry3,
    sbtlParaDrawDataEntry2,
    sbtlParaDrawDataEntry,
    sbtlRefSbtl67DataEntry4,
    NULL
};

char* sblist_panel[22] = {
    sbtlFunc26DataEntry5,
    sbtlStrHitDrawMode,
    sbtlStrUnitAreaHit,
    sbtlStrUnitAreaPriv,
    sbtlStrUnitAreaFrid,
    sbtlStrUnitAreaEnmy,
    sbtlRefSbtl67DataEntry7,
    sbtlStrCameraMove,
    sbtlStrCameraLock,
    sbtlStrAiPanel,
    sbtlRefSbtl67DataEntry6,
    sbtlStrDanganNumChk,
    sbtlFunc26DataEntry4,
    sbtlFunc26DataEntry3,
    sbtlStrMashingun,
    sbtlFunc26DataEntry2,
    sbtlStrEquipGunNo,
    sbtlStrEquipSordno,
    sbtlFunc26DataEntry,
    sbtlStrPositionReset,
    sbtlStrParaReset,
    sbtlStrEncountNo2
};

SBDEBUG_FLGWORK sbflg_panel[22] = {
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_UNIT, 1 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x40 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_UNIT, 2 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_UNIT, 4 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_UNIT, 8 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_UNIT, 0x10 },
    { 1, SBDEBUG_MODE_MODELSEL, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_NONE, 0 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 2 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 0x80 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_UNIT, 0x20 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 4 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ALL, 8 },
    { 2, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_SEIKAKU, 8 },
    { 2, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_HGUN, 8 },
    { 2, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_MGUN, 8 },
    { 0, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_YALL, 4 },
    { 2, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_EQPG, 8 },
    { 2, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_EQPS, 8 },
    { 2, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_MAG, 8 },
    { 1, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_NONE, 0 },
    { 2, SBDEBUG_MODE_MAIN, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_ECNT, 8 }
};

SBDEBUG_FLGWORK sbflg_mdlselect[2] = {
    { 1, SBDEBUG_MODE_MODELMENU, SBDEBUG_MODE_MAIN, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_MODELMENU, SBDEBUG_MODE_OPTION, SBDEBUG_FLGTYPE_NONE, 0 }
};

char* sblist_modelmenu[10] = {
    sbtlStr18,
    sbtlStr17,
    sbtlStr16,
    sbtlStr15,
    sbtlStr14,
    sbtlStr13,
    sbtlStrAi,
    sbtlStr12,
    sbtlStr11,
    NULL
};

char* sblist_mdlmenupanel[4] = {
    sbtlStrMotionChange,
    sbtlStrUnitChange,
    sbtlLeaf114DataEntry2,
    sbtlLeaf114DataEntry
};

SBDEBUG_FLGWORK sbflg_mdlmenupanel[9] = {
    { 1, SBDEBUG_MODE_MOTIONCHNG, SBDEBUG_MODE_MODELSEL, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_UNITCHNG, SBDEBUG_MODE_MODELSEL, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_SOUSA, SBDEBUG_MODE_MODELSEL, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_VISUAL, SBDEBUG_MODE_MODELSEL, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_SKILLCHANGE, SBDEBUG_MODE_MODELSEL, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_EFFCYLINDER, SBDEBUG_MODE_MODELSEL, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_AI, SBDEBUG_MODE_MODELSEL, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_BST_K, SBDEBUG_MODE_MODELSEL, SBDEBUG_FLGTYPE_NONE, 0 },
    { 1, SBDEBUG_MODE_PARA, SBDEBUG_MODE_MODELSEL, SBDEBUG_FLGTYPE_NONE, 0 }
};

char* sbtlCallDestroyFontRequestAfter2Data[28] = {
    sbtlStr00Loop,
    sbtlStr01Loop,
    sbtlStr02Loop,
    sbtlStr03Reserve,
    sbtlStr04,
    sbtlStr05,
    sbtlStr06,
    sbtlCallDestroyFontRequestAfter2Data3Entry4,
    sbtlStr08,
    sbtlStr09,
    sbtlStr102,
    sbtlStr112,
    sbtlCallDestroyFontRequestAfter2Data3Entry3,
    sbtlCallDestroyFontRequestAfter2Data3Entry2,
    sbtlCallDestroyFontRequestAfter2Data3Entry,
    sbtlStr152,
    sbtlStr16Reserve,
    sbtlStr17Reserve,
    sbtlStr18Reserve,
    sbtlStr19Reserve,
    sbtlStr20Reserve,
    sbtlStr21Reserve,
    sbtlStr22Reserve,
    sbtlStr23Reserve,
    sbtlStr24Reserve,
    sbtlStr25Reserve,
    sbtlStr26Reserve,
    NULL
};

char* sbtlCallDestroyFontRequestAfter2Data2[28] = {
    sbtlCallDestroyFontRequestAfter2Data4Entry,
    sbtlStr01,
    sbtlStr02,
    sbtlStr03,
    sbtlCallDestroyFontRequestAfter2Data2Entry15,
    sbtlCallDestroyFontRequestAfter2Data2Entry14,
    sbtlCallDestroyFontRequestAfter2Data2Entry13,
    sbtlCallDestroyFontRequestAfter2Data2Entry12,
    sbtlCallDestroyFontRequestAfter2Data2Entry11,
    sbtlCallDestroyFontRequestAfter2Data2Entry10,
    sbtlCallDestroyFontRequestAfter2Data2Entry9,
    sbtlStr113,
    sbtlStr122,
    sbtlStr132,
    sbtlStr142,
    sbtlStr153,
    sbtlStr162,
    sbtlStr172,
    sbtlStr182,
    sbtlCallDestroyFontRequestAfter2Data2Entry8,
    sbtlCallDestroyFontRequestAfter2Data2Entry7,
    sbtlCallDestroyFontRequestAfter2Data2Entry6,
    sbtlCallDestroyFontRequestAfter2Data2Entry5,
    sbtlCallDestroyFontRequestAfter2Data2Entry4,
    sbtlCallDestroyFontRequestAfter2Data2Entry3,
    sbtlCallDestroyFontRequestAfter2Data2Entry2,
    sbtlCallDestroyFontRequestAfter2Data2Entry,
    NULL
};

char* sbtlCallDestroyFontRequestAfter2Data3[28] = {
    sbtlStr00Loop,
    sbtlStr01Loop,
    sbtlStr02Loop,
    sbtlStr03Reserve,
    sbtlStr04,
    sbtlStr05,
    sbtlStr06,
    sbtlCallDestroyFontRequestAfter2Data3Entry4,
    sbtlStr08,
    sbtlStr09,
    sbtlStr102,
    sbtlStr112,
    sbtlCallDestroyFontRequestAfter2Data3Entry3,
    sbtlCallDestroyFontRequestAfter2Data3Entry2,
    sbtlCallDestroyFontRequestAfter2Data3Entry,
    sbtlStr152,
    sbtlStr16Reserve,
    sbtlStr17Reserve,
    sbtlStr18Reserve,
    sbtlStr19Reserve,
    sbtlStr20Reserve,
    sbtlStr21Reserve,
    sbtlStr22Reserve,
    sbtlStr23Reserve,
    sbtlStr24Reserve,
    sbtlStr25Reserve,
    sbtlStr26Reserve,
    NULL
};

char* sbtlCallDestroyFontRequestAfter2Data4[40] = {
    sbtlCallDestroyFontRequestAfter2Data4Entry,
    sbtlStr012,
    sbtlStr022,
    sbtlStr032,
    sbtlStr042,
    sbtlStr052,
    sbtlStr06Reserve,
    sbtlStr07,
    sbtlStr082,
    sbtlStr092,
    sbtlStr10Reserve,
    sbtlStr114,
    sbtlCallDestroyFontRequestAfter2Data4Entry5,
    sbtlStr133,
    sbtlStr14Reserve,
    sbtlStr154,
    sbtlCallDestroyFontRequestAfter2Data4Entry4,
    sbtlCallDestroyFontRequestAfter2Data4Entry3,
    sbtlStr183,
    sbtlStr19,
    sbtlStr20,
    sbtlStr21,
    sbtlStr22,
    sbtlStr23,
    sbtlStr24,
    sbtlStr25,
    sbtlCallDestroyFontRequestAfter2Data4Entry2,
    sbtlStr27,
    sbtlStr28,
    sbtlStr29,
    sbtlStr30,
    sbtlStr31,
    sbtlStr32,
    sbtlStr33,
    sbtlStr34,
    sbtlStr35,
    sbtlStr36,
    sbtlStr37,
    sbtlStr38,
    sbtlStr39
};

char* sblist_visualmenu00[4] = {
    sbtlVisualDrawDataEntry4,
    sbtlVisualDrawDataEntry3,
    sbtlVisualDrawDataEntry2,
    sbtlVisualDrawDataEntry
};

char* sblist_visualmenu01[4] = {
    sbtlVisualDrawData2Entry4,
    sbtlVisualDrawData2Entry3,
    sbtlVisualDrawData2Entry2,
    sbtlVisualDrawData2Entry
};

char* sblist_visualmenu02[4] = {
    sbtlStrCylinder,
    sbtlVisualDrawData5Entry2,
    sbtlVisualDrawData5Entry,
    NULL
};

char* sblist_visualmenu01_e[4] = {
    sbtlStrECenter,
    sbtlVisualDrawData2Entry3,
    sbtlVisualDrawData2Entry2,
    sbtlVisualDrawData2Entry
};

char* sblist_visualmenu02_e[4] = {
    sbtlStrECylinder,
    sbtlVisualDrawData5Entry2,
    sbtlVisualDrawData5Entry,
    NULL
};

char* sblist_visualmenu03[4] = {
    sbtlStrDefMSpd,
    sbtlStrDefRSpd,
    sbtlStrDefMvspd,
    sbtlVisualDrawData6Entry
};

char* sblist_visualmenu04[12] = {
    sbtlStrMotionPara,
    sbtlVisualDrawData7Entry2,
    sbtlEditDrawData3Entry,
    sbtlStrActionFrm,
    sbtlStrStopFrm,
    sbtlStrEndMotion,
    sbtlStrMotionSpd,
    sbtlStrHitRadian,
    sbtlStrHokanFrm,
    sbtlStrCancelFrm,
    sbtlVisualDrawData7Entry,
    NULL
};

char* sblist_visualmenu05[2] __attribute__((section(".data"))) = {
    sbtlVisualDrawData8Entry2,
    sbtlVisualDrawData8Entry
};

char* sblist_visualmenu06[2] __attribute__((section(".data"))) = {
    sbtlVisualDrawData9Entry,
    NULL
};

char* sblist_visualmenu06_0[2] __attribute__((section(".data"))) = {
    sbtlStrEffcySave,
    NULL
};

u16 motion_flg[2] __attribute__((section(".data"))) = {
    MOTION_FLG_SPEED_ON, MOTION_FLG_NCANCEL
};

char* sbtlData20[34] = {
    sbtlData22Entry5,
    sbtlData22Entry4,
    sbtlData22Entry3,
    sbtlData20Entry10,
    sbtlData20Entry9,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData22Entry2,
    sbtlData20Entry7,
    sbtlData20Entry6,
    sbtlData22Entry,
    sbtlData20Entry5,
    sbtlData20Entry4,
    sbtlData20Entry3,
    sbtlData20Entry2,
    sbtlData20Entry,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    sbtlData20Entry8,
    NULL
};

char* sbtlData21[28] = {
    sbtlData22Entry5,
    sbtlData22Entry2,
    sbtlData22Entry3,
    sbtlData22Entry9,
    sbtlData22Entry4,
    sbtlData22Entry,
    sbtlData20Entry10,
    sbtlData21Entry7,
    sbtlData21Entry6,
    sbtlData21Entry5,
    sbtlData21Entry4,
    sbtlData21Entry3,
    sbtlData22Entry8,
    sbtlData22Entry7,
    sbtlData22Entry6,
    sbtlData21Entry2,
    sbtlData22Entry9,
    sbtlData21Entry,
    sbtlData21Entry,
    sbtlData21Entry,
    sbtlData21Entry,
    sbtlData21Entry,
    sbtlData21Entry,
    sbtlData21Entry,
    sbtlData21Entry,
    sbtlData21Entry,
    sbtlData21Entry,
    NULL
};

char* sbtlData22[44] = {
    sbtlData22Entry5,
    sbtlData22Entry2,
    sbtlData22Entry3,
    sbtlData22Entry38,
    sbtlData22Entry4,
    sbtlData22Entry,
    sbtlData22Entry9,
    sbtlData22Entry37,
    sbtlData22Entry36,
    sbtlData22Entry35,
    sbtlData22Entry9,
    sbtlData22Entry34,
    sbtlData22Entry8,
    sbtlData22Entry33,
    sbtlData22Entry9,
    sbtlData22Entry32,
    sbtlData22Entry7,
    sbtlData22Entry6,
    sbtlData22Entry31,
    sbtlData22Entry30,
    sbtlData22Entry29,
    sbtlData22Entry28,
    sbtlData22Entry27,
    sbtlData22Entry26,
    sbtlData22Entry25,
    sbtlData22Entry24,
    sbtlData22Entry23,
    sbtlData22Entry22,
    sbtlData22Entry21,
    sbtlData22Entry20,
    sbtlData22Entry19,
    sbtlData22Entry18,
    sbtlData22Entry17,
    sbtlData22Entry16,
    sbtlData22Entry15,
    sbtlData22Entry14,
    sbtlData22Entry13,
    sbtlData22Entry12,
    sbtlData22Entry11,
    sbtlData22Entry10,
    sbtlData22Entry9,
    sbtlData22Entry9,
    sbtlData22Entry9,
    NULL
};

char* sbtlMotionDataSaveData[64] = {
    sbtlStrTCYV,
    sbtlStrXP,
    sbtlStr40,
    sbtlStrSV,
    sbtlStrGFN,
    sbtlStrAO,
    sbtlStr26,
    sbtlStrVA,
    sbtlStrOJEffectMotionIVLZ,
    sbtlStr000,
    sbtlStr001W,
    sbtlStr002X,
    sbtlStr003U,
    sbtlStr0042,
    sbtlMotionDataSaveData2Entry14,
    sbtlMotionDataSaveData2Entry13,
    sbtlMotionDataSaveData2Entry12,
    sbtlMotionDataSaveData2Entry11,
    sbtlStr009,
    sbtlStr004,
    sbtlStr002,
    sbtlStr00E,
    sbtlStr00EA,
    sbtlMotionDataSaveData2Entry10,
    sbtlStr00,
    sbtlStr010AE,
    sbtlStr011C,
    sbtlStr012C,
    sbtlMotionDataSaveData2Entry9,
    sbtlMotionDataSaveData2Entry8,
    sbtlMotionDataSaveData2Entry7,
    sbtlMotionDataSaveData2Entry6,
    sbtlMotionDataSaveData2Entry5,
    sbtlMotionDataSaveData2Entry4,
    sbtlMotionDataSaveData2Entry3,
    sbtlMotionDataSaveData2Entry2,
    sbtlMotionDataSaveData2Entry,
    sbtlStr00000Loop,
    sbtlStr01001Loop,
    sbtlStr02002XLoop,
    sbtlStr03003Reserve,
    sbtlStr04004W,
    sbtlStr05005W,
    sbtlStr06006AN,
    sbtlStr07007,
    sbtlStr08008X,
    sbtlStr09009,
    sbtlStr1000,
    sbtlStr1100K,
    sbtlStr1200S,
    sbtlStr1300,
    sbtlStr1400A,
    sbtlStr1500,
    sbtlStr16010Reserve,
    sbtlStr17011Reserve,
    sbtlStr18012Reserve,
    sbtlStr19013Reserve,
    sbtlStr20014Reserve,
    sbtlStr21015Reserve,
    sbtlStr22016Reserve,
    sbtlStr23017Reserve,
    sbtlStr24018Reserve,
    sbtlStr25019Reserve,
    sbtlStr2601Reserve
};

char* sbtlMotionMoveDataData[2] __attribute__((section(".data"))) = {
    sbtlStrVJI,
    NULL
};

char* sbtlMotionDataSaveData2[82] = {
    sbtlStrTCYV,
    sbtlStrXP,
    sbtlStr40,
    sbtlStrSV,
    sbtlStrGFN,
    sbtlStrAO,
    sbtlStr26,
    sbtlStrVA,
    sbtlStrOJEffectMotionIVLZ,
    sbtlStr000,
    sbtlStr001W,
    sbtlStr002X,
    sbtlStr003U,
    sbtlStr0042,
    sbtlMotionDataSaveData2Entry14,
    sbtlMotionDataSaveData2Entry13,
    sbtlMotionDataSaveData2Entry12,
    sbtlMotionDataSaveData2Entry11,
    sbtlStr009,
    sbtlStr004,
    sbtlStr002,
    sbtlStr00E,
    sbtlStr00EA,
    sbtlMotionDataSaveData2Entry10,
    sbtlStr00,
    sbtlStr010AE,
    sbtlStr011C,
    sbtlStr012C,
    sbtlMotionDataSaveData2Entry9,
    sbtlMotionDataSaveData2Entry8,
    sbtlMotionDataSaveData2Entry7,
    sbtlMotionDataSaveData2Entry6,
    sbtlMotionDataSaveData2Entry5,
    sbtlMotionDataSaveData2Entry4,
    sbtlMotionDataSaveData2Entry3,
    sbtlMotionDataSaveData2Entry2,
    sbtlMotionDataSaveData2Entry,
    sbtlStr000,
    sbtlStr001,
    sbtlStr002XX,
    sbtlStr003XX,
    sbtlStr004W,
    sbtlStr005W,
    sbtlStr006,
    sbtlStr007,
    sbtlStr008,
    sbtlStr0092,
    sbtlMotionDataSaveData2Entry19,
    sbtlStr00BK,
    sbtlStr00C,
    sbtlStr00DI,
    sbtlMotionDataSaveData2Entry18,
    sbtlStr00FAC,
    sbtlStr010,
    sbtlStr011A,
    sbtlStr0122,
    sbtlStr013,
    sbtlStr014RAPX,
    sbtlStr015RBAY,
    sbtlStr016,
    sbtlStr017C,
    sbtlStr018C,
    sbtlStr019C,
    sbtlStr01A,
    sbtlStr01B1,
    sbtlStr01C2,
    sbtlStr01D3,
    sbtlStr01EZ,
    sbtlStr01F,
    sbtlStr020,
    sbtlStr021,
    sbtlStr022RVU,
    sbtlStr023K,
    sbtlStr024K,
    sbtlStr025K,
    sbtlStr026K,
    sbtlStr027K,
    sbtlMotionDataSaveData2Entry17,
    sbtlMotionDataSaveData2Entry16,
    sbtlMotionDataSaveData2Entry15,
    sbtlStrVJI,
    NULL
};

char* sblist_efhitmenu_open_move[6] = {
    sbtlStrSave,
    sbtlStrPosReset,
    sbtlStrModelchange,
    sbtlData24Entry3,
    sbtlData24Entry2,
    sbtlData24Entry
};

char* sblist_efhitmenu_open_mdl[6] = {
    sbtlStrSaveMht,
    sbtlStrNew,
    sbtlStrModelchange,
    sbtlData24Entry3,
    sbtlData24Entry2,
    sbtlData24Entry
};

char* sblist_efhitmenu_open[6] = {
    sbtlStrSaveEht,
    sbtlStrLoadEht,
    sbtlStrNew,
    sbtlData24Entry2,
    sbtlData24Entry,
    NULL
};

char* sblist_efhitmenu_page_00[2] __attribute__((section(".data"))) = {
    sbtlStrPageList,
    NULL
};

char* sblist_efhitmenu_page_01[16] = {
    sbtlStrPage01,
    sbtlStrPage02,
    sbtlStrPage03,
    sbtlStrPage04,
    sbtlStrPage05,
    sbtlStrPage06,
    sbtlStrPage07,
    sbtlStrPage08,
    sbtlStrPage09,
    sbtlStrPage10,
    sbtlStrPage11,
    sbtlStrPage12,
    sbtlStrPage13,
    sbtlStrPage14,
    sbtlStrPage15,
    sbtlStrPage16
};

char* sblist_efhitmenu_page_02[2] __attribute__((section(".data"))) = {
    sbtlStrNewPage,
    NULL
};

char* sblist_efhitmenu_selmenu[4] = {
    sbtlStrEdit,
    sbtlStrCopy,
    sbtlStrDelete,
    NULL
};

char* sblist_efhitmenu_delok_00[2] __attribute__((section(".data"))) = {
    sbtlStrDeleteOk,
    NULL
};

char* sblist_efhitmenu_exitok_00[2] __attribute__((section(".data"))) = {
    sbtlStrExitOk,
    NULL
};

char* sblist_efhitmenu_delok_01[2] __attribute__((section(".data"))) = {
    sbtlData29Entry2,
    sbtlData29Entry
};

char* sblist_efhitmenu_hittype[4] = {
    sbtlData30Entry2,
    sbtlStrCylinder2,
    sbtlData30Entry,
    NULL
};

char* sblist_efhitmenu_edit_page[16] = {
    sbtlData31Entry16,
    sbtlData31Entry15,
    sbtlData31Entry14,
    sbtlData31Entry13,
    sbtlData31Entry12,
    sbtlData31Entry11,
    sbtlData31Entry10,
    sbtlData31Entry9,
    sbtlData31Entry8,
    sbtlData31Entry7,
    sbtlData31Entry6,
    sbtlData31Entry5,
    sbtlData31Entry4,
    sbtlData31Entry3,
    sbtlData31Entry2,
    sbtlData31Entry
};

char* sblist_efhitmenu_edit_para00[4] = {
    sbtlVisualDrawData5Entry2,
    sbtlEffHitEditorDraw_Functbl_CylinderDataEntry2,
    sbtlEffHitEditorDraw_Functbl_CylinderDataEntry,
    sbtlEditDraw_MOVEDataEntry
};

char* sblist_efhitmenu_edit_para00_0[4] = {
    sbtlVisualDrawData5Entry2,
    sbtlVisualDrawData5Entry,
    sbtlEffHitEditorDraw_Functbl_CylinderDataEntry,
    sbtlEditDraw_MOVEDataEntry
};

char* sblist_efhitmenu_edit_para00_1[6] = {
    sbtlVisualDrawData5Entry2,
    sbtlEffHitEditorDraw_Functbl_RingDataEntry,
    sbtlVisualDrawData5Entry,
    sbtlEffHitEditorDraw_Functbl_CylinderDataEntry,
    sbtlEditDraw_MOVEDataEntry,
    NULL
};

char* sblist_mdlhitmenu_edit_para00[4] = {
    sbtlVisualDrawData5Entry2,
    sbtlEffHitEditorDraw_Functbl_CylinderDataEntry2,
    sbtlEffHitEditorDraw_Functbl_CylinderDataEntry,
    sbtlEditDraw_MOVEDataEntry
};

char* sblist_mdlhitmenu_edit_para00_0[4] = {
    sbtlVisualDrawData5Entry2,
    sbtlVisualDrawData5Entry,
    sbtlEffHitEditorDraw_Functbl_CylinderDataEntry,
    sbtlEditDraw_MOVEDataEntry
};

char* sblist_mdlhitmenu_edit_para00_1[6] = {
    sbtlVisualDrawData5Entry2,
    sbtlEffHitEditorDraw_Functbl_RingDataEntry,
    sbtlVisualDrawData5Entry,
    sbtlEffHitEditorDraw_Functbl_CylinderDataEntry,
    sbtlEditDraw_MOVEDataEntry,
    NULL
};

char* sblist_mdlhitmenu_edit_para02[2] __attribute__((section(".data"))) = {
    sbtlEditDrawData3Entry2,
    sbtlData35Entry
};

char* sblist_mdlhitmenu_edit_para03[2] __attribute__((section(".data"))) = {
    sbtlStrMtionno,
    NULL
};

char* sblist_efhitmenu_edit_para01[2] __attribute__((section(".data"))) = {
    sbtlStrPoseture,
    NULL
};

char* sblist_efhitmenu_edit_para01_1[4] = {
    sbtlData34Entry3,
    sbtlData34Entry2,
    sbtlData34Entry,
    NULL
};

char* sblist_mdlmovemenu_edit_para00[4] = {
    sbtlEditDraw_MOVEDataEntry,
    sbtlEditDraw_MOVEDataEntry2,
    sbtlStrPositionReset,
    NULL
};

char* sblist_efhitmenu_edit_para02[6] = {
    sbtlEditDrawData3Entry2,
    sbtlData35Entry4,
    sbtlEditDraw_MOVEDataEntry2,
    sbtlData35Entry3,
    sbtlStrGravity,
    sbtlData35Entry2
};

char* sblist_efhitmenu_edit_para02_0[8] = {
    sbtlEditDrawData3Entry2,
    sbtlData35Entry,
    sbtlData35Entry4,
    sbtlEditDraw_MOVEDataEntry2,
    sbtlData35Entry3,
    sbtlStrGravity,
    sbtlData35Entry2,
    NULL
};

char* sblist_efhitmenu_edit_para03[2] __attribute__((section(".data"))) = {
    sbtlStrBehaviorType,
    NULL
};

char* sblist_efhitmenu_edit_para03_0[2] __attribute__((section(".data"))) = {
    sbtlData36Entry,
    sbtlStrPathPb
};

char* sblist_efhitmenu_edit_para03_1[2] __attribute__((section(".data"))) = {
    sbtlStrBezier,
    sbtlDebug_HitTypeMoveDrawDataEntry
};

char* sblist_efhitmenu_edit_para04[6] = {
    sbtlStrAttachEpl,
    sbtlDebug_EPLParaDrawDataEntry3,
    sbtlEditDraw_MOVEDataEntry,
    sbtlDebug_EPLParaDrawDataEntry2,
    sbtlDebug_EPLParaDrawDataEntry,
    NULL
};

char* sblist_efhitmenu_edit_para05_00[2] __attribute__((section(".data"))) = {
    sbtlStrEplMenu,
    NULL
};

char* sblist_efhitmenu_edit_para05_01[2] __attribute__((section(".data"))) = {
    sbtlCallBtlDebugDrawNormalListCol4Data2Entry2,
    sbtlCallBtlDebugDrawNormalListCol4Data2Entry
};

char* sblist_efhitmenu_edit_para06_00[22] = {
    sbtlStrAll,
    sbtlStrInvork,
    sbtlStrTarget,
    sbtlStrWorld,
    sbtlStrIparty,
    sbtlStrTparty,
    sbtlStrCombia,
    sbtlStrCombib,
    sbtlStrIxT,
    sbtlStrAllH,
    sbtlStrInvorkH,
    sbtlStrTargetH,
    sbtlStrWorldH,
    sbtlStrIpartyH,
    sbtlStrTpartyH,
    sbtlStrCombiaH,
    sbtlStrCombibH,
    sbtlStrIxTH,
    sbtlData37Entry2,
    sbtlData37Entry,
    sbtlCallBtlDebugDrawNormalListCol8DataEntry,
    sbtlStrHittype
};

char* sblist_effhitemenu01[2] __attribute__((section(".data"))) = {
    sbtlEditDrawData3Entry,
    NULL
};

char* sblist_effhitemenu02[18] = {
    nbUnitBadStatusNamePara,
    sbtlStrStTime,
    sbtlEditDraw_MOVEDataEntry,
    sbtlEditDrawData3Entry8,
    sbtlEditDrawData3Entry7,
    sbtlStrDirFlg,
    sbtlEditDrawData3Entry2,
    sbtlEditDrawData3Entry6,
    sbtlEditDrawData3Entry5,
    sbtlVisualDrawData5Entry2,
    sbtlEditDrawData3Entry4,
    sbtlStrKakudoRl,
    sbtlStrHitbkLife,
    sbtlStrHitbkPara,
    sbtlEditDrawData3Entry3,
    sbtlStrKasokudo,
    sbtlStrHomingpara,
    NULL
};

char* sblist_effhitemenu03[6] = {
    sbtlStrStartPos,
    sbtlStrPosition,
    sbtlStrHelperno,
    sbtlVisualDrawData5Entry,
    sbtlData38Entry,
    NULL
};

char* sblist_effhitemenu03_0[6] = {
    sbtlStrTargetpos,
    sbtlStrPosition,
    sbtlStrHelperno,
    sbtlVisualDrawData5Entry,
    sbtlData38Entry,
    NULL
};

char* sblist_effhitemenu03_a1[6] = {
    sbtlStrInterpo1,
    sbtlData40Entry3,
    sbtlData40Entry2,
    sbtlVisualDrawData5Entry,
    sbtlData40Entry,
    NULL
};

char* sblist_effhitemenu03_a2[6] = {
    sbtlStrInterpo2,
    sbtlData40Entry3,
    sbtlData40Entry2,
    sbtlVisualDrawData5Entry,
    sbtlData40Entry,
    NULL
};

char* sblist_effhitemenu04[6] = {
    sblist_effhitemenu03_a1Entry,
    sbtlStrPosition,
    sbtlStrHelperno,
    sbtlVisualDrawData5Entry,
    sbtlData38Entry,
    NULL
};

char* sblist_effhitemenu05[2] __attribute__((section(".data"))) = {
    sblist_effhitemenu03_a1Entry3,
    sblist_effhitemenu03_a1Entry2
};

s32 sbtlData42[14] = {
    SBTLDMG_HITFLG_ACTER, SBTLDMG_HITFLG_HITCLR, SBTLDMG_HITFLG_OLDON, SBTLDMG_HITFLG_NHITOFF, SBTLDMG_HITFLG_NONSTOP, SBTLDMG_HITFLG_IDOUON, SBTLDMG_HITFLG_EFFCLR, SBTLDMG_HITFLG_HOMING, SBTLDMG_HITFLG_GROUND, SBTLDMG_HITFLG_I_STTIM, SBTLDMG_HITFLG_MUGEN, SBTLDMG_HITFLG_ACTERAT, SBTLDMG_HITFLG_LOOP, 0
};
INCLUDE_SDATA(const s32, "code_r1_0014F378", fldFreeMapDataData);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapFileLoadData7);

INCLUDE_SDATA(const s32, "code_r1_0014F378", pFldMap);

INCLUDE_SDATA(const s32, "code_r1_0014F378", pMapAreaNow);

INCLUDE_SDATA(const s32, "code_r1_0014F378", pMapAreaDat);

INCLUDE_SDATA(const s32, "code_r1_0014F378", pMapColmNow);

INCLUDE_SDATA(const s32, "code_r1_0014F378", pMapColmDat);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaRefreshData);

INCLUDE_SDATA(const s32, "code_r1_0014F378", fldData25);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaRefreshData2);

INCLUDE_SDATA(const s32, "code_r1_0014F378", fldData26);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaRefreshData3);

INCLUDE_SDATA(const s32, "code_r1_0014F378", fldData27);

INCLUDE_SDATA(const s32, "code_r1_0014F378", pMapObjNow);

INCLUDE_SDATA(const s32, "code_r1_0014F378", pMapObjSub);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaRefreshData4);

INCLUDE_SDATA(const s32, "code_r1_0014F378", fldData28);

INCLUDE_SDATA(const s32, "code_r1_0014F378", pMapEveHitNow);

INCLUDE_SDATA(const s32, "code_r1_0014F378", fldData29);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData99);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapFileLoadData8);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData100);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaRefreshData5);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData101);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaRefreshData6);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaRefreshData7);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData102);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaData17);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData103);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData104);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData105);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapFileLoadData9);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapFileLoadData10);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData106);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData107);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData108);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData109);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData110);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapFileLoadData11);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData111);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaData18);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaData19);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaData20);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaData21);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData112);

INCLUDE_SDATA(const s32, "code_r1_0014F378", fldFreeMapDataData2);

INCLUDE_SDATA(const s32, "code_r1_0014F378", pMpbPointNow);

INCLUDE_SDATA(const s32, "code_r1_0014F378", fldFreeMapDataData3);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData113);

INCLUDE_SDATA(const s32, "code_r1_0014F378", fldFreeMapDataData4);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaData22);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapMapSetAreaData23);

INCLUDE_SDATA(const s32, "code_r1_0014F378", gSubFloorID);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData114);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapData115);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapNpcTblRefresh_WaitData);

INCLUDE_SDATA(const s32, "code_r1_0014F378", mapNpcTblRefresh_WaitData2);

