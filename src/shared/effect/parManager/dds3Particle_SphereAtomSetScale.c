#include "shared/effect/parManager/decls.h"

/* Scale the position, record scale, and first two tail values. */
void dds3Particle_SphereAtomSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->recordScale = hData->recordScale * scale;
    hData->tailValues[0] = hData->tailValues[0] * scale;
    hData->tailValues[1] = hData->tailValues[1] * scale;
}
