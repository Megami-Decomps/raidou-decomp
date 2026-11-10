#ifndef DDS_SND_COMMAND_PACKET_H

#define DDS_SND_COMMAND_PACKET_H
#include "common.h"

/* Sixteen-byte track-command payload shared by both titles. */
typedef struct CmdPacket {
    u32 trackId; /* 0x00 */
    u32 unk4;    /* 0x04 */
    u16 setting; /* 0x08 */
    u16 unkA;    /* 0x0A */
    u32 unkC;    /* 0x0C */
} CmdPacket;

typedef char CmdPacket_size_must_be_0x10[(sizeof(CmdPacket) == 0x10) ? 1 : -1];

#endif /* DDS_SND_COMMAND_PACKET_H */
