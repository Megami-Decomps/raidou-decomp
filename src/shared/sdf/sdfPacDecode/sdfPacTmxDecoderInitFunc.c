#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacTmxDecoderInitFunc(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr,
                             sdfPacTmxDecodeCb_t *tmxCb)
{
    cb->decodeCb = tmxCb;
    tmxCb->rest = hdr->size - 0x10;
    if (cb->flag & SDF_PAC_STATE_STAY) {
        cb->decoder = sdfPacTmxStayDecoderFunc;
        tmxCb->workPtr = (u8 *)(cb->srcData - 0x10);
        return;
    }
    if (cb->flag & SDF_PAC_STATE_FROMTAIL) {
        tmxCb->memHandle = sdfMemAllocFromTailDirect(hdr->size);
    } else {
        tmxCb->memHandle = sdfMemAllocDirect(hdr->size);
    }
    tmxCb->workPtr = sdfMemLock(tmxCb->memHandle);
    memcpy(tmxCb->workPtr, hdr, 0x10);
    tmxCb->workPtr += 0x10;
    switch (hdr->flags) {
    case 0:
        cb->decoder = sdfPacTmxDecoderFunc;
        break;
    case 1: {
        struct sdfDecomp_t *decomp = sdfChipAllocDirect(sizeof(*decomp));

        cb->decompBuffer = (u8 *)decomp;
        sdfDecompInit(decomp, tmxCb->workPtr);
        cb->decoder = sdfPacTmxDecompDecoderFunc;
        break;
    }
    }
}
