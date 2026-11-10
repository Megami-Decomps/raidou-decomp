#include "shared/effect/effPCPMisc/decls.h"

void effPCPRingPolyExpDelete(effPCPRingPolyExp_tag *hData) {
    dds3RingPolyExpListDelete(hData->hPoly);
    sdfChipFreeDirect(hData);
}
