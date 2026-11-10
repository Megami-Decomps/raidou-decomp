#include "shared/sdf/sdfMat/decls.h"

sdfDmaTag_t *sdfMatMakePacket(sdfMat_t *aMat, sdfDmaTag_t *aBuffer, s32 aId)
{
    aBuffer->id = 0x30;
    aBuffer->addr = (void *)EE_PHYSICAL(EE_ADDR(aMat->packet[aId]));
    aBuffer->qwc = 0xA;
    aBuffer->dat0 = 0;
    aBuffer->dat1 = 0;
    return (sdfDmaTag_t *)((u8 *)aBuffer + 0x10);
}
