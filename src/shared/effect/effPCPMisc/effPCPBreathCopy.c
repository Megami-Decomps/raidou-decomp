#include "shared/effect/effPCPMisc/decls.h"

effPCPBreath_tag *effPCPBreathCopy(effPCPBreath_tag *work) {
    effPCPBreath_tag *child;

    child = effPCPBreathCreate(&work->param, NULL);
    if (work->hD3P != NULL) {
        child->hD3P = dds3EffectCopy(work->hD3P);
    }
    return child;
}
