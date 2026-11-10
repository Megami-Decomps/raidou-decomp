#include "shared/sdf/sdfMat/decls.h"

void sdfMatFreeDelayed(sdfMat_t *aMat)
{
    if (aMat != NULL) {
        sdfDelayedResrcEntry(&sdfMatDelayedResrcCB2, aMat);
    }
}
