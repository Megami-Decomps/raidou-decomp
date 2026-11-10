#include "shared/model/mdlEffect/decls.h"

/* Destroy each part according to its stored kind, then release the owning request. */
void mdlEffectResrcListFree(sdfItemArray_t *efcResList) {
    s32 partIndex;

    if (efcResList != NULL) {
        for (partIndex = 0; partIndex < efcResList->count; partIndex++) {
            mdlEffectResrcItem_t *partEntry = &((mdlEffectResrcItem_t *)efcResList->items)[partIndex];

            switch (partEntry->type) {
            case MDL_PART_BILLBOARD:
                dds3BillboardDelete(partEntry->handle);
                break;
            case MDL_PART_EFFECT:
                dds3EffectDelete(partEntry->handle);
                break;
            case MDL_PART_OBJECT:
                mdlDeleteEffectResrc_IPU(partEntry->handle);
                break;
            }
        }
        sdfItemArrayFree(efcResList);
    }
}
