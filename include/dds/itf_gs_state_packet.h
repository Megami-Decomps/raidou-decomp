#ifndef DDS_ITF_GS_STATE_PACKET_H

#define DDS_ITF_GS_STATE_PACKET_H
#include "common.h"

/* State writers finalize a 0x30-byte packet and fill these two trailing qwords. */
typedef struct ItfGsStatePacket {
    u8 pad00[0x20];
    u64 value;        /* 0x20: GS register payload */
    u64 registerCode; /* 0x28: TEST, ALPHA, CLAMP, or another GS register */
} ItfGsStatePacket;

typedef char ItfGsStatePacket_size_must_be_0x30[
    (sizeof(ItfGsStatePacket) == 0x30) ? 1 : -1];

#endif /* DDS_ITF_GS_STATE_PACKET_H */
