#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacTexListDecoderTmxHeaderEndFunc(sdfPacDecodeCb_t *cb)
{
    sdfPacTmxDecoderInitFunc(cb, &cb->slot.list->currHeader, &cb->slot.list->tmxCb);
    cb->onEnd = sdfPacTexListDecoderTmxBodyEndFunc;
}
