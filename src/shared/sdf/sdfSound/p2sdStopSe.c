#include "shared/sdf/sdfSound/decls.h"

/* Command 0x30 copies a full 16-byte payload; only its identifier is set. */
void p2sdStopSe(u32 soundId) {
    u32 packet[4];

    packet[0] = soundId;
    sdfSndSendCmd(0x30, 0, packet, sizeof(packet));
}
