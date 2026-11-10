#include "shared/effect/effPCPMisc/decls.h"

effPCPAgiFireball2_tag *effPCPAgiFireball2Copy(effPCPAgiFireball2_tag *src) {
    effPCPAgiFireball2_tag *work;
    effPCPAgiFireball_tag *sub;
    u32 i;

    work = sdfChipAllocDirect(sizeof(effPCPAgiFireball2_tag));
    work->params = src->params;
    work->frame = 0;
    for (i = 0; i < 3; i++) {
        work->blockSets[i] = sdfChipAllocDirect(sizeof(effPCPAgiFireball_tag));
        memset(work->blockSets[i], 0, 0x10C);
        memcpy(&work->blockSets[i]->param, &src->blockSets[i]->param,
               sizeof(work->blockSets[i]->param));
        sub = work->blockSets[i];
        sub->frame = 0;
        sub->color = 0x80808080;
        sub->mode = 0;
        SDF_VU0_STOREIDENTITY(sub->matrix);
        sub->source = src->blockSets[i];
    }
    return work;
}
