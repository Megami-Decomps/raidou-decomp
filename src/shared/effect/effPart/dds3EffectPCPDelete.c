#include "shared/effect/effPart/decls.h"

void dds3EffectPCPDelete(dds3EffectPCP_t *hData)
{
    EffectPCPFuncTable[hData->kateg].destroy(hData->pHandle);
    sdfChipFreeDirect(hData);
}
