#include "shared/effect/parManager/decls.h"

/* Create through the selected dispatch entry and record its index; returns the
 * new instance, which dds3EffectCreate keeps. */
dds3Particle_Basic_t *dds3ParticleCreate(s32 dispatchIndex, void *creationData) {
    dds3Particle_Basic_t *createdObject;

    createdObject = dds3ParticleFuncTable[dispatchIndex].func(creationData);
    createdObject->type = dispatchIndex;
    return createdObject;
}
