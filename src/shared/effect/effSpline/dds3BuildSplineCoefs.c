#include "shared/effect/effSpline/decls.h"

void dds3BuildSplineCoefs(dds3Spline_t *primitive, f32 *tangents) {
    f32 *cubic = primitive->coefs;
    f32 *quadratic = primitive->coefB;
    f32 *linear = primitive->coefC;
    f32 *points = primitive->points;
    s32 count = primitive->count;
    s32 segment;
    for (segment = 0; segment < count - 1; segment++) {
        s32 component;
        for (component = 0; component < 3; component++) {
            f32 endValue = points[segment * 3 + component + 3];
            f32 startValue = points[segment * 3 + component];
            f32 startTangent = tangents[component * count + segment];
            f32 endTangent = tangents[component * count + segment + 1];
            cubic[segment * 3 + component] = 2.0f * (startValue - endValue) +
                (startTangent + endTangent);
            quadratic[segment * 3 + component] = (endValue - startValue) * 3.0f -
                (2.0f * startTangent + endTangent);
            linear[segment * 3 + component] = startTangent;
        }
    }
}
