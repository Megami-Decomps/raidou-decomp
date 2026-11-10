#include "shared/effect/effPCPMisc/decls.h"

void effPCPRingPolyExp2Delete(effPCPRingPolyExp2_tag *hData) {
    dds3RingPolyExpListDelete(hData->hPoly);
    sdfChipFreeDirect(hData);
}
