#include "shared/effect/effManager/decls.h"

effNode_t *dds3EffectCopy(effNode_t *node)
{
    effNode_t *copy = sdfChipAllocDirect(sizeof(effNode_t));
    s32 instance = node->instance;

    copy->type = node->type;
    copy->arg = node->arg;
    copy->instance = EffMajorTable[node->type].copy(instance);
    return copy;
}
