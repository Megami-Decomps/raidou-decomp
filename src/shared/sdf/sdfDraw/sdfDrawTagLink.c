#include "shared/sdf/sdfDraw/decls.h"

void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket) {
    s32 last;

    last = aTag->tail;
    if (last == 0) {
        aTag->head = aPacket;
    }
    else {
        (EE_ADDR_AS(SdfDmaTag *, last))->kind = SDF_DMA_TAG_NEXT_BYTE;
        (EE_ADDR_AS(SdfDmaTag *, last))->address = EE_PHYSICAL(aPacket);
    }
    aTag->tail = aPacket;
}
