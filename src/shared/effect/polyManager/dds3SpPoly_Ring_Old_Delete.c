#include "shared/effect/polyManager/decls.h"

void dds3SpPoly_Ring_Old_Delete(dds3SpPoly_Ring_Old_t3 *hData) {
    dds3TrackPolyDelete(hData->hTrackPoly);
    sdfChipFreeDirect(hData);
}
