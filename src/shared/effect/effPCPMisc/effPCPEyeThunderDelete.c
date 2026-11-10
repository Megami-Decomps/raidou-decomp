#include "shared/effect/effPCPMisc/decls.h"

void effPCPEyeThunderDelete(effPCPEyeThunder_tag *hData) {
    effPCP_ThunderPolyDelete(hData->pThunderPoly);
    sdfChipFreeDirect(hData);
}
