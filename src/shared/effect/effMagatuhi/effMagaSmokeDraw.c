#include "shared/effect/effMagatuhi/decls.h"

void effMagaSmokeDraw(effMagaSmoke_tag *pData) {
    sdfVector4_t out;
    sdfVector4_t origin;
    dds3MagatuhiBill_tag *valueWork;
    effMagaSmokePart_tag *particle;
    u8 respawn;
    u32 count;
    s32 life, spread, fadeIn, fadeOut;
    u32 tintColor;
    f32 radial, fade;
    f32 angleStep, scaleStep;
    s32 age;
    u32 i;

    particle = pData->pPart;
    tintColor = pData->abgr;
    count = pData->param.particleCount;
    respawn = pData->param.loop;
    life = SDF_FRAMES(pData->param.lifetimeFrames);
    spread = pData->param.delaySpread;
    angleStep = SDF_FRAME_STEP(pData->param.angleStep);
    scaleStep = SDF_FRAME_STEP(pData->param.scaleStep);
    fadeIn = SDF_FRAMES(pData->param.fadeIn);
    fadeOut = SDF_FRAMES(pData->param.fadeOut);
    valueWork = pData->pMagatuhi->valueWork;
    SDF_VU0_COPYVECTOR_MEMOPS(&origin, pData->param.origin);
    SDF_VU0_LOADMATRIX(pData->matrix);
    for (i = 0; i < count; i++, particle++) {
        age = particle->age;
        if (age == 0) {
            effMagaSmokePartInit(pData, i);
        }
        if (age > 0 && age <= life) {
            radial = particle->amplitude * sdfSin(particle->angle);
            particle->dirUp[1] += SDF_FRAME_STEP(particle->up_s);
            out.x = particle->pos[0] + particle->dirUp[0] * radial;
            out.y = particle->pos[1] + particle->dirUp[1];
            out.z = particle->pos[2] + particle->dirUp[2] * radial;
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_LOADVECTOR(&out);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&out);
            particle->amplitude += scaleStep;
            particle->angle += angleStep;
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
            particle->age = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        } else {
            particle->age++;
        }
    }
    effMagaCylinderDrawTrack(pData->pMagatuhi);
}
