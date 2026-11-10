#ifndef DDS_SDF_DMA_TAG_H

#define DDS_SDF_DMA_TAG_H
#include "common.h"

#if !defined(SMT3_DEFINES_SdfDmaTagHeader)
/* DMA tag prefix followed by the two VIF words used by indexed-model writers.
 * The control byte stays whole, and address is a hardware word rather than a
 * CPU pointer. */
typedef struct SdfDmaTagHeader {
    u16 quadwordCount;
    u8 reservedByte;
    u8 control;
    u32 address;
    u32 firstVifCode;
    u32 secondVifCode;
} QWORD_ALIGNED SdfDmaTagHeader;
#endif

#if !defined(SMT3_DEFINES_SdfDmaTag)
/* Packet-list code views the same 16-byte row as QWC/ID/ADDR/VIF commands. */
typedef struct SdfDmaTag {
    u16 quadwordCount;
    u8 pad02;
    u8 kind;
    u32 address;
    u64 vifCommands;
} QWORD_ALIGNED SdfDmaTag;
#endif

typedef char SdfDmaTagHeader_size_must_be_0x10[
    (sizeof(SdfDmaTagHeader) == 0x10) ? 1 : -1];

typedef char SdfDmaTagHeader_alignment_must_be_4[
    (__alignof__(SdfDmaTagHeader) == 4) ? 1 : -1];

typedef char SdfDmaTag_size_must_be_0x10[
    (sizeof(SdfDmaTag) == 0x10) ? 1 : -1];

#endif /* DDS_SDF_DMA_TAG_H */
