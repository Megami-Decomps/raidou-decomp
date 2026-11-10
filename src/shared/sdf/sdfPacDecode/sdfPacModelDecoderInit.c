#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacModelDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr)
{
    sdfPacMemDecoderFuncInit(cb, hdr);
    cb->onEnd = sdfPacModelDecoderEndFunc;
}
