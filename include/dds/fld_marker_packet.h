#ifndef DDS_FLD_MARKER_PACKET_H

#define DDS_FLD_MARKER_PACKET_H
#include "eff.h"

/* Stack-built input shared by the field marker packet producers and submitter. */
typedef struct FldMarkerPacket {
    f32 pos[3];
    u8 opaque0C[4];
    BillTextureQuad uv;
    f32 quad[8];
    s32 color;
    f32 scale;
} FldMarkerPacket;

typedef char FldMarkerPacketSizeCheck[
    (sizeof(FldMarkerPacket) == 0x48) ? 1 : -1];

#endif
