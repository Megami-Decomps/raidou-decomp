#include "shared/sdf/sdfMat/decls.h"

void sdfMatListFree(sdfItemArray_t *aList)
{
    s32 i;

    for (i = 0; i < aList->count; i++) {
        sdfMatFree(((sdfMat_t **)aList->items)[i]);
    }
    sdfItemArrayFree(aList);
}
