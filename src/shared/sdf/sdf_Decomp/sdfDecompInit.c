#include "shared/sdf/sdf_Decomp/decls.h"

void sdfDecompInit(sdfDecompCb_t *cb, u8 *dst) {
    cb->dstPtr = dst;
    cb->stat = 1;
}
