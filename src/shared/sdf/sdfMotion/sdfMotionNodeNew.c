#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotionNodeNew(sdfMotionCtrlr_t *aCtrlr, u32 aTypeId, u32 aNodeId)
{
    return sdfMotGenNodeTable[aTypeId >> 16](aCtrlr, aTypeId, aNodeId);
}
