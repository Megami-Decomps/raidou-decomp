#include "shared/sdf/sdfIpu/decls.h"

/* Hand the finished buffer's packet to the image set, building it on first use. */
s32 sdfIpuSetChainTag(sdfIpuDecodeCb_t *cb)
{
    u32 *packet;

    if (cb->activeBuffCount == 0) {
        return 0;
    }
    packet = EE_ADDR_AS(u32 *, (cb->getBuffId * 4 + EE_ADDR_S(cb) + 0x28));
    if (*packet == 0) {
        sdfIpuMakeBltPacket(cb);
    }
    sdfImageSetChainTag(*packet, EE_ADDR_AS(void *, (*packet + cb->transferPacketBytes - 0x10)));
    sdfIpuFinishActiveFrame(cb);
    return 1;
}
