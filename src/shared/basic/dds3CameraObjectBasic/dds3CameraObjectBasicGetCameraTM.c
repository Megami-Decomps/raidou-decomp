#include "shared/basic/dds3CameraObjectBasic/decls.h"

/* The recovered name is misleading: retail stores a target, not a matrix. */
void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *basic, const sdfVector4_t *target)
{
    SDF_VU0_COPYVECTOR(&((dds3CameraObjectBasicWorkData_t *)basic->workdata)->targetPos, target);
}
