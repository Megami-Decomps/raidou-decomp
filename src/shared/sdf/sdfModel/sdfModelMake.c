#include "shared/sdf/sdfModel/decls.h"

sdfModelRootNode_t *sdfModelMake(void *aTexList, sdfModelDef_t *aModelData)
{
    s32 i = 0;
    sdfModelRootNode_t *root = sdfModelMakeRoot(aTexList, aModelData);
    u32 *def = aModelData->modelDef;
    s32 count = def[0];
    sdfModelNodeDef_t *nodeDef = (sdfModelNodeDef_t *)(def + 4);

    if (count != i) {
        do {
            sdfModelNodeSet(root->nodeList->items[i], nodeDef);
            nodeDef++;
            i++;
        } while (i != count);
    }
    return root;
}
