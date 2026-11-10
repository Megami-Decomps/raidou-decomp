#include "shared/effect/effPCPMisc/decls.h"

effPCPHama02_tag *effPCPHama02Create(void *args) {
    effPCPHama02_tag *work = sdfChipAllocDirect(sizeof(effPCPHama02_tag));
    effPartHandle_t **handle = &work->handle[1];
    s32 i = 0;

    do {
        if (i == 0) {
            work->handle[0] = dds3EffectPCPDataPartCreate(args, 0);
            work->handle[1] = dds3EffectPCPDataPartCreate(args, 1);
        } else {
            handle[-1] = dds3EffectPartCopy(work->handle[0]);
            handle[0] = dds3EffectPartCopy(work->handle[1]);
        }
        handle += 2;
        effPCPHama02PartInit(work, i++);
    } while (i < 8);
    work->color = 0x80808080;
    work->scale = 1.0f;
    work->position[0] = 0;
    work->position[1] = 0;
    work->position[2] = 0;
    return work;
}
