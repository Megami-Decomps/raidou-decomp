#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotGenMatDiffuse(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatColor_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatColor_t));
    sdfMotGenMatInit(&node->base, aCtrlr, &sdfMotMatDiffuseTable, aNodeId);
    return &node->base.nodeBase;
}
