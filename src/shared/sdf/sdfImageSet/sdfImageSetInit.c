#include "shared/sdf/sdfImageSet/decls.h"

void sdfImageSetInit(void)
{
    sdfImageSetCb_t *cb;

    cb = &sdfImageSetCb;
    cb->semaId = sdfCreateSema(1, 0x7F, 0);
    sdfImageSetClearCb(cb);
}
