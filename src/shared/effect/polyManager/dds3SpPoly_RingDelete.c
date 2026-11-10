#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_RingDelete(dds3SpPoly_Ring_t3 *hData) {
    dds3TrackPolyDelete(hData->hTrackPoly);
    sdfMemFreeDirect(hData->hMem);
}
