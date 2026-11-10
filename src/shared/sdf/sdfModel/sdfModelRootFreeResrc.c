#include "shared/sdf/sdfModel/decls.h"

void sdfModelRootFreeResrc(sdfModelRootNode_t *aRoot)
{
    s32 i;

    for (i = 0; i < aRoot->nodeList->count; i++) {
        sdfModelNodeFree(aRoot->nodeList->items[i]);
    }
    sdfItemArrayFree(aRoot->nodeList);
    sdfChipFreeDirect(aRoot);
}
