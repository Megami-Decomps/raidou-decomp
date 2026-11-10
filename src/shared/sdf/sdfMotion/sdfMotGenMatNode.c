#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotGenMatNode(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    return sdfMotGenMatTable[aTypeId & 0xFFFF](aCtrlr, aTypeId, aNodeId);
}
