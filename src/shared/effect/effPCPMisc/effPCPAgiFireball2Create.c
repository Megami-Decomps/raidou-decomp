#include "shared/effect/effPCPMisc/decls.h"

effPCPAgiFireball2_tag *effPCPAgiFireball2Create(effPCPAgiFireball2Param_t *pAFB2Param, effPCPBlockSetInputs_t *pAFB2Part) {
    effPCPAgiFireball2_tag *work;
    effPCPAgiFireballParam_t *sub;
    u32 i;

    work = sdfChipAllocDirect(sizeof(effPCPAgiFireball2_tag));
    work->params = *pAFB2Param;
    sub = pAFB2Param->group;
    work->frame = 0;
    for (i = 0; i < 3; i++) {
        pAFB2Part->selected.pHelper = pAFB2Part->head[i];
        work->blockSets[i] = effPCPAgiFireballCreate(sub, &pAFB2Part->selected);
        sub++;
    }
    return work;
}
