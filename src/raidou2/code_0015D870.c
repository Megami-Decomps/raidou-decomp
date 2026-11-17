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

INCLUDE_ASM(const s32, "code_0015D870", mapEncPointGet);

void fldSetPlayerTargetData(s32 *ix, s32 *iz, f32 x, f32 z) {
    f32 dz = -(z - mapData104) / 100.0f;

    *ix = (x - mapData103) / 100.0f;
    *iz = dz;
}

INCLUDE_ASM(const s32, "code_0015D870", fldGetNowCutID);

INCLUDE_ASM(const s32, "code_0015D870", fldProcSequenceSub4);

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

INCLUDE_ASM(const s32, "code_0015D870", mapGetCut_FXY);

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

INCLUDE_ASM(const s32, "code_0015D870", mapCutSet_From_Idx);

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
