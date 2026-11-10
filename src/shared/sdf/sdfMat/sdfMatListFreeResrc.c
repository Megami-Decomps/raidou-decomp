#include "shared/sdf/sdfMat/decls.h"

void sdfMatListFreeResrc(void *aList, s32 aDelayed)
{
    if (aList == NULL) {
        return;
    }
    if (aDelayed != 0) {
        sdfDelayedResrcEntry(&sdfMatDelayedResrcCB, aList);
    } else {
        sdfItemArrayFree(aList);
    }
}
