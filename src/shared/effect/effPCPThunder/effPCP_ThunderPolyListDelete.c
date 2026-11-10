#include "shared/effect/effPCPThunder/decls.h"

void effPCP_ThunderPolyListDelete(effPCP_ThunderPolyList_t *hData) {
    s32 fragmentIndex;

    for (fragmentIndex = 0; fragmentIndex < hData->head.count - 1; fragmentIndex++) {
        effPCP_ThunderPolyDelete(hData->handles[fragmentIndex]);
    }
    sdfChipFreeDirect(hData);
}
