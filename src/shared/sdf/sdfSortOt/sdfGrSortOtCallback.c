#include "shared/sdf/sdfSortOt/decls.h"

void sdfGrSortOtCallback(sdfGrOt_t *aOt, s32 aFunc, sdfDrawTag_t *aData) {
    switch (aFunc) {
    case SDF_POOL_REBUILD_CHAIN:
        sdfGrSortOtSetup(aOt);
        break;
    case SDF_POOL_PUSH_FREE:
        if (aData->head != 0) {
            aData->next = aOt->noSortTag;
            aOt->noSortTag = aData;
        }
        break;
    }
}
