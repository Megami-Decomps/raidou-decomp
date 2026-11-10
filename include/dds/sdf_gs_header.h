#ifndef DDS_SDF_GS_HEADER_H

#define DDS_SDF_GS_HEADER_H
#include "sdf_gs_packet.h"

/* GIF tag followed by a REF transfer and the patchable NEXT link. */
typedef struct SdfDmaReferenceChainPacket {
    SdfGsPacketHeader header;
    u64 referenceTag;
    u64 referenceVifCommands;
    u64 nextTag;
    u64 zeroTailWord;
} SdfDmaReferenceChainPacket;

typedef char SdfDmaReferenceChainPacket_size_must_be_0x40[
    (sizeof(SdfDmaReferenceChainPacket) == 0x40) ? 1 : -1];

#endif
