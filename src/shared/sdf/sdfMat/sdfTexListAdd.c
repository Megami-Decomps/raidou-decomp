#include "shared/sdf/sdfMat/decls.h"

void sdfTexListAdd(sdfItemArray_t *aList, sdfTexHandle_t *aTexHandle)
{
    if (aList->count >= (s16)aList->capacity) {
        sdfItemArrayGrow(aList);
    }
    ((u32 *)aList->items)[aList->count] = EE_ADDR(aTexHandle);
    aList->count++;
}
