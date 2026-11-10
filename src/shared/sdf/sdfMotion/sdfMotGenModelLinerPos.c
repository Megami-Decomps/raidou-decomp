#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotGenModelLinerPos(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeModelVec_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeModelVec_t));
    sdfMotGenModelInit(&node->modelBase, aCtrlr, &sdfMotModelLinerPosTable, aNodeId);
    return &node->modelBase.nodeBase;
}
