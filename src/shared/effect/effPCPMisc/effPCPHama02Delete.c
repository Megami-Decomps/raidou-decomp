#include "shared/effect/effPCPMisc/decls.h"

void effPCPHama02Delete(effPCPHama02_tag *hData) {
    effPartHandle_t **handle = &hData->handle[1];
    s32 i;

    for (i = 7; i >= 0; i--) {
        dds3EffectPartDelete(handle[-1]);
        dds3EffectPartDelete(handle[0]);
        handle += 2;
    }
    sdfChipFreeDirect(hData);
}
