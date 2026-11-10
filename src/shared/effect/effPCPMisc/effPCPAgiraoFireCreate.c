#include "shared/effect/effPCPMisc/decls.h"

effPCPAgiraoFire_tag *effPCPAgiraoFireCreate(effPCPAgiraoFireParam_t *pParam, effPCPAgiraoFirePartList_t **pPartList) {
    effPCPAgiraoFire_tag *work;
    struct effHandle_t **handle;
    u32 i;

    work = sdfChipAllocDirect(sizeof(effPCPAgiraoFire_tag));
    work->head = *pParam;
    work->frame = 0;
    work->color = 0x80808080;
    handle = work->handles;
    for (i = 0; i < 7; i++) {
        handle[0] = dds3EffectLoad(pPartList[i]);
        handle[7] = dds3EffectCopy(handle[0]);
        handle[14] = dds3EffectCopy(handle[0]);
        handle++;
    }
    return work;
}
