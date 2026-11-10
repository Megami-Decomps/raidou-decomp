#include "shared/effect/effPCPMisc/decls.h"

void effPCPMudoDelete(effPCPMudo_tag *hData) {
    dds3EffectPartDelete(hData->primaryHandle);
    dds3EffectPartDelete(hData->secondaryHandle);
    sdfMemFreeDirect(hData->allocationHandle);
}
