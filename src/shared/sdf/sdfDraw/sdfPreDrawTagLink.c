#include "shared/sdf/sdfDraw/decls.h"

void sdfPreDrawTagLink(sdfPreDrawTag_t *aTag, sdfPreDrawHook_t *aHook) {
    if (aTag->tail == 0) {
        aTag->head = aHook;
    }
    else {
        aTag->tail->next = aHook;
    }
    aTag->tail = aHook;
    aHook->next = NULL;
}
