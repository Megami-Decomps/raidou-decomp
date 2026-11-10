#include "shared/effect/parManager/decls.h"

/* Frame zero reinitializes to -1 before incrementing; refresh position and modulated packed color. */
void dds3Particle_SpiralBlendColor(effParticleOverlay_t *effect) {
    effParticleRecord_t *record = (effParticleRecord_t *)effect->buffer->pPhysics;
    s32 frame = record->frame;
    u32 baseColor;

    if (frame == 0) {
        func_00162270(effect);
        frame = record->frame;
    }
    record->frame = frame + 1;
    record->x = effect->x;
    effect->drawEndFrame = frame + 2;
    record->y = effect->y;
    baseColor = effect->color | (effect->alpha << 24);
    record->z = effect->z;
    record->color = baseColor;
    record->color = dds3ParticleColorBlend(baseColor, effect->colorMask);
}
