#include "shared/sdf/sdfSound/decls.h"

/* Starts a track with the default 0x7f setting after preparing its identifier. */
void p2sdPlayBGM(s32 MidiNo) {
    sdfSndTrackPacket_t packet;
    p2sdLoadSMGFile(MidiNo);
    packet.trackId = MidiNo;
    packet.unk4 = 0;
    packet.setting = 0x7F;
    sdfSndSendCmd(0x20, 0, &packet, 0x10);
}
