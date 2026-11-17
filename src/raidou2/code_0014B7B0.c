#include "common.h"
#include "mapData.h"
#include "fld.h"

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallSetOt6);

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

INCLUDE_ASM(const s32, "code_0014B7B0", mapHitAidaCalc);

INCLUDE_ASM(const s32, "code_0014B7B0", mapHamidasiCheck);

INCLUDE_ASM(const s32, "code_0014B7B0", mapFileLoadSub2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf12);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCASHDATA);

INCLUDE_ASM(const s32, "code_0014B7B0", mdlCallMemGetSize);

INCLUDE_ASM(const s32, "code_0014B7B0", mdlCallMemGetSize2);

INCLUDE_ASM(const s32, "code_0014B7B0", mdlCallMemFreeDirect);

INCLUDE_ASM(const s32, "code_0014B7B0", mdlGetModelName);

INCLUDE_ASM(const s32, "code_0014B7B0", mdlCallMemGetInfo);

INCLUDE_ASM(const s32, "code_0014B7B0", sbtlCheckEndEscape);

INCLUDE_RODATA(const s32, "code_0014B7B0", mapObjFloorAdd_CashTable);

INCLUDE_RODATA(const s32, "code_0014B7B0", mapObjFloorAdd_CashTable2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjFloorAdd_Cash);

INCLUDE_ASM(const s32, "code_0014B7B0", mapAddHukidashi);

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf13);

s32 mapObjectRefreshSub(s16 uniq, s32 major, s32 minor, s32 motion, u32 cut_bit, f32 x, f32 y, f32 z, f32 r, f32 s) {
    return mapObjFloorAdd_Cash(-1, uniq, major, minor, motion, cut_bit, 0, x, y, z, r, s);
}

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjCheck);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjDel);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjAllDel);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjVisible);

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf14);

INCLUDE_ASM(const s32, "code_0014B7B0", nbStartEventPacks);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjColor);

INCLUDE_ASM(const s32, "code_0014B7B0", mapFileLoadSub3);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjNpcKuse);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjNpcKuseStop);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjNpcKuseScriptStop);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjHitSize);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjFloor);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjMask);

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf15);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjNoClip);

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

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjGetBasic);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjGetModelMinID);

INCLUDE_RODATA(const s32, "code_0014B7B0", mapObjRotTable);

INCLUDE_RODATA(const s32, "code_0014B7B0", mapObjScaleTable);

INCLUDE_RODATA(const s32, "code_0014B7B0", mapObjTaskTable2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjTask);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjEveHitSet);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjEveHitSet2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallPositioningGuardSystem2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf16);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallBitCheck2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapFunc);

INCLUDE_ASM(const s32, "code_0014B7B0", mapAnaLabel);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallSetPlayerTargetData2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallSetPlayerTargetData3);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallSetPlayerTargetData4);

INCLUDE_ASM(const s32, "code_0014B7B0", mapProcSequenceSub4);

INCLUDE_ASM(const s32, "code_0014B7B0", mapEveHitTaskFldSub);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallGetFloorY);

INCLUDE_ASM(const s32, "code_0014B7B0", mapEveHitTaskFldSub2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapGridHitAdd3);

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf17);

INCLUDE_ASM(const s32, "code_0014B7B0", mapLeaf18);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallPositioningGuardSystem3);

INCLUDE_ASM(const s32, "code_0014B7B0", mapWarpGet_Pos);

INCLUDE_ASM(const s32, "code_0014B7B0", mapWarpChk_Idx);

INCLUDE_ASM(const s32, "code_0014B7B0", mapWarpGo_Idx);

INCLUDE_ASM(const s32, "code_0014B7B0", mapWarpGo_Idx_fld);

INCLUDE_ASM(const s32, "code_0014B7B0", mapWarpFloorRefresh);

INCLUDE_ASM(const s32, "code_0014B7B0", mapWallObjAdd);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallBitOn);

INCLUDE_ASM(const s32, "code_0014B7B0", mapFileLoadSub4);

INCLUDE_ASM(const s32, "code_0014B7B0", mapWallGetUniq);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallWallGetUniq);

INCLUDE_RODATA(const s32, "code_0014B7B0", mapCallSetOtTable);

INCLUDE_ASM(const s32, "code_0014B7B0", mapCallSetOt);

INCLUDE_ASM(const s32, "code_0014B7B0", mapSetAreaRefreshSub2);

INCLUDE_ASM(const s32, "code_0014B7B0", mapObjLightAdd);
