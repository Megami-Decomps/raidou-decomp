#include "shared/effect/effPCPMisc/decls.h"

effPCPAgiraoFire_tag *effPCPAgiraoFireCreatePCF(void *data) {
    effPCPAgiraoFireParam_t *params;
    effPCPAgiraoFirePartList_t *descriptors[7];
    effPCPAgiraoFirePartList_t **dst;
    u32 i;

    params = dds3EffectPCPDataGetData(data, 0);
    dst = descriptors;
    i = 0;
    do {
        i++;
        *dst = dds3EffectPCPDataGetData(data, i);
        dst++;
    } while (i < 7);
    return effPCPAgiraoFireCreate(params, descriptors);
}
