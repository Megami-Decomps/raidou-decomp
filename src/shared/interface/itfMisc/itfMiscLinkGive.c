#include "shared/interface/itfMisc/decls.h"

/* Unlink an active node and append it to the free-list tail; membership is trusted. */
void itfMiscLinkGive(itfLink_t *pelem, itfmisclinkstack_t *pstack) {
    itfLink_t *previous = pelem->previous;
    itfLink_t *next = pelem->next;

    if (previous != NULL) {
        previous->next = next;
    } else {
        pstack->puse_top = next;
    }
    if (next != NULL) {
        next->previous = previous;
    } else {
        pstack->puse_tail = previous;
    }
    pelem->next = NULL;
    {
        itfLink_t *lastFree = pstack->pdisuse_tail;
        pelem->previous = lastFree;
        if (lastFree != NULL) {
            lastFree->next = pelem;
        }
    }
    pstack->pdisuse_tail = pelem;
    if (pstack->pdisuse_top == NULL) {
        pstack->pdisuse_top = pelem;
    }
}
