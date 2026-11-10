#include "shared/basic/dds3Basic/decls.h"

/* vu0 routine: reset translation/rotation and set unit scale. */
void dds3BackupPoseSet(dds3PoseSet_t *pPoseSet)
{
    sdfVector4_t *position = &pPoseSet->pos;
    sdfVector4_t *rotation;
    sdfVector4_t *scale;

    SDF_VU0_STOREIDENTITYVECTOR(position);
    rotation = &pPoseSet->rot;
    SDF_VU0_STOREIDENTITYVECTOR(rotation);
    SDF_VU0_SETSCALE1VECTOR();
    scale = &pPoseSet->scl;
    SDF_VU0_STOREVECTOR(scale);
}
