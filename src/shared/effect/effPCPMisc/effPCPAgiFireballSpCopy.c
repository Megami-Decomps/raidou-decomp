#include "shared/effect/effPCPMisc/decls.h"

effPCPAgiFireball_tag *effPCPAgiFireballSpCopy(effPCPAgiFireball_tag *src) {
    effPCPAgiFireball_tag *work;

    work = sdfChipAllocDirect(sizeof(effPCPAgiFireball_tag));
    memset(work, 0, 0x10C);
    work->param = src->param;
    work->color = 0x80808080;
    work->mode = 1;
    work->frame = 0;
    SDF_VU0_STOREIDENTITY(work->matrix);
    work->source = src;
    return work;
}
