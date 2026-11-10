#include "shared/sdf/sdfMat/decls.h"

void sdfMatUpdatePacketFaceDiv(sdfMat_t *aMat, sdfMatPacket_t *aPacket)
{
    aPacket->fdBias = aMat->fdBias;
    aPacket->fdSoften = aMat->fdSoften;
}
