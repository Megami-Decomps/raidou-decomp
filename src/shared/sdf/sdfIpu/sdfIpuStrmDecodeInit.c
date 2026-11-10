#include "shared/sdf/sdfIpu/decls.h"

/* Store the callback and movie context, then allocate the feed ring and leading mirror bytes. */
void sdfIpuStrmDecodeInit(sdfIpuDecodeCb_t *node, struct sdfIpuInitParm_t *format, SdfStreamRead readSource, struct sdfMovieCb_t *readContext) {
    sdfIpuInitCb(node, format);
    node->read = readSource;
    node->strmCallbackParm = readContext;
    node->active = 1;
    node->scratchBuffer = (u8 *)sdfAllocAuto(0x10100) + 0x100;
}
