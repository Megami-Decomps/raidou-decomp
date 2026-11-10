#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_RingBuffInit(dds3SpPoly_Ring_t3 *ring, s32 number)
{
    dds3SpPoly_RingPhysics_t *record;
    f32 spread;
    f32 radius;
    f32 endRadius;
    s32 frames;

    record = ring->pPhysics;
    record += number;
    spread = ring->initialRadiusJitter;
    radius = ring->initialRadius * (dds3GetRandFloatA(dds3EffectBase) * spread + (1.0f - spread));
    spread = ring->targetRadiusJitter;
    endRadius = ring->targetRadius * (dds3GetRandFloatA(dds3EffectBase) * spread + (1.0f - spread));
    record->frame = 0;
    frames = ring->head.frame;
    if (frames > 0) {
        record->spread_s = (endRadius - radius) / (f32)frames;
    } else {
        record->spread_s = endRadius - radius;
    }
    func_00168A50(ring, number, radius);
}
