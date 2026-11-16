#include "common.h"
#include "mapData.h"
#include "fld.h"
#include "dds3ObjectBase.h"
#include "sdk_libc.h"
#include "fpu.h"
#include "code_r1_00128378.h"
#include "dds3Randomize.h"
#include "EventBit.h"
#include "dds3SceneBasic.h"
#include "dds3Basic.h"

INCLUDE_ASM(const s32, "code_0014B660", mapKaidanAdd);

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

INCLUDE_ASM(const s32, "code_0014B660", mapWHitAdd);

INCLUDE_ASM(const s32, "code_0014B660", mapFileLoadSub5);

INCLUDE_ASM(const s32, "code_0014B660", mapNpcTblAdd);

/* NON_MATCHING: 59 of 248 words match.  */
#ifdef NON_MATCHING
void mapNpcTblAdd_fld(s32 map, s32 flor, s32 point) {
    f32 x;
    f32 y;
    f32 z;
    f32 r;
    s32 i;
    s32 j;
    s32 obj;
    s32 id1;
    s32 id2;
    s32 mot;
    s32 idx;
    char *label;
    s32 cng;
    f32 size;

    id2 = -1;
    label = NULL;
    cng = -1;
    id1 = -1;
    idx = -1;
    size = 1.0f;
    mot = 0;
    for (i = 0; i < 640; i++) {
        if (mapEXFloorData[i].map != map) {
            continue;
        }
        if (mapEXFloorData[i].flor != flor) {
            continue;
        }
        if (mapEXFloorData[i].point != (point & 0xFFF)) {
            continue;
        }
        if (mapEXFloorData[i].flag1 != 0 && !evtBitCheck(mapEXFloorData[i].flag1)) {
            return;
        }
        if (mapEXFloorData[i].flag2 != 0 && !evtBitCheck(mapEXFloorData[i].flag2)) {
            return;
        }
        if (mapEXFloorData[i].flag3 != 0 && !evtBitCheck(mapEXFloorData[i].flag3)) {
            return;
        }
        size = mapEXFloorData[i].model_size;
        id1 = mapEXFloorData[i].model_id1;
        id2 = mapEXFloorData[i].model_id2;
        mot = mapEXFloorData[i].model_mot;
        label = mapEXFloorData[i].scr_label;
        idx = i;
        if (mapEXFloorData[i].model_cflag != 0 && evtBitCheck(mapEXFloorData[i].model_cflag)) {
            id2 = mapEXFloorData[i].model_cid2;
            cng = 1;
            mot = mapEXFloorData[i].model_cmot;
            size = mapEXFloorData[i].model_csize;
        }
        break;
    }
    if (id1 == -1) {
        return;
    }
    mapObjPointGet(point, &x, &y, &z, &r);
    obj = mapAddHukidashi(point, id1, id2, mot, -1, 1, x, y, z, r, size);
    mapObjTaskData2[obj].npc_cng = cng;
    if (id1 != 3 || id2 != 186) {
        mapObjHitSize(point, 25.0f);
    }
    mapObjFloor(point, flor);
    dds3ObjectBaseOnFlag(mapObjGetBasic(point), 0x400);
    dds3ObjectBaseOnFlag(mapObjGetBasic(point), 0x80000);
    dds3GetRadiusPoseSet(mapObjGetBasic(point), 40.0f);
    dds3ObjectBaseOnFlag(mapObjGetBasic(point), 0x800);
    if (label != NULL) {
        mapObjEveHitSet(point, 2, label);
    }
    if (id1 == 3 && (id2 < 98 || mapObjTaskData2[obj].model_id2 != 105)) {
        mapObjNpcKuse(mapObjTaskData2[obj].uniq, (dds3GetRandA(NULL) >> 4) % 12 * 60 + 360);
    }
    for (j = 0; j < 128; j++) {
        if (mapObjTaskData2[j].uniq == point) {
            mapObjTaskData2[j].npc_idx = idx;
            break;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0014B660", mapNpcTblAdd_fld);
#endif

s32 mapNpcTbl_GetModelID(s32 idx) {
    s32 id;

    id = (mapEXFloorData[idx].model_id1 << 16) | mapEXFloorData[idx].model_id2;
    if (mapEXFloorData[idx].model_cflag != 0 && evtBitCheck(mapEXFloorData[idx].model_cflag)) {
        id = (mapEXFloorData[idx].model_id1 << 16) | mapEXFloorData[idx].model_cid2;
    }
    return id;
}

void mapNpcTblRefreshSub(s32 point) {
    s32 i;
    s32 j;
    s32 obj;
    s32 found;
    s32 on;
    s32 cng;
    s32 map;
    s32 flor;

    obj = 0;
    map = fldGlobalBase.amSBlock;
    flor = fldGlobalBase.nameU;
    for (i = 0; i < 640; i++) {
        if (mapEXFloorData[i].map != map) {
            continue;
        }
        if (mapEXFloorData[i].flor != flor) {
            continue;
        }
        if (mapEXFloorData[i].point != (point & 0xFFF)) {
            continue;
        }
        on = TRUE;
        if (mapEXFloorData[i].flag1 != 0) {
            on = evtBitCheck(mapEXFloorData[i].flag1) != 0;
        }
        if (mapEXFloorData[i].flag2 != 0 && !evtBitCheck(mapEXFloorData[i].flag2)) {
            on = FALSE;
        }
        if (mapEXFloorData[i].flag3 != 0 && !evtBitCheck(mapEXFloorData[i].flag3)) {
            on = FALSE;
        }
        cng = -1;
        if (mapEXFloorData[i].model_cflag != 0 && evtBitCheck(mapEXFloorData[i].model_cflag)) {
            cng = 1;
        }
        found = FALSE;
        for (j = 0; j < 128; j++) {
            if (mapObjTaskData2[j].use != 0 && mapObjTaskData2[j].uniq == point) {
                obj = j;
                found = TRUE;
                break;
            }
        }
        if (found) {
            if (!on) {
                npc_delcash[npc_delcash_cnt][0] = i;
                npc_delcash[npc_delcash_cnt][1] = obj;
                npc_delcash_cnt++;
                continue;
            }
            if (mapObjTaskData2[obj].npc_cng == cng) {
                continue;
            }
            npc_delcash[npc_delcash_cnt][0] = i;
            npc_delcash[npc_delcash_cnt][1] = obj;
            npc_delcash_cnt++;
            npc_addcash[npc_addcash_cnt][0] = i;
            npc_addcash[npc_addcash_cnt][1] = point;
            npc_addcash[npc_addcash_cnt][2] = flor;
            npc_addcash[npc_addcash_cnt][3] = mapNpcTbl_GetModelID(i);
            npc_addcash_cnt++;
        } else if (on == TRUE) {
            npc_addcash[npc_addcash_cnt][0] = i;
            npc_addcash[npc_addcash_cnt][1] = point;
            npc_addcash[npc_addcash_cnt][2] = flor;
            npc_addcash[npc_addcash_cnt][3] = mapNpcTbl_GetModelID(i);
            npc_addcash_cnt++;
        }
    }
}

/* NON_MATCHING: 114 of 258 words match.  */
#ifdef NON_MATCHING
void mapNpcTblRefresh(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 obj;
    s32 found;
    s32 on;
    s32 cng;
    s32 point;
    s32 map;
    s32 flor;

    obj = 0;
    npc_delcash_cnt = 0;
    map = fldGlobalBase.amSBlock;
    flor = fldGlobalBase.nameU;
    npc_addcash_cnt = 0;
    mapNpcTblRefresh_WaitData = 0;
    for (j = 0; j < 128; j++) {
        if (mapObjTaskData2[j].use == 0) {
            continue;
        }
        if (mapObjTaskData2[j].floor == gSubFloorID) {
            continue;
        }
        if (mapObjTaskData2[j].floor == flor) {
            continue;
        }
        if (mapObjTaskData2[j].npc_idx <= 0) {
            continue;
        }
        npc_delcash[npc_delcash_cnt][0] = mapObjTaskData2[j].npc_idx;
        npc_delcash[npc_delcash_cnt][1] = j;
        npc_delcash_cnt++;
    }
    if (mapMapSetAreaRefreshData5 != NULL) {
        for (k = 0; k < mapMapSetAreaRefreshData5->pointCount; k++) {
            mapNpcTblRefreshSub(mapMapSetAreaRefreshData6[k].id);
        }
    }
    if (pMapAreaNow != NULL) {
        for (k = 0; k < pMapAreaNow->pointCount; k++) {
            mapNpcTblRefreshSub(mapMapSetAreaRefreshData2[k].id);
        }
    }
    if (mapData101 == NULL) {
        return;
    }
    for (k = 0; k < mapData101->pointCount; k++) {
        point = mapMapSetAreaRefreshData7[k].id;
        for (i = 0; i < 640; i++) {
            if (mapEXFloorData[i].map != map) {
                continue;
            }
            if (mapEXFloorData[i].flor != gSubFloorID) {
                continue;
            }
            if (mapEXFloorData[i].point != (point & 0xFFF)) {
                continue;
            }
            on = TRUE;
            if (mapEXFloorData[i].flag1 != 0) {
                on = evtBitCheck(mapEXFloorData[i].flag1) != 0;
            }
            if (mapEXFloorData[i].flag2 != 0 && !evtBitCheck(mapEXFloorData[i].flag2)) {
                on = FALSE;
            }
            if (mapEXFloorData[i].flag3 != 0 && !evtBitCheck(mapEXFloorData[i].flag3)) {
                on = FALSE;
            }
            cng = -1;
            if (mapEXFloorData[i].model_cflag != 0 && evtBitCheck(mapEXFloorData[i].model_cflag)) {
                cng = 1;
            }
            found = FALSE;
            for (j = 0; j < 128; j++) {
                if (mapObjTaskData2[j].use != 0 && mapObjTaskData2[j].uniq == point &&
                    mapObjTaskData2[j].floor == gSubFloorID) {
                    obj = j;
                    found = TRUE;
                    break;
                }
            }
            if (found) {
                if (!on) {
                    npc_delcash[npc_delcash_cnt][0] = i;
                    npc_delcash[npc_delcash_cnt][1] = obj;
                    npc_delcash_cnt++;
                    continue;
                }
                if (mapObjTaskData2[obj].npc_cng == cng) {
                    continue;
                }
                npc_delcash[npc_delcash_cnt][0] = i;
                npc_delcash[npc_delcash_cnt][1] = obj;
                npc_delcash_cnt++;
                npc_addcash[npc_addcash_cnt][0] = i;
                npc_addcash[npc_addcash_cnt][1] = point;
                npc_addcash[npc_addcash_cnt][2] = gSubFloorID;
                npc_addcash[npc_addcash_cnt][3] = mapNpcTbl_GetModelID(i);
                npc_addcash_cnt++;
            } else if (on == TRUE) {
                npc_addcash[npc_addcash_cnt][0] = i;
                npc_addcash[npc_addcash_cnt][1] = point;
                npc_addcash[npc_addcash_cnt][2] = gSubFloorID;
                npc_addcash[npc_addcash_cnt][3] = mapNpcTbl_GetModelID(i);
                npc_addcash_cnt++;
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_0014B660", mapNpcTblRefresh);
#endif

/* NON_MATCHING: 94 of 215 words match.  */
#ifdef NON_MATCHING
s32 mapNpcTblRefresh_Wait(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 obj;
    s32 tmp[4];

    if (mapNpcTblRefresh_WaitData == -1) {
        return -1;
    }
    if (mapNpcTblRefresh_WaitData == 0) {
        if (npc_delcash_cnt > 0) {
            for (i = 0; i < npc_delcash_cnt; i++) {
                obj = npc_delcash[i][1];
                dds3ObjectBaseOnFlag(mapObjTaskData2[obj].basic, 1);
                mapObjTaskData2[obj].scr_name[0] = 0;
            }
            mapNpcTblRefresh_WaitData = 1;
            return 1;
        }
        mapNpcTblRefresh_WaitData = 10;
    }
    if (mapNpcTblRefresh_WaitData == 1) {
        mapNpcTblRefresh_WaitData = 2;
        return 1;
    }
    if (mapNpcTblRefresh_WaitData == 2) {
        mapNpcTblRefresh_WaitData = 3;
        return 1;
    }
    if (mapNpcTblRefresh_WaitData == 3) {
        if (npc_delcash_cnt > 0) {
            for (i = 0; i < npc_delcash_cnt; i++) {
                obj = npc_delcash[i][1];
                dds3DeleteSceneBasicData(mapObjTaskData2[obj].basic);
                mapObjTaskData2[obj].basic = NULL;
                mapObjTaskData2[obj].visible = 0;
                mapObjTaskData2[obj].use = 0;
                mapObjTaskData2[obj].uniq = -1;
            }
        }
        mapNpcTblRefresh_WaitData = 4;
        return 1;
    }
    if (mapNpcTblRefresh_WaitData == 4) {
        mapNpcTblRefresh_WaitData = 5;
        return 1;
    }
    if (mapNpcTblRefresh_WaitData == 5) {
        if (npc_addcash_cnt > 0) {
            mapNpcTblRefresh_WaitData = 10;
            return 1;
        }
        mapNpcTblRefresh_WaitData = -1;
        return 0;
    }
    if (mapNpcTblRefresh_WaitData == 10) {
        if (npc_addcash_cnt <= 0) {
            mapNpcTblRefresh_WaitData = -1;
            return 0;
        }
        for (j = 0; j < npc_addcash_cnt - 1; j++) {
            for (k = j + 1; k < npc_addcash_cnt; k++) {
                if (npc_addcash[k][3] < npc_addcash[j][3]) {
                    tmp[0] = npc_addcash[j][0];
                    tmp[1] = npc_addcash[j][1];
                    tmp[2] = npc_addcash[j][2];
                    tmp[3] = npc_addcash[j][3];
                    npc_addcash[j][0] = npc_addcash[k][0];
                    npc_addcash[j][1] = npc_addcash[k][1];
                    npc_addcash[j][2] = npc_addcash[k][2];
                    npc_addcash[j][3] = npc_addcash[k][3];
                    npc_addcash[k][0] = tmp[0];
                    npc_addcash[k][1] = tmp[1];
                    npc_addcash[k][2] = tmp[2];
                    npc_addcash[k][3] = tmp[3];
                }
            }
        }
        mapNpcTblRefresh_WaitData2 = 0;
        mapNpcTblRefresh_WaitData = 11;
    }
    if (mapNpcTblRefresh_WaitData == 11) {
        if (mapNpcTblRefresh_WaitData2 < npc_addcash_cnt) {
            mapNpcTblAdd_fld(fldGlobalBase.amSBlock, npc_addcash[mapNpcTblRefresh_WaitData2][2],
                             npc_addcash[mapNpcTblRefresh_WaitData2][1]);
            mapNpcTblRefresh_WaitData2++;
        } else {
            mapNpcTblRefresh_WaitData = 12;
        }
        return 1;
    }
    if (mapNpcTblRefresh_WaitData == 12) {
        mapNpcTblRefresh_WaitData = -1;
        return 0;
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "code_0014B660", mapNpcTblRefresh_Wait);
#endif

INCLUDE_ASM(const s32, "code_0014B660", mapBtlReLoadMapSBtlSceneSub);

INCLUDE_ASM(const s32, "code_0014B660", mapMapObjectRefresh);
