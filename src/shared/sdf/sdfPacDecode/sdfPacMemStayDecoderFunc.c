#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacMemStayDecoderFunc(sdfPacDecodeCb_t *cb)
{
    s32 len = cb->rest;

    if (cb->srcLength < len) {
        len = cb->srcLength;
    }
    if (len != 0) {
        sdfPacUseSrc(cb, len);
        cb->rest -= len;
        if (cb->rest == 0) {
            cb->onEnd(cb);
        }
    }
}
