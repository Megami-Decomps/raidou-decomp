#include "shared/model/mdlEffect/decls.h"

/* Resolve a 16-byte part slot; retail checks only the upper index bound. */
mdlEffectResrcItem_t *mdlGetEffectResrc(mdlObj_t *handle, s32 effectIndex) {
    sdfItemArray_t *slotTable;

    slotTable = handle->sub->effectResrcList;
    if (slotTable == NULL) {
        return NULL;
    }
    if (effectIndex >= slotTable->count) {
        return NULL;
    }
    return (mdlEffectResrcItem_t *)((u8 *)slotTable->items + effectIndex * MDL_PART_SLOT_BYTES);
}
