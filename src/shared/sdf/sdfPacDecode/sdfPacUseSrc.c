#include "shared/sdf/sdfPacDecode/decls.h"

void sdfPacUseSrc(sdfPacDecodeCb_t *cb, s32 size)
{
    cb->srcData += size;
    cb->srcLength -= size;
    cb->srcTotal += size;
}
