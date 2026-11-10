#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotGenMorphNode(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    return sdfMotGenMorphTable[aTypeId & 0xFFFF](aCtrlr, aTypeId, aNodeId);
}
