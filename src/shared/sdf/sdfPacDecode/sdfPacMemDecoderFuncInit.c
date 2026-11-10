#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacMemDecoderFuncInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr)
{
    s32 size = hdr->realSize;
    sdfPacDecodeResrc_t *aWork;

    if (size == 0) {
        size = hdr->size + (hdr->flags & SDF_PAC_EXTRA_MASK) - sizeof(*hdr);
    }
    if (cb->flag & SDF_PAC_STATE_STAY) {
        aWork = sdfPacNewResrc(cb, hdr);
        aWork->data = (u8 *)(hdr + 1);
        cb->decoder = sdfPacMemStayDecoderFunc;
        return;
    }
    cb->stat = 2;
    aWork = sdfPacNewResrc(cb, hdr);
    if (cb->flag & SDF_PAC_STATE_FROMTAIL) {
        aWork->resrc = sdfMemAllocFromTailDirect(size);
    } else {
        aWork->resrc = sdfMemAllocDirect(size);
    }
    aWork->data = sdfMemLock(aWork->resrc);
    cb->dst = aWork->data;
    switch (hdr->flags & SDF_PAC_ENCODING_MASK) {
    case 0:
        cb->decoder = sdfPacMemDecoderFunc;
        break;
    case 1: {
        struct sdfDecomp_t *decomp = sdfChipAllocDirect(sizeof(*decomp));

        cb->decomp = decomp;
        sdfDecompInit(decomp, cb->dst);
        cb->decoder = sdfPacMemDecompDecoderFunc;
        break;
    }
    }
}
