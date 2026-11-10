#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacMemDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr)
{
    sdfPacMemDecoderFuncInit(cb, hdr);
    cb->onEnd = sdfPacEndBlock;
}
