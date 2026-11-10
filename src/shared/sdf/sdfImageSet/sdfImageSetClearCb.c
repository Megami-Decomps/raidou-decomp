#include "shared/sdf/sdfImageSet/decls.h"

void sdfImageSetClearCb(sdfImageSetCb_t *cb)
{
    cb->packetHead = NULL;
    cb->packetTail = NULL;
    cb->tagHead = 0;
    cb->tagTail = NULL;
}
