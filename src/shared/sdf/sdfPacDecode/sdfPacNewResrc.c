#include "shared/sdf/sdfPacDecode/decls.h"

sdfPacDecodeResrc_t *sdfPacNewResrc(sdfPacDecodeCb_t *cb, const sdfPacStdBlockHeader_t *hdr)
{
    s32 extra = hdr->flags & SDF_PAC_EXTRA_MASK;
    sdfPacDecodeResrc_t *aWork = sdfChipAllocZeroDirect(extra + sizeof(sdfPacDecodeResrc_t));

    aWork->cb = cb;
    memcpy(aWork->header, hdr, extra + sizeof(*hdr));
    if (cb->resrcTail == NULL) {
        cb->resrcHead = aWork;
    } else {
        cb->resrcTail->next = aWork;
    }
    cb->resrcTail = aWork;
    return aWork;
}
