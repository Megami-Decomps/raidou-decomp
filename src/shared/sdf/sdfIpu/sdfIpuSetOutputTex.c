#include "shared/sdf/sdfIpu/decls.h"

void sdfIpuSetOutputTex(sdfIpuDecodeCb_t *node, struct sdfIpuInitParm_t *params, u8 *sourceData, s32 sourceSize, sdfTexHandle_t *source) {
    struct sdfIpuInitParm_t streamParams = *params;
    switch (source->pixelFormat) {
    case 0:
        streamParams.rgbMode = 0;
        break;
    case 2:
        streamParams.rgbMode = 1;
        break;
    }
    sdfIpuStartDecodeTask(node, &streamParams, (SdfStreamRead)sourceData, EE_ADDR_AS(struct sdfMovieCb_t *, sourceSize), sdfTexGetPixelAddr(source));
}
