#include "shared/effect/effPCPMisc/decls.h"

void effPCPAgiFireball2Delete(effPCPAgiFireball2_tag *hData) {
    u32 i;
    effPCPAgiFireball_tag **blockSets;

    blockSets = hData->blockSets;
    for (i = 0; i < 3; i++) {
        effPCPAgiFireballDelete(blockSets[i]);
    }
    sdfChipFreeDirect(hData);
}
