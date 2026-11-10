#ifndef DDS_SDF_GS_PACKET_H

#define DDS_SDF_GS_PACKET_H
#include "../draw.h"
#include "common.h"

/* Packed ALPHA operands: (A - B) * C / 128 + D. Cs/Cd denote
 * source/destination colors, and As/Ad their alpha values. */
enum {
    SDF_GS_ALPHA_INTERPOLATE_SOURCE_ALPHA = 0x44,      /* (Cs - Cd) * As / 128 + Cd */
    SDF_GS_ALPHA_ADD_SOURCE_ALPHA = 0x48,              /* Cs * As / 128 + Cd */
    SDF_GS_ALPHA_SUBTRACT_SOURCE_ALPHA = 0x42,         /* Cd - Cs * As / 128 */
    SDF_GS_ALPHA_INTERPOLATE_DESTINATION_ALPHA = 0x54, /* (Cs - Cd) * Ad / 128 + Cd */
    SDF_GS_ALPHA_ADD_DESTINATION_ALPHA = 0x58,         /* Cs * Ad / 128 + Cd */
    SDF_GS_ALPHA_SUBTRACT_DESTINATION_ALPHA = 0x52     /* Cd - Cs * Ad / 128 */
};

/* Appending the packet patches the DMA tag's link; the VIF and GIF words
 * retain the renderer's original commands and register-list encoding. */
typedef struct SdfGsPacketHeader {
    u64 dmaTag;
    u64 vifCommands;
    u64 gifTag;
    u64 gifRegisters;
} QWORD_ALIGNED SdfGsPacketHeader;

typedef struct SdfGsRegisterWrite {
    u64 value;
    u64 registerId;
} SdfGsRegisterWrite;

/* A single A+D write follows the texture-flush DMA/VIF/GIF header. */
typedef struct SdfGsTextureFlushPacket {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite textureFlush;
} SdfGsTextureFlushPacket;

typedef struct SdfGsTexturePacket {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite sampling;
    SdfGsRegisterWrite texture;
    SdfGsRegisterWrite clamp;
} SdfGsTexturePacket;

typedef char SdfGsPacketHeader_size_must_be_0x20[
    (sizeof(SdfGsPacketHeader) == 0x20) ? 1 : -1];

typedef char SdfGsRegisterWrite_size_must_be_0x10[
    (sizeof(SdfGsRegisterWrite) == 0x10) ? 1 : -1];

typedef char SdfGsTexturePacket_size_must_be_0x50[
    (sizeof(SdfGsTexturePacket) == 0x50) ? 1 : -1];

typedef char SdfGsTextureFlushPacket_size_must_be_0x30[
    (sizeof(SdfGsTextureFlushPacket) == 0x30) ? 1 : -1];

#endif /* DDS_SDF_GS_PACKET_H */
