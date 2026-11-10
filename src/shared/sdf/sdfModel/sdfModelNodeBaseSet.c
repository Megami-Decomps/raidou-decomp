#include "shared/sdf/sdfModel/decls.h"

/* vu0 routine: native SDK vector/matrix operations around model state. */
void sdfModelNodeBaseSet(sdfModelNode_t *aNode, sdfModelNodeDef_t *aDef)
{
    aNode->shape = aDef;
    aNode->id = aDef->id;
    FromEuler(aDef->rotate.x, aDef->rotate.y, aDef->rotate.z);
    SDF_VU0_STOREVECTOR(&aNode->rotate);
    SDF_VU0_COPYVECTOR(&aNode->pos, &aDef->pos);
    SDF_VU0_COPYVECTOR(&aNode->scale, &aDef->scale);
    aNode->pos.w = 1.0f;
    sdfModelMakeLocalTm(aNode);
    aNode->bounds = aDef->bounds;
}
