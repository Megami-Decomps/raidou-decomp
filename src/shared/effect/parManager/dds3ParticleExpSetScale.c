#include "shared/effect/parManager/decls.h"

void dds3ParticleExpSetScale(dds3Particle_Basic_t *hData, float scale) {
    dds3ParticleSetScale(hData, scale);
    hData->scaleEnd *= scale;
}
