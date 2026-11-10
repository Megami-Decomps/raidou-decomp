#include "shared/effect/effMagatuhi/decls.h"

/* Replay the first slot of each three-slot drift group from a random age. */
void effMagaSmokeRandomize(effMagatuhiCallback_t *hEffect) {
    u32 k;
    effMagaSmoke_tag *work;
    dds3MagatuhiBill_tag *valueWork;
    effMagaSmokePart_tag *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 frameOffset; /* random age, then the initial state offset skipped before replay */
    sdfVector4_t out;
    sdfVector4_t origin;
    f32 lift;
    f32 angle;
    f32 scale;
    f32 liftStep;
    f32 angleStep;
    f32 scaleStep;

    if (hEffect->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        if (dds3MagatuhiGetType(hEffect->effect) == EFF_MAGATUHI_REPLAY_DRIFT) {
            work = dds3MagatuhiGetHandle(hEffect->effect);
            count = work->param.particleCount;
            valueWork = work->pMagatuhi->valueWork;
            maxSteps = work->param.reserve;
            frames = work->param.lifetimeFrames;
            particle = work->pPart;
            SDF_VU0_COPYVECTOR(&origin, work->param.origin);
            SDF_VU0_LOADMATRIX(work->matrix);
            for (i = 0; i < count; i += EFF_MAGATUHI_REPLAY_GROUP_STRIDE, particle += EFF_MAGATUHI_REPLAY_GROUP_STRIDE) {
                effMagaSmokePartInit(work, i);
                frameOffset = dds3GetRandA((u8 *)dds3EffectBase) % frames;
                particle->age = SDF_FRAMES(frameOffset);
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < frameOffset) {
                    steps = maxSteps;
                    frameOffset -= steps;
                } else {
                    steps = maxSteps - frameOffset;
                    frameOffset = 0;
                }
                angleStep = work->param.angleStep;
                scaleStep = work->param.scaleStep;
                liftStep = particle->up_s;
                angle = particle->angle + angleStep * (f32)frameOffset;
                scale = particle->amplitude + scaleStep * (f32)frameOffset;
                lift = particle->dirUp[1] + liftStep * (f32)frameOffset;
                out.w = 0;
                for (k = 0; k < steps; k++) {
                    /* Native ordering: lift advances before sampling, scale/angle after. */
                    lift += liftStep;
                    /* Keep the sine evaluation even though its result is unused here. */
                    sdfSin(angle);
                    out.x = particle->pos[0] + particle->dirUp[0] * scale;
                    out.y = particle->pos[1] + lift;
                    out.z = particle->pos[2] + particle->dirUp[2] * scale;
                    SDF_VU0_LOADVECTOR_B(&origin);
                    SDF_VU0_LOADVECTOR(&out);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(&out);
                    effMagaCylinderSetPartPos(valueWork, i, &out);
                    scale += scaleStep;
                    angle += angleStep;
                }
                particle->amplitude = scale;
                particle->angle = angle;
                particle->dirUp[1] = lift;
            }
        }
    }
}
