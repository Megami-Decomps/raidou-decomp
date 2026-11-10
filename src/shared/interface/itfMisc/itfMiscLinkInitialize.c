#include "shared/interface/itfMisc/decls.h"

/* Initialize a FIFO free list and empty active list.
 * Native do/while writes at least two nodes; the s8 index can wrap. */
void itfMiscLinkInitialize(itfmisclinkstack_t *pool, itfLink_t *nodes, s32 count, s32 stride) {
    s8 index = 0;
    itfLink_t *previous = NULL;
    itfLink_t *node = nodes;
    itfLink_t *next;

    do {
        count--;
        next = (itfLink_t *)((u8 *)node + stride);
        node->previous = previous;
        node->index = index;
        index++;
        node->next = next;
        previous = node;
        node = next;
    } while (count >= 2);
    node->previous = previous;
    node->index = index;
    node->next = NULL;
    pool->pdisuse_tail = node;
    pool->pdisuse_top = nodes;
    pool->puse_tail = NULL;
    pool->puse_top = NULL;
}
