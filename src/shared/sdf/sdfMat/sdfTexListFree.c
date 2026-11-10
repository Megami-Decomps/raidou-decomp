#include "shared/sdf/sdfMat/decls.h"

void sdfTexListFree(sdfItemArray_t *aList, s32 aReleaseItems)
{
    s32 i;
    s32 count;

    if (aList == NULL) {
        return;
    }
    if (aReleaseItems != 0) {
        count = aList->count;
        for (i = 0; i < count; i++) {
            sdfTexFreeDelayed(EE_ADDR_AS(sdfTexHandle_t *, ((u32 *)aList->items)[i]));
        }
    }
    sdfItemArrayFree(aList);
}
