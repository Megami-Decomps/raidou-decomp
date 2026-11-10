#include "shared/sdf/sdfTex/decls.h"

void sdfTexMakeGIFPacket(sdfTexHandle_t *aHandle)
{
    aHandle->packet = sdfTexMakeGIFPacketSub(aHandle, 0);
}
