#include "shared/basic/dds3Basic/decls.h"

s32 dds3CheckPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag)
{
    return (pPoseSet->flag & flag) != 0;
}
