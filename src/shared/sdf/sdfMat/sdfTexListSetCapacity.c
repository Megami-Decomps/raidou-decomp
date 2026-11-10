#include "shared/sdf/sdfMat/decls.h"

void sdfTexListSetCapacity(sdfItemArray_t *aList, s32 aCapa, s32 aReleaseItems)
{
    s32 i;
    s32 count;

    count = aList->count;
    if (aCapa < count && aReleaseItems != 0) {
        /* Retail only counts down to the new capacity here; the loop body is
         * empty and ee-gcc pads it to the R5900 short-loop minimum. */
        i = aCapa;
        do {
            i++;
        } while (i != count);
        aList->count = aCapa;
    }
    func_002BFF98(aList, aCapa);
}
