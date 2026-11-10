#include "shared/effect/effPart/decls.h"

void dds3EffectPCPDraw(dds3EffectPCP_t *hData)
{
    EffectPCPFuncTable[hData->kateg].draw(hData->pHandle);
}
