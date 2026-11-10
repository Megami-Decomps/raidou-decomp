#include "shared/sdf/sdfModel/decls.h"

void sdfModelNodeAdd(sdfModelRootNode_t *aRoot, sdfModelNode_t *aParent, sdfModelNode_t *aNode)
{
    s16 count;
    sdfModelNode_t **items;
    sdfModelNodeList_t *list;
    s32 newCount;

    list = aRoot->nodeList;
    count = list->count;
    newCount = count + 1;
    if ((s16)list->capacity < newCount) {
        sdfItemArrayGrow(list);
        list = aRoot->nodeList;
    }
    items = list->items;
    aNode->root = aRoot;
    list->count = newCount;
    items[count] = aNode;
    sdfModelNodeSetParent(aNode, aParent);
}
