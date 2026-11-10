#include "shared/interface/itfMisc/decls.h"

/* Multiply XYZ, cap only the positive product at 255 << 16, then shift by 16.
 * Negative products are not clamped; W comes from the argument. */
void itfMiscSetRatioRGB(itfColorVector_t *output, s32 scaleX, s32 scaleY, s32 scaleZ,
                   s32 w, const itfColorVector_t *input, s32 count) {
    while (count > 0) {
        s32 x = scaleX * input->x;
        s32 y = scaleY * input->y;
        s32 z = scaleZ * input->z;

        output->w = w;
        if (x > ITF_FIXED_COMPONENT_MAX) x = ITF_FIXED_COMPONENT_MAX;
        if (y > ITF_FIXED_COMPONENT_MAX) y = ITF_FIXED_COMPONENT_MAX;
        if (z > ITF_FIXED_COMPONENT_MAX) z = ITF_FIXED_COMPONENT_MAX;
        output->x = x >> ITF_FIXED_FRACTION_BITS;
        output->y = y >> ITF_FIXED_FRACTION_BITS;
        output->z = z >> ITF_FIXED_FRACTION_BITS;
        output++;
        input++;
        count--;
    }
}
