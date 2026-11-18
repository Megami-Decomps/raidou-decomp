#include "common.h"
#include "mapData.h"
#include "mapLbWork.h"
#include "sdfTex.h"
#include "fld.h"
#include "sbtlWork.h"
#include "dds3CameraObjectBasic.h"
#include "kernel_draw.h"
#include "ps2asm.h"
#include "dds3KernelDraw.h"
#include "sdfVu0.h"
#include "fldProcess.h"

INCLUDE_ASM(const s32, "code_0015D870", mapMapSetAreaRefresh);

INCLUDE_ASM(const s32, "code_0015D870", mapMapSetEnc);

INCLUDE_ASM(const s32, "code_0015D870", mapMapSetForm);

void mapEncPointGet(s16 point_id, f32 *x, f32 *y, f32 *z, f32 *r) {
    MpbPoint_t *pt;
    s32 i;
    f32 pos[2];
    f32 py;

    *x = 0.0f;
    *y = 0.0f;
    *z = 0.0f;
    *r = 0.0f;
    if (mapData112 == NULL) {
        return;
    }
    pt = pMpbPointNow;
    for (i = 0; i < mapData112->num_encpos; i++, pt++) {
        if (pt->id == point_id) {
            PositioningGuardSystem(pt->x, pt->y, &pos[0], &pos[1]);
            switch (pt->ofs) {
            case 0:
                pos[0] += 50.0f;
                pos[1] -= 50.0f;
                break;
            case 1:
                break;
            case 2:
                pos[0] += 100.0f;
                break;
            case 3:
                pos[1] -= 100.0f;
                break;
            case 4:
                pos[0] += 100.0f;
                pos[1] -= 100.0f;
                break;
            }
            py = mapGetY(pos[0], pos[1]);
            *x = pos[0];
            *y = py;
            *z = pos[1];
            *r = -mapKazeLabelTable[pt->dir];
            return;
        }
    }
}

void fldSetPlayerTargetData(s32 *ix, s32 *iz, f32 x, f32 z) {
    f32 dz = -(z - mapData104) / 100.0f;

    *ix = (x - mapData103) / 100.0f;
    *iz = dz;
}

INCLUDE_ASM(const s32, "code_0015D870", fldGetNowCutID);

void fldProcSequenceSub4(f32 x, f32 z, f32 *outX, f32 *outZ) {
    f32 dz = -(z - mapData104) / 100.0f;

    *outX = (x - mapData103) / 100.0f;
    *outZ = dz;
}

INCLUDE_ASM(const s32, "code_0015D870", fldProcSequenceSub5);

void PositioningGuardSystem(s32 ix, s32 iy, f32 *x, f32 *z) {
    *x = mapData103 + (f32)(ix * 100);
    *z = mapData104 - (f32)(iy * 100);
}

INCLUDE_ASM(const s32, "code_0015D870", fldWallObjTaskSub);

INCLUDE_ASM(const s32, "code_0015D870", mapLeaf21);

INCLUDE_ASM(const s32, "code_0015D870", mapFileLoadSub6);

void mapHitWorkCalcSetupSub(f32 *outx, f32 *outz, f32 x, f32 z) {
    s32 ix = (x - mapData103) / 100.0f;
    s32 iz = (z - mapData104) / 100.0f;

    *outx = (f32)(ix * 100) + mapData103;
    *outz = (f32)(iz * 100) + mapData104;
}

INCLUDE_ASM(const s32, "code_0015D870", mapGetLight);

INCLUDE_ASM(const s32, "code_0015D870", mapCallGetNowCutID);

INCLUDE_ASM(const s32, "code_0015D870", mapGetHit);

INCLUDE_ASM(const s32, "code_0015D870", mapGetAttr);

INCLUDE_ASM(const s32, "code_0015D870", mapGetAttr_DevilWall);

INCLUDE_ASM(const s32, "code_0015D870", mapGetAttr_Btl);

s32 fldGetFldBgNameList(f32 x, f32 z) {
    MpbAreaHead_t *head;
    s32 ix;
    s32 iz;

    head = mapMapSetAreaData18;
    if (pMapAreaNow == NULL) {
        return 0;
    }
    if (head == NULL) {
        return 0;
    }
    fldSetPlayerTargetData(&ix, &iz, x, z);
    return mapMapSetAreaData22[head->rnd_encw * iz + ix];
}

INCLUDE_ASM(const s32, "code_0015D870", fldRefMapAreaNow);

INCLUDE_ASM(const s32, "code_0015D870", mapRefMapAreaNow);

s32 mapGetCut_FXY(f32 x, f32 z) {
    s32 ix;
    s32 iz;
    s32 cut;

    fldSetPlayerTargetData(&ix, &iz, x, z);
    cut = pMapColmNow[pMapAreaNow->w * iz + ix].cut & 0x7F;
    if (fldGlobalBase.word21C == 1 && cut == 27) {
        cut = 33;
    }
    return fldSpecificSceneOnVisible(cut);
}

s32 mapProcSequenceSub5(f32 x, f32 z) {
    s32 i;

    for (i = 0; i < 64; i++) {
        if (mapData27[i].use != 0 && mapData27[i].x0 < x && x < mapData27[i].x1 && z < mapData27[i].z0 &&
            mapData27[i].z1 < z && mapData27[i].type == 2) {
            return mapData27[i].basyo;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_0015D870", mapGetFloorY);

f32 mapGetY(f32 x, f32 z) {
    return mapGetFloorY(-1, x, z);
}

INCLUDE_ASM(const s32, "code_0015D870", mapCallSetPlayerTargetData5);

INCLUDE_ASM(const s32, "code_0015D870", mapCallSetPlayerTargetData6);

INCLUDE_ASM(const s32, "code_0015D870", mapCutSet_From_Pos);

INCLUDE_ASM(const s32, "code_0015D870", mapCatChkMain);

INCLUDE_ASM(const s32, "code_0015D870", mapCallGetNowCutID2);

INCLUDE_ASM(const s32, "code_0015D870", mapCatGetNakamaPos);

INCLUDE_ASM(const s32, "code_0015D870", mapCallBitOff);

INCLUDE_ASM(const s32, "code_0015D870", mapCatChkMain_fld);

void mapCutSet_From_Idx(void) {
    fldMapAreaHead_t *area;
    MapColm_t *colm;
    s32 ix;
    s32 iz;
    volatile f32 target[4] QWORD_ALIGNED;
    volatile f32 eye[4] QWORD_ALIGNED;

    area = pMapAreaNow;
    colm = pMapColmNow;
    if (area == NULL || colm == NULL) {
        return;
    }
    fldSetPlayerTargetData(&ix, &iz, mapCutSet_From_IdxData, mapCutSet_From_IdxData3);
    if (fldGlobalBase.cutState != 1) {
        return;
    }
    if (CheckStopBattle() == 0) {
        return;
    }
    fldGlobalBase.cutIdx = colm[area->w * iz + ix].cut & 0x7F;
    if (fldGlobalBase.hitMatterIndex > 0) {
        fldGlobalBase.cutIdx = fldGlobalBase.hitMatterIndex;
    }
    fldGlobalBase.cutState = 2;
    fldGlobalBase.cutIdxPrev = fldGlobalBase.cutIdx;
    mapCutSetTblID(fldGlobalBase.cutIdx);
    mapCutBgLoad_NB();
    mapCutSetCam(mapData97, 0);
    dds3CameraObjectBasicFourceCalc(mapData97);
    sdf3MakeScreenParam(&dds3ScreenParam);
    SDF_VU0_LOADVECTOR(&aViewTarget);
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR(600.0f);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_LOADVECTOR_B(gMapScrollReset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(target);
    SDF_VU0_LOADVECTOR(&aViewEye);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(eye);
    LookAtMatrix((const void *)target, (const void *)eye, &aViewDown);
    SDF_VU0_STOREMATRIX_NOMEM(&sdfViewMatrix);
    sdf3DSetCamera(&dds3ScreenParam, &sdfViewMatrix);
    sdf3MakeScreenParam(&dds3ScreenParam);
    mapBgSetScroll(mapCutSet_From_IdxData, mapCutSet_From_IdxData2, mapCutSet_From_IdxData3, 0);
    sdf3MakeScreenParam(&dds3ScreenParam);
    mapBgSetScroll(mapCutSet_From_IdxData, mapCutSet_From_IdxData2, mapCutSet_From_IdxData3, 0);
    RestartBattle();
}

INCLUDE_ASM(const s32, "code_0015D870", mapCallWarpGetPos);

INCLUDE_ASM(const s32, "code_0015D870", mapProcSequenceSub6);

INCLUDE_ASM(const s32, "code_0015D870", mapProcSequenceSub7);

INCLUDE_ASM(const s32, "code_0015D870", mapCallSetOt7);

INCLUDE_ASM(const s32, "code_0015D870", mapCallSetOt8);

void mapCallTexAllocFromHead(void) {
    fldTitleMiniTex2 = sdfTexAllocFromHead(128, 128, 19, 0, 0, 1);
    sdfTexMakeGIFPacket(fldTitleMiniTex2);
    fldTitleMiniWork2 = sdfTexAllocFromHead(128, 128, 20, 0, 0, 1);
    sdfTexMakeGIFPacket(fldTitleMiniWork2);
}
