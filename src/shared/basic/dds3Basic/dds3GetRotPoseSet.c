#include "shared/basic/dds3Basic/decls.h"

void dds3GetRotPoseSet(dds3Basic_t *pBasic)
{
    sdfVector4_t *rotation = &pBasic->poseset->rot;

    SDF_VU0_LOADVECTOR_BARRIER(rotation);
    Normalize();
}
