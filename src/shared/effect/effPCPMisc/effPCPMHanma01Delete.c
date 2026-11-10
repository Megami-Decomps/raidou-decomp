#include "shared/effect/effPCPMisc/decls.h"

void effPCPMHanma01Delete(effPCPMHanma01_tag *hData) {
    effPartHandle_t **handle = &hData->handle[1];
    s32 i;

    for (i = 5; i >= 0; i--) {
        dds3EffectPartDelete(handle[-1]);
        dds3EffectPartDelete(handle[0]);
        handle += 2;
    }
    sdfChipFreeDirect(hData);
}
