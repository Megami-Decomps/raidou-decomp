#include "shared/effect/effMagatuhi/decls.h"

void effMagaCylinderDraw(effMagaCylinder_tag *pData) {
    sdfVector4_t out;
    sdfVector4_t origin;
    dds3MagatuhiBill_tag *valueWork;
    effMagaCylinderPart_tag *particle;
    u8 respawn;
    u32 count;
    s32 life, spread, fadeIn, fadeOut;
    u32 tintColor;
    f32 angle, radius, fade;
    s32 age;
    u32 i;

    particle = pData->pPart;
    count = pData->param.particleCount;
    respawn = pData->param.loop;
    life = SDF_FRAMES(pData->param.life);
    spread = pData->param.timing;
    fadeIn = SDF_FRAMES(pData->param.fade_in);
    fadeOut = SDF_FRAMES(pData->param.fade_out);
    tintColor = pData->abgr;
    valueWork = pData->pMagatuhi->valueWork;
    SDF_VU0_COPYVECTOR_MEMOPS(&origin, pData->param.origin);
    SDF_VU0_LOADMATRIX(pData->matrix);
    for (i = 0; i < count; i++, particle++) {
        age = particle->frame;
        if (age == 0) {
            effMagaCylinderPartInit(pData, i);
        }
        if (age > 0 && age <= life) {
            angle = particle->rotate;
            radius = particle->radius;
            out.x = sdfCos(angle) * radius;
            out.y = particle->height;
            out.z = sdfSin(angle) * radius;
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_LOADVECTOR(&out);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&out);
            particle->rotate += SDF_FRAME_STEP(particle->rotate_s);
            particle->radius += SDF_FRAME_STEP(particle->radius_s);
            if (age < fadeIn) {
                fade = (f32)age / (f32)fadeIn;
            } else if (life - age <= fadeOut) {
                fade = (f32)(life - age) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagaCylinderSetPart(valueWork, i,
                                   effMagaCylinderCalcColor(((u32)(fade * EFF_MAGATUHI_FADE_ALPHA_SCALE) << EFF_MAGATUHI_ALPHA_SHIFT) | EFF_MAGATUHI_NEUTRAL_RGB, tintColor));
            if (SDF_FRAME_DUE(age)) effMagaCylinderSetPartPos(valueWork, i, &out);
        }
        if (age >= life && respawn) {
            particle->frame = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        } else {
            particle->frame++;
        }
    }
    effMagaCylinderDrawTrack(pData->pMagatuhi);
}
