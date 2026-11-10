#include "shared/effect/effPCPMisc/decls.h"

effPCPHama02_tag *effPCPHama02Copy(effPCPHama02_tag *source) {
    effPCPHama02_tag *work = sdfChipAllocDirect(sizeof(effPCPHama02_tag));
    effPartHandle_t **handle = &work->handle[1];
    s32 i = 0;

    do {
        handle[-1] = dds3EffectPartCopy(source->handle[0]);
        handle[0] = dds3EffectPartCopy(source->handle[1]);
        handle += 2;
        effPCPHama02PartInit(work, i);
        i++;
    } while (i < 8);
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->position[0] = 0;
    work->position[1] = 0;
    work->position[2] = 0;
    return work;
}
