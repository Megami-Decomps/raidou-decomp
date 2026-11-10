#include "shared/effect/effPCPMisc/decls.h"

effPCPAgiFireball_tag *effPCPAgiFireballCreatePCF(void *parameterTable) {
    effPCPAgiFireballPartList_t set;
    effPCPAgiFireballParam_t *first;
    u32 i;

    first = dds3EffectPCPDataGetData(parameterTable, 0);
    set.pHelper = dds3EffectPCPDataGetData(parameterTable, 1);
    for (i = 0; i < 5; i++) {
        set.pAttach[i] = dds3EffectPCPDataGetData(parameterTable, 2 + i);
    }
    set.pTrack[0] = dds3EffectPCPDataGetData(parameterTable, 7);
    set.pTrack[1] = dds3EffectPCPDataGetData(parameterTable, 8);
    set.pTrack[2] = dds3EffectPCPDataGetData(parameterTable, 9);
    for (i = 0; i < 5; i++) {
        set.pStart[i] = dds3EffectPCPDataGetData(parameterTable, 10 + i);
    }
    set.pOkSmoke = dds3EffectPCPDataGetData(parameterTable, 15);
    return effPCPAgiFireballCreate(first, &set);
}
