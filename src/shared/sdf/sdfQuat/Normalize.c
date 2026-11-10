#include "shared/sdf/sdfQuat/decls.h"

/* vu0 routine: normalize all four components of vf10. The IDB called this
 * SDF_QUAT_INNERPRODUCT, which belongs to the dot product below. */
void Normalize(void)
{
    VU0_SEQ_NORMALIZE();
}
