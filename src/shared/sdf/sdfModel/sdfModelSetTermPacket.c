#include "shared/sdf/sdfModel/decls.h"

sdfDmaTag_t *sdfModelSetTermPacket(sdfDmaTag_t *aBuffer)
{
    *(u64 *)&aBuffer->dat0 = 0;
    *(u64 *)aBuffer = 0x60000000;
    return aBuffer + 1;
}
