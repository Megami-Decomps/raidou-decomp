#include "shared/effect/effManager/decls.h"

void dds3EffectDelete(effNode_t *pData)
{
    EffMajorTable[pData->type].del(pData->instance);
    sdfChipFreeDirect(pData);
}
