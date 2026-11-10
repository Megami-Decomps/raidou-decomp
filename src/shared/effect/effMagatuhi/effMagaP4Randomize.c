#include "shared/effect/effMagatuhi/decls.h"

/* Seed every fourth slot with interpolated samples between two curve positions.
 * Keep the asymmetric duration/age subtraction and unchecked span divisor. */
void effMagaP4Randomize(effMagatuhiCallback_t *hEffect) {
    effMagaPoint4_tag *work = dds3MagatuhiGetHandle(hEffect->effect);
    dds3MagatuhiBill_tag *valueWork = work->pMagatuhi->valueWork;
    u32 historyFrames = work->param.duration;
    s32 lifetimeFrames = work->param.life;
    u32 count = work->param.reserve;
    s32 *ages = work->pPart;
    sdfVector4_t samplePosition;
    f32 startPosition[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 endPosition[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 blendFraction;
    f32 blendStep;
    u32 particleIndex;
    u32 sampleIndex;
    s32 sampleSpan;
    effCubicBezierSlot_t *slot;

    for (particleIndex = 0; particleIndex < count; particleIndex += EFF_MAGATUHI_BEZIER_GROUP_STRIDE) {
        effMagaPoint4PartInit(work, particleIndex);
        *ages = dds3GetRandA((u8 *)dds3EffectBase) % lifetimeFrames;
        slot = dds3Bezier4PGetPart(work->pBezier4P, particleIndex);
        if (historyFrames < *ages) {
            slot->t = slot->step * SDF_FRAMES_F((f32)(*ages - historyFrames));
            sampleSpan = historyFrames;
        } else {
            slot->t = 0.0f;
            sampleSpan = historyFrames - *ages;
        }
        blendFraction = 0.0f;
        dds3Bezier4PPartRun(work->pBezier4P, particleIndex, startPosition);
        slot->t = slot->step * SDF_FRAMES_F((f32)*ages);
        dds3Bezier4PPartRun(work->pBezier4P, particleIndex, endPosition);
        blendStep = 1.0f / (f32)sampleSpan;
        for (sampleIndex = 0; sampleIndex < sampleSpan; sampleIndex++) {
            SDF_VU0_LOADVECTOR(startPosition);
            SDF_VU0_LOADVECTOR_B(endPosition);
            SDF_VU0_INTERPOLATE(blendFraction);
            SDF_VU0_STOREVECTOR(&samplePosition);
            effMagaCylinderSetPartPos(valueWork, particleIndex, &samplePosition);
            blendFraction += blendStep;
        }
        *ages = SDF_FRAMES(*ages) + 1;
        ages += EFF_MAGATUHI_BEZIER_GROUP_STRIDE;
    }
}
