#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacTmxDecompDecoderFunc(sdfPacDecodeCb_t *cb)
{
    s32 len = cb->rest;
    s32 done = sdfDecomp(cb->decomp, cb->srcData, len);

    sdfPacUseSrc(cb, len - cb->decomp->remain);
    if (done != 0) {
        sdfPacTmxDecodeCb_t *buf = cb->decodeCb;

        buf->texHandle = sdfTexSet(sdfMemLock(buf->memHandle));
        sdfMemFreeHandleDirect(&buf->memHandle);
        sdfChipFreeDirect(cb->decomp);
        cb->onEnd(cb);
    }
}
