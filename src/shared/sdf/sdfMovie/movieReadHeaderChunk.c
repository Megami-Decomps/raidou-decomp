#include "shared/sdf/sdfMovie/decls.h"

void movieReadHeaderChunk(sdfMovieCb_t *aMov)
{
    s32 remaining = aMov->readRemain;
    sdfMovieStrmPac_t *stream = aMov->readCb.pac;
    s32 size;

    if (remaining != 0) {
        if (0x10000 - stream->pacBufferedBytes < 0x4000) {
            aMov->readStat = 5;
        } else if (0x10000 + (stream->unk6C - stream->ringLength) < 0x4000) {
            aMov->readStat = 5;
        } else {
            aMov->readStat = 4;
            size = remaining < 0x4001 ? remaining : 0x4000;
            sdfFileCmdRead(aMov->fileHandle, stream->pacInputBuffer, size);
        }
    }
}
