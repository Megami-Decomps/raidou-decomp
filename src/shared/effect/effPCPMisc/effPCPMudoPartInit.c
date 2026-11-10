#include "shared/effect/effPCPMisc/decls.h"

void effPCPMudoPartInit(effPCPMudo_tag *pPCPMudo) {
    pPCPMudo->historyCount = 0;
    pPCPMudo->updateCount = 0;
    pPCPMudo->baseColor = 0x80808080;
}
