#include "common.h"
#include "fld.h"
#include "mapData.h"
#include "field.h"
#include "fldProcess.h"
#include "sdfTex.h"
#include "sdfMemory.h"
#include "code_0016A7D8.h"

void fldCallFindSceneEntryData(f32 x, f32 z, f32 x2, f32 z2, s32 scene, s32 id, s32 rot) {
    am_map = scene;
    mapCallSetOt2Data7 = x;
    mapCallSetOt2Data8 = z;
    fldData34 = x2;
    fldData35 = z2;
    mapCallSetOt2Data9 = rot;
    am_floor = id;
    fldData36 = am_floor = fldFindSceneEntryData(id);
    mapCallSetOt2Data10 = fldFindSceneEntryData(fldGlobalBase.word194);
    draw_flags = 0;
    mapFunc();
}

void fldMapAllAmap(void) {
    sdfMemHandle_t *file;
    u32 tex;

    am_floor = fldData36;
    tex = 0;
    if (!fldGlobalBase.nakamaMode) {
        am_top_x = mapCallSetOt2Data7 - 32.0f;
        am_top_z = mapCallSetOt2Data8 - 28.0f;
    } else {
        am_top_x = fldData34 - 32.0f;
        am_top_z = fldData35 - 28.0f;
    }
    mapAutoMapCalcPosLimit(0, 0);
    file = sdfDevLoadFileEx("/map/all/amap.tmx", &tex, NULL);
    effQueuedResourceNameRecord4 = sdfTexSet(EE_ADDR_AS(TMX_HEADER_t *, tex));
    sdfMemFreeDelayedDirect(file);
    mapFunc10();
}

void fldCallTexFreeDelayed(void) {
    if (effQueuedResourceNameRecord4 != NULL) {
        sdfTexFreeDelayed(effQueuedResourceNameRecord4);
        effQueuedResourceNameRecord4 = NULL;
    }
}

s32 fldLeaf24(void) {
    return effQueuedResourceNameRecord4 != NULL;
}

s32 mapAutoMapUpdateLimit(s32 floor) {
    mapOt2Block_t *block;
    s32 w;
    s32 h;
    mapOt2Cell_t *grid;
    s32 x;
    s32 z;
    s32 kind;
    s32 num;
    s32 maxX;
    s32 maxZ;
    s32 minX;
    s32 minZ;

    block = mapCallSetOt2Data2[floor];
    if (block == NULL) {
        return -1;
    }
    maxX = 0;
    maxZ = 0;
    w = block->w;
    minX = 512;
    minZ = 448;
    h = block->h;
    grid = mapData35[floor];
    mapAM_MaskSetCurrent(am_map, floor);
    num = 0;
    for (x = 0; x < w; x++) {
        for (z = 0; z < h; z++) {
            kind = grid[x + z * w].unk01 & 0x1F;
            if (mapAM_MaskChk_ID(am_map, floor, grid[x + z * w].id) && kind) {
                if (maxX < x) {
                    maxX = x;
                }
                if (x < minX) {
                    minX = x;
                }
                num++;
                if (maxZ < z) {
                    maxZ = z;
                }
                if (z < minZ) {
                    minZ = z;
                }
            }
        }
    }
    am_max_x = maxX;
    am_max_z = maxZ;
    am_min_x = minX;
    am_min_z = minZ;
    return num;
}

INCLUDE_ASM(const s32, "code_0016A7D8", mapAutoMapCalcPosLimit);

s32 fldDrawTopSub(void) {
    return draw_flags;
}
