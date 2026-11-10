#include "shared/effect/effPCPMisc/decls.h"

void effPCPBreathDelete(effPCPBreath_tag *hData) {
    if (hData->hD3P != NULL) {
        dds3EffectDelete(hData->hD3P);
    }
    sdfChipFreeDirect(hData);
}
