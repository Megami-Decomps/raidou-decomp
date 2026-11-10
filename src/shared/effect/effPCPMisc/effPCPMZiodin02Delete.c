#include "shared/effect/effPCPMisc/decls.h"

void effPCPMZiodin02Delete(effPCPZiodin01_tag *hData) {
    effPCP_ThunderPoly_t **handle = hData->handles;
    s32 remaining;

    for (remaining = 7; remaining >= 0; remaining--) {
        effPCP_ThunderPolyDelete(*handle++);
    }
    sdfChipFreeDirect(hData);
}
