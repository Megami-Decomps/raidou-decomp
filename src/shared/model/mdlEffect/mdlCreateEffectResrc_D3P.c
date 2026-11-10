#include "shared/model/mdlEffect/decls.h"

/* Append a newly created effect to the next part-list slot. */
void mdlCreateEffectResrc_D3P(sdfItemArray_t *partList, void *descriptor) {
    mdlEffectResrcItem_t *partEntry = &((mdlEffectResrcItem_t *)partList->items)[partList->count];

    partEntry->type = MDL_PART_EFFECT;
    partEntry->count = 0;
    partEntry->handle = dds3EffectLoad(descriptor);
    partList->count += 1;
}
