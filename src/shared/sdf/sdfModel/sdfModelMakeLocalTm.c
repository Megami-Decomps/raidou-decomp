#include "shared/sdf/sdfModel/decls.h"

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelMakeLocalTm(sdfModelNode_t *aNode) {
    SDF_VU0_LOADVECTOR(&aNode->rotate);
    ToMatrix();
    SDF_VU0_LOAD_ROW3(&aNode->pos);
    SDF_VU0_STOREMATRIX(&aNode->localNoScaledTm);
}
