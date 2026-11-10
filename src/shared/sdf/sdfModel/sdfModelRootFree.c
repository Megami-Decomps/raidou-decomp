#include "shared/sdf/sdfModel/decls.h"

void sdfModelRootFree(sdfModelRootNode_t *aRoot, s32 aReleaseNodes, s32 aReleaseMatList)
{
    if (aRoot == NULL) {
        return;
    }
    if (aReleaseMatList != 0) {
        sdfMatListFreeResrc(aRoot->matList, 1);
    }
    if (aRoot->morphList != NULL) {
        sdfItemArrayFree(aRoot->morphList);
    }
    if (aReleaseNodes != 0) {
        sdfDelayedResrcEntry(&sdfModelDelayedResrcCB, aRoot);
    } else {
        sdfItemArrayFree(aRoot->nodeList);
        sdfChipFreeDirect(aRoot);
    }
}
