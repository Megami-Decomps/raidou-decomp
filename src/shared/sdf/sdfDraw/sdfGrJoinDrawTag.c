#include "shared/sdf/sdfDraw/decls.h"

void sdfGrJoinDrawTag(sdfDrawTag_t *aFore, sdfDrawTag_t *aBack) {
    SdfDmaTag *tailTag = EE_ADDR_AS(SdfDmaTag *, aFore->tail);
    u32 previousSource;
    u32 incomingSource;

    previousSource = aFore->lightPacket;
    incomingSource = aBack->lightPacket;
    if (previousSource != incomingSource) {
        if (incomingSource != 0) {
            tailTag = sdfGrAddExPacket(tailTag, EE_ADDR_AS(SdfDmaTag *, incomingSource));
        } else {
            aBack->lightPacket = previousSource;
        }
    }
    previousSource = aFore->fogPacket;
    incomingSource = aBack->fogPacket;
    if (previousSource != incomingSource) {
        if (incomingSource != 0) {
            tailTag = sdfGrAddExPacket(tailTag, EE_ADDR_AS(SdfDmaTag *, incomingSource));
        } else {
            aBack->fogPacket = previousSource;
        }
    }
    tailTag->kind = SDF_DMA_TAG_NEXT_BYTE;
    tailTag->address = EE_PHYSICAL(aBack->head);
}
