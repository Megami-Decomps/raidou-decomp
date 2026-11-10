#include "shared/effect/effPCPMisc/decls.h"

effPCPAgiFireball_tag *effPCPAgiFireballSpCreatePCF(void *parameterTable) {
    effPCPAgiFireball_tag *work;

    work = effPCPAgiFireballCreatePCF(parameterTable);
    work->mode = 1;
    return work;
}
