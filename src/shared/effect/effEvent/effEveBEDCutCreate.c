#include "shared/effect/effEvent/decls.h"

/* Allocate the compact record, copy its init prefix, then attach the new effect. */
effEveBEDCut_t *effEveBEDCutCreate(effBTL_t *hEveBED, u16 CutNum, const void *pParams) {
    const effEveBEDParams_t *init = (const effEveBEDParams_t *)pParams;
    effEveBEDCut_t *work = sdfChipAllocDirect(sizeof(effEveBEDCut_t));

    memcpy(work, init, sizeof(*init));
    work->actor = NULL;
    work->effect = effBTLCutCreate(hEveBED, CutNum, 0, 0);
    effBTLCutSetScale(work->effect, init->scale);
    return work;
}
