#include "shared/sdf/sdfModel/decls.h"

void sdfModelNodeSetParent(sdfModelNode_t *aNode, sdfModelNode_t *aParent)
{
    if (aParent == NULL) {
        sdfModelRootNode_t *root = aNode->root;
        sdfModelNode_t *head = root->headNode;

        if (head != aNode) {
            root->headNode = aNode;
            if (head != NULL) {
                aNode->children = head;
                head->parent = aNode;
            }
            aNode->parent = NULL;
        }
    } else if (aNode->parent != aParent) {
        sdfModelNode_t *head;

        sdfModelNodeUnlink(aNode);
        head = aParent->children;
        if (head == NULL) {
            aParent->children = aNode;
        } else {
            sdfModelNode_t *prev = head->prev;

            aNode->prev = prev;
            prev->next = aNode;
            head->prev = aNode;
            aNode->next = head;
        }
        aNode->parent = aParent;
    }
}
