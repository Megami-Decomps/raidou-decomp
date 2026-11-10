#include "shared/sdf/sdfIpu/decls.h"

/* Allocate two width-by-height frames, using four bytes per pixel for mode zero, two otherwise. */
void sdfIpuAllocBuff(sdfIpuDecodeCb_t *cb) {
    s32 bytesPerPixel = 4;
    s32 size;
    if (cb->rgbMode != SDF_STREAM_OUTPUT_RGBA32) {
        bytesPerPixel = 2;
    }
    size = cb->width * cb->height;
    size *= bytesPerPixel;
    cb->buffSize = size;
    cb->frameBuffers[0] = sdfAllocAuto(size);
    cb->frameBuffers[1] = sdfAllocAuto(size);
}
