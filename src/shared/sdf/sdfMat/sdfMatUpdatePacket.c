#include "shared/sdf/sdfMat/decls.h"

void sdfMatUpdatePacket(sdfMat_t *aMat, s32 aId)
{
    u8 duty = aMat->duty;
    sdfMatPacket_t *packet = aMat->packet[aId];

    if ((duty >> aId) & 1) {
        sdfMatUpdatePacketBasic(aMat, packet);
    }
    if (duty & (4 << aId)) {
        sdfMatUpdatePacketUvwParm(aMat, packet);
    }
    if (duty & (0x10 << aId)) {
        sdfMatUpdatePacketMultiTex(aMat, packet);
    }
    if (duty & (0x40 << aId)) {
        sdfMatUpdatePacketFaceDiv(aMat, packet);
    }
    aMat->duty = duty & (0x55 << (aId ^ 1));
}
