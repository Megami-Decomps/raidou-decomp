#include "shared/effect/effEvent/decls.h"

/* Copy the packed 0x30-byte record while retaining its native packed layout. */
void effEveBEDCutSetParams(effEveBEDCut_t *hEveBEDCut, const void *pParams) {
    *(effEveBEDRecord_t *)hEveBEDCut = *(const effEveBEDRecord_t *)pParams;
}
