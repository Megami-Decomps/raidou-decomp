#include "shared/sdf/sdfModel/decls.h"

sdfDmaTag_t *sdfModelSetTmRefPacket(sdfModelNode_t *aNode, sdfDmaTag_t *aBuffer, s32 aId)
{
    addr_t addr = EE_PHYSICAL(EE_ADDR((u8 *)aNode->tmPacket + aId * sizeof(sdfModelTmImage_t)));

    *(u64 *)aBuffer = ((u64)addr << 32) | 0x30000008;
    aBuffer->dat1 = 0x6C07C000;
    aBuffer->dat0 = 0;
    return aBuffer + 1;
}
