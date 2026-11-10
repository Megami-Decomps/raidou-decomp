#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacMotionDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr)
{
    sdfPacMemDecoderFuncInit(cb, hdr);
    cb->onEnd = sdfPacMotionDecoderEndFunc;
}
