#include "shared/effect/effPCPMisc/decls.h"

effPCPHama01_tag *effPCPHama01Create(void *src) {
    effPCPHama01_tag *work = sdfChipAllocDirect(sizeof(effPCPHama01_tag));
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < 8; slotIndex++) {
        if (slotIndex == 0) {
            work->pair[0][0] = dds3EffectPCPDataPartCreate(src, 0);
            work->pair[0][1] = dds3EffectPCPDataPartCreate(src, 2);
        } else if (slotIndex == 1) {
            work->pair[1][0] = dds3EffectPCPDataPartCreate(src, 1);
            work->pair[1][1] = dds3EffectPartCopy(work->pair[0][1]);
        } else {
            work->pair[slotIndex][0] = dds3EffectPartCopy(work->pair[slotIndex & 1][0]);
            work->pair[slotIndex][1] = dds3EffectPartCopy(work->pair[slotIndex & 1][1]);
        }
        effPCPHama01PartInit(work, slotIndex);
    }
    work->shared[0] = dds3EffectPCPDataPartCreate(src, 3);
    work->state[0] = 0;
    for (slotIndex = 1; slotIndex < 8; slotIndex++) {
        work->shared[slotIndex] = dds3EffectPartCopy(work->shared[0]);
        work->state[slotIndex] = 0;
    }
    work->pos[0] = 0.0f;
    work->abgr = 0x80808080;
    work->scale = 1.0f;
    work->pos[1] = 0.0f;
    work->pos[2] = 0.0f;
    work->frame = 0;
    return work;
}
