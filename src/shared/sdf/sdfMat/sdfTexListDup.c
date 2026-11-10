#include "shared/sdf/sdfMat/decls.h"

sdfItemArray_t *sdfTexListDup(sdfItemArray_t *aList)
{
    s32 count;
    sdfItemArray_t *dup;
    s32 i;

    if (aList == NULL) {
        return NULL;
    }
    count = aList->count;
    dup = sdfTexListNew(count);
    for (i = 0; i < count; i++) {
        ((u32 *)dup->items)[i] = EE_ADDR(sdfTexDup(EE_ADDR_AS(sdfTexHandle_t *, ((u32 *)aList->items)[i])));
    }
    dup->count = count;
    return dup;
}
