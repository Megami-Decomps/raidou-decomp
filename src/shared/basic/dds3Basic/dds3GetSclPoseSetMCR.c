#include "shared/basic/dds3Basic/decls.h"

void dds3GetSclPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *scale)
{
    dds3PoseSet_t *pose = basic->poseset;
    sdfVector4_t *src = &pose->scl;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_LOADVECTOR(src);
    SDF_VU0_LOADVECTOR_B_BARRIER(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&pose->scl);
}
