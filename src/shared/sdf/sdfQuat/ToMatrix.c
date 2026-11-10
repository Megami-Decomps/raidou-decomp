#include "shared/sdf/sdfQuat/decls.h"

/* vu0 routine: shared quaternion registers are the input/output interface. */
void ToMatrix(void)
{
    VU0_SEQ_TO_MATRIX();
}
