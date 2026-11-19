#include "common.h"
#include "mapData.h"
#include "fld.h"
#include "mdl.h"
#include "mdlManager.h"
#include "sdfMemory.h"
#include "enums.h"
#include "dds3Randomize.h"
#include "dds3ObjectBase.h"
#include "sdk_libc.h"
#include "EventBit.h"

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallSetOt6);

/* NON_MATCHING: 74 of 87 words match. Fragile: the enums.h, field.h and mapData.h declaration sets move the posx/posz spill order. */
#ifdef NON_MATCHING
s32 mapHitCalc(s32 mode, f32 posx, f32 posz, f32 r, f32 *getx, f32 *getz) {
    f32 tmpx;
    f32 tmpz;
    f32 newx;
    f32 newz;
    f32 retx;
    f32 retz;
    s32 hit;
    s32 attr;

    newx = posx;
    newz = posz;
    hit = mapHamidasiCHitSub(newx, newz, &tmpx, &tmpz);
    newx = tmpx;
    newz = tmpz;
    hit |= mapHamidasiNpcSub(newx, newz, &tmpx, &tmpz);
    newx = tmpx;
    newz = tmpz;
    hit |= mapHamidasiWHitSub(&newx, &newz, r);
    if (mode == 0) {
        attr = mapGetAttr(posx, posz);
        if ((attr & 3) == 3 || (attr & 0xC) == 0xC) {
            r = 50.0f;
        }
    }
    mapHitWorkCalcSetup(mode, newx, newz);
    hit |= mapHitWorkCalc(newx, newz, r, &retx, &retz);
    *getx = retx;
    *getz = retz;
    newx = retx;
    newz = retz;
    if (mapGetHit(newx, newz) == 0) {
        return -1;
    }
    return hit;
}
#else
INCLUDE_ASM(const s32, "code_0014B7B0", mapHitCalc);
#endif

INCLUDE_ASM(const s32, "code_0014B7B0", mapHitAidaCalc);

INCLUDE_ASM(const s32, "code_0014B7B0", mapHamidasiCheck);

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
        mapWallObjTaskData[i].unk30 = 0;
    }
    for (i = 0; i < 32; i++)
    {
        mapData32[i].use = 0;
        mapData32[i].uniq = -1;
    }
    for (i = 0; i < 32; i++)
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

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf12);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCASHDATA);

#define MDL_MEM_CACHE_COUNT 7

extern sdfMemHandle_t *mdlData6[MDL_MEM_CACHE_COUNT];
extern MODELTYPE mdlData7[MDL_MEM_CACHE_COUNT];
extern s32 mdlData8[MDL_MEM_CACHE_COUNT];
extern s32 mdlCallMemGetInfoData;

INCLUDE_ASM(const s32, "code_0014B7B0", mdlCallMemGetSize);

void mdlCallMemGetSize2(void) {
    s32 i;
    sdfMemHandle_t *previous;
    s32 size;
    void *source;
    sdfMemInfo_t info;

    for (i = mdlCallMemGetInfoData - 1; i >= 0; i--) {
        previous = mdlData6[i];
        if (previous != NULL) {
            size = sdfMemGetSize(previous);
            mdlData6[i] = sdfMemAllocDirect(size);
            source = sdfMemGetPtr(previous);
            memcpy(sdfMemGetPtr(mdlData6[i]), source, size);
            sdfMemFreeDirect(previous);
        }
    }
    sdfMemGetInfo(&info);
}

void mdlCallMemFreeDirect(void) {
    s32 i;

    for (i = 0; i < MDL_MEM_CACHE_COUNT; i++) {
        if (mdlData6[i] != NULL) {
            sdfMemFreeDirect(mdlData6[i]);
            mdlData6[i] = NULL;
            mdlData7[i] = 0;
            mdlData8[i] = 0;
        }
    }
    mdlCallMemGetInfoData = 0;
}

sdfMemHandle_t *mdlGetModelName(MODELTYPE aMajor, s32 aMinor) {
    s32 i;

    for (i = 0; i < mdlCallMemGetInfoData; i++) {
        if (mdlData7[i] == aMajor && mdlData8[i] == aMinor) {
            return mdlData6[i];
        }
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_0014B7B0", mdlCallMemGetInfo);

/* NON_MATCHING: 4 of 7 words match.  */
#ifdef NON_MATCHING
void sbtlCheckEndEscape(void)
{
    mdlCallMemFreeDirect();
}
#else
INCLUDE_ASM(const s32, "code_0014B7B0", sbtlCheckEndEscape);
#endif

INCLUDE_RODATA(const s32, "code_0014B7B0", mapObjFloorAdd_CashTable);

INCLUDE_RODATA(const s32, "code_0014B7B0", mapObjFloorAdd_CashTable2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjFloorAdd_Cash);

s32 mapAddHukidashi(s16 uniq, s32 major, s32 minor, s32 motion, u32 cut_bit, s32 cash, f32 x, f32 y, f32 z, f32 r, f32 s) {
    return mapObjFloorAdd_Cash(-1, uniq, major, minor, motion, cut_bit, cash, x, y, z, r, s);
}

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf13);

s32 mapObjectRefreshSub(s16 uniq, s32 major, s32 minor, s32 motion, u32 cut_bit, f32 x, f32 y, f32 z, f32 r, f32 s) {
    return mapObjFloorAdd_Cash(-1, uniq, major, minor, motion, cut_bit, 0, x, y, z, r, s);
}

s32 mapObjCheck(s16 uniq) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].uniq == uniq) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjDel);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjAllDel);

void mapObjVisible(s16 uniq, s32 visible) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].uniq == uniq) {
            mapObjTaskData2[i].visible = visible;
            if (visible == 0) {
                dds3ObjectBaseOnFlag(mapObjTaskData2[i].basic, 1);
            } else {
                dds3ObjectBaseOffFlag(mapObjTaskData2[i].basic, 1);
            }
            return;
        }
    }
}

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf14);

INCLUDE_ASM(const s32, "code_0014B7B0", nbStartEventPacks);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjColor);

void mapFileLoadSub3(s16 uniq, s32 spcial) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].uniq == uniq) {
            mapObjTaskData2[i].spcial = spcial;
            return;
        }
    }
}

void mapObjNpcKuse(s16 uniq, s32 wait) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].uniq == uniq) {
            mapObjTaskData2[i].npc_kuse_wait = wait;
            mapObjTaskData2[i].npc_kuse_time = 0;
            return;
        }
    }
}

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjNpcKuseStop);

void mapObjNpcKuseScriptStop(s32 SerialID) {
    MAP_OBJ_t *obj;
    s32 i;

    obj = mapObjTaskData2;
    for (i = 0; i < 128; i++, obj++) {
        if (obj->use != 0 && obj->basic != NULL && obj->basic->serialID == SerialID) {
            if (obj->npc_kuse_time > 0) {
                mapObjNpcKuse(obj->uniq, ((dds3GetRandA(NULL) >> 4) % 12) * 60 + 360);
            }
            return;
        }
    }
}

void mapObjHitSize(s16 uniq, f32 size) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].uniq == uniq) {
            mapObjTaskData2[i].hitsize = size;
            return;
        }
    }
}

void mapObjFloor(s16 uniq, s32 floor) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].uniq == uniq) {
            mapObjTaskData2[i].floor = floor;
            return;
        }
    }
}

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjMask);

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf15);

void mapObjNoClip(s16 uniq) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].uniq == uniq) {
            mapObjTaskData2[i].noclip = 1;
            return;
        }
    }
}

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallUnitObjectBasicSetOt);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjOnOff);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjPos);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjPosH);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjRot);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjRotQ);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjScale);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjMotionCng);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjMotionSpeed);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjMotionEndCheck);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjMotionPause);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjMotionPauseAll);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjMotionPauseOffAll);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjGetFloorBasic);

dds3Basic_t *mapObjGetBasic(s16 uniq) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].use == 1 && mapObjTaskData2[i].basic != NULL && mapObjTaskData2[i].uniq == uniq) {
            return mapObjTaskData2[i].basic;
        }
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjGetModelMinID);

INCLUDE_RODATA(const s32, "code_0014B7B0", mapObjRotTable);

INCLUDE_RODATA(const s32, "code_0014B7B0", mapObjScaleTable);

INCLUDE_RODATA(const s32, "code_0014B7B0", mapObjTaskTable2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjTask);

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

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjEveHitSet2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallPositioningGuardSystem2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf16);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallBitCheck2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapFunc);

INCLUDE_ASM(const s32, "code_0014B7B0", mapAnaLabel);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallSetPlayerTargetData2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallSetPlayerTargetData3);

/* NON_MATCHING: 61 of 65 words match.  */
#ifdef NON_MATCHING
s32 mapCallSetPlayerTargetData4(f32 x, f32 z)
{
    s32 ix;
    s32 iz;
    s32 i;

    for (i = 0; i < 64; i++) {
        if (mapWallObjTaskData[i].use != 0 && mapWallObjTaskData[i].type == 6) {
            fldSetPlayerTargetData(&ix, &iz, x, z);
            if (ix >= mapWallObjTaskData[i].eveix0 && ix < mapWallObjTaskData[i].eveix1 &&
                iz >= mapWallObjTaskData[i].eveiy0 && iz < mapWallObjTaskData[i].eveiy1 &&
                mapWallObjTaskData[i].floor == fldGlobalBase.nameU) {
                return i;
            }
        }
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "code_0014B7B0", mapCallSetPlayerTargetData4);
#endif

INCLUDE_ASM(const s32, "code_0014B7B0", mapProcSequenceSub4);

INCLUDE_ASM(const s32, "code_0014B7B0", mapEveHitTaskFldSub);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallGetFloorY);

INCLUDE_ASM(const s32, "code_0014B7B0", mapEveHitTaskFldSub2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapGridHitAdd3);

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf17);

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf18);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallPositioningGuardSystem3);

/* NON_MATCHING: 220 of 224 words match.  */
#ifdef NON_MATCHING
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
        *objz = mapData24[i].z;
        fldGlobalBase.word154 = mapData24[i].id;
        fldGlobalBase.nowDoorSe = mapData24[i].se;
        return i;
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "code_0014B7B0", mapWarpGet_Pos);
#endif

INCLUDE_ASM(const s32, "code_0014B7B0", mapWarpChk_Idx);

INCLUDE_ASM(const s32, "code_0014B7B0", mapWarpGo_Idx);

INCLUDE_ASM(const s32, "code_0014B7B0", mapWarpGo_Idx_fld);

INCLUDE_ASM(const s32, "code_0014B7B0", mapWarpFloorRefresh);

INCLUDE_ASM(const s32, "code_0014B7B0", mapWallObjAdd);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallBitOn);

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

INCLUDE_ASM(const s32, "code_0014B7B0", mapWallGetUniq);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallWallGetUniq);

INCLUDE_RODATA(const s32, "code_0014B7B0", mapCallSetOtTable);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallSetOt);

INCLUDE_ASM(const s32, "code_0014B7B0", mapSetAreaRefreshSub2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjLightAdd);
