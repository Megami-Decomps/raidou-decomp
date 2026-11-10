#include "shared/basic/dds3EventObjectBasic/decls.h"

s32 EventObjectBasicMainProcuder(dds3Basic_t *pBasic)
{
    dds3PoseSet_t *pose;

    pose = pBasic->poseset;
    if (dds3CheckPoseSetFlag(pose, POSESET_FLAG_UPDATEPOSE) == 1) {
        dds3OffPoseSetFlag(pose, POSESET_FLAG_UPDATEPOSE);
        dds3SetRadiusPoseSet(pose);
    }
    return 1;
}
