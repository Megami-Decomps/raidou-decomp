#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacTmxDecoderEndFunc(sdfPacDecodeCb_t *cb)
{
    cb->resrcTail->resrc = cb->slot.decodeCb->texHandle;
    sdfChipFreeDirect(cb->slot.decodeCb);
    sdfPacEndBlock(cb);
}
