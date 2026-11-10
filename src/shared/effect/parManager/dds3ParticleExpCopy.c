#include "shared/effect/parManager/decls.h"

dds3Particle_Basic_t *dds3ParticleExpCopy(dds3Particle_Basic_t *source) {
    dds3Particle_Basic_t *particle = dds3ParticleFuncTable[source->type].func(source);
    particle->type = source->type;
    if (source->numberO == PAR_BILLBOARD_CLONE_FROM_RESOURCE) {
        billHandle_t *billboard = dds3BillboardCopy(source->hBillboard);
        dds3BillboardSetScale(billboard, particle->billboardScale, particle->billboardScaleY);
        dds3BillboardSetMode(billboard, particle->billboardMode);
        dds3BillboardSetAnimColorAdvMode(billboard);
        particle->hBillboard = billboard;
    }
    return particle;
}
