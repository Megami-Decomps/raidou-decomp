#include "shared/effect/effPCPMisc/decls.h"

effPCPRadiateThunder_tag *effPCPRadiateThunderCreatePCF(void *args) {
    effPCPRadiateThunderParam_t *work;
    void *resources[4];
    u32 i;

    work = dds3EffectPCPDataGetData(args, 0);
    for (i = 0; i < 4; i++) {
        if (work->activeGroups[i] != 0) {
            resources[i] = dds3EffectPCPDataGetData(args, i + 1);
        } else {
            resources[i] = NULL;
        }
    }
    return effPCPRadiateThunderCreate(work, resources);
}
