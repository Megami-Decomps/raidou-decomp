#include "shared/effect/effPCPMisc/decls.h"

effPCPAgiraoFire_tag *effPCPAgiraoFireCopy(effPCPAgiraoFire_tag *src) {
    effPCPAgiraoFire_tag *work;
    struct effHandle_t **sourceHandleCursor;
    struct effHandle_t **destinationHandleCursor;
    u32 slotIndex;

    work = sdfChipAllocDirect(sizeof(effPCPAgiraoFire_tag));
    work->head = src->head;
    work->frame = 0;
    work->color = 0x80808080;
    sourceHandleCursor = (src->handles + 14);
    destinationHandleCursor = (work->handles + 14);
    for (slotIndex = 0; slotIndex < 7; slotIndex++) {
        destinationHandleCursor[-14] = dds3EffectCopy(sourceHandleCursor[-14]);
        destinationHandleCursor[-7] = dds3EffectCopy(sourceHandleCursor[-7]);
        destinationHandleCursor[0] = dds3EffectCopy(sourceHandleCursor[0]);
        sourceHandleCursor++;
        destinationHandleCursor++;
    }
    return work;
}
