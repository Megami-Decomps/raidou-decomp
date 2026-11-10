#include "shared/interface/fmGslCont/decls.h"

/* Unlink the first live node after the sentinel, or NULL when its slot is empty. */
FontMem_t *fmRentFontMem(void) {
    FontMem_t *resourceNode = gFGC.root->next;

    if (resourceNode->id == 0) {
        return NULL;
    }
    resourceNode->prev->next = resourceNode->next;
    gFGC.unit_nums -= 1;
    resourceNode->next->prev = resourceNode->prev;
    return resourceNode;
}
