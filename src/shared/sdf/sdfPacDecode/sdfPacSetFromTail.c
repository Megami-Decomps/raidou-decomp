#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacSetFromTail(sdfPacDecodeCb_t *cb)
{
    cb->flag |= SDF_PAC_STATE_FROMTAIL;
}
