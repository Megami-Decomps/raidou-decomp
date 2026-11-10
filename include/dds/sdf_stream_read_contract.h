#ifndef DDS_SDF_STREAM_READ_CONTRACT_H

#define DDS_SDF_STREAM_READ_CONTRACT_H
#include "common.h"

/* QUERY returns buffered bytes and writes an EOF byte to dataBuffer; COPY reads
 * byteCount bytes and returns movie->remainingBytes; RESUME may queue another
 * read when the consumer has room and returns zero. Results are operation-specific. */
typedef s32 SdfStreamReadResult;

typedef s32 SdfStreamReadOperation;

struct sdfIpuDecodeCb_t;

struct movieStream_t;

typedef SdfStreamReadResult (*SdfStreamReadCallback)(
    struct sdfIpuDecodeCb_t *node, struct movieStream_t *context,
    SdfStreamReadOperation operation, void *dataBuffer, s32 byteCount);

#endif /* DDS_SDF_STREAM_READ_CONTRACT_H */
