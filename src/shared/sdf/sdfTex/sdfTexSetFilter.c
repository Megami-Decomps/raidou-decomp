#include "shared/sdf/sdfTex/decls.h"

void sdfTexSetFilter(sdfTexHandle_t *aHandle, s32 aMagMode, s32 aMinMode)
{
    sdfTexPacket_t *packet;

    packet = aHandle->packet;
    packet->tex1.data = (packet->tex1.data & ~0x1E0) | (aMagMode << 5) | (aMinMode << 6);
}
