#include "shared/sdf/sdfSortOt/decls.h"

void sdfGrSortOtSetup(sdfGrOt_t *aOt) {
    sdfDrawTag_t *head = NULL;
    sdfDrawTag_t *tail = NULL;

    sdfGrSortOtFixup(aOt->noSortTag, &head, &tail);
    if (aOt->data[0] != NULL) {
        sdfGrSortOtTraverse(aOt->data[0], &head, &tail);
    }
    aOt->tagHead = head;
    aOt->tagTail = tail;
}
