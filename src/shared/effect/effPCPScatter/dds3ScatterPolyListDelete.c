#include "shared/effect/effPCPScatter/decls.h"

void dds3ScatterPolyListDelete(dds3ScatterPolyList_tag *hPoly)
{
    if (hPoly->sharedResource != NULL) {
        dds3ScatterPolyListTexResourceDelete(hPoly->sharedResource);
    }
    sdfMatFreeDelayed(hPoly->drawAsset);
    sdfMemFreeDirect(hPoly->allocation);
}
