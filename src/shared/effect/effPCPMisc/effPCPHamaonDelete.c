#include "shared/effect/effPCPMisc/decls.h"

void effPCPHamaonDelete(effPCPHamaon_tag *hData) {
    s32 i;
    s32 j;

    dds3EffectPartDelete(hData->base);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            dds3EffectPartDelete(hData->handle[i][j]);
        }
    }
    sdfChipFreeDirect(hData);
}
