#include "shared/effect/effPart/decls.h"

effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData)
{
    effPartHandle_t *hEffPart;

    hEffPart = sdfChipAllocDirect(sizeof(effPartHandle_t));
    hEffPart->type = type;
    hEffPart->pHandle = dds3EffectFuncTable[type].create(pData);
    return hEffPart;
}
