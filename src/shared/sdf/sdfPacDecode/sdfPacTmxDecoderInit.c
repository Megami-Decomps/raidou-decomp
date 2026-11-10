#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacTmxDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr)
{
    sdfPacTmxDecodeCb_t *buf;

    sdfPacNewResrc(cb, hdr);
    buf = sdfChipAllocDirect(sizeof(*buf));
    cb->slot.decodeCb = buf;
    sdfPacTmxDecoderInitFunc(cb, hdr, buf);
    cb->onEnd = sdfPacTmxDecoderEndFunc;
}
