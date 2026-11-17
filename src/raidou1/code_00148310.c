#include "common.h"
#include "mapData.h"
#include "fld.h"
#include "dds3ObjectBase.h"
#include "sdk_libc.h"
#include "fpu.h"
#include "code_r1_00128378.h"
#include "dds3Randomize.h"
#include "EventBit.h"
#include "sdfMath.h"
#include "dds3KernelDraw.h"
#include "fldProcess.h"
#include "ps2asm.h"

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

/* NON_MATCHING: 19 of 419 words match.  */
#ifdef NON_MATCHING
char *mapScriptGet_Pos(f32 x, f32 y, f32 z, f32 r, s32 *attr, f32 *objx, f32 *objy, f32 *objz, s32 *type) {
    s32 ix;
    s32 iz;
    s32 i;
    s32 hit;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    f32 angle;

    *type = 0;
    fldGlobalBase.word154 = -1;
    fldGlobalBase.word148 = 0;
    fldGlobalBase.word14C = 0;
    fldGlobalBase.word150 = 0;
    fldGlobalBase.nowTakaraTblIdx = 0;
    fldGlobalBase.word158 = 0;
    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].use != 1) {
            continue;
        }
        if (mapObjTaskData2[i].visible == 0) {
            continue;
        }
        if (mapObjTaskData2[i].uniq <= 0) {
            continue;
        }
        if (mapObjTaskData2[i].scr_name[0] == '\0') {
            continue;
        }
        hit = 0;
        dy = y - mapObjTaskData2[i].y;
        dx = x - mapObjTaskData2[i].x;
        dz = z - mapObjTaskData2[i].z;
        dist = fsqrtf(dx * dx + dy * dy + dz * dz);
        if (!(dist < mapObjTaskData2[i].evehitsize)) {
            continue;
        }
        if (mapObjTaskData2[i].evehitdir == 0 && dist < 70.0f) {
            hit = 1;
        } else {
            angle = R360_LR(sdfAtan2(dx, dz) * 57.32484055f, -r);
            if (angle < 80.0f && angle > -80.0f) {
                hit = 1;
            }
        }
        if (!hit) {
            continue;
        }
        *objx = mapObjTaskData2[i].x;
        *objy = mapObjTaskData2[i].y;
        *attr = mapObjTaskData2[i].act;
        *objz = mapObjTaskData2[i].z;
        if (mapObjTaskData2[i].basic != NULL) {
            fldGlobalBase.nowTalkModelID = mapObjTaskData2[i].basic->serialID;
        }
        fldGlobalBase.mskIndex = 0;
        fldGlobalBase.nowTakaraTblIdx = fldGlobalBase.nowHealTblIdx = mapObjTaskData2[i].uniq & 0xFF;
        fldGlobalBase.word148 = 0;
        if (mapObjTaskData2[i].npc_idx > 0) {
            fldGlobalBase.mskIndex = mapNpcTableGetMSK(mapObjTaskData2[i].npc_idx);
        }
        if ((u16)mapObjTaskData2[i].uniq >= 0x2000 && (u16)mapObjTaskData2[i].uniq < 0x2040) {
            if ((u8)mapObjTaskData2[i].uniq == 0) {
                DDS3_GBWK->unk2C72 = 0;
                DDS3_GBWK->unk2C70 = 0;
            } else {
                DDS3_GBWK->unk2C70 = 1;
                DDS3_GBWK->unk2C72 = (u8)mapObjTaskData2[i].uniq;
            }
        }
        *type = 1;
        if (fldGlobalBase.nakamaMode != 0 && mapObjTaskData2[i].npc_idx > 0 && mapObjTaskData2[i].npc_idx < 320) {
            fldGlobalBase.word150 = mapObjTaskData2[i].npc_idx;
            fldGlobalBase.word14C = 1;
        }
        if (mapObjTaskData2[i].npc_idx > 0 && mapObjTaskData2[i].npc_kuse_time > 0) {
            fldGlobalBase.word158 = mapObjTaskData2[i].uniq;
        }
        return mapObjTaskData2[i].scr_name;
    }

    fldSetPlayerTargetData(&ix, &iz, x, z);
    for (i = 0; i < 128; i++) {
        if (mapScriptGet_PosData2[i].use != 1) {
            continue;
        }
        if (mapScriptGet_PosData2[i].on != 1) {
            continue;
        }
        if (mapScriptGet_PosData2[i].flag0 != 0) {
            if (mapScriptGet_PosData2[i].flag0j == 0) {
                if (evtBitCheck(mapScriptGet_PosData2[i].flag0)) {
                    continue;
                }
            } else {
                if (!evtBitCheck(mapScriptGet_PosData2[i].flag0)) {
                    continue;
                }
            }
        }
        if (mapScriptGet_PosData2[i].flag1 != 0) {
            if (mapScriptGet_PosData2[i].flag1j == 0) {
                if (evtBitCheck(mapScriptGet_PosData2[i].flag1)) {
                    continue;
                }
            } else {
                if (!evtBitCheck(mapScriptGet_PosData2[i].flag1)) {
                    continue;
                }
            }
        }
        if (ix < mapScriptGet_PosData2[i].ix || ix >= mapScriptGet_PosData2[i].ix + mapScriptGet_PosData2[i].iw) {
            continue;
        }
        if (iz < mapScriptGet_PosData2[i].iy || iz >= mapScriptGet_PosData2[i].iy + mapScriptGet_PosData2[i].ih) {
            continue;
        }
        if (fldGlobalBase.nakamaMode == 0 && ((mapScriptGet_PosData2[i].id & 0xF000) == 0x1000 || (mapScriptGet_PosData2[i].id & 0xF000) == 0x2000) &&
            mapScriptGet_PosData2[i].scr_name[0] == '\0') {
            continue;
        }
        if (mapScriptGet_PosData2[i].act == 15 && fldGlobalBase.nakamaMode == 0) {
            continue;
        }
        if (mapScriptGet_PosData2[i].act == 16 && fldGlobalBase.nakamaMode != 0) {
            continue;
        }
        if (mapScriptGet_PosData2[i].act != 0 && !(mapScriptGet_PosData2[i].act == 15 && fldGlobalBase.nakamaMode != 0) &&
            !(mapScriptGet_PosData2[i].act == 16 && fldGlobalBase.nakamaMode == 0) && mapScriptGet_PosData2[i].act != 17 && mapScriptGet_PosData2[i].act != 18) {
            hit = 0;
            switch (mapScriptGet_PosData2[i].dir) {
            case 0:
                hit = 1;
                break;
            case 1:
                angle = R360_LR(fldGlobalBase.floatC0, 90.0f);
                if (angle < 80.0f && angle > -80.0f) {
                    hit = 1;
                }
                break;
            case 2:
                angle = R360_LR(fldGlobalBase.floatC0, 270.0f);
                if (angle < 80.0f && angle > -80.0f) {
                    hit = 1;
                }
                break;
            case 3:
                angle = R360_LR(fldGlobalBase.floatC0, 0.0f);
                if (angle < 80.0f && angle > -80.0f) {
                    hit = 1;
                }
                break;
            case 4:
                angle = R360_LR(fldGlobalBase.floatC0, 180.0f);
                if (angle < 80.0f && angle > -80.0f) {
                    hit = 1;
                }
                break;
            }
            if (!hit) {
                continue;
            }
        }
        *objx = mapScriptGet_PosData2[i].x;
        *objy = mapScriptGet_PosData2[i].y;
        *objz = mapScriptGet_PosData2[i].z;
        *attr = mapScriptGet_PosData2[i].act;
        *type = 2;
        fldGlobalBase.mskIndex = mapScriptGet_PosData2[i].msk;
        fldGlobalBase.word148 = mapScriptGet_PosData2[i].id;
        fldGlobalBase.word15C = mapScriptGet_PosData2[i].dir;
        mapEveHitMsk_Find(mapScriptGet_PosData2[i].id);
        return mapScriptGet_PosData2[i].scr_name;
    }
    return NULL;
}
#else
INCLUDE_ASM(const s32, "code_00148310", mapScriptGet_Pos);
#endif

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

/* NON_MATCHING: 87 of 95 words match.  */
#ifdef NON_MATCHING
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
#else
INCLUDE_ASM(const s32, "code_00148310", mapCallPositioningGuardSystem3);
#endif

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

/* NON_MATCHING: 649 of 676 words match.  */
#ifdef NON_MATCHING
void mapWallObjTask(f32 x, f32 y, f32 z)
{
    f32 vec[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    s32 ix, iz;
    f32 x0, z0, x1, z1, x2, z2, x3, z3;
    f32 py, y3, y0, pz, px, rot;
    f32 scale;
    s32 i, uniq, link, found, color;

    eglSetOt(0x44);
    eglLinkAlpha(0);
    y0 = 0.0f;
    y3 = 0.0f;
    py = 0.0f;
    pz = 0.0f;
    rot = 0.0f;
    px = 0.0f;
    fldSetPlayerTargetData(&ix, &iz, fldGlobalBase.focusX, fldGlobalBase.focusZ);
    SDF_VU0_LOADVECTOR_MEMOPS(*(u128 *)vec);
    for (i = 0; i < 4; i++) {
        mapObjVisible(0x9000 + i, 0);
        mapObjVisible(0x9010 + i, 0);
        mapObjVisible(0x9020 + i, 0);
        mapObjVisible(0x9030 + i, 0);
        mapObjVisible(0x9040 + i, 0);
        mapObjVisible(0x9050 + i, 0);
        mapObjVisible(0x9060 + i, 0);
        mapObjVisible(0x9070 + i, 0);
    }
    for (i = 0; i < 64; i++) {
        if (mapWallObjTaskData[i].use == 0) {
            continue;
        }
        if (ix >= mapWallObjTaskData[i].vix0 && ix < mapWallObjTaskData[i].vix1
            && iz >= mapWallObjTaskData[i].viy0 && iz < mapWallObjTaskData[i].viy1) {
            mapWallObjTaskData[i].alpha += 8;
            if (mapWallObjTaskData[i].alpha > 128) {
                mapWallObjTaskData[i].alpha = 128;
            }
        } else {
            mapWallObjTaskData[i].alpha -= 8;
            if (mapWallObjTaskData[i].alpha < 0) {
                mapWallObjTaskData[i].alpha = 0;
            }
        }
        if (mapWallObjTaskData[i].alpha == 0) {
            mapWallObjTaskData[i].view = 0;
            mapWallObjTaskData[i].link_idx = -1;
            continue;
        }
        uniq = -1;
        switch (mapWallObjTaskData[i].type) {
        case 0:
            link = mapWallObjTaskData[i].link_idx;
            if (link == -1) {
                found = mapWallGetUniq(0);
                if (found != -1) {
                    uniq = found + 0x9060;
                    mapWallObjTaskData[i].link_idx = found;
                }
            } else {
                uniq = link + 0x9060;
            }
            break;
        case 1:
            link = mapWallObjTaskData[i].link_idx;
            if (link == -1) {
                found = mapWallGetUniq(1);
                if (found != -1) {
                    uniq = found + 0x9070;
                    mapWallObjTaskData[i].link_idx = found;
                }
            } else {
                uniq = link + 0x9070;
            }
            break;
        case 2:
            link = mapWallObjTaskData[i].link_idx;
            if (link == -1) {
                found = mapWallGetUniq(2);
                if (found != -1) {
                    uniq = found + 0x9000;
                    mapWallObjTaskData[i].link_idx = found;
                }
            } else {
                uniq = link + 0x9000;
            }
            break;
        case 3:
            link = mapWallObjTaskData[i].link_idx;
            if (link == -1) {
                found = mapWallGetUniq(3);
                if (found != -1) {
                    uniq = found + 0x9010;
                    mapWallObjTaskData[i].link_idx = found;
                }
            } else {
                uniq = link + 0x9010;
            }
            break;
        case 4:
            link = mapWallObjTaskData[i].link_idx;
            if (link == -1) {
                found = mapWallGetUniq(4);
                if (found != -1) {
                    uniq = found + 0x9020;
                    mapWallObjTaskData[i].link_idx = found;
                }
            } else {
                uniq = link + 0x9020;
            }
            break;
        case 5:
            link = mapWallObjTaskData[i].link_idx;
            if (link == -1) {
                found = mapWallGetUniq(5);
                if (found != -1) {
                    uniq = found + 0x9030;
                    mapWallObjTaskData[i].link_idx = found;
                }
            } else {
                uniq = link + 0x9030;
            }
            break;
        case 6:
            link = mapWallObjTaskData[i].link_idx;
            if (link == -1) {
                found = mapWallGetUniq(6);
                if (found != -1) {
                    uniq = found + 0x9040;
                    mapWallObjTaskData[i].link_idx = found;
                }
            } else {
                uniq = link + 0x9040;
            }
            break;
        case 7:
            link = mapWallObjTaskData[i].link_idx;
            if (link == -1) {
                found = mapWallGetUniq(7);
                if (found != -1) {
                    uniq = found + 0x9050;
                    mapWallObjTaskData[i].link_idx = found;
                }
            } else {
                uniq = link + 0x9050;
            }
            break;
        }
        if (uniq == -1) {
            continue;
        }
        fldWallObjTaskSub(mapWallObjTaskData[i].floor, mapWallObjTaskData[i].ix, mapWallObjTaskData[i].iy, &x0, &z0);
        fldWallObjTaskSub(mapWallObjTaskData[i].floor, mapWallObjTaskData[i].ix + mapWallObjTaskData[i].iw,
                          mapWallObjTaskData[i].iy, &x1, &z1);
        fldWallObjTaskSub(mapWallObjTaskData[i].floor, mapWallObjTaskData[i].ix,
                          mapWallObjTaskData[i].iy + mapWallObjTaskData[i].ih, &x2, &z2);
        fldWallObjTaskSub(mapWallObjTaskData[i].floor, mapWallObjTaskData[i].ix + mapWallObjTaskData[i].iw,
                          mapWallObjTaskData[i].iy + mapWallObjTaskData[i].ih, &x3, &z3);
        switch (mapWallObjTaskData[i].dir) {
        case 0:
            y0 = mapGetFloorY(mapWallObjTaskData[i].floor, x0, z0);
            mapGetFloorY(mapWallObjTaskData[i].floor, x1 - 10.0f, z1);
            mapGetFloorY(mapWallObjTaskData[i].floor, x2, z2);
            y3 = mapGetFloorY(mapWallObjTaskData[i].floor, x3 - 10.0f, z3);
            break;
        case 8:
            y0 = mapGetFloorY(mapWallObjTaskData[i].floor, x0, z0);
            mapGetFloorY(mapWallObjTaskData[i].floor, x1 - 10.0f, z1);
            mapGetFloorY(mapWallObjTaskData[i].floor, x2, z2);
            y3 = mapGetFloorY(mapWallObjTaskData[i].floor, x3 - 10.0f, z3);
            break;
        case 4:
            y0 = mapGetFloorY(mapWallObjTaskData[i].floor, x0, z0);
            mapGetFloorY(mapWallObjTaskData[i].floor, x1, z1);
            mapGetFloorY(mapWallObjTaskData[i].floor, x2, z2 + 10.0f);
            y3 = mapGetFloorY(mapWallObjTaskData[i].floor, x3, z3 + 10.0f);
            break;
        case 12:
            y0 = mapGetFloorY(mapWallObjTaskData[i].floor, x0, z0);
            mapGetFloorY(mapWallObjTaskData[i].floor, x1, z1);
            mapGetFloorY(mapWallObjTaskData[i].floor, x2, z2 + 10.0f);
            y3 = mapGetFloorY(mapWallObjTaskData[i].floor, x3, z3 + 10.0f);
            break;
        }
        switch (mapWallObjTaskData[i].dir) {
        case 0:
            rot = 180.0f;
            py = (y0 + y3) * 0.5f;
            px = (x0 + x3) * 0.5f;
            pz = z3;
            break;
        case 8:
            rot = 0.0f;
            py = (y0 + y3) * 0.5f;
            px = (x0 + x3) * 0.5f;
            pz = z0;
            break;
        case 4:
            rot = 90.0f;
            py = (y0 + y3) * 0.5f;
            pz = (z0 + z3) * 0.5f;
            px = x0;
            break;
        case 12:
            rot = -90.0f;
            py = (y0 + y3) * 0.5f;
            pz = (z0 + z3) * 0.5f;
            px = x3;
            break;
        }
        color = mapWallObjTaskData[i].alpha << 24;
        eglSetOt(0x44);
        switch (mapWallObjTaskData[i].type) {
        case 0:
            if (mapWallObjTaskData[i].view == 0) {
                mapWallObjTaskData[i].view = 1;
                p2sdPlaySE_Vol(32, 127);
            }
            scale = mapWallObjTaskData[i].haba;
            mapObjPos(uniq, px, py, pz);
            mapObjRot(uniq, rot);
            mapObjScale(uniq, scale / 3.0f, scale / 3.0f, 1.0f);
            mapObjVisible(uniq, 1);
            mapObjColor(uniq, color | 0x808080);
            break;
        case 1:
            if (mapWallObjTaskData[i].view == 0) {
                mapWallObjTaskData[i].view = 1;
                p2sdPlaySE_Vol(32, 127);
            }
            scale = mapWallObjTaskData[i].haba;
            mapObjPos(uniq, px, py, pz);
            mapObjRot(uniq, rot);
            mapObjScale(uniq, scale * 0.25f, scale * 0.25f, 1.0f);
            mapObjVisible(uniq, 1);
            mapObjColor(uniq, color | 0x808080);
            break;
        default:
            if (mapWallObjTaskData[i].view == 0) {
                mapWallObjTaskData[i].view = 1;
                p2sdPlaySE_Vol(32, 127);
            }
            scale = mapWallObjTaskData[i].haba;
            mapObjPos(uniq, px, py, pz);
            mapObjRot(uniq, rot);
            mapObjScale(uniq, scale, scale, 4.0f);
            mapObjVisible(uniq, 1);
            mapObjColor(uniq, color | 0x808080);
            break;
        }
    }
}
#else
INCLUDE_RODATA(const s32, "code_00148310", mapWallObjTaskTable);

INCLUDE_ASM(const s32, "code_00148310", mapWallObjTask);
#endif

INCLUDE_ASM(const s32, "code_00148310", mapSetAreaRefreshSub2);

INCLUDE_ASM(const s32, "code_00148310", mapObjLightAdd);
