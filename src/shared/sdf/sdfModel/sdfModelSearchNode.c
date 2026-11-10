#include "shared/sdf/sdfModel/decls.h"

sdfModelNode_t *sdfModelSearchNode(sdfModelRootNode_t *aRoot, s32 aId)
{
    sdfModelNodeList_t *list = aRoot->nodeList;
    s16 count;
    sdfModelNode_t **items;
    s32 i;

    count = list->count;
    items = list->items;
    if (aRoot->flags & SDF_MODEL_FLAG_COMPLEX_NODE_ID) {
        for (i = 0; i < count; i++) {
            sdfModelNode_t *node = items[i];

            if (node->id == aId) {
                return node;
            }
        }
        return 0;
    }
    if ((u32)aId >= (u32)count) {
        return 0;
    }
    return items[aId];
}
