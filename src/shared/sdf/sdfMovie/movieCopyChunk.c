#include "shared/sdf/sdfMovie/decls.h"

s32 movieCopyChunk(sdfIpuDecodeCb_t *aNode, sdfMovieCb_t *aMov, s32 aOp, void *aBuf, s32 aSize)
{
    sdfMovieStrmLinear_t *stream = aMov->readCb.linear;
    u8 *src;
    u8 *base;
    s32 over;
    s32 first;

    switch (aOp) {
    case 0:
        if (aMov->cancel != 0) {
            *(u8 *)aBuf = 1;
            return 0;
        }
        if (aMov->readRemain == 0) {
            *(u8 *)aBuf = 1;
        }
        return stream->bufferedBytes;
    case 1:
        if (aMov->cancel != 0) {
            return 0;
        }
        src = stream->readCursor;
        base = stream->bufferStart;
        over = (src - base) + aSize - 0x20000;
        if (over >= 0) {
            first = aSize - over;
            memcpy(aBuf, src, first);
            if (over > 0) {
                memcpy((u8 *)aBuf + first, base, over);
            }
            stream->readCursor = base + over;
        } else {
            memcpy(aBuf, src, aSize);
            stream->readCursor = src + aSize;
        }
        stream->bufferedBytes -= aSize;
        return aMov->readRemain;
    case 2:
        if (aMov->cancel != 0) {
            return 0;
        }
        if (aMov->readStat == 5) {
            movieReadNextChunk(aMov);
        }
        return 0;
    }
    return 0;
}
