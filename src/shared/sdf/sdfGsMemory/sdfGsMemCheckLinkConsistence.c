#include "shared/sdf/sdfGsMemory/decls.h"

void sdfGsMemCheckLinkConsistence(void) {
    sdfGsMemSlice_t *node;

    node = gsMemHandleLinkHead;
    while (node->next != NULL) {
        node = node->next;
    }
    node = gsMemHandleLinkTail;
    while (node->prev != NULL) {
        node = node->prev;
    }
}
