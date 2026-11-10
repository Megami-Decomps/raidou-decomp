#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotGenMatMultiDiffuse(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatColor_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatColor_t));
    sdfMotGenMatInit(&node->base, aCtrlr, &sdfMotMatMultiDiffuseTable, aNodeId);
    return &node->base.nodeBase;
}
