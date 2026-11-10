#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacMemDecoderFunc(sdfPacDecodeCb_t *cb)
{
    s32 len = cb->rest;

    if (cb->srcLength < len) {
        len = cb->srcLength;
    }
    if (len != 0) {
        memcpy(cb->dst, cb->srcData, len);
        sdfPacUseSrc(cb, len);
        cb->dst += len;
        cb->rest -= len;
        if (cb->rest == 0) {
            cb->onEnd(cb);
        }
    }
}
