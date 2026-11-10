#include "shared/sdf/sdfModel/decls.h"

/* vu0 routine: native SDK vector/matrix operations around model state. */
sdfModelNode_t *sdfModelNodeNew(void) {
    sdfModelNode_t *node = sdfChipAllocZeroDirect(sizeof(sdfModelNode_t));

    node->color = 0x80808080;
    node->drawAttr = -1;
    node->prev = node;
    node->next = node;
    SDF_VU0_STOREIDENTITYVECTOR(&node->rotate);
    SDF_VU0_STOREIDENTITYVECTOR(&node->pos);
    SDF_VU0_SETSCALE1VECTOR();
    SDF_VU0_STOREVECTOR(&node->scale);
    SDF_VU0_STOREIDENTITY(&node->localNoScaledTm);
    return node;
}
