#include "shared/sdf/sdfMotion/decls.h"

void sdfMotionNodeBaseInit(sdfMotionNode_t *aNode, sdfMotionCtrlr_t *aCtrlr, sdfMotionNodeVTBL_t *aVTBL)
{
    aNode->vtbl = aVTBL;
    aNode->owner = aCtrlr;
}
