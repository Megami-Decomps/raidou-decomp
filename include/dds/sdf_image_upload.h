#ifndef DDS_SDF_IMAGE_UPLOAD_H

#define DDS_SDF_IMAGE_UPLOAD_H
#include "common.h"

struct sdfMemHandle_t;

/* Policy for who retains pixel storage until the queued DMA completes. */
typedef u8 SdfImageUploadAllocationMode;

enum {
    /* Caller retains the source; the upload worker does not release it. */
    SDF_UPLOAD_BORROWED = 0,
    /* Upload queue owns these allocations and releases them after DMA. */
    SDF_UPLOAD_GENERAL_HEAP = 1,
    SDF_UPLOAD_CHIP_HEAP = 2
};

/* Pixel upload inputs shared by texture and font-atlas producers. */
typedef struct SdfImageUploadRequest {
    u8 *pixels;
    struct sdfMemHandle_t *allocation;
    SdfImageUploadAllocationMode allocationMode;
    u8 format;
    u16 bufferWidth;
    u32 destination;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} SdfImageUploadRequest;

#endif
