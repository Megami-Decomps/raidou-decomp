#include "common.h"
#include "mapData.h"
#include "fld.h"
#include "dds3ObjectBase.h"
#include "sdk_libc.h"
#include "fpu.h"
#include "code_r1_00128378.h"
#include "dds3Randomize.h"

s32 mapAddHukidashi(s16 uniq, s32 major, s32 minor, s32 motion, u32 cut_bit, s32 cash, f32 x, f32 y, f32 z, f32 r, f32 s) {
    return mapObjFloorAdd_Cash(-1, uniq, major, minor, motion, cut_bit, cash, x, y, z, r, s);
}

INCLUDE_ASM(const s32, "code_00144130", mapLeaf13);

INCLUDE_ASM(const s32, "code_00144130", mapObjectRefreshSub);

INCLUDE_ASM(const s32, "code_00144130", mapObjCheck);

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

INCLUDE_ASM(const s32, "code_00144130", mapObjOnOff);

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

