#include "shared/basic/dds3Basic/decls.h"

s32 dds3InitPoseSet(dds3Basic_t *basic)
{
    dds3PoseSet_t *pose;

    if (basic == NULL) {
        return 0;
    }
    if (basic->poseset != NULL) {
        return 0;
    }
    pose = sdfChipAllocDirect(sizeof(dds3PoseSet_t));
    if (pose == NULL) {
        return 0;
    }
    pose->flag = POSESET_FLAG_UPDATEPOSE;
    dds3BackupPoseSet(pose);
    SDF_VU0_STOREIDENTITYVECTOR(&pose->plane);
    basic->poseset = pose;
    pose->frameFall = 0;
    pose->r = 0.0f;
    return 1;
}
