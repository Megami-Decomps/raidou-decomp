#include "shared/sdf/sdfModel/decls.h"

void sdfModelNodeUnlink(sdfModelNode_t *aNode)
{
    sdfModelNode_t *parent = aNode->parent;

    if (parent == NULL) {
        sdfModelRootNode_t *root = aNode->root;

        if (root->headNode == aNode) {
            root->headNode = NULL;
        }
        return;
    }
    {
        sdfModelNode_t *next = aNode->next;
        sdfModelNode_t *prev = aNode->prev;

        if (next != aNode) {
            next->prev = prev;
            prev->next = next;
            if (parent->children == aNode) {
                parent->children = next;
            }
            aNode->prev = aNode;
            aNode->next = aNode;
            return;
        }
        if (parent->children == aNode) {
            parent->children = NULL;
        }
    }
}
