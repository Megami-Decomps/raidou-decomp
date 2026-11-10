#include "shared/effect/effSpline/decls.h"

void dds3AllocSplinePoints(dds3Spline_t *primitive, s32 flattenEqualComponents) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(primitive->count * 12);
    f32 *tangentData = (f32 *)sdfMemLock(handle);

    dds3BuildSplinePoints(tangentData, primitive->points, primitive->count);
    if (flattenEqualComponents == 0) {
        dds3BuildSplineCoefs(primitive, tangentData);
    } else {
        dds3BuildSplineCoefsSkipDup(primitive, tangentData);
    }
    sdfMemFreeDirect(handle);
}
