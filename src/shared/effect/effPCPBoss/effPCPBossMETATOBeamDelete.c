#include "shared/effect/effPCPBoss/decls.h"

void effPCPBossMETATOBeamDelete(effPCPBossMETATOBeam_t *hData)
{
    effPCP_ThunderPolyListDelete(hData->polyLists[1]);
    effPCP_ThunderPolyListDelete(hData->polyLists[0]);
    sdfChipFreeDirect(hData);
}
