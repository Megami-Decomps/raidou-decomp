#include "shared/basic/dds3Basic/decls.h"

void dds3OnPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag)
{
    pPoseSet->flag |= flag;
}
