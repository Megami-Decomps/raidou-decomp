#include "shared/effect/effPCPMisc/decls.h"

void effPCPHwave01Delete(effPCPHwave01_tag *hData) {
    dds3EffectPartDelete(hData->hPB);
    dds3EffectPartDelete(hData->hD3P);
    sdfChipFreeDirect(hData);
}
