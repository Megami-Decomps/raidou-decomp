#include "shared/sdf/sdfMotion/decls.h"

void sdfMotInterpolateKeyUvwSub(f32 *aDst, f32 *aSrc0, f32 *aSrc1, f32 aRate)
{
    s32 i;

    for (i = 0; i != 5; i++) {
        aDst[i] = aSrc0[i] * (1.0f - aRate) + aSrc1[i] * aRate;
    }
}
