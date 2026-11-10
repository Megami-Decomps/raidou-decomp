#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_SphereRingDelete(dds3SpPoly_SphereRing_t3 *hData) {
    dds3TrackPolyDelete(hData->hTrackPoly);
    sdfMemFreeDirect(hData->hMem);
}
