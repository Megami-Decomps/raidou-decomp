#include "shared/sdf/sdfVu1/decls.h"

void corePktEmitBase(sdfCorePacketCb_t *cb) {
    if ((cb->attrBits & SDF_VU_TEXTURED_FLAG) != 0) {
        corePktEmitTexTris(cb);
        return;
    }
    corePktEmitTris(cb);
}
