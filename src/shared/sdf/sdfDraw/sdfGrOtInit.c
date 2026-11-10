#include "shared/sdf/sdfDraw/decls.h"

void sdfGrOtInit(sdfGrOt_t *aOt, s32 aNumOt) {
    sdfGrOt_t *node = aOt;
    sdfGrOt_t *next;
    s32 remaining = aNumOt - 1;

    node->regFunc = sdfGrOtNormalReg;
    node->callbackFunc = sdfGrOtNormalCallback;
    node->tagHead = 0;
    node->tagTail = 0;
    node->noSortTag = 0;
    node->data = 0;
    if (remaining == 0) {
        node->next = NULL;
        return;
    }
link:
    next = node + 1;
    remaining--;
    node->next = next;
    node = next;
    next->regFunc = sdfGrOtNormalReg;
    next->callbackFunc = sdfGrOtNormalCallback;
    next->tagHead = 0;
    next->tagTail = 0;
    next->noSortTag = 0;
    next->data = 0;
    if (remaining != 0) {
        goto link;
    }
    node->next = NULL;
}
