#include "shared/sdf/sdfDraw/decls.h"

void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, addr_t aPacket) {
    s32 previousTailAddress;

    previousTailAddress = aTag->tail;
    if (previousTailAddress == 0) {
        aTag->head = aPacket;
    }
    else {
        (EE_ADDR_AS(SdfDmaTag *, previousTailAddress))->kind = SDF_DMA_TAG_NEXT_BYTE;
        (EE_ADDR_AS(SdfDmaTag *, previousTailAddress))->address = EE_PHYSICAL(aPacket);
    }
    aTag->tail = aPacket + SDF_DMA_EXTENDED_TAIL_OFFSET;
}
