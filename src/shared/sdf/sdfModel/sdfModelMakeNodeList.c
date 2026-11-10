#include "shared/sdf/sdfModel/decls.h"

void sdfModelMakeNodeList(sdfModelRootNode_t *aRoot) {
    sdfModelNode_t **nodeArray = aRoot->nodeList->items;
    sdfModelNode_t *currentNode = aRoot->headNode;
    sdfModelNode_t **output;

    if (currentNode != NULL) {
        output = nodeArray;
        do {
            *output++ = currentNode;
            if (currentNode->children != NULL) {
                currentNode = currentNode->children;
            } else {
                do {
                    sdfModelNode_t *nextSibling = currentNode->next;
                    if (nextSibling != NULL) {
                        currentNode = nextSibling;
                        break;
                    }
                    currentNode = currentNode->parent;
                } while (currentNode != NULL);
            }
        } while (currentNode != NULL);
    }
}
