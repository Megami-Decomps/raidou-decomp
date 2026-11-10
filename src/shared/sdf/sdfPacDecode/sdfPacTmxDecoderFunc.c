#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacTmxDecoderFunc(sdfPacDecodeCb_t *cb)
{
    sdfPacTmxDecodeCb_t *buf = cb->decodeCb;
    s32 len = buf->rest;

    if (cb->srcLength < len) {
        len = cb->srcLength;
    }
    if (len != 0) {
        memcpy(buf->workPtr, cb->srcData, len);
        sdfPacUseSrc(cb, len);
        buf->workPtr += len;
        buf->rest -= len;
        if (buf->rest == 0) {
            buf->texHandle = sdfTexSet(sdfMemLock(buf->memHandle));
            sdfMemFreeHandleDirect(&buf->memHandle);
            cb->onEnd(cb);
        }
    }
}
