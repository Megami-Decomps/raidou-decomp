#include "shared/basic/dds3Basic/decls.h"

/* Load the cached matrix, rebuilding it only when a pose setter invalidated it. */
void dds3GetTMPoseSet(dds3Basic_t *pBasic)
{
    dds3PoseSet_t *pose = pBasic->poseset;
    u32 flags = pose->flag;

    if (flags & POSESET_FLAG_UPDATETM) {
        SDF_VU0_LOADMATRIX(&pose->workTM);
    } else {
        pose->flag = flags | POSESET_FLAG_UPDATETM;
        dds3MakeMatrix(&pose->scl, &pose->rot, &pose->pos);
        SDF_VU0_STOREMATRIX(&pose->workTM);
    }
}
