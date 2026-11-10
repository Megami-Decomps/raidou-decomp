#include "shared/effect/effPCPMisc/decls.h"

effPCPAgiFireball_tag *effPCPAgiFireballCopy(effPCPAgiFireball_tag *src) {
    effPCPAgiFireball_tag *work;

    work = sdfChipAllocDirect(sizeof(effPCPAgiFireball_tag));
    memset(work, 0, 0x10C);
    work->param = src->param;
    work->frame = 0;
    work->color = 0x80808080;
    work->mode = 0;
    SDF_VU0_STOREIDENTITY(work->matrix);
    effPCPAgiFireballCopySub(work, src);
    work->source = NULL;
    return work;
}
