#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_RollRingDelete(dds3SpPoly_RollRing_t3 *hData) {
    dds3TrackPolyDelete(hData->hTrackPoly);
    sdfMemFreeDirect(hData->allocation);
}
