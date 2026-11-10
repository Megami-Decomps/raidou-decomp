#include "shared/sdf/sdfIpu/decls.h"

/* Clear the decoder, choose 32-bit/16-bit/dithered output and copy playback options. */
void sdfIpuInitCb(sdfIpuDecodeCb_t *cb, struct sdfIpuInitParm_t *parm) {
    memset(cb, 0, 0x8C);
    if (parm->rgbMode == 0) {
        cb->rgbMode = SDF_STREAM_OUTPUT_RGBA32;
    } else if (parm->ditherMode == 0) {
        cb->rgbMode = SDF_STREAM_OUTPUT_RGBA16;
    } else {
        cb->rgbMode = SDF_STREAM_OUTPUT_RGBA16_DITHERED;
    }
    cb->loopMode = parm->loopMode;
    cb->fps = parm->fps;
}
