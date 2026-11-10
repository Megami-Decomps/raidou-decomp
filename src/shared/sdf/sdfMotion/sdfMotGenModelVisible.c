#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotGenModelVisible(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeModelFlag_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeModelFlag_t));
    sdfMotGenModelInit(&node->modelBase, aCtrlr, &sdfMotModelVisibleTable, aNodeId);
    return &node->modelBase.nodeBase;
}
