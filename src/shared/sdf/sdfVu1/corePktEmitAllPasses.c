#include "shared/sdf/sdfVu1/decls.h"

void corePktEmitAllPasses(sdfCorePacketCb_t *cb) {
    u32 selectedFlags;

    corePktEmitBase(cb);
    selectedFlags = cb->attrBits;
    if ((selectedFlags & 0x1000) != 0) {
        corePktEmitMultiTexPass(cb);
        selectedFlags = cb->attrBits;
    }
    if ((selectedFlags & 1) != 0) {
        corePktEmitRefMapPass(cb);
        return;
    }
}
