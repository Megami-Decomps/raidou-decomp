#include "common.h"
#include "dds3KernelCore.h"
#include "fld.h"
#include "fldNpcPath.h"
#include "fpu.h"
#include "ps2asm.h"
#include "sdfMath.h"
#include "sdfQuat.h"
#include "code_00174790.h"
extern sdfVector4_t mapNpcPathMake_RotTable;

INCLUDE_ASM(const s32, "code_00174790", mapCallPrimAllocDrawTag);

INCLUDE_ASM(const s32, "code_00174790", mapCallPrimAllocDrawTag2);

INCLUDE_ASM(const s32, "code_00174790", mapLeaf27);

void mapProcSequenceSub12(s32 arg) {
    mapData147 = arg;
}

s32 mapFunc11(dds3ProcessID_t *id) {
    if (mapData148 == 0) {
        return 0;
    }
    if (mapData147 == 0) {
        mapFunc11Data2++;
    }
    if (mapData147 == 1) {
        if (mapFunc11Data > 0) {
            mapFunc11Data--;
        } else {
            mapLeaf28();
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "code_00174790", mapLeaf28);

INCLUDE_ASM(const s32, "code_00174790", mapBtlEndWaitInitSub);

dds3ProcessID_t *mapFadeProc(void) {
    return dds3StartProcess(fldEncDrawName2, 0x2B0D, 0, 1, mapFunc11, NULL, NULL);
}

INCLUDE_ASM(const s32, "code_00174790", GetElement);

void mapNpcPathMake_KeyFrame(s32 num, f32 spd, mapNpcPathData_t *pathdata) {
    sdfVector3_t *pItem;
    u32 *pKey = pathdata->pKey;
    u32 key = 0;
    s32 i;

    pKey[0] = 0;
    pItem = pathdata->item.pItem;
    for (i = 1; i < num; i++) {
        f32 dx = pItem[i].x - pItem[i - 1].x;
        f32 dy = pItem[i].y - pItem[i - 1].y;
        f32 dz = pItem[i].z - pItem[i - 1].z;

        key += (s32)(fsqrtf(dx * dx + dy * dy + dz * dz) / spd);
        pKey[i] = key;
    }
}

/* NON_MATCHING: 50 of 70 words match.  */
#ifdef NON_MATCHING
void mapNpcPathMake_Rot(s32 num, mapNpcPathData_t *pathdataT, mapNpcPathData_t *pathdataR) {
    sdfVector4_t axis;
    sdfVector4_t *pItemR;
    sdfVector3_t *pItem;
    s32 i;

    axis = mapNpcPathMake_RotTable;
    pItemR = pathdataR->item.pItemR;
    pItem = pathdataT->item.pItem;
    for (i = 1; i < num; i++) {
        f32 dx = pItem[i].x - pItem[i - 1].x;
        f32 dz = pItem[i].z - pItem[i - 1].z;

        if (dx > 0.099999994f || dx < -0.099999994f || dz > 0.099999994f || dz < -0.099999994f) {
            f32 angle = sdfAtan2(dx, dz);

            SDF_VU0_LOADVECTOR(&axis);
            FromRotation(3.13999963f - angle);
            SDF_VU0_STOREVECTOR(&pItemR[i]);
        }
    }
    SDF_VU0_LOADVECTOR(&pItemR[1]);
    SDF_VU0_STOREVECTOR(pItemR);
}
#else
INCLUDE_ASM(const s32, "code_00174790", mapNpcPathMake_Rot);
#endif

INCLUDE_ASM(const s32, "code_00174790", mapCallAtan22);

INCLUDE_RODATA(const s32, "code_00174790", fldEncDrawName2);

INCLUDE_RODATA(const s32, "code_00174790", mapNpcPathMake_RotTable);

