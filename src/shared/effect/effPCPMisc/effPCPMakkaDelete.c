#include "shared/effect/effPCPMisc/decls.h"

void effPCPMakkaDelete(effPCPMakka_tag *hData) {
    u32 i = 0;

    if (hData->number != 0) {
        effPartHandle_t **handle = hData->handle;
        do {
            if (*handle != 0) {
                dds3EffectPartDelete(*handle);
            }
            handle++;
            i++;
        } while (i < hData->number);
    }
    sdfChipFreeDirect(hData);
}
