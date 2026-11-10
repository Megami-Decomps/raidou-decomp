#include "shared/model/mdlEffect/decls.h"

/* Append a newly created billboard to the next part-list slot. */
void mdlCreateEffectResrc_P2A(sdfItemArray_t *partList, void *descriptor) {
    mdlEffectResrcItem_t *partEntry = &((mdlEffectResrcItem_t *)partList->items)[partList->count];

    partEntry->count = 0;
    partEntry->type = MDL_PART_BILLBOARD;
    partEntry->handle = dds3BillboardCreate(1, EE_ADDR(descriptor));
    partList->count += 1;
}
