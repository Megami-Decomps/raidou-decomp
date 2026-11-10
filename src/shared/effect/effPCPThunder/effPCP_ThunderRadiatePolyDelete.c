#include "shared/effect/effPCPThunder/decls.h"

void effPCP_ThunderRadiatePolyDelete(effPCP_ThunderRadiatePoly_t *hData) {
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->hMem);
}
