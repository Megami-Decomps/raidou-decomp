#include "shared/sdf/sdfDraw/decls.h"

sdfGrRefPacketHeader_t *sdfGrNewExPacket(SdfDmaTag *ex) {
    sdfGrRefPacketHeader_t *referenceNode = EE_ADDR_AS(sdfGrRefPacketHeader_t *, sdfGrAllocPacketBuff(SDF_DMA_REFERENCE_NODE_BYTES));
    u64 dmaHeader = ex->quadwordCount;
    addr_t payloadAddress = EE_PHYSICAL(EE_ADDR(ex) + SDF_QWORD_BYTES);
    s64 packedAddress = (s64)payloadAddress << 32;

    dmaHeader |= SDF_DMA_TAG_REF_WORD;
    dmaHeader |= packedAddress;
    referenceNode->dmaTag = dmaHeader;
    QUAD_ZERO(referenceNode->nextTag);
    referenceNode->vifCommands = ex->vifCommands;
    return referenceNode;
}
