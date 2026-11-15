#ifndef SDF3D_H
#define SDF3D_H

#include "common.h"
#include "sdf_resource.h"

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

void sdf3DInit(s32 aNestLevelLimit);
void sdf3DLinkInitPacket(sdfDrawTag_t *aTag, DmaPacketHeader *aPacket);
void sdf3DMakeDirectRefPacket(DmaPacketHeader *aPacket, addr_t aData, s32 aLeng);

#endif
