#include "shared/effect/effPCPBoss/decls.h"

effPCPCrack_t *effPCPCrack2Create(effPCPCrackParam_t *pParam, void *pPB)
{
    effPCPCrack_t *work;
    u32 groupIndex;

    work = effPCPCrackCreate(pParam, pPB);
    for (groupIndex = 0; groupIndex < work->MarkCount; groupIndex++) {
        work->pPart[groupIndex].rotationStartAge = pParam->rotationStartAge;
        work->pPart[groupIndex].rotate = 0.0f;
        work->pPart[groupIndex].rotate_add = pParam->angularSpeed;
    }
    return work;
}
