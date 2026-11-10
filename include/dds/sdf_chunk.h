#ifndef DDS_SDF_CHUNK_H

#define DDS_SDF_CHUNK_H
#include "common.h"

struct dds_sdfModelRootNode_t;

/* Variable-size serialized chunk streams share only this eight-byte header.
 * The word at +8 belongs to each chunk's payload. */
typedef struct SdfChunkHeader {
    u32 id;   /* 0x00: zero terminates the chunk stream */
    u32 size; /* 0x04: byte extent to the next chunk */
} SdfChunkHeader;

typedef char SdfChunkHeader_size_must_be_8[(sizeof(SdfChunkHeader) == 8) ? 1 : -1];

/* MPOS has an eight-byte opaque prefix after the common header, then 0x40-byte
 * rows. Its enclosing chunk extent remains variable and is stored in size. */
typedef struct SdfMapPositionChunkPrefix {
    SdfChunkHeader header;
    u8 payload08[8];
} SdfMapPositionChunkPrefix;

typedef struct SdfMapPositionRecord {
    u32 nodeId;      /* 0x00: draw-node lookup key */
    s32 id;          /* 0x04: map-position record lookup key */
    u8 reserved08[8];
    u128 position;   /* 0x10 */
    u128 up;         /* 0x20 */
    u128 direction;  /* 0x30 */
} SdfMapPositionRecord;

typedef char SdfMapPositionChunkPrefix_size_must_be_0x10[
    (sizeof(SdfMapPositionChunkPrefix) == 0x10) ? 1 : -1];

typedef char SdfMapPositionRecord_size_must_be_0x40[
    (sizeof(SdfMapPositionRecord) == 0x40) ? 1 : -1];

#endif /* DDS_SDF_CHUNK_H */
