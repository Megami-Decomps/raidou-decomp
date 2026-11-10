#include "shared/effect/effPCPThunder/decls.h"

void effPCP_ThunderPolyLowDelete(effPCP_ThunderPoly_t *hData) {
    dds3TrackPolyDelete(hData->state.secondarySystem);
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}
