#include "shared/effect/parManager/decls.h"

/* Release the radial emitter's extra allocation, shared resources, and block. */
void dds3Particle_SphereSpiral2Delete(dds3Particle_SphereSpiral2_t3 *hData) {
    if (hData->childHandle != 0) {
        sdfMemFreeDirect(hData->childHandle);
    }
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}
