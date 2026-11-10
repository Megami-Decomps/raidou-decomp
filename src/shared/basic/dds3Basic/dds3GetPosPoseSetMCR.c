#include "shared/basic/dds3Basic/decls.h"

void dds3GetPosPoseSetMCR(dds3Basic_t *basic, const sdfVector4_t *offset)
{
    dds3PoseSet_t *pose = basic->poseset;
    sdfVector4_t *position = &pose->pos;

    pose->flag = (pose->flag | POSESET_FLAG_UPDATEPOSE) & ~POSESET_FLAG_UPDATETM;
    SDF_VU0_LOADVECTOR(position);
    SDF_VU0_LOADVECTOR_B_BARRIER(offset);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&pose->pos);
}
