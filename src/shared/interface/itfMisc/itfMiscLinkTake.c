#include "shared/interface/itfMisc/decls.h"

/* Move the free-list head to the active-list tail; exhausted pools return NULL. */
itfLink_t *itfMiscLinkTake(itfmisclinkstack_t *pstack) {
    itfLink_t *node = pstack->pdisuse_top;
    itfLink_t *next;

    if (node == NULL) {
        return NULL;
    }
    next = node->next;
    if (pstack->puse_top != NULL) {
        node->previous = pstack->puse_tail;
        pstack->puse_tail->next = node;
    } else {
        node->previous = NULL;
        pstack->puse_top = node;
    }
    node->next = NULL;
    pstack->puse_tail = node;
    if (next != NULL) {
        next->previous = NULL;
    } else {
        pstack->pdisuse_tail = NULL;
    }
    pstack->pdisuse_top = next;
    return node;
}
