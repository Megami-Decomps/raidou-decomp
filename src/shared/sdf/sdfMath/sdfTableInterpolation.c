#include "shared/sdf/sdfMath/decls.h"

/* Binary-search samples, using zero as the implicit lower endpoint.
 * The step is integer 1/sampleCount converted to f32, not a float reciprocal.
 * The caller must provide valid table bounds for the final sample index.
 */
f32 sdfTableInterpolation(f32 aValue, const f32 *aTable, s32 aTableSize) {
    f32 positionStep = 1 / aTableSize;
    s32 lowerIndex = 0;
    s32 upperIndex = aTableSize;
    s32 sampleIndex;
    f32 lowerValue;
    f32 upperValue;
    do {
        sampleIndex = lowerIndex + upperIndex;
        sampleIndex >>= 1;
        upperValue = aTable[sampleIndex];
        if (aValue < upperValue) {
            upperIndex = sampleIndex;
        } else {
            sampleIndex++;
            lowerIndex = sampleIndex;
        }
    } while (lowerIndex < upperIndex);
    upperValue = aTable[sampleIndex];
    lowerValue = 0.0f;
    if (sampleIndex != 0) {
        lowerValue = aTable[sampleIndex - 1];
    }
    return sampleIndex * positionStep + (aValue - lowerValue) * positionStep / (upperValue - lowerValue);
}
