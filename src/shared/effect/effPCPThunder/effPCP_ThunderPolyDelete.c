#include "shared/effect/effPCPThunder/decls.h"

void effPCP_ThunderPolyDelete(effPCP_ThunderPoly_t *hData) {
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}
