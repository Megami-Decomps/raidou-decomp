#include "shared/sdf/sdfQuat/decls.h"

/* vu0 routine: shared quaternion registers are the input/output interface. */
void Mult(void)
{
    VU0_SEQ_MULT();
}
