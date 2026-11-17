#include "common.h"
#include "mapData.h"
#include "fld.h"
#include "dds3ObjectBase.h"
#include "sdk_libc.h"
#include "fpu.h"
#include "dds3KernelCore.h"
#include "code_r1_00128378.h"
#include "dds3Randomize.h"
#include "dds3KernelDraw.h"
#include "evtUnitManager.h"
#include "ps2asm.h"

void mapDebugLightSet(s32 lightid) {
    LightTbl_t *tbl;

    if (dds3SearchProcessName(mapStrLightEditUnit) == NULL) {
        tbl = &LightDataTbl[fldGlobalBase.amSBlock][lightid];
        mapData21.col[0] = tbl->col0[0];
        mapData21.col[1] = tbl->col0[1];
        mapData21.col[2] = tbl->col0[2];
        mapUnitSetLightData.col[0] = tbl->col1[0];
        mapUnitSetLightData.col[1] = tbl->col1[1];
        mapUnitSetLightData.col[2] = tbl->col1[2];
        mapUnitSetLightData2.col[0] = tbl->col2[0];
        mapUnitSetLightData2.col[1] = tbl->col2[1];
        mapUnitSetLightData2.col[2] = tbl->col2[2];
        fldSkyPlayerIdx[0] = tbl->cola[0];
        fldSkyPlayerIdx[1] = tbl->cola[1];
        fldSkyPlayerIdx[2] = tbl->cola[2];
        mapData21.dir[0] = tbl->dir0[0];
        mapData21.dir[1] = tbl->dir0[1];
        mapData21.dir[2] = tbl->dir0[2];
        mapUnitSetLightData.dir[0] = tbl->dir1[0];
        mapUnitSetLightData.dir[1] = tbl->dir1[1];
        mapUnitSetLightData.dir[2] = tbl->dir1[2];
        mapUnitSetLightData2.dir[0] = tbl->dir2[0];
        mapUnitSetLightData2.dir[1] = tbl->dir2[1];
        mapUnitSetLightData2.dir[2] = tbl->dir2[2];
    }
}

/* NON_MATCHING: 117 of 271 words match.  */
#ifdef NON_MATCHING
void mapUnitSetLight(dds3Basic_t *unit, s32 lightid, s32 intr) {
    evtUnit_t *h;
    LightTbl_t *tbl;
    f32 dir[4];

    if (unit == NULL) {
        return;
    }
    if (dds3SearchProcessName(mapStrLightEditUnit) != NULL) {
        h = evtGetUnitHandle2(unit);
        evtUnitLightOn(h);
        evtSetUnitLightAndAmbient3(h, intr,
            (s32)(mapData21.col[0] * 128.0f) | ((s32)(mapData21.col[2] * 128.0f) << 16) |
                ((s32)(mapData21.col[1] * 128.0f) << 8) | 0x80000000,
            (s32)(mapUnitSetLightData.col[0] * 128.0f) | ((s32)(mapUnitSetLightData.col[2] * 128.0f) << 16) |
                ((s32)(mapUnitSetLightData.col[1] * 128.0f) << 8) | 0x80000000,
            (s32)(mapUnitSetLightData2.col[0] * 128.0f) | ((s32)(mapUnitSetLightData2.col[2] * 128.0f) << 16) |
                ((s32)(mapUnitSetLightData2.col[1] * 128.0f) << 8) | 0x80000000,
            (s32)(fldSkyPlayerIdx[0] * 128.0f) | ((s32)(fldSkyPlayerIdx[2] * 128.0f) << 16) |
                ((s32)(fldSkyPlayerIdx[1] * 128.0f) << 8) | 0x80000000);
        dir[0] = mapData21.dir[0];
        dir[1] = mapData21.dir[1];
        dir[2] = mapData21.dir[2];
        dir[3] = 0.0f;
        SDF_VU0_LOADVECTOR(dir);
        evtSetUnitLightDir(h, intr);
        dir[0] = mapUnitSetLightData.dir[0];
        dir[1] = mapUnitSetLightData.dir[1];
        dir[2] = mapUnitSetLightData.dir[2];
        SDF_VU0_LOADVECTOR(dir);
        evtSetUnitLightDir1(h, intr);
        dir[0] = mapUnitSetLightData2.dir[0];
        dir[1] = mapUnitSetLightData2.dir[1];
        dir[2] = mapUnitSetLightData2.dir[2];
        SDF_VU0_LOADVECTOR(dir);
        evtSetUnitLightDir2(h, intr);
    } else {
        tbl = &LightDataTbl[fldGlobalBase.amSBlock][lightid];
        h = evtGetUnitHandle2(unit);
        evtUnitLightOn(h);
        evtSetUnitLightAndAmbient3(h, intr,
            (s32)(tbl->col0[0] * 128.0f) | ((s32)(tbl->col0[2] * 128.0f) << 16) |
                ((s32)(tbl->col0[1] * 128.0f) << 8) | 0x80000000,
            (s32)(tbl->col1[0] * 128.0f) | ((s32)(tbl->col1[2] * 128.0f) << 16) |
                ((s32)(tbl->col1[1] * 128.0f) << 8) | 0x80000000,
            (s32)(tbl->col2[0] * 128.0f) | ((s32)(tbl->col2[2] * 128.0f) << 16) |
                ((s32)(tbl->col2[1] * 128.0f) << 8) | 0x80000000,
            (s32)(tbl->cola[0] * 128.0f) | ((s32)(tbl->cola[2] * 128.0f) << 16) |
                ((s32)(tbl->cola[1] * 128.0f) << 8) | 0x80000000);
        dir[0] = tbl->dir0[0];
        dir[1] = tbl->dir0[1];
        dir[2] = tbl->dir0[2];
        dir[3] = 0.0f;
        SDF_VU0_LOADVECTOR(dir);
        evtSetUnitLightDir(h, intr);
        dir[0] = tbl->dir1[0];
        dir[1] = tbl->dir1[1];
        dir[2] = tbl->dir1[2];
        dir[3] = 0.0f;
        SDF_VU0_LOADVECTOR(dir);
        evtSetUnitLightDir1(h, intr);
        dir[0] = tbl->dir2[0];
        dir[1] = tbl->dir2[1];
        dir[2] = tbl->dir2[2];
        dir[3] = 0.0f;
        SDF_VU0_LOADVECTOR(dir);
        evtSetUnitLightDir2(h, intr);
    }
}
#else
INCLUDE_ASM(const s32, "code_00140960", mapUnitSetLight);
#endif

INCLUDE_ASM(const s32, "code_00140960", nbMoveFormation);

void mapGetLightParam(mapLightParam_t *param, f32 x, f32 y, f32 z)
{
    LightTbl_t *tbl;
    s32 r;
    s32 g;
    s32 b;

    if (dds3SearchProcessName(mapStrLightEditUnit) != NULL) {
        param->col0[0] = mapData21.col[0];
        param->col0[1] = mapData21.col[1];
        param->col0[2] = mapData21.col[2];
        r = mapData21.col[0] * 128.0f;
        g = mapData21.col[1] * 128.0f;
        b = mapData21.col[2] * 128.0f;
        param->rgb0 = r | (b << 16) | (g << 8) | 0x80000000;
        param->col1[0] = mapUnitSetLightData.col[0];
        param->col1[1] = mapUnitSetLightData.col[1];
        param->col1[2] = mapUnitSetLightData.col[2];
        r = mapUnitSetLightData.col[0] * 128.0f;
        g = mapUnitSetLightData.col[1] * 128.0f;
        b = mapUnitSetLightData.col[2] * 128.0f;
        param->rgb1 = r | (b << 16) | (g << 8) | 0x80000000;
        param->col2[0] = mapUnitSetLightData2.col[0];
        param->col2[1] = mapUnitSetLightData2.col[1];
        param->col2[2] = mapUnitSetLightData2.col[2];
        r = mapUnitSetLightData2.col[0] * 128.0f;
        g = mapUnitSetLightData2.col[1] * 128.0f;
        b = mapUnitSetLightData2.col[2] * 128.0f;
        param->rgb2 = r | (b << 16) | (g << 8) | 0x80000000;
        param->cola[0] = fldSkyPlayerIdx[0];
        param->cola[1] = fldSkyPlayerIdx[1];
        param->cola[2] = fldSkyPlayerIdx[2];
        r = fldSkyPlayerIdx[0] * 128.0f;
        g = fldSkyPlayerIdx[1] * 128.0f;
        b = fldSkyPlayerIdx[2] * 128.0f;
        param->rgba = r | (b << 16) | (g << 8) | 0x80000000;
        param->dir0.x = mapData21.dir[0];
        param->dir0.y = mapData21.dir[1];
        param->dir0.z = mapData21.dir[2];
        param->dir0.w = 0.0f;
        param->dir1.x = mapUnitSetLightData.dir[0];
        param->dir1.y = mapUnitSetLightData.dir[1];
        param->dir1.z = mapUnitSetLightData.dir[2];
        param->dir1.w = 0.0f;
        param->dir2.x = mapUnitSetLightData2.dir[0];
        param->dir2.y = mapUnitSetLightData2.dir[1];
        param->dir2.z = mapUnitSetLightData2.dir[2];
        param->dir2.w = 0.0f;
    } else {
        tbl = &LightDataTbl[fldGlobalBase.amSBlock][mapGetLight(x, z)];
        param->col0[0] = tbl->col0[0];
        param->col0[1] = tbl->col0[1];
        param->col0[2] = tbl->col0[2];
        r = tbl->col0[0] * 128.0f;
        g = tbl->col0[1] * 128.0f;
        b = tbl->col0[2] * 128.0f;
        param->rgb0 = r | (b << 16) | (g << 8) | 0x80000000;
        param->col1[0] = tbl->col1[0];
        param->col1[1] = tbl->col1[1];
        param->col1[2] = tbl->col1[2];
        r = tbl->col1[0] * 128.0f;
        g = tbl->col1[1] * 128.0f;
        b = tbl->col1[2] * 128.0f;
        param->rgb1 = r | (b << 16) | (g << 8) | 0x80000000;
        param->col2[0] = tbl->col2[0];
        param->col2[1] = tbl->col2[1];
        param->col2[2] = tbl->col2[2];
        r = tbl->col2[0] * 128.0f;
        g = tbl->col2[1] * 128.0f;
        b = tbl->col2[2] * 128.0f;
        param->rgb2 = r | (b << 16) | (g << 8) | 0x80000000;
        param->cola[0] = tbl->cola[0];
        param->cola[1] = tbl->cola[1];
        param->cola[2] = tbl->cola[2];
        r = tbl->cola[0] * 128.0f;
        g = tbl->cola[1] * 128.0f;
        b = tbl->cola[2] * 128.0f;
        param->rgba = r | (b << 16) | (g << 8) | 0x80000000;
        param->dir0.x = tbl->dir0[0];
        param->dir0.y = tbl->dir0[1];
        param->dir0.z = tbl->dir0[2];
        param->dir0.w = 0.0f;
        param->dir1.x = tbl->dir1[0];
        param->dir1.y = tbl->dir1[1];
        param->dir1.z = tbl->dir1[2];
        param->dir1.w = 0.0f;
        param->dir2.x = tbl->dir2[0];
        param->dir2.y = tbl->dir2[1];
        param->dir2.z = tbl->dir2[2];
        param->dir2.w = 0.0f;
    }
}

void mapCutSetLight(void) {
    f32 col[4];
    f32 dir[4];

    dir[0] = 0.78f;
    dir[1] = 0.4f;
    dir[2] = -0.5f;
    dir[3] = 0.0f;
    dds3SetLightDir(0, 0, dir);
    col[0] = 1.5f;
    col[1] = 1.1f;
    col[2] = 1.0f;
    col[3] = 0.0f;
    dds3SetLightColor(0, 0, col);
    dir[0] = -0.08f;
    dir[1] = 0.85f;
    dir[2] = 0.52f;
    dir[3] = 0.0f;
    dds3SetLightDir(0, 1, dir);
    col[0] = 1.0f;
    col[1] = 1.1f;
    col[2] = 0.9f;
    col[3] = 0.0f;
    dds3SetLightColor(0, 1, col);
    dir[0] = -0.76f;
    dir[1] = -0.65f;
    dir[2] = 0.02f;
    dir[3] = 0.0f;
    dds3SetLightDir(0, 2, dir);
    col[0] = 0.5f;
    col[1] = 0.5f;
    col[2] = 0.6f;
    col[3] = 0.0f;
    dds3SetLightColor(0, 2, col);
    col[0] = 0.1f;
    col[1] = 0.1f;
    col[2] = 0.1f;
    col[3] = 1.0f;
    dds3SetAmbientColor(0, col);
}

INCLUDE_ASM(const s32, "code_00140960", mapHitAidaCalcSub);

s32 mapHamidasiWHitSubSub(f32 *outx, f32 *outz, f32 newx, f32 newz, f32 r, f32 x0, f32 z0, f32 x1, f32 z1, f32 vx, f32 vz) {
    f32 dx;
    f32 dz;
    f32 t;
    f32 px;
    f32 pz;
    f32 len;

    dx = x1 - x0;
    dz = z1 - z0;
    t = ((newx - x0) * dx + (newz - z0) * dz) / (dx * dx + dz * dz);
    px = dx * t + x0;
    pz = dz * t + z0;
    len = fsqrtf((px - newx) * (px - newx) + (pz - newz) * (pz - newz));
    if (t >= 0.0f && t <= 1.0f && len <= r) {
        *outx = px + vx * (r + 1.0f);
        *outz = pz + vz * (r + 1.0f);
        return 1;
    }
    *outx = newx;
    *outz = newz;
    return 0;
}

s32 mapHamidasiMoveBoxHitSub(f32 *outx, f32 *outz, f32 newx, f32 newz, f32 r, f32 x0, f32 z0, f32 x1, f32 z1, f32 vx, f32 vz) {
    f32 dx;
    f32 dz;
    f32 len;

    dx = newx - x0;
    dz = newz - z0;
    len = fsqrtf(dx * dx + dz * dz);
    if (len < r) {
        *outx = x0 + (dx / len) * (r + 1.0f);
        *outz = z0 + (dz / len) * (r + 1.0f);
        return 1;
    }
    dx = newx - x1;
    dz = newz - z1;
    len = fsqrtf(dx * dx + dz * dz);
    if (len < r) {
        *outx = x1 + (dx / len) * r;
        *outz = z1 + (dz / len) * r;
        return 1;
    }
    *outx = newx;
    *outz = newz;
    return 0;
}

INCLUDE_ASM(const s32, "code_00140960", mapHamidasiCheckSub);

s32 mapHamidasiCHitSub(f32 cx, f32 cz, f32 *updx, f32 *updz) {
    f32 hitx;
    f32 hitz;
    s32 hit;
    s32 i;

    hit = 0;
    for (i = 0; i < 128; i++) {
        if (mapObjTaskData2[i].use != 0 && (mapObjTaskData2[i].floor == -1 || mapObjTaskData2[i].floor == fldGlobalBase.nameU) && mapObjTaskData2[i].hitsize > 10.0f) {
            hit |= mapGetCircleCircleH(cx, cz, 45.0f, mapObjTaskData2[i].x, mapObjTaskData2[i].z, mapObjTaskData2[i].hitsize, &hitx, &hitz);
            cx = hitx;
            cz = hitz;
        }
    }
    *updx = cx;
    *updz = cz;
    return hit;
}

s32 mapHamidasiNpcSub(f32 cx, f32 cz, f32 *updx, f32 *updz) {
    f32 hitx;
    f32 hitz;
    s32 hit;
    s32 i;
    MAP_CHIT_t *p;

    p = mapData28;
    hit = 0;
    for (i = 0; i < 256; i++, p++) {
        if (p->use != 0) {
            hit |= mapGetCircleCircleH(cx, cz, 45.0f, p->x, p->z, p->r, &hitx, &hitz);
            cx = hitx;
            cz = hitz;
        }
    }
    *updx = cx;
    *updz = cz;
    return hit;
}

/* NON_MATCHING: 47 of 50 words match. Fragile: the fld.h declaration set moves it in the full unit. */
#ifdef NON_MATCHING
s32 mapHamidasiWHitSub(f32 *cx, f32 *cz, f32 r) {
    f32 hitx;
    f32 hitz;
    s32 hit;
    s32 i;
    MAP_WHIT_t *p;

    p = mapData29;
    hit = 0;
    for (i = 0; i < 128; i++, p++) {
        if (p->use != 0 && mapHamidasiWHitSubSub(&hitx, &hitz, *cx, *cz, r, p->x0, p->z0, p->x1, p->z1, p->vx, p->vz)) {
            hit = 1;
            *cx = hitx;
            *cz = hitz;
        }
    }
    return hit;
}
#else
INCLUDE_ASM(const s32, "code_00140960", mapHamidasiWHitSub);
#endif

INCLUDE_ASM(const s32, "code_00140960", mapHamidasiMoveBoxHit);

INCLUDE_RODATA(const s32, "code_00140960", mapStrLightEditUnit);

