#include "shared/effect/effPCPScatter/decls.h"

void dds3ScatterPolyListTexResourceDelete(dds3ScatterPolyListTexResource_tag *hSplTexRes)
{
    if (--hSplTexRes->refCount == 0) {
        sdfTexFreeDelayed(hSplTexRes->textureHandle);
        sdfChipFreeDirect(hSplTexRes);
    }
}
