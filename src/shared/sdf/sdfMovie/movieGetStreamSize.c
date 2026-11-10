#include "shared/sdf/sdfMovie/decls.h"

s32 movieGetStreamSize(void *aHandle, s32 aOp, void *aBuf, s32 aSize, sdfMovieCb_t *aMov)
{
    sdfMovieStrmLinear_t *stream = aMov->readCb.linear;
    s32 enabled;

    aMov->fileHandle = aHandle;
    if (aMov->cancel != 0) {
        if (aMov->readStat != 6 && aMov->readStat != 7) {
            aMov->readStat = 7;
            sdfFileCmdClose(aHandle);
            return 0;
        }
    }
    switch (aMov->readStat) {
    case 0:
        if (aOp == 2) {
            aMov->readStat = 1;
            sdfFileCmdFileSize(aHandle);
        }
        break;
    case 1:
        if (aOp == 4) {
            aMov->fileSize = aSize;
            aMov->readRemain = aSize;
            movieReadNextChunk(aMov);
        }
        break;
    case 4:
        if (aOp == 5) {
            enabled = DIntr();
            stream->bufferedBytes += aSize;
            aMov->readRemain -= aSize;
            if (enabled != 0) {
                EIntr();
            }
            if (aMov->readRemain == 0) {
                aMov->readStat = 7;
                sdfFileCmdClose(aHandle);
            } else {
                movieReadNextChunk(aMov);
            }
        }
        break;
    case 7:
        if (aOp == 7) {
            aMov->fileHandle = NULL;
            aMov->readStat = 6;
            sdfFileCmdFree(aHandle);
        }
        break;
    }
    return 0;
}
