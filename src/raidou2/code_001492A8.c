#include "common.h"
#include "fpu.h"
#include "dds3KernelDraw.h"

INCLUDE_ASM(const s32, "code_001492A8", mapDebugLightSet);

INCLUDE_ASM(const s32, "code_001492A8", mapUnitSetLight);

INCLUDE_ASM(const s32, "code_001492A8", nbMoveFormation);

INCLUDE_ASM(const s32, "code_001492A8", nbLIGHTEDITUNIT);

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

INCLUDE_ASM(const s32, "code_001492A8", mapHitAidaCalcSub);

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

INCLUDE_ASM(const s32, "code_001492A8", mapHamidasiCheckSub);

INCLUDE_ASM(const s32, "code_001492A8", mapHamidasiCHitSub);

INCLUDE_ASM(const s32, "code_001492A8", mapHamidasiNpcSub);

INCLUDE_ASM(const s32, "code_001492A8", mapHamidasiWHitSub);

INCLUDE_ASM(const s32, "code_001492A8", mapHamidasiMoveBoxHit);

INCLUDE_RODATA(const s32, "code_001492A8", mapStrLightEditUnit);

