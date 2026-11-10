#include "shared/effect/effPCPMisc/decls.h"

void effPCPBufuIceDelete(effPCPBufuIce_tag *hData) {
    dds3EffectPartDelete(hData->handle0);
    sdfChipFreeDirect(hData);
}
