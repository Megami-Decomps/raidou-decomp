#include "shared/sdf/sdfDraw/decls.h"

SdfDmaTag *sdfGrAddExPacket(SdfDmaTag *p, SdfDmaTag *ex) {
    sdfGrRefPacketHeader_t *referenceNode;

    referenceNode = sdfGrNewExPacket(ex);
    p->kind = SDF_DMA_TAG_NEXT_BYTE;
    p->address = EE_PHYSICAL(EE_ADDR(referenceNode));
    return (SdfDmaTag *)(EE_ADDR(referenceNode) + SDF_QWORD_BYTES);
}
