#include "shared/effect/parManager/decls.h"

/* Scale position and the third variant-specific tail value. */
void dds3Particle_RadiateSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->tailValues[2] = hData->tailValues[2] * scale;
}
