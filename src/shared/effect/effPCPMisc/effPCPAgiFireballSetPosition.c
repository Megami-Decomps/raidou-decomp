#include "shared/effect/effPCPMisc/decls.h"

void effPCPAgiFireballSetPosition(effPCPAgiFireball_tag *hData, const void *pPos) {
    SDF_VU0_COPYVECTOR(hData->param.pos, pPos);
}
