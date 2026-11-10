#include "shared/effect/effPCPMisc/decls.h"

void effPCPMakkaUpdateNode(sdfModelNode_t *node) {
    sdfModelNode_t *child;

    SDF_VU0_STOREVECTOR(node->scale);
    child = node->children;
    if (child != NULL) {
        do {
            effPCPMakkaUpdateNode(child);
            child = child->next;
        } while (child != node->children);
    }
}
