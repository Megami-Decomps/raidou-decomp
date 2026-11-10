#include "shared/basic/dds3Basic/decls.h"

void dds3SetRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot)
{
    dds3PoseSet_t *pose = pBasic->poseset;
    sdfVector4_t *dst = &pose->rot;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_COPYVECTOR(dst, rot);
}
