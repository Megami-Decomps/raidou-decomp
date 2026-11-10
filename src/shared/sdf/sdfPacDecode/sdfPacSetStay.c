#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacSetStay(sdfPacDecodeCb_t *cb)
{
    cb->flag |= SDF_PAC_STATE_STAY;
}
