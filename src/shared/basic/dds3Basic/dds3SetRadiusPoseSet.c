#include "shared/basic/dds3Basic/decls.h"

void dds3SetRadiusPoseSet(dds3PoseSet_t *pose)
{
    SDF_VU0_COPYVECTOR(&pose->workscl, &pose->scl);
    SDF_VU0_COPYVECTOR(&pose->workrot, &pose->rot);
    SDF_VU0_COPYVECTOR(&pose->workpos, &pose->pos);
}
