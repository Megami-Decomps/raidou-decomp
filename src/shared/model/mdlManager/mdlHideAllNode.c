#include "shared/model/mdlManager/decls.h"

void mdlHideAllNode(mdlObj_t *aHandle)
{
    sdfModelNode_t **items;
    s32 count;
    s32 i;

    count = aHandle->inner->nodeList->count;
    items = aHandle->inner->nodeList->items;
    for (i = 0; i < count; i++) {
        items[i]->flags = 1;
    }
}
