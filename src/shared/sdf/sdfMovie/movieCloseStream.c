#include "shared/sdf/sdfMovie/decls.h"

void movieCloseStream(sdfMovieCb_t *aMov)
{
    u8 state;
    sdfMovieStrmLinear_t *stream;
    sdfMovieStrmPac_t *pac;

    if (aMov->stat == 0) {
        return;
    }
    state = aMov->readStat;
    aMov->cancel = 1;
    if (state == 5) {
        aMov->readStat = 7;
        sdfFileCmdClose(aMov->fileHandle);
        state = aMov->readStat;
    }
    if (state != 6) {
        do {
            sdfDummyFunc();
        } while (aMov->readStat != 6);
    }
    if (aMov->mode == 0) {
        stream = aMov->readCb.linear;
        sdfMemFreeDirect(stream->allocation);
        sdfChipFreeDirect(stream);
    } else {
        pac = aMov->readCb.pac;
        sdfMemFreeDirect(pac->payloadAllocation);
        sdfMemFreeDirect(pac->allocation);
        sdfChipFreeDirect(pac);
    }
    sdfIpuFree(&aMov->decodeCb);
    if (aMov->sndActive != 0) {
        p2sdSyncMovie();
    }
    aMov->stat = 0;
}
