#include "shared/effect/effPCPNeedle/decls.h"

void effPCPNeedleSpiralDelete(effPCPNeedleSpiral_tag *hData) {
    dds3TrackPolyDelete(hData->hTrackPoly);
    dds3NeedleModelDelete(hData->hNModel);
    sdfMemFreeDirect(hData->hMem);
}
