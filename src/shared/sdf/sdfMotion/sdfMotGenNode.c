#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotGenNode(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    return sdfMotGenNodeDef[aTypeId & 0xFFFF](aCtrlr, aTypeId, aNodeId);
}
