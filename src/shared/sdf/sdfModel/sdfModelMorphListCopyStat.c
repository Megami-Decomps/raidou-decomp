#include "shared/sdf/sdfModel/decls.h"

void sdfModelMorphListCopyStat(sdfModelMorphList_t *aDst, sdfModelMorphList_t *aSrc)
{
    s16 count;

    if (aDst == NULL) {
        return;
    }
    count = aDst->count;
    if (count > 0) {
        memcpy(aDst->items, aSrc->items, count * 16);
    }
}
