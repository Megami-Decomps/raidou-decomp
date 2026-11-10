#include "shared/sdf/sdfModel/decls.h"

void sdfModelMakeMorphList(sdfModelRootNode_t *root, s32 morphCount)
{
    sdfModelMorphList_t *list;
    sdfModelMorph_t *items;
    s32 i;
    s32 j;

    if (morphCount > 0) {
        list = (sdfModelMorphList_t *)sdfItemArrayNew(morphCount, 0x10, 1);
        root->morphList = list;
        items = list->items;
        for (i = 0; i != morphCount; i++) {
            for (j = 0; j != 2; j++) {
                items[i].target[j].targetId = 0;
                items[i].target[j].blend = 0.0f;
            }
        }
        list->count = morphCount;
    }
}
