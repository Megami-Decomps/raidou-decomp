#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacTmxStayDecoderFunc(sdfPacDecodeCb_t *cb)
{
    sdfPacTmxDecodeCb_t *buf = cb->decodeCb;
    s32 len = buf->rest;

    if (cb->srcLength < len) {
        len = cb->srcLength;
    }
    if (len != 0) {
        sdfPacUseSrc(cb, len);
        buf->rest -= len;
        if (buf->rest == 0) {
            buf->texHandle = sdfTexSetDirect(buf->workPtr);
            cb->onEnd(cb);
        }
    }
}
