#include "shared/model/mdlEffect/decls.h"

/* Prepend a cleared, typed resource item to the owner's list. */
dds3ModelEffectCtrlr_t *mdlCreateEffectCtrlr(mdlObj_t *aHandle, s32 aType, s32 aPlayGroup) {
    dds3ModelEffectCtrlr_t *item = sdfChipAllocZeroDirect(sizeof(dds3ModelEffectCtrlr_t));
    dds3ModelEffectCtrlr_t *previousHead = aHandle->resourceItems;
    item->type = aType;
    item->next = previousHead;
    item->playGroup = aPlayGroup;
    aHandle->resourceItems = item;
    return item;
}
