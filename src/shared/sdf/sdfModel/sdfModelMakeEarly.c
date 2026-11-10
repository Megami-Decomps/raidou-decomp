#include "shared/sdf/sdfModel/decls.h"

sdfModelRootNode_t *sdfModelMakeEarly(void *aTexList, sdfModelDef_t *aModelData)
{
    s32 i = 0;
    sdfModelRootNode_t *root = sdfModelMakeRoot(aTexList, aModelData);
    u32 *def;
    s32 count;
    sdfModelNodeDef_t *nodeDef;

    root->flags |= SDF_MODEL_FLAG_EARLY_SETUP;
    def = aModelData->modelDef;
    count = def[0];
    nodeDef = (sdfModelNodeDef_t *)(def + 4);
    if (count != i) {
        do {
            sdfModelNodeBaseSet(root->nodeList->items[i], nodeDef);
            nodeDef++;
            i++;
        } while (i != count);
    }
    return root;
}
