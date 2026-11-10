#include "shared/effect/polyManager/decls.h"

void dds3TrackPolyDelete(dds3TrackPoly_t *hTrackPoly) {
    sdfMatFreeDelayed(hTrackPoly->pMat);
    sdfMemFreeDirect(hTrackPoly->hMem);
}
