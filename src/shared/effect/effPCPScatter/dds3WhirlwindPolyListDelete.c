#include "shared/effect/effPCPScatter/decls.h"

void dds3WhirlwindPolyListDelete(dds3WhirlwindPolyList_tag *hPoly) {
    dds3WhirlwindPolyList_tag *current;

    current = hPoly;
    if (current->sharedResource != 0) {
        dds3ScatterPolyListTexResourceDelete(current->sharedResource);
    }
    sdfMatFreeDelayed(current->asset);
    sdfMemFreeDirect(current->allocation);
    sdfChipFreeDirect(hPoly);
}
