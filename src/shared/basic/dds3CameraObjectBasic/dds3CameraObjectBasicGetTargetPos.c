#include "shared/basic/dds3CameraObjectBasic/decls.h"

/* vu0 routine: return the cached world-space target in vf10. */
void dds3CameraObjectBasicGetTargetPos(dds3Basic_t *pBasic)
{
    sdfVector4_t *target = &((dds3CameraObjectBasicWorkData_t *)pBasic->workdata)->targetPos;

    SDF_VU0_LOADVECTOR_BARRIER(target);
}
