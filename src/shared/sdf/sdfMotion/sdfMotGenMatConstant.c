#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotGenMatConstant(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatFloat_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatFloat_t));
    sdfMotGenMatInit(&node->matBase, aCtrlr, &sdfMotMatConstantTable, aNodeId);
    return &node->matBase.nodeBase;
}
