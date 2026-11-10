#include "common.h"

#include "shared/sdf/sdfQuat/Mult.c"

#include "shared/sdf/sdfQuat/Inverse.c"

#include "shared/sdf/sdfQuat/Normalize.c"

INCLUDE_ASM(const s32, "sdf/sdfQuat", func_003E4A08);

#include "shared/sdf/sdfQuat/SDF_QUAT_INNERPRODUCT.c"

#include "shared/sdf/sdfQuat/FromRotation.c"

#include "shared/sdf/sdfQuat/FromRotation_B.c"

#include "shared/sdf/sdfQuat/ToMatrix.c"

#include "shared/sdf/sdfQuat/FromEulerXYZ.c"

#include "shared/sdf/sdfQuat/FromEuler.c"

INCLUDE_ASM(const s32, "sdf/sdfQuat", LookAt);

INCLUDE_ASM(const s32, "sdf/sdfQuat", Slerp);

#include "shared/sdf/sdfQuat/Lerp.c"

#include "shared/sdf/sdfQuat/EulerAngleAxisY.c"
