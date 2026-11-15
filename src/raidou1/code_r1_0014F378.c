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
    s32 *extra;
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
        mapMapSetAreaData2[i] = (u8 *)map + area->colmOfs;
        mapMapSetAreaData3[i] = (u8 *)map + area->hit64Ofs;
        mapMapSetAreaData4[i] = (u8 *)map + area->pointOfs;
        mapMapSetAreaData5[i] = (u8 *)map + area->hit20Ofs;
        mapMapSetAreaData6[i] = (u8 *)map + area->objOfs;
        mapMapSetAreaData7[i] = (u8 *)map + area->kaidanOfs;
        mapMapSetAreaData8[i] = (u8 *)map + area->eveHitOfs;
        if (area->extraOfs != 0)
        {
            mapCallSetOt2Data2[i] = (s32 *)((u8 *)map + area->extraOfs);
        }
        extra = mapCallSetOt2Data2[i];
        if (extra != NULL)
        {
            mapData36[i] = (u8 *)map + extra[1];
            mapData35[i] = (u8 *)map + extra[0];
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
            mapMapSetAreaData15[i] = (u8 *)sub4 + sub4->entry[i].ofs0;
            mapFileLoadData6[i] = (u8 *)sub4 + sub4->entry[i].ofs1;
            mapMapSetAreaData16[i] = (u8 *)sub4 + sub4->entry[i].ofs2;
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
        if (mapFileLoadSub7())
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
                if (fldGlobalBase.word1E8 == 0)
                {
                    sdfMotionSeek(mdlGetMotionCtrlr(dds3ObjectBaseGetModelHandle(mapObjGetBasic(-16384)), 0), 810.0f);
                }
                mapObjFloorAdd_Cash(0, -16383, 3, 102, 1, -1, 1, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
                mapObjNoClip(-16383);
                mapFileLoadSub3(-16383, 14);
                mapFileLoadSub8(48);
                if (fldGlobalBase.word1E8 == 0)
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
    return mdlCallMemGetSize2();
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
    if (mapFileLoadSub7() != 0)
    {
        mefMistTexDestroy();
    }
    return gSubFloorID = -1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0014F378", fldFreeMapData);
#endif
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

