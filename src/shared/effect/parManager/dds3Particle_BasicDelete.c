#include "shared/effect/parManager/decls.h"

/* Release the selected sub-effect resource, billboard and packet-buffer
 * allocation described by the common emitter prefix. */
void dds3Particle_BasicDelete(void *pParBasic)
{
    effEmitterHead_t *owner = (effEmitterHead_t *)pParBasic;
    switch (owner->sub.type) {
    case PAR_KIND_STATE_HISTORY:
        dds3Particle_BlurDelete(owner->sub.payload.history.table);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        dds3TrackPolyDelete(owner->sub.payload.crossStrip.drawSystem);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        dds3TrackPolyDelete(owner->sub.payload.crossTriangle.drawSystem);
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        Eff_ModelTrackPolyListDelete(owner->sub.payload.trackPolygon.modelList);
        break;
    }
    dds3BillboardDelete(owner->billboard);
    dds3Particle_PhysicsDelete(owner->buffer);
}
