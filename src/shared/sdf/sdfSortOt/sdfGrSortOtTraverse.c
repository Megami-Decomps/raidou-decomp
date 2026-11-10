#include "shared/sdf/sdfSortOt/decls.h"

void sdfGrSortOtTraverse(sdfSortOtNode_tag *node, sdfDrawTag_t **head_p, sdfDrawTag_t **tail_p) {
    sdfSortOtNode_tag *next;

    do {
        if (node->right != NULL) {
            sdfGrSortOtTraverse(node->right, head_p, tail_p);
        }
        sdfGrSortOtFixup(node->tag, head_p, tail_p);
        next = node->left;
        node = next;
    } while (next != NULL);
}
