#include "common.h"
#include "fld.h"
#include "mapData.h"
#include "fldProcess.h"
#include "sdfTex.h"
#include "code_0016A7D8.h"
#include "ps2asm.h"
#include "code_00195698.h"
#include "code_0016AD90.h"
#include "sdfVu0.h"

INCLUDE_ASM(const s32, "code_0017A2B0", fldCallFindSceneEntryData);

INCLUDE_ASM(const s32, "code_0017A2B0", fldMapAllAmap);

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

void mapAutoMapCalcPosLimit(s32 dx, s32 dz) {
    s32 max;
    s32 min;

    mapAutoMapUpdateLimit(am_floor);
    if (am_max_x - am_min_x > 32) {
        max = am_max_x - 32;
        min = am_min_x - 32;
        am_top_x += dx;
        if (am_top_x >= max) {
            am_top_x = max;
            draw_flags &= ~0x20;
        } else {
            draw_flags |= 0x20;
        }
        if (am_top_x <= min) {
            am_top_x = min;
            draw_flags &= ~0x10;
        } else {
            draw_flags |= 0x10;
        }
    } else {
        am_top_x = am_min_x - ((64 - (am_max_x - am_min_x)) >> 1);
        draw_flags &= ~0x10;
        draw_flags &= ~0x20;
    }
    if (am_max_z - am_min_z > 28) {
        max = am_max_z - 28;
        min = am_min_z - 28;
        am_top_z += dz;
        if (am_top_z >= max) {
            am_top_z = max;
            draw_flags &= ~0x8;
        } else {
            draw_flags |= 0x8;
        }
        if (am_top_z <= min) {
            am_top_z = min;
            draw_flags &= ~0x4;
        } else {
            draw_flags |= 0x4;
        }
    } else {
        draw_flags &= ~0x4;
        draw_flags &= ~0x8;
        am_top_z = am_min_z - ((limit_ofs[3] - limit_ofs[1] - (am_max_z - am_min_z)) >> 1) - limit_ofs[1];
    }
}

s32 fldDrawTopSub(void) {
    return draw_flags;
}

INCLUDE_ASM(const s32, "code_0017A2B0", fldCallBitCheck3);

INCLUDE_ASM(const s32, "code_0017A2B0", fldLeaf25);

INCLUDE_ASM(const s32, "code_0017A2B0", mapFunc5);

INCLUDE_ASM(const s32, "code_0017A2B0", mapFunc6);

INCLUDE_ASM(const s32, "code_0017A2B0", mapCallSin11);

INCLUDE_ASM(const s32, "code_0017A2B0", mapCallSin12);

void mapCallCutGetCam(s32 x0, s32 y0, s32 x1, s32 y1) {
    sdfVector4_t a;
    sdfVector4_t b;
    sdfVector4_t dir;
    u32 col[3];
    f32 angle;
    f32 vx;
    f32 vy;
    f32 fx0;
    f32 fy0;
    f32 fx1;
    f32 fy1;

    fx0 = x0;
    fy0 = y0;
    fx1 = x1;
    fy1 = y1;
    if (mapCallCutGetCamData + 0.1f > 6.28f) {
        mapCallCutGetCamData = 0.0f;
    } else {
        mapCallCutGetCamData += 0.1f;
    }
    mapCutGetCam(&a, &b, &angle);
    a.x = fx0;
    b.x = fx1;
    b.z = fy1;
    a.y = 0.0f;
    a.z = fy0;
    angle *= 0.75f;
    a.w = 0.0f;
    b.y = 0.0f;
    b.w = 0.0f;
    SDF_VU0_LOADVECTOR(&b);
    SDF_VU0_LOADVECTOR_B(&a);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR_BITS(224.0f);
    SDF_VU0_STOREVECTOR(&dir);
    fx1 = fx0 + dir.x;
    fy1 = fy0 + dir.z;
    SDF_VU0_LOADVECTOR_B(&a);
    SDF_VU0_LOADIDENTITY();
    RotateMatrixY(angle);
    SDF_VU0_LOADVECTOR(&dir);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&a);
    SDF_VU0_LOADIDENTITY();
    RotateMatrixY(-angle);
    SDF_VU0_LOADVECTOR(&dir);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&b);
    itfGetVectorPosBezierPt3(&vx, &vy, a.x, a.z, b.x, b.z, fx1, fy1, 0.5f);
    col[0] = 0x235A8296;
    col[1] = 0x0A5A8296;
    col[2] = 0x0A5A8296;
    itfDrawFanBezierPt3(fx0, fy0, a.x, a.z, b.x, b.z, vx, vy, col[0], col[1], col[2], 103);
}
