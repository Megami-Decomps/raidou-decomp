#include "shared/effect/effPart/decls.h"

effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc)
{
    effPartHandle_t *hEffPart;

    hEffPart = sdfChipAllocDirect(sizeof(effPartHandle_t));
    hEffPart->type = hEffPartSrc->type;
    hEffPart->pHandle = dds3EffectFuncTable[hEffPartSrc->type].copy(hEffPartSrc->pHandle);
    return hEffPart;
}
