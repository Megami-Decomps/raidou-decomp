#include "shared/effect/effPCPMisc/decls.h"

effPCPMHanma01_tag *effPCPMHanma01Copy(effPCPMHanma01_tag *source) {
    effPartHandle_t **sourceHandle;
    effPartHandle_t **workHandle;
    s32 i;
    effPCPMHanma01_tag *work = sdfChipAllocDirect(sizeof(effPCPMHanma01_tag));
    sourceHandle = &source->handle[1];
    workHandle = &work->handle[1];

    for (i = 0; i < 6; i++) {
        workHandle[-1] = dds3EffectPartCopy(sourceHandle[-1]);
        workHandle[0] = dds3EffectPartCopy(sourceHandle[0]);
        sourceHandle += 2;
        workHandle += 2;
        effPCPMHanma01PartInit(work, i);
    }
    work->abgr = 0x80808080;
    work->scale = 1.0f;
    work->pos[0] = 0.0f;
    work->pos[1] = 0.0f;
    work->pos[2] = 0.0f;
    return work;
}
