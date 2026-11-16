#include "common.h"
#include "mapData.h"
#include "fld.h"
#include "dds3ObjectBase.h"
#include "sdk_libc.h"
#include "fpu.h"
#include "code_r1_00128378.h"
#include "dds3Randomize.h"
#include "EventBit.h"

void mapObjEveHitSet(s16 uniq, s32 act, char *scr_name) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].uniq == uniq) {
            mapObjTaskData2[i].act = act;
            mapObjTaskData2[i].evehitdir = 1;
            strcpy(mapObjTaskData2[i].scr_name, scr_name);
            return;
        }
    }
}

INCLUDE_ASM(const s32, "code_00148310", mapObjEveHitSet2);

/* NON_MATCHING: 97 of 109 words match.  */
#ifdef NON_MATCHING
void mapCallPositioningGuardSystem2(s32 id, s32 ix, s32 iy, s32 iw, s32 ih, s32 dir, u16 flag0, u8 flag0j, u16 flag1, u8 flag1j, s32 act, s32 msk, void *scr_name) {
    f32 x;
    f32 z;
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapScriptGet_PosData2[i].use == 0) {
            mapScriptGet_PosData2[i].use = 1;
            mapScriptGet_PosData2[i].flag1j = flag1j;
            mapScriptGet_PosData2[i].on = 1;
            mapScriptGet_PosData2[i].id = id;
            mapScriptGet_PosData2[i].act = act;
            mapScriptGet_PosData2[i].msk = msk;
            mapScriptGet_PosData2[i].dir = dir;
            mapScriptGet_PosData2[i].flag0 = flag0;
            mapScriptGet_PosData2[i].flag0j = flag0j;
            mapScriptGet_PosData2[i].flag1 = flag1;
            mapScriptGet_PosData2[i].ix = ix;
            mapScriptGet_PosData2[i].iy = iy;
            mapScriptGet_PosData2[i].iw = iw;
            mapScriptGet_PosData2[i].ih = ih;
            PositioningGuardSystem(ix + iw / 2, iy + ih / 2, &x, &z);
            if (iw == 1) {
                x += 50.0f;
            }
            if (ih == 1) {
                z -= 50.0f;
            }
            mapScriptGet_PosData2[i].x = x;
            mapScriptGet_PosData2[i].z = z;
            mapScriptGet_PosData2[i].y = mapGetY(x, z);
            strcpy(mapScriptGet_PosData2[i].scr_name, scr_name);
            return;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00148310", mapCallPositioningGuardSystem2);
#endif

INCLUDE_ASM(const s32, "code_00148310", mapLeaf16);

INCLUDE_ASM(const s32, "code_00148310", mapNpcTableGetMSK);

INCLUDE_ASM(const s32, "code_00148310", mapScriptGet_Pos);

s32 mapCallSetPlayerTargetData(f32 x, f32 z) {
    s32 ix;
    s32 iz;
    s32 i;

    fldGlobalBase.previousFloor = -1;
    fldSetPlayerTargetData(&ix, &iz, x, z);
    for (i = 0; i < 64; i++) {
        if (mapWallObjTaskData[i].use != 0 && ix >= mapWallObjTaskData[i].vix0 && ix < mapWallObjTaskData[i].vix1 && iz >= mapWallObjTaskData[i].viy0 && iz < mapWallObjTaskData[i].viy1 && mapWallObjTaskData[i].floor == fldGlobalBase.nameU) {
            fldGlobalBase.previousFloor = mapWallObjTaskData[i].type;
        }
    }
    for (i = 0; i < 64; i++) {
        if (mapWallObjTaskData[i].use != 0 && (u16)mapWallObjTaskData[i].type >= 2 && ix >= mapWallObjTaskData[i].eveix0 && ix < mapWallObjTaskData[i].eveix1 && iz >= mapWallObjTaskData[i].eveiy0 && iz < mapWallObjTaskData[i].eveiy1 && mapWallObjTaskData[i].floor == fldGlobalBase.nameU) {
            mapCallSetPlayerTargetDataData = i;
            fldGlobalBase.previousFloor = mapWallObjTaskData[i].type;
            mapData98 = mapWallObjTaskData[i].flag;
            return mapWallObjTaskData[i].encountid;
        }
    }
    return -1;
}

/* NON_MATCHING: 64 of 67 words match.  */
#ifdef NON_MATCHING
s32 mapCallSetPlayerTargetData2(f32 x, f32 z) {
    s32 ix;
    s32 iz;
    s32 i;

    for (i = 0; i < 64; i++) {
        if (mapWallObjTaskData[i].use != 0 && mapWallObjTaskData[i].type == 1) {
            fldSetPlayerTargetData(&ix, &iz, x, z);
            if (ix >= mapWallObjTaskData[i].ix && ix < mapWallObjTaskData[i].ix + mapWallObjTaskData[i].iw &&
                iz >= mapWallObjTaskData[i].iy && iz < mapWallObjTaskData[i].iy + mapWallObjTaskData[i].ih &&
                mapWallObjTaskData[i].floor == fldGlobalBase.nameU) {
                return i;
            }
        }
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "code_00148310", mapCallSetPlayerTargetData2);
#endif

INCLUDE_ASM(const s32, "code_00148310", mapProcSequenceSub4);

INCLUDE_ASM(const s32, "code_00148310", mapGridHitAdd3);

INCLUDE_ASM(const s32, "code_00148310", mapLeaf17);

INCLUDE_ASM(const s32, "code_00148310", mapLeaf18);

void mapCallPositioningGuardSystem3(s32 id, s32 ix, s32 iy, s32 iw, s32 ih, s32 act, s32 dir, s32 fieldID, s32 areaID, s32 posID, s32 se) {
    f32 x;
    f32 z;
    s32 i;

    for (i = 0; i < 80; i++) {
        if (mapData24[i].use == 0) {
            mapData24[i].areaID = areaID;
            mapData24[i].posID = posID;
            mapData24[i].use = 1;
            mapData24[i].id = id;
            mapData24[i].act = act;
            mapData24[i].dir = dir;
            mapData24[i].se = se;
            mapData24[i].fieldID = fieldID;
            mapData24[i].ix = ix;
            mapData24[i].iy = iy;
            mapData24[i].iw = iw;
            mapData24[i].ih = ih;
            PositioningGuardSystem(ix + iw / 2, iy + ih / 2, &x, &z);
            if (iw == 1) {
                x += 50.0f;
            }
            if (ih == 1) {
                z -= 50.0f;
            }
            mapData24[i].x = x;
            mapData24[i].z = z;
            mapData24[i].y = mapGetY(x, z);
            return;
        }
    }
}

s32 mapWarpGet_Pos(f32 x, f32 y, f32 z, s32 *attr, f32 *objx, f32 *objy, f32 *objz) {
    s32 ix;
    s32 iz;
    s32 i;
    s32 found;
    f32 deg;

    fldSetPlayerTargetData(&ix, &iz, x, z);
    for (i = 0; i < 80; i++) {
        if (mapData24[i].use != 1) {
            continue;
        }
        if ((mapData24[i].id & 0xF) == 1) {
            if (evtBitCheck(2993)) {
                continue;
            }
        } else if ((mapData24[i].id & 0xF) == 2) {
            if (!evtBitCheck(2993)) {
                continue;
            }
        }
        if (mapData24[i].fieldID == 63) {
            if (evtBitCheck(342)) {
                continue;
            }
        } else if (mapData24[i].act == 4) {
            if (evtBitCheck(175) && !evtBitCheck(179)) {
                continue;
            }
        }
        if (ix < mapData24[i].ix || ix >= mapData24[i].ix + mapData24[i].iw) {
            continue;
        }
        if (iz < mapData24[i].iy || iz >= mapData24[i].iy + mapData24[i].ih) {
            continue;
        }
        if ((u8)mapData24[i].act != 0) {
            if ((u8)mapData24[i].act == 15 && fldGlobalBase.nakamaMode != 0) {
                goto take;
            }
            if ((u8)mapData24[i].act == 16 && fldGlobalBase.nakamaMode == 0) {
                goto take;
            }
            if ((u8)mapData24[i].act != 17) {
                if ((u8)mapData24[i].act == 18) {
                    goto take;
                }
                found = FALSE;
                switch (mapData24[i].dir) {
                case 0:
                    deg = R360_LR(fldGlobalBase.floatC0, 0.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 1:
                    deg = R360_LR(fldGlobalBase.floatC0, 337.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 2:
                    deg = R360_LR(fldGlobalBase.floatC0, 315.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 3:
                    deg = R360_LR(fldGlobalBase.floatC0, 292.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 4:
                    deg = R360_LR(fldGlobalBase.floatC0, 270.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 5:
                    deg = R360_LR(fldGlobalBase.floatC0, 248.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 6:
                    deg = R360_LR(fldGlobalBase.floatC0, 225.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 7:
                    deg = R360_LR(fldGlobalBase.floatC0, 202.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 8:
                    deg = R360_LR(fldGlobalBase.floatC0, 180.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 9:
                    deg = R360_LR(fldGlobalBase.floatC0, 158.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 10:
                    deg = R360_LR(fldGlobalBase.floatC0, 135.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 11:
                    deg = R360_LR(fldGlobalBase.floatC0, 112.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 12:
                    deg = R360_LR(fldGlobalBase.floatC0, 90.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 13:
                    deg = R360_LR(fldGlobalBase.floatC0, 67.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 14:
                    deg = R360_LR(fldGlobalBase.floatC0, 45.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 15:
                    deg = R360_LR(fldGlobalBase.floatC0, 22.0f);
                    if (deg < 80.0f && deg > -80.0f) {
                        found = TRUE;
                    }
                    break;
                case 16:
                    found = TRUE;
                    break;
                }
                if (!found) {
                    continue;
                }
            }
        }
    take:
        *objx = mapData24[i].x;
        *objy = mapData24[i].y;
        *attr = mapData24[i].act;
        fldGlobalBase.nowDoorSe = mapData24[i].se;
        *objz = mapData24[i].z;
        return i;
    }
    return -1;
}

INCLUDE_ASM(const s32, "code_00148310", mapWarpChk_Idx);

INCLUDE_ASM(const s32, "code_00148310", mapWarpGo_Idx);

INCLUDE_ASM(const s32, "code_00148310", mapCallTitleStartFld);

INCLUDE_ASM(const s32, "code_00148310", mapCallMapSetArea);

INCLUDE_ASM(const s32, "code_00148310", mapCallBitCheck2);

s32 mapFileLoadSub4(s32 floor, s16 kind) {
    MAP_WALLOBJ_t *wall;
    s32 count;
    s32 i;

    wall = mapWallObjTaskData;
    count = 0;
    for (i = 0; i < 64; i++, wall++) {
        if (wall->use != 0 && wall->type == kind && wall->floor == floor) {
            count++;
        }
    }
    return count;
}

INCLUDE_ASM(const s32, "code_00148310", mapWallGetUniq);

INCLUDE_RODATA(const s32, "code_00148310", mapWallObjTaskTable);

INCLUDE_ASM(const s32, "code_00148310", mapWallObjTask);

INCLUDE_ASM(const s32, "code_00148310", mapSetAreaRefreshSub2);

INCLUDE_ASM(const s32, "code_00148310", mapObjLightAdd);
