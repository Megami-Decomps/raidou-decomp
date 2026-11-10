#include "shared/effect/effPCPMisc/decls.h"

effPCPMudo_tag *effPCPMudoCopy(effPCPMudo_tag *source) {
    sdfMemHandle_t *resource = sdfMemAllocDirect(0x1354);
    effPCPMudo_tag *work = EE_ADDR_AS(void *, sdfMemLock(resource));
    effPartHandle_t *firstHandle = source->primaryHandle;
    work->allocationHandle = resource;
    work->primaryHandle = dds3EffectPartCopy(firstHandle);
    work->secondaryHandle = dds3EffectPartCopy(source->secondaryHandle);
    effPCPMudoPartInit(work);
    work->vectorWords[0] = 0;
    work->vectorWords[1] = 0;
    work->vectorWords[2] = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    return work;
}
