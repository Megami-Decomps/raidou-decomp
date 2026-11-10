#include "shared/sdf/sdfDelayedResrc/decls.h"

void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, SdfPendingCallback aFreeFunc) {
    if (sdfDelayedResrcSemaId < 0) {
        sdfDelayedResrcSemaId = sdfCreateSema(1, 0x7f, 0);
    }
    aCB->freeFuncPtr = aFreeFunc;
    aCB->chunk = NULL;
}
