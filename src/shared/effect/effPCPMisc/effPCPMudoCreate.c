#include "shared/effect/effPCPMisc/decls.h"

effPCPMudo_tag *effPCPMudoCreate(void *source) {
    sdfMemHandle_t *resource = sdfMemAllocDirect(0x1354);
    effPCPMudo_tag *work = EE_ADDR_AS(void *, sdfMemLock(resource));
    work->allocationHandle = resource;
    work->primaryHandle = dds3EffectPCPDataPartCreate(source, 0);
    work->secondaryHandle = dds3EffectPCPDataPartCreate(source, 1);
    effPCPMudoPartInit(work);
    work->vectorWords[0] = 0;
    work->vectorWords[1] = 0;
    work->vectorWords[2] = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    return work;
}
