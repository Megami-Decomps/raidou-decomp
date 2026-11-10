#ifndef DDS_SDF_TEXTURE_DRAW_PACKET_H

#define DDS_SDF_TEXTURE_DRAW_PACKET_H
#include "sdf.h"

#ifndef SDF_GS_CONTEXT_1

#define SDF_GS_CONTEXT_1 0

#endif

#ifndef SDF_GS_CONTEXT_2

#define SDF_GS_CONTEXT_2 1

#endif

/* Variable-sized packet header initialized by sdf2DMakePrimPacketHeader. */
typedef struct SdfDrawPacket {
    u16 quadwords;
    u8 pad02[6];
    u32 pad08;
    u32 command;
    u64 gifTag;
    u64 payloadHeader;
} SdfDrawPacket;

/* Fixed-size texture packet; constructor allocation and writes cover 0x50. */
typedef struct SdfTextureDrawPacket {
    SdfDrawPacket header;
    u64 textureWordA;
    u64 registerAddressA;
    u64 textureWordB;
    u64 registerAddressB;
    u64 textureWordC;
    u64 registerAddressC;
} SdfTextureDrawPacket;

typedef char SdfDrawPacket_size_must_be_0x20[
    (sizeof(SdfDrawPacket) == 0x20) ? 1 : -1];

typedef char SdfTextureDrawPacket_size_must_be_0x50[
    (sizeof(SdfTextureDrawPacket) == 0x50) ? 1 : -1];

#endif
