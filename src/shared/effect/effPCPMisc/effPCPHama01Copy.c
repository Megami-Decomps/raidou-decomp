#include "shared/effect/effPCPMisc/decls.h"

effPCPHama01_tag *effPCPHama01Copy(effPCPHama01_tag *src) {
    effPCPHama01_tag *work = sdfChipAllocDirect(sizeof(effPCPHama01_tag));
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < 8; slotIndex++) {
        work->pair[slotIndex][0] = dds3EffectPartCopy(src->pair[slotIndex & 1][0]);
        work->pair[slotIndex][1] = dds3EffectPartCopy(src->pair[slotIndex & 1][1]);
        work->shared[slotIndex] = dds3EffectPartCopy(src->shared[0]);
        work->state[slotIndex] = 0;
        effPCPHama01PartInit(work, slotIndex);
    }
    work->pos[0] = 0.0f;
    work->abgr = 0x80808080;
    work->scale = 1.0f;
    work->pos[1] = 0.0f;
    work->pos[2] = 0.0f;
    work->frame = 0;
    return work;
}
