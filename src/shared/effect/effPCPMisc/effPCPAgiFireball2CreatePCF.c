#include "shared/effect/effPCPMisc/decls.h"

effPCPAgiFireball2_tag *effPCPAgiFireball2CreatePCF(void *args) {
    effPCPBlockSetInputs_t set;
    void **group;
    void *first;
    s32 idx;
    u32 i;
    u32 j;

    idx = 1;
    first = dds3EffectPCPDataGetData(args, 0);
    for (i = 0; i < 3; i++) {
        set.head[i] = dds3EffectPCPDataGetData(args, idx++);
    }
    group = &set.selected.pAttach[0];
    for (j = 0; j < 5; j++) {
        group[j] = dds3EffectPCPDataGetData(args, idx++);
    }
    for (j = 0; j < 3; j++) {
        set.selected.pTrack[j] = dds3EffectPCPDataGetData(args, idx++);
    }
    for (j = 0; j < 5; j++) {
        set.selected.pStart[j] = dds3EffectPCPDataGetData(args, idx++);
    }
    set.selected.pOkSmoke = dds3EffectPCPDataGetData(args, idx);
    return effPCPAgiFireball2Create(first, &set);
}
