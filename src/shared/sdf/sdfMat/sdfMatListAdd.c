#include "shared/sdf/sdfMat/decls.h"

void sdfMatListAdd(sdfItemArray_t *aList, sdfMat_t *aMat)
{
    if (aList->count >= (s16)aList->capacity) {
        sdfMatListGrow(aList);
    }
    ((sdfMat_t **)aList->items)[aList->count] = aMat;
    aList->count++;
}
