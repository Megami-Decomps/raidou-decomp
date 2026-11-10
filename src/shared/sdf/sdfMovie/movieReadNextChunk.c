#include "shared/sdf/sdfMovie/decls.h"

void movieReadNextChunk(sdfMovieCb_t *aMov)
{
    s32 remaining = aMov->readRemain;
    sdfMovieStrmLinear_t *stream = aMov->readCb.linear;
    s32 size;
    u8 *dst;
    u8 *next;

    if (remaining != 0) {
        if (0x20000 - stream->bufferedBytes < 0x4000) {
            aMov->readStat = 5;
        } else {
            aMov->readStat = 4;
            size = remaining < 0x4001 ? remaining : 0x4000;
            dst = stream->writeCursor;
            next = dst + size;
            if (next >= stream->bufferStart + 0x20000) {
                next = stream->bufferStart;
            }
            stream->writeCursor = next;
            sdfFileCmdRead(aMov->fileHandle, dst, size);
        }
    }
}
