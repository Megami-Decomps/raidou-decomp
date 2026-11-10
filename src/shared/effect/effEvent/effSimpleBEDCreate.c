#include "shared/effect/effEvent/decls.h"

/* Initialize a holder and its effect record; the teardown flag starts set. */
effSimpleBED_t *effSimpleBEDCreate(effBTL_t *pData, f32 scale) {
    effSimpleBED_t *work = sdfChipAllocDirect(sizeof(effSimpleBED_t));

    work->pParams.rot[3] = 1.0f;
    work->pParams.r = 50.0f;
    work->pParams.h = 180.0f;
    work->pParams.center[1] = -90.0f;
    work->pParams.scale = scale;
    work->pParams.abgr = EFF_EVENT_NEUTRAL_COLOR;
    work->pParams.rot[0] = 0;
    work->pParams.rot[1] = 0;
    work->pParams.rot[2] = 0;
    work->pParams.center[0] = 0;
    work->pParams.center[2] = 0;
    work->pParams.center[3] = 0;
    work->hBED = effEveBEDCreate(pData);
    work->hCut = effEveBEDCutCreate(work->hBED, 0, &work->pParams);
    work->original = 1;
    return work;
}
