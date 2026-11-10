#include "shared/effect/effModelTrackPoly/decls.h"

effMTPolyListHandle_t *Eff_ModelTrackPolyListCreate(Eff_ModelTrackPolyParam_t *pParam, u32 number) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(number * sizeof(effModelTrackPoly_t *) + sizeof(effMTPolyListHandle_t));
    effMTPolyListHandle_t *list = (effMTPolyListHandle_t *)sdfMemLock(handle);
    u32 workIndex;

    list->hMem = handle;
    list->hPolyList = (effModelTrackPoly_t **)(list + 1);
    list->number = number;
    for (workIndex = 0; workIndex < number; workIndex++) {
        list->hPolyList[workIndex] = Eff_ModelTrackPolyCreate(pParam);
    }
    return list;
}
