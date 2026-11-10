#include "shared/effect/parManager/decls.h"

/* Create another object of the same dispatch index and return the new instance. */
dds3Particle_Basic_t *dds3ParticleCopy(dds3Particle_Basic_t *obj) {
    dds3Particle_Basic_t *createdObject;

    createdObject = dds3ParticleFuncTable[obj->type].func(obj);
    createdObject->type = obj->type;
    return createdObject;
}
