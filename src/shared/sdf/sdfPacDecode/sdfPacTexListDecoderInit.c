#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacTexListDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr)
{
    sdfPacNewResrc(cb, hdr);
    cb->slot.list = sdfChipAllocDirect(sizeof(sdfPacTexListDecodeCb_t));
    cb->decoder = sdfPacTexListDecoder0;
}
