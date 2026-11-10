#include "shared/effect/effPCPBoss/decls.h"

effPCPCrack_t *effPCPCrack2Copy(effPCPCrack_t *src)
{
    effPCPCrack_t *work;
    u32 groupIndex;

    work = effPCPCrackCopy(src);
    for (groupIndex = 0; groupIndex < work->MarkCount; groupIndex++) {
        work->pPart[groupIndex].rotationStartAge = src->pPart[groupIndex].rotationStartAge;
        work->pPart[groupIndex].rotate = 0.0f;
        work->pPart[groupIndex].rotate_add = src->pPart[groupIndex].rotate_add;
    }
    return work;
}
