#include "shared/basic/dds3Basic/decls.h"

void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag)
{
    pPoseSet->flag &= ~flag;
}
