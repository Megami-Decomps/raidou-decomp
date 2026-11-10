#include "shared/sdf/sdfMat/decls.h"

void sdfMatUpdatePacketUvwParm(sdfMat_t *aMat, sdfMatPacket_t *aPacket)
{
    sdfMatSetUvwParm(&aPacket->texTm, aMat->uvwParm);
}
