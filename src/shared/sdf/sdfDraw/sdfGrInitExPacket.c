#include "shared/sdf/sdfDraw/decls.h"

void sdfGrInitExPacket(sdfDrawTag_t *p) {
    sdfGrRefPacketHeader_t *referenceNode;
    u32 priorHeadAddress;
    u32 sourceTagAddress;

    sourceTagAddress = p->fogPacket;
    if (sourceTagAddress != 0) {
        referenceNode = sdfGrNewExPacket(EE_ADDR_AS(SdfDmaTag *, sourceTagAddress));
        priorHeadAddress = EE_PHYSICAL(p->head);
        p->head = EE_ADDR(referenceNode);
        *(u64 *)&referenceNode->nextTag = ((s64)priorHeadAddress << 32) | SDF_DMA_TAG_NEXT_WORD;
    }
    sourceTagAddress = p->lightPacket;
    if (sourceTagAddress != 0) {
        referenceNode = sdfGrNewExPacket(EE_ADDR_AS(SdfDmaTag *, sourceTagAddress));
        priorHeadAddress = EE_PHYSICAL(p->head);
        p->head = EE_ADDR(referenceNode);
        *(u64 *)&referenceNode->nextTag = ((s64)priorHeadAddress << 32) | SDF_DMA_TAG_NEXT_WORD;
    }
}
