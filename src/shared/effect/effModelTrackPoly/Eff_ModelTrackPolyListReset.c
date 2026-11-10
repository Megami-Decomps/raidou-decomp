#include "shared/effect/effModelTrackPoly/decls.h"

void Eff_ModelTrackPolyListReset(effMTPolyListHandle_t *hMTPolyList, s32 index) {
    Eff_ModelTrackPolyReset(hMTPolyList->hPolyList[index]);
}
