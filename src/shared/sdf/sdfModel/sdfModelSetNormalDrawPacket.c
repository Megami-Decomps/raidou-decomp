#include "shared/sdf/sdfModel/decls.h"

sdfDmaTag_t *sdfModelSetNormalDrawPacket(sdfModelNode_t *aNode, sdfShapePacket_t *aDataPtr,
                                         sdfDmaTag_t *aBuffer, s32 aId)
{
    u32 packed = *(u32 *)&aDataPtr->packetSize;
    u16 matIndex = packed >> 16;
    u16 qwc = packed;
    sdfDmaTag_t *body;

    body = sdfModelSetMatRefPacket(aNode->root, matIndex, aBuffer, aId);
    body->id = 0x30;
    body->qwc = qwc;
    body->addr = (void *)EE_PHYSICAL(EE_ADDR(aDataPtr->dataBody));
    body->dat0 = 0;
    body->dat1 = 0;
    return body + 1;
}
