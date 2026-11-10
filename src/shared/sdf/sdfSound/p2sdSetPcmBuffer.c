#include "shared/sdf/sdfSound/decls.h"

void p2sdSetPcmBuffer(saddr_t data, s32 size) {
    s32 *packet;
    sdfSndPcmRequest.size = size;
    sdfSndPcmRequest.pending = 1;
    sdfSndPcmRequest.data = data;
    if (data != 0 && size > 0) {
        packet = &sdfSndPcmBufferAddr;
        *packet = data;
        sdfSndSendSyncCmd(0x1B0, 0, packet, 4);
    }
    p2sdSmgHandle = -1;
}
