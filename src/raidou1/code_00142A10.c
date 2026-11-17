#include "common.h"
#include "mapData.h"
#include "fld.h"
#include "dds3ObjectBase.h"
#include "sdk_libc.h"
#include "fpu.h"
#include "code_r1_00128378.h"
#include "dds3Randomize.h"

INCLUDE_ASM(const s32, "code_00142A10", mapCallSetOt6);

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

INCLUDE_ASM(const s32, "code_00142A10", mapHitAidaCalc);

/* NON_MATCHING: 108 of 132 words match. Fragile: shifts with the count of declarations in fld.h and mapData.h. */
#ifdef NON_MATCHING
void mapHamidasiCheck(void) {
    f32 cx;
    f32 cz;
    f32 outx;
    f32 outz;
    f32 r;
    s32 hit;
    s32 attr;

    r = 70.0f;
    cx = fldGlobalBase.focusX;
    cz = fldGlobalBase.focusZ;
    if (fldGlobalBase.amSBlock == 13) {
        r = 50.0f;
    }
    if (fldGlobalBase.amSBlock == 31) {
        r = 50.0f;
    }
    if (fldGlobalBase.nakamaMode == 0 && fldGlobalBase.amSBlock == 2 && fldGlobalBase.nameU == 0) {
        if (mapHamidasiMoveBoxHit(0, &cx, &cz, r)) {
            fldGlobalBase.focusX = cx;
            fldGlobalBase.focusZ = cz;
        }
        if (mapHamidasiMoveBoxHit(1, &cx, &cz, r)) {
            fldGlobalBase.focusX = cx;
            fldGlobalBase.focusZ = cz;
        }
        if (mapHamidasiMoveBoxHit(2, &cx, &cz, r)) {
            fldGlobalBase.focusX = cx;
            fldGlobalBase.focusZ = cz;
        }
        if (mapHamidasiMoveBoxHit(3, &cx, &cz, r)) {
            fldGlobalBase.focusX = cx;
            fldGlobalBase.focusZ = cz;
        }
        if (mapHamidasiMoveBoxHit(4, &cx, &cz, r)) {
            fldGlobalBase.focusX = cx;
            fldGlobalBase.focusZ = cz;
        }
    }
    hit = mapHitCalc(0, fldGlobalBase.focusX, fldGlobalBase.focusZ, r, &cx, &cz);
    if (hit == -1) {
        cx = fldGlobalBase.floatA8;
        cz = fldGlobalBase.floatB0;
    }
    if (hit != 0) {
        fldGlobalBase.focusX = cx;
        fldGlobalBase.focusZ = cz;
    }
    if (fldGlobalBase.nakamaMode != 0) {
        attr = mapGetAttr_DevilWall(fldGlobalBase.focusX, fldGlobalBase.focusZ);
        if ((u8)attr != 0) {
            mapHitWorkCalcSetupSub(&outx, &outz, cx, cz);
            mapSeWallHit(mapHamidasiCheckSub(&cx, &cz, 20.0f, outx, outz, attr));
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00142A10", mapHamidasiCheck);
#endif
