#include "shared/sdf/sdfModel/decls.h"

sdfModelRootNode_t *sdfModelMakeRoot(void *aTexList, sdfModelDef_t *aModelData) {
    s32 itemIndex = 0;
    sdfModelRootNode_t *model = sdfModelRootNew();
    sdfModelNodeDefList_t *list;
    s32 count;
    sdfModelNodeDef_t *item;
    sdfModelNode_t *node;
    sdfModelNode_t *parent;

    model->texList = aTexList;
    model->matList = sdfMatListMake(aTexList, aModelData->matDef_New);
    model->extraData = aModelData->extraData;
    sdfModelMakeMorphList(model, aModelData->morphCount);
    list = aModelData->modelDef;
    count = list->count;
    list++;
    sdfModelAllocNodeList(model, count);
    item = (sdfModelNodeDef_t *)list;
    if (count != itemIndex) {
        do {
            node = sdfModelNodeNew();
            parent = NULL;
            if (item->parentIndex >= 0) {
                parent = ((sdfModelNode_t **)model->nodeList->items)[item->parentIndex];
            }
            sdfModelNodeAdd(model, parent, node);
            itemIndex++;
            node->id = item->id;
            item++;
        } while (itemIndex != count);
    }
    return model;
}
