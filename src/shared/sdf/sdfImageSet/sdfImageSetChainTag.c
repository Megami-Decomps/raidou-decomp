#include "shared/sdf/sdfImageSet/decls.h"

void sdfImageSetChainTag(u32 head, sdfImageSetTag_t *tail)
{
    sdfImageSetCb_t *cb = &sdfImageSetCb;
    sdfImageSetTag_t *last;

    WaitSema(cb->semaId);
    last = cb->tagTail;
    if (last != NULL) {
        last->vifCodes = 0;
        last->tag = SDF_DMA_TAG(head, 0x20000000);
    } else {
        cb->tagHead = head;
    }
    cb->tagTail = tail;
    SignalSema(cb->semaId);
}
