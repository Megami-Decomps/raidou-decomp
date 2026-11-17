#include "common.h"
#include "fldNpcPath.h"
#include "fpu.h"
#include "code_00174790.h"
#include "dds3KernelCore.h"

INCLUDE_ASM(const s32, "code_001870F0", mapCallPrimAllocDrawTag);

INCLUDE_ASM(const s32, "code_001870F0", mapCallPrimAllocDrawTag2);

INCLUDE_ASM(const s32, "code_001870F0", mapLeaf27);

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

INCLUDE_ASM(const s32, "code_001870F0", mapLeaf28);

INCLUDE_ASM(const s32, "code_001870F0", mapBtlEndWaitInitSub);

dds3ProcessID_t *mapFadeProc(void) {
    return dds3StartProcess(fldEncDrawName2, 0x2B0D, 0, 1, mapFunc11, NULL, NULL);
}

INCLUDE_ASM(const s32, "code_001870F0", GetElement);

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

INCLUDE_ASM(const s32, "code_001870F0", mapNpcPathMake_Rot);

INCLUDE_ASM(const s32, "code_001870F0", mapCallAtan22);

INCLUDE_RODATA(const s32, "code_001870F0", fldEncDrawName2);

INCLUDE_RODATA(const s32, "code_001870F0", mapNpcPathMake_RotTable);

