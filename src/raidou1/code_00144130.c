#include "common.h"
#include "mapData.h"
#include "fld.h"
#include "dds3ObjectBase.h"
#include "sdk_libc.h"
#include "fpu.h"
#include "code_r1_00128378.h"
#include "dds3Randomize.h"
#include "fldProcess.h"

s32 mapAddHukidashi(s16 uniq, s32 major, s32 minor, s32 motion, u32 cut_bit, s32 cash, f32 x, f32 y, f32 z, f32 r, f32 s) {
    return mapObjFloorAdd_Cash(-1, uniq, major, minor, motion, cut_bit, cash, x, y, z, r, s);
}

INCLUDE_ASM(const s32, "code_00144130", mapLeaf13);

INCLUDE_ASM(const s32, "code_00144130", mapObjectRefreshSub);

s32 mapObjCheck(s16 uniq) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].uniq == uniq) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "code_00144130", mapObjDel);

INCLUDE_ASM(const s32, "code_00144130", mapObjAllDel);

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

INCLUDE_ASM(const s32, "code_00144130", mapLeaf14);

INCLUDE_ASM(const s32, "code_00144130", nbStartEventPacks);

INCLUDE_ASM(const s32, "code_00144130", mapObjColor);

INCLUDE_ASM(const s32, "code_00144130", mapFileLoadSub3);

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

INCLUDE_ASM(const s32, "code_00144130", mapObjNpcKuseStop);

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

INCLUDE_ASM(const s32, "code_00144130", mapObjMask);

INCLUDE_ASM(const s32, "code_00144130", mapLeaf15);

INCLUDE_ASM(const s32, "code_00144130", mapObjNoClip);

/* NON_MATCHING: 16 of 370 words match. 9 of 370 words match; same blocks, branch-likely and register choices differ. */
#ifdef NON_MATCHING
void mapObjOnOff(void) {
    MAP_OBJ_t *obj;
    s32 i;
    s32 cut;
    u16 uniq;

    fldGlobalBase.word2D8 = 0;
    mapGenbaPosEffClipSet();
    for (i = 0; i < 128; i++) {
        obj = &mapObjTaskData2[i];
        if (obj->use != 1) {
            continue;
        }
        if (obj->basic == NULL) {
            continue;
        }
        cut = fldGlobalBase.hitMatterIndex != 0 ? fldGlobalBase.hitMatterIndex : fldGlobalBase.cutIdx;
        if (obj->visible == 0) {
            continue;
        }
        uniq = obj->uniq;
        if (uniq == 0xE028) {
            if (fldGlobalBase.cutIdx == 10) {
                dds3ObjectBaseOnFlag(obj->basic, 1);
            } else {
                dds3ObjectBaseOffFlag(obj->basic, 1);
            }
            continue;
        }
        if (fldGlobalBase.amSBlock == 22 && fldGlobalBase.nameU == 2 && (u16)(uniq - 426) < 5) {
            dds3ObjectBaseOnFlag(obj->basic, 1);
            continue;
        }
        if (fldGlobalBase.amSBlock == 13) {
            if (fldGlobalBase.nameU == 3 && obj->uniq == 461) {
                if (fldGlobalBase.cutIdx == 9) {
                    dds3ObjectBaseOnFlag(obj->basic, 1);
                } else {
                    dds3ObjectBaseOffFlag(obj->basic, 1);
                }
                continue;
            }
            if (fldGlobalBase.nameU == 4) {
                if (obj->uniq == 461) {
                    if (fldGlobalBase.cutIdx == 9) {
                        dds3ObjectBaseOnFlag(obj->basic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(obj->basic, 1);
                    }
                    continue;
                }
                if (obj->uniq == 4150) {
                    if (fldGlobalBase.cutIdx == 16) {
                        dds3ObjectBaseOnFlag(obj->basic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(obj->basic, 1);
                    }
                    continue;
                }
            }
            if (fldGlobalBase.nameU == 5) {
                if (obj->uniq == 461) {
                    if (fldGlobalBase.cutIdx == 13) {
                        dds3ObjectBaseOnFlag(obj->basic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(obj->basic, 1);
                    }
                    continue;
                }
                if (obj->uniq == 4148) {
                    if (fldGlobalBase.cutIdx == 13) {
                        dds3ObjectBaseOnFlag(obj->basic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(obj->basic, 1);
                    }
                    continue;
                }
                if (obj->uniq == 4149) {
                    if (fldGlobalBase.cutIdx == 13) {
                        dds3ObjectBaseOnFlag(obj->basic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(obj->basic, 1);
                    }
                    continue;
                }
                if (obj->uniq == 4150) {
                    if (fldGlobalBase.cutIdx == 13) {
                        dds3ObjectBaseOnFlag(obj->basic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(obj->basic, 1);
                    }
                    continue;
                }
            }
            if (fldGlobalBase.nameU == 9) {
                if (mapObjTaskData2[i].uniq == 0 && mapObjTaskData2[i].x > 1049.0f && mapObjTaskData2[i].x < 1051.0f &&
                    mapObjTaskData2[i].z > 5149.0f && mapObjTaskData2[i].z < 5151.0f) {
                    dds3ObjectBaseOffFlag(mapObjTaskData2[i].basic, 1);
                    continue;
                }
                if (mapObjTaskData2[i].uniq == 0 && mapObjTaskData2[i].x > 549.0f && mapObjTaskData2[i].x < 551.0f &&
                    mapObjTaskData2[i].z > 6149.0f && mapObjTaskData2[i].z < 6151.0f) {
                    if (fldGlobalBase.cutIdx == 31) {
                        dds3ObjectBaseOnFlag(mapObjTaskData2[i].basic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(mapObjTaskData2[i].basic, 1);
                    }
                    continue;
                }
                if (mapObjTaskData2[i].uniq == 4157) {
                    if (fldGlobalBase.cutIdx == 31) {
                        dds3ObjectBaseOnFlag(mapObjTaskData2[i].basic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(mapObjTaskData2[i].basic, 1);
                    }
                    continue;
                }
                if ((u16)(mapObjTaskData2[i].uniq - 4164) < 3 ||
                    mapObjTaskData2[i].uniq == 443 || mapObjTaskData2[i].uniq == 514) {
                    if (fldGlobalBase.cutIdx == 29) {
                        dds3ObjectBaseOnFlag(obj->basic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(obj->basic, 1);
                    }
                    continue;
                }
                if (mapObjTaskData2[i].uniq == 0 && mapObjTaskData2[i].x > -1451.0f && mapObjTaskData2[i].x < -1449.0f &&
                    mapObjTaskData2[i].z > 2149.0f && mapObjTaskData2[i].z < 2151.0f) {
                    if (fldGlobalBase.cutIdx == 29) {
                        dds3ObjectBaseOnFlag(mapObjTaskData2[i].basic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(mapObjTaskData2[i].basic, 1);
                    }
                    continue;
                }
            }
            if (fldGlobalBase.nameU == 10) {
                if (mapObjTaskData2[i].uniq == 4157) {
                    if (fldGlobalBase.cutIdx == 24) {
                        dds3ObjectBaseOnFlag(mapObjTaskData2[i].basic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(mapObjTaskData2[i].basic, 1);
                    }
                    continue;
                }
                if (mapObjTaskData2[i].uniq == 0 && mapObjTaskData2[i].x > -1951.0f && mapObjTaskData2[i].x < -1949.0f &&
                    mapObjTaskData2[i].z > 6149.0f && mapObjTaskData2[i].z < 6151.0f) {
                    if (fldGlobalBase.cutIdx == 24) {
                        dds3ObjectBaseOnFlag(mapObjTaskData2[i].basic, 1);
                    } else {
                        dds3ObjectBaseOffFlag(mapObjTaskData2[i].basic, 1);
                    }
                    continue;
                }
            }
        }
        if (mapObjTaskData2[i].noclip) {
            dds3ObjectBaseOffFlag(mapObjTaskData2[i].basic, 1);
            continue;
        }
        if (mapBigClipChkF(cut, mapObjTaskData2[i].x, mapObjTaskData2[i].y, mapObjTaskData2[i].z)) {
            if (((mapObjTaskData2[i].cut_bit >> fldGlobalBase.cutIdx) & 1) == 0) {
                dds3ObjectBaseOnFlag(mapObjTaskData2[i].basic, 1);
                continue;
            }
            if (mapObjTaskData2[i].model_id1 == 3 && mapObjTaskData2[i].uniq >= 0) {
                fldGlobalBase.word2D8++;
            }
            dds3ObjectBaseOffFlag(obj->basic, 1);
        } else {
            dds3ObjectBaseOnFlag(mapObjTaskData2[i].basic, 1);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00144130", mapObjOnOff);
#endif

INCLUDE_ASM(const s32, "code_00144130", mapObjPos);

INCLUDE_ASM(const s32, "code_00144130", mapObjPosH);

INCLUDE_ASM(const s32, "code_00144130", mapObjRot);

INCLUDE_ASM(const s32, "code_00144130", mapObjRotQ);

INCLUDE_ASM(const s32, "code_00144130", mapObjScale);

INCLUDE_ASM(const s32, "code_00144130", mapObjMotionCng);

INCLUDE_ASM(const s32, "code_00144130", mapObjMotionSpeed);

INCLUDE_ASM(const s32, "code_00144130", mapObjMotionEndCheck);

INCLUDE_ASM(const s32, "code_00144130", mapObjMotionPause);

INCLUDE_ASM(const s32, "code_00144130", mapObjMotionPauseAll);

INCLUDE_ASM(const s32, "code_00144130", mapObjMotionPauseOffAll);

INCLUDE_ASM(const s32, "code_00144130", mapObjGetFloorBasic);

dds3Basic_t *mapObjGetBasic(s16 uniq) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].use == 1 && mapObjTaskData2[i].basic != NULL && mapObjTaskData2[i].uniq == uniq) {
            return mapObjTaskData2[i].basic;
        }
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_00144130", mapObjGetModelMinID);

INCLUDE_RODATA(const s32, "code_00144130", mapObjRotTable);

INCLUDE_RODATA(const s32, "code_00144130", mapObjScaleTable);

