#include "shared/sdf/sdfDraw/decls.h"

sdfDrawTag_t *sdfGrOtClose(sdfGrOt_t *aOt) {
    sdfDrawTag_t *tail = NULL;
    sdfDrawTag_t *head = NULL;

    for (; aOt != NULL; aOt = aOt->next) {
        aOt->callbackFunc(aOt, 0, NULL);
        if (aOt->tagHead != 0) {
            if (head != 0) {
                sdfGrJoinDrawTag(tail, aOt->tagHead);
            } else {
                head = aOt->tagHead;
                sdfGrInitExPacket(head);
            }
            tail = aOt->tagTail;
        }
    }
    if (tail != NULL) {
        (EE_ADDR_AS(SdfDmaTag *, tail->tail))->kind = SDF_DMA_TAG_END_BYTE;
        (EE_ADDR_AS(SdfDmaTag *, tail->tail))->address = 0;
    }
    return head;
}
