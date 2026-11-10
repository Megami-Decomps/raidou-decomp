#include "shared/effect/effEvent/decls.h"

/* Clone the init block/effect using the same handle; clear the shared-teardown flag. */
effSimpleBED_t *effSimpleBEDCopy(effSimpleBED_t *hSimpleBEDSrc) {
    effEveBEDParams_t *block = &hSimpleBEDSrc->pParams;
    effSimpleBED_t *work = sdfChipAllocDirect(sizeof(effSimpleBED_t));

    work->hCut = effEveBEDCutCreate(hSimpleBEDSrc->hBED, 0, block);
    work->hBED = hSimpleBEDSrc->hBED;
    memcpy(&work->pParams, block, sizeof(work->pParams));
    work->original = 0;
    return work;
}
