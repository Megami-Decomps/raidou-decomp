#include "shared/effect/effPCPMisc/decls.h"

effPCPAgiFireball_tag *effPCPAgiFireballSp2CreatePCF(void *parameterTable) {
    effPCPAgiFireball_tag *work;

    work = effPCPAgiFireballCreatePCF(parameterTable);
    work->mode = 2;
    return work;
}
