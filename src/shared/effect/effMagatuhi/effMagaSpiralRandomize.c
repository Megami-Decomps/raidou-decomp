#include "shared/effect/effMagatuhi/decls.h"

/* Advance the orbiting-particle family: each group of three slots is replayed from a random delay, one orbit step at a time, into the value table. */
void effMagaSpiralRandomize(effMagatuhiCallback_t *hEffect) {
    u32 k;
    effMagaSpiral_tag *work;
    dds3MagatuhiBill_tag *valueWork;
    effMagaSpiralPart_tag *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 frameOffset; /* random age, then the initial state offset skipped before replay */
    sdfVector4_t out;
    sdfVector4_t origin;
    f32 height;
    f32 angle;
    f32 radius;
    f32 heightStep;
    f32 angleStep;
    f32 radiusStep;

    if (hEffect->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        if (dds3MagatuhiGetType(hEffect->effect) == EFF_MAGATUHI_REPLAY_ORBIT) {
            work = dds3MagatuhiGetHandle(hEffect->effect);
            count = work->param.particleCount;
            valueWork = work->pMagatuhi->valueWork;
            maxSteps = work->param.reserve;
            frames = work->param.life;
            particle = work->pPart;
            SDF_VU0_COPYVECTOR(&origin, work->param.origin);
            SDF_VU0_LOADMATRIX(work->matrix);
            for (i = 0; i < count; i += EFF_MAGATUHI_REPLAY_GROUP_STRIDE, particle += EFF_MAGATUHI_REPLAY_GROUP_STRIDE) {
                effMagaSpiralPartInit(work, i);
                frameOffset = dds3GetRandA((u8 *)dds3EffectBase) % frames;
                particle->frame = SDF_FRAMES(frameOffset);
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < frameOffset) {
                    steps = maxSteps;
                    frameOffset -= steps;
                } else {
                    steps = maxSteps - frameOffset;
                    frameOffset = 0;
                }
                angleStep = particle->rotate_s;
                radiusStep = particle->radius_s;
                heightStep = particle->up_s;
                angle = particle->rotate + angleStep * (f32)frameOffset;
                radius = particle->radius + radiusStep * (f32)frameOffset;
                height = particle->up + heightStep * (f32)frameOffset;
                out.w = 0;
                for (k = 0; k < steps; k++) {
                    out.x = sdfCos(angle) * radius;
                    out.y = height;
                    out.z = sdfSin(angle) * radius;
                    SDF_VU0_LOADVECTOR_B(&origin);
                    SDF_VU0_LOADVECTOR(&out);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(&out);
                    effMagaCylinderSetPartPos(valueWork, i, &out);
                    height += heightStep;
                    angle += angleStep;
                    radius += radiusStep;
                }
                particle->up = height;
                particle->radius = radius;
                particle->rotate = angle;
            }
        }
    }
}
