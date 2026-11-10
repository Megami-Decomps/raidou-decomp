#ifndef DDS_DDS3_FRAME_DRAW_BUFFER_H

#define DDS_DDS3_FRAME_DRAW_BUFFER_H
#include "common.h"

#define DDS3_FRAME_DRAW_BASE_BYTES 0x40

#define DDS3_FRAME_DRAW_BUFFER_STRIDE 0x1F40

/* Shared frame draw-buffer stride. Packet contents beyond the base view stay opaque. */
typedef struct dds3FrameDrawBuffer_t {
    u8 baseBytes[DDS3_FRAME_DRAW_BASE_BYTES];
    u8 unknown40[DDS3_FRAME_DRAW_BUFFER_STRIDE - DDS3_FRAME_DRAW_BASE_BYTES];
} dds3FrameDrawBuffer_t;

typedef char dds3FrameDrawBuffer_size_must_be_0x1F40[
    (sizeof(dds3FrameDrawBuffer_t) == DDS3_FRAME_DRAW_BUFFER_STRIDE) ? 1 : -1];

#endif /* DDS_DDS3_FRAME_DRAW_BUFFER_H */
