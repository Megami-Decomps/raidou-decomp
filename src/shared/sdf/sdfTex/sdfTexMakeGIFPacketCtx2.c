#include "shared/sdf/sdfTex/decls.h"

void sdfTexMakeGIFPacketCtx2(sdfTexHandle_t *aHandle)
{
    aHandle->packetCtx2 = sdfTexMakeGIFPacketSub(aHandle, 1);
}
