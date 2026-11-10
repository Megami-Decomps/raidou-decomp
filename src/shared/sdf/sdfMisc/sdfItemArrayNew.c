#include "shared/sdf/sdfMisc/decls.h"

/* Allocate element storage with a fixed element stride and growth increment. */
sdfItemArray_t *sdfItemArrayNew(s32 aInitCapa, s32 aElemSize, s32 aGrowStep) {
    sdfItemArray_t *request = sdfChipAllocDirect(sizeof(*request));
    request->growStep = aGrowStep;
    request->count = 0;
    request->capacity = aInitCapa;
    request->elemSize = aElemSize;
    if (aInitCapa != 0) {
        request->memHandle = sdfMemAllocDirect(aElemSize * aInitCapa);
        request->items = EE_ADDR_AS(void *, sdfMemLock(request->memHandle));
    } else {
        request->memHandle = 0;
        request->items = 0;
    }
    return request;
}
