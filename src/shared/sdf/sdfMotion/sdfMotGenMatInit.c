#include "shared/sdf/sdfMotion/decls.h"

void sdfMotGenMatInit(sdfMotNodeMatBase_t *aBase, sdfMotionCtrlr_t *aCtrlr, sdfMotionNodeVTBL_t *aVTBL, u32 aNodeId)
{
    sdfMotionNodeBaseInit(&aBase->nodeBase, aCtrlr, aVTBL);
    aBase->mat = aCtrlr->rootNode->matList->items[aNodeId];
}
