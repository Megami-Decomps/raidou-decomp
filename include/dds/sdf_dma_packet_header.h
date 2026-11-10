#ifndef DDS_SDF_DMA_PACKET_HEADER_H

#define DDS_SDF_DMA_PACKET_HEADER_H
#include "common.h"

#if !defined(SMT3_DEFINES_DmaPacketHeader)
/* Shared 0x20-byte DMA/VIF reference header. Unrecovered bytes stay opaque. */
typedef struct DmaPacketHeader {
    u16 quadwords; /* 0x00 */
    u8 pad02[2]; /* 0x02 */
    u32 address; /* 0x04 */
    u32 tag; /* 0x08 */
    u32 command; /* 0x0C */
    u16 unknown10; /* 0x10 */
    u8 opaque12[6]; /* 0x12 */
    u32 unknown18; /* 0x18 */
    u32 unknown1C; /* 0x1C */
} QWORD_ALIGNED DmaPacketHeader;
#endif

typedef char DmaPacketHeader_size_must_be_0x20[
    (sizeof(DmaPacketHeader) == 0x20) ? 1 : -1];

#endif /* DDS_SDF_DMA_PACKET_HEADER_H */
