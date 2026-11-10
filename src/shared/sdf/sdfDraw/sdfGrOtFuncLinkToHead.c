#include "shared/sdf/sdfDraw/decls.h"

void sdfGrOtFuncLinkToHead(sdfGrOt_t *aOt, sdfDrawTag_t *aTag) {
    sdfDrawTag_t *firstList;

    if (aTag->tail == 0) {
        return;
    }
    firstList = aOt->tagHead;
    if (firstList == NULL) {
        aOt->tagTail = aTag;
    } else {
        sdfGrJoinDrawTag(aTag, firstList);
    }
    aTag->next = firstList;
    aOt->tagHead = aTag;
}
