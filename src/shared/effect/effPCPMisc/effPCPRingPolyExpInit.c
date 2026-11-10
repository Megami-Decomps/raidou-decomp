#include "shared/effect/effPCPMisc/decls.h"

void effPCPRingPolyExpInit(effPCPRingPolyExp_tag *pPCPRingPolyExp) {
    effPCPRingPolyExpSetVertex(pPCPRingPolyExp->param.radius_s, pPCPRingPolyExp);
    pPCPRingPolyExp->frame = 0;
}
