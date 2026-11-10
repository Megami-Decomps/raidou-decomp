#include "shared/sdf/sdfMotion/decls.h"

void sdfMotGenModelInit(sdfMotNodeModelBase_t *aBase, sdfMotionCtrlr_t *aCtrlr, sdfMotionNodeVTBL_t *aVTBL, u32 aNodeId)
{
    sdfMotionNodeBaseInit(&aBase->nodeBase, aCtrlr, aVTBL);
    aBase->modelNode = sdfModelSearchNode(aCtrlr->rootNode, aNodeId);
}
