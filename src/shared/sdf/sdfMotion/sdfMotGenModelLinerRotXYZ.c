#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotGenModelLinerRotXYZ(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeModelVec_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeModelVec_t));
    sdfMotGenModelInit(&node->modelBase, aCtrlr, &sdfMotModelLinerRotXYZTable, aNodeId);
    return &node->modelBase.nodeBase;
}
