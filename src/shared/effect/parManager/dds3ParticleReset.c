#include "shared/effect/parManager/decls.h"

/* Reissue the callback, reload the native updater's repeat count, and arm restart. */
void dds3ParticleReset(dds3Particle_Basic_t *hData) {
    dds3ParticleAllBuffInitTable[hData->type](hData);
    hData->pendingRestartSteps = SDF_FRAMES(hData->d_frame);
    hData->state = 1;
}
