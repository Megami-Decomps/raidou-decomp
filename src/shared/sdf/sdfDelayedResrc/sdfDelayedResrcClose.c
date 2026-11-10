#include "shared/sdf/sdfDelayedResrc/decls.h"

sdfDelayedResrcChunk_t *sdfDelayedResrcClose(void) {
    sdfDelayedResrcChunk_t *head = sdfDelayedResrcLink;
    sdfDelayedResrcChunk_t *node;

    sdfDelayedResrcLink = NULL;
    for (node = head; node != NULL; node = node->next) {
        node->owner->chunk = NULL;
    }
    return head;
}
