#include "shared/effect/parManager/decls.h"

void dds3Particle_SmokeOffsetDelete(effTemplatePacketList_t *hData) {
    sdfMemFreeDirect(hData->auxiliaryAllocation);
    dds3Particle_BasicDelete(hData);
    sdfChipFreeDirect(hData);
}
