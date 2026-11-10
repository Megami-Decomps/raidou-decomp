#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacTexListDecoder0(sdfPacDecodeCb_t *cb)
{
    u32 count;
    sdfPacTexListDecodeCb_t *list;
    sdfPacDecodeResrc_t *aWork;

    if (cb->srcLength != 0) {
        count = *(const u32 *)cb->srcData;
        sdfPacUseSrc(cb, 4);
        list = cb->slot.list;
        aWork = cb->resrcTail;
        list->count = count;
        list->currNum = 0;
        aWork->resrc = sdfTexListNew(count);
        cb->rest = ((count * 4 + 0x53) & -0x40) - 0x14;
        cb->decoder = sdfPacTexListDecoder1;
    }
}
