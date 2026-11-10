#include "shared/effect/effModelTrackPoly/decls.h"

void Eff_ModelTrackPolyListDelete(effMTPolyListHandle_t *list) {
    u32 count = list->number;
    u32 workIndex = 0;

    while (workIndex < count) {
        Eff_ModelTrackPolyDelete(list->hPolyList[workIndex]);
        workIndex++;
    }
    sdfMemFreeDirect(list->hMem);
}
