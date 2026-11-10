#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotGenModelLinerScale(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeModelVec_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeModelVec_t));
    sdfMotGenModelInit(&node->modelBase, aCtrlr, &sdfMotModelLinerScaleTable, aNodeId);
    return &node->modelBase.nodeBase;
}
