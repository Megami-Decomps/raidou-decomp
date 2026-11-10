#include "shared/effect/effPCPMisc/decls.h"

effPCPMHanma01_tag *effPCPMHanma01Create(void *source) {
    effPCPMHanma01_tag *work = sdfChipAllocDirect(sizeof(effPCPMHanma01_tag));
    effPartHandle_t **handle = &work->handle[1];
    s32 i;

    for (i = 0; i < 6; i++) {
        if (i == 0) {
            work->handle[1] = dds3EffectPCPDataPartCreate(source, 6);
        }
        handle[-1] = dds3EffectPCPDataPartCreate(source, i);
        handle[0] = dds3EffectPartCopy(work->handle[1]);
        handle += 2;
        effPCPMHanma01PartInit(work, i);
    }
    work->abgr = 0x80808080;
    work->scale = 1.0f;
    work->pos[0] = 0.0f;
    work->pos[1] = 0.0f;
    work->pos[2] = 0.0f;
    return work;
}
