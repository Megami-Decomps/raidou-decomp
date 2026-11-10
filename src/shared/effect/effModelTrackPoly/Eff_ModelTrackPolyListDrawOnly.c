#include "shared/effect/effModelTrackPoly/decls.h"

void Eff_ModelTrackPolyListDrawOnly(effMTPolyListHandle_t *hMTPolyList) {
    u32 count = hMTPolyList->number;
    u32 workIndex = 0;

    while (workIndex < count) {
        Eff_ModelTrackPolyDrawOnly(hMTPolyList->hPolyList[workIndex]);
        workIndex++;
    }
}
