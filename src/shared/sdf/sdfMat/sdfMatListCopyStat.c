#include "shared/sdf/sdfMat/decls.h"

void sdfMatListCopyStat(sdfItemArray_t *aDst, sdfItemArray_t *aSrc)
{
    s32 i;
    s32 count = aDst->count;
    u32 *src = aSrc->items;
    u32 *dst = aDst->items;

    for (i = 0; i < count; i++) {
        sdfMatCopyStat(EE_ADDR_AS(sdfMat_t *, dst[i]), EE_ADDR_AS(sdfMat_t *, src[i]));
    }
}
