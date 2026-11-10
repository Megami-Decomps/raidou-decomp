#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotGenMatMultiUvw(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    sdfMotNodeMatUvw_t *node;

    node = sdfChipAllocDirect(sizeof(sdfMotNodeMatUvw_t));
    sdfMotGenMatInit(&node->base, aCtrlr, &sdfMotMatMultiUvwTable, aNodeId);
    return &node->base.nodeBase;
}
