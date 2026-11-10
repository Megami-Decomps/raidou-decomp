#include "shared/sdf/sdfDraw/decls.h"

void sdfGrOtNormalReg(sdfGrOt_t *aOt, sdfDrawTag_t *aTag) {
    sdfDrawTag_t *last;

    if (aTag->head != 0) {
        last = aOt->tagTail;
        if (last == NULL) {
            aOt->tagHead = aTag;
        }
        else {
            last->next = aTag;
            sdfGrJoinDrawTag(last, aTag);
        }
        aOt->tagTail = aTag;
    }
}
