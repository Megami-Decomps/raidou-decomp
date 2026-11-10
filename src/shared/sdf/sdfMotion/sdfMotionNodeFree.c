#include "shared/sdf/sdfMotion/decls.h"

void sdfMotionNodeFree(sdfMotionNode_t *aNode)
{
    aNode->vtbl->destroy(aNode);
}
