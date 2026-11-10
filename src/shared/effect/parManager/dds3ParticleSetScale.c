#include "shared/effect/parManager/decls.h"

/* Call the selected entry with factor, additionally scale kinds 2..4, then restart. */
void dds3ParticleSetScale(dds3Particle_Basic_t *hData, f32 scale) {
    dds3ParticleSetScaleTable[hData->type](hData, scale);
    switch (hData->kindState.type) {
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        hData->kindState.payload.crossStrip.scale *= scale;
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        hData->kindState.payload.crossTriangle.scale *= scale;
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        hData->kindState.payload.trackPolygon.scale *= scale;
        break;
    }
    dds3ParticleReset(hData);
}
