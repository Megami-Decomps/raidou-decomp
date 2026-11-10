#include "shared/sdf/sdfSortOt/decls.h"

void sdfGrSortOtReg(sdfGrOt_t *aOt, sdfDrawTag_t *aTag) {
    if (aTag->head != 0) {
        if (*(s32 *)&aTag->z == SDF_POOL_FREE_KIND) {
            aTag->next = aOt->noSortTag;
            aOt->noSortTag = aTag;
        } else {
            /* The key tree views the pool node's +0xC word as its float key. */
            sdfSortOtAvlRegist(aOt->data, aTag);
        }
    }
}
