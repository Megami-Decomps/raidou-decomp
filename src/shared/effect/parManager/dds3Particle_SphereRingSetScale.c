#include "shared/effect/parManager/decls.h"

/* Scale the position and record scale of a short template. */
void dds3Particle_SphereRingSetScale(effTemplatePacketList_t *hData, float scale) {
    hData->head.scale[0] = hData->head.scale[0] * scale;
    hData->head.scale[1] = hData->head.scale[1] * scale;
    hData->head.scale[2] = hData->head.scale[2] * scale;
    hData->recordScale = hData->recordScale * scale;
}
