#include "shared/sdf/sdfDraw/decls.h"

void sdfDrawTagLinkCall(sdfDrawTag_t *aTag, addr_t aPacket) {
    s32 previousTailAddress;

    (EE_ADDR_AS(SdfDmaTag *, aPacket))->kind = SDF_DMA_TAG_CALL_BYTE;
    previousTailAddress = aTag->tail;
    if (previousTailAddress == 0) {
        aTag->head = aPacket;
    }
    else {
        (EE_ADDR_AS(SdfDmaTag *, previousTailAddress))->kind = SDF_DMA_TAG_NEXT_BYTE;
        (EE_ADDR_AS(SdfDmaTag *, previousTailAddress))->address = EE_PHYSICAL(aPacket);
    }
    aTag->tail = aPacket + SDF_QWORD_BYTES;
}
