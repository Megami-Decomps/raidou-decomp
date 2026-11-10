#include "shared/effect/effBattleMisc/decls.h"

/* The selected position provider leaves its vector in vf10. */
void effBTLPartGetPositionPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr, sdfVector4_t *pPos) {
    GetPositionPtrTable[pPosPtr->scaleKind](pFormation, pPosPtr);
    SDF_VU0_STOREVECTOR(pPos);
}
