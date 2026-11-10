#include "shared/basic/dds3Basic/decls.h"

void dds3AddRotPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *rot)
{
    dds3PoseSet_t *pose = pBasic->poseset;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_LOADVECTOR(&pose->rot);
    SDF_VU0_LOADVECTOR_B(rot);
    Mult();
    SDF_VU0_STOREVECTOR(&pose->rot);
}
